#include "Engine/AI/FiniteStateMachines/FiniteStateMachine.hpp"
#include <sstream>
#include "GenericFactory.hpp"
#include "Engine/AI/Blackboards/Comparator.hpp"

bool AI::TransitionData::CanTransition(const StateMachineContext& context)const
{
   for (const auto& comparator : comparators)
    {
        if (!comparator->Evaluate(*context.blackboard))
        {
            return false;
        }
    }
    return true;
}

void AI::FiniteStateMachine::Execute(StateMachineContext& context)
{
    if (m_states.empty()) return;

    if (!context.currentState.has_value())
    {
        context.currentState = m_defaultState.value();
        m_states[context.currentState.value()]->Initialize(context);
    }

    const auto currentStateIndex = context.currentState.value();
    m_states[currentStateIndex]->Update(context);

    for (const auto& transitionData : m_transitions[currentStateIndex])
    {
        if (!transitionData.CanTransition(context))
        {
            continue;
        }

        auto typeIndex = transitionData.StateToGoTo();
        const std::unique_ptr<State>& endedState = m_states.at(currentStateIndex);
        endedState->End(context);

        context.currentState = typeIndex;
        const std::unique_ptr<State>& stateToInit = m_states.at(context.currentState.value());
        stateToInit->Initialize(context);
    }
}

void AI::FiniteStateMachine::SetCurrentState(size_t stateToSet, StateMachineContext& context) const
{
    const auto currentStateIndex = context.currentState.value();
    const std::unique_ptr<State>& endedState = m_states.at(currentStateIndex);
    endedState->End(context);

    context.currentState = stateToSet;
    const std::unique_ptr<State>& stateToInit = m_states.at(context.currentState.value());
    stateToInit->Initialize(context);
}

std::stringstream AI::FiniteStateMachine::Serialize() const
{
    nlohmann::json json;

    json["version"] = "1.0";
    json["behavior-structure-type"] = "FSM";

    nlohmann::json states = nlohmann::json::array();
    nlohmann::json transitions = nlohmann::json::array();

    if (!m_editorData.empty())
    {
        json["editor-data"] = m_editorData;
    }

    int index = 0;
    for (auto& state : m_states)
    {
        nlohmann::json stateObject = nlohmann::json::object();
        auto name = std::string(typeid(*state).name());

        std::size_t i = name.find("class");
        if (i != std::string::npos)
        {
            name.erase(i, 5);
        }

        i = name.find("AI::");
        if (i != std::string::npos)
        {
            name.erase(i, 4);
        }

        stateObject["default"] = m_defaultState.value() == index;

        stateObject["name"] = name;
        stateObject["editor-variables"] = nlohmann::json();

        for (auto& variable : state->editorVariables)
        {
            nlohmann::json jsonVariable;
            jsonVariable["name"] = variable.first;
            jsonVariable["value"] = variable.second->ToString();
            stateObject["editor-variables"].push_back(jsonVariable);
        }

        states.push_back(stateObject);
        index++;
    }

    for (auto& pair : m_transitions)
    {
        nlohmann::json transitionData = nlohmann::json::object();
        transitionData["from"] = pair.first;
        std::string fromIndex = std::to_string(pair.first);

        for (const auto& data : pair.second)
        {
            nlohmann::json transition = nlohmann::json::object();
            transition["to"] = data.StateToGoTo();
            nlohmann::json comparators = nlohmann::json::array();

            for (const auto& comparator : data.comparators)
            {
                comparators.emplace_back(comparator->ToString());
            }

            transition["comparators"] = comparators;
            transitionData["transitions"].push_back(transition);
        }
        transitions.push_back(transitionData);
    }
    json["states"] = states;
    json["transition-data"] = transitions;
    return std::stringstream(json.dump());
}


bool AI::FiniteStateMachine::DeserializeStates(nlohmann::json& json,std::unique_ptr<FiniteStateMachine>& stateMachine)
{
    for (int i = 0; i < json.size(); i++)
    {
        std::string name = json[i]["name"].get<std::string>();
        erase_if(name, isspace);

        bool isDefault = false;

        isDefault = json[i]["default"].get<bool>();
        auto state = GenericFactory<State>::Instance().CreateProduct(name);

        if (state == nullptr) continue;

        if (isDefault)
        {
            stateMachine->m_defaultState = i;
        }

        for (auto& x : json[i]["editor-variables"])
        {
            if (!state->editorVariables.contains(x["name"])) continue;
            state->editorVariables[x["name"]]->Deserialize(x["value"]);
        }

        stateMachine->m_states.push_back (std::move(state));
    }

    if (stateMachine->m_states.empty()) return false;
    if (!stateMachine->m_defaultState.has_value())
    {
        stateMachine->m_defaultState = 0;
    }
    return true;
}

void AI::FiniteStateMachine::DeserializeTransitions(nlohmann::json json, AI::TransitionData& tempData)
{
    for (int i = 0; i < json["comparators"].size();i++)
    {
        std::stringstream stream = std::stringstream(json["comparators"][i].get<std::string>());
        std::string comparisonKey;
        std::string typeName;
        int comparisonType;
        stream >> comparisonKey;
        stream >> typeName;
        stream >> comparisonType;

        if (typeName == "float")
        {
            float value;
            stream >> value;
            tempData.comparators.push_back(std::make_unique<Comparator<float>>(comparisonKey, static_cast<ComparisonType>(comparisonType), value));
        }
        if (typeName == "double")
        {
            double value;
            stream >> value;
            tempData.comparators.push_back(std::make_unique<Comparator<double>>(comparisonKey, static_cast<ComparisonType>(comparisonType), value));
        }
        if (typeName == "bool")
        {
            bool value;
            stream >> value;
            tempData.comparators.push_back(std::make_unique<Comparator<bool>>(comparisonKey, static_cast<ComparisonType>(comparisonType), value));
        }
        if (typeName == "int")
        {
            int value;
            stream >> value;
            tempData.comparators.push_back(std::make_unique<Comparator<int>>(comparisonKey, static_cast<ComparisonType>(comparisonType), value));
        }
        if (typeName.find("string") != std::string::npos)
        {
            std::string value;
            stream >> value;
            tempData.comparators.push_back(std::make_unique<Comparator<std::string>>(comparisonKey, static_cast<ComparisonType>(comparisonType), value));
        }
    }
}

void AI::FiniteStateMachine::DeserializeTransitionData(nlohmann::json json,std::unique_ptr<FiniteStateMachine>& stateMachine)
{
    for (int i = 0; i < json.size(); i++)
    {
        size_t from = json[i]["from"].get<size_t>();
        for (int j = 0; j < json[i]["transitions"].size(); j++)
        {
            size_t stateTo = json[i]["transitions"][j]["to"];
            auto tempData = TransitionData(stateTo);

            DeserializeTransitions(json[i]["transitions"][j], tempData);

            auto transitionData = std::ranges::find_if(stateMachine->m_transitions[from], [stateTo](const TransitionData& data){ return data.StateToGoTo() == stateTo; });

            if (transitionData == stateMachine->m_transitions[from].end())
            {
                stateMachine->m_transitions[from].push_back(std::move(tempData));
            }
            else
            {
                transitionData->comparators.push_back(std::move(tempData.comparators[0]));
            }
        }
    }
}

std::unique_ptr<AI::FiniteStateMachine> AI::FiniteStateMachine::DeserializeFromStringStream(std::stringstream& stream)
{
    int transitionDataAmount = 0;
    stream >> transitionDataAmount;
    nlohmann::json json = nlohmann::json::parse(stream.str());

    std::unique_ptr<FiniteStateMachine> temp = std::make_unique<FiniteStateMachine>();
    temp->m_editorData = json["editor-data"];

    if (!DeserializeStates(json["states"], temp)) return std::move(temp);
    DeserializeTransitionData(json["transition-data"], temp);
    return std::move(temp);
}


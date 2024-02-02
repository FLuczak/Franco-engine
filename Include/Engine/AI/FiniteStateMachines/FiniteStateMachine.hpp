#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include "Engine/AI/Blackboards/Blackboard.hpp"
#include "Engine/AI/Blackboards/Comparator.hpp"
#include "Engine/AI/Utils/ExecutionContext.hpp"
#include "Engine/EditorVariables.h"

namespace AI
{
class State;

using stateTypeIndex = std::type_index;

struct StateMachineContext : AIExecutionContext
{
    float deltaTime = 0.0f;
    std::unique_ptr<Blackboard> blackboard = std::make_unique<Blackboard>();
    size_t GetCurrentState() const { return currentState.value(); }
private:
    std::optional<size_t> currentState;
    friend class FiniteStateMachine;
};

struct TransitionData
{
    TransitionData(size_t stateToGoTo) : m_stateToGoTo(stateToGoTo) {}
    TransitionData(std::vector<std::unique_ptr<IComparator>>& comparatorsToSet, size_t stateToGoTo): comparators(std::move(comparatorsToSet)), m_stateToGoTo(stateToGoTo)
    {
    }

    /**
     * \brief Check all comparators in this transitionData and return whether or not
     * the transition can be made
     * \param context - a context to check if the transition can check against
     * \return - bool whether or not the transition can be made
     */
    bool CanTransition(const StateMachineContext& context) const;

    const size_t StateToGoTo() const { return m_stateToGoTo; }
    std::vector<std::unique_ptr<IComparator>> comparators;
private:
    size_t m_stateToGoTo;
};


/**
 * \brief Base class for the state machine state
 */
class State
{
public:
    virtual ~State() = default;
    /**
     * \brief Update- This function is called until the state does not stop
     * \param context - context on which the Update is going to be based on
     */
    virtual void Update(StateMachineContext& context) {}
    /**
     * \brief A set up function for a state- it gets called once when the state
     * is set to a current state in the state machine
     * \param context - context on which the state is based on
     */
    virtual void Initialize(StateMachineContext& context) {}
    /**
     * \brief A clean up function for a state- it gets called once when the state ends
     * \param context - context the end execution should be based on
     */
    virtual void End(StateMachineContext& context) {}

    /**
     * \brief Register a variable for serialization
     * \param name - the name of the variable
     * \param variable - a reference to the variable to be serialized
     */
    void RegisterEditorVariable(const std::string& name, EditorVariable* variable)
    {
        editorVariables[name] = variable;
    }

    std::unordered_map<std::string, EditorVariable*> editorVariables{};
};

/**
 * \brief A transition builder struct used to add more comparators to add a transition
 */
struct TransitionBuilder
{
    TransitionBuilder(TransitionData& data) : m_data(data) {}

    /**
     * \brief Add a comparator of type ComparatorType to the comparator
     * \tparam ComparatorType - Type of comparator
     * \param comparator - the comparator
     * \return 
     */
    template <typename ComparatorType>
    TransitionBuilder& AddComparator(const Comparator<ComparatorType> comparator)
    {
        auto temp = std::make_unique<Comparator<ComparatorType>>(comparator);
        m_data.comparators.push_back(std::move(temp));
        return *this;
    }

private:
    TransitionData& m_data;
};

class FiniteStateMachine
{
public:
    /**
     * \brief Add a state of a given type to the state machine. If there is no default state specified,
     * this state is going to be the default state
     * \tparam T - The type of the state
     * \param IsDefaultState - whether or not the state should be a default state of the state machine
     * \return - A state referecne added to the state machine.
     */
    template <typename T, typename... Args>
    size_t AddState(bool IsDefaultState = false, Args... args)
    {
        static_assert(std::is_base_of_v<State, T>);
        if (!m_defaultState.has_value() || IsDefaultState)
        {
            m_defaultState = {m_states.size()};
        }

        m_states.push_back(std::move(std::make_unique<T>(args...)));
        return m_states.size() - 1;
    }

    /**
     * \brief Add a transition between a state From to a state To with a given comparator of type ComparatorType.
     * IMPORTANT: if there's already a transition between, a comparator is added to the previously created transition.
     * If one of the provided states do not exist, the transition will not be created.
     * \tparam ComparatorType - The type of comparator in this transtion
     *\ param from - index of the node to transition from
     * \param to - index of the node to transition to
     * \param comparator - the comparator this transition should have added
     */
    template <typename ComparatorType>
    TransitionBuilder AddTransition(size_t from, size_t to,const Comparator<ComparatorType> comparator)
    {
        assert(from < m_states.size());
        assert(to < m_states.size());

        for (auto&& transitionData : m_transitions[from])
        {
            if (to == transitionData.StateToGoTo())
            {
                std::unique_ptr<Comparator<ComparatorType>> tempComparator =std::make_unique<Comparator<ComparatorType>>(comparator);
                transitionData.comparators.push_back(std::move(tempComparator));
                return TransitionBuilder(transitionData);
            }
        }

        TransitionData temp = TransitionData(to);

        std::unique_ptr<Comparator<ComparatorType>> tempComparator = std::make_unique<Comparator<ComparatorType>>(comparator);
        temp.comparators.push_back( std::move(tempComparator));
        
        auto& ref = m_transitions[from];
        ref.push_back(std::move(temp));
        return TransitionBuilder(ref.back());
    }

    /**
     * \brief Creates a transition from one state to another without a comparator
     * \param from - id of the state A
     * \param to - id of the state B to transition to
     * \return - A transitionbuilder to add more comparators
     */
    TransitionBuilder AddTransition(size_t from, size_t to)
    {
        assert(from < m_states.size());
        assert(to < m_states.size());

        for (auto& transitionData : m_transitions[from])
        {
            if (to == transitionData.StateToGoTo())
            {
                return TransitionBuilder(transitionData);
            }
        }

        TransitionData temp = TransitionData(to);
        auto& ref = m_transitions[from];
        ref.push_back(std::move(temp));
        return {ref.back()};
    }

    /**
     * \brief Removes the state of type T from the list of states, as well as all transitions to this state.
     * \param i - state index
     */
    void RemoveState(size_t i)
    {
        if (i >= m_states.size()) return;
        m_states.erase(m_states.begin() + i);
        m_transitions.erase(i);

        for (auto& element : m_transitions)
        {
             erase_if(element.second,[i](const TransitionData& data)
             {
                return data.StateToGoTo() == i;
             });
        }
    }

    /**
     * \brief Execute the finite state machine using a provided StateMachineContext
     * \param context - the given state machine context
     */
    void Execute(StateMachineContext& context);

    /**
     * \brief Given a state Id and a state machine execution context
     * end the current state and set it to 'stateToSet'
     * \param stateToSet - An id a state should be set to
     * \param context - StateMachineContext
     */
    void SetCurrentState(size_t stateToSet,StateMachineContext& context) const;

    /**
     * \brief Turn this state machine into a string stream
     * \return - a string stream containing information about this FSM states and transitions between them
     */
    std::stringstream Serialize() const;
    
    /**
     * \brief - Given a stringstream, return a unique ptr to a finitestatemachine created by deserializing
     * them from the string stream.
     * \param stream - a string stream with states and transitions to them
     * \return - a unique ptr to a finite state machine
     */
    static std::unique_ptr<FiniteStateMachine> DeserializeFromStringStream(std::stringstream& stream);
private:
    static bool DeserializeStates(nlohmann::json& json, std::unique_ptr<FiniteStateMachine>& stateMachine);
    static void DeserializeTransitions(nlohmann::json json, AI::TransitionData& tempData);
    static void DeserializeTransitionData(nlohmann::json json, std::unique_ptr<FiniteStateMachine>& stateMachine);

    std::optional<size_t> m_defaultState = {};
    std::vector<std::unique_ptr<State>> m_states;
    std::unordered_map<size_t, std::vector<TransitionData>> m_transitions;
    friend class FiniteStateMachine;
    nlohmann::json m_editorData;
    friend class FiniteStateMachine;
};
}  // namespace AI


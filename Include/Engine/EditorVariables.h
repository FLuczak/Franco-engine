#pragma once
#include <sstream>
#include <string>
#include <typeindex>

#include "AI/BehaviorTrees/Behaviors.hpp"
#include "AI/FiniteStateMachines/FiniteStateMachine.hpp"
#include "magic_enum/magic_enum.hpp"
#include "Visit struct/visit_struct.hpp"
#include "Engine/Component.hpp"


template <typename U>
concept HasToString = requires(const U & type)
{
    std::to_string(type);
};

template <typename U>
concept HasIterators = requires(const U & type)
{
    type.begin();
    type.end();
};

template <typename U, typename  T>
concept HasPushBack = requires(U u, T t)
{
    { u.push_back(t) } -> std::same_as<void>;
    { u.push_back(std::move(t)) } -> std::same_as<void>;
};

template <typename U, typename  T>
concept HasInsert = requires(U u, T t)
{
    { u.insert(t) } -> std::same_as<void>;
    { u.insert(std::move(t)) } -> std::same_as<void>;
};

class EditorVariable
{
public:
    virtual ~EditorVariable() = default;
    virtual std::string ToString() const = 0;
    virtual std::type_index GetTypeInfo() const = 0;
    virtual void Deserialize(const std::string& serializedValue) = 0;
    virtual std::vector<std::string> GetEnumNames() = 0;
    virtual std::type_index GetUnderlyingType() const = 0;
};


template <typename T>
class SerializedField : public EditorVariable
{
public:
    SerializedField(T& variable) :value(variable) {};
    SerializedField(Component& component, const std::string& name, T& variable) : value(variable)
    {
        component.RegisterSerializedField(name, this);
    }

    SerializedField(AI::BehaviorTreeAction& action, const std::string& name, T& variable) : value(variable)
    {
        action.RegisterEditorVariable(name, this);
    }

   SerializedField(AI::State& state, const std::string& name, T& variable) : value(variable)
   {
       state.RegisterEditorVariable(name, this);
   }

    T GetValue() { return value; }

    std::string ValueToString(T& val)const
    {
        if constexpr (std::is_enum_v<T>)
        {
            return std::string(magic_enum::enum_name(val));
        }

        if constexpr (std::is_same<T, std::string>::value)
        {
            return val;
        }

        if constexpr (HasToString<T>)
        {
            return std::to_string(val);
        }

        if constexpr (visit_struct::traits::is_visitable<T>::value)
        {
            std::stringstream ss;
            visit_struct::for_each(val, [&ss](const char* name, const auto& value)
                {
                    ss << name << ":" << value << ' ';
                });
            return ss.str();
        }

        if constexpr (HasIterators<T>)
        {
            std::stringstream ss;
            for (auto element : val)
            {
                SerializedField<decltype(element)> f(element);
                ss << "{" << f.ToString() << "}";
            }
            return ss.str();
        }

        return "";
    }

    std::string ToString() const override
    {
        return ValueToString(value);
    }

    T GetDeserializedValue(const std::string& string)
    {
        T toReturn;
        std::istringstream stringStream(string);

        if constexpr (visit_struct::traits::is_visitable<T>::value)
        {
            visit_struct::for_each(toReturn, [&stringStream](const char* name, auto& value)
                {
                    std::string tempValue;
                    stringStream >> tempValue;
                    size_t colonPos = tempValue.find(':');

                    std::string stringName = tempValue.substr(0, colonPos);
                    std::stringstream val = std::stringstream(tempValue.substr(colonPos + 1));

                    val >> value;
                });
            return toReturn;
        }

        if constexpr (std::is_same<T, std::string>::value)
        {
            stringStream >> toReturn;
            return toReturn;
        }

        if constexpr (std::is_enum_v<T>)
        {
            auto temp = magic_enum::enum_cast<T>(stringStream.str());
            if (temp.has_value())
            {
                toReturn = temp.value();
            }
            return toReturn;
        }

        if constexpr (HasToString<T>)
        {
            stringStream >> toReturn;
            return toReturn;
        }

        if constexpr (HasIterators<T> && !std::is_same<T, std::string>::value)
        {
            std::string element;
            while (std::getline(stringStream, element, '}'))
            {
                // Extract the content inside curly braces
                std::string content = element.substr(1, element.size() - 1);

                using ElementType = typename std::remove_const<std::remove_reference<decltype(*std::begin(std::declval<T>()))>::type>::type;
                ElementType d;
                SerializedField<ElementType> temp(d);
                temp.Deserialize(content);

                if constexpr (HasPushBack<T, ElementType>)
                {
                    toReturn.push_back(temp.GetValue());
                }
                else if constexpr (HasInsert<T, ElementType> && !HasPushBack<T, ElementType>)
                {
                    toReturn.insert(temp.GetValue());
                }
            }
            return toReturn;
        }

        return toReturn;
    }

    void Deserialize(const std::string& serializedValue) override
    {
        value = GetDeserializedValue(serializedValue);
    }

    std::vector<std::string> ExtractElements(const std::string& input)
    {
        std::vector<std::string> elements;
        std::string element;

        bool insideBraces = false;
        for (char c : input)
        {
            if (c == '{')
            {
                insideBraces = true;
            }
            else if (c == '}')
            {
                insideBraces = false;
                if (!element.empty())
                {
                    elements.push_back(element);
                    element.clear();
                }
            }
            else if (insideBraces)
            {
                element += c;
            }
        }

        return elements;
    }

    std::type_index GetTypeInfo() const override
    {
        return typeid(T);
    }

    std::vector<std::string> GetEnumNames() override
    {
        assert(std::is_enum_v<T>);
        std::vector<std::string> toReturn;
        if constexpr (std::is_enum_v<T>)
        {
            constexpr auto names = magic_enum::enum_names<T>();
            for (auto name : names)
            {
                toReturn.emplace_back(name);
            }
            return toReturn;
        }

        return {};
    }

    std::type_index GetUnderlyingType() const override
    {
        if constexpr (HasIterators<T>)
        {
            return typeid(*value.begin());
        }
        return typeid(void*);
    }

private:
    T& value;
};

#define SERIALIZE_FIELD(type, name) \
        type name;                             \
        SerializedField<type> name##Helper{*this, #name, name};
#pragma once
#include <sstream>
#include <typeindex>

#include "GenericFactory.hpp"
#include "Visit struct/visit_struct.hpp"
#include "nlohmann/json.hpp"

struct Transform;
class Entity;

template <typename U>
concept HasToString = requires(const U & type)
{
    std::to_string(type);
};

class EditorVariable
{
public:
    virtual ~EditorVariable() = default;
    virtual std::string ToString() const = 0;
    virtual std::type_index GetTypeInfo() const = 0;
    virtual void Deserialize(const std::string& serializedValue) = 0;
};

class Component
{
public:
    Component(Entity& entityToSet) : entity(entityToSet) {};
	Component(Component&& other) noexcept:entity(other.entity)
    {
        editorVariables = other.editorVariables;
    }

    Entity& GetEntity() const { return entity; }
    Transform& GetTransform() const;

    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void OnDestroy();

    virtual ~Component();

    void RegisterSerializedField(const std::string& name, EditorVariable* variable)
    {
        editorVariables[name] = variable;
    }

    nlohmann::json Serialize();
    void Deserialize(nlohmann::json& json);

    bool tickInEditor = false;
	std::unordered_map<std::string, EditorVariable*>& GetSerializedFields() { return editorVariables; }
protected:
    std::unordered_map<std::string, EditorVariable*> editorVariables{};
    std::reference_wrapper<Entity> entity;

    friend class BehaviorEditorVariable;
private:
    void SetOwner(Entity& owner) { entity = owner; }
    friend class EntityInspector;
    friend class Entity;
};


template <typename T>
class SerializedField : public EditorVariable
{
public:
    SerializedField(Component& component, const std::string& name, T& variable) : value(variable)
    {
        component.RegisterSerializedField(name, this);
    }

    std::string ToString() const override
    {
        if constexpr (std::is_same<T, std::string>::value)
        {
	        return value;
        }

        if constexpr (HasToString<T>)
        {
            return std::to_string(value);
        }

        if constexpr (visit_struct::traits::is_visitable<T>::value)
        {
            std::stringstream ss;
            visit_struct::for_each(value,[&ss](const char* name, const auto& value) 
            {
            	ss << name << ":" << value << ' ';
            });
            return ss.str();
        }
    }

    void Deserialize(const std::string& serializedValue) override
    {
        std::istringstream stringStream(serializedValue);

        if constexpr (visit_struct::traits::is_visitable<T>::value)
        {
            visit_struct::for_each(value, [&stringStream](const char* name,  auto& value)
            {
            	std::string tempValue;
                stringStream >> tempValue;
                size_t colonPos = tempValue.find(':');

                std::string stringName = tempValue.substr(0, colonPos);
                std::stringstream val = std::stringstream(tempValue.substr(colonPos + 1));

                val >> value;
            });
        }

        if constexpr (std::is_same<T, std::string>::value)
        {
            stringStream >> value;
        }

        if constexpr (HasToString<T>)
        {
            stringStream >> value;
        }
    }

    std::type_index GetTypeInfo() const override
    {
        return typeid(T);
    }
private:
    T& value;
};

#define SERIALIZE_FIELD(type, name) \
        type name;                             \
        SerializedField<type> name##Helper{*this, #name, name};


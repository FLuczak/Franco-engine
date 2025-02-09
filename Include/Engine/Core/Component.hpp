#pragma once
#include "nlohmann/json.hpp"
#include "GenericFactory.hpp"

class EditorVariable;
struct CollisionEvent;
struct Transform;
class Entity;


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

    virtual void OnCollisionEnter(CollisionEvent& event){};
    virtual void OnCollisionStay(CollisionEvent& event){};
    virtual void OnCollisionLeave(CollisionEvent& event){};

    virtual ~Component();

    void RegisterSerializedField(const std::string& name, EditorVariable* variable);

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





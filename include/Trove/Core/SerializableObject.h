#pragma once
#include "lilly/string.hpp"
#include "lilly/vector.hpp"
#include "Field.h"
#include "Trove/Core/CPacket.h"
#include <cstdint>

struct QWorld;
struct Component;
template<typename T>
T* createComponent(short componentId);
struct ComponentPair {
    uintptr_t encrypted = 0;
    Component* component = nullptr;

    ComponentPair(Component* ptr, const uintptr_t encryptionKey) noexcept {
        if (!ptr) return;
        this->component = ptr;
        encrypted = reinterpret_cast<uintptr_t>(ptr) ^ encryptionKey;
    }
};

struct SerializableObject : public SerializableField {
    enum class ObjectType : char {
        SERVICE = 0,
        LOCAL_PLAYER = 1,
        ENTITY = 2,
        ITEM = 3,
    };

    void *field_28;
    void *field_30;
    void* field_38;
    int64_t entityId;
    void *field_48;
    bool isEntity;
    ObjectType objectType;
    uint32_t flags;
    bool field_58;
    TField<Trove::String> prefabField;
    TField<double> maxGameTime;
    TField<double> doubleField2;
    SmallVector<uint16_t, 4> componentIds;
    SmallVector<ComponentPair, 4> components;
    Vector<SerializableObject*> effects;
    Vector<SerializableObject*> banners;
    void* field_1D0;
    QWorld *world;
    void *network;

    [[nodiscard]] Component* getComponentById(short component_id) const;
    [[nodiscard]] std::vector<Component*> getAllComponents() const;
    void assignComponent(Component* component);

    template <typename T>
    T* getComponent(const short componentId = T::getId()) const {
        return static_cast<T*>(getComponentById(componentId));
    }

    template <typename T>
    T* requireComponent(const short componentId = T::getId()) {
        T* component = static_cast<T*>(getComponentById(componentId));
        if (!component) {
            if ((component = createComponent<T>(componentId))) {
                assignComponent(component);
            }
        }
        return static_cast<T*>(component);
    }

    void queuePacket(const Trove::ComponentPacket&) const;

    template <typename... Args>
    void queuePacket(const char* name, Args&&... args) {
        Trove::ComponentPacket packet(name, std::forward<Args>(args)...);
        queuePacket(packet);
    }

    template<typename Ret, typename... Args>
    Ret call_virtual(unsigned int index, Args&&... args) {
        auto vtable = *reinterpret_cast<void***>(const_cast<SerializableObject*>(this));

        auto fn = reinterpret_cast<Ret(*)(const SerializableObject*, Args...)>(
            vtable[index]
        );

        return fn(this, std::forward<Args>(args)...);
    }
    template<typename Ret, typename... Args>
    Ret call_virtual(unsigned int index, Args&&... args) const {
        auto vtable = *reinterpret_cast<void***>(const_cast<SerializableObject*>(this));

        auto fn = reinterpret_cast<Ret(*)(const SerializableObject*, Args...)>(
            vtable[index]
        );

        return fn(this, std::forward<Args>(args)...);
    }

    [[nodiscard]] int32_t getObjectType() const {
    	return call_virtual<int32_t>(14);
    }
    [[nodiscard]] bool isPlayer() const {
    	return getObjectType() == 0;
    }
    [[nodiscard]] bool isNPC() const {
    	return getObjectType() == 44;
    }
    [[nodiscard]] const Trove::String& getPrefab() const {
        static const Trove::String none("N/A");
        return prefabField.data.empty() ? none : prefabField.data;
    }
    [[nodiscard]] const Trove::String& getName() const { return getPrefab(); }
};

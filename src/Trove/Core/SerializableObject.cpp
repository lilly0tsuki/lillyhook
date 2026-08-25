#include "Trove/Core/SerializableObject.h"
#include "Trove/Core/Packet.h"
#include "Trove/Components/Component.h"

Component* SerializableObject::getComponentById(const short component_id) const {
    if (componentIds.size() != components.size()) {
        return nullptr;
    }
    for (unsigned int i = 0; i < componentIds.size(); ++i) {
        if (componentIds.at(i) == component_id) {
            return components.at(i).component;
        }
    }
    return nullptr;
}

std::vector<Component*> SerializableObject::getAllComponents() const {
    std::vector<Component*> valid;
    for (const auto& component : components) {
        if (component.component) {
            valid.push_back(component.component);
        }
    }
    return valid;
}

void SerializableObject::assignComponent(Component* component) {
    if (!component) return;

    const ComponentPair pair(component, components.inlineBuffer[0].encrypted);
    components.push_back(pair);
    componentIds.push_back(component->getComponentId());
    component->assignParent(this);
}

void SerializableObject::queuePacket(const Trove::ComponentPacket& packetData) const {
    CPacket pkt;
    pkt.entityId = this->entityId;
    pkt.msg = packetData.header.msg;
    pkt.type = packetData.header.type;
    pkt.componentId = packetData.header.id;

    BufferView view;
    packetData.buffer.getReadView(view);
    pkt.data = &view;
    pkt.packetType = PacketType::World;
    pkt.sendPacket();
}

#include "Trove/Core/Packet.h"
#include "Trove/Ext.h"
#include "lilly/buffers.hpp"

void Packet::sendLobby(Trove::Result& outResult) {
	SocialPacket spacket;

	BufferWriter writer;
	IWriteBuffer* pwriter = &writer;
	this->write(pwriter);

	BufferView view;
	writer.getReadView(view);
	spacket.data = &view;
	spacket.key = this->getKey();
	Trove::sendPacket(Trove::getLobbyServer(), &outResult, &spacket);
}
void Packet::sendWorld(Trove::Result& outResult) {
	Trove::sendPacket(Trove::getWorldServer(), &outResult, this);
}
void Packet::sendAuth(Trove::Result& outResult) {
	Trove::sendPacket(Trove::getLoginServer(), &outResult, this);
}

bool Packet::sendPacket() {
	Trove::Result outResult;
	switch (packetType) {
		case PacketType::Lobby:
			sendLobby(outResult);
			break;

		case PacketType::World:
			sendWorld(outResult);
			break;

		case PacketType::Auth:
			sendAuth(outResult);
			break;

		default:
			outResult.code = 1;
			break;
	}
	return outResult.code == 0;
}

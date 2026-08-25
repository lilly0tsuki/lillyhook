#include "Trove/Components/Friendslist.h"
#include "lilly/string.hpp"

void FriendslistComponent::requestSpecificWorld(long long worldId) {
	queuePacket("RequestSpecificWorld", worldId);
}

void FriendslistComponent::joinPlayer(Trove::String playerName) {
	queuePacket("JoinPlayer", playerName);
}

void FriendslistComponent::invitePlayer(Trove::String playerName) {
	queuePacket("InvitePlayer", playerName, 0);
}

void FriendslistComponent::invitePlayerToPVPTeam(Trove::String playerName) {
	queuePacket("InvitePlayerToPVPTeam", playerName);
}

void FriendslistComponent::respondToPVPInvite(long long playerId, int errorCode = 0) {
	queuePacket("RespondToPVPInvite", playerId, errorCode);
}

void FriendslistComponent::onJoinPVPResponse(long long playerId, long long worldId, Trove::String playerName, bool response) {
	queuePacket("OnJoinPVPResponse", playerId, worldId, playerName, response);
}

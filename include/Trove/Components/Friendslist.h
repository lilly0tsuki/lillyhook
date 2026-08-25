#pragma once
#include "Trove/Components/Component.h"
#include "lilly/string.hpp"

struct FriendslistComponent : Component {
	void requestSpecificWorld(long long worldId);
	void joinPlayer(Trove::String playerName);
	void invitePlayer(Trove::String playerName);
	void invitePlayerToPVPTeam(Trove::String playerName);
	void respondToPVPInvite(long long playerId, int errorCode);
	void onJoinPVPResponse(long long playerId, long long worldId, Trove::String playerName, bool response);
	static constexpr short getId() { return ComponentId::FRIENDSLIST; }
};

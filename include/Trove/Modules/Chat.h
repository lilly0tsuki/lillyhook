#pragma once
#include "Trove/Components/UI/UIComponent.h"
#include "Trove/Core/QWorld.h"
#include "Trove/Ext.h"
#include "lilly/string.hpp"

static constexpr unsigned int RGB(unsigned int r, unsigned int g, unsigned int b) {
	return (r) | (g << 8) | (b << 16);
}
enum Color {
	red = RGB(200, 0, 0),
	brightred = RGB(255, 0, 0),
	darkred = RGB(150, 0, 0),

	orange = RGB(255, 140, 0),
	brightorange = RGB(255, 165, 0),
	darkorange = RGB(200, 100, 0),

	burntorange = RGB(204, 85, 0),
	brightburntorange = RGB(255, 100, 0),
	darkburntorange = RGB(170, 70, 0),

	green = RGB(0, 200, 0),
	brightgreen = RGB(0, 255, 0),
	darkgreen = RGB(0, 150, 0),

	limegreen = RGB(50, 205, 50),
	brightlime = RGB(0, 255, 0),
	darklime = RGB(34, 139, 34),

	teal = RGB(0, 128, 128),
	brightteal = RGB(0, 255, 255),
	darkteal = RGB(0, 100, 100),

	olive = RGB(128, 128, 0),
	brightolive = RGB(173, 255, 47),
	darkolive = RGB(85, 107, 47),

	yellow = RGB(200, 200, 0),
	brightyellow = RGB(255, 255, 0),
	darkyellow = RGB(150, 150, 0),

	gold = RGB(255, 215, 0),
	brightgold = RGB(255, 223, 0),
	darkgold = RGB(184, 134, 11),

	blue = RGB(0, 0, 200),
	brightblue = RGB(0, 0, 255),
	darkblue = RGB(0, 0, 150),

	skyblue = RGB(135, 206, 235),
	brightskyblue = RGB(135, 206, 250),
	darkskyblue = RGB(70, 130, 180),

	navy = RGB(0, 0, 128),
	brightnavy = RGB(25, 25, 112),
	darknavy = RGB(0, 0, 100),

	magenta = RGB(200, 0, 200),
	brightmagenta = RGB(255, 0, 255),
	darkmagenta = RGB(150, 0, 150),

	violet = RGB(238, 130, 238),
	brightviolet = RGB(221, 160, 221),
	darkviolet = RGB(148, 0, 211),

	cyan = RGB(0, 200, 200),
	brightcyan = RGB(0, 255, 255),
	darkcyan = RGB(0, 150, 150),

	turquoise = RGB(64, 224, 208),
	brightturquoise = RGB(72, 209, 204),
	darkturquoise = RGB(47, 79, 79),

	white = RGB(200, 200, 200),
	brightwhite = RGB(255, 255, 255),
	darkwhite = RGB(150, 150, 150),

	gray = RGB(128, 128, 128),
	brightgray = RGB(192, 192, 192),
	darkgray = RGB(80, 80, 80),

	black = RGB(0, 0, 0),
	brightblack = RGB(50, 50, 50),
	darkblack = RGB(0, 0, 0)
};

struct ChannelInfo {
	enum class ChannelType {
		SYSTEM = 1,
		GENERAL = 2,
		CLUB = 3
	} channelType;
	struct ChannelDetails {
		Trove::String displayName;
		Trove::String fullName;
		Trove::String channelId;
	} channelDetails;
};

struct ChatUIComponent : UIComponent {
	unsigned char _pad_0x0[568]{};
	std::vector<ChannelInfo*> channelList;

	void chatInfo(const Trove::String& message, const Trove::String& author, Color msgcolor) {
		Trove::chatInfo(this, &message, msgcolor, &author);
	}

	[[nodiscard]] std::vector<ChannelInfo*> getAllChannels() const {
		return channelList;
	}

	[[nodiscard]] ChannelInfo* getChannelByDisplayName(const Trove::String& name) const {
		for (auto* channel : channelList) {
			if (channel && channel->channelDetails.displayName == name) {
				return channel;
			}
		}
		return nullptr;
	}

	[[nodiscard]] ChannelInfo* getChannelByFullName(const Trove::String& name) const {
		for (auto* channel : channelList) {
			if (channel && channel->channelDetails.fullName == name) {
				return channel;
			}
		}
		return nullptr;
	}

	[[nodiscard]] ChannelInfo* getChannelByID(const Trove::String& id) const {
		for (auto* channel : channelList) {
			if (channel && channel->channelDetails.channelId == id) {
				return channel;
			}
		}
		return nullptr;
	}

	static constexpr short getId() {
		return ComponentId::UI_CHAT;
	}
};

static ChatUIComponent* initChatUI() {
	constexpr auto UISERVICEID = -9223372036854775807;
	const QWorld* world = Trove::getWorld();
	if (!world) return nullptr;

	const auto uiService = world->getService(UISERVICEID);
	if (!uiService) return nullptr;
	return uiService->getComponent<ChatUIComponent>();
}
static void to_chat(const Trove::String& message, const Color msgcolor, const Trove::String& author) {
	const auto chatComponent = initChatUI();
	if (!chatComponent) return;
	chatComponent->chatInfo(message, author, msgcolor);
}
namespace Trove {
	constexpr Color SUCCESS_COLOR = Color::green;
	constexpr Color ERROR_COLOR = Color::red;
	constexpr Color INFO_COLOR = Color::yellow;

	inline void print(const Trove::String& message, const Color msgcolor = SUCCESS_COLOR) {
		to_chat(message, msgcolor, "System");
	}

	inline void err(const Trove::String& message) {
		to_chat(message, ERROR_COLOR, "Error");
	}
}

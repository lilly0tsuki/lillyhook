#include "Trove/Commands/Commands.h"
#include "Trove/Core/World.h"
#include "Trove/Ext.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

namespace Trove::Commands {
	std::map<std::string, unsigned int> jokes = {
		{"I told my wife she was drawing her eyebrows too high. She looked surprised.", 0},
		{"I told my computer I needed a break. It froze.", 1},
		{"Parallel lines have so much in common. It's a shame they'll never meet.", 2},
		{"I asked my dog what's two minus two. He said nothing.", 3},
		{"I told my Wi-Fi we needed space. Now it's giving me distance.", 4},
		{"I used to play piano by ear, but now I use my hands.", 5},
		{"Why can't a nose be 12 inches long? Because then it'd be a foot.", 6},
		{"I'd tell you a chemistry joke, but I'd get no reaction.", 7},
		{"I'm reading a book on anti-gravity. It's impossible to put down.", 8},
		{"My wallet and I are no longer on speaking terms.", 9},
		{"I'd agree with you, but then we'd both be wrong.", 10},
		{"I told my suitcase we're not going on vacation. Now it's emotional baggage.", 11},
		{"I used to be addicted to soap, but I'm clean now.", 12},
		{"I stayed up all night wondering where the sun went. Then it dawned on me.", 13},
		{"I'd tell you a time-travel joke, but you didn't laugh last time.", 14},
		{"Why don't skeletons fight each other? They don't have the guts.", 15},
		{"My job is secure. No one else wants it.", 16},
		{"The early bird gets the worm, but the second mouse gets the cheese.", 17},
		{"I'm on a whiskey diet. I've lost three days already.", 18},
		{"I told my phone to stop autocorrecting me. It ducking ignored me.", 19},
		{"I used to think I was indecisive, but now I'm not so sure.", 20},
		{"My math teacher called me average. How mean.", 21},
		{"I don't trust stairs. They're always up to something.", 22},
		{"I have a fear of speed bumps, but I'm slowly getting over it.", 23},
		{"Some people say money talks. Mine just says goodbye.", 24},
		{"My therapist says I have a preoccupation with vengeance. We'll see about that.", 25},
		{"Why did the scarecrow win an award? He was outstanding in his field.", 26},
		{"I told my plants I love them. They're rooting for me.", 27},
		{"I threw a boomerang years ago. I live in constant fear.", 28},
		{"I'm writing a book on reverse psychology. Don't read it.", 29},
		{"My printer and I are no longer compatible-it keeps giving me bad paper jams.", 30},
		{"I told my calendar its days were numbered. It wasn't surprised.", 31},
		{"I used to be a banker, but I lost interest.", 32},
		{"I'm friends with 25 letters of the alphabet. I don't know Y.", 33},
		{"I ordered a chicken and an egg online. I'll let you know.", 34},
		{"My pen stopped working. I can't draw any conclusions.", 35},
		{"I tried to catch fog yesterday. I mist.", 36},
		{"I used to hate facial hair, but it grew on me.", 37},
		{"My shoes keep falling apart. I guess that's sole destroying.", 38},
		{"I'm terrified of elevators. I'm taking steps to avoid them.", 39},
		{"I bought shoes from a drug dealer. I don't know what he laced them with, but I've been tripping all day.", 40},
		{"I told my coffee it was brewing trouble. It couldn't espresso itself.", 41},
		{"I used to work at a calendar factory, but I got fired for taking days off.", 42},
		{"I'm reading a book about teleportation. It's bound to take me places.", 43},
		{"My GPS and I had a disagreement. Now we're taking different routes.", 44},
		{"I tried to write with a broken pencil. It was pointless.", 45},
		{"I told my refrigerator a joke. It gave me the cold shoulder.", 46},
		{"I used to be afraid of hurdles, but I got over it.", 47},
		{"My vacuum and I broke up. It was just picking up dirt on me.", 48},
		{"I tried to organize a hide-and-seek tournament, but it was hard to find good players.", 49},
		{"I told my bed we needed to split up. Now I sleep on the couch.", 50}
	};
	static bool channelBlacklisted(Trove::String& channelId) {
		for (std::vector<Trove::String> blacklist = { "/r1", "/r2", "/r3" }; const auto& blacklisted : blacklist) {
			if (channelId == blacklisted) {
				return true;
			}
		}
		return false;
	}
	static unsigned int bounded_rand(const unsigned int range) {
		while (true) {
			constexpr unsigned int rand_max = 0x7FFF;
			const unsigned int x = std::rand();
			if (const unsigned int r = x % range; x - r <= rand_max - range + 1) {
				return r;
			}
		}
	}
	static void jotdCommandHandler(const CommandData& data) {
		std::vector<ChannelInfo*> channels = data.world.getServiceByName("UI")->getComponent<ChatUIComponent>()->getAllChannels();
		const unsigned int jokeIndex = bounded_rand(jokes.size());
		Trove::String joke;
		for (const auto& [key, value] : jokes) {
			if (value == jokeIndex) {
				joke = key;
				break;
			}
		}
		if (joke.empty()) {
			joke = "Why did the chicken cross the road? To get to the other side!";
		}
		unsigned int channelIndex = bounded_rand(channels.size());
		ChannelInfo* channel = channels[channelIndex];
		while (channelBlacklisted(channel->channelDetails.channelId)) {
			channelIndex = bounded_rand(channels.size());
			channel = channels[channelIndex];
		}
		Trove::sendChatMessage(&channel->channelDetails.fullName, &joke);
	}
	static bool _reg_jotdCommandHandler = Trove::Commands::defer("jotd", jotdCommandHandler);
}

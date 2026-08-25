#include "Trove/Commands/Commands.h"
#include "Trove/Core/Chunk.h"
#include "Trove/Core/QPlayerCharacter.h"
#include "Trove/Core/World.h"
#include "Trove/Modules/Chat.h"
#include "lilly/vectors.h"
#include <algorithm>
#include <format>
#include <utility>
#include <vector>

namespace Trove::Commands {
	constexpr auto NO_BLOCKS_ERROR = "No chunks found.";
	constexpr auto BLOCK_INFO_FORMAT =
		"Chunk Position: ({}, {}, {})\n"
		"Chunk Object: 0x{:X}";
	constexpr auto TOTAL_BLOCKS_FORMAT = "Total Chunks: {}";

	static void chunksCommandHandler(const CommandData& data) {
		const Vec3& pos = data.player.position;

		std::vector<std::pair<IVec3, Chunk*>> chunks;

		for (const auto& chunkPair : data.world.chunks) {
			chunks.emplace_back(chunkPair);
		}

		std::ranges::sort(chunks, [&pos](const auto& a, const auto& b) {
			const auto aPos = Vec3(static_cast<float>(a.first.x), static_cast<float>(a.first.y), static_cast<float>(a.first.z));
			const auto bPos = Vec3(static_cast<float>(b.first.x), static_cast<float>(b.first.y), static_cast<float>(b.first.z));
			return Vec3::distance(pos, aPos) > Vec3::distance(pos, bPos);
		});

		if (chunks.empty()) {
			Trove::err(NO_BLOCKS_ERROR);
			return;
		}

		for (const auto& [fst, snd] : chunks) {
			const auto& [x, y, z] = fst;
			Chunk* chunk = snd;

			Trove::print(std::format(BLOCK_INFO_FORMAT,
				x, y, z,
				reinterpret_cast<size_t>(chunk))
			);
		}
		Trove::print(std::format(TOTAL_BLOCKS_FORMAT, chunks.size()));
	}
	static bool _reg_chunksCommandHandler = Trove::Commands::defer("chunks", chunksCommandHandler);
}

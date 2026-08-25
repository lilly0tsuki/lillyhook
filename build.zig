const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = std.builtin.OptimizeMode.ReleaseFast;

    const target = b.standardTargetOptions(.{
        .default_target = .{
            .cpu_arch = .x86_64,
            .os_tag = .windows,
            .cpu_model = .native,
        },
    });

    const lib = b.addLibrary(.{
        .name = "lillyhook",
        .linkage = .dynamic,
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libcpp = true,
        }),
    });

    lib.root_module.addIncludePath(b.path("include"));

    const compile_flags = &[_][]const u8{
        "-Wall",
        "-Wextra",
        "-pedantic-errors",
        "-std=c++23",
    };

    const source_files = &[_][]const u8{
        "src/DllMain.cpp",
        "src/Trove/Commands/adventure.cpp",
        "src/Trove/Commands/calc.cpp",
        "src/Trove/Commands/chunks.cpp",
        "src/Trove/Commands/clear.cpp",
        "src/Trove/Commands/cmd.cpp",
        "src/Trove/Commands/crashserver.cpp",
        "src/Trove/Commands/cshub.cpp",
        "src/Trove/Commands/entities.cpp",
        "src/Trove/Commands/exec.cpp",
        "src/Trove/Commands/execas.cpp",
        "src/Trove/Commands/execpkt.cpp",
        "src/Trove/Commands/exp.cpp",
        "src/Trove/Commands/exportcollections.cpp",
        "src/Trove/Commands/face.cpp",
        "src/Trove/Commands/fill.cpp",
        "src/Trove/Commands/find.cpp",
        "src/Trove/Commands/fjoin.cpp",
        "src/Trove/Commands/harvest.cpp",
        "src/Trove/Commands/hvclip.cpp",
        "src/Trove/Commands/joinplayer.cpp",
        "src/Trove/Commands/jotd.cpp",
        "src/Trove/Commands/look.cpp",
        "src/Trove/Commands/market.cpp",
        "src/Trove/Commands/move.cpp",
        "src/Trove/Commands/obj.cpp",
        "src/Trove/Commands/objcomp.cpp",
        "src/Trove/Commands/open.cpp",
        "src/Trove/Commands/quse.cpp",
        "src/Trove/Commands/report.cpp",
        "src/Trove/Commands/req.cpp",
        "src/Trove/Commands/services.cpp",
        "src/Trove/Commands/stat.cpp",
        "src/Trove/Commands/take.cpp",
        "src/Trove/Commands/tp.cpp",
        "src/Trove/Commands/use.cpp",
        "src/Trove/Commands/worldinfo.cpp",
        "src/Trove/Components/AdventureComponent.cpp",
        "src/Trove/Components/AuctionComponent.cpp",
        "src/Trove/Components/CollectionsComponent.cpp",
        "src/Trove/Components/FriendslistComponent.cpp",
        "src/Trove/Components/LocalizationComponent.cpp",
        "src/Trove/Components/PlayerModeComponent.cpp",
        "src/Trove/Components/StatsComponent.cpp",
        "src/Trove/Components/UpdateComponent.cpp",
        "src/Trove/Core/Block.cpp",
        "src/Trove/Core/Chunk.cpp",
        "src/Trove/Core/CPacket.cpp",
        "src/Trove/Core/Deco.cpp",
        "src/Trove/Core/SerializableObject.cpp",
        "src/Trove/Core/Packet.cpp",
        "src/Trove/Core/World.cpp",
        "src/Trove/Inventory/Item/Item.cpp",
        "src/Trove/Inventory/Item/ThrowableComponent.cpp",
        "src/Trove/Inventory/InventoryComponent.cpp",
        "src/Trove/Modules/CrashServerModule.cpp",
        "src/Trove/Protobuf/PacketSerializer.cpp",
        "src/Trove/Protobuf/PrefabDeserializer.cpp",
    };

    lib.root_module.addCSourceFiles(.{
        .files = source_files,
        .flags = compile_flags,
    });

    b.installArtifact(lib);
}

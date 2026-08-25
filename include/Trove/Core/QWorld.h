#pragma once
#include <lilly/vectors.h>
#include "World.h"
#include <condition_variable>
#include <cstdint>

struct Block;
struct BlockInfo;
struct Chunk;

struct QWorld : public World {
    void* unk_1_q;
    void* unk_2_q;
    void* unk_3_q;
    void* unk_4_q;
    void* unk_5_q;
    void* unk_6_q;
    void** vtable_1_q;
    char inline_1_q[456];
    char map_3_q[32];
    Trove::hashmap<void*, void*> map_508_q;
    void* unk_7_q;
    int64_t unk_8_q;
    volatile long interlocked_state_q;
    std::condition_variable cnd_var_q;
    char pad_1384_q[64];
    int64_t mutex_q;
    char pad_1456_q[72];
    void* thread_h_q;
    bool unk_bool_1_q;
    int32_t unk_int_1_q;
};

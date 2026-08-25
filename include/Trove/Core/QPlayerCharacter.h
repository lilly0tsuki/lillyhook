#pragma once
#include "QPawn.h"

struct QPlayerCharacter : public QPawn {
    void *field_A60;
    Vec3 field_A68;
    uint32_t field_A74;
    uint32_t field_A78;
    bool field_A7C;
};

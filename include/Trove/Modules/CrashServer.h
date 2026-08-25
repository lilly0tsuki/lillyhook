#pragma once
#include "Trove/Core/QPlayerCharacter.h"
#include "lilly/string.hpp"
enum CrashMethod {
	CRASH_EQUIP = 1,
};

bool crashServer(const QPlayerCharacter& localPlayer, CrashMethod method = CRASH_EQUIP, Trove::String* result_message = nullptr);

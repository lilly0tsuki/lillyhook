#pragma once
#include "Trove/Components/Component.h"

struct UIComponent : Component {

	const char* getUIName() {
		return reinterpret_cast<const char* (*)()>((*reinterpret_cast<void***>(this))[25])();
	}
};
#pragma once
#include "Trove/Components/Component.h"
#include "lilly/vectors.h"

struct CameraComponent : Component {
	unsigned char _pad_0x5D[39]{};
	float pitch;
	float yaw;
	float fov;
	float zoom;
	float min_zoom;
	float max_zoom;
	float zoom_tick;
	unsigned char _pad_0xA4[116]{};
	float ease_in_speed;
	float ease_out_speed;
	unsigned char _pad_0x120[48]{};
	Vec3 center_offset;

	static constexpr short getId() { return ComponentId::CAMERA; }
};

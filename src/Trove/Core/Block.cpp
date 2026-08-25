#include "Trove/Core/Block.h"

unsigned short Block::type() const { return m_type; }
unsigned short Block::light() const { return m_light; }
unsigned int Block::variant() const { return m_variant & 0xFFFFFF00; }
unsigned char Block::permission() const { return m_variant & 0xFF; }

void Block::setType(const unsigned short type) {
	m_type = type;
}

void Block::setLight(const unsigned short light) {
	m_light = light;
}

void Block::setVariant(const unsigned int variant) {
	const unsigned char permission = m_variant & 0xFF;
	m_variant = (variant & 0xFFFFFF00) | permission;
}

void Block::setPermission(const unsigned char permission) {
	m_variant = (m_variant & 0xFFFFFF00) | (permission & 0xFF);
}
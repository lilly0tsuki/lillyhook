#include "Trove/Components/Localization.h"
#include "Trove/Ext.h"
#include "lilly/string.hpp"

Trove::String& LocalizationComponent::translateName() const {
	return *Trove::translate(&name);
}
Trove::String& LocalizationComponent::translateDescription() const {
	return *Trove::translate(&description);
}

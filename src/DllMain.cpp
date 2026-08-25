#include "Trove/Commands/Commands.h"
#include "lilly/external.hpp"

int __stdcall DllMain(void*, unsigned int reason, void*) {
	if (reason == 1) {
		aobscan::initFind();
		Trove::Commands::registerCommands();
	}
	return 1;
}

#pragma once
#include <iostream>

class CommandConsole
{
	enum Command {
		Shutdown
	};

	Command _pendingCommand;
	
};


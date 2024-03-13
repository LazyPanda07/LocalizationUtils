#pragma once

#include "BuildCommand.h"

namespace commands
{
	class DebugBuildCommand : public BuildCommand
	{
	public:
		DebugBuildCommand(const json::JSONParser& settings);

		~DebugBuildCommand() = default;
	};
}

#pragma once

#include "BuildCommand.h"

namespace commands
{
	class ReleaseBuildCommand : public BuildCommand
	{
	public:
		ReleaseBuildCommand(const json::JSONParser& settings);

		~ReleaseBuildCommand() = default;
	};
}

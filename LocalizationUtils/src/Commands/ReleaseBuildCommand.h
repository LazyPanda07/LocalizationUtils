#pragma once

#include "BuildCommand.h"

namespace commands
{
	class ReleaseBuildCommand : public BuildCommand
	{
	private:
		static constexpr std::string_view command = R"(cl /O2 /GL /LD /std:c++20 /utf-8 /MT *.cpp /link /OUT:"{outputFolder}{fileName}.dll" /IMPLIB:{fileName}.lib)";

	public:
		ReleaseBuildCommand(const json::JSONParser& settings);

		~ReleaseBuildCommand() = default;
	};
}

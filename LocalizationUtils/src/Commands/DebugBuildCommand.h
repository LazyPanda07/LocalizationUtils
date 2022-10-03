#pragma once

#include "BuildCommand.h"

namespace commands
{
	class DebugBuildCommand : public BuildCommand
	{
	private:
		static constexpr std::string_view command = R"(cl /O2 /GL /LDd /std:c++20 /utf-8 /MTd *.cpp /link /OUT:"{outputFolder}{fileName}.dll" /IMPLIB:{fileName}.lib)";

	public:
		DebugBuildCommand(const json::JSONParser& settings);

		~DebugBuildCommand() = default;
	};
}

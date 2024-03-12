#pragma once

#include "BuildCommand.h"

namespace commands
{
	class DebugBuildCommand : public BuildCommand
	{
	private:
#ifdef __LINUX__
		static constexpr std::string_view command = R"(g++ -shared -fPIC -O0 -std=c++20 *.cpp -o lib{0}{1}.so)";
#else
		static constexpr std::string_view command = R"(cl /Od /GL /LDd /std:c++20 /utf-8 /MTd *.cpp /link /OUT:"{0}{1}.dll" /IMPLIB:{1}.lib)";
#endif

	public:
		DebugBuildCommand(const json::JSONParser& settings);

		~DebugBuildCommand() = default;
	};
}

#pragma once

#include "BuildCommand.h"

namespace commands
{
	class ReleaseBuildCommand : public BuildCommand
	{
	private:
#ifdef __LINUX__
		static constexpr std::string_view command = R"(g++ -shared -fPIC -O3 -std=c++20 *.cpp -o lib{0}{1}.so)";
#else
		static constexpr std::string_view command = R"(cl /O2 /GL /LD /std:c++20 /utf-8 /MT *.cpp /link /OUT:"{0}{1}.dll" /IMPLIB:{1}.lib)";
#endif
		
	public:
		ReleaseBuildCommand(const json::JSONParser& settings);

		~ReleaseBuildCommand() = default;
	};
}

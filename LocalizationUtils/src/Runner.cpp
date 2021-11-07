#include "Runner.h"

#include "Commands/GenerateCommand.h"
#include "Commands/DebugBuildCommand.h"
#include "Commands/ReleaseBuildCommand.h"

using namespace std;

void Runner::initCommands()
{
	commands[commands_names::generate_command] = make_unique<commands::GenerateCommand>(settings);
	commands[commands_names::debug_build_command] = make_unique<commands::DebugBuildCommand>(settings);
	commands[commands_names::release_build_command] = make_unique<commands::ReleaseBuildCommand>(settings);
}

Runner::Runner(const json::JSONParser& settings) :
	settings(settings)
{
	this->initCommands();
}

void Runner::run(string_view commandName) const
{
	commands.at(commandName)->run();
}

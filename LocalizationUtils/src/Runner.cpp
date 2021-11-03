#include "Runner.h"

#include "Commands/GenerateCommand.h"

using namespace std;

void Runner::initCommands()
{
	commands[commands_names::generate_command] = make_unique<commands::GenerateCommand>(settings);
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

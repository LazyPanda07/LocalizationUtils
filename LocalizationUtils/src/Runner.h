#pragma once

#include "Commands/ICommand.h"

class Runner
{
private:
	const json::JSONParser& settings;
	std::unordered_map<std::string_view, std::unique_ptr<commands::interfaces::ICommand>> commands;

private:
	void initCommands();

public:
	Runner(const json::JSONParser& settings);

	void run(std::string_view commandName) const;

	~Runner() = default;
};

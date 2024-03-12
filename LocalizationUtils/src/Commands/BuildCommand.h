#pragma once

#include "ICommand.h"

namespace commands
{
	class BuildCommand : public interfaces::ICommand
	{
	private:
		std::string_view buildCommand;
		std::string outputFolder;

	public:
		BuildCommand(const json::JSONParser& settings, std::string_view buildCommand, const std::string& outputFolder);

		void run() const override;

		virtual ~BuildCommand() = default;
	};
}

#pragma once

#include "ICommand.h"

namespace commands
{
	class BuildCommand : public interfaces::ICommand
	{
	private:
		std::string outputFolder;

	public:
		BuildCommand(const json::JSONParser& settings, const std::string& outputFolder);

		void run() const override;

		virtual ~BuildCommand() = default;
	};
}

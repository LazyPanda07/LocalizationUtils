#pragma once

#include "ICommand.h"

namespace commands
{
	class GenerateCommand : public interfaces::ICommand
	{
	public:
		GenerateCommand(const json::JSONParser& settings);

		void run() const override;

		~GenerateCommand() = default;
	};
}

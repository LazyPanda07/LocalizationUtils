#pragma once

#include "ICommand.h"

namespace commands
{
	class GenerateSettingsFile : public interfaces::ICommand
	{
	public:
		GenerateSettingsFile(const json::JSONParser& settings);

		void run() const override;

		~GenerateSettingsFile() = default;
	};
}

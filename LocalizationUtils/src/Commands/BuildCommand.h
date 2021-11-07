#pragma once

#include "ICommand.h"

namespace commands
{
	class BuildCommand : public interfaces::ICommand
	{
	private:
		const std::string_view* buildCommand;

	public:
		BuildCommand(const json::JSONParser& settings, const std::string_view* buildCommand);

		void run() const override;

		virtual ~BuildCommand() = default;
	};
}

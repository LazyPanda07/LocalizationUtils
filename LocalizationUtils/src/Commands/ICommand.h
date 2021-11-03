#pragma once

#include "headers.h"

namespace commands
{
	namespace interfaces
	{
		class ICommand
		{
		protected:
			const json::JSONParser& settings;

		public:
			ICommand(const json::JSONParser& settings);

			virtual void run() const = 0;

			virtual ~ICommand() = default;
		};
	}
}

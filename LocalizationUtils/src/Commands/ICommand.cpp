#include "ICommand.h"

namespace commands
{
	namespace interfaces
	{
		ICommand::ICommand(const json::JSONParser& settings) :
			settings(settings)
		{

		}
	}
}

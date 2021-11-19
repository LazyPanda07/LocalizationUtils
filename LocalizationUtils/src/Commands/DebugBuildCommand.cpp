#include "DebugBuildCommand.h"

using namespace std;

namespace commands
{
	DebugBuildCommand::DebugBuildCommand(const json::JSONParser& settings) :
		BuildCommand(settings, &command, settings.getString(settings::debugOutputFolderSetting))
	{

	}
}

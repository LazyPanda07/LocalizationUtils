#include "DebugBuildCommand.h"

using namespace std;

namespace commands
{
	DebugBuildCommand::DebugBuildCommand(const json::JSONParser& settings) :
		BuildCommand(settings, settings.getString(settings::debugOutputFolderSetting).empty() ? global::outputFolder : settings.getString(settings::debugOutputFolderSetting))
	{

	}
}

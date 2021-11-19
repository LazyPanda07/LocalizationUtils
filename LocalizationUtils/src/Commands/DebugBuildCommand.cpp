#include "DebugBuildCommand.h"

using namespace std;

namespace commands
{
	DebugBuildCommand::DebugBuildCommand(const json::JSONParser& settings) :
		BuildCommand(settings, &command, global::outputFolder.empty() ? settings.getString(settings::debugOutputFolderSetting) : global::outputFolder)
	{

	}
}

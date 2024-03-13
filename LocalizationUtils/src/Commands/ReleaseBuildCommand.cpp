#include "ReleaseBuildCommand.h"

using namespace std;

namespace commands
{
	ReleaseBuildCommand::ReleaseBuildCommand(const json::JSONParser& settings) :
		BuildCommand(settings, settings.getString(settings::releaseOutputFolderSetting).empty() ?  global::outputFolder : settings.getString(settings::releaseOutputFolderSetting))
	{

	}
}

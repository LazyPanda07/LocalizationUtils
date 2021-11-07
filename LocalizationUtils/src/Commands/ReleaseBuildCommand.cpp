#include "ReleaseBuildCommand.h"

using namespace std;

namespace commands
{
	ReleaseBuildCommand::ReleaseBuildCommand(const json::JSONParser& settings) :
		BuildCommand(settings, &command, settings.getString(settings::releaseOutputFolderSetting))
	{

	}
}

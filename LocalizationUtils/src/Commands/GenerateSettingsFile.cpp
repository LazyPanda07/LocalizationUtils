#include "GenerateSettingsFile.h"

using namespace std;

namespace commands
{
	GenerateSettingsFile::GenerateSettingsFile(const json::JSONParser& settings) : 
		ICommand(settings)
	{

	}

	void GenerateSettingsFile::run() const
	{
		if (filesystem::exists(global::settingsFile))
		{
			return;
		}

		json::JSONBuilder settingsFile(utility::codepage);
		vector<json::utility::objectSmartPointer<json::utility::jsonObject>> otherLanguages;

		settingsFile[settings::originalLanguageSetting] = ""s;
		settingsFile[settings::otherLanguagesSetting] = move(otherLanguages);
		settingsFile[settings::pathToVisualStudioSetting] = ""s;
		settingsFile[settings::debugOutputFolderSetting] = ""s;
		settingsFile[settings::releaseOutputFolderSetting] = ""s;

		ofstream(global::settingsFile) << settingsFile;
	}
}

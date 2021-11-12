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

		settingsFile[settings::originalLanguageSetting] = "en"s;
		settingsFile[settings::otherLanguagesSetting] = move(otherLanguages);
		settingsFile.appendString(settings::pathToVisualStudioSetting, R"(C:\\Program Files\\Microsoft Visual Studio\\2022\\Community)");
		settingsFile[settings::debugOutputFolderSetting] = ""s;
		settingsFile[settings::releaseOutputFolderSetting] = ""s;

		ofstream(global::settingsFile) << settingsFile;
	}
}

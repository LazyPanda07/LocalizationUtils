#include "Utility.h"

#include <format>
#include <fstream>

#include "JSONBuilder.h"

#include "Constants.h"

using namespace std;

namespace utility
{
	filesystem::path makeLocalizationFile(const string& language, const filesystem::path& pathToLocalizationFolder, const json::JSONBuilder& localizationData)
	{
		filesystem::path fileName = pathToLocalizationFolder / format(global::localizationFile, language);

		ofstream(fileName) << localizationData;

		return fileName;
	}

	json::JSONBuilder copyOriginalLanguage(const json::JSONParser& originalLanguageKeys, const unordered_set<string>& existingKeys)
	{
		json::JSONBuilder result(CP_UTF8);
		unordered_set<string>::const_iterator endIterator = existingKeys.end();

		for (const auto& i : originalLanguageKeys)
		{
			if (existingKeys.find(i->first) == endIterator)
			{
				result[i->first] = ""s;
			}
		}

		return result;
	}

	string convertToUTF8(const string& text)
	{
		return json::utility::toUTF8JSON(text, codepage);
	}

	filesystem::path generateSettingsFile()
	{
		filesystem::path pathToSettinsFile = utility::makePath(global::startFolder, global::settingsFile);

		if (filesystem::exists(pathToSettinsFile))
		{
			return pathToSettinsFile;
		}

		json::JSONBuilder settingsFile(utility::codepage);
		vector<json::utility::objectSmartPointer<json::utility::jsonObject>> otherLanguages;

		settingsFile[settings::originalLanguageSetting] = "en"s;
		settingsFile[settings::otherLanguagesSetting] = move(otherLanguages);
		settingsFile.appendString(settings::pathToVisualStudioSetting, R"(C:\\Program Files\\Microsoft Visual Studio\\2022\\Community)");
		settingsFile[settings::debugOutputFolderSetting] = ""s;
		settingsFile[settings::releaseOutputFolderSetting] = ""s;

		ofstream(pathToSettinsFile) << settingsFile;

		return pathToSettinsFile;
	}
}

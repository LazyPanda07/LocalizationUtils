#include "Utility.h"

#include <format>
#include <fstream>

#include "JSONBuilder.h"

#include "Constants.h"

using namespace std;

namespace utility
{
	string getLocalizationUtilsVersion()
	{
		string version = "1.2.2";

		return version;
	}

	filesystem::path makeLocalizationFile(const string& language, const filesystem::path& pathToLocalizationFolder, const json::JSONBuilder& localizationData)
	{
		filesystem::path fileName = pathToLocalizationFolder / format(global::localizationFile, language);

		if (!filesystem::exists(fileName))
		{
			ofstream(fileName) << localizationData;
		}

		return fileName;
	}

	json::JSONBuilder copyOriginalLanguage(const json::JSONParser& originalLanguageKeys, const unordered_set<string>& existingKeys)
	{
		json::JSONBuilder result(CP_UTF8);
		unordered_set<string>::const_iterator endIterator = existingKeys.end();

		for (const auto& [key, value] : originalLanguageKeys)
		{
			if (existingKeys.find(key) == endIterator)
			{
				result[key] = ""s;
			}
		}

		return result;
	}

	filesystem::path generateSettingsFile()
	{
		filesystem::path pathToSettingsFile = utility::makePath(global::startFolder, global::settingsFile);

		if (filesystem::exists(pathToSettingsFile))
		{
			return pathToSettingsFile;
		}

		json::JSONBuilder settingsFile(CP_UTF8);
		vector<json::utility::jsonObject> otherLanguages;

		settingsFile[settings::originalLanguageSetting] = "en"s;
		settingsFile[settings::otherLanguagesSetting] = move(otherLanguages);
		settingsFile[settings::debugOutputFolderSetting] = ""s;
		settingsFile[settings::releaseOutputFolderSetting] = ""s;
		settingsFile[settings::fileNameSetting] = "LocalizationData";

		ofstream(pathToSettingsFile) << settingsFile;

		return pathToSettingsFile;
	}

	void appendSlash(string& source)
	{
		if (source.size() && (source.back() != '/' && source.back() != '\\'))
		{
			source += getSeparator();
		}
	}

	char getSeparator()
	{
#ifdef __LINUX__
		return '/';
#else
		return '\\';
#endif
	}
}

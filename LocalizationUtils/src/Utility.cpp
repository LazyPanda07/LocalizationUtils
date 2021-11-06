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
}

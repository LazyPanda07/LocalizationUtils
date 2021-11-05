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

	json::JSONBuilder copyOriginalLanguage(const json::JSONParser& originalLanguageKeys)
	{
		json::JSONBuilder result(CP_UTF8);

		for (const auto& i : originalLanguageKeys)
		{
			result[i->first] = ""s;
		}

		return result;
	}
}

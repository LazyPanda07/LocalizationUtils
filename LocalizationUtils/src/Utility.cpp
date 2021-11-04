#include "Utility.h"

#include <format>
#include <fstream>

#include "JSONBuilder.h"

#include "Constants.h"

using namespace std;

namespace utility
{
	filesystem::path makeLocalizationFile(const string& language, const filesystem::path& pathToLocalizationFolder)
	{
		filesystem::path fileName = pathToLocalizationFolder / format(global::localizationSettingsFile, language);

		ofstream(fileName) << json::JSONBuilder(CP_UTF8);

		return fileName;
	}
}

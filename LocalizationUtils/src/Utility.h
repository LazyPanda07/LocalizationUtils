#pragma once

#include <string>
#include <filesystem>

#include "JSONBuilder.h"
#include "JSONParser.h"

namespace utility
{
	std::filesystem::path makeLocalizationFile(const std::string& language, const std::filesystem::path& pathToLocalizationFolder);

	json::JSONBuilder copyOriginalLanguage(const json::JSONParser& originalLanguageKeys);
}

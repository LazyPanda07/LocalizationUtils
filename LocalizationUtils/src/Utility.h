#pragma once

#include <string>
#include <filesystem>
#include <unordered_set>

#include "JSONBuilder.h"
#include "JSONParser.h"

namespace utility
{
	std::filesystem::path makeLocalizationFile(const std::string& language, const std::filesystem::path& pathToLocalizationFolder, const json::JSONBuilder& localizationData = json::JSONBuilder(CP_UTF8));

	json::JSONBuilder copyOriginalLanguage(const json::JSONParser& originalLanguageKeys, const std::unordered_set<std::string>& existingKeys = std::unordered_set<std::string>());
}

#pragma once

#include <string>
#include <filesystem>

namespace utility
{
	std::filesystem::path makeLocalizationFile(const std::string& language, const std::filesystem::path& pathToLocalizationFolder);
}

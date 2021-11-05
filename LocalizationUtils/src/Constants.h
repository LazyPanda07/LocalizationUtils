#pragma once

#include <string>

namespace global
{
	inline std::string startFolder;

	inline constexpr std::string_view localizationFile = "localization_{}.json";
}

namespace settings
{
	inline constexpr std::string_view settingsFile = "localization_utils_settings.json";

	inline const std::string originalLanguageSetting = "originalLanguage";
	inline const std::string otherLanguagesSetting = "otherLanguages";

	inline const std::string_view intermediateFolderName = "intermediate";
	inline const std::string_view localizationFolderName = "localization";
}

namespace commands_names
{
	inline constexpr std::string_view generate_command = "generate";
}

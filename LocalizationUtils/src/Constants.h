#pragma once

#include <string>

namespace settings
{
	inline constexpr std::string_view settingsFile = "localization_utils_settings.json";

	inline const std::string mainLanguageSetting = "mainLanguage";
	inline const std::string otherLanguagesSetting = "otherLanguages";
}

namespace commands_names
{
	inline constexpr std::string_view generate_command = "generate";
}

#pragma once

#include <string>

namespace global
{
	inline std::string startFolder;

	inline constexpr std::string_view localizationFile = "localization_{}.json";
}

namespace build
{
	inline constexpr std::string_view pathTox64Tools = "VC\\Auxiliary\\Build\\vcvars64.bat";
}

namespace utility
{
	inline constexpr uint32_t codepage = 1251;
}

namespace files
{
	inline constexpr std::string_view settingsFile = "localization_utils_settings.json";
	inline constexpr std::string_view metaFile = "localization_meta.json";
}

namespace settings
{
	inline const std::string originalLanguageSetting = "originalLanguage";
	inline const std::string otherLanguagesSetting = "otherLanguages";
	inline const std::string pathToVisualStudioSetting = "pathToVisualStudio";
	inline const std::string debugOutputFolderSetting = "debugOutputFolder";
	inline const std::string releaseOutputFolderSetting = "releaseOutputFolder";
}

namespace folders
{
	inline constexpr std::string_view intermediateFolder = "intermediate";
	inline constexpr std::string_view localizationFolder = "localization";
}

namespace commands_names
{
	inline constexpr std::string_view generate_command = "generate";
	inline constexpr std::string_view debug_build_command = "debug_build";
	inline constexpr std::string_view release_build_command = "release_build";
}

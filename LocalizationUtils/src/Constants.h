#pragma once

#include <string>

namespace global
{
	inline std::string startFolder;
	inline std::string outputFolder;

	inline constexpr std::string_view localizationFile = "localization_{}.json";

	inline std::string settingsFile = "localization_utils_settings.json";
}

namespace build
{
	inline constexpr std::string_view pathTox64Tools = "VC\\Auxiliary\\Build\\vcvars64.bat";
}

namespace utility
{
	inline constexpr uint32_t codepage = 1251;

	inline const std::string buildHashUtilitySetting = "buildHash";
}

namespace files
{
	inline constexpr std::string_view metaFile = "localization_meta.json";

	inline constexpr std::string_view generatedCPPFile = "localization.cpp";
}

namespace settings
{
	inline const std::string originalLanguageSetting = "originalLanguage";
	inline const std::string otherLanguagesSetting = "otherLanguages";
	inline const std::string pathToVisualStudioSetting = "pathToVisualStudio";
	inline const std::string debugOutputFolderSetting = "debugOutputFolder";
	inline const std::string releaseOutputFolderSetting = "releaseOutputFolder";
	inline const std::string fileNameSetting = "fileName";
}

namespace folders
{
	inline constexpr std::string_view intermediateFolder = "intermediate";
	inline constexpr std::string_view binariesFolder = "binaries";
	inline constexpr std::string_view localizationFolder = "localization";

	inline constexpr std::string_view debugBinariesFolder = "debug";
	inline constexpr std::string_view releaseBinariesFolder = "release";
}

namespace commands_names
{
	inline constexpr std::string_view generate_command = "generate";
	inline constexpr std::string_view debug_build_command = "debug_build";
	inline constexpr std::string_view release_build_command = "release_build";
}

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

	std::string convertToUTF8(const std::string& text);

	std::filesystem::path generateSettingsFile();

	void appendSlash(std::string& source);

	template<typename T, typename... Args>
	std::string makePath(T&& part, Args&&... args)
	{
		if constexpr (sizeof...(args))
		{
			std::string tem(part);

			if (tem.back() == '\\' || tem.back() == '/')
			{
				return std::move(tem) + makePath(std::forward<Args>(args)...);
			}
			else
			{
				return std::move(tem) + '\\' + makePath(std::forward<Args>(args)...);
			}
		}
		else
		{
			return std::string(part);
		}
	}
}

#pragma once

#include "headers.h"

class LocalizationSourceFileGenerator
{
private:
	const json::JSONParser& settings;
	std::filesystem::path localizationFolder;

private:
	void appendCore(std::ofstream& cppFile, const std::string& originalLanguage) const;

	void appendLanguage(std::ofstream& cppFile, const json::JSONParser& dictionary, const std::string& language) const;

	void appendDictionaryWithAllLanguages(std::ofstream& cppFile, const std::vector<std::string>& languages) const;

public:
	LocalizationSourceFileGenerator(const json::JSONParser& settings);

	void generate() const;

	~LocalizationSourceFileGenerator() = default;
};

#pragma once

#include "headers.h"

class LocalizationSourceFileGenerator
{
private:
	const json::JSONParser& settings;
	std::filesystem::path localizationFolder;

private:
	static void appendCore(std::ostream& cppFile, const std::string& originalLanguage);

	static bool appendLanguage(std::ostream& cppFile, const json::JSONParser& dictionary, const std::string& language);

	static void appendDictionaryWithAllLanguages(std::ostream& cppFile, const std::vector<std::string>& languages);

	static void appendCCFunctions(std::ostream& cppFile);

public:
	LocalizationSourceFileGenerator(const json::JSONParser& settings);

	void generate() const;

	~LocalizationSourceFileGenerator() = default;
};

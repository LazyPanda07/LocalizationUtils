#pragma once

#include "headers.h"

class CMakeFileGenerator
{
private:
	const json::JSONParser& settings;

public:
	CMakeFileGenerator(const json::JSONParser& settings);

	void generate(const std::filesystem::path& pathToIntermediate, const std::filesystem::path& pathToBinaryDir) const;

	std::string getResultName() const;

	std::string getGenerator() const;

	std::string getMake() const;

	~CMakeFileGenerator() = default;
};

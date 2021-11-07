#pragma once

#include "headers.h"

class Validator
{
private:
	const json::JSONParser& settings;

private:
	void checkError(const std::string& setting, std::vector<std::string>& errors, json::utility::variantTypeEnum type = json::utility::variantTypeEnum::jString) const;

public:
	Validator(const json::JSONParser& settings);

	std::vector<std::string> validate() const;

	~Validator() = default;
};

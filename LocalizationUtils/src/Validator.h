#pragma once

#include "headers.h"

class Validator
{
private:
	const json::JSONParser& settings;

public:
	Validator(const json::JSONParser& settings);

	std::vector<std::string> validate() const;

	~Validator() = default;
};

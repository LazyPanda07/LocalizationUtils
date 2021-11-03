#include "Validator.h"

using namespace std;

Validator::Validator(const json::JSONParser& settings) :
	settings(settings)
{

}

vector<string> Validator::validate() const
{
	using json::utility::variantTypeEnum;

	vector<string> errors;

	if (!settings.contains(settings::mainLanguageSetting, variantTypeEnum::jString))
	{
		errors.push_back(format(R"(Can't find "{}" setting in "{}")"sv, settings::mainLanguageSetting, settings::settingsFile));
	}

	if (!settings.contains(settings::otherLanguagesSetting, variantTypeEnum::jJSONArray))
	{
		errors.push_back(format(R"(Can't find "{}" setting in "{}")"sv, settings::otherLanguagesSetting, settings::settingsFile));
	}

	return errors;
}

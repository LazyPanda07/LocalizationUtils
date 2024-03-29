#include "Validator.h"

using namespace std;

void Validator::checkError(const string& setting, vector<string>& errors, json::utility::variantTypeEnum type) const
{
	if (!settings.contains(setting, type))
	{
		errors.push_back(format(R"(Can't find "{}" setting in "{}")"sv, setting, global::settingsFile));
	}
}

Validator::Validator(const json::JSONParser& settings) :
	settings(settings)
{

}

vector<string> Validator::validate() const
{
	vector<string> errors;

	this->checkError(settings::originalLanguageSetting, errors);

	this->checkError(settings::otherLanguagesSetting, errors, json::utility::variantTypeEnum::jJSONArray);

	if (global::outputFolder.empty())
	{
		this->checkError(settings::debugOutputFolderSetting, errors);

		this->checkError(settings::releaseOutputFolderSetting, errors);
	}

	return errors;
}

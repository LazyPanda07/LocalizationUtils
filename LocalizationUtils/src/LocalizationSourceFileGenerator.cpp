#include "LocalizationSourceFileGenerator.h"

using namespace std;

void LocalizationSourceFileGenerator::appendCore(ofstream& cppFile, const string& originalLanguage) const
{
	cppFile << utility::convertToUTF8(format(R"(#include <unordered_map>

using namespace std;

#define LOCALIZATION_API extern "C" __declspec(dllexport)

LOCALIZATION_API const string originalLanguage = "{}";
)"sv, originalLanguage));
}

bool LocalizationSourceFileGenerator::appendLanguage(ofstream& cppFile, const json::JSONParser& dictionary, const string& language) const
{
	using namespace utility;

	if (dictionary.getParsedData().data.empty())
	{
		return false;
	}

	string result = convertToUTF8(format(R"(LOCALIZATION_API const unordered_map<string, string> {}Dictionary = 
{}
)"sv, language, '{'));

	for (const auto& i : dictionary)
	{
		const string& key = i->first;
		const string& value = get<string>(i->second);

		result += convertToUTF8("\t{ \"") + key + convertToUTF8("\", \"") + value + convertToUTF8("\" },\n");
	}

	result.pop_back();
	result.pop_back();

	result += convertToUTF8("\n};\n");

	cppFile << result << endl;

	return true;
}

void LocalizationSourceFileGenerator::appendDictionaryWithAllLanguages(ofstream& cppFile, const vector<string>& languages) const
{
	using namespace utility;

	string result = convertToUTF8("LOCALIZATION_API const unordered_map<string, const unordered_map<string, string>*> dictionaries = \n{");

	for (const auto& i : languages)
	{
		result += convertToUTF8("\t{ \"") + i + convertToUTF8("\", &") + i + convertToUTF8("Dictionary") + convertToUTF8(" },\n");
	}

	result.pop_back();
	result.pop_back();

	result += convertToUTF8("\n};\n");

	cppFile << result;
}

LocalizationSourceFileGenerator::LocalizationSourceFileGenerator(const json::JSONParser& settings) :
	settings(settings)
{
	localizationFolder /= global::startFolder;

	localizationFolder /= folders::localizationFolder;
}

void LocalizationSourceFileGenerator::generate() const
{
	vector<string> languages = json::utility::JSONArrayWrapper(settings.getArray(settings::otherLanguagesSetting)).getAsStringArray();
	filesystem::directory_iterator it(localizationFolder);
	filesystem::path intermediateFolder;

	intermediateFolder /= global::startFolder;

	intermediateFolder /= folders::intermediateFolder;

	ofstream cppFile(intermediateFolder / files::generatedCPPFile);

	languages.insert(languages.begin(), settings.getString(settings::originalLanguageSetting));

	this->appendCore(cppFile, settings.getString(settings::originalLanguageSetting));

	for (const auto& i : it)
	{
		string fileName = i.path().string();

		if (fileName.find(files::metaFile) != string::npos)
		{
			continue;
		}

		string language = string(fileName.begin() + fileName.rfind('_') + 1, fileName.begin() + fileName.rfind('.'));

		if (ranges::find(languages, language) == languages.end())
		{
			continue;
		}

		if (!this->appendLanguage(cppFile, json::JSONParser(ifstream(fileName)), language))
		{
			erase(languages, language);
		}
	}

	this->appendDictionaryWithAllLanguages(cppFile, languages);
}

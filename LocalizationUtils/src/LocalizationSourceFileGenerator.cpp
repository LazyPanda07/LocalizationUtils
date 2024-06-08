#include "LocalizationSourceFileGenerator.h"

using namespace std;

void LocalizationSourceFileGenerator::appendCore(ostream& cppFile, const string& originalLanguage)
{
	cppFile << utility::convertToUTF8(format(R"(#include <unordered_map>
#include <string>
#include <string_view>
#include <cstdint>

using namespace std;

#ifdef __LINUX__
#define LOCALIZATION_API extern "C" __attribute__((visibility("default")))
#else
#define LOCALIZATION_API extern "C" __declspec(dllexport)
#endif

constexpr string_view originalLanguage = "{}";

constexpr string_view nullptrSizeError = "size must be valid address";
constexpr string_view nullptrKeysError = "keys must be valid address";
constexpr string_view nullptrValuesError = "values must be valid address";
constexpr string_view noLanguageError = "Wrong language";
)", originalLanguage));
}

bool LocalizationSourceFileGenerator::appendLanguage(ostream& cppFile, const json::JSONParser& dictionary, const string& language)
{
	using namespace utility;

	if (dictionary.getParsedData().data.empty())
	{
		return false;
	}

	string result = convertToUTF8(format(R"(const unordered_map<string, string> {}Dictionary = 
{}
)", language, '{'));

	for (const auto& [key, value] : dictionary)
	{
		result += convertToUTF8("\t{ \"") + key + convertToUTF8("\", \"") + get<string>(value) + convertToUTF8("\" },\n");
	}

	result.pop_back();
	result.pop_back();

	result += convertToUTF8("\n};\n");

	cppFile << result << endl;

	return true;
}

void LocalizationSourceFileGenerator::appendDictionaryWithAllLanguages(ostream& cppFile, const vector<string>& languages)
{
	using namespace utility;

	if (languages.empty())
	{
		return;
	}

	string result = convertToUTF8("const unordered_map<string, const unordered_map<string, string>*> dictionaries = \n{");

	for (const auto& i : languages)
	{
		result += convertToUTF8("\t{ \"") + i + convertToUTF8("\", &") + i + convertToUTF8("Dictionary") + convertToUTF8(" },\n");
	}

	result.pop_back();
	result.pop_back();

	result += convertToUTF8("\n};\n\n");

	cppFile << result;
}

void LocalizationSourceFileGenerator::appendCCFunctions(ostream& cppFile)
{
	cppFile << utility::convertToUTF8(R"(LOCALIZATION_API const char* getLocalizedString(const char* key, const char* language)
{
	auto it = dictionaries.find(language);

	if (it == dictionaries.end())
	{
		return nullptr;
	}

	const unordered_map<string, string>& dictionary = *(it->second);

	auto valueIt = dictionary.find(key);

	if (valueIt == dictionary.end())
	{
		return nullptr;
	}

	return valueIt->second.data();
}

LOCALIZATION_API const char* getOriginalLanguage()
{
	return originalLanguage.data();
}

LOCALIZATION_API bool findLanguage(const char* language)
{
	return dictionaries.contains(language);
}

/**
* @brief Application must call freeDictionariesLanguages to free resources
*/
LOCALIZATION_API const char** getDictionariesLanguages(uint64_t* size)
{
	if (!size)
	{
		return nullptr;
	}

	*size = static_cast<uint64_t>(dictionaries.size());

	const char** result = new const char* [*size];
	auto it = dictionaries.begin();

	for (uint64_t i = 0; i < *size; i++)
	{
		result[i] = it->first.data();

		++it;
	}

	return result;
}

LOCALIZATION_API void freeDictionariesLanguages(const char** data)
{
	delete[] data;
}

/**
* @brief Application must call freeDictionary to free resources
* @return Error description
*/
LOCALIZATION_API const char* getDictionary(const char* language, uint64_t* size, const char*** keys, const char*** values)
{
	if (!size)
	{
		return nullptrSizeError.data();
	}
	
	if (!keys)
	{
		return nullptrKeysError.data();
	}

	if (!values)
	{
		return nullptrValuesError.data();
	}

	auto it = dictionaries.find(language);

	if (it == dictionaries.end())
	{
		return noLanguageError.data();
	}

	const unordered_map<string, string>& dictionary = *(it->second);

	*size = static_cast<uint64_t>(dictionary.size());

	*keys = new const char* [*size];
	*values = new const char* [*size];
	auto valueIt = dictionary.begin();

	for (uint64_t i = 0; i < *size; i++)
	{
		const auto& [key, value] = *valueIt;

		*keys[i] = key.data();
		*values[i] = value.data();

		++valueIt;
	}

	return nullptr;
}

LOCALIZATION_API void freeDictionary(const char** keys, const char** values)
{
	delete[] keys;
	delete[] values;
}
)");
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

	LocalizationSourceFileGenerator::appendCore(cppFile, settings.getString(settings::originalLanguageSetting));

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

		if (!LocalizationSourceFileGenerator::appendLanguage(cppFile, json::JSONParser(ifstream(fileName)), language))
		{
			erase(languages, language);
		}
	}

	LocalizationSourceFileGenerator::appendDictionaryWithAllLanguages(cppFile, languages);

	LocalizationSourceFileGenerator::appendCCFunctions(cppFile);
}

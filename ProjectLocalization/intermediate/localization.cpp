#include <unordered_map>

using namespace std;

#define LOCALIZATION_API extern "C" __declspec(dllexport)

LOCALIZATION_API const string originalLanguage = "en";
LOCALIZATION_API const unordered_map<string, string> enDictionary = 
{
	{ "first", "first" },
	{ "second", "second" }
};

LOCALIZATION_API const unordered_map<string, string> ruDictionary = 
{
	{ "first", "" },
	{ "second", "" }
};

LOCALIZATION_API const unordered_map<string, const unordered_map<string, string>*> dictionaries = 
{	{ "en", &enDictionary },
	{ "ru", &ruDictionary }
};

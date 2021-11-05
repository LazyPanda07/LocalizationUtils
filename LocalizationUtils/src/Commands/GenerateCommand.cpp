#include "GenerateCommand.h"

using namespace std;

namespace commands
{
	void GenerateCommand::start(const filesystem::path& localizationFolder, const string& originalLanguage, const vector<string>& otherLanguages) const
	{
		json::JSONBuilder localizationSettingsBuilder(CP_UTF8);

		filesystem::create_directory(localizationFolder);

		utility::makeLocalizationFile(originalLanguage, localizationFolder);

		for (const auto& i : otherLanguages)
		{
			utility::makeLocalizationFile(i, localizationFolder);
		}
	}

	void GenerateCommand::repeat(const filesystem::path& localizationFolder, const string& originalLanguage, const vector<string>& otherLanguages) const
	{
		filesystem::directory_iterator it(localizationFolder);
		unordered_map<string, string> localizationFiles;
		vector<string> localizationKeys;

		for (const auto& i : it)
		{
			string fileName = i.path().string();
			string language = string(fileName.begin() + fileName.rfind('_') + 1, fileName.begin() + fileName.rfind('.'));

			localizationFiles[move(language)] = move(fileName);
		}

		{
			json::JSONParser tem;

			if (localizationFiles.find(originalLanguage) == localizationFiles.end())
			{
				tem.setJSONData((ostringstream() << ifstream(utility::makeLocalizationFile(originalLanguage, localizationFolder)).rdbuf()).str());
			}
			else
			{
				string pathToOriginalLanguageLocalizationFile = (localizationFolder / global::localizationSettingsFile).string();

				tem.setJSONData((ostringstream() << ifstream(format(pathToOriginalLanguageLocalizationFile, originalLanguage)).rdbuf()).str());
			}

			auto& data = const_cast<vector<pair<string, json::utility::jsonObject::variantType>>&>(tem.getParsedData().data);
			
			localizationKeys.reserve(data.size());

			for (auto& [key, _] : data)
			{
				localizationKeys.push_back(move(key));
			}
		}

		if (otherLanguages.size() && (otherLanguages.size() - 1 < localizationFiles.size()))
		{
			for (const auto& i : otherLanguages)
			{
				if (localizationFiles.find(i) == localizationFiles.end())
				{
					json::JSONBuilder keysFromOriginal(CP_UTF8);

					ranges::for_each(localizationKeys, [&keysFromOriginal](const string& key) { keysFromOriginal[key] = ""s; });

					localizationFiles[i] = utility::makeLocalizationFile(i, localizationFolder, keysFromOriginal).string();
				}
			}
		}
	}

	GenerateCommand::GenerateCommand(const json::JSONParser& settings) :
		ICommand(settings)
	{

	}

	void GenerateCommand::run() const
	{
		const string& originalLanguage = settings.getString(settings::originalLanguageSetting);
		vector<string> otherLanguages = json::utility::JSONArrayWrapper(settings.getArray(settings::otherLanguagesSetting)).getAsStringArray();
		filesystem::path localizationFolder;

		localizationFolder /= global::startFolder;

		localizationFolder /= settings::localizationFolderName;
		
		if (!filesystem::exists(localizationFolder))
		{
			this->start(localizationFolder, originalLanguage, otherLanguages);
		}
		else
		{
			this->repeat(localizationFolder, originalLanguage, otherLanguages);
		}
	}
}

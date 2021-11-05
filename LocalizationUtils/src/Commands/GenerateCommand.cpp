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
		json::JSONParser localizationKeys;

		for (const auto& i : it)
		{
			string fileName = i.path().string();
			string language = string(fileName.begin() + fileName.rfind('_') + 1, fileName.begin() + fileName.rfind('.'));

			localizationFiles[move(language)] = move(fileName);
		}

		if (localizationFiles.find(originalLanguage) == localizationFiles.end())
		{
			localizationKeys.setJSONData((ostringstream() << ifstream(utility::makeLocalizationFile(originalLanguage, localizationFolder)).rdbuf()).str());
		}
		else
		{
			string pathToOriginalLanguageLocalizationFile = (localizationFolder / global::localizationFile).string();

			localizationKeys.setJSONData((ostringstream() << ifstream(format(pathToOriginalLanguageLocalizationFile, originalLanguage)).rdbuf()).str());
		}

		if (otherLanguages.size() && (otherLanguages.size() - 1 < localizationFiles.size()))
		{
			for (const auto& i : otherLanguages)
			{
				if (localizationFiles.find(i) == localizationFiles.end())
				{
					utility::makeLocalizationFile(i, localizationFolder, utility::copyOriginalLanguage(localizationKeys));
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

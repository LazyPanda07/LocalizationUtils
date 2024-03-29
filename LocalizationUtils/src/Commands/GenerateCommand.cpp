#include "GenerateCommand.h"

using namespace std;

namespace commands
{
	void GenerateCommand::start(const filesystem::path& localizationFolder, const string& originalLanguage, const vector<string>& otherLanguages) const
	{
		json::JSONBuilder metaBuilder(CP_UTF8);

		filesystem::create_directory(localizationFolder);

		metaBuilder[originalLanguage] = encoding::SHA256::getHash((ostringstream() << ifstream(utility::makeLocalizationFile(originalLanguage, localizationFolder)).rdbuf()).str());

		for (const string& otherLanguage : otherLanguages)
		{
			metaBuilder[otherLanguage] = encoding::SHA256::getHash((ostringstream() << ifstream(utility::makeLocalizationFile(otherLanguage, localizationFolder)).rdbuf()).str());
		}

		ofstream(localizationFolder / files::metaFile) << metaBuilder;
	}

	json::utility::jsonObject GenerateCommand::updateOtherLanguages(const filesystem::path& localizationFolder, const vector<string>& otherLanguages) const
	{
		json::utility::jsonObject result;
		json::JSONBuilder metaBuilder(json::JSONParser(ifstream(localizationFolder / files::metaFile)).getParsedData(), CP_UTF8);

		for (const string& otherLanguage : otherLanguages)
		{
			if (!metaBuilder.contains(otherLanguage, json::utility::variantTypeEnum::jString))
			{
				metaBuilder[otherLanguage] = encoding::SHA256::getHash((ostringstream() << ifstream(utility::makeLocalizationFile(otherLanguage, localizationFolder)).rdbuf()).str());
			}
		}

		ofstream(localizationFolder / files::metaFile) << metaBuilder;

		metaBuilder.getObject(result);

		return result;
	}

	void GenerateCommand::repeat(const filesystem::path& localizationFolder, const string& originalLanguage, const vector<string>& otherLanguages) const
	{
		unordered_map<string, string> localizationFiles;
		json::JSONParser localizationKeys;
		json::JSONParser metaParser(this->updateOtherLanguages(localizationFolder, otherLanguages));

		for (const auto& it : filesystem::directory_iterator(localizationFolder))
		{
			string fileName = it.path().string();

			if (fileName.find(files::metaFile) != string::npos)
			{
				continue;
			}

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

			localizationKeys.setJSONData((ostringstream() << ifstream(vformat(pathToOriginalLanguageLocalizationFile, make_format_args(originalLanguage))).rdbuf()).str());
		}

		localizationFiles.erase(originalLanguage);

		if (otherLanguages.size() != localizationFiles.size())
		{
			for (const string& otherLanguage : otherLanguages)
			{
				if (localizationFiles.find(otherLanguage) == localizationFiles.end())
				{
					utility::makeLocalizationFile(otherLanguage, localizationFolder, utility::copyOriginalLanguage(localizationKeys));
				}
			}
		}

		const string& previousHash = metaParser.getString(originalLanguage);
		encoding::SHA256 currentHash;
		unordered_map<string, string> updatedHashes;
		json::JSONBuilder updateMetaBuilder(metaParser.getParsedData(), CP_UTF8);

		for (const auto& [key, value] : localizationKeys)
		{
			currentHash.update(key);
		}

		if (currentHash.getHash() != previousHash)
		{
			updateMetaBuilder[originalLanguage] = currentHash.getHash();

			for (const string& otherLanguage : otherLanguages)
			{
				const string& pathToCurrentFile = localizationFiles[otherLanguage];
				json::JSONParser languageParser = ifstream(pathToCurrentFile);
				unordered_set<string> existingKeys;
				unordered_map<const string*, const string*> values;

				existingKeys.reserve(languageParser.getParsedData().data.size());
				values.reserve(languageParser.getParsedData().data.size());

				for (const auto& [keyLanguage, valueLanguage] : languageParser)
				{
					const string& key = *existingKeys.insert(keyLanguage).first;

					values[&key] = &get<string>(valueLanguage);
				}

				json::JSONBuilder updateBuilder = utility::copyOriginalLanguage(localizationKeys, existingKeys);

				for (const auto& [key, value] : values)
				{
					updateBuilder[*key] = *value;
				}

				ofstream(pathToCurrentFile) << updateBuilder;
				updatedHashes[otherLanguage] = encoding::SHA256::getHash((ostringstream() << ifstream(pathToCurrentFile).rdbuf()).str());
			}
		}
		else
		{
			for (const auto& i : otherLanguages)
			{
				string checkHash = encoding::SHA256::getHash((ostringstream() << ifstream(localizationFiles[i]).rdbuf()).str());

				if (metaParser.contains(i, json::utility::variantTypeEnum::jString) && checkHash == metaParser.getString(i))
				{
					continue;
				}

				updatedHashes[i] = move(checkHash);
			}
		}

		for (auto& [language, hash] : updatedHashes)
		{
			updateMetaBuilder[language] = move(hash);
		}

		ofstream(localizationFolder / files::metaFile) << updateMetaBuilder;
	}

	GenerateCommand::GenerateCommand(const json::JSONParser& settings) :
		ICommand(settings)
	{

	}

	void GenerateCommand::run() const
	{
		const string& originalLanguage = settings.getString(settings::originalLanguageSetting);
		vector<string> otherLanguages = json::utility::JSONArrayWrapper(settings.getArray(settings::otherLanguagesSetting)).getAsStringArray();
		filesystem::path localizationFolder = utility::makePath(global::startFolder, folders::localizationFolder);

		if (!filesystem::exists(localizationFolder))
		{
			this->start(localizationFolder, originalLanguage, otherLanguages);
		}
		else
		{
			if (!filesystem::exists(localizationFolder / files::metaFile))
			{
				this->start(localizationFolder, originalLanguage, otherLanguages);
			}

			this->repeat(localizationFolder, originalLanguage, otherLanguages);
		}
	}
}

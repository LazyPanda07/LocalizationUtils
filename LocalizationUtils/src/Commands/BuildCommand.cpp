#include "BuildCommand.h"

#include "LocalizationSourceFileGenerator.h"

using namespace std;

namespace commands
{
	BuildCommand::BuildCommand(const json::JSONParser& settings, const string_view* buildCommand, const string& outputFolder) :
		ICommand(settings),
		buildCommand(buildCommand),
		outputFolder(outputFolder)
	{
		if (outputFolder.size() && (outputFolder.back() != '/' && outputFolder.back() != '\\'))
		{
			this->outputFolder += '\\';
		}
	}

	void BuildCommand::run() const
	{
		filesystem::path pathToBuildTools;
		filesystem::path intermediateFolder;
		filesystem::path pathToMetaFile = filesystem::path(global::startFolder) / filesystem::path(folders::localizationFolder) / files::metaFile;
		json::JSONParser metaParser = ifstream(pathToMetaFile);
		string newBuildHash;
		bool updateHash = false;

		pathToBuildTools /= settings.getString(settings::pathToVisualStudioSetting);

		pathToBuildTools /= build::pathTox64Tools;

		intermediateFolder /= global::startFolder;

		intermediateFolder /= folders::intermediateFolder;

		if (!filesystem::exists(pathToBuildTools))
		{
			throw runtime_error(format(R"(Can't find "{}" file)"sv, pathToBuildTools.string()));
		}

		if (!filesystem::exists(intermediateFolder))
		{
			filesystem::create_directory(intermediateFolder);
		}

		encoding::SHA256 newHash;

		for (const auto& i : metaParser)
		{
			if (i->first == utility::buildHashUtilitySetting)
			{
				continue;
			}

			newHash.update(get<string>(i->second));
		}

		newBuildHash = newHash.getHash();

		if (!metaParser.contains(utility::buildHashUtilitySetting, json::utility::variantTypeEnum::jString) || newBuildHash != metaParser.getString(utility::buildHashUtilitySetting))
		{
			LocalizationSourceFileGenerator generator(settings);

			generator.generate();

			updateHash = true;
		}

		string build = format(*buildCommand, outputFolder);

		filesystem::create_directories(outputFolder);

		system(format(R"(cd "{}" && call "{}" && {})"sv, intermediateFolder.string(), pathToBuildTools.string(), build).data());

		if (updateHash)
		{
			json::JSONBuilder updateBuilder(metaParser.getParsedData(), CP_UTF8);

			updateBuilder[utility::buildHashUtilitySetting] = move(newBuildHash);

			ofstream(pathToMetaFile) << updateBuilder;
		}
	}
}
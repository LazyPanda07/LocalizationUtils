#include "BuildCommand.h"

#include "LocalizationSourceFileGenerator.h"

#pragma warning(disable: 4305)

using namespace std;

namespace commands
{
	void BuildCommand::copyDLL() const
	{
		if (global::outputFolder == outputFolder)
		{
			return;
		}

		if (!filesystem::exists(global::outputFolder))
		{
			filesystem::create_directories(global::outputFolder);
		}

		filesystem::copy
		(
			utility::makePath(outputFolder, string(settings.getString("fileName")).append(".dll")),
			utility::makePath(global::outputFolder, string(settings.getString("fileName")).append(".dll")),
			filesystem::copy_options::overwrite_existing
		);
	}

	BuildCommand::BuildCommand(const json::JSONParser& settings, const string_view& buildCommand, const string& outputFolder) :
		ICommand(settings),
		buildCommand(buildCommand),
		outputFolder(outputFolder)
	{
		utility::appendSlash(this->outputFolder);
	}

	void BuildCommand::run() const
	{
		filesystem::path pathToBuildTools = utility::makePath(settings.getString(settings::pathToVisualStudioSetting), build::pathTox64Tools);
		filesystem::path intermediateFolder = utility::makePath(global::startFolder, folders::intermediateFolder);
		filesystem::path pathToMetaFile = utility::makePath(global::startFolder, folders::localizationFolder, files::metaFile);
		json::JSONParser metaParser = ifstream(pathToMetaFile);
		string newBuildHash;
		bool updateHash = false;

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
		updateHash =
			!metaParser.contains(utility::buildHashUtilitySetting, json::utility::variantTypeEnum::jString) ||
			newBuildHash != metaParser.getString(utility::buildHashUtilitySetting);

		if (!filesystem::exists(intermediateFolder / files::generatedCPPFile) || updateHash)
		{
			LocalizationSourceFileGenerator generator(settings);

			generator.generate();
		}

		if (filesystem::exists(utility::makePath(outputFolder, string(settings.getString("fileName")).append(".dll"))) && !updateHash)
		{
			this->copyDLL();

			return;
		}

		string build = vformat(buildCommand, make_format_args(outputFolder, settings.getString("fileName")));

		filesystem::create_directories(outputFolder);

		system(format(R"(cd "{}" && call "{}" && {})"sv, intermediateFolder.string(), pathToBuildTools.string(), build).data());

		if (global::outputFolder.size() && global::outputFolder != outputFolder)
		{
			this->copyDLL();
		}

		if (updateHash)
		{
			json::JSONBuilder updateBuilder(metaParser.getParsedData(), CP_UTF8);

			updateBuilder[utility::buildHashUtilitySetting] = move(newBuildHash);

			ofstream(pathToMetaFile) << updateBuilder;
		}
	}
}

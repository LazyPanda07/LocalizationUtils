#include "BuildCommand.h"

#include "LocalizationSourceFileGenerator.h"
#include "ReleaseBuildCommand.h"

#pragma warning(disable: 4305)

using namespace std;

namespace commands
{
	BuildCommand::BuildCommand(const json::JSONParser& settings, const string_view& buildCommand, const string& outputFolder) :
		ICommand(settings),
		buildCommand(buildCommand),
		outputFolder(global::outputFolder.empty() ? outputFolder : global::outputFolder)
	{
		utility::appendSlash(this->outputFolder);
	}

	void BuildCommand::run() const
	{
		filesystem::path pathToBuildTools = utility::makePath(settings.getString(settings::pathToVisualStudioSetting), build::pathTox64Tools);
		filesystem::path intermediateFolder = utility::makePath(global::startFolder, folders::intermediateFolder);
		filesystem::path pathToBinariesFolder = utility::makePath(global::startFolder, folders::binariesFolder, (dynamic_cast<const ReleaseBuildCommand*>(this) ? folders::releaseBinariesFolder : folders::debugBinariesFolder));
		filesystem::path pathToMetaFile = utility::makePath(global::startFolder, folders::localizationFolder, files::metaFile);
		string fileName = settings.getString("fileName") + ".dll";
		json::JSONParser metaParser = ifstream(pathToMetaFile);
		string newBuildHash;
		bool isHashUpdated = false;

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
		isHashUpdated =
			!metaParser.contains(utility::buildHashUtilitySetting, json::utility::variantTypeEnum::jString) ||
			newBuildHash != metaParser.getString(utility::buildHashUtilitySetting);

		if (!filesystem::exists(intermediateFolder / files::generatedCPPFile) || isHashUpdated)
		{
			LocalizationSourceFileGenerator(settings).generate();
		}

		filesystem::create_directories(pathToBinariesFolder);

		if (isHashUpdated || !filesystem::exists(pathToBinariesFolder / fileName))
		{
			string build = vformat(buildCommand, make_format_args(pathToBinariesFolder.string() + '\\', settings.getString("fileName")));

			system(format(R"(cd "{}" && call "{}" && {})"sv, intermediateFolder.string(), pathToBuildTools.string(), build).data());
		}

		if (outputFolder.size())
		{
			filesystem::create_directories(outputFolder);

			if (isHashUpdated || !filesystem::exists(utility::makePath(outputFolder, fileName)))
			{
				filesystem::copy
				(
					pathToBinariesFolder / fileName,
					utility::makePath(outputFolder, fileName),
					filesystem::copy_options::overwrite_existing
				);
			}
		}

		if (isHashUpdated)
		{
			json::JSONBuilder updateBuilder(metaParser.getParsedData(), CP_UTF8);

			updateBuilder[utility::buildHashUtilitySetting] = move(newBuildHash);

			ofstream(pathToMetaFile) << updateBuilder;
		}
	}
}

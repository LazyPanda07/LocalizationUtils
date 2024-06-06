#include "BuildCommand.h"

#include "LocalizationSourceFileGenerator.h"
#include "ReleaseBuildCommand.h"
#include "CMakeFileGenerator.h"

#pragma warning(disable: 4305)

using namespace std;

namespace commands
{
	BuildCommand::BuildCommand(const json::JSONParser& settings, const string& outputFolder) :
		ICommand(settings),
		outputFolder(global::outputFolder.empty() ? outputFolder : global::outputFolder)
	{
		utility::appendSlash(this->outputFolder);
	}

	void BuildCommand::run() const
	{
		filesystem::path intermediateFolder = utility::makePath(global::startFolder, folders::intermediateFolder);
		filesystem::path pathToBinariesFolder = utility::makePath(global::startFolder, folders::binariesFolder, (dynamic_cast<const ReleaseBuildCommand*>(this) ? folders::releaseBinariesFolder : folders::debugBinariesFolder));
		filesystem::path pathToMetaFile = utility::makePath(global::startFolder, folders::localizationFolder, files::metaFile);
		CMakeFileGenerator cmakeGenerator(settings);
		string fileName = cmakeGenerator.getResultName();
		json::JSONParser metaParser = ifstream(pathToMetaFile);
		string newBuildHash;
		bool isHashUpdated = false;
		string_view buildType = dynamic_cast<const ReleaseBuildCommand*>(this) ?
			"Release" :
			"Debug";

		if (!filesystem::exists(intermediateFolder))
		{
			filesystem::create_directory(intermediateFolder);
		}

		encoding::SHA256 newHash;

		for (const auto& [key, value] : metaParser)
		{
			if (key == utility::buildHashUtilitySetting)
			{
				continue;
			}

			newHash.update(get<string>(value));
		}

		newBuildHash = newHash.getHash();
		isHashUpdated =
			!metaParser.contains(utility::buildHashUtilitySetting, json::utility::variantTypeEnum::jString) ||
			newBuildHash != metaParser.getString(utility::buildHashUtilitySetting);

		filesystem::create_directories(pathToBinariesFolder);

		if (!filesystem::exists(intermediateFolder / files::generatedCPPFile) || isHashUpdated)
		{
			LocalizationSourceFileGenerator(settings).generate();
		}

		cmakeGenerator.generate(intermediateFolder, pathToBinariesFolder);

		if (isHashUpdated || !filesystem::exists(pathToBinariesFolder / fileName))
		{
			if (filesystem::exists(intermediateFolder / "build"))
			{
				filesystem::remove_all(intermediateFolder / "build");
			}

			string build = format("cmake -DCMAKE_BUILD_TYPE={} {} ..", buildType, cmakeGenerator.getGenerator());

			if (system(format(R"(cd "{}" && mkdir build && cd build && {} && {} install)"sv, intermediateFolder.string(), build, cmakeGenerator.getMake()).data()))
			{
				cerr << "Command error" << endl;

				exit(1);
			}
		}

		if (outputFolder.size())
		{
			filesystem::create_directories(outputFolder);

			filesystem::copy
			(
				pathToBinariesFolder / fileName,
				utility::makePath(outputFolder, fileName),
				filesystem::copy_options::overwrite_existing
			);
		}

		if (isHashUpdated)
		{
			json::JSONBuilder updateBuilder(metaParser.getParsedData(), CP_UTF8);

			updateBuilder[utility::buildHashUtilitySetting] = move(newBuildHash);

			ofstream(pathToMetaFile) << updateBuilder;
		}
	}
}

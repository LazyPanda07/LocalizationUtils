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
		if (outputFolder.back() != '/' && outputFolder.back() != '\\')
		{
			this->outputFolder += '\\';
		}
	}

	void BuildCommand::run() const
	{
		filesystem::path pathToBuildTools;
		filesystem::path intermediateFolder;

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

		LocalizationSourceFileGenerator generator(settings);

		generator.generate();

		string build = format(*buildCommand, outputFolder);

		filesystem::create_directories(outputFolder);

		system(format(R"(cd "{}" && call "{}" && {})"sv, intermediateFolder.string(), pathToBuildTools.string(), build).data());
	}
}

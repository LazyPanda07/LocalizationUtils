#include "BuildCommand.h"

using namespace std;

namespace commands
{
	BuildCommand::BuildCommand(const json::JSONParser& settings, const string_view* buildCommand) :
		ICommand(settings),
		buildCommand(buildCommand)
	{

	}

	void BuildCommand::run() const
	{
		filesystem::path pathToBuildTools;
		filesystem::path intermediateFolder;
		string outputFolder = settings.getString(settings::outputFolderSetting);

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

		if (outputFolder.back() != '/' && outputFolder.back() != '\\')
		{
			outputFolder += '\\';
		}

		ofstream(intermediateFolder / "test.cpp") << "#include <unordered_map>";

		string build = format(*buildCommand, outputFolder);

		filesystem::create_directories(outputFolder);

		system(format(R"(cd "{}" && call "{}" && {})"sv, intermediateFolder.string(), pathToBuildTools.string(), build).data());
	}
}

/*
* Run through console with arguments
* argv[0] - System reserved for path to executable
* argv[1] - Path to folder with localization
* argv[2] - Command
* argv[3] - Path to output folder
*/

#include "headers.h"

#include "Runner.h"
#include "Validator.h"

#ifndef __LINUX__
#pragma comment(lib, "JSON.lib")
#pragma comment(lib, "SHA256.lib")
#endif // !__LINUX__

using namespace std;

int main(int argc, char** argv)
{
	if (argc <= 2)
	{
		cout << "No commands provided" << endl;

		return -1;
	}
	else if (argc == 4)
	{
		global::outputFolder = argv[3];
	}

	global::startFolder = filesystem::absolute(argv[1]).string();

	if (!filesystem::exists(global::startFolder))
	{
		filesystem::create_directories(global::startFolder);
	}

	json::JSONParser settings = ifstream(utility::generateSettingsFile());

	Validator validator(settings);

	vector<string> errors = validator.validate();

	if (errors.size())
	{
		for (const auto& i : errors)
		{
			cout << i << endl;
		}

		return -2;
	}

	Runner runner(settings);

	try
	{
		runner.run(argv[2]);
	}
	catch (const bad_variant_access&)
	{
		cout << format(R"("{}" only accepts string values)"sv, settings::otherLanguagesSetting) << endl;

		return -3;
	}
	catch (const out_of_range&)
	{
		cout << format(R"(Undefined command "{}")"sv, argv[1]) << endl;

		return -4;
	}
	catch (const runtime_error& e)
	{
		cout << e.what() << endl;

		return -5;
	}

	return 0;
}

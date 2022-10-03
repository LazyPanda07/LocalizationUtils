#include "headers.h"

#include "Runner.h"
#include "Validator.h"

#pragma comment(lib, "JSON.lib")
#pragma comment(lib, "SHA256.lib")

using namespace std;

int main(int argc, char** argv)
{
	if (argc <= 2)
	{
		if (argc == 2 && (argv[1] == commands_names::debug_build_command || argv[1] == commands_names::release_build_command))
		{
			goto start;
		}

		cout << "No commands provided" << endl;

		system("pause");

		return -1;
	}
	else if (argc == 4)
	{
		global::outputFolder = argv[3];
	}

start:
	global::startFolder = argv[1];

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

		system("pause");

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

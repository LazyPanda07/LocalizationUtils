#include "headers.h"

#include "Runner.h"
#include "Validator.h"

#include "Commands/GenerateSettingsFile.h"

#pragma comment(lib, "JSON.lib")
#pragma comment(lib, "SHA256.lib")

using namespace std;

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cout << "No commands provided" << endl;

		system("pause");

		return -1;
	}

	if (argc > 2)
	{
		if (filesystem::is_directory(argv[2]))
		{
			string_view tem = argv[2];

			if (tem.back() == '\\' || tem.back() == '/')
			{
				global::settingsFile = string(tem).append(global::settingsFile);
			}
			else
			{
				global::settingsFile = string(tem).append("\\").append(global::settingsFile);
			}
		}
		else
		{
			global::settingsFile = argv[2];
		}	
	}

	if (argc == 4)
	{
		global::outputFolder = argv[3];
	}

	if (string_view(argv[0]).find('\\') != string_view::npos)
	{
		global::startFolder = string(argv[0], string_view(argv[0]).rfind('\\'));
	}
	else
	{
		global::startFolder = filesystem::current_path().string();
	}

	if (argv[1] == commands_names::generate_settings_file_command)
	{
		commands::GenerateSettingsFile(json::JSONParser()).run();

		return 0;
	}

	if (!filesystem::exists(global::settingsFile))
	{
		cout << format(R"(Can't find "{}" file)"sv, global::settingsFile) << endl;

		system("pause");

		return -2;
	}

	json::JSONParser settings = ifstream(global::settingsFile);

	Validator validator(settings);

	vector<string> errors = validator.validate();

	if (errors.size())
	{
		for (const auto& i : errors)
		{
			cout << i << endl;
		}

		system("pause");

		return -3;
	}

	Runner runner(settings);

	try
	{
		runner.run(argv[1]);
	}
	catch (const bad_variant_access&)
	{
		cout << format(R"("{}" only accepts string values)"sv, settings::otherLanguagesSetting) << endl;

		return -4;
	}
	catch (const out_of_range&)
	{
		cout << format(R"(Undefined command "{}")"sv, argv[1]) << endl;

		return -5;
	}
	catch (const runtime_error& e)
	{
		cout << e.what() << endl;

		return -6;
	}

	return 0;
}

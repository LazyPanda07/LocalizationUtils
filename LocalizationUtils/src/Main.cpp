#include "headers.h"

#include "Runner.h"
#include "Validator.h"

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

	global::startFolder = string(argv[0], string_view(argv[0]).rfind('\\'));

	if (!filesystem::exists(settings::settingsFile))
	{
		cout << format(R"(Can't find "{}" file)"sv, settings::settingsFile) << endl;

		system("pause");

		return -1;
	}

	json::JSONParser settings = ifstream(settings::settingsFile);

	Validator validator(settings);

	vector<string> errors = validator.validate();

	if (errors.size())
	{
		for (const auto& i : errors)
		{
			cout << i << endl;
		}

		system("pause");

		return -1;
	}

	Runner runner(settings);

	try
	{
		runner.run(argv[1]);
	}
	catch (const bad_variant_access&)
	{
		cout << format(R"("{}" only accepts string values)"sv, settings::otherLanguagesSetting) << endl;

		return -1;
	}

	return 0;
}

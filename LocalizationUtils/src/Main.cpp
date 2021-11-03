#include "headers.h"

#pragma comment(lib, "JSON.lib")
#pragma comment(lib, "SHA256.lib")

using namespace std;

int main(int argc, char** argv)
{
	if (!filesystem::exists(settings::settingsFile))
	{
		cout << format(R"(Can't find "{}" file)"sv, settings::settingsFile) << endl;

		system("pause");

		return -1;
	}

	json::JSONParser settings = ifstream(settings::settingsFile);



	return 0;
}

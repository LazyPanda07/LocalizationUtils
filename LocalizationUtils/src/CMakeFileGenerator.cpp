#include "CMakeFileGenerator.h"

#include <algorithm>

using namespace std;

CMakeFileGenerator::CMakeFileGenerator(const json::JSONParser& settings) :
	settings(settings)
{

}

void CMakeFileGenerator::generate(const filesystem::path& pathToIntermediate, const filesystem::path& pathToBinaryDir) const
{
	ofstream result(pathToIntermediate / "CMakeLists.txt");
	string binaryPath = pathToBinaryDir.string();

	replace(binaryPath.begin(), binaryPath.end(), '\\', '/');

	result << "cmake_minimum_required(VERSION 3.27.0)" << endl << endl;

	result << "set(CMAKE_CXX_STANDARD 20)" << endl;
	result << format(R"(set(CMAKE_INSTALL_PREFIX {} CACHE STRING ""))", binaryPath) << endl << endl;

	result << R"(if (UNIX)
	add_definitions(-D__LINUX__)
endif (UNIX))" << endl << endl;

	result << format("project({})", settings.getString("fileName")) << endl << endl;

	result << "add_library(${PROJECT_NAME} SHARED localization.cpp)" << endl << endl;

	result << "install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})" << endl;
}

string CMakeFileGenerator::getResultName() const
{
	const string& fileName = settings.getString("fileName");

#ifdef __LINUX__
	return "lib" + fileName + ".so";
#else
	return fileName + ".dll";
#endif // __LINUX__
}

string CMakeFileGenerator::getGenerator() const
{
#ifdef __LINUX__
	return "";
#else
	return R"(-G "NMake Makefiles")";
#endif // __LINUX__
}

string CMakeFileGenerator::getMake() const
{
#ifdef __LINUX__
	return "make";
#else
	return "nmake";
#endif // __LINUX__
}

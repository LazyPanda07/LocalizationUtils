cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install-dir)

if (UNIX)
	add_definitions(-D__LINUX__)
endif (UNIX)

project(LocalizationUtils)

add_executable(
	${PROJECT_NAME}
	LocalizationUtils/src/LocalizationSourceFileGenerator.cpp
	LocalizationUtils/src/CMakeFileGenerator.cpp
	LocalizationUtils/src/Main.cpp
	LocalizationUtils/src/Runner.cpp
	LocalizationUtils/src/Utility.cpp
	LocalizationUtils/src/Validator.cpp
	LocalizationUtils/src/Commands/BuildCommand.cpp
	LocalizationUtils/src/Commands/DebugBuildCommand.cpp
	LocalizationUtils/src/Commands/GenerateCommand.cpp
	LocalizationUtils/src/Commands/ICommand.cpp
	LocalizationUtils/src/Commands/ReleaseBuildCommand.cpp
)

add_subdirectory(JSON)
add_subdirectory(SHA256)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	LocalizationUtils/src/
	JSON/src/
	SHA256/src/
)

target_link_libraries(
	${PROJECT_NAME}
	JSON
	SHA256
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES LocalizationUtils/localization_utils_settings.json DESTINATION ${CMAKE_INSTALL_PREFIX})

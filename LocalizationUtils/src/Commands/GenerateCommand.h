#pragma once

#include "ICommand.h"

namespace commands
{
	class GenerateCommand : public interfaces::ICommand
	{
	private:
		void start(const std::filesystem::path& localizationFolder, const std::string& mainLanguage, const std::vector<std::string>& otherLanguages) const;

		void repeat(const std::filesystem::path& localizationFolder, const std::string& mainLanguage, const std::vector<std::string>& otherLanguages) const;

	public:
		GenerateCommand(const json::JSONParser& settings);

		void run() const override;

		~GenerateCommand() = default;
	};
}

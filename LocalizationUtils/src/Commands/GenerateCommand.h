#pragma once

#include "ICommand.h"

namespace commands
{
	class GenerateCommand : public interfaces::ICommand
	{
	private:
		void start(const std::filesystem::path& localizationFolder, const std::string& originalLanguage, const std::vector<std::string>& otherLanguages) const;

		json::utility::jsonObject updateOtherLanguages(const std::filesystem::path& localizationFolder, const std::vector<std::string>& otherLanguages) const;

		void repeat(const std::filesystem::path& localizationFolder, const std::string& originalLanguage, const std::vector<std::string>& otherLanguages) const;

	public:
		GenerateCommand(const json::JSONParser& settings);

		void run() const override;

		~GenerateCommand() = default;
	};
}

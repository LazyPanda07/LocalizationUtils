import platform
import subprocess
import json
import unittest
import os


class Test(unittest.TestCase):
    def test_functionality(self):
        executable_path = "../"
        working_dir = "../"
        arm = os.get_env("MARCH", "")

        if platform.system() == "Windows":
            executable_path += "Windows/LocalizationUtils.exe"
            working_dir += "Windows"
        elif arm == "armv8-a":
            executable_path += "qemu-aarch64 LinuxARM/LocalizationUtils"
            working_dir += "LinuxARM"
        else:
            executable_path += "Linux/LocalizationUtils"
            working_dir += "Linux"

        with open(f"{working_dir}/localization_utils_settings.json", "r") as file:
            settings = json.load(file)

        settings["otherLanguages"] = ["ru"]

        with open(f"{working_dir}/localization_utils_settings.json", "w") as file:
            file.write(json.dumps(settings))

        self.assertFalse(subprocess.run([executable_path, ".", "generate"], cwd=working_dir).returncode)

        en = {
            "first": "First",
            "second": "Second"
        }

        with open(f"{working_dir}/localization/localization_en.json", "w") as file:
            file.write(json.dumps(en))

        self.assertFalse(subprocess.run([executable_path, ".", "generate"], cwd=working_dir).returncode)

        with open(f"{working_dir}/localization/localization_ru.json", "r") as file:
            ru = json.load(file)

        for key in en.keys():
            self.assertTrue(key in ru)

        ru["first"] = "Первый"
        ru["second"] = "Второй"

        with open(f"{working_dir}/localization/localization_ru.json", "w", encoding="utf-8") as file:
            file.write(json.dumps(ru, ensure_ascii=False))

        self.assertFalse(subprocess.run([executable_path, ".", "generate"], cwd=working_dir).returncode)

        self.assertFalse(
            subprocess.run([executable_path, ".", "release_build", "release_bin"], cwd=working_dir).returncode
        )

        self.assertFalse(
            subprocess.run([executable_path, ".", "debug_build", "debug_bin"], cwd=working_dir).returncode
        )


if __name__ == '__main__':
    unittest.main()

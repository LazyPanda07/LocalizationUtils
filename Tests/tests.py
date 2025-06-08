import platform
import subprocess
import json
import unittest
import os
from typing import List

arm = os.getenv("MARCH", "")


def run_process(args: List[str], working_dir: str) -> int:
    global arm

    if arm == "armv8-a":
        temp_args: List = ["qemu-aarch64"]

        for arg in args:
            temp_args.append(arg)

        return subprocess.run(args=temp_args, cwd=working_dir).returncode
    else:
        return subprocess.run(args=args, cwd=working_dir).returncode


class Test(unittest.TestCase):
    def test_functionality(self):
        executable_path = "../"
        working_dir = "../"

        if platform.system() == "Windows":
            executable_path += "Windows/LocalizationUtils.exe"
            working_dir += "Windows"
        elif arm == "armv8-a":
            executable_path += "LinuxARM/LocalizationUtils"
            working_dir += "LinuxARM"
        else:
            executable_path += "Linux/LocalizationUtils"
            working_dir += "Linux"

        with open(f"{working_dir}/localization_utils_settings.json", "r") as file:
            settings = json.load(file)

        settings["otherLanguages"] = ["ru"]

        with open(f"{working_dir}/localization_utils_settings.json", "w") as file:
            file.write(json.dumps(settings))

        self.assertFalse(run_process([executable_path, ".", "generate"], working_dir))

        en = {
            "first": "First",
            "second": "Second"
        }

        with open(f"{working_dir}/localization/localization_en.json", "w") as file:
            file.write(json.dumps(en))

        self.assertFalse(run_process([executable_path, ".", "generate"], working_dir))

        with open(f"{working_dir}/localization/localization_ru.json", "r") as file:
            ru = json.load(file)

        for key in en.keys():
            self.assertTrue(key in ru)

        ru["first"] = "Первый"
        ru["second"] = "Второй"

        with open(f"{working_dir}/localization/localization_ru.json", "w", encoding="utf-8") as file:
            file.write(json.dumps(ru, ensure_ascii=False))

        self.assertFalse(run_process([executable_path, ".", "generate"], working_dir))
        self.assertFalse(run_process([executable_path, ".", "release_build", "release_bin"], working_dir))
        self.assertFalse(run_process([executable_path, ".", "debug_build", "debug_bin"], working_dir))


if __name__ == '__main__':
    unittest.main()

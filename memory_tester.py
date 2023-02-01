# %%
from pathlib import Path
import subprocess
import os
TESTS_TO_RUN = ["reverse", "utilities/wcat", "utilities/wgrep", "utilities/wunzip", "utilities/wzip"]
#TESTS_TO_RUN = ["reverse"]
# %%

current_dir = os.getcwd()
print(f"now: {os.getcwd()}")

for path_to_test in TESTS_TO_RUN:
    test_path = Path(current_dir, path_to_test).absolute()
    os.chdir(test_path)
    print(f"cwd: {os.getcwd()}")
    for test in test_path.glob("tests/*.run"):
        with open(test, "r") as file:
            test_name = path_to_test.replace("utilities/", "")
            command = file.read()
            command = command.replace(f"./{test_name}", f"valgrind ./{test_name}")
            print(command, end="")
            p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            o, e = p.communicate()
            # print(o.decode())
            if (e.decode().find("All heap blocks were freed -- no leaks are possible") == -1):
                print(f"{test} failed valgrind test")
# %%

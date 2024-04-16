
import os, sys

arguments = sys.argv[1:]
if len(arguments) == 0:
    exit(1)
code = arguments[0]

with open("main.c", "r") as f:
    original = f.read()
    f.close()

new = original.replace("inject", code)

with open("main.c", "w") as f:
    f.write(new)
    f.close()

os.system("make compiled")

with open("main.c", "w") as f:
    f.write(original)
    f.close()


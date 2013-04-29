import os
import subprocess as sp

for currpath, dirs, files in os.walk('.'):
	for subdir in dirs:
		currfile = subdir + "\sources.mk"
		subprocess.call(["echo", "sources = ", ">", currfile])
		
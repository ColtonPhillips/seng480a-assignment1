import os
import subprocess

fnull = open(os.devnull, 'w')

for jfile in os.listdir("."):
	if jfile.endswith(".j"):
		test_result = subprocess.call(["jasmin", jfile])

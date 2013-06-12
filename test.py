import os
import subprocess

fnull = open(os.devnull, 'w')

for jfile in os.listdir("jvm-files"):
	if jfile.endswith(".j") and "Test" in jfile:
		test_name = jfile[:-2]
		test_result = subprocess.call(["./MyJVM", "jvm-files/"+test_name], stdout=fnull)
		if test_result is not 1:
			print(test_name + " failed")

fnull.close()

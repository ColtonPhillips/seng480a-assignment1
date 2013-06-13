# J Files with "Test" in the title are asserted to throw an error code of 1
# These files are made to break, during the verification process.

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

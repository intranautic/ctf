# Automated Pin tool

Pin tool for automatically solving the password

# Build

Download Intel Pin for linux from https://www.intel.com/content/www/us/en/developer/articles/tool/pin-a-binary-instrumentation-tool-downloads.html

This tool was tested against Pin 3.28, which is the newest version as of this writing.
It will likely still work with future releases, but this is the known good version to use.

Copy this entire 'solver' folder to 'path_to_pin/source/tools/solver'
cd to that directory then type 'make' to build the tool

# Usage

From inside the 'path_to_pin/source/tools/solver' directory, run the following:
```
../../../pin -t obj-intel64/solver.so -o password.bin -- ./encrypted_box
```

This will begin analyzing the target and writing the solved password to password.bin

After it finishes, the password file can be tested against the target with:
```
cat password.bin | ./encrypted_box
```

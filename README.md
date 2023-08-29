## Features:
- works with any C string: functions will take a pointer to string, size, and capacity.
- in-place string replacement
- in-place regex string replacement
- reverse standard string functions: strrstr, strrspn, etc.
## Installation:
```
sudo ./install
```
and
```
#include <jstr/jstr.h>
```
## Function suffixes:
- _p(): returns the new pointer to the end of the string.
- _f(): assumes that there be enough space in the string: it will not grow it with realloc.
- _now(): compiles regular expression now.
## Scripts:
- install: builds and copies all the header files to /usr/local/jstr (requires sudo)
- uninstall: removes /usr/local/jstr (requires sudo)
- build: generates additional functions
- clean: removes files generated by ./build
- fmt: formats code

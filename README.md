# jstring

## Features:
- works with any C string type: functions will take a pointer to string, size, and capacity<br>
- in-place literal string and regex replacement: supports backreferences and searching from a specified index<br>
- reverse standard string functions: memrchr(), strrstr(), strrspn(), strrcspn()<br>
- standard string functions for non-nul terminated strings: memspn(), memcspn(), mempbrk()<br>
- extensions to standard string functions: strnstr(), strnchr(), strcasestr\_len()<br>
- string formatting: asprintf() and vsprintfstrlen() which returns the number of characters vsprintf will write excluding the nul<br>

## Installation:
```
./build && sudo ./install
#include <jstr/jstr.h>
```
or
```
./build && sudo ./install-to /path/to/some/dir
#include "/path/to/some/dir/jstr.h"
```
Some headers may need to be explicitly included since they rely on POSIX.<br>

## Disclaimer:
Some of the code are licensed under LGPL. You can define JSTR\_USE\_LGPL as 0 or use ./remove-lgpl to exclude the LGPL code.<br>
- jstr\_ty, jstrl\_ty must be initialized with zeros.<br>
```
jstr_ty j = JSTR_INIT;
```

## Error handling:
JSTR\_ERR is returned as error. The programmer is expected to check the return value of every function call.<br>
When a memory error is encountered, all related resources will be freed by the function in which that error occured.<br>
Use jstr\_err\(\) to print the error message or jstr\_err\_exit() to also exit.<br>
When debugging, you may want to define JSTR\_PANIC as 1 to automatically call jstr\_err\_exit() on errors.<br>

## Function affixes: 
- \*\_mem\*(): the string need not be nul-terminated<br>
- \*\_len(): take the length of the string<br>
- \*str\*\_len(): the string must be nul-termimated. The size is only used to save a strlen()<br>
- \*stp\*(), \*\_p(), \*P(): return a pointer to the end of the string<br>
- \*\_unsafe(): assume that there be enough space in the string: the string will not be grown<br>
- \*\_from(): operate starting from a specified index<br>
The index passed by the user is assumed to be not out of bounds.<br>
- \*\_bref(): enable backreferences for the replacement string.<br>
\\0 is interpreted as referring to the whole matched string<br>
- [Pp]namespace\_\*(): private functions or macros used internally<br>

## Scripts:
- build: generate functions<br>
- install: build and copy headers to /usr/local/include/jstr (requires sudo)<br>
- install-to: install to a specified directory<br>
- uninstall: remove /usr/local/include/jstr (requires sudo)<br>
- clean: remove files generated by build<br>
- fmt: format files<br>
- remove-lgpl: remove LGPL code from ./jstr. Use this before ./install and after ./build.<br>
For example,
```
./build && ./remove-lgpl && sudo ./install
```

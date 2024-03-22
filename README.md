# jstring

jstring is a dynamic string C library which aims to make string handling simple and efficient.

## Features:

- compatible with any string type: functions take a pointer to string, size, and capacity.
- in-place literal string and regex replacement (with backreference replacement). Literal
replaceall is O\(n\) and does zero memory allocation if the capacity of the string is
larger than the length of the string plus the modified string.
- reverse standard string functions: memrchr\(\), strrstr\(\), strrspn\(\), strrcspn\(\),
etc.
- standard string functions for non-nul terminated strings: memspn\(\), memcspn\(\),
mempbrk\(\), etc.
- extensions to standard string functions: strnchr\(\), strcasestr\_len\(\).
- string formatting: asprintf\(\) and vsprintfstrlen\(\), which returns the maximum
number of characters to store the characters written by vsprintf including the
nul terminator.

## Installation:

```
./build && sudo ./install
#include <jstr/jstr.h>
```

or

```
./build && ./install-to /path/to/some/dir
#include "/path/to/some/dir/jstr.h"
```

The following headers will need to be explicitly included (as they rely on POSIX):

```
jstr-io.h
jstr-regex.h
```

## Examples:

https://github.com/IAKOBVS/find-and-replace

## Disclaimer:

You must zero-initialize a jstr\_ty string.
```
jstr_ty j = JSTR_INIT;
```

jstr\_tolower\(\) and jstr\_toupper\(\) will not handle EOF correctly. The tolower of
EOF may not equal EOF.

Do not pass a string with embedded NULs to a function that takes a char \*
parameter. char \* implies no embedded NULs. Use ones which takes a void \*.

## Configuration:

You can customize the library by defining certain macros in jstr-config.h before
including any header. For example:
```
#define JSTR_USE_UNLOCKED_IO 1
#include "jstr.h"
```

## Error handling:

A negative number is returned as error. The programmer is expected to check the return value
and handle the error.

When a memory error is encountered, the user is expected to free
all the related resources. Use jstr\_err\(\) to print the error message or jstr\_errdie\(\)
to also exit. When debugging, you may want to define JSTR\_PANIC as 1 to automatically
call jstr\_errdie\(\) on errors.

For jstr-regex.h, jstrre\_rm\*\(\), jstrre\_rplc\*\(\) will return a negative number indicating
the negated value of the regex error code. To print an error message, pass the negation of
the returned error code. This is done so that we can utilize the return value of the function
both as the number of replacements done or as error values.

## Naming conventions

### Functions or macros:
- \*\_mem\*\(\): the string need not be nul-terminated.
- \*\_len\(\): take the length of the string.
- \*str\*\_len\(\): the string passed to this function must be nul-termimated.
The size parameter is only used to potentially save a strlen\(\).
- \*stp\*\(\), \*\_p\(\), \*P\(\): return a pointer to the end of the string.
- \*\_unsafe\(\): assume that there be enough space in the string: the string will
not be grown.
- \*\_from\(\): operate starting from a specified index. The index passed by the user
is assumed to be within bounds.
- \*\_bref\(\): enable backreferences for the replacement string. \0 is interpreted
as referring to the whole matched string.
- \*\_at\(\): return an element of from an array type. When JSTR\_DEBUG is defined as
1, do bounds checking at runtime.
- \*\_chk\*\(\): evaluate to true if the value passed is an error value.
- \*\_err\(\): print an error message.
- \*\_errdie\(\): print an error message and exit\(\).
- namespace\_\_\*\(\): internal functions.
### Headers:
- \_\*.h: internal headers.

## Scripts:

- build: generate functions.
- test: run tests.
- install: build and copy headers to /usr/local/include/jstr \(requires sudo\).
- install-to: install to a specified directory.
- uninstall: remove /usr/local/include/jstr \(requires sudo\).
- clean: remove files generated by build.
- update: update the repository and build.
- fmt: format files.

## Contributing:

Do not put blank lines inside a function. The perl script splits each block of
code by blank lines. If you need to put blank lines, add a comment. For example:
```
void f()
{
	Some code...
	/* */
	Some other code...
}
```

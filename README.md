# Assert

Implementation of a flexible assert as described in [the blog article](https://www.dominikgrabiec.com/c++/2023/02/28/making_a_flexible_assert.html). Currently works in Windows using MSVC (Visual Studio 2022 and above), not tested extensively in other compilers (just run through Compiler Explorer).

Note that this implementation isn't quite ready to use straight from the repository. The main customisation point is the `print_fatal_message` function in `Assert.cpp` which should be changed to suit your needs.
* A call to flush your logging system should be added to the top of the function so that your logs are complete when the program exits.
* If using in Windows then it needs to include the Windows header file, either `#include <windows.h>` to include the default Windows header file, or using the header file from [this repository](https://github.com/DominikGrabiec/WindowsHeader)). 
* If using it on another operating system then the function will need to remove the Windows specific stuff and put in equivalent functions for your OS.
* This is also the place where you can integrate these asserts into another wider error handling system.

By default asserts will be enabled in both Debug and Release mode, but can be disabled by removing the `#define ASSERTS_ENABLED` line and instead conditionally defining it elsewhere (like in a build system or config header).

To use the new/conformant MSVC preprocessor just add `/Zc:preprocessor` to your compiler options.

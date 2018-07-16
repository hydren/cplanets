# Building

#### Prerequisites:
* C++ compiler (MinGW, Linux GCC, MSVC10 tested)
* *SDL*, *SDL_image*, *SDL_ttf* and *SDL_gfx* **development** libraries (SDL 1.2 tested)

#### Building
Assuming you are on Linux, you can build it using 'make':

* Install `make` if not installed.
* Open a terminal, go to the `build/` folder.
* Execute the configure script: `./configure.sh`
* If the previous script ended ok, build using the makefile: `make`

> When running into problems using older versions of SDL_gfx, try uncommenting in the file `src_libs/SDL_widgets/config.h`, the following line: `#define arcColor pieColor`

##### Testing the executable
Run the program it by executing: `./build/cplanets`
> The `data/` folder must be in the same folder as the executable to work.

#### Eclipse project
You can also use eclipse to build the project (tested to work with MinGW and Linux GCC). Here is how:

##### Prerequisites for eclipse:
- Get the Eclipse IDE for C/C++ Developers. Link: https://www.eclipse.org/downloads/packages/
- When using MinGW, make sure the g++ program is on the OS path variable. On Windows you need to add the /bin directory to your PATH variable.

##### Instructions:
- Clone the code from [this](https://gitlab.com/hydren/cplanets.git) repository (make sure to use the `--recursive` options)
- If you simply downloaded, you'll need to download the submodules as well ([SDL_widgets](https://gitlab.com/hydren/SDL_widgets) and [futil](https://gitlab.com/hydren/futil))
- Import the project using the [Import Projects wizard](http://help.eclipse.org/kepler/index.jsp?topic=%2Forg.eclipse.platform.doc.user%2Ftasks%2Ftasks-importproject.htm).
- If you want to clone from Eclipse, select workspace and on the projects pane left-click and select `Import -> Projects from git`. Choose `Clone URI`. On the URI field, paste the repository adress https://gitlab.com/hydren/cplanets.git. Click next and the choose `master` and next. Wait for the download and then select `Import existing projects` and then next and finish.
- Left-click project and select build configuration according to your environment (i.e. `linux-gcc-release` for *Linux*, `mingw-release` for *Windows*) and then build.
- Any problems indicates that something probably went wrong with previous steps (missing/wrongly installed SDL libraries, missing/wrongly installed submodules)
- If something wents wrong because of some cross-gcc stuff, install the `C/C++ GCC Cross Compiler Support` plugin on Eclipse via `Install new software` option on Eclipse.

#####Additional steps when compiling with MSVC compiler:
- Uncomment the flag `DISABLE_FUTIL_SNPRINTF_IMPLEMENTATION` in the file `src_libs/futil/config.h`.
- `unistd.h` and `dirent.h` must be available either in the MSVC's `include/` folder or in the build include paths. The following are recommended:
  * `unistd.h` [partial implementation](https://gist.github.com/mbikovitsky/39224cf521bfea7eabe9)*
  * `dirent.h` for [Windows](https://github.com/tronkko/dirent)

> *The `unistd.h` appointed by the link (at the time of this writing) requires `getopt.h`. Either comment the #include "getopt.h" statement or adventure yourself to compile it, available here: [Full getopt Port for Visual C++ at CodeProject](http://www.codeproject.com/Articles/157001/Full-getopt-Port-for-Unicode-and-Multibyte-Microso).
> Also, this version of the header contains SDL-conflicting typedefs (used to emulate some types (int8_t, etc) available in sys/types.h) which can also be easily commented out ;)

##### Testing the executable:
- Run the program by left clicking on the executable (created inside the build folder - usually `/linux-gcc-release/`, `/mingw-release/`, etc) and selecting `Run as application`. 
- Alternatively, open the project folder using your file explorer and run directly the executable inside the build folder. Note that this way requires that all **required DLLs** be present **on the build folder or in the system's path**. The required DLLs are included within the release packages.

#### Experimental VS2010 project:
There is also a experimental VS2010 project available. 

##### Instructions:
- Clone the code from [this](https://gitlab.com/hydren/cplanets.git) repository (make sure to use the `--recursive` options)
- If you simply downloaded, you'll need to download the submodules as well ([SDL_widgets](https://gitlab.com/hydren/SDL_widgets) and [futil](https://gitlab.com/hydren/futil))
- Open VS2010 and load the project file `cplanets.vcxproj`.
- Select `Release` configuration and then build. 

##### Testing the executable
- Run the program by clicking on the run button.
> Note that this way also requires that all **required DLLs** be present **on the build folder or in the system’s path**. The required DLLs are included within the release packages.

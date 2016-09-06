# Building

Prerequisites:
* C++ compiler (MinGW, Linux GCC tested)
* SDL, SDL_image, SDL_ttf, SDL_gfx development libraries (SDL 1.2 tested)

Assuming you are on Linux, you can build it using 'make':

* Install 'make' if not installed.
* Open a terminal, go to the build/ folder.
* Execute the configure script: ./configure.sh
* If the previous script ended ok, build using the makefile: make
* Run it by executing: ./cplanets

Known problems:
* In windows builds, a flag must be uncommented in the file `src_libs/futil/flags.h`: `#define FUTIL_DISABLE_BYTE_TYPE`
* When running into problems using older versions of SDL, try uncommenting in the file `src_libs/SDL_widgets/config.h`, the following line: `#define INCLUDE_FOR_OLDER_SDL`
* When running into problems using older versions of SDL_gfx, try uncommenting in the file `src_libs/SDL_widgets/config.h`, the following line: `#define arcColor pieColor`

You can also use eclipse and build the project (tested to work with MinGW and Linux GCC). Here is how:

Prerequisites for eclipse:
- Get the Eclipse IDE for C/C++ Developers. Link: https://www.eclipse.org/downloads/packages/
- When using gcc/MinGW, make sure the g++ program is on the OS path variable. On linux this is almost always true, but on Windows you need to add the /bin directory to your PATH variable.

Instructions:
- Clone the code from [this](https://github.com/hydren/cplanets.git) repository (make sure to use the --recursive options)
- If you simply downloaded, you'll need to download the submodules as well ([SDL_widgets](https://github.com/hydren/SDL_widgets) and [futil](https://github.com/hydren/futil))
- Import the project using the [Import Projects wizard](http://help.eclipse.org/kepler/index.jsp?topic=%2Forg.eclipse.platform.doc.user%2Ftasks%2Ftasks-importproject.htm).
- If you want to clone from Eclipse, select workspace and on the projects pane left-click and select Import -> Projects from git. Choose URI. On the URI field, paste the github adress https://github.com/hydren/cplanets.git. Click next and the choose 'master' and next. Wait for the download and then select "import existing projects" and then next and finish.
- Left-click project and select build configuration according to your environment (i.e. linux-release for Linux, windows-release for Windows).
- Any problems indicates that something probably went wrong with previous steps (missing/wrongly installed SDL libraries, missing/wrongly installed submodules)
- If something wents wrong because of some cross-gcc stuff, install the "C/C++ GCC Cross Compiler Support" plugin on Eclipse via "Install new software" functionality on Eclipse.

Running:
- Run the program left clicking on the executable (created inside the build folder - usually "/linux-release/", "/windows-release/", etc) and selecting run as application. 
- Alternatively, open the project folder using your file explorer and run directly the executable inside the build folder.


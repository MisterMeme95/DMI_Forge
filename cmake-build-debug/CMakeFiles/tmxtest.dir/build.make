# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2023.2.2\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2023.2.2\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\jonat\GitHub\DMI_Forge

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/tmxtest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tmxtest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tmxtest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tmxtest.dir/flags.make

CMakeFiles/tmxtest.dir/tmx.cpp.obj: CMakeFiles/tmxtest.dir/flags.make
CMakeFiles/tmxtest.dir/tmx.cpp.obj: CMakeFiles/tmxtest.dir/includes_CXX.rsp
CMakeFiles/tmxtest.dir/tmx.cpp.obj: C:/Users/jonat/GitHub/DMI_Forge/tmx.cpp
CMakeFiles/tmxtest.dir/tmx.cpp.obj: CMakeFiles/tmxtest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tmxtest.dir/tmx.cpp.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tmxtest.dir/tmx.cpp.obj -MF CMakeFiles\tmxtest.dir\tmx.cpp.obj.d -o CMakeFiles\tmxtest.dir\tmx.cpp.obj -c C:\Users\jonat\GitHub\DMI_Forge\tmx.cpp

CMakeFiles/tmxtest.dir/tmx.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tmxtest.dir/tmx.cpp.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\tmx.cpp > CMakeFiles\tmxtest.dir\tmx.cpp.i

CMakeFiles/tmxtest.dir/tmx.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tmxtest.dir/tmx.cpp.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\tmx.cpp -o CMakeFiles\tmxtest.dir\tmx.cpp.s

# Object files for target tmxtest
tmxtest_OBJECTS = \
"CMakeFiles/tmxtest.dir/tmx.cpp.obj"

# External object files for target tmxtest
tmxtest_EXTERNAL_OBJECTS =

tmxtest.exe: CMakeFiles/tmxtest.dir/tmx.cpp.obj
tmxtest.exe: CMakeFiles/tmxtest.dir/build.make
tmxtest.exe: C:/Users/jonat/.conan2/p/b/tmxli4b63b68a2f810/p/lib/libtmxlite-s-d.a
tmxtest.exe: C:/Users/jonat/.conan2/p/b/zstdb6f4fab44e769/p/lib/libzstd.a
tmxtest.exe: C:/Users/jonat/.conan2/p/b/pugix2f8df93edd2b6/p/lib/libpugixml.a
tmxtest.exe: C:/Users/jonat/.conan2/p/b/zlib6f01ae82dbf0d/p/lib/libz.a
tmxtest.exe: CMakeFiles/tmxtest.dir/linkLibs.rsp
tmxtest.exe: CMakeFiles/tmxtest.dir/objects1.rsp
tmxtest.exe: CMakeFiles/tmxtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tmxtest.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\tmxtest.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tmxtest.dir/build: tmxtest.exe
.PHONY : CMakeFiles/tmxtest.dir/build

CMakeFiles/tmxtest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\tmxtest.dir\cmake_clean.cmake
.PHONY : CMakeFiles/tmxtest.dir/clean

CMakeFiles/tmxtest.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\jonat\GitHub\DMI_Forge C:\Users\jonat\GitHub\DMI_Forge C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles\tmxtest.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tmxtest.dir/depend

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
include png/CMakeFiles/pngtest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include png/CMakeFiles/pngtest.dir/compiler_depend.make

# Include the progress variables for this target.
include png/CMakeFiles/pngtest.dir/progress.make

# Include the compile flags for this target's objects.
include png/CMakeFiles/pngtest.dir/flags.make

png/CMakeFiles/pngtest.dir/pngtest.c.obj: png/CMakeFiles/pngtest.dir/flags.make
png/CMakeFiles/pngtest.dir/pngtest.c.obj: png/CMakeFiles/pngtest.dir/includes_C.rsp
png/CMakeFiles/pngtest.dir/pngtest.c.obj: C:/Users/jonat/GitHub/DMI_Forge/png/pngtest.c
png/CMakeFiles/pngtest.dir/pngtest.c.obj: png/CMakeFiles/pngtest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object png/CMakeFiles/pngtest.dir/pngtest.c.obj"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT png/CMakeFiles/pngtest.dir/pngtest.c.obj -MF CMakeFiles\pngtest.dir\pngtest.c.obj.d -o CMakeFiles\pngtest.dir\pngtest.c.obj -c C:\Users\jonat\GitHub\DMI_Forge\png\pngtest.c

png/CMakeFiles/pngtest.dir/pngtest.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pngtest.dir/pngtest.c.i"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\png\pngtest.c > CMakeFiles\pngtest.dir\pngtest.c.i

png/CMakeFiles/pngtest.dir/pngtest.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pngtest.dir/pngtest.c.s"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\png\pngtest.c -o CMakeFiles\pngtest.dir\pngtest.c.s

# Object files for target pngtest
pngtest_OBJECTS = \
"CMakeFiles/pngtest.dir/pngtest.c.obj"

# External object files for target pngtest
pngtest_EXTERNAL_OBJECTS =

png/pngtest.exe: png/CMakeFiles/pngtest.dir/pngtest.c.obj
png/pngtest.exe: png/CMakeFiles/pngtest.dir/build.make
png/pngtest.exe: png/libpng16d.dll.a
png/pngtest.exe: C:/MinGW/msys/1.0/lib/libz.dll.a
png/pngtest.exe: png/CMakeFiles/pngtest.dir/linkLibs.rsp
png/pngtest.exe: png/CMakeFiles/pngtest.dir/objects1.rsp
png/pngtest.exe: png/CMakeFiles/pngtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable pngtest.exe"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\pngtest.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
png/CMakeFiles/pngtest.dir/build: png/pngtest.exe
.PHONY : png/CMakeFiles/pngtest.dir/build

png/CMakeFiles/pngtest.dir/clean:
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && $(CMAKE_COMMAND) -P CMakeFiles\pngtest.dir\cmake_clean.cmake
.PHONY : png/CMakeFiles/pngtest.dir/clean

png/CMakeFiles/pngtest.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\jonat\GitHub\DMI_Forge C:\Users\jonat\GitHub\DMI_Forge\png C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png\CMakeFiles\pngtest.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : png/CMakeFiles/pngtest.dir/depend


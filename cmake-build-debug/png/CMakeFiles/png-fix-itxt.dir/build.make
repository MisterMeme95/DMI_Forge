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
include png/CMakeFiles/png-fix-itxt.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include png/CMakeFiles/png-fix-itxt.dir/compiler_depend.make

# Include the progress variables for this target.
include png/CMakeFiles/png-fix-itxt.dir/progress.make

# Include the compile flags for this target's objects.
include png/CMakeFiles/png-fix-itxt.dir/flags.make

png/CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.obj: png/CMakeFiles/png-fix-itxt.dir/flags.make
png/CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.obj: png/CMakeFiles/png-fix-itxt.dir/includes_C.rsp
png/CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.obj: C:/Users/jonat/GitHub/DMI_Forge/png/contrib/tools/png-fix-itxt.c
png/CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.obj: png/CMakeFiles/png-fix-itxt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object png/CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.obj"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT png/CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.obj -MF CMakeFiles\png-fix-itxt.dir\contrib\tools\png-fix-itxt.c.obj.d -o CMakeFiles\png-fix-itxt.dir\contrib\tools\png-fix-itxt.c.obj -c C:\Users\jonat\GitHub\DMI_Forge\png\contrib\tools\png-fix-itxt.c

png/CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.i"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\png\contrib\tools\png-fix-itxt.c > CMakeFiles\png-fix-itxt.dir\contrib\tools\png-fix-itxt.c.i

png/CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.s"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\png\contrib\tools\png-fix-itxt.c -o CMakeFiles\png-fix-itxt.dir\contrib\tools\png-fix-itxt.c.s

# Object files for target png-fix-itxt
png__fix__itxt_OBJECTS = \
"CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.obj"

# External object files for target png-fix-itxt
png__fix__itxt_EXTERNAL_OBJECTS =

png/png-fix-itxt.exe: png/CMakeFiles/png-fix-itxt.dir/contrib/tools/png-fix-itxt.c.obj
png/png-fix-itxt.exe: png/CMakeFiles/png-fix-itxt.dir/build.make
png/png-fix-itxt.exe: C:/MinGW/msys/1.0/lib/libz.dll.a
png/png-fix-itxt.exe: png/CMakeFiles/png-fix-itxt.dir/linkLibs.rsp
png/png-fix-itxt.exe: png/CMakeFiles/png-fix-itxt.dir/objects1.rsp
png/png-fix-itxt.exe: png/CMakeFiles/png-fix-itxt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable png-fix-itxt.exe"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\png-fix-itxt.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
png/CMakeFiles/png-fix-itxt.dir/build: png/png-fix-itxt.exe
.PHONY : png/CMakeFiles/png-fix-itxt.dir/build

png/CMakeFiles/png-fix-itxt.dir/clean:
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && $(CMAKE_COMMAND) -P CMakeFiles\png-fix-itxt.dir\cmake_clean.cmake
.PHONY : png/CMakeFiles/png-fix-itxt.dir/clean

png/CMakeFiles/png-fix-itxt.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\jonat\GitHub\DMI_Forge C:\Users\jonat\GitHub\DMI_Forge\png C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png\CMakeFiles\png-fix-itxt.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : png/CMakeFiles/png-fix-itxt.dir/depend


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

# Utility rule file for pnglibconf_out.

# Include any custom commands dependencies for this target.
include png/CMakeFiles/pnglibconf_out.dir/compiler_depend.make

# Include the progress variables for this target.
include png/CMakeFiles/pnglibconf_out.dir/progress.make

png/CMakeFiles/pnglibconf_out: png/pnglibconf.out

png/pnglibconf.out: png/pnglibconf.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating pnglibconf.out"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && "C:\Program Files\JetBrains\CLion 2023.2.2\bin\cmake\win\x64\bin\cmake.exe" -DINPUT=C:/Users/jonat/GitHub/DMI_Forge/cmake-build-debug/png/pnglibconf.c -DOUTPUT=C:/Users/jonat/GitHub/DMI_Forge/cmake-build-debug/png/pnglibconf.out -P C:/Users/jonat/GitHub/DMI_Forge/cmake-build-debug/png/scripts/genout.cmake

png/pnglibconf.c: C:/Users/jonat/GitHub/DMI_Forge/png/scripts/pnglibconf.dfa
png/pnglibconf.c: png/scripts/options.awk
png/pnglibconf.c: C:/Users/jonat/GitHub/DMI_Forge/png/pngconf.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating pnglibconf.c"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && "C:\Program Files\JetBrains\CLion 2023.2.2\bin\cmake\win\x64\bin\cmake.exe" -DOUTPUT=pnglibconf.c -P C:/Users/jonat/GitHub/DMI_Forge/cmake-build-debug/png/scripts/gensrc.cmake

pnglibconf_out: png/CMakeFiles/pnglibconf_out
pnglibconf_out: png/pnglibconf.c
pnglibconf_out: png/pnglibconf.out
pnglibconf_out: png/CMakeFiles/pnglibconf_out.dir/build.make
.PHONY : pnglibconf_out

# Rule to build all files generated by this target.
png/CMakeFiles/pnglibconf_out.dir/build: pnglibconf_out
.PHONY : png/CMakeFiles/pnglibconf_out.dir/build

png/CMakeFiles/pnglibconf_out.dir/clean:
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && $(CMAKE_COMMAND) -P CMakeFiles\pnglibconf_out.dir\cmake_clean.cmake
.PHONY : png/CMakeFiles/pnglibconf_out.dir/clean

png/CMakeFiles/pnglibconf_out.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\jonat\GitHub\DMI_Forge C:\Users\jonat\GitHub\DMI_Forge\png C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png\CMakeFiles\pnglibconf_out.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : png/CMakeFiles/pnglibconf_out.dir/depend


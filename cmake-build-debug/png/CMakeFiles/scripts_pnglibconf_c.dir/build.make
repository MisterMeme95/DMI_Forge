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

# Utility rule file for scripts_pnglibconf_c.

# Include any custom commands dependencies for this target.
include png/CMakeFiles/scripts_pnglibconf_c.dir/compiler_depend.make

# Include the progress variables for this target.
include png/CMakeFiles/scripts_pnglibconf_c.dir/progress.make

png/CMakeFiles/scripts_pnglibconf_c: png/scripts/pnglibconf.c

png/scripts/pnglibconf.c: C:/Users/jonat/GitHub/DMI_Forge/png/scripts/pnglibconf.dfa
png/scripts/pnglibconf.c: png/scripts/options.awk
png/scripts/pnglibconf.c: C:/Users/jonat/GitHub/DMI_Forge/png/pngconf.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating scripts/pnglibconf.c"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && "C:\Program Files\JetBrains\CLion 2023.2.2\bin\cmake\win\x64\bin\cmake.exe" -DOUTPUT=scripts/pnglibconf.c -P C:/Users/jonat/GitHub/DMI_Forge/cmake-build-debug/png/scripts/gensrc.cmake

scripts_pnglibconf_c: png/CMakeFiles/scripts_pnglibconf_c
scripts_pnglibconf_c: png/scripts/pnglibconf.c
scripts_pnglibconf_c: png/CMakeFiles/scripts_pnglibconf_c.dir/build.make
.PHONY : scripts_pnglibconf_c

# Rule to build all files generated by this target.
png/CMakeFiles/scripts_pnglibconf_c.dir/build: scripts_pnglibconf_c
.PHONY : png/CMakeFiles/scripts_pnglibconf_c.dir/build

png/CMakeFiles/scripts_pnglibconf_c.dir/clean:
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png && $(CMAKE_COMMAND) -P CMakeFiles\scripts_pnglibconf_c.dir\cmake_clean.cmake
.PHONY : png/CMakeFiles/scripts_pnglibconf_c.dir/clean

png/CMakeFiles/scripts_pnglibconf_c.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\jonat\GitHub\DMI_Forge C:\Users\jonat\GitHub\DMI_Forge\png C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\png\CMakeFiles\scripts_pnglibconf_c.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : png/CMakeFiles/scripts_pnglibconf_c.dir/depend

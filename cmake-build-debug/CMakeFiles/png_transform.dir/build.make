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
include CMakeFiles/png_transform.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/png_transform.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/png_transform.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/png_transform.dir/flags.make

CMakeFiles/png_transform.dir/png_transform.c.obj: CMakeFiles/png_transform.dir/flags.make
CMakeFiles/png_transform.dir/png_transform.c.obj: CMakeFiles/png_transform.dir/includes_C.rsp
CMakeFiles/png_transform.dir/png_transform.c.obj: C:/Users/jonat/GitHub/DMI_Forge/png_transform.c
CMakeFiles/png_transform.dir/png_transform.c.obj: CMakeFiles/png_transform.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/png_transform.dir/png_transform.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/png_transform.dir/png_transform.c.obj -MF CMakeFiles\png_transform.dir\png_transform.c.obj.d -o CMakeFiles\png_transform.dir\png_transform.c.obj -c C:\Users\jonat\GitHub\DMI_Forge\png_transform.c

CMakeFiles/png_transform.dir/png_transform.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/png_transform.dir/png_transform.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\png_transform.c > CMakeFiles\png_transform.dir\png_transform.c.i

CMakeFiles/png_transform.dir/png_transform.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/png_transform.dir/png_transform.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\png_transform.c -o CMakeFiles\png_transform.dir\png_transform.c.s

CMakeFiles/png_transform.dir/PixelManip.c.obj: CMakeFiles/png_transform.dir/flags.make
CMakeFiles/png_transform.dir/PixelManip.c.obj: CMakeFiles/png_transform.dir/includes_C.rsp
CMakeFiles/png_transform.dir/PixelManip.c.obj: C:/Users/jonat/GitHub/DMI_Forge/PixelManip.c
CMakeFiles/png_transform.dir/PixelManip.c.obj: CMakeFiles/png_transform.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/png_transform.dir/PixelManip.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/png_transform.dir/PixelManip.c.obj -MF CMakeFiles\png_transform.dir\PixelManip.c.obj.d -o CMakeFiles\png_transform.dir\PixelManip.c.obj -c C:\Users\jonat\GitHub\DMI_Forge\PixelManip.c

CMakeFiles/png_transform.dir/PixelManip.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/png_transform.dir/PixelManip.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\PixelManip.c > CMakeFiles\png_transform.dir\PixelManip.c.i

CMakeFiles/png_transform.dir/PixelManip.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/png_transform.dir/PixelManip.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\PixelManip.c -o CMakeFiles\png_transform.dir\PixelManip.c.s

CMakeFiles/png_transform.dir/data_structure.c.obj: CMakeFiles/png_transform.dir/flags.make
CMakeFiles/png_transform.dir/data_structure.c.obj: CMakeFiles/png_transform.dir/includes_C.rsp
CMakeFiles/png_transform.dir/data_structure.c.obj: C:/Users/jonat/GitHub/DMI_Forge/data_structure.c
CMakeFiles/png_transform.dir/data_structure.c.obj: CMakeFiles/png_transform.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/png_transform.dir/data_structure.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/png_transform.dir/data_structure.c.obj -MF CMakeFiles\png_transform.dir\data_structure.c.obj.d -o CMakeFiles\png_transform.dir\data_structure.c.obj -c C:\Users\jonat\GitHub\DMI_Forge\data_structure.c

CMakeFiles/png_transform.dir/data_structure.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/png_transform.dir/data_structure.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\data_structure.c > CMakeFiles\png_transform.dir\data_structure.c.i

CMakeFiles/png_transform.dir/data_structure.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/png_transform.dir/data_structure.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\data_structure.c -o CMakeFiles\png_transform.dir\data_structure.c.s

# Object files for target png_transform
png_transform_OBJECTS = \
"CMakeFiles/png_transform.dir/png_transform.c.obj" \
"CMakeFiles/png_transform.dir/PixelManip.c.obj" \
"CMakeFiles/png_transform.dir/data_structure.c.obj"

# External object files for target png_transform
png_transform_EXTERNAL_OBJECTS =

png_transform.exe: CMakeFiles/png_transform.dir/png_transform.c.obj
png_transform.exe: CMakeFiles/png_transform.dir/PixelManip.c.obj
png_transform.exe: CMakeFiles/png_transform.dir/data_structure.c.obj
png_transform.exe: CMakeFiles/png_transform.dir/build.make
png_transform.exe: C:/Users/jonat/.conan2/p/b/libpnda601e8f63f19/p/lib/libpng16d.a
png_transform.exe: C:/Users/jonat/.conan2/p/b/zlib6f01ae82dbf0d/p/lib/libz.a
png_transform.exe: CMakeFiles/png_transform.dir/linkLibs.rsp
png_transform.exe: CMakeFiles/png_transform.dir/objects1.rsp
png_transform.exe: CMakeFiles/png_transform.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable png_transform.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\png_transform.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/png_transform.dir/build: png_transform.exe
.PHONY : CMakeFiles/png_transform.dir/build

CMakeFiles/png_transform.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\png_transform.dir\cmake_clean.cmake
.PHONY : CMakeFiles/png_transform.dir/clean

CMakeFiles/png_transform.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\jonat\GitHub\DMI_Forge C:\Users\jonat\GitHub\DMI_Forge C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles\png_transform.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/png_transform.dir/depend

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
include Test_Cases/CMakeFiles/image_convert.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Test_Cases/CMakeFiles/image_convert.dir/compiler_depend.make

# Include the progress variables for this target.
include Test_Cases/CMakeFiles/image_convert.dir/progress.make

# Include the compile flags for this target's objects.
include Test_Cases/CMakeFiles/image_convert.dir/flags.make

Test_Cases/CMakeFiles/image_convert.dir/Image_Converter.c.obj: Test_Cases/CMakeFiles/image_convert.dir/flags.make
Test_Cases/CMakeFiles/image_convert.dir/Image_Converter.c.obj: Test_Cases/CMakeFiles/image_convert.dir/includes_C.rsp
Test_Cases/CMakeFiles/image_convert.dir/Image_Converter.c.obj: C:/Users/jonat/GitHub/DMI_Forge/Test_Cases/Image_Converter.c
Test_Cases/CMakeFiles/image_convert.dir/Image_Converter.c.obj: Test_Cases/CMakeFiles/image_convert.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object Test_Cases/CMakeFiles/image_convert.dir/Image_Converter.c.obj"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT Test_Cases/CMakeFiles/image_convert.dir/Image_Converter.c.obj -MF CMakeFiles\image_convert.dir\Image_Converter.c.obj.d -o CMakeFiles\image_convert.dir\Image_Converter.c.obj -c C:\Users\jonat\GitHub\DMI_Forge\Test_Cases\Image_Converter.c

Test_Cases/CMakeFiles/image_convert.dir/Image_Converter.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/image_convert.dir/Image_Converter.c.i"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\Test_Cases\Image_Converter.c > CMakeFiles\image_convert.dir\Image_Converter.c.i

Test_Cases/CMakeFiles/image_convert.dir/Image_Converter.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/image_convert.dir/Image_Converter.c.s"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\Test_Cases\Image_Converter.c -o CMakeFiles\image_convert.dir\Image_Converter.c.s

Test_Cases/CMakeFiles/image_convert.dir/__/DMI_Struct.c.obj: Test_Cases/CMakeFiles/image_convert.dir/flags.make
Test_Cases/CMakeFiles/image_convert.dir/__/DMI_Struct.c.obj: Test_Cases/CMakeFiles/image_convert.dir/includes_C.rsp
Test_Cases/CMakeFiles/image_convert.dir/__/DMI_Struct.c.obj: C:/Users/jonat/GitHub/DMI_Forge/DMI_Struct.c
Test_Cases/CMakeFiles/image_convert.dir/__/DMI_Struct.c.obj: Test_Cases/CMakeFiles/image_convert.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object Test_Cases/CMakeFiles/image_convert.dir/__/DMI_Struct.c.obj"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT Test_Cases/CMakeFiles/image_convert.dir/__/DMI_Struct.c.obj -MF CMakeFiles\image_convert.dir\__\DMI_Struct.c.obj.d -o CMakeFiles\image_convert.dir\__\DMI_Struct.c.obj -c C:\Users\jonat\GitHub\DMI_Forge\DMI_Struct.c

Test_Cases/CMakeFiles/image_convert.dir/__/DMI_Struct.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/image_convert.dir/__/DMI_Struct.c.i"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\DMI_Struct.c > CMakeFiles\image_convert.dir\__\DMI_Struct.c.i

Test_Cases/CMakeFiles/image_convert.dir/__/DMI_Struct.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/image_convert.dir/__/DMI_Struct.c.s"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\DMI_Struct.c -o CMakeFiles\image_convert.dir\__\DMI_Struct.c.s

Test_Cases/CMakeFiles/image_convert.dir/__/DMIParse.c.obj: Test_Cases/CMakeFiles/image_convert.dir/flags.make
Test_Cases/CMakeFiles/image_convert.dir/__/DMIParse.c.obj: Test_Cases/CMakeFiles/image_convert.dir/includes_C.rsp
Test_Cases/CMakeFiles/image_convert.dir/__/DMIParse.c.obj: C:/Users/jonat/GitHub/DMI_Forge/DMIParse.c
Test_Cases/CMakeFiles/image_convert.dir/__/DMIParse.c.obj: Test_Cases/CMakeFiles/image_convert.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object Test_Cases/CMakeFiles/image_convert.dir/__/DMIParse.c.obj"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT Test_Cases/CMakeFiles/image_convert.dir/__/DMIParse.c.obj -MF CMakeFiles\image_convert.dir\__\DMIParse.c.obj.d -o CMakeFiles\image_convert.dir\__\DMIParse.c.obj -c C:\Users\jonat\GitHub\DMI_Forge\DMIParse.c

Test_Cases/CMakeFiles/image_convert.dir/__/DMIParse.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/image_convert.dir/__/DMIParse.c.i"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\DMIParse.c > CMakeFiles\image_convert.dir\__\DMIParse.c.i

Test_Cases/CMakeFiles/image_convert.dir/__/DMIParse.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/image_convert.dir/__/DMIParse.c.s"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\DMIParse.c -o CMakeFiles\image_convert.dir\__\DMIParse.c.s

Test_Cases/CMakeFiles/image_convert.dir/__/Icon_State.c.obj: Test_Cases/CMakeFiles/image_convert.dir/flags.make
Test_Cases/CMakeFiles/image_convert.dir/__/Icon_State.c.obj: Test_Cases/CMakeFiles/image_convert.dir/includes_C.rsp
Test_Cases/CMakeFiles/image_convert.dir/__/Icon_State.c.obj: C:/Users/jonat/GitHub/DMI_Forge/Icon_State.c
Test_Cases/CMakeFiles/image_convert.dir/__/Icon_State.c.obj: Test_Cases/CMakeFiles/image_convert.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object Test_Cases/CMakeFiles/image_convert.dir/__/Icon_State.c.obj"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT Test_Cases/CMakeFiles/image_convert.dir/__/Icon_State.c.obj -MF CMakeFiles\image_convert.dir\__\Icon_State.c.obj.d -o CMakeFiles\image_convert.dir\__\Icon_State.c.obj -c C:\Users\jonat\GitHub\DMI_Forge\Icon_State.c

Test_Cases/CMakeFiles/image_convert.dir/__/Icon_State.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/image_convert.dir/__/Icon_State.c.i"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\Icon_State.c > CMakeFiles\image_convert.dir\__\Icon_State.c.i

Test_Cases/CMakeFiles/image_convert.dir/__/Icon_State.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/image_convert.dir/__/Icon_State.c.s"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\Icon_State.c -o CMakeFiles\image_convert.dir\__\Icon_State.c.s

Test_Cases/CMakeFiles/image_convert.dir/__/PixelManip.c.obj: Test_Cases/CMakeFiles/image_convert.dir/flags.make
Test_Cases/CMakeFiles/image_convert.dir/__/PixelManip.c.obj: Test_Cases/CMakeFiles/image_convert.dir/includes_C.rsp
Test_Cases/CMakeFiles/image_convert.dir/__/PixelManip.c.obj: C:/Users/jonat/GitHub/DMI_Forge/PixelManip.c
Test_Cases/CMakeFiles/image_convert.dir/__/PixelManip.c.obj: Test_Cases/CMakeFiles/image_convert.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object Test_Cases/CMakeFiles/image_convert.dir/__/PixelManip.c.obj"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT Test_Cases/CMakeFiles/image_convert.dir/__/PixelManip.c.obj -MF CMakeFiles\image_convert.dir\__\PixelManip.c.obj.d -o CMakeFiles\image_convert.dir\__\PixelManip.c.obj -c C:\Users\jonat\GitHub\DMI_Forge\PixelManip.c

Test_Cases/CMakeFiles/image_convert.dir/__/PixelManip.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/image_convert.dir/__/PixelManip.c.i"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\PixelManip.c > CMakeFiles\image_convert.dir\__\PixelManip.c.i

Test_Cases/CMakeFiles/image_convert.dir/__/PixelManip.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/image_convert.dir/__/PixelManip.c.s"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\PixelManip.c -o CMakeFiles\image_convert.dir\__\PixelManip.c.s

Test_Cases/CMakeFiles/image_convert.dir/__/data_structure.c.obj: Test_Cases/CMakeFiles/image_convert.dir/flags.make
Test_Cases/CMakeFiles/image_convert.dir/__/data_structure.c.obj: Test_Cases/CMakeFiles/image_convert.dir/includes_C.rsp
Test_Cases/CMakeFiles/image_convert.dir/__/data_structure.c.obj: C:/Users/jonat/GitHub/DMI_Forge/data_structure.c
Test_Cases/CMakeFiles/image_convert.dir/__/data_structure.c.obj: Test_Cases/CMakeFiles/image_convert.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object Test_Cases/CMakeFiles/image_convert.dir/__/data_structure.c.obj"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT Test_Cases/CMakeFiles/image_convert.dir/__/data_structure.c.obj -MF CMakeFiles\image_convert.dir\__\data_structure.c.obj.d -o CMakeFiles\image_convert.dir\__\data_structure.c.obj -c C:\Users\jonat\GitHub\DMI_Forge\data_structure.c

Test_Cases/CMakeFiles/image_convert.dir/__/data_structure.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/image_convert.dir/__/data_structure.c.i"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\jonat\GitHub\DMI_Forge\data_structure.c > CMakeFiles\image_convert.dir\__\data_structure.c.i

Test_Cases/CMakeFiles/image_convert.dir/__/data_structure.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/image_convert.dir/__/data_structure.c.s"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\jonat\GitHub\DMI_Forge\data_structure.c -o CMakeFiles\image_convert.dir\__\data_structure.c.s

# Object files for target image_convert
image_convert_OBJECTS = \
"CMakeFiles/image_convert.dir/Image_Converter.c.obj" \
"CMakeFiles/image_convert.dir/__/DMI_Struct.c.obj" \
"CMakeFiles/image_convert.dir/__/DMIParse.c.obj" \
"CMakeFiles/image_convert.dir/__/Icon_State.c.obj" \
"CMakeFiles/image_convert.dir/__/PixelManip.c.obj" \
"CMakeFiles/image_convert.dir/__/data_structure.c.obj"

# External object files for target image_convert
image_convert_EXTERNAL_OBJECTS =

Test_Cases/image_convert.exe: Test_Cases/CMakeFiles/image_convert.dir/Image_Converter.c.obj
Test_Cases/image_convert.exe: Test_Cases/CMakeFiles/image_convert.dir/__/DMI_Struct.c.obj
Test_Cases/image_convert.exe: Test_Cases/CMakeFiles/image_convert.dir/__/DMIParse.c.obj
Test_Cases/image_convert.exe: Test_Cases/CMakeFiles/image_convert.dir/__/Icon_State.c.obj
Test_Cases/image_convert.exe: Test_Cases/CMakeFiles/image_convert.dir/__/PixelManip.c.obj
Test_Cases/image_convert.exe: Test_Cases/CMakeFiles/image_convert.dir/__/data_structure.c.obj
Test_Cases/image_convert.exe: Test_Cases/CMakeFiles/image_convert.dir/build.make
Test_Cases/image_convert.exe: C:/Users/jonat/.conan2/p/b/libpnda601e8f63f19/p/lib/libpng16d.a
Test_Cases/image_convert.exe: C:/Users/jonat/.conan2/p/b/zlib6f01ae82dbf0d/p/lib/libz.a
Test_Cases/image_convert.exe: Test_Cases/CMakeFiles/image_convert.dir/linkLibs.rsp
Test_Cases/image_convert.exe: Test_Cases/CMakeFiles/image_convert.dir/objects1.rsp
Test_Cases/image_convert.exe: Test_Cases/CMakeFiles/image_convert.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking C executable image_convert.exe"
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\image_convert.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Test_Cases/CMakeFiles/image_convert.dir/build: Test_Cases/image_convert.exe
.PHONY : Test_Cases/CMakeFiles/image_convert.dir/build

Test_Cases/CMakeFiles/image_convert.dir/clean:
	cd /d C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases && $(CMAKE_COMMAND) -P CMakeFiles\image_convert.dir\cmake_clean.cmake
.PHONY : Test_Cases/CMakeFiles/image_convert.dir/clean

Test_Cases/CMakeFiles/image_convert.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\jonat\GitHub\DMI_Forge C:\Users\jonat\GitHub\DMI_Forge\Test_Cases C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Test_Cases\CMakeFiles\image_convert.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : Test_Cases/CMakeFiles/image_convert.dir/depend


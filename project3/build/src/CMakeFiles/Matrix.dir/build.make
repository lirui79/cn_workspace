# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\cm_workspace\project3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\cm_workspace\project3\build

# Include any dependencies generated for this target.
include src/CMakeFiles/Matrix.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/Matrix.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/Matrix.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/Matrix.dir/flags.make

src/CMakeFiles/Matrix.dir/matrix.c.obj: src/CMakeFiles/Matrix.dir/flags.make
src/CMakeFiles/Matrix.dir/matrix.c.obj: src/CMakeFiles/Matrix.dir/includes_C.rsp
src/CMakeFiles/Matrix.dir/matrix.c.obj: ../src/matrix.c
src/CMakeFiles/Matrix.dir/matrix.c.obj: src/CMakeFiles/Matrix.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\cm_workspace\project3\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/Matrix.dir/matrix.c.obj"
	cd /d D:\cm_workspace\project3\build\src && C:\PROGRA~1\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/Matrix.dir/matrix.c.obj -MF CMakeFiles\Matrix.dir\matrix.c.obj.d -o CMakeFiles\Matrix.dir\matrix.c.obj -c D:\cm_workspace\project3\src\matrix.c

src/CMakeFiles/Matrix.dir/matrix.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Matrix.dir/matrix.c.i"
	cd /d D:\cm_workspace\project3\build\src && C:\PROGRA~1\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\cm_workspace\project3\src\matrix.c > CMakeFiles\Matrix.dir\matrix.c.i

src/CMakeFiles/Matrix.dir/matrix.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Matrix.dir/matrix.c.s"
	cd /d D:\cm_workspace\project3\build\src && C:\PROGRA~1\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\cm_workspace\project3\src\matrix.c -o CMakeFiles\Matrix.dir\matrix.c.s

# Object files for target Matrix
Matrix_OBJECTS = \
"CMakeFiles/Matrix.dir/matrix.c.obj"

# External object files for target Matrix
Matrix_EXTERNAL_OBJECTS =

src/libMatrix.a: src/CMakeFiles/Matrix.dir/matrix.c.obj
src/libMatrix.a: src/CMakeFiles/Matrix.dir/build.make
src/libMatrix.a: src/CMakeFiles/Matrix.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\cm_workspace\project3\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libMatrix.a"
	cd /d D:\cm_workspace\project3\build\src && $(CMAKE_COMMAND) -P CMakeFiles\Matrix.dir\cmake_clean_target.cmake
	cd /d D:\cm_workspace\project3\build\src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Matrix.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/Matrix.dir/build: src/libMatrix.a
.PHONY : src/CMakeFiles/Matrix.dir/build

src/CMakeFiles/Matrix.dir/clean:
	cd /d D:\cm_workspace\project3\build\src && $(CMAKE_COMMAND) -P CMakeFiles\Matrix.dir\cmake_clean.cmake
.PHONY : src/CMakeFiles/Matrix.dir/clean

src/CMakeFiles/Matrix.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\cm_workspace\project3 D:\cm_workspace\project3\src D:\cm_workspace\project3\build D:\cm_workspace\project3\build\src D:\cm_workspace\project3\build\src\CMakeFiles\Matrix.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/Matrix.dir/depend


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
CMAKE_SOURCE_DIR = D:\cm_workspace\project5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\cm_workspace\project5\bulid

# Include any dependencies generated for this target.
include CMakeFiles/avltree.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/avltree.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/avltree.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/avltree.dir/flags.make

CMakeFiles/avltree.dir/avltree.cpp.obj: CMakeFiles/avltree.dir/flags.make
CMakeFiles/avltree.dir/avltree.cpp.obj: ../avltree.cpp
CMakeFiles/avltree.dir/avltree.cpp.obj: CMakeFiles/avltree.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\cm_workspace\project5\bulid\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/avltree.dir/avltree.cpp.obj"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/avltree.dir/avltree.cpp.obj -MF CMakeFiles\avltree.dir\avltree.cpp.obj.d -o CMakeFiles\avltree.dir\avltree.cpp.obj -c D:\cm_workspace\project5\avltree.cpp

CMakeFiles/avltree.dir/avltree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/avltree.dir/avltree.cpp.i"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\cm_workspace\project5\avltree.cpp > CMakeFiles\avltree.dir\avltree.cpp.i

CMakeFiles/avltree.dir/avltree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/avltree.dir/avltree.cpp.s"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\cm_workspace\project5\avltree.cpp -o CMakeFiles\avltree.dir\avltree.cpp.s

# Object files for target avltree
avltree_OBJECTS = \
"CMakeFiles/avltree.dir/avltree.cpp.obj"

# External object files for target avltree
avltree_EXTERNAL_OBJECTS =

avltree.exe: CMakeFiles/avltree.dir/avltree.cpp.obj
avltree.exe: CMakeFiles/avltree.dir/build.make
avltree.exe: CMakeFiles/avltree.dir/linklibs.rsp
avltree.exe: CMakeFiles/avltree.dir/objects1.rsp
avltree.exe: CMakeFiles/avltree.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\cm_workspace\project5\bulid\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable avltree.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\avltree.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/avltree.dir/build: avltree.exe
.PHONY : CMakeFiles/avltree.dir/build

CMakeFiles/avltree.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\avltree.dir\cmake_clean.cmake
.PHONY : CMakeFiles/avltree.dir/clean

CMakeFiles/avltree.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\cm_workspace\project5 D:\cm_workspace\project5 D:\cm_workspace\project5\bulid D:\cm_workspace\project5\bulid D:\cm_workspace\project5\bulid\CMakeFiles\avltree.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/avltree.dir/depend

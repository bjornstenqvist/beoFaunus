# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/work/BackupWork/NewAllGo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/work/BackupWork/NewAllGo

# Include any dependencies generated for this target.
include src/examples/CMakeFiles/example_stockmayer.dir/depend.make

# Include the progress variables for this target.
include src/examples/CMakeFiles/example_stockmayer.dir/progress.make

# Include the compile flags for this target's objects.
include src/examples/CMakeFiles/example_stockmayer.dir/flags.make

src/examples/CMakeFiles/example_stockmayer.dir/stockmayer.cpp.o: src/examples/CMakeFiles/example_stockmayer.dir/flags.make
src/examples/CMakeFiles/example_stockmayer.dir/stockmayer.cpp.o: src/examples/stockmayer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/work/BackupWork/NewAllGo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/examples/CMakeFiles/example_stockmayer.dir/stockmayer.cpp.o"
	cd /home/work/BackupWork/NewAllGo/src/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_stockmayer.dir/stockmayer.cpp.o -c /home/work/BackupWork/NewAllGo/src/examples/stockmayer.cpp

src/examples/CMakeFiles/example_stockmayer.dir/stockmayer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_stockmayer.dir/stockmayer.cpp.i"
	cd /home/work/BackupWork/NewAllGo/src/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/work/BackupWork/NewAllGo/src/examples/stockmayer.cpp > CMakeFiles/example_stockmayer.dir/stockmayer.cpp.i

src/examples/CMakeFiles/example_stockmayer.dir/stockmayer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_stockmayer.dir/stockmayer.cpp.s"
	cd /home/work/BackupWork/NewAllGo/src/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/work/BackupWork/NewAllGo/src/examples/stockmayer.cpp -o CMakeFiles/example_stockmayer.dir/stockmayer.cpp.s

# Object files for target example_stockmayer
example_stockmayer_OBJECTS = \
"CMakeFiles/example_stockmayer.dir/stockmayer.cpp.o"

# External object files for target example_stockmayer
example_stockmayer_EXTERNAL_OBJECTS =

src/examples/stockmayer: src/examples/CMakeFiles/example_stockmayer.dir/stockmayer.cpp.o
src/examples/stockmayer: src/examples/CMakeFiles/example_stockmayer.dir/build.make
src/examples/stockmayer: src/libfaunus.so
src/examples/stockmayer: src/xdrfile/libxdrfile.a
src/examples/stockmayer: src/examples/CMakeFiles/example_stockmayer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/work/BackupWork/NewAllGo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable stockmayer"
	cd /home/work/BackupWork/NewAllGo/src/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/example_stockmayer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/examples/CMakeFiles/example_stockmayer.dir/build: src/examples/stockmayer

.PHONY : src/examples/CMakeFiles/example_stockmayer.dir/build

src/examples/CMakeFiles/example_stockmayer.dir/clean:
	cd /home/work/BackupWork/NewAllGo/src/examples && $(CMAKE_COMMAND) -P CMakeFiles/example_stockmayer.dir/cmake_clean.cmake
.PHONY : src/examples/CMakeFiles/example_stockmayer.dir/clean

src/examples/CMakeFiles/example_stockmayer.dir/depend:
	cd /home/work/BackupWork/NewAllGo && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/work/BackupWork/NewAllGo /home/work/BackupWork/NewAllGo/src/examples /home/work/BackupWork/NewAllGo /home/work/BackupWork/NewAllGo/src/examples /home/work/BackupWork/NewAllGo/src/examples/CMakeFiles/example_stockmayer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/examples/CMakeFiles/example_stockmayer.dir/depend


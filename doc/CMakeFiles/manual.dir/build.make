# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/bjorn/github/NewAllGo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bjorn/github/NewAllGo

# Utility rule file for manual.

# Include the progress variables for this target.
include doc/CMakeFiles/manual.dir/progress.make

doc/CMakeFiles/manual:
	/usr/bin/doxygen doc/Doxyfile

manual: doc/CMakeFiles/manual
manual: doc/CMakeFiles/manual.dir/build.make

.PHONY : manual

# Rule to build all files generated by this target.
doc/CMakeFiles/manual.dir/build: manual

.PHONY : doc/CMakeFiles/manual.dir/build

doc/CMakeFiles/manual.dir/clean:
	cd /home/bjorn/github/NewAllGo/doc && $(CMAKE_COMMAND) -P CMakeFiles/manual.dir/cmake_clean.cmake
.PHONY : doc/CMakeFiles/manual.dir/clean

doc/CMakeFiles/manual.dir/depend:
	cd /home/bjorn/github/NewAllGo && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bjorn/github/NewAllGo /home/bjorn/github/NewAllGo/doc /home/bjorn/github/NewAllGo /home/bjorn/github/NewAllGo/doc /home/bjorn/github/NewAllGo/doc/CMakeFiles/manual.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : doc/CMakeFiles/manual.dir/depend


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
include scripts/CMakeFiles/pyfaunus.dir/depend.make

# Include the progress variables for this target.
include scripts/CMakeFiles/pyfaunus.dir/progress.make

# Include the compile flags for this target's objects.
include scripts/CMakeFiles/pyfaunus.dir/flags.make

scripts/CMakeFiles/pyfaunus.dir/pyfaunus.cpp.o: scripts/CMakeFiles/pyfaunus.dir/flags.make
scripts/CMakeFiles/pyfaunus.dir/pyfaunus.cpp.o: scripts/pyfaunus.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/work/BackupWork/NewAllGo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object scripts/CMakeFiles/pyfaunus.dir/pyfaunus.cpp.o"
	cd /home/work/BackupWork/NewAllGo/scripts && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pyfaunus.dir/pyfaunus.cpp.o -c /home/work/BackupWork/NewAllGo/scripts/pyfaunus.cpp

scripts/CMakeFiles/pyfaunus.dir/pyfaunus.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pyfaunus.dir/pyfaunus.cpp.i"
	cd /home/work/BackupWork/NewAllGo/scripts && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/work/BackupWork/NewAllGo/scripts/pyfaunus.cpp > CMakeFiles/pyfaunus.dir/pyfaunus.cpp.i

scripts/CMakeFiles/pyfaunus.dir/pyfaunus.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pyfaunus.dir/pyfaunus.cpp.s"
	cd /home/work/BackupWork/NewAllGo/scripts && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/work/BackupWork/NewAllGo/scripts/pyfaunus.cpp -o CMakeFiles/pyfaunus.dir/pyfaunus.cpp.s

# Object files for target pyfaunus
pyfaunus_OBJECTS = \
"CMakeFiles/pyfaunus.dir/pyfaunus.cpp.o"

# External object files for target pyfaunus
pyfaunus_EXTERNAL_OBJECTS =

scripts/pyfaunus.so: scripts/CMakeFiles/pyfaunus.dir/pyfaunus.cpp.o
scripts/pyfaunus.so: scripts/CMakeFiles/pyfaunus.dir/build.make
scripts/pyfaunus.so: src/libfaunus.so
scripts/pyfaunus.so: /usr/lib/x86_64-linux-gnu/libpython3.8.so
scripts/pyfaunus.so: src/xdrfile/libxdrfile.a
scripts/pyfaunus.so: scripts/CMakeFiles/pyfaunus.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/work/BackupWork/NewAllGo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library pyfaunus.so"
	cd /home/work/BackupWork/NewAllGo/scripts && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pyfaunus.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
scripts/CMakeFiles/pyfaunus.dir/build: scripts/pyfaunus.so

.PHONY : scripts/CMakeFiles/pyfaunus.dir/build

scripts/CMakeFiles/pyfaunus.dir/clean:
	cd /home/work/BackupWork/NewAllGo/scripts && $(CMAKE_COMMAND) -P CMakeFiles/pyfaunus.dir/cmake_clean.cmake
.PHONY : scripts/CMakeFiles/pyfaunus.dir/clean

scripts/CMakeFiles/pyfaunus.dir/depend:
	cd /home/work/BackupWork/NewAllGo && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/work/BackupWork/NewAllGo /home/work/BackupWork/NewAllGo/scripts /home/work/BackupWork/NewAllGo /home/work/BackupWork/NewAllGo/scripts /home/work/BackupWork/NewAllGo/scripts/CMakeFiles/pyfaunus.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : scripts/CMakeFiles/pyfaunus.dir/depend


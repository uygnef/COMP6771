# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /home/yu/下载/clion-2017.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/yu/下载/clion-2017.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yu/6771/a5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yu/6771/a5/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/a5.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/a5.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/a5.dir/flags.make

CMakeFiles/a5.dir/main.cpp.o: CMakeFiles/a5.dir/flags.make
CMakeFiles/a5.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yu/6771/a5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/a5.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/a5.dir/main.cpp.o -c /home/yu/6771/a5/main.cpp

CMakeFiles/a5.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a5.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yu/6771/a5/main.cpp > CMakeFiles/a5.dir/main.cpp.i

CMakeFiles/a5.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a5.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yu/6771/a5/main.cpp -o CMakeFiles/a5.dir/main.cpp.s

CMakeFiles/a5.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/a5.dir/main.cpp.o.requires

CMakeFiles/a5.dir/main.cpp.o.provides: CMakeFiles/a5.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/a5.dir/build.make CMakeFiles/a5.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/a5.dir/main.cpp.o.provides

CMakeFiles/a5.dir/main.cpp.o.provides.build: CMakeFiles/a5.dir/main.cpp.o


CMakeFiles/a5.dir/BucketSort.cpp.o: CMakeFiles/a5.dir/flags.make
CMakeFiles/a5.dir/BucketSort.cpp.o: ../BucketSort.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yu/6771/a5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/a5.dir/BucketSort.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/a5.dir/BucketSort.cpp.o -c /home/yu/6771/a5/BucketSort.cpp

CMakeFiles/a5.dir/BucketSort.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a5.dir/BucketSort.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yu/6771/a5/BucketSort.cpp > CMakeFiles/a5.dir/BucketSort.cpp.i

CMakeFiles/a5.dir/BucketSort.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a5.dir/BucketSort.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yu/6771/a5/BucketSort.cpp -o CMakeFiles/a5.dir/BucketSort.cpp.s

CMakeFiles/a5.dir/BucketSort.cpp.o.requires:

.PHONY : CMakeFiles/a5.dir/BucketSort.cpp.o.requires

CMakeFiles/a5.dir/BucketSort.cpp.o.provides: CMakeFiles/a5.dir/BucketSort.cpp.o.requires
	$(MAKE) -f CMakeFiles/a5.dir/build.make CMakeFiles/a5.dir/BucketSort.cpp.o.provides.build
.PHONY : CMakeFiles/a5.dir/BucketSort.cpp.o.provides

CMakeFiles/a5.dir/BucketSort.cpp.o.provides.build: CMakeFiles/a5.dir/BucketSort.cpp.o


# Object files for target a5
a5_OBJECTS = \
"CMakeFiles/a5.dir/main.cpp.o" \
"CMakeFiles/a5.dir/BucketSort.cpp.o"

# External object files for target a5
a5_EXTERNAL_OBJECTS =

a5: CMakeFiles/a5.dir/main.cpp.o
a5: CMakeFiles/a5.dir/BucketSort.cpp.o
a5: CMakeFiles/a5.dir/build.make
a5: CMakeFiles/a5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yu/6771/a5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable a5"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/a5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/a5.dir/build: a5

.PHONY : CMakeFiles/a5.dir/build

CMakeFiles/a5.dir/requires: CMakeFiles/a5.dir/main.cpp.o.requires
CMakeFiles/a5.dir/requires: CMakeFiles/a5.dir/BucketSort.cpp.o.requires

.PHONY : CMakeFiles/a5.dir/requires

CMakeFiles/a5.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/a5.dir/cmake_clean.cmake
.PHONY : CMakeFiles/a5.dir/clean

CMakeFiles/a5.dir/depend:
	cd /home/yu/6771/a5/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yu/6771/a5 /home/yu/6771/a5 /home/yu/6771/a5/cmake-build-debug /home/yu/6771/a5/cmake-build-debug /home/yu/6771/a5/cmake-build-debug/CMakeFiles/a5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/a5.dir/depend

# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andryleon/gitPro/Yan

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andryleon/gitPro/Yan

# Include any dependencies generated for this target.
include demo/CMakeFiles/echoclient.dir/depend.make

# Include the progress variables for this target.
include demo/CMakeFiles/echoclient.dir/progress.make

# Include the compile flags for this target's objects.
include demo/CMakeFiles/echoclient.dir/flags.make

demo/CMakeFiles/echoclient.dir/echoclient.cc.o: demo/CMakeFiles/echoclient.dir/flags.make
demo/CMakeFiles/echoclient.dir/echoclient.cc.o: demo/echoclient.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/andryleon/gitPro/Yan/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object demo/CMakeFiles/echoclient.dir/echoclient.cc.o"
	cd /home/andryleon/gitPro/Yan/demo && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/echoclient.dir/echoclient.cc.o -c /home/andryleon/gitPro/Yan/demo/echoclient.cc

demo/CMakeFiles/echoclient.dir/echoclient.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/echoclient.dir/echoclient.cc.i"
	cd /home/andryleon/gitPro/Yan/demo && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/andryleon/gitPro/Yan/demo/echoclient.cc > CMakeFiles/echoclient.dir/echoclient.cc.i

demo/CMakeFiles/echoclient.dir/echoclient.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/echoclient.dir/echoclient.cc.s"
	cd /home/andryleon/gitPro/Yan/demo && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/andryleon/gitPro/Yan/demo/echoclient.cc -o CMakeFiles/echoclient.dir/echoclient.cc.s

demo/CMakeFiles/echoclient.dir/echoclient.cc.o.requires:
.PHONY : demo/CMakeFiles/echoclient.dir/echoclient.cc.o.requires

demo/CMakeFiles/echoclient.dir/echoclient.cc.o.provides: demo/CMakeFiles/echoclient.dir/echoclient.cc.o.requires
	$(MAKE) -f demo/CMakeFiles/echoclient.dir/build.make demo/CMakeFiles/echoclient.dir/echoclient.cc.o.provides.build
.PHONY : demo/CMakeFiles/echoclient.dir/echoclient.cc.o.provides

demo/CMakeFiles/echoclient.dir/echoclient.cc.o.provides.build: demo/CMakeFiles/echoclient.dir/echoclient.cc.o

# Object files for target echoclient
echoclient_OBJECTS = \
"CMakeFiles/echoclient.dir/echoclient.cc.o"

# External object files for target echoclient
echoclient_EXTERNAL_OBJECTS =

bin/echoclient: demo/CMakeFiles/echoclient.dir/echoclient.cc.o
bin/echoclient: lib/libyohub.a
bin/echoclient: demo/CMakeFiles/echoclient.dir/build.make
bin/echoclient: demo/CMakeFiles/echoclient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/echoclient"
	cd /home/andryleon/gitPro/Yan/demo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/echoclient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
demo/CMakeFiles/echoclient.dir/build: bin/echoclient
.PHONY : demo/CMakeFiles/echoclient.dir/build

demo/CMakeFiles/echoclient.dir/requires: demo/CMakeFiles/echoclient.dir/echoclient.cc.o.requires
.PHONY : demo/CMakeFiles/echoclient.dir/requires

demo/CMakeFiles/echoclient.dir/clean:
	cd /home/andryleon/gitPro/Yan/demo && $(CMAKE_COMMAND) -P CMakeFiles/echoclient.dir/cmake_clean.cmake
.PHONY : demo/CMakeFiles/echoclient.dir/clean

demo/CMakeFiles/echoclient.dir/depend:
	cd /home/andryleon/gitPro/Yan && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andryleon/gitPro/Yan /home/andryleon/gitPro/Yan/demo /home/andryleon/gitPro/Yan /home/andryleon/gitPro/Yan/demo /home/andryleon/gitPro/Yan/demo/CMakeFiles/echoclient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : demo/CMakeFiles/echoclient.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.6

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jwawerla/projects/ifd

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jwawerla/projects/ifd

# Include any dependencies generated for this target.
include src/CMakeFiles/ifd.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/ifd.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/ifd.dir/flags.make

src/CMakeFiles/ifd.dir/main.cpp.o: src/CMakeFiles/ifd.dir/flags.make
src/CMakeFiles/ifd.dir/main.cpp.o: src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jwawerla/projects/ifd/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/ifd.dir/main.cpp.o"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ifd.dir/main.cpp.o -c /home/jwawerla/projects/ifd/src/main.cpp

src/CMakeFiles/ifd.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ifd.dir/main.cpp.i"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jwawerla/projects/ifd/src/main.cpp > CMakeFiles/ifd.dir/main.cpp.i

src/CMakeFiles/ifd.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ifd.dir/main.cpp.s"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jwawerla/projects/ifd/src/main.cpp -o CMakeFiles/ifd.dir/main.cpp.s

src/CMakeFiles/ifd.dir/main.cpp.o.requires:
.PHONY : src/CMakeFiles/ifd.dir/main.cpp.o.requires

src/CMakeFiles/ifd.dir/main.cpp.o.provides: src/CMakeFiles/ifd.dir/main.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/ifd.dir/build.make src/CMakeFiles/ifd.dir/main.cpp.o.provides.build
.PHONY : src/CMakeFiles/ifd.dir/main.cpp.o.provides

src/CMakeFiles/ifd.dir/main.cpp.o.provides.build: src/CMakeFiles/ifd.dir/main.cpp.o
.PHONY : src/CMakeFiles/ifd.dir/main.cpp.o.provides.build

src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o: src/CMakeFiles/ifd.dir/flags.make
src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o: src/baserobotctrl.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jwawerla/projects/ifd/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ifd.dir/baserobotctrl.cpp.o -c /home/jwawerla/projects/ifd/src/baserobotctrl.cpp

src/CMakeFiles/ifd.dir/baserobotctrl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ifd.dir/baserobotctrl.cpp.i"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jwawerla/projects/ifd/src/baserobotctrl.cpp > CMakeFiles/ifd.dir/baserobotctrl.cpp.i

src/CMakeFiles/ifd.dir/baserobotctrl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ifd.dir/baserobotctrl.cpp.s"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jwawerla/projects/ifd/src/baserobotctrl.cpp -o CMakeFiles/ifd.dir/baserobotctrl.cpp.s

src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o.requires:
.PHONY : src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o.requires

src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o.provides: src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/ifd.dir/build.make src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o.provides.build
.PHONY : src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o.provides

src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o.provides.build: src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o
.PHONY : src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o.provides.build

src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o: src/CMakeFiles/ifd.dir/flags.make
src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o: src/pathplannerlookup.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jwawerla/projects/ifd/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ifd.dir/pathplannerlookup.cpp.o -c /home/jwawerla/projects/ifd/src/pathplannerlookup.cpp

src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ifd.dir/pathplannerlookup.cpp.i"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jwawerla/projects/ifd/src/pathplannerlookup.cpp > CMakeFiles/ifd.dir/pathplannerlookup.cpp.i

src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ifd.dir/pathplannerlookup.cpp.s"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jwawerla/projects/ifd/src/pathplannerlookup.cpp -o CMakeFiles/ifd.dir/pathplannerlookup.cpp.s

src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o.requires:
.PHONY : src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o.requires

src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o.provides: src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/ifd.dir/build.make src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o.provides.build
.PHONY : src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o.provides

src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o.provides.build: src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o
.PHONY : src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o.provides.build

src/CMakeFiles/ifd.dir/patch.cpp.o: src/CMakeFiles/ifd.dir/flags.make
src/CMakeFiles/ifd.dir/patch.cpp.o: src/patch.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jwawerla/projects/ifd/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/ifd.dir/patch.cpp.o"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ifd.dir/patch.cpp.o -c /home/jwawerla/projects/ifd/src/patch.cpp

src/CMakeFiles/ifd.dir/patch.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ifd.dir/patch.cpp.i"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jwawerla/projects/ifd/src/patch.cpp > CMakeFiles/ifd.dir/patch.cpp.i

src/CMakeFiles/ifd.dir/patch.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ifd.dir/patch.cpp.s"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jwawerla/projects/ifd/src/patch.cpp -o CMakeFiles/ifd.dir/patch.cpp.s

src/CMakeFiles/ifd.dir/patch.cpp.o.requires:
.PHONY : src/CMakeFiles/ifd.dir/patch.cpp.o.requires

src/CMakeFiles/ifd.dir/patch.cpp.o.provides: src/CMakeFiles/ifd.dir/patch.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/ifd.dir/build.make src/CMakeFiles/ifd.dir/patch.cpp.o.provides.build
.PHONY : src/CMakeFiles/ifd.dir/patch.cpp.o.provides

src/CMakeFiles/ifd.dir/patch.cpp.o.provides.build: src/CMakeFiles/ifd.dir/patch.cpp.o
.PHONY : src/CMakeFiles/ifd.dir/patch.cpp.o.provides.build

src/CMakeFiles/ifd.dir/patchmanager.cpp.o: src/CMakeFiles/ifd.dir/flags.make
src/CMakeFiles/ifd.dir/patchmanager.cpp.o: src/patchmanager.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jwawerla/projects/ifd/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/ifd.dir/patchmanager.cpp.o"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ifd.dir/patchmanager.cpp.o -c /home/jwawerla/projects/ifd/src/patchmanager.cpp

src/CMakeFiles/ifd.dir/patchmanager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ifd.dir/patchmanager.cpp.i"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jwawerla/projects/ifd/src/patchmanager.cpp > CMakeFiles/ifd.dir/patchmanager.cpp.i

src/CMakeFiles/ifd.dir/patchmanager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ifd.dir/patchmanager.cpp.s"
	cd /home/jwawerla/projects/ifd/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jwawerla/projects/ifd/src/patchmanager.cpp -o CMakeFiles/ifd.dir/patchmanager.cpp.s

src/CMakeFiles/ifd.dir/patchmanager.cpp.o.requires:
.PHONY : src/CMakeFiles/ifd.dir/patchmanager.cpp.o.requires

src/CMakeFiles/ifd.dir/patchmanager.cpp.o.provides: src/CMakeFiles/ifd.dir/patchmanager.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/ifd.dir/build.make src/CMakeFiles/ifd.dir/patchmanager.cpp.o.provides.build
.PHONY : src/CMakeFiles/ifd.dir/patchmanager.cpp.o.provides

src/CMakeFiles/ifd.dir/patchmanager.cpp.o.provides.build: src/CMakeFiles/ifd.dir/patchmanager.cpp.o
.PHONY : src/CMakeFiles/ifd.dir/patchmanager.cpp.o.provides.build

# Object files for target ifd
ifd_OBJECTS = \
"CMakeFiles/ifd.dir/main.cpp.o" \
"CMakeFiles/ifd.dir/baserobotctrl.cpp.o" \
"CMakeFiles/ifd.dir/pathplannerlookup.cpp.o" \
"CMakeFiles/ifd.dir/patch.cpp.o" \
"CMakeFiles/ifd.dir/patchmanager.cpp.o"

# External object files for target ifd
ifd_EXTERNAL_OBJECTS =

src/ifd.so: src/CMakeFiles/ifd.dir/main.cpp.o
src/ifd.so: src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o
src/ifd.so: src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o
src/ifd.so: src/CMakeFiles/ifd.dir/patch.cpp.o
src/ifd.so: src/CMakeFiles/ifd.dir/patchmanager.cpp.o
src/ifd.so: /usr/lib/libQtGui.so
src/ifd.so: /usr/lib/libXext.so
src/ifd.so: /usr/lib/libX11.so
src/ifd.so: /usr/lib/libm.so
src/ifd.so: /usr/lib/libQtCore.so
src/ifd.so: /usr/lib/libQtGui.so
src/ifd.so: /usr/lib/libXext.so
src/ifd.so: /usr/lib/libX11.so
src/ifd.so: /usr/lib/libm.so
src/ifd.so: /usr/lib/libQtCore.so
src/ifd.so: src/CMakeFiles/ifd.dir/build.make
src/ifd.so: src/CMakeFiles/ifd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module ifd.so"
	cd /home/jwawerla/projects/ifd/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ifd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/ifd.dir/build: src/ifd.so
.PHONY : src/CMakeFiles/ifd.dir/build

src/CMakeFiles/ifd.dir/requires: src/CMakeFiles/ifd.dir/main.cpp.o.requires
src/CMakeFiles/ifd.dir/requires: src/CMakeFiles/ifd.dir/baserobotctrl.cpp.o.requires
src/CMakeFiles/ifd.dir/requires: src/CMakeFiles/ifd.dir/pathplannerlookup.cpp.o.requires
src/CMakeFiles/ifd.dir/requires: src/CMakeFiles/ifd.dir/patch.cpp.o.requires
src/CMakeFiles/ifd.dir/requires: src/CMakeFiles/ifd.dir/patchmanager.cpp.o.requires
.PHONY : src/CMakeFiles/ifd.dir/requires

src/CMakeFiles/ifd.dir/clean:
	cd /home/jwawerla/projects/ifd/src && $(CMAKE_COMMAND) -P CMakeFiles/ifd.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/ifd.dir/clean

src/CMakeFiles/ifd.dir/depend:
	cd /home/jwawerla/projects/ifd && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jwawerla/projects/ifd /home/jwawerla/projects/ifd/src /home/jwawerla/projects/ifd /home/jwawerla/projects/ifd/src /home/jwawerla/projects/ifd/src/CMakeFiles/ifd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/ifd.dir/depend


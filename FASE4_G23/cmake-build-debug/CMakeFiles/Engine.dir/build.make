# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/152/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/152/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/joel/Downloads/FASE4_G23

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/joel/Downloads/FASE4_G23/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Engine.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Engine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Engine.dir/flags.make

CMakeFiles/Engine.dir/engine/engine.cpp.o: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/engine/engine.cpp.o: ../engine/engine.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joel/Downloads/FASE4_G23/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Engine.dir/engine/engine.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Engine.dir/engine/engine.cpp.o -c /home/joel/Downloads/FASE4_G23/engine/engine.cpp

CMakeFiles/Engine.dir/engine/engine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Engine.dir/engine/engine.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/joel/Downloads/FASE4_G23/engine/engine.cpp > CMakeFiles/Engine.dir/engine/engine.cpp.i

CMakeFiles/Engine.dir/engine/engine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/engine/engine.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/joel/Downloads/FASE4_G23/engine/engine.cpp -o CMakeFiles/Engine.dir/engine/engine.cpp.s

CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.o: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.o: ../engine/tinyxml2/tinyxml2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joel/Downloads/FASE4_G23/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.o -c /home/joel/Downloads/FASE4_G23/engine/tinyxml2/tinyxml2.cpp

CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/joel/Downloads/FASE4_G23/engine/tinyxml2/tinyxml2.cpp > CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.i

CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/joel/Downloads/FASE4_G23/engine/tinyxml2/tinyxml2.cpp -o CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.s

# Object files for target Engine
Engine_OBJECTS = \
"CMakeFiles/Engine.dir/engine/engine.cpp.o" \
"CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.o"

# External object files for target Engine
Engine_EXTERNAL_OBJECTS =

Engine: CMakeFiles/Engine.dir/engine/engine.cpp.o
Engine: CMakeFiles/Engine.dir/engine/tinyxml2/tinyxml2.cpp.o
Engine: CMakeFiles/Engine.dir/build.make
Engine: /usr/lib/x86_64-linux-gnu/libGL.so
Engine: /usr/lib/x86_64-linux-gnu/libGLU.so
Engine: /usr/lib/x86_64-linux-gnu/libglut.so
Engine: /usr/lib/x86_64-linux-gnu/libXmu.so
Engine: /usr/lib/x86_64-linux-gnu/libXi.so
Engine: /usr/lib/x86_64-linux-gnu/libIL.so
Engine: CMakeFiles/Engine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/joel/Downloads/FASE4_G23/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Engine"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Engine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Engine.dir/build: Engine

.PHONY : CMakeFiles/Engine.dir/build

CMakeFiles/Engine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Engine.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Engine.dir/clean

CMakeFiles/Engine.dir/depend:
	cd /home/joel/Downloads/FASE4_G23/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joel/Downloads/FASE4_G23 /home/joel/Downloads/FASE4_G23 /home/joel/Downloads/FASE4_G23/cmake-build-debug /home/joel/Downloads/FASE4_G23/cmake-build-debug /home/joel/Downloads/FASE4_G23/cmake-build-debug/CMakeFiles/Engine.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Engine.dir/depend


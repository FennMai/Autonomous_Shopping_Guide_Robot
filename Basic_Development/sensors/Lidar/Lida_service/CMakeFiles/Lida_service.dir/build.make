# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service

# Include any dependencies generated for this target.
include CMakeFiles/Lida_service.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Lida_service.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Lida_service.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Lida_service.dir/flags.make

CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.o: CMakeFiles/Lida_service.dir/flags.make
CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.o: Lida_service_autogen/mocs_compilation.cpp
CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.o: CMakeFiles/Lida_service.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.o -MF CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.o -c /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/Lida_service_autogen/mocs_compilation.cpp

CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/Lida_service_autogen/mocs_compilation.cpp > CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.i

CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/Lida_service_autogen/mocs_compilation.cpp -o CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.s

CMakeFiles/Lida_service.dir/main.cpp.o: CMakeFiles/Lida_service.dir/flags.make
CMakeFiles/Lida_service.dir/main.cpp.o: main.cpp
CMakeFiles/Lida_service.dir/main.cpp.o: CMakeFiles/Lida_service.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Lida_service.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Lida_service.dir/main.cpp.o -MF CMakeFiles/Lida_service.dir/main.cpp.o.d -o CMakeFiles/Lida_service.dir/main.cpp.o -c /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/main.cpp

CMakeFiles/Lida_service.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lida_service.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/main.cpp > CMakeFiles/Lida_service.dir/main.cpp.i

CMakeFiles/Lida_service.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lida_service.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/main.cpp -o CMakeFiles/Lida_service.dir/main.cpp.s

CMakeFiles/Lida_service.dir/widget.cpp.o: CMakeFiles/Lida_service.dir/flags.make
CMakeFiles/Lida_service.dir/widget.cpp.o: widget.cpp
CMakeFiles/Lida_service.dir/widget.cpp.o: CMakeFiles/Lida_service.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Lida_service.dir/widget.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Lida_service.dir/widget.cpp.o -MF CMakeFiles/Lida_service.dir/widget.cpp.o.d -o CMakeFiles/Lida_service.dir/widget.cpp.o -c /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/widget.cpp

CMakeFiles/Lida_service.dir/widget.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lida_service.dir/widget.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/widget.cpp > CMakeFiles/Lida_service.dir/widget.cpp.i

CMakeFiles/Lida_service.dir/widget.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lida_service.dir/widget.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/widget.cpp -o CMakeFiles/Lida_service.dir/widget.cpp.s

CMakeFiles/Lida_service.dir/qcustomplot.cpp.o: CMakeFiles/Lida_service.dir/flags.make
CMakeFiles/Lida_service.dir/qcustomplot.cpp.o: qcustomplot.cpp
CMakeFiles/Lida_service.dir/qcustomplot.cpp.o: CMakeFiles/Lida_service.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Lida_service.dir/qcustomplot.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Lida_service.dir/qcustomplot.cpp.o -MF CMakeFiles/Lida_service.dir/qcustomplot.cpp.o.d -o CMakeFiles/Lida_service.dir/qcustomplot.cpp.o -c /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/qcustomplot.cpp

CMakeFiles/Lida_service.dir/qcustomplot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lida_service.dir/qcustomplot.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/qcustomplot.cpp > CMakeFiles/Lida_service.dir/qcustomplot.cpp.i

CMakeFiles/Lida_service.dir/qcustomplot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lida_service.dir/qcustomplot.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/qcustomplot.cpp -o CMakeFiles/Lida_service.dir/qcustomplot.cpp.s

# Object files for target Lida_service
Lida_service_OBJECTS = \
"CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/Lida_service.dir/main.cpp.o" \
"CMakeFiles/Lida_service.dir/widget.cpp.o" \
"CMakeFiles/Lida_service.dir/qcustomplot.cpp.o"

# External object files for target Lida_service
Lida_service_EXTERNAL_OBJECTS =

Lida_service: CMakeFiles/Lida_service.dir/Lida_service_autogen/mocs_compilation.cpp.o
Lida_service: CMakeFiles/Lida_service.dir/main.cpp.o
Lida_service: CMakeFiles/Lida_service.dir/widget.cpp.o
Lida_service: CMakeFiles/Lida_service.dir/qcustomplot.cpp.o
Lida_service: CMakeFiles/Lida_service.dir/build.make
Lida_service: /usr/lib/x86_64-linux-gnu/libQt5PrintSupport.so.5.15.3
Lida_service: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.15.3
Lida_service: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.15.3
Lida_service: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.15.3
Lida_service: CMakeFiles/Lida_service.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable Lida_service"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Lida_service.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Lida_service.dir/build: Lida_service
.PHONY : CMakeFiles/Lida_service.dir/build

CMakeFiles/Lida_service.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Lida_service.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Lida_service.dir/clean

CMakeFiles/Lida_service.dir/depend:
	cd /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/CMakeFiles/Lida_service.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Lida_service.dir/depend

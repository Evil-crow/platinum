# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_SOURCE_DIR = /home/Crow/CLionProjects/platinum

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/Crow/CLionProjects/platinum/build

# Include any dependencies generated for this target.
include CMakeFiles/platinum.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/platinum.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/platinum.dir/flags.make

CMakeFiles/platinum.dir/platinum.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/platinum.cc.o: ../platinum.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/platinum.dir/platinum.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/platinum.cc.o -c /home/Crow/CLionProjects/platinum/platinum.cc

CMakeFiles/platinum.dir/platinum.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/platinum.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/platinum.cc > CMakeFiles/platinum.dir/platinum.cc.i

CMakeFiles/platinum.dir/platinum.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/platinum.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/platinum.cc -o CMakeFiles/platinum.dir/platinum.cc.s

CMakeFiles/platinum.dir/config/config.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/config/config.cc.o: ../config/config.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/platinum.dir/config/config.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/config/config.cc.o -c /home/Crow/CLionProjects/platinum/config/config.cc

CMakeFiles/platinum.dir/config/config.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/config/config.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/config/config.cc > CMakeFiles/platinum.dir/config/config.cc.i

CMakeFiles/platinum.dir/config/config.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/config/config.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/config/config.cc -o CMakeFiles/platinum.dir/config/config.cc.s

CMakeFiles/platinum.dir/utility/logger.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/utility/logger.cc.o: ../utility/logger.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/platinum.dir/utility/logger.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/utility/logger.cc.o -c /home/Crow/CLionProjects/platinum/utility/logger.cc

CMakeFiles/platinum.dir/utility/logger.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/utility/logger.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/utility/logger.cc > CMakeFiles/platinum.dir/utility/logger.cc.i

CMakeFiles/platinum.dir/utility/logger.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/utility/logger.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/utility/logger.cc -o CMakeFiles/platinum.dir/utility/logger.cc.s

CMakeFiles/platinum.dir/utility/buffer/buffer.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/utility/buffer/buffer.cc.o: ../utility/buffer/buffer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/platinum.dir/utility/buffer/buffer.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/utility/buffer/buffer.cc.o -c /home/Crow/CLionProjects/platinum/utility/buffer/buffer.cc

CMakeFiles/platinum.dir/utility/buffer/buffer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/utility/buffer/buffer.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/utility/buffer/buffer.cc > CMakeFiles/platinum.dir/utility/buffer/buffer.cc.i

CMakeFiles/platinum.dir/utility/buffer/buffer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/utility/buffer/buffer.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/utility/buffer/buffer.cc -o CMakeFiles/platinum.dir/utility/buffer/buffer.cc.s

CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.o: ../utility/buffer/writequeue.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.o -c /home/Crow/CLionProjects/platinum/utility/buffer/writequeue.cc

CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/utility/buffer/writequeue.cc > CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.i

CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/utility/buffer/writequeue.cc -o CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.s

CMakeFiles/platinum.dir/utility/buffer/task.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/utility/buffer/task.cc.o: ../utility/buffer/task.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/platinum.dir/utility/buffer/task.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/utility/buffer/task.cc.o -c /home/Crow/CLionProjects/platinum/utility/buffer/task.cc

CMakeFiles/platinum.dir/utility/buffer/task.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/utility/buffer/task.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/utility/buffer/task.cc > CMakeFiles/platinum.dir/utility/buffer/task.cc.i

CMakeFiles/platinum.dir/utility/buffer/task.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/utility/buffer/task.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/utility/buffer/task.cc -o CMakeFiles/platinum.dir/utility/buffer/task.cc.s

CMakeFiles/platinum.dir/net/ip_address.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/net/ip_address.cc.o: ../net/ip_address.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/platinum.dir/net/ip_address.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/net/ip_address.cc.o -c /home/Crow/CLionProjects/platinum/net/ip_address.cc

CMakeFiles/platinum.dir/net/ip_address.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/net/ip_address.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/net/ip_address.cc > CMakeFiles/platinum.dir/net/ip_address.cc.i

CMakeFiles/platinum.dir/net/ip_address.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/net/ip_address.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/net/ip_address.cc -o CMakeFiles/platinum.dir/net/ip_address.cc.s

CMakeFiles/platinum.dir/net/socket.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/net/socket.cc.o: ../net/socket.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/platinum.dir/net/socket.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/net/socket.cc.o -c /home/Crow/CLionProjects/platinum/net/socket.cc

CMakeFiles/platinum.dir/net/socket.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/net/socket.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/net/socket.cc > CMakeFiles/platinum.dir/net/socket.cc.i

CMakeFiles/platinum.dir/net/socket.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/net/socket.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/net/socket.cc -o CMakeFiles/platinum.dir/net/socket.cc.s

CMakeFiles/platinum.dir/net/socketops.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/net/socketops.cc.o: ../net/socketops.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/platinum.dir/net/socketops.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/net/socketops.cc.o -c /home/Crow/CLionProjects/platinum/net/socketops.cc

CMakeFiles/platinum.dir/net/socketops.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/net/socketops.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/net/socketops.cc > CMakeFiles/platinum.dir/net/socketops.cc.i

CMakeFiles/platinum.dir/net/socketops.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/net/socketops.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/net/socketops.cc -o CMakeFiles/platinum.dir/net/socketops.cc.s

CMakeFiles/platinum.dir/net/acceptor.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/net/acceptor.cc.o: ../net/acceptor.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/platinum.dir/net/acceptor.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/net/acceptor.cc.o -c /home/Crow/CLionProjects/platinum/net/acceptor.cc

CMakeFiles/platinum.dir/net/acceptor.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/net/acceptor.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/net/acceptor.cc > CMakeFiles/platinum.dir/net/acceptor.cc.i

CMakeFiles/platinum.dir/net/acceptor.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/net/acceptor.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/net/acceptor.cc -o CMakeFiles/platinum.dir/net/acceptor.cc.s

CMakeFiles/platinum.dir/net/tcp_server.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/net/tcp_server.cc.o: ../net/tcp_server.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/platinum.dir/net/tcp_server.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/net/tcp_server.cc.o -c /home/Crow/CLionProjects/platinum/net/tcp_server.cc

CMakeFiles/platinum.dir/net/tcp_server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/net/tcp_server.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/net/tcp_server.cc > CMakeFiles/platinum.dir/net/tcp_server.cc.i

CMakeFiles/platinum.dir/net/tcp_server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/net/tcp_server.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/net/tcp_server.cc -o CMakeFiles/platinum.dir/net/tcp_server.cc.s

CMakeFiles/platinum.dir/net/tcp_connection.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/net/tcp_connection.cc.o: ../net/tcp_connection.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/platinum.dir/net/tcp_connection.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/net/tcp_connection.cc.o -c /home/Crow/CLionProjects/platinum/net/tcp_connection.cc

CMakeFiles/platinum.dir/net/tcp_connection.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/net/tcp_connection.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/net/tcp_connection.cc > CMakeFiles/platinum.dir/net/tcp_connection.cc.i

CMakeFiles/platinum.dir/net/tcp_connection.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/net/tcp_connection.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/net/tcp_connection.cc -o CMakeFiles/platinum.dir/net/tcp_connection.cc.s

CMakeFiles/platinum.dir/reactor/epoller.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/reactor/epoller.cc.o: ../reactor/epoller.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/platinum.dir/reactor/epoller.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/reactor/epoller.cc.o -c /home/Crow/CLionProjects/platinum/reactor/epoller.cc

CMakeFiles/platinum.dir/reactor/epoller.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/reactor/epoller.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/reactor/epoller.cc > CMakeFiles/platinum.dir/reactor/epoller.cc.i

CMakeFiles/platinum.dir/reactor/epoller.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/reactor/epoller.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/reactor/epoller.cc -o CMakeFiles/platinum.dir/reactor/epoller.cc.s

CMakeFiles/platinum.dir/reactor/event_loop.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/reactor/event_loop.cc.o: ../reactor/event_loop.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/platinum.dir/reactor/event_loop.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/reactor/event_loop.cc.o -c /home/Crow/CLionProjects/platinum/reactor/event_loop.cc

CMakeFiles/platinum.dir/reactor/event_loop.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/reactor/event_loop.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/reactor/event_loop.cc > CMakeFiles/platinum.dir/reactor/event_loop.cc.i

CMakeFiles/platinum.dir/reactor/event_loop.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/reactor/event_loop.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/reactor/event_loop.cc -o CMakeFiles/platinum.dir/reactor/event_loop.cc.s

CMakeFiles/platinum.dir/reactor/channel.cc.o: CMakeFiles/platinum.dir/flags.make
CMakeFiles/platinum.dir/reactor/channel.cc.o: ../reactor/channel.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/platinum.dir/reactor/channel.cc.o"
	/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/platinum.dir/reactor/channel.cc.o -c /home/Crow/CLionProjects/platinum/reactor/channel.cc

CMakeFiles/platinum.dir/reactor/channel.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/platinum.dir/reactor/channel.cc.i"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Crow/CLionProjects/platinum/reactor/channel.cc > CMakeFiles/platinum.dir/reactor/channel.cc.i

CMakeFiles/platinum.dir/reactor/channel.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/platinum.dir/reactor/channel.cc.s"
	/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Crow/CLionProjects/platinum/reactor/channel.cc -o CMakeFiles/platinum.dir/reactor/channel.cc.s

# Object files for target platinum
platinum_OBJECTS = \
"CMakeFiles/platinum.dir/platinum.cc.o" \
"CMakeFiles/platinum.dir/config/config.cc.o" \
"CMakeFiles/platinum.dir/utility/logger.cc.o" \
"CMakeFiles/platinum.dir/utility/buffer/buffer.cc.o" \
"CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.o" \
"CMakeFiles/platinum.dir/utility/buffer/task.cc.o" \
"CMakeFiles/platinum.dir/net/ip_address.cc.o" \
"CMakeFiles/platinum.dir/net/socket.cc.o" \
"CMakeFiles/platinum.dir/net/socketops.cc.o" \
"CMakeFiles/platinum.dir/net/acceptor.cc.o" \
"CMakeFiles/platinum.dir/net/tcp_server.cc.o" \
"CMakeFiles/platinum.dir/net/tcp_connection.cc.o" \
"CMakeFiles/platinum.dir/reactor/epoller.cc.o" \
"CMakeFiles/platinum.dir/reactor/event_loop.cc.o" \
"CMakeFiles/platinum.dir/reactor/channel.cc.o"

# External object files for target platinum
platinum_EXTERNAL_OBJECTS =

../bin/platinum: CMakeFiles/platinum.dir/platinum.cc.o
../bin/platinum: CMakeFiles/platinum.dir/config/config.cc.o
../bin/platinum: CMakeFiles/platinum.dir/utility/logger.cc.o
../bin/platinum: CMakeFiles/platinum.dir/utility/buffer/buffer.cc.o
../bin/platinum: CMakeFiles/platinum.dir/utility/buffer/writequeue.cc.o
../bin/platinum: CMakeFiles/platinum.dir/utility/buffer/task.cc.o
../bin/platinum: CMakeFiles/platinum.dir/net/ip_address.cc.o
../bin/platinum: CMakeFiles/platinum.dir/net/socket.cc.o
../bin/platinum: CMakeFiles/platinum.dir/net/socketops.cc.o
../bin/platinum: CMakeFiles/platinum.dir/net/acceptor.cc.o
../bin/platinum: CMakeFiles/platinum.dir/net/tcp_server.cc.o
../bin/platinum: CMakeFiles/platinum.dir/net/tcp_connection.cc.o
../bin/platinum: CMakeFiles/platinum.dir/reactor/epoller.cc.o
../bin/platinum: CMakeFiles/platinum.dir/reactor/event_loop.cc.o
../bin/platinum: CMakeFiles/platinum.dir/reactor/channel.cc.o
../bin/platinum: CMakeFiles/platinum.dir/build.make
../bin/platinum: CMakeFiles/platinum.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/Crow/CLionProjects/platinum/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX executable ../bin/platinum"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/platinum.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/platinum.dir/build: ../bin/platinum

.PHONY : CMakeFiles/platinum.dir/build

CMakeFiles/platinum.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/platinum.dir/cmake_clean.cmake
.PHONY : CMakeFiles/platinum.dir/clean

CMakeFiles/platinum.dir/depend:
	cd /home/Crow/CLionProjects/platinum/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/Crow/CLionProjects/platinum /home/Crow/CLionProjects/platinum /home/Crow/CLionProjects/platinum/build /home/Crow/CLionProjects/platinum/build /home/Crow/CLionProjects/platinum/build/CMakeFiles/platinum.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/platinum.dir/depend


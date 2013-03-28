# This Makefile should be used as a template for future Makefiles.
# It’s heavily commented, so hopefully you can understand what each
# line does.
# We’ll use gcc for C compilation and g++ for C++ compilation
CC = gcc
CXX = g++
# Let’s leave a place holder for additional include directories
INCLUDES =
# Compilation options:
# -g for debugging info and -Wall enables all warnings
CFLAGS = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)
# Linking options:
# -g for debugging info
LDFLAGS = -g
# List the libraries you need to link with in LDLIBS
# For example, use "-lm" for the math library
LDLIBS =
#
#
#
#
#
#
#
#
all: simple_server simple_client
#
#
simple_server: simple_server.o
#
simple_client: simple_client.o
#
#
#
#
#
#
# make uses the following implicit rule to compile a .c file into a .o
# file:
#
#
#$(CC) -c $(CFLAGS) <the-.c-file>
#
simple_server.o: simple_server.c
#
simple_client.o: simple_client.c
#
#
#
#
#
#Always provide the "clean" target that removes intermediate files.
#What you remove depend on your choice of coding tools
#(different editors generate different backup files for example).
#And the "clean" target is not a file name, so we tell make that
#it’s a "phony" target.
.PHONY: clean
clean: 
	rm -f *.o a.out core main
# "all" target is useful if your Makefile builds multiple programs.
# Here we’ll have it first do "clean", and rebuild the main target.
#.PHONY: all
#all: 
#	clean simple_server simple_client


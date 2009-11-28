#
# Description: Simple makefile to build byzantine generals code sample
# Author: Reshen Amin [reshen@zensrc.com]
#

# General safe conventions, see GNU make man.
SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

# Paths
prefix = ..
srcdir = $(prefix)/src
objdir = $(prefix)/obj
boost_lib_dir = /usr/include/boost-1_33_1 

# Compiler related
CC      = g++
CDEBUG = -g
DEFS =  
CFLAGS  = $(CDEBUG) -I$(srcdir) -I$(boost_lib_dir) $(DEFS) -Wall
LDFLAGS  = -g

# List of source files (including paths)
SOURCES = 	$(srcdir)/main.cpp \
			$(srcdir)/bgeneralthread.cpp

# Replace the path in the source file listing with a separate directory for objects
OBJECTS = $(addprefix $(objdir)/, $(notdir $(SOURCES:.cpp=.o))) 

LIBS = 


# Binary target name
EXECUTABLE = bgenerals.exe


# Targets

# Phony is good practice to keep target rules working despite poorly named file creations 
# http://www.gnu.org/software/make/manual/make.html#Phony-Targets
.PHONY: all
all: $(EXECUTABLE) TAGS

# Automatic variables used http://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# $@ file name of the target of the rule
# $% target member name
# $< name of the first prereq
#
$(EXECUTABLE): $(OBJECTS)
	@echo "============="
	@echo "Linking the target $@"
	@echo "============="
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# Simple brute force rule to build obj/*.o based on their src/*.cpp counterparts
.PHONY: $(objdir)/%.o
$(objdir)/%.o: $(srcdir)/%.cpp
	@mkdir -p $(dir $@)
	@echo "============="
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Do not add phony rule, we want TAGS to be checked to save a little build time.
TAGS: $(SOURCES)
	@echo "============="
	@echo "Creating TAGS"
	@echo "============="
	etags $(SOURCES)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) TAGS


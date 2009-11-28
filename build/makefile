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
SOURCES = $(srcdir)/main.cpp

# Take the path to each of the source files, and append $(objdir) to it.  In this way,
# if we have source files organized into multiple src/ folders, the ../obj/ folders will 
# contain the .o files.
OBJECTS = $(join $(addsuffix $(objdir)/, $(dir $(SOURCES))), $(notdir $(SOURCES:.cpp=.o))) 
LIBS = 

# Binary target name
EXECUTABLE = bgenerals.exe

# TAGS output
TAGS_OUTPUT = $(srcdir)/TAGS


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

.PHONY: $(OBJECTS)
$(OBJECTS): $(SOURCES)
	@mkdir -p $(dir $@)
	@echo "============="
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

TAGS: $(SOURCES)
	@echo "============="
	@echo "Creating TAGS"
	@echo "============="
	etags -f $(TAGS_OUTPUT) $(SOURCES)

# Leave TAGS out of clean, since we likely still want to use the last good 
# tags build to help navigate src code.
.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(TAGS_OUTPUT)


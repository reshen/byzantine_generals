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
VPATH = $(srcdir)

# Compiler related
CC      = g++
CDEBUG = -g
DEFS =  
CFLAGS  = $(CDEBUG) -I$(srcdir) $(DEFS) -Wall
LDFLAGS  = -g

SOURCES = main.cpp
OBJECTS = $(join $(addsuffix $(objdir)/, $(dir $(SOURCES))), $(notdir $(SOURCES:.cpp=.o))) 
LIBS = 
SOURCES_FULL_PATH = $(join $(addsuffix $(srcdir)/, $(dir $(SOURCES))), $(notdir $(SOURCES))) 

# Binary target name
EXECUTABLE = bgenerals


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

.PHONY: $(objdir)/%.o
$(objdir)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "============="
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

TAGS: $(SOURCES_FULL_PATH)
	@echo "============="
	@echo "Creating TAGS"
	@echo "============="
	etags $(SOURCES_FULL_PATH)

# Leave TAGS out of clean, since we likely still want to use the last good 
# tags build to help navigate src code.
.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)


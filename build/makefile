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
srcdir = $(prefix)/src/
objdir = $(prefix)/obj/
boost_inc_dir = /usr/include/boost-1_33_1 
boost_lib_dir = /usr/lib

# Compiler related
CC      = g++
CDEBUG = -g
DEFS =  
CFLAGS  = $(CDEBUG) -I$(srcdir) -I$(boost_inc_dir) $(DEFS) -Wall
LDFLAGS  = -g -L$(boost_lib_dir)

# List of source files (including paths)
SRCS =	$(srcdir)main.cpp \
		$(srcdir)bgeneralthread.cpp

# Replace the path in the source file listing with a separate directory for objects
OBJS = $(addprefix $(objdir), $(notdir $(SRCS:.cpp=.o))) 

# Libraries used
LIBS = -lboost_thread-gcc-mt

# Binary target name (name of exe we're trying to build)
TARGET = bgenerals.exe


# Targets

# Phony is good practice to keep target rules working despite poorly named file creations 
# http://www.gnu.org/software/make/manual/make.html#Phony-Targets
.PHONY: all
all: $(TARGET) TAGS

# In order to build $(TARGET), we first build each of the $(OBJS).
# Then we use the given command to link those $(OBJS) into our
# $(TARGET) executable.  $^ is a shortcut for $(OBJS).  $@ is a
# shortcut for $(TARGET).
#
# The default compile rule will compile each of the $(OBJS) for us.
$(TARGET): $(OBJS)
	@echo "============="
	@echo "Linking the target $@"
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# Simple brute force rule to build obj/*.o based on their src/*.cpp counterparts
.PHONY: $(objdir)%.o
$(objdir)%.o: $(srcdir)%.cpp
	@mkdir -p $(dir $@)
	@echo "============="
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Do not add phony rule, we want TAGS to be checked to save a little build time.
TAGS: $(SRCS)
	@echo "============="
	@echo "Creating TAGS"
	etags $(SRCS)

# Use "make clean" to remove all of the support files.
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) TAGS Makefile.depend

# This automatically uses the 'makedepend' utility to add any
# dependencies that our source files have, namely .h files.  This way,
# if the .h files change, the code will be re-compiled.
# <http://stackoverflow.com/questions/1500715/gnu-makefile-handling-dependencies>
# makedepend generates the dependency list, perl regexp magic prepends Makefile.depend and 
# corrects the paths to the .o files since we house them in a separate directory.
-include Makefile.depend
Makefile.depend: $(SRCS)
	@echo "============="
	@echo "Creating Makefile.depend"
	makedepend -f- $(SRCS) 2> /dev/null | \
	perl -p -e "s/^(.*)\/(.*\.o:.*)/Makefile.depend $(subst /,\/,$(objdir))\2/" > Makefile.depend


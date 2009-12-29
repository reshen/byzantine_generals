#
# Description: Simple makefile to build byzantine generals code sample
# Author: Reshen Amin [reshen@zensrc.com]
#

# General safe conventions, see GNU make man.
SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

#
# Paths
#
prefix = ..
srcdir = $(prefix)/src/
objdir = $(prefix)/obj/
boost_inc_dir = /usr/include/boost
boost_lib_dir = /usr/lib

#
# Compiler related
#
CC      = g++

# -O0 : Reduce compilation time and make debugging produce 
#       expected results (no-optimiation) [default]
# -g : Produce debugging information
CDEBUG = -O0 -g

DEFS =  

# Try and stack the deck in our favor, turn on many types of 
# warnings, treat them as errors. 
#
# -Wall : Enable most warnings
# -Wextra : Enable extra warnings
# -Wshadow : Warn about shadow variables
# -Werror : Treat warnings as failures
# -MD : Dependency generation
CFLAGS  = $(CDEBUG) -I$(srcdir) -I$(boost_inc_dir) $(DEFS) -Wall -Wextra -Wshadow -Werror -MD 
LDFLAGS  = -L$(boost_lib_dir)

# List of source files (including paths)
SRCS =	$(srcdir)main.cpp \
		$(srcdir)byzgeneralthread.cpp \
		$(srcdir)order.cpp

# Replace the path in the source file listing with a separate directory for objects
OBJS = $(addprefix $(objdir), $(notdir $(SRCS:.cpp=.o))) 

# Libraries used
LIBS = -lboost_thread-mt -lboost_serialization-mt

# Binary target name (name of exe we're trying to build)
TARGET = byzgenerals
TARGET_RUNTIME_ARGUMENTS = 4 1

#
# Targets
#

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
# Dependency generation reference <http://make.paulandlesley.org/autodep.html>
#
# Also perform static analysis on all of the files, in another effort to 
# stack the deck in our favor and catch errors early (Force our static 
# analysis tool to return an error and halt the makefile when it finds 
# problems).
.PHONY: $(objdir)%.o
$(objdir)%.o: $(srcdir)%.cpp
	@mkdir -p $(dir $@)
	@echo "============="
	@echo -n "Static analysis: "
	cppcheck -q --error-exitcode=1 $<
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Dependency generation $<"
	@cp $(objdir)$*.d $(objdir)$*.P; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(objdir)$*.d >> $(objdir)$*.P; \
		rm -f $(objdir)$*.d

.PHONY: check
check: all
	@echo "============="
	@echo "Performing valgrind analysis"
	valgrind --track-origins=yes --leak-check=full $(TARGET) $(TARGET_RUNTIME_ARGUMENTS)

# Add profiling info to build
.PHONY: profile
profile: CFLAGS += -pg
profile: $(TARGET)

# Do not add phony rule, we want TAGS to be checked to save a little build time.
TAGS: $(SRCS)
	@echo "============="
	@echo "Creating TAGS"
	etags $(SRCS)

# Use "make clean" to remove all of the support files.
.PHONY: clean
clean:
	rm -f $(OBJS) $(OBJS:%.o=%.P) $(TARGET) TAGS

#
# Include dependency rules / targets
#
-include $(OBJS:%.o=%.P)


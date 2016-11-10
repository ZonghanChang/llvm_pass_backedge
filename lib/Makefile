# Makefile for backedge pass

# Path to top level of LLVM hierarchy
LEVEL = ../..

# Name of the library to build
LIBRARYNAME = backedge

# Make the shared library become a loadable module so the tools can
# dlopen/dlsym on the resulting library.
LOADABLE_MODULE = 1

# Might need to change this to .so in LINUX
 LIB_EXT = .dylib
#LIB_EXT = .so

TEST_BackEdge = $(LEVEL)/Debug+Asserts/bin/opt -load $(LEVEL)/Debug+Asserts/lib/$(LIBRARYNAME)$(LIB_EXT) -BackEdge <

# Include the makefile implementation stuff
include $(LEVEL)/Makefile.common

test1:
	$(TEST_BackEdge) ../tests/test1.bc > /dev/null

test2:
	$(TEST_BackEdge) ../tests/test2.bc > /dev/null

test3:
	$(TEST_BackEdge) ../tests/test3.bc > /dev/null

test4:
	$(TEST_BackEdge) ../tests/test4.bc > /dev/null


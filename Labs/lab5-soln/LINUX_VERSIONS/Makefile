#! /usr/bin/make

.NULL: .exe

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
TARGETS = $(basename $(SOURCES))

INIT_SHADER = ../../Common/InitShader.o

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

# Linux (default)
#LDLIBS = -lglut -lGL -lXmu -lX11  -lm
LDLIBS = -lGLEW -lglut -lGL -lXmu -lX11  -lm

# Windows - cygwin
ifneq (,$(findstring CYGWIN,$(uname_S)))
	CXXOPTS = -mno-cygwin -mwin32
	CXXDEFS = -DFREEGLUT_STATIC -DGLEW_STATIC
	LDDIRS = -L../../lib/windows
	LDLIBS = -lfreeglut -lglew32 -lopengl32 -lgdi32 -lwinmm
endif

# MinGW
ifneq (,$(findstring MINGW,$(uname_S)))
	CXXDEFS = -DFREEGLUT_STATIC -DGLEW_STATIC
	LDDIRS = -L../../lib/windows
	LDLIBS = -static-libgcc -static-libstdc++ -lfreeglut_static -lglew32 -lopengl32 -lgdi32 -lwinmm -Wl,--subsystem,windows
endif



CXXINCS = -I../../include
CXXFLAGS = $(CXXOPTS) $(CXXDEFS) $(CXXINCS)
LDFLAGS = $(LDOPTS) $(LDDIRS) $(LDLIBS)

DIRT = $(wildcard *.o *.i *~ */*~ *.log *.exe)

#-----------------------------------------------------------------------------

.PHONY: Makefile

default all: $(TARGETS)

$(TARGETS): $(INIT_SHADER)

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

#-----------------------------------------------------------------------------

%.i: %.cpp
	$(CXX) -E $(CXXFLAGS) $< | uniq > $@

#-----------------------------------------------------------------------------

clean:
	$(RM) $(DIRT)

rmtargets:
	$(RM) $(TARGETS) $(INIT_SHADER)

clobber: clean rmtargets

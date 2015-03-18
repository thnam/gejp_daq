CC = gcc
CFLAGS = -g -Wall -I/usr/include
CXX = g++
CXXFLAGS = -g -Wall -I/usr/include

MIDAS_INC = $(MIDASSYS)/include
CFLAGS += -I$(MIDAS_INC)
CXXFLAGS += -I$(MIDAS_INC)

#ROOTLIBDIR := $(shell $(ROOTSYS)/bin/root-config --libdir)
#ROOTLIBS   := -L$(ROOTLIBDIR) $(shell $(ROOTSYS)/bin/root-config --libs)  
#ROOTGLIBS  := -L$(ROOTLIBDIR) $(shell $(ROOTSYS)/bin/root-config --glibs) 
#ROOTCFLAGS := $(shell $(ROOTSYS)/bin/root-config --cflags)
#CXXFLAGS += -DHAVE_ROOT $(ROOTCFLAGS)

TARGETS = r
SRC = v1290
DRV = bt617.o rpv130.o v006.o v005.o

.PHONY: clean drivers

all: drivers

drivers: $(DRV)

r: $(SRC).o $(DRV)
	$(CXX) $(CXXFLAGS) -o $@ $< $(DRV) $(ROOTLIBS)

.c.o:
	$(CXX) $(CXXFLAGS) -c $< 

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< 

clean:
	rm -f *.o
	rm -f $(TARGETS)


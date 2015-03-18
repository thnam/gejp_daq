# Makefile
CXX = g++
UFE = frontend
ANALYZER = analyzer

ifndef MIDASSYS
	missmidas::
	@echo "...";
	@echo "Missing definition of environment variable 'MIDASSYS' !";
	@echo "...";
endif

# Detect OS type and set flags respectively
# Linux 
OSTYPE = $(shell uname)
ifeq ($(OSTYPE),Linux)
	OSTYPE = linux
endif
ifeq ($(OSTYPE),linux)
	OS_DIR = linux
	OSFLAGS = -DOS_LINUX -Dextname
	# OSFLAGS += -m32 
	CXXFLAGS   = -g -O2 -Wall -Wuninitialized -I$(MIDASSYS)/include
	CFLAGS = $(CXXFLAGS)
	LIBS = -lm -lz -lutil -lnsl -lpthread -lrt 
endif

ifeq ($(OSTYPE),Darwin)
	OSTYPE = darwin
endif
ifeq ($(OSTYPE),darwin)
	OS_DIR = darwin
	OSFLAGS = -DOS_LINUX -DOS_DARWIN -Dextname
	# OSFLAGS += -m32 
	CXXFLAGS   = -g -O2 -Wall -Wuninitialized -I$(MIDASSYS)/include
	CFLAGS = $(CXXFLAGS)
	LIBS = -lm -lz 
endif

# MIDAS library
MIDASLIBS = $(MIDASSYS)/$(OSTYPE)/lib/libmidas.a 
MFE       = $(MIDASSYS)/$(OSTYPE)/lib/mfe.o
MANA 			= $(MIDASSYS)/$(OSTYPE)/lib/rmana.o

# VME drivers
DRVDIR = ./bit3_vme_drivers
CFLAGS += -I$(DRVDIR)
CXXFLAGS += -I$(DRVDIR)

DRV = $(DRVDIR)/*.o

SRCDIR = src
OBJDIR = obj

# ROOT flags
ROOTLIBDIR := $(shell $(ROOTSYS)/bin/root-config --libdir)
ROOTLIBS   := -L$(ROOTLIBDIR) $(shell $(ROOTSYS)/bin/root-config --libs) 
ROOTGLIBS  := -L$(ROOTLIBDIR) $(shell $(ROOTSYS)/bin/root-config --glibs)
ROOTCFLAGS := $(shell $(ROOTSYS)/bin/root-config --cflags)
RPATH    += -Wl,-rpath,$(ROOTLIBDIR)
CXXFLAGS += -DHAVE_ROOT $(ROOTCFLAGS)
CXXFLAGS += -DHAVE_MIDAS -I$(MIDASSYS)/include

ODBSETUP  = odbsetup
######################################################################
# RULES
######################################################################
all: $(UFE)  $(ODBSETUP) $(ANALYZER)

$(UFE): $(OBJDIR)/$(UFE).o $(MIDASLIBS) $(MFE) 
	@echo "Linking ..."
	$(CXX) -o $@ $(CXXFLAGS) $(OSFLAGS) $^ $(LIBS) $(DRV)

$(ODBSETUP): $(OBJDIR)/$(ODBSETUP).o $(MIDASLIBS) 
	$(CXX) -o $@ $(CXXFLAGS) $(OSFLAGS) $^ $(MIDASLIBS) $(LIBS)

$(ANALYZER): $(OBJDIR)/$(ANALYZER).o $(MANA)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(ROOTANALIB) $(MIDASLIBS) $(ROOTGLIBS) \
		-lm -lz -lpthread -lrt -lutil $(RPATH)

$(OBJDIR)/%.o: $(SRCDIR)/%.cc  Makefile
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) $(OSFLAGS) -c $< -o $@

%.o: %.cc  Makefile
	$(CXX) $(CXXFLAGS) $(OSFLAGS) -c $< -o $@

clean::
	-rm -f *.o $(UFE) $(UFE2) $(ODBSETUP) $(OBJS) $(ANALYZER)
	rm -rf $(OBJDIR)
# end

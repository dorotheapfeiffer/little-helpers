ROOTCONFIG = $(ROOTSYS)/bin/root-config
ROOTCINT   = $(ROOTSYS)/bin/rootcint

#CXX = clang++
CXX = $(shell $(ROOTCONFIG) --cxx)
#LD  = clang++
LD = $(shell $(ROOTCONFIG) --ld)

CXXFLAGS = $(shell $(ROOTCONFIG) --cflags) 
LDFLAGS  = $(shell $(ROOTCONFIG) --ldflags)
LIBS     = $(shell $(ROOTCONFIG) --glibs)
%SOFLAGS  = -shared

EXECUTABLE = apv2root
INCLUDES   = $(wildcard R*.h)
SOURCES    = $(wildcard *.cxx)
OBJECTS    = $(SOURCES:.cxx=.o)
DICT       = $(EXECUTABLE)Dict
DICTIONARY = $(DICT).cxx
DICTOBJ    = $(DICT).o 
LINKDEF    = $(EXECUTABLE)LinkDef.h

all: executable

forceall: clean executable

executable: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(DICTOBJ)
	$(CXX) -v $(CXXFLAGS) -std=c++11 -O2 -Wall $^ $(LIBS) -o $@ # This line works
	

objects: $(OBJECTS) $(DICTOBJ)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -std=c++11 -O2 -Wall  -c -o $@ $<

dictionary: $(DICTIONARY)

$(DICTIONARY):  $(INCLUDES) $(LINKDEF)
	$(ROOTCINT) -f $(DICTIONARY) -c -p $^
#$(INCLUDES) $(LINKDEF)
# -c -p $^

clean:
	rm -f $(EXECUTABLE) $(TARGET) $(OBJECTS) $(DICTIONARY) $(DICTOBJ) $(DICT)_rdict.pcm
	
.PHONY: clean

ROOTCONFIG = $(ROOTSYS)/bin/root-config
ROOTCINT   = $(ROOTSYS)/bin/rootcint

#CXX = clang++
CXX = $(shell $(ROOTCONFIG) --cxx)
#LD  = clang++
LD = $(shell $(ROOTCONFIG) --ld)

CXXFLAGS = $(shell $(ROOTCONFIG) --cflags) 
CXX2011 = -std=c++11
LDFLAGS  = $(shell $(ROOTCONFIG) --ldflags)
LIBS     = $(shell $(ROOTCONFIG) --glibs)
%SOFLAGS  = -shared

EXECUTABLE = manipulateJSON
TARGET     = lib$(EXECUTABLE).so
INCLUDES   = $(EXECUTABLE)LinkDef.h
SOURCES    = $(wildcard *.cpp)
OBJECTS    = $(SOURCES:.cpp=.o)
DICT       = $(EXECUTABLE)Dict
DICTIONARY = $(DICT).cpp
DICTOBJ    = $(DICT).o 


all: executable

forceall: clean executable

executable: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(DICTOBJ)
	$(CXX) -v $(CXXFLAGS) $(CXX2011) -O2  -Wall $^ $(LIBS) $(TBBLIB) -o $@ # This line works
	

target: $(TARGET)

$(TARGET): $(OBJECTS) $(DICTOBJ)
	$(LD) $(LDFLAGS) $(SOFLAGS) -Wall -O2 $(LIBS) $(TBBLIB) -o $@ $^

objects: $(OBJECTS) $(DICTOBJ)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(CXX2011) -O2 -Wall  -c -o $@ $<

dictionary: $(DICTIONARY)

$(DICTIONARY): 
	$(ROOTCINT) -f $(DICTIONARY) -c -p  $(INCLUDES) 


clean:
	rm -f $(EXECUTABLE) $(TARGET) $(OBJECTS) $(DICTIONARY) $(DICTOBJ) $(DICT)_rdict.pcm *.pcm *_h.d *_cpp.d *_cpp.so *_h.so
	
.PHONY: clean

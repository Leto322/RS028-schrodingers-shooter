appname := SchShooter.out

CXX := g++
CXXFLAGS := -std=c++17 -lBox2D -lGL -lglut -lGLU -lopenal -lalut -lm

srcfiles := $(shell find source -maxdepth 1 -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(LDFLAGS) -o $(appname) $(objects) $(CXXFLAGS)

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f $(objects)

dist-clean: clean
	rm -f *~ .depend

include .depend
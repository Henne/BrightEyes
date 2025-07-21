CXX=g++
CXXFLAGS=-Og -g -Wextra

all: dump_obj exe_comp

clean:
	rm -f dump_obj exe_comp

dump_obj: dump_obj.cpp
	$(CXX) $(CXXFLAGS) -o dump_obj dump_obj.cpp

exe_comp: exe_comp.cpp
	$(CXX) $(CXXFLAGS) -o exe_comp exe_comp.cpp

CXXFLAGS += -std=c++17 -O3
# CXXFLAGS += -g -pedantic -Wall -std=c++17 -Og

OBJ := new.o rangeHash.o rangeTree.o main.o

all: rangeContainer

rangeContainer: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

new.o: new.cpp new.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

rangeHash.o: rangeHash.cpp rangeHash.h range.h new.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

rangeTree.o: rangeTree.cpp rangeTree.h range.h new.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

main.o: main.cpp rangeHash.h rangeTree.h range.h new.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY: all clean

clean:
	rm -f $(OBJ) rangeContainer

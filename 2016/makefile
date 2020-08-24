# makefile voor TheSixhundredProject
CXX=g++
CXXFLAGS=-Wall --std=c++11
all: 5.out 14.out 17.out

5.out: md5.o 5.o
	$(CXX) $(CXXFLAGS) -o $@ $^

5.o: 5.cpp md5.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

14.out: md5.o 14.o
	$(CXX) $(CXXFLAGS) -o $@ $^

14.o: 14.cpp md5.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

17.out: md5.o 17.o
	$(CXX) $(CXXFLAGS) -o $@ $^

17.o: 17.cpp md5.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

md5.o: md5.cpp md5.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<


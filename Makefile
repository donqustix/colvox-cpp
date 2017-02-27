CXX = g++
CXXFLAGS = -std=c++14 -pedantic -pthread -Wall -Wextra
LDFLAGS = -lSDL2 -lGLEW -lGL

PROJECT_NAME = MineCPP
PROJECT_SRCS = $(wildcard src/*.cpp)

all: $(PROJECT_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o bin/$(PROJECT_NAME) $(LDFLAGS)

run:
	./bin/$(PROJECT_NAME)

bin/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


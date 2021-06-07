CXX = g++
CXXFLAGS = -std=c++2a -Wall -I h -c
LXXFLAGS = -std=c++2a -Ih -pthread
OBJECTS = ./obj/maze.o ./obj/main.o
TARGET = main


$(TARGET): $(OBJECTS)
	$(CXX) $(LXXFLAGS) -o $(TARGET) $(OBJECTS)
./obj/maze.o: ./cpp/maze.cpp
	$(CXX) $(CXXFLAGS) ./cpp/maze.cpp -o ./obj/maze.o
./obj/main.o: ./cpp/main.cpp ./h/maze.h
	$(CXX) $(CXXFLAGS) ./cpp/main.cpp -o ./obj/main.o
clean:
	rm -fv $(TARGET) $(OBJECTS)

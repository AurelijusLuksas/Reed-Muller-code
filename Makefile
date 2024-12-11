CXX = g++
CXXFLAGS = -std=c++11 -Wall -O3 -fopenmp

# List of source files
SRCS = main.cpp encoder.cpp decoder.cpp stringOperations.cpp channel.cpp picture.cpp

# Output executable
TARGET = reedmuller

# Build target
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean target
clean:
	rm -f $(TARGET)
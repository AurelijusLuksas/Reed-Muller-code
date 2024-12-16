CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3 -fopenmp

# List of source files
SRCS = main.cpp encoder.cpp decoder.cpp stringOperations.cpp channel.cpp picture.cpp
BENCHMARK_SRCS = benchmark.cpp encoder.cpp decoder.cpp stringOperations.cpp channel.cpp

# Output executable
TARGET = reedmuller
BENCHMARK_TARGET = benchmark

# Build target
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Build benchmark target
$(BENCHMARK_TARGET): $(BENCHMARK_SRCS)
	$(CXX) $(CXXFLAGS) -o $(BENCHMARK_TARGET) $(BENCHMARK_SRCS)

# Clean target
clean:
	rm -f $(TARGET) $(BENCHMARK_TARGET)
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2

# Executable name
TARGET = astar_search

# Source files
SRCS = proj.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compile the source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild everything
rebuild: clean all

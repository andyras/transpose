# name of main executable
### This line should be just about all you have to change ###
BIN = transpose
#############################################################

# flags for compiler and linker
CXX = icpc
CPP = $(CXX)
CPPFLAGS = -O3 -Wall -xHOST -ipo -no-prec-div
INCLUDES = -I./include
LDFLAGS = 

# directories for headers, objects and source files
INCDIR = ./include
OBJDIR = ./obj
SRCDIR = ./src

# make will look for .cpp files in $(SRCDIR)
vpath %.cpp $(SRCDIR)

# list of source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# object files have same name as .cpp files, but with .o extension
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,obj/%.o,$(SOURCES))

# build the main executable; this should be listed first
$(BIN): $(OBJECTS)
	$(CPP) -o $@ $^ $(LDFLAGS)

# automatic rule for building objects
$(OBJDIR)/%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

# all objects depend on object directory
$(OBJECTS): | $(OBJDIR)

# make object directory if it does not exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean build
clean:
	rm -rf $(OBJDIR)
	rm -f $(BIN)

build:
	make clean
	make

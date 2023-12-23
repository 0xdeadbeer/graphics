CC=g++
CFLAGS=-std=c++20 `pkg-config --cflags glfw3 glew glm`
LDFLAGS=`pkg-config --libs glfw3 glew glm`
TARGET=boilerplate
SDIR=src
ADIR=assets
ODIR=build

SRC=$(shell find $(SDIR) -type f -name *.cpp) 
OBJ=$(SRC:.cpp=.o)

all: $(TARGET)

.PHONY: default
$(TARGET): $(OBJ) 
	mkdir -p build 
	cp -rf $(ADIR) $(ODIR)/
	$(CC) -o $(ODIR)/$@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< 

run: 
	$(ODIR)/$(TARGET)

.PHONY: clean 
clean: 
	rm -f $(ODIR)/$(TARGET) $(OBJ)

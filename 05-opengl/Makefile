CC=g++
CFLAGS=`pkg-config --cflags glfw3 glew`
LDFLAGS=`pkg-config --libs glfw3 glew`
INCLUDE=/usr/include/stb/
TARGET=opengl
SDIR=src
ADIR=assets
ODIR=build

SRC=$(shell find $(SDIR) -type f -name *.cpp) 
OBJ=$(SRC:.cpp=.o)

all: $(TARGET)

.PHONY: default
$(TARGET): $(OBJ) 
	mkdir -p build 
	cp -rf $(ADIR) $(ODIR)/$(ADIR)
	$(CC) -o $(ODIR)/$@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< 

run: 
	$(ODIR)/$(TARGET)

.PHONY: clean 
clean: 
	rm -f $(ODIR)/$(TARGET) $(OBJ)

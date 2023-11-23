CC=gcc 
CFLAGS=`pkg-config --cflags sdl2 SDL2_net`
LDFLAGS=`pkg-config --libs sdl2 SDL2_net`
TARGET=server
SDIR=src
ADIR=assets
ODIR=build

SRC=$(shell find $(SDIR) -type f -name *.c) 
OBJ=$(SRC:.c=.o)

all: $(TARGET)

.PHONY: default
$(TARGET): $(OBJ) 
	mkdir -p build 
	cp -rf $(ADIR) $(ODIR)/$(ADIR)
	$(CC) -o $(ODIR)/$@ $^ $(LDFLAGS)

%.o: %.c 
	$(CC) $(CFLAGS) -o $@ -c $< 

run: 
	$(ODIR)/$(TARGET)

.PHONY: clean 
clean: 
	rm -f $(ODIR)/$(TARGET) $(OBJ)

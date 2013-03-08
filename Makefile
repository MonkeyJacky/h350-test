#CC = mipsel-linux-gcc
#TOOLCHAINDIR := /opt/mipsel-linux-uclibc
CC =gcc

CFLAGS = -g -Wall -Wno-pointer-sign  -I$(TOOLCHAINDIR)/usr/include/SDL

LDFLAG := -L./ -lSDL -lSDL_ttf -lSDL_gfx -lSDL_image\
    -lSDL_mixer

BINARY = ./test.bin
OBJS= 

all: $(BINARY)

OBJECTFILES=$(OBJS)main.o $(OBJS)wejpconfig.o $(OBJS)crc.o $(OBJS)sdl_shape.o
	
$(BINARY): $(OBJECTFILES)
	@echo -e "Linking \033[1m$(BINARY)\033[0m"
	$(CC) -o $(BINARY) $(OBJECTFILES) $(LDFLAG) 
	#mipsel-linux-strip $(BINARY) $(BINARY)

%.o: src/%.c
	@echo -e "Compiling \033[1m$<\033[0m"
	@$(CC) -fPIC $(CFLAGS) -c -o $@ $<

clean:
	@rm -f $(OBJECTFILES)
	@rm -f $(BINARY)

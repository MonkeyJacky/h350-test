CC = mipsel-linux-gcc
TOOLCHAINDIR := /opt/mipsel-linux-uclibc
#CC =gcc

CFLAGS = -g -Wall -Wno-pointer-sign  -I$(TOOLCHAINDIR)/usr/include/SDL

LDFLAG := -L./ -lSDL -lSDL_ttf -lSDL_gfx -lSDL_image\
    -lSDL_mixer

BINARY = ./test.dge
OBJS=

all: $(BINARY)

OBJECTFILES=$(OBJS)main.o $(OBJS)wejpconfig.o $(OBJS)crc.o $(OBJS)sdl_shape.o\
	    $(OBJS)init_parameters.o $(OBJS)key_test.o $(OBJS)wifi_test.o\
	    $(OBJS)lcd_test.o $(OBJS)tfcard_test.o $(OBJS)battery_test.o\
	    $(OBJS)internal_card_test.o $(OBJS)tvout_test.o\
	    $(OBJS)sound_test.o
	
$(BINARY): $(OBJECTFILES)
	@echo -e "Linking \033[1m$(BINARY)\033[0m"
	$(CC) -o $(BINARY) $(OBJECTFILES) $(LDFLAG) 
	mipsel-linux-strip $(BINARY) $(BINARY)

%.o: src/%.c
	@echo -e "Compiling \033[1m$<\033[0m"
	@$(CC) -fPIC $(CFLAGS) -c -o $@ $<

clean:
	@rm -f $(OBJECTFILES)
	@rm -f $(BINARY)

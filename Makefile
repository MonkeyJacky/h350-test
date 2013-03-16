TARGET     := test.dge
BUILD      := build
TOOLCHAINDIR := /opt/mipsel-linux-uclibc
SOURCES    := src
BINPATH    := ${TOOLCHAINDIR}/usr/bin
LIBPATH    := $(TOOLCHAINDIR)/usr/lib

ARCH	:= mipsel-linux-
CC	:= ${BINPATH}/$(ARCH)gcc
CFLAGS	:= -O3  -Wall -s -fomit-frame-pointer -funroll-loops  -I$(TOOLCHAINDIR)/usr/include -I$(TOOLCHAINDIR)/usr/include/SDL
LDFLAG := -s -lSDLmain -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lconfuse -lpthread

ifneq ($(notdir $(CURDIR)), $(BUILD))

export INCLUDE	:= $(foreach dir, $(BUILD), -I$(CURDIR)/$(dir))
export LIBDIRS	:= $(foreach dir, $(LIBPATH) $(BUILD), -L$(CURDIR)/$(dir))
export OUTPUT	:= $(CURDIR)/$(TARGET)
export VPATH    := $(foreach dir, $(SOURCES), $(CURDIR)/$(dir))
CFILES   := $(foreach dir, $(SOURCES), $(notdir $(wildcard $(dir)/*.c)))

export OFILES := $(CFILES:.c=.o)

.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

rebuild: clean $(BUILD)

clean:
	@echo clean ... $(TARGET)
	@rm -fr $(BUILD) $(TARGET)

else

DEPENDS := $(OFILES:.o=.d)


$(OUTPUT): $(OFILES) 
	$(CC) $^ $(LIBDIRS) $(LDFLAG) -o $@

%.o : %.c
	@$(CC) -MM $(CFLAGS) $(INCLUDE) -o $*.d $<
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@


-include $(DEPENDS)

endif


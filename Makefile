GCC = nspire-gcc
LD = nspire-ld
LIBS = -lm
GCCFLAGS = -O3 -Wall -W
OBJCOPY := "$(shell which arm-elf-objcopy 2>/dev/null)"
ifeq (${OBJCOPY},"")
	OBJCOPY := arm-none-eabi-objcopy
endif
EXE = jpeg.tns
OBJS = stb_image.o main.o frame.o file.o timer.o
DISTDIR = .
vpath %.tns $(DISTDIR)

all: $(EXE)

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $<

$(EXE): $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(@:.tns=.elf)
	$(OBJCOPY) -O binary $(@:.tns=.elf) $@

clean:
	rm -f *.o *.elf
	rm -f $(EXE)

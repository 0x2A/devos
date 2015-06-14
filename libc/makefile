
SRCEXT   = c
SRCDIR   = src
OBJDIR   = ../build

#determine target names and src paths
SRCS    := $(shell find $(SRCDIR) -name '*.c')
ASMSRCS    := $(shell find $(SRCDIR) -name '*.asm')
OBJS    = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
ASMOBJS    = $(patsubst $(SRCDIR)/%.asm,$(OBJDIR)/%.o,$(ASMSRCS))

NASM	= nasm
CC      = /usr/local/cross/bin/i686-elf-gcc
INC_PATH = -I./include -I../kernel/include
CFLAGS  = -fomit-frame-pointer -fno-stack-protector  -finline-functions -nostdlib -fno-builtin $(INC_PATH)
NASMFLAGS = -felf32

all: $(OBJS) $(ASMOBJS)

#rule for c files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@
	
#rule for asm files, compiling with nasm
$(OBJDIR)/%.o: $(SRCDIR)/%.asm
	@echo "Compiling $<..."
	$(NASM) $(NASMFLAGS) -o $@ $<


clean:
	rm -rf $(OBJECTS) build
	find . -name "*~" -exec rm {} \;
	find . -name "*.o" -exec rm {} \;
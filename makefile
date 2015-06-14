
OUTPUTNAME = kernel.bin
BUILDDIR = build

LD	= /usr/local/cross/bin/i686-elf-ld
LDFLAGS = -T link.ld -L/cross/lib/gcc/i686-elf/5.1.0 -o $(BUILDDIR)/$(OUTPUTNAME) 

build: compileall
	@echo "linking..."
	$(LD) $(LDFLAGS) $(shell find $(BUILDDIR) -name '*.o') -lgcc
	
compileall:
	@mkdir -p $(BUILDDIR)
	
	@echo "compiling kernel"
	@cd kernel; make
	@echo "compiling drivers"
	@cd drivers; make
	@echo "compiling system"
	@cd libc; make


clean:
	rm -rf $(BUILDDIR)
	find . -name "*.o" -exec rm {} \;
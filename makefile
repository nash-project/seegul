CPP=clang++
SRC=./src

INCLUDES=-I$(SRC)/include

CPPFLAGS=-Wall -Wextra $(INCLUDES) -g3
LDFLAGS=


TARGET=seegul
BUILDDIR=bin

CPPSOURCES=$(shell find $(SRC) -name '*.cc')
OBJECTS = $(patsubst $(SRC)/%.cc, $(BUILDDIR)/%.o, $(CPPSOURCES))



.PHONY: all build clean run dirs


all: build

build: dirs $(OBJECTS) $(TARGET)

$(TARGET): $(OBJECTS)

	@$(CPP) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o $(TARGET)

$(BUILDDIR)/%.o: $(SRC)/%.cc
	@echo "[$(CC)]===>[$<]->[$@]"
	@$(CC) $(CPPFLAGS) -c -o $@ $<

clean:
	-@rm -rf $(OBJECTS) $(TARGET) $(BUILDDIR) temp.o

dirs:
	@mkdir -p $(BUILDDIR)
	@cd $(SRC) \
	&& dirs=$$(find -type d) \
	&& cd ../$(BUILDDIR) \
	&& mkdir -p $$dirs

run: build
	./$(TARGET)


link: run
	ld -m elf_i386 -s -o temp temp.o

disa: link
	objdump -d temp
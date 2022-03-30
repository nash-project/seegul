CPP=clang++
SRC=./src

INCLUDES=-I./include

CPPFLAGS=-Wall -Wextra $(INCLUDES) -g3


BUILDDIR=bin

CPPSOURCES=$(shell find $(SRC) -name '*.cc')
OBJECTS = $(patsubst $(SRC)/%.cc, $(BUILDDIR)/%.o, $(CPPSOURCES))

.PHONY: all build clean dirs

all: build

build: dirs $(OBJECTS)


$(BUILDDIR)/%.o: $(SRC)/%.cc
	@echo "[$(CC)]===>[$<]->[$@]"
	@$(CC) $(CPPFLAGS) -c -o $@ $<

clean:
	-@rm -rf $(OBJECTS) $(TARGET) $(BUILDDIR) temp.o seegul.a

dirs:
	@mkdir -p $(BUILDDIR)
	@cd $(SRC) \
	&& dirs=$$(find -type d) \
	&& cd ../$(BUILDDIR) \
	&& mkdir -p $$dirs

build-lib: build
	ar rcs seegul.a $(shell find $(BUILDDIR) -type f -name "*.o")
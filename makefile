CC = gcc
CFLAGS = -I.

ODIR=obj
dummy_build_folder := $(shell mkdir -p $(ODIR))

_OBJ = main.o itertools.o ll.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

_OBJPERM = permtest.o itertools.o
OBJPERM = $(patsubst %, $(ODIR)/%, $(_OBJPERM))

_OBJCOMB = combtest.o itertools.o
OBJCOMB = $(patsubst %, $(ODIR)/%, $(_OBJCOMB))

$(ODIR)/%.o: %.c itertools.h
	$(CC) -c --std=c11 -Wall -o $@ $< -g

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -g

permtest: $(OBJPERM)
	$(CC) -o $@ $^ $(CFLAGS) -g

combtest: $(OBJCOMB)
	$(CC) -o $@ $^ $(CFLAGS) -g

clean:
	rm $(ODIR)/*.o

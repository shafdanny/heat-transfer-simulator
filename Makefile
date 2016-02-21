CC = gcc
CFLAGS = -std=c99 -g -Wall

MKDIR_P = @mkdir -p

DEPS = 

SRCDIR = src
OBJDIR = bin/obj
BINDIR = bin

_OBJ = main.o
OBJ = $(patsubst %, $(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(CFLAGS) -c -o $@ $<

all: dir main

dir:
	$(MKDIR_P) $(BINDIR) $(OBJDIR)

main: $(OBJ)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^


clean: all
	-rm -f $(OBJDIR)/*.o a.out

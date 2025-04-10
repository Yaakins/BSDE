CC=gcc

BUILDDIR=./build
SRCDIR=./src
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))

OBJFLAGS=-c -I include
CFLAGS=
DEBUGFLAGS=-g

.PHONY: all run clean

all: $(OBJS)
	gcc $(OBJS) -o $(BUILDDIR)/main $(CFLAGS)

run:
	sudo $(BUILDDIR)/main

clean:
ifneq (,$(wildcard $(BUILDDIR)/*))
	rm $(BUILDDIR)/*
endif

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	gcc $< -o $@ $(OBJFLAGS)

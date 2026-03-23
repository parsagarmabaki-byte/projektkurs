# =========================
# Project: projektkurs
# SDL2 + SDL2_image
# Works on MSYS2 MinGW64 / Windows
# =========================

CC      := gcc
TARGET  := projektkurs.exe
SRCDIR  := source
OBJDIR  := build

SRC     := $(SRCDIR)/main.c
OBJ     := $(OBJDIR)/main.o

CFLAGS  := -g -Wall -Wextra -I/mingw64/include/SDL2
LDFLAGS := -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lm

.PHONY: all clean run

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJ): $(SRC) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)
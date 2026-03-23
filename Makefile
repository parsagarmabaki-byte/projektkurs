# Detektera OS
ifeq ($(OS),Windows_NT)
    PLATFORM = windows
else
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Darwin)
        PLATFORM = mac
    else
        PLATFORM = linux
    endif
endif

# ─── Gemensamt ───────────────────────────────────────────
CC      = gcc
SRCDIR  = ./source
INCDIR  = ./include
CFLAGS  = -g -c
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lm

# ─── Per plattform ───────────────────────────────────────
ifeq ($(PLATFORM),mac)
    CC      = gcc-15
    TARGET  = main
    CFLAGS += -I/opt/homebrew/include/SDL2
    LDFLAGS += -L/opt/homebrew/lib

else ifeq ($(PLATFORM),linux)
    CC      = gcc
    TARGET  = main
    CFLAGS += -I/usr/include/SDL2

else ifeq ($(PLATFORM),windows)
    CC      = gcc
    TARGET  = main.exe
    CFLAGS += -I/mingw64/include/SDL2
    LDFLAGS += -L/mingw64/lib
endif

# ─── Bygg-regler ─────────────────────────────────────────
$(TARGET): main.o
	$(CC) main.o -o $(TARGET) $(LDFLAGS)

main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(SRCDIR)/main.c

# ─── Clean ───────────────────────────────────────────────
clean:
ifeq ($(PLATFORM),windows)
	del /Q $(TARGET) *.o
else
	rm -f $(TARGET) *.o
endif
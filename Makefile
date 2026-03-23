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
TARGET  = simpleSDLexample1
CFLAGS  = -g -c
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lm

# ─── Per plattform ───────────────────────────────────────
ifeq ($(PLATFORM),mac)
    CC      = gcc-15   # Ändra till gcc-13 eller gcc-14 beroende på version
    CFLAGS += -I/opt/homebrew/include/SDL2
    LDFLAGS += -L/opt/homebrew/lib

else ifeq ($(PLATFORM),linux)
    CC      = gcc
    CFLAGS += -I/usr/include/SDL2
    # LDFLAGS behöver ingen -L på Linux vanligtvis

else ifeq ($(PLATFORM),windows)
    CC      = gcc
    CFLAGS += -IC:/msys64/mingw64/include/SDL2   # Justera sökväg
    LDFLAGS += -LC:/msys64/mingw64/lib
    TARGET  = simpleSDLexample1.exe
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
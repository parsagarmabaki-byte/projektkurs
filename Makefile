# =========================
# Project: among_us
# SDL2 + SDL2_image
# =========================

# ─── Detektera OS ────────────────────────────────────────
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
SRCDIR  = source
OBJDIR  = build
SRC     = $(SRCDIR)/main.c
OBJ     = $(OBJDIR)/main.o
CFLAGS  = -g -c
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lm

# ─── Per plattform ───────────────────────────────────────
ifeq ($(PLATFORM),mac)
    CC      = gcc-15
    TARGET  = among_us
    CFLAGS += -I/opt/homebrew/include
    LDFLAGS += -L/opt/homebrew/lib

else ifeq ($(PLATFORM),linux)
    CC      = gcc
    TARGET  = among_us
    CFLAGS += -I/usr/include

else ifeq ($(PLATFORM),windows)
    CC       = gcc
    TARGET   = among_us.exe
    CFLAGS  += -I/mingw64/include
    LDFLAGS += -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lm
endif

# ─── Bygg-regler ─────────────────────────────────────────
all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJ): $(SRC) | $(OBJDIR)
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
ifeq ($(PLATFORM),windows)
	del /Q $(TARGET)
	rmdir /S /Q $(OBJDIR)
else
	rm -rf $(OBJDIR) $(TARGET)
endif
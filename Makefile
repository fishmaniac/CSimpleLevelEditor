CC := gcc
CFLAGS := -Isrc/include
LDFLAGS := -Lsrc/lib
LIBS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

all: SimpleLevelEditor

SimpleLevelEditor: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS) -Wall

.PHONY: clean

clean:
	rm -f SimpleLevelEditor

check:
	@echo Checking SDL2_ttf library...
	@if exist src\lib\libSDL2_ttf.a ( \
		echo SDL2_ttf library found. \
	) else ( \
		echo SDL2_ttf library not found. \
	)

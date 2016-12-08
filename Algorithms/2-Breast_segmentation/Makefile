
VPATH = includes
CC=gcc
LDFLAGS=-g -I includes
SOURCES=test.c bmplib.c AdrizDrawing.c tahomabold_20.c tahomabold_32.c extras.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=test
ifeq ($(OS),Windows_NT)
   RM = del /Q
   FLAGS2= -lmingw32 -lSDLmain -lSDL -lpthread -lSDL_mixer
else
   RM = rm -rf
   FLAGS2= -lSDLmain -lSDL -lpthread -lSDL_mixer
endif

all: clean $(EXECUTABLE) cleano_bjetcs

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).exe
	
cleano_bjetcs:
	$(RM) $(OBJECTS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@  $(FLAGS2) -lm -O2

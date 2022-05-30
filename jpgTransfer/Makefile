.SUFFIXES : .c .o

CC = gcc
CXX = g++
PROGRAM = bmp2jpeg.bin
CFLAGS = -g

LDFLAGS = -lm
INCLUDES = 

SRCS = main.c \
		bmp.c \
		jpegenc.c \
		jpeg_exp.c

OBJS = main.o \
		bmp.o \
		jpegenc.o \
		jpeg_exp.c

all: $(PROGRAM)

bmp2jpeg.bin: $(OBJS) $(SRCS) Makefile
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS) 
 
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ${*F}.c -o ${*F}.o

clean:
	rm -f $(PROGRAM) *.o

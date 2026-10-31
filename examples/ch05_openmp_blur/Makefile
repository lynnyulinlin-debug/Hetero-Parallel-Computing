# ch05_openmp_blur Makefile

CC      ?= gcc
CFLAGS  ?= -O3 -std=c11 -Wall -Wextra -pedantic -fopenmp -I../common
LDLIBS  ?= -lm

TARGET  = openmp_blur
SRC     = main.c blur_serial.c blur_openmp.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

.PHONY: run clean
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

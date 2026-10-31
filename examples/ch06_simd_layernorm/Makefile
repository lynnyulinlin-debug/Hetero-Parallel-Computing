# ch06_simd_layernorm Makefile

CC      ?= gcc
CFLAGS  ?= -O3 -std=c11 -Wall -Wextra -pedantic -fopenmp -march=native -I../common
LDLIBS  ?= -lm

TARGET  = simd_layernorm
SRC     = main.c layernorm_serial.c layernorm_simd.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

.PHONY: run clean
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

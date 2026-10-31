# ch04_task_pool Makefile

CC      ?= gcc
CFLAGS  ?= -O3 -std=c11 -Wall -Wextra -pedantic -pthread -I../common
LDLIBS  ?= -lm

TARGET  = task_pool
SRC     = main.c task_pool.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

.PHONY: run clean
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

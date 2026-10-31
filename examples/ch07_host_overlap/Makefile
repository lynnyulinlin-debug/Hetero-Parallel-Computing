# ch07_host_overlap Makefile
# 默认仅构建 CPU 版本；启用 CUDA：make USE_CUDA=1

CC      ?= gcc
NVCC    ?= nvcc
CFLAGS  ?= -O3 -std=c11 -Wall -Wextra -pedantic -D_POSIX_C_SOURCE=200809L -I../common
NVFLAGS ?= -O3 -std=c++14
LDLIBS  ?= -lm

TARGET = host_overlap
SRC    = main.c pipeline_serial.c

ifeq ($(USE_CUDA),1)
SRC     += pipeline_cuda.cu
$(TARGET): $(SRC)
	$(NVCC) $(NVFLAGS) -DUSE_CUDA $(SRC) -o $(TARGET) $(LDLIBS)
else
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)
endif

.PHONY: run clean
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

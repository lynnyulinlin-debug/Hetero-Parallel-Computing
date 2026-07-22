# ch09_reduce Makefile
# 默认仅构建 CPU 版本（串行 + OpenMP），无需 CUDA 工具链即可运行。
# 启用 CUDA：  make USE_CUDA=1

CC      ?= gcc
NVCC    ?= nvcc
CFLAGS  ?= -O3 -fopenmp -Wall -Wextra
NVFLAGS ?= -O3

COMMON_SRC = reduce_serial.c reduce_openmp.c main.c
TARGET     = reduce

ifeq ($(USE_CUDA),1)
$(TARGET): $(COMMON_SRC) reduce_cuda_naive.cu reduce_cuda_shared.cu
	$(NVCC) $(NVFLAGS) -DUSE_CUDA -Xcompiler -fopenmp \
		$(COMMON_SRC) reduce_cuda_naive.cu reduce_cuda_shared.cu -o $(TARGET)
else
$(TARGET): $(COMMON_SRC)
	$(CC) $(CFLAGS) $(COMMON_SRC) -o $(TARGET)
endif

.PHONY: run clean
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

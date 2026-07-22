# ch09_vec_add Makefile
# 默认仅构建 CPU 版本（串行 + OpenMP），无需 CUDA 工具链即可运行。
# 启用 CUDA：  make USE_CUDA=1
#
# 目标：
#   make            构建 vec_add（CPU-only）
#   make USE_CUDA=1 构建含 CUDA 实现的 vec_add
#   make run        构建并运行
#   make clean

CC      ?= gcc
NVCC    ?= nvcc
CFLAGS  ?= -O3 -fopenmp -Wall
NVFLAGS ?= -O3

COMMON_SRC = vec_add_serial.c vec_add_openmp.c main.c
TARGET     = vec_add

ifeq ($(USE_CUDA),1)
# 含 CUDA：用 nvcc 统一链接，主机侧文件用 -Xcompiler 传 OpenMP。
$(TARGET): $(COMMON_SRC) vec_add_cuda.cu
	$(NVCC) $(NVFLAGS) -DUSE_CUDA -Xcompiler -fopenmp \
		$(COMMON_SRC) vec_add_cuda.cu -o $(TARGET)
else
# CPU-only。
$(TARGET): $(COMMON_SRC)
	$(CC) $(CFLAGS) $(COMMON_SRC) -o $(TARGET)
endif

.PHONY: run clean
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

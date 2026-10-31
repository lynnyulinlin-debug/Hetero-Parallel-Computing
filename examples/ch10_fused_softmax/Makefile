# ch10_fused_softmax Makefile
# 默认仅构建 CPU 版本（串行 + OpenMP），无需 CUDA 工具链即可运行。
# 启用 CUDA：  make USE_CUDA=1

CC      ?= gcc
NVCC    ?= nvcc
CFLAGS  ?= -O3 -fopenmp -Wall -Wextra
NVFLAGS ?= -O3
LDLIBS  ?= -lm

COMMON_SRC = softmax_serial.c softmax_unfused.c softmax_openmp.c main.c
TARGET     = fused_softmax

ifeq ($(USE_CUDA),1)
$(TARGET): $(COMMON_SRC) softmax_cuda.cu
	$(NVCC) $(NVFLAGS) -DUSE_CUDA -Xcompiler -fopenmp \
		$(COMMON_SRC) softmax_cuda.cu -o $(TARGET) $(LDLIBS)
else
$(TARGET): $(COMMON_SRC)
	$(CC) $(CFLAGS) $(COMMON_SRC) -o $(TARGET) $(LDLIBS)
endif

.PHONY: run clean
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

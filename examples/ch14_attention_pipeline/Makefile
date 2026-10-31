# ch14_attention_pipeline Makefile
# 默认仅构建 CPU 版本（串行 + OpenMP），无需 CUDA 工具链即可运行。
# 启用 CUDA：  make USE_CUDA=1

CC      ?= gcc
NVCC    ?= nvcc
CFLAGS  ?= -O3 -fopenmp -Wall -Wextra
NVFLAGS ?= -O3
LDLIBS  ?= -lm

COMMON_SRC = attention_serial.c attention_openmp.c main.c
TARGET     = attention_pipeline

ifeq ($(USE_CUDA),1)
$(TARGET): $(COMMON_SRC) attention_cuda.cu
	$(NVCC) $(NVFLAGS) -DUSE_CUDA -Xcompiler -fopenmp \
		$(COMMON_SRC) attention_cuda.cu -o $(TARGET) $(LDLIBS)
else
$(TARGET): $(COMMON_SRC)
	$(CC) $(CFLAGS) $(COMMON_SRC) -o $(TARGET) $(LDLIBS)
endif

.PHONY: run clean
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

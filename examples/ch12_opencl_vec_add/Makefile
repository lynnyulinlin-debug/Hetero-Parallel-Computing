CC ?= cc
CFLAGS ?= -O2 -std=c11 -Wall -Wextra -Wpedantic
OPENCL_LIBS ?= -lOpenCL
TARGET := opencl_vec_add

$(TARGET): main.c vec_add_serial.c vec_add_opencl.c vec_add.h vec_add.cl
	$(CC) $(CFLAGS) -DCL_TARGET_OPENCL_VERSION=120 \
		-DHETEROPAR_OPENCL_KERNEL_PATH='"vec_add.cl"' \
		main.c vec_add_serial.c vec_add_opencl.c -o $@ $(OPENCL_LIBS)

.PHONY: run clean
run: $(TARGET)
	./$(TARGET) 4099

clean:
	rm -f $(TARGET)

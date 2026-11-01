#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include "vec_add.h"

static char *load_source(size_t *size) {
    FILE *file = fopen(HETEROPAR_OPENCL_KERNEL_PATH, "rb");
    if (!file) return NULL;
    fseek(file, 0, SEEK_END); *size = (size_t)ftell(file); rewind(file);
    char *source = malloc(*size + 1);
    if (source) { fread(source, 1, *size, file); source[*size] = '\0'; }
    fclose(file); return source;
}

int vec_add_opencl(const float *a, const float *b, float *c, int n) {
    cl_platform_id platform; cl_device_id device; cl_int err;
    if (clGetPlatformIDs(1, &platform, NULL) != CL_SUCCESS ||
        clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL) != CL_SUCCESS) {
        printf("OPENCL SKIP: no OpenCL platform or device available\n"); return 1;
    }
    char name[128] = {0}; clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(name), name, NULL);
    printf("OpenCL device: %s\n", name);
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);
    size_t bytes = (size_t)n * sizeof(float), source_size; char *source = load_source(&source_size);
    if (!context || !queue || !source) { free(source); if (queue) clReleaseCommandQueue(queue); if (context) clReleaseContext(context); return -1; }
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source, &source_size, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) { char log[2048] = {0}; clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(log), log, NULL); fprintf(stderr, "OpenCL build failed:\n%s\n", log); free(source); clReleaseProgram(program); clReleaseCommandQueue(queue); clReleaseContext(context); return -1; }
    cl_kernel kernel = clCreateKernel(program, "vec_add", &err);
    cl_mem da = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, bytes, (void *)a, &err);
    cl_mem db = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, bytes, (void *)b, &err);
    cl_mem dc = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, &err);
    size_t global = (size_t)n;
    err |= clSetKernelArg(kernel, 0, sizeof(da), &da); err |= clSetKernelArg(kernel, 1, sizeof(db), &db);
    err |= clSetKernelArg(kernel, 2, sizeof(dc), &dc); err |= clSetKernelArg(kernel, 3, sizeof(n), &n);
    err |= clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
    err |= clEnqueueReadBuffer(queue, dc, CL_TRUE, 0, bytes, c, 0, NULL, NULL);
    clReleaseMemObject(da); clReleaseMemObject(db); clReleaseMemObject(dc); clReleaseKernel(kernel);
    clReleaseProgram(program); clReleaseCommandQueue(queue); clReleaseContext(context); free(source);
    return err == CL_SUCCESS ? 0 : -1;
}

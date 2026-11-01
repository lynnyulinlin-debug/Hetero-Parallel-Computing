#ifndef CH12_OPENCL_VEC_ADD_H
#define CH12_OPENCL_VEC_ADD_H

void vec_add_serial(const float *a, const float *b, float *c, int n);
int vec_add_opencl(const float *a, const float *b, float *c, int n);

#endif

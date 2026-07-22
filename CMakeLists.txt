cmake_minimum_required(VERSION 3.18)
project(heteropar_tutorial LANGUAGES C)

# 统一构建（可选）。初学者也可直接进各示例目录用其 Makefile。
#
# CPU-only（默认）：
#   cmake -S . -B build && cmake --build build
# 启用 CUDA 示例：
#   cmake -S . -B build -DHETEROPAR_ENABLE_CUDA=ON && cmake --build build

option(HETEROPAR_ENABLE_CUDA "构建 CUDA 示例（需 CUDA Toolkit）" OFF)

find_package(OpenMP REQUIRED)

if(HETEROPAR_ENABLE_CUDA)
    enable_language(CUDA)
endif()

# 共享头文件（计时/校验/CUDA 辅助）。
add_library(heteropar_common INTERFACE)
target_include_directories(heteropar_common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/examples/common)

# --- ch09_vec_add ---------------------------------------------------------
set(VEC_ADD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/ch09_vec_add)
if(HETEROPAR_ENABLE_CUDA)
    add_executable(vec_add
        ${VEC_ADD_DIR}/main.c
        ${VEC_ADD_DIR}/vec_add_serial.c
        ${VEC_ADD_DIR}/vec_add_openmp.c
        ${VEC_ADD_DIR}/vec_add_cuda.cu)
    target_compile_definitions(vec_add PRIVATE USE_CUDA)
    set_target_properties(vec_add PROPERTIES CUDA_ARCHITECTURES "native")
else()
    add_executable(vec_add
        ${VEC_ADD_DIR}/main.c
        ${VEC_ADD_DIR}/vec_add_serial.c
        ${VEC_ADD_DIR}/vec_add_openmp.c)
endif()
target_link_libraries(vec_add PRIVATE heteropar_common OpenMP::OpenMP_C)

# 后续示例（ch09_reduce / ch11 / ch14）填充后在此追加 target。

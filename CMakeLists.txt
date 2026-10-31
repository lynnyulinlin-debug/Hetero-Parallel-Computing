cmake_minimum_required(VERSION 3.18)
project(heteropar_tutorial LANGUAGES C)

# CMake/CTest 是仓库级入口；各示例目录的 Makefile 只保留为教学快捷入口。
option(HETEROPAR_BUILD_HOST "构建 CPU / Host 示例" ON)
option(HETEROPAR_ENABLE_CUDA "构建可选 CUDA 示例与硬件正确性测试" OFF)
option(HETEROPAR_BUILD_SIMULATED_DEPLOYMENT "注册 Python 教学部署契约测试" ON)
option(HETEROPAR_ENABLE_TESTS "启用 CTest 正确性测试" ON)
option(HETEROPAR_NATIVE_ARCH "启用本机架构优化（-march=native）" OFF)

if(HETEROPAR_ENABLE_TESTS)
    include(CTest)
endif()

if(NOT HETEROPAR_BUILD_HOST)
    message(FATAL_ERROR "当前仓库至少需要 HETEROPAR_BUILD_HOST=ON 才有可构建示例")
endif()

find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

if(HETEROPAR_ENABLE_CUDA)
    enable_language(CUDA)
    find_package(CUDAToolkit REQUIRED)
endif()

if(HETEROPAR_BUILD_SIMULATED_DEPLOYMENT)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
endif()

add_library(heteropar_common INTERFACE)
target_include_directories(heteropar_common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/examples/common)

function(heteropar_apply_c_defaults target)
    target_compile_features(${target} PRIVATE c_std_11)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

function(heteropar_add_cpu_test target)
    if(BUILD_TESTING)
        add_test(NAME ${target}_cpu_correctness COMMAND $<TARGET_FILE:${target}> ${ARGN})
        set_tests_properties(${target}_cpu_correctness PROPERTIES
            LABELS "cpu;correctness"
            ENVIRONMENT "OMP_NUM_THREADS=2"
            TIMEOUT 60)
    endif()
endfunction()

function(heteropar_add_cuda_test target)
    if(BUILD_TESTING)
        add_test(NAME ${target}_cuda_correctness COMMAND $<TARGET_FILE:${target}> ${ARGN})
        set_tests_properties(${target}_cuda_correctness PROPERTIES
            LABELS "cuda;correctness;hardware"
            SKIP_REGULAR_EXPRESSION "CUDA executable but no available device"
            TIMEOUT 60)
    endif()
endfunction()

# --- Host 控制面：第 4-7 章 -----------------------------------------------
add_executable(ch04_task_pool
    examples/ch04_task_pool/main.c
    examples/ch04_task_pool/task_pool.c)
heteropar_apply_c_defaults(ch04_task_pool)
target_link_libraries(ch04_task_pool PRIVATE heteropar_common Threads::Threads m)
heteropar_add_cpu_test(ch04_task_pool 4096 257 2 3)

add_executable(ch05_openmp_blur
    examples/ch05_openmp_blur/main.c
    examples/ch05_openmp_blur/blur_serial.c
    examples/ch05_openmp_blur/blur_openmp.c)
heteropar_apply_c_defaults(ch05_openmp_blur)
target_link_libraries(ch05_openmp_blur PRIVATE heteropar_common OpenMP::OpenMP_C m)
heteropar_add_cpu_test(ch05_openmp_blur 63 47 2)

add_executable(ch06_simd_layernorm
    examples/ch06_simd_layernorm/main.c
    examples/ch06_simd_layernorm/layernorm_serial.c
    examples/ch06_simd_layernorm/layernorm_simd.c)
heteropar_apply_c_defaults(ch06_simd_layernorm)
target_link_libraries(ch06_simd_layernorm PRIVATE heteropar_common OpenMP::OpenMP_C m)
if(HETEROPAR_NATIVE_ARCH AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ch06_simd_layernorm PRIVATE -march=native)
endif()
heteropar_add_cpu_test(ch06_simd_layernorm 17 31)

add_executable(ch07_host_overlap
    examples/ch07_host_overlap/main.c
    examples/ch07_host_overlap/pipeline_serial.c)
heteropar_apply_c_defaults(ch07_host_overlap)
target_link_libraries(ch07_host_overlap PRIVATE heteropar_common m)
heteropar_add_cpu_test(ch07_host_overlap 4099 257 2)

# --- GPU / 云端算子：CPU 参考和 OpenMP 对照（第 9、10、14 章） ------------
add_executable(ch09_vec_add
    examples/ch09_vec_add/main.c
    examples/ch09_vec_add/vec_add_serial.c
    examples/ch09_vec_add/vec_add_openmp.c)
heteropar_apply_c_defaults(ch09_vec_add)
target_link_libraries(ch09_vec_add PRIVATE heteropar_common OpenMP::OpenMP_C)
heteropar_add_cpu_test(ch09_vec_add 4099)

add_executable(ch09_reduce
    examples/ch09_reduce/main.c
    examples/ch09_reduce/reduce_serial.c
    examples/ch09_reduce/reduce_openmp.c)
heteropar_apply_c_defaults(ch09_reduce)
target_link_libraries(ch09_reduce PRIVATE heteropar_common OpenMP::OpenMP_C)
heteropar_add_cpu_test(ch09_reduce 4099)

add_executable(ch09_gemm
    examples/ch09_gemm/main.c
    examples/ch09_gemm/gemm_serial.c
    examples/ch09_gemm/gemm_openmp.c)
heteropar_apply_c_defaults(ch09_gemm)
target_link_libraries(ch09_gemm PRIVATE heteropar_common OpenMP::OpenMP_C)
heteropar_add_cpu_test(ch09_gemm 31)

add_executable(ch10_fused_softmax
    examples/ch10_fused_softmax/main.c
    examples/ch10_fused_softmax/softmax_serial.c
    examples/ch10_fused_softmax/softmax_unfused.c
    examples/ch10_fused_softmax/softmax_openmp.c)
heteropar_apply_c_defaults(ch10_fused_softmax)
target_link_libraries(ch10_fused_softmax PRIVATE heteropar_common OpenMP::OpenMP_C m)
heteropar_add_cpu_test(ch10_fused_softmax 17 31)

add_executable(ch14_attention_pipeline
    examples/ch14_attention_pipeline/main.c
    examples/ch14_attention_pipeline/attention_serial.c
    examples/ch14_attention_pipeline/attention_openmp.c)
heteropar_apply_c_defaults(ch14_attention_pipeline)
target_link_libraries(ch14_attention_pipeline PRIVATE heteropar_common OpenMP::OpenMP_C m)
heteropar_add_cpu_test(ch14_attention_pipeline 17 31)

if(HETEROPAR_ENABLE_CUDA)
    # Task 3: CUDA targets retain the same C driver and CPU reference as their
    # CPU counterparts, so correctness remains comparable across implementations.
    add_executable(ch09_vec_add_cuda
        examples/ch09_vec_add/main.c
        examples/ch09_vec_add/vec_add_serial.c
        examples/ch09_vec_add/vec_add_openmp.c
        examples/ch09_vec_add/vec_add_cuda.cu)
    heteropar_apply_c_defaults(ch09_vec_add_cuda)
    target_compile_definitions(ch09_vec_add_cuda PRIVATE USE_CUDA)
    target_link_libraries(ch09_vec_add_cuda PRIVATE heteropar_common OpenMP::OpenMP_C CUDA::cudart)
    heteropar_add_cuda_test(ch09_vec_add_cuda 4099)

    add_executable(ch09_reduce_cuda
        examples/ch09_reduce/main.c
        examples/ch09_reduce/reduce_serial.c
        examples/ch09_reduce/reduce_openmp.c
        examples/ch09_reduce/reduce_cuda_naive.cu
        examples/ch09_reduce/reduce_cuda_shared.cu)
    heteropar_apply_c_defaults(ch09_reduce_cuda)
    target_compile_definitions(ch09_reduce_cuda PRIVATE USE_CUDA)
    target_link_libraries(ch09_reduce_cuda PRIVATE heteropar_common OpenMP::OpenMP_C CUDA::cudart)
    heteropar_add_cuda_test(ch09_reduce_cuda 4099)

    add_executable(ch09_gemm_cuda
        examples/ch09_gemm/main.c
        examples/ch09_gemm/gemm_serial.c
        examples/ch09_gemm/gemm_openmp.c
        examples/ch09_gemm/gemm_cuda.cu)
    heteropar_apply_c_defaults(ch09_gemm_cuda)
    target_compile_definitions(ch09_gemm_cuda PRIVATE USE_CUDA)
    target_link_libraries(ch09_gemm_cuda PRIVATE heteropar_common OpenMP::OpenMP_C CUDA::cudart)
    heteropar_add_cuda_test(ch09_gemm_cuda 31)
endif()

if(HETEROPAR_BUILD_SIMULATED_DEPLOYMENT AND BUILD_TESTING)
    add_test(NAME deployment_report_contract
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_deployment_contract.py)
    set_tests_properties(deployment_report_contract PROPERTIES
        LABELS "python;contract;simulated"
        TIMEOUT 30)
endif()

cmake_minimum_required(VERSION 3.10)

project(stack)

# Опция для включения покрытия кода
option(ENABLE_COVERAGE "Enable code coverage" OFF)

# Флаги компиляции для покрытия (только для GCC/Clang)
if(ENABLE_COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Code coverage enabled for GCC/Clang")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    elseif(MSVC)
        message(WARNING "Code coverage with MSVC requires external tools")
    endif()
endif()

# Опция для включения санитайзеров
option(ENABLE_SANITIZERS "Enable sanitizers" OFF)

if(ENABLE_SANITIZERS)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Sanitizers enabled")
        set(SANITIZER_FLAGS "-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZER_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER_FLAGS}")
    endif()
endif()

add_library(stack STATIC stack.c)
add_executable(main main.c)
target_link_libraries(main stack)

# Добавляем исполняемый файл для тестирования
add_executable(test_stack test_stack.cpp)
target_link_libraries(test_stack PRIVATE stack)

# Добавляем исполняемый файл для бенчмарков
add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark PRIVATE stack)

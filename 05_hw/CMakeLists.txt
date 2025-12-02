cmake_minimum_required(VERSION 3.10)

project(stack)
add_library(stack STATIC stack.c)
add_executable(main main.c)
target_link_libraries(main stack)

# Добавляем исполняемый файл для тестирования
add_executable(test_stack test_stack.cpp)
target_link_libraries(test_stack PRIVATE stack)

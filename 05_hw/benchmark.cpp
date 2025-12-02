#include <iostream>
#include <chrono>
#include <iomanip>
#include "stack.h"

// Простой класс таймера с использованием high_resolution_clock
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool running;

public:
    Timer() : running(false) {}

    // Запустить таймер
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
        running = true;
    }

    // Остановить таймер
    void stop() {
        end_time = std::chrono::high_resolution_clock::now();
        running = false;
    }

    // Получить время в миллисекундах
    double elapsed_ms() const {
        auto end = running ? std::chrono::high_resolution_clock::now() : end_time;
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_time);
        return duration.count() / 1000.0; // Конвертируем в миллисекунды
    }

    // Получить время в микросекундах
    int64_t elapsed_us() const {
        auto end = running ? std::chrono::high_resolution_clock::now() : end_time;
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start_time).count();
    }

    // Получить время в наносекундах
    int64_t elapsed_ns() const {
        auto end = running ? std::chrono::high_resolution_clock::now() : end_time;
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_time).count();
    }
};

// Бенчмарк для операции push
void benchmark_push(int num_operations) {
    Stack s;
    initStack(&s);
    
    Timer timer;
    timer.start();
    
    for (int i = 0; i < num_operations; ++i) {
        push(&s, i);
    }
    
    timer.stop();
    
    double total_ms = timer.elapsed_ms();
    double avg_ns = static_cast<double>(timer.elapsed_ns()) / num_operations;
    
    std::cout << "Push benchmark (" << num_operations << " operations):\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(3) << total_ms << " ms\n";
    std::cout << "  Average per operation: " << std::fixed << std::setprecision(2) << avg_ns << " ns\n";
    std::cout << "  Operations per second: " << std::fixed << std::setprecision(0) 
              << (num_operations / (total_ms / 1000.0)) << " ops/sec\n";
    std::cout << std::endl;
    
    // Очистка
    destroyStack(&s);
}

// Бенчмарк для операции pop
void benchmark_pop(int num_operations) {
    Stack s;
    initStack(&s);
    
    // Предварительно заполняем стек
    for (int i = 0; i < num_operations; ++i) {
        push(&s, i);
    }
    
    Timer timer;
    timer.start();
    
    for (int i = 0; i < num_operations; ++i) {
        pop(&s);
    }
    
    timer.stop();
    
    double total_ms = timer.elapsed_ms();
    double avg_ns = static_cast<double>(timer.elapsed_ns()) / num_operations;
    
    std::cout << "Pop benchmark (" << num_operations << " operations):\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(3) << total_ms << " ms\n";
    std::cout << "  Average per operation: " << std::fixed << std::setprecision(2) << avg_ns << " ns\n";
    std::cout << "  Operations per second: " << std::fixed << std::setprecision(0) 
              << (num_operations / (total_ms / 1000.0)) << " ops/sec\n";
    std::cout << std::endl;
    
    // Стек уже пуст после pop, просто очищаем
    destroyStack(&s);
}

// Бенчмарк для комбинированных операций push/pop
void benchmark_push_pop_mixed(int num_operations) {
    Stack s;
    initStack(&s);
    
    Timer timer;
    timer.start();
    
    // Чередуем push и pop
    for (int i = 0; i < num_operations; ++i) {
        push(&s, i);
        push(&s, i + 1);
        pop(&s);
    }
    
    timer.stop();
    
    double total_ms = timer.elapsed_ms();
    int total_ops = num_operations * 3; // 2 push + 1 pop на итерацию
    double avg_ns = static_cast<double>(timer.elapsed_ns()) / total_ops;
    
    std::cout << "Mixed push/pop benchmark (" << total_ops << " total operations):\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(3) << total_ms << " ms\n";
    std::cout << "  Average per operation: " << std::fixed << std::setprecision(2) << avg_ns << " ns\n";
    std::cout << "  Operations per second: " << std::fixed << std::setprecision(0) 
              << (total_ops / (total_ms / 1000.0)) << " ops/sec\n";
    std::cout << std::endl;
    
    // Очистка
    destroyStack(&s);
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  Stack Performance Benchmarks\n";
    std::cout << "========================================\n\n";
    
    // Запускаем бенчмарки с разным количеством операций
    const int small = 1000;
    const int medium = 10000;
    const int large = 100000;
    
    std::cout << "--- Small scale (1,000 operations) ---\n";
    benchmark_push(small);
    benchmark_pop(small);
    benchmark_push_pop_mixed(small);
    
    std::cout << "--- Medium scale (10,000 operations) ---\n";
    benchmark_push(medium);
    benchmark_pop(medium);
    benchmark_push_pop_mixed(medium);
    
    std::cout << "--- Large scale (100,000 operations) ---\n";
    benchmark_push(large);
    benchmark_pop(large);
    benchmark_push_pop_mixed(large);
    
    std::cout << "========================================\n";
    std::cout << "  Benchmarks completed!\n";
    std::cout << "========================================\n";
    
    return 0;
}

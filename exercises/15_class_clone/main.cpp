#include "../exercise.h"

// READ: 复制构造函数 <https://zh.cppreference.com/w/cpp/language/copy_constructor>
// READ: 函数定义（显式弃置）<https://zh.cppreference.com/w/cpp/language/function>

class DynFibonacci {
    size_t *cache;
    int cached;
    int capacity;

public:
    // 动态设置容量的构造器
    DynFibonacci(int cap) : cached(0), capacity(cap) {
        cache = new size_t[capacity]{}; // 初始化为0
        if (capacity > 0) cache[cached++] = 0;
        if (capacity > 1) cache[cached++] = 1;
    }

    // 复制构造器
    DynFibonacci(const DynFibonacci& other) : 
        capacity(other.capacity), 
        cached(other.cached) {
        cache = new size_t[capacity];
        std::copy(other.cache, other.cache + cached, cache);
    }

    // 移动构造器
    DynFibonacci(DynFibonacci&& other) noexcept :
        cache(other.cache),
        cached(other.cached),
        capacity(other.capacity) {
        other.cache = nullptr;
        other.cached = 0;
        other.capacity = 0;
    }

    // 赋值运算符重载
    DynFibonacci& operator=(const DynFibonacci& other) {
        if (this != &other) {
            delete[] cache;
            capacity = other.capacity;
            cached = other.cached;
            cache = new size_t[capacity];
            std::copy(other.cache, other.cache + cached, cache);
        }
        return *this;
    }

    // 移动赋值运算符重载
    DynFibonacci& operator=(DynFibonacci&& other) noexcept {
        if (this != &other) {
            delete[] cache;
            cache = other.cache;
            cached = other.cached;
            capacity = other.capacity;
            other.cache = nullptr;
            other.cached = 0;
            other.capacity = 0;
        }
        return *this;
    }

    // 析构器释放内存
    ~DynFibonacci() {
        delete[] cache;
    }

    size_t get(int i) {
        if (i < 0 || i >= capacity) {
            throw std::out_of_range("Index out of range");
        }
        
        for (; cached <= i && cached < capacity; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        
        return cache[i];
    }

    // const版本：仅读取已缓存的值
    size_t get(int i) const {
        if (i < 0 || i >= capacity || i >= cached) {
            return 0;
        }
        return cache[i];
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    DynFibonacci const fib_ = fib;
    ASSERT(fib_.get(10) == fib.get(10), "Object cloned");
    return 0;
}

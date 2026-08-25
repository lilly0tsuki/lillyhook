#include <vector>
#include <cstdint>

template <typename T, size_t N>
struct SmallVector : public std::vector<T> {
    T inlineBuffer[N];
    bool usingInline = N != 0;
};

template <typename T>
struct Vector : public std::vector<T> {
    int32_t allocator;
};

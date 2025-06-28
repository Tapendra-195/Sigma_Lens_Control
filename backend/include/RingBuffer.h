#pragma once
#include <cstddef>

template <typename T, size_t Size>
class RingBuffer {
public:
    RingBuffer() : head(0), tail(0), count(0) {}

    bool push(const T& value) {
        if (isFull()) return false;
        buffer[head] = value;
        head = (head + 1) % Size;
        ++count;
        return true;
    }

    bool pop(T& out) {
        if (isEmpty()) return false;
        out = buffer[tail];
        tail = (tail + 1) % Size;
        --count;
        return true;
    }

    bool peek(T& out) const {
        if (isEmpty()) return false;
        out = buffer[tail];
        return true;
    }

    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == Size; }
    size_t available() const { return count; }
    size_t capacity() const { return Size; }

private:
    T buffer[Size];
    size_t head;
    size_t tail;
    size_t count;
};

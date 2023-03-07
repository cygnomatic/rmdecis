//
// Created by catslashbin on 23-3-7.
//

#ifndef CYGNOIDES_DECISION_AVG_QUEUE_H
#define CYGNOIDES_DECISION_AVG_QUEUE_H

#include <queue>

template<class T>
class AvgQueue : public std::queue<T> {
public:
    explicit AvgQueue(std::size_t max_length) : max_length(max_length) {}

    using std::queue<T>::front;
    using std::queue<T>::back;
    using std::queue<T>::pop;
    using std::queue<T>::empty;
    using std::queue<T>::size;

    void push(const T &item) override {
        if (this->size() >= max_length)
            this->pop();
        sum_ += item;
        return std::queue<T>::push(item);
    }

    void pop() override {
        sum_ -= front;
        std::queue<T>::pop();
    }

    T getAvg() {
        return sum_ / this->size();
    }

private:
    std::size_t max_length;
    T sum_{};
};


#endif //CYGNOIDES_DECISION_AVG_QUEUE_H

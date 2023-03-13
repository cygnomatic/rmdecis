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

    //using std::queue<T>::front;
    //using std::queue<T>::back;
    //using std::queue<T>::empty;
    //using std::queue<T>::size;

    void pop() {
        sum_ -= std::queue<T>::front();
        std::queue<T>::pop();
    }

    void push(const T &item) {
        if (this->size() >= max_length)
            this->pop();
        sum_ += item;
        return std::queue<T>::push(item);
    }

    T getAvg() {
        return sum_ / this->size();
    }

    T sum_{};

private:
    std::size_t max_length;
};


#endif //CYGNOIDES_DECISION_AVG_QUEUE_H

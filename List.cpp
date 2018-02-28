#pragma once

#include <cassert>
#include <cstdlib>

using namespace std;

template <typename T>
class list_node_ {
    template <typename> friend class List;
    private:
        T val_;
        list_node_ <T> *next_;
        list_node_() {}
        list_node_(const list_node_ <T> &object) : val_(object.val_), next_(object.next_) {}
        list_node_(const T&, list_node_ <T>* const);
        ~list_node_() {}

        list_node_ <T>* next() const;
        T& value();
};

template <typename T>
list_node_ <T>::list_node_(const T& x, list_node_ <T>* const y) {
    val_ = x;
    next_ = y;
}

template <typename T>
list_node_<T>* list_node_ <T>::next() const {
    return next_;
}

template <typename T>
T& list_node_ <T>::value() {
    return val_;
}

template <typename T>
class List {
    private:
        list_node_ <T> *begin_ptr_, *end_ptr_;
        size_t size_;
    public:

//constructors&destructors
        List();
        List(const size_t, const T& = T());
        List(const List <T>&);
        ~List();

//iterators
        list_node_ <T>* begin() const;
        list_node_ <T>* end() const;

//capacity
        bool empty() const;
        size_t size() const;

//element access
        T& operator [] (const size_t);
        T& at(const size_t);

//modifiers&operators
        void clear();
        void push_back(const T&);
        void pop_back();
        void push_front(const T&);
        void pop_front();
        List <T>& operator = (const List <T>&);

//friend functions
        template <typename T1> friend void swap(List <T>&, List <T>&);

// iterator
        class iterator {
            template <typename> friend class List;
            private:
                list_node_ <T> *cur_iterator_;
            public:
                iterator() {}
                iterator(list_node_ <T>* const object) : cur_iterator_(object) {}
                iterator(const List <T>::iterator& object) : cur_iterator_(object.cur_iterator_) {};

                List <T>::iterator& operator = (list_node_ <T>* const object) {
                    cur_iterator_ = object;
                    return *this;
                }
                List <T>::iterator& operator = (const List <T>::iterator& object) {
                    cur_iterator_ = object.cur_iterator_;
                    return *this;
                }
                bool operator == (const List <T>::iterator& object) const {
                    return this -> cur_iterator_ == object.cur_iterator_;
                }
                bool operator != (const List <T>::iterator& object) const {
                    return this -> cur_iterator_ != object.cur_iterator_;
                }
                bool operator == (list_node_ <T>* const object) const {
                    return this -> cur_iterator_ == object;
                }
                bool operator != (list_node_ <T>* const object) const {
                    return this -> cur_iterator_ != object;
                }
                T& operator * () {
                    return this -> cur_iterator_ -> value();
                }
                List <T>::iterator& operator ++ () {
                    this -> cur_iterator_ = this -> cur_iterator_ -> next();
                    return *this;
                }
                List <T>::iterator operator ++ (int) {
                    iterator old(*this);
                    this -> cur_iterator_ = this -> cur_iterator_ -> next();
                    return old;
                }
        };
};

template <typename T>
List <T>::List() {
    size_ = 0;
    begin_ptr_ = end_ptr_ = nullptr;
}

template <typename T>
List <T>::List(const size_t n, const T& x) {
    if (!n) {
        begin_ptr_ = end_ptr_ = nullptr;
        size_ = 0;
        return;
    }
    auto cur = new list_node_ <T> (x, nullptr);
    begin_ptr_ = cur;
    for (int i = 1; i < n; ++i) {
        auto next = new list_node_ <T> (x, nullptr);
        cur -> next_ = next;
        cur = next;
    }
    end_ptr_ = cur;
    size_ = n;
}

template <typename T>
List <T>::List(const List <T>& object) {
    if (!object.size()) {
        begin_ptr_ = end_ptr_ = nullptr;
        size_ = 0;
        return;
    }
    List <T>::iterator it = object.begin();
    begin_ptr_ = new list_node_ <T> (*(it.cur_iterator_));
    auto cur = begin_ptr_;
    ++it;
    for (; it != object.end(); ++it) {
        auto next = new list_node_ <T> (*(it.cur_iterator_));
        cur -> next_ = next;
        cur = next;
    }
    end_ptr_ = cur;
    size_ = object.size();
}

template <typename T>
List <T>::~List() {
    auto cur = begin_ptr_;
    for (size_t i = 0; i < size_; ++i) {
        auto next = cur -> next();
        delete cur;
        cur = next;
    }
    begin_ptr_ = end_ptr_ = nullptr;
    size_ = 0;
}

template <typename T>
list_node_ <T>* List <T>::begin() const {
    return begin_ptr_;
}

template <typename T>
list_node_ <T>* List <T>::end() const {
    return end_ptr_ -> next();
}

template <typename T>
bool List <T>::empty() const {
    return size_ == 0;
}

template <typename T>
size_t List <T>::size() const {
    return size_;
}

template <typename T>
T& List<T>::operator [] (const size_t id) {
    auto cur = begin_ptr_;
    for (int i = 0; i < id; ++i)
        cur = cur -> next();
    return cur -> value();
}

template <typename T>
T& List<T>::at(const size_t id) {
    assert(id < size_);
    auto cur = begin_ptr_;
    for (int i = 0; i < id; ++i)
        cur = cur -> next();
    return cur -> value();
}

template <typename T>
void List<T>::clear() {
    auto cur = begin_ptr_;
    for (size_t i = 0; i < size_; ++i) {
        auto next = cur -> next();
        delete cur;
        cur = next;
    }
    begin_ptr_ = nullptr;
    end_ptr_ = nullptr;
    size_ = 0;
}

template <typename T>
void List<T>::push_back(const T& object) {
    ++size_;
    auto new_node = new list_node_ <T>(object, nullptr);
    if (end_ptr_)
        end_ptr_ -> next_ = new_node;
    else
        begin_ptr_ = new_node;
    end_ptr_ = new_node;
}

template <typename T>
void List<T>::pop_back() {
    auto cur = begin_ptr_;
    for (int i = 2; i < size_; ++i)
        cur = cur -> next();
    if (size_ != 1) {
        delete end_ptr_;
        end_ptr_ = cur;
        cur -> next_ = nullptr;
        --size_;
        return;
    }
    delete begin_ptr_;
    begin_ptr_ = end_ptr_ = nullptr;
    size_ = 0;
}

template <typename T>
void List<T>::push_front(const T& object) {
    ++size_;
    auto new_node = new list_node_ <T>(object, begin_ptr_);
    begin_ptr_ = new_node;
    if (size_ == 1)
        end_ptr_ = new_node;
}

template <typename T>
void List<T>::pop_front() {
    --size_;
    auto new_beg = begin_ptr_ -> next();
    delete begin_ptr_;
    begin_ptr_ = new_beg;
    if (!size_)
        end_ptr_ = nullptr;
}

template <typename T>
List <T>& List <T>::operator = (const List <T> &object) {
    List <T> res = List <T> (object);
    swap(res, *this);
    return *this;
}

template <typename T>
void swap(List <T> &x, List <T> &y) {
    swap(x.begin_ptr_, y.begin_ptr_);
    swap(x.end_ptr_, y.end_ptr_);
    swap(x.size_, y.size_);
}

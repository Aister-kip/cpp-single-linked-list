#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto this_copy(*this);
            ++(*this);
            return this_copy;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        assert(size_ == 0 && head_.next_node == nullptr);
        SingleLinkedList tmp;
        tmp.copy_(values.begin(), values.end());
        swap(tmp);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);
        SingleLinkedList tmp;
        tmp.copy_(other.begin(), other.end());
        swap(tmp);
    }

    ~SingleLinkedList() {
        Clear();
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList tmp(rhs);
            swap(tmp);
        }
        assert(this != nullptr);
        return *this;
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator end() noexcept {
        return {};
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return {};
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
    }


    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return GetSize();
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PopFront() noexcept {
        assert(IsEmpty());
        Node* first = head_.next_node;
        Node* second = first->next_node;
        delete first;
        head_.next_node = second;
        --size_;
    }

    void Clear() {
        while (head_.next_node != nullptr) {
            Node* temp_ptr = head_.next_node;
            head_.next_node = head_.next_node->next_node;;
            delete temp_ptr;
        }
        size_ = 0;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);

    }
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* new_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = new_node;
        ++size_;
        return Iterator{ new_node };
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        if (pos.node_->next_node == nullptr) {
            return {};
        }
        auto required_node = pos.node_->next_node;
        pos.node_->next_node = required_node->next_node;
        delete required_node;
        --size_;
        return Iterator{ pos.node_->next_node };
    }

private:
    Node head_ = { {}, nullptr };
    size_t size_ = 0;

    template <typename InputIterator>
    void copy_(InputIterator from, InputIterator to) {
        Clear();
        Iterator it(&head_);
        while (from != to) {
            it = InsertAfter(it, *from);
            ++from;
        }
    }
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (rhs < lhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}

#pragma once

#include <cstddef>
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
        Type value{};
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node)
            :node_(node)
        {
        }

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept
            :node_(other.node_)
        {
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept {
            assert (node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            Iterator result(node_);
            node_ = node_->next_node;
            return result;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert (node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert (node_ != nullptr);
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

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{head_.next_node};
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{nullptr};
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{nullptr};
    }

    SingleLinkedList()
        : size_(0)
        , head_()
    {
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        Assign(values.begin(), values.end());
    }

    SingleLinkedList(const SingleLinkedList& other) {
        Assign(other.begin(), other.end());
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (&rhs == this) {return *this;}
        SingleLinkedList<Type> tmp(rhs);
        swap(tmp);
        return *this;
    }
 
    void swap(SingleLinkedList& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(head_.next_node, other.head_.next_node);
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return (size_ == 0);
    }

    void PushFront(const Type& value) {
        auto node = new Node(value, head_.next_node);
        head_.next_node = node;
        ++size_;
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            auto node = head_.next_node;
            head_.next_node = node->next_node;
            delete node;
        }
        size_ = 0;
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        Iterator it(&head_);
        return it;
    }

     [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        const Iterator it(const_cast<Node*>(&head_));
        return it;
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Iterator prev{pos.node_};
        assert(prev.node_);
        auto node = new Node(value, prev.node_->next_node);
        prev.node_->next_node = node;
        Iterator it(node);
        ++size_;
        return it;

    }

    void PopFront() noexcept {
        assert(!IsEmpty());
        auto node = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = node;
        --size_;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ != nullptr);
        Iterator it{ pos.node_ };

        auto removed = it.node_->next_node;
        it.node_->next_node = it.node_->next_node->next_node;
        delete removed;
        return ++it;
    }

    ~SingleLinkedList() {
        Clear();
    }

private:

    template <typename InputIterator>
    void Assign(InputIterator from, InputIterator to) {
        SingleLinkedList<Type> tmp;
        Node** node_ptr = &tmp.head_.next_node;
        while (from != to) {
            assert(*node_ptr == nullptr);
            *node_ptr = new Node (*from,nullptr);
            ++tmp.size_;
            node_ptr = &((*node_ptr)->next_node);
            ++from;
        }
        swap(tmp);
    }

    size_t size_ = 0;
    Node head_;
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
    return !(lhs==rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}

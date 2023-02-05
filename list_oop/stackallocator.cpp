#include <iostream>
#include <cstddef>
#include <cstdio>

template <size_t N>
class StackStorage {
 public:
  size_t sz_ = 0;
  alignas(std::max_align_t) uint8_t storage[N];
};

template <typename T, size_t N>
class StackAllocator {
 public:
  StackStorage<N>* storage;

  using value_type = T;
  using pointer = T*;
  using reference = T&;

  T* allocate(size_t n) {
    if ((storage->sz_ + (alignof(T) - (storage->sz_) % alignof(T)) % alignof(T) + sizeof(T) * n) >= N) {
      throw std::bad_alloc();
    } else {
      if (reinterpret_cast<uintptr_t>((storage->storage + storage->sz_)) % alignof(T) != 0) {
        storage->sz_ += alignof(T) - reinterpret_cast<uintptr_t>((storage->storage + storage->sz_)) % alignof(T);
      }
      uint8_t* new_ptr = storage->storage + storage->sz_;
      T* new_bucket = reinterpret_cast<T*>(new_ptr);
      storage->sz_ += sizeof(T) * n;
      return new_bucket;
    }
  }

  void construct(T* point) {
    new(point) T();
  }

  void construct(T* point, const T& val) {
    new(point) T(val);
  }

  void destroy(T* point) {
    point->~T();
  }

  template <typename U>
  struct rebind {
    using other = StackAllocator<U, N>;
  };

  void deallocate(T*, size_t) {}

  template <typename U>
  StackAllocator(const StackAllocator<U, N>& alloc) {
    storage = alloc.storage;
  }

  template <typename U>
  StackAllocator& operator=(const StackAllocator<U, N>& other) {
    storage = other.storage;
    return *this;
  }

  StackAllocator(StackStorage<N>& arr) {
    storage = &arr;
  }

  template <typename U>
  bool operator==(const StackAllocator<U, N>& other) {
    return storage == other.storage;
  }

  template <typename U>
  bool operator!=(const StackAllocator<U, N>& other) {
    return !((*this) == other);
  }

};

template <bool F, class S, class U>
struct Conditional;

template <class S, class U>
struct Conditional<false, S, U> {
  using type = U;
};

template <class S, class U>
struct Conditional<true, S, U> {
  using type = S;
};

template <bool F, class S, class U>
using Conditional_val = typename Conditional<F, S, U>::type;

template <typename T, class Allocator = std::allocator<T>>
class List {
 private:

  struct BaseNode{
    BaseNode* next;
    BaseNode* prev;

    BaseNode(): next(this), prev(this){}
  };

  struct Node : BaseNode{
    T val;

    Node(){}

    Node(const T& value): val(value){}

    explicit Node(Node* Next, Node* Prev, const T& value): BaseNode::next(Next->next),
                                                          BaseNode::prev(Prev->prev),
                                                          val(value){}
  };

  using AllocTraitsT = std::allocator_traits<Allocator>;
  using AllocTraitsNode = std::allocator_traits<typename AllocTraitsT::template rebind_alloc<Node>>;
  using AllocTraitsBaseNode = std::allocator_traits<typename AllocTraitsT::template rebind_alloc<BaseNode>>;

  BaseNode* tail;
  size_t size_ = 0;
  Allocator AllocT_;
  typename AllocTraitsT::template rebind_alloc<BaseNode> BaseNodeAlloc_;
  typename AllocTraitsT::template rebind_alloc<Node> NodeAlloc_;

 public:

  template <bool IsConst>
  class common_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
   public:
    BaseNode* it_;

    using value_type = Conditional_val<IsConst, const T, T>;
    using reference = Conditional_val<IsConst, const T&, T&>;
    using pointer = Conditional_val<IsConst, const T*, T*>;

    common_iterator(const BaseNode* first_data) {
      it_ = first_data;
    }

    common_iterator(BaseNode* first_data) {
      it_ = first_data;
    }

    common_iterator(const common_iterator<false>& it) {
      it_ = it.it_;
    }

    Conditional_val<IsConst, const T&, T&> operator*() const {
      return static_cast<Node*>(it_)->val;
    }


    Conditional_val<IsConst, const T*, T*> operator->() const {
      return &static_cast<Node*>(it_)->val;
    }

    common_iterator<IsConst>& operator++() {
      it_ = it_->next;
      return *this;
    }

    common_iterator<IsConst> operator++(int) {
      common_iterator<IsConst> copy_it = *this;
      ++*this;
      return copy_it;
    }

    common_iterator<IsConst>& operator--() {
      it_ = it_->prev;
      return *this;
    }

    common_iterator<IsConst> operator--(int) {
      common_iterator<IsConst> copy_it = *this;
      --*this;
      return copy_it;
    }

    bool operator==(const common_iterator<IsConst>& other_it) const {
      return it_ == other_it.it_;
    }

    bool operator!=(const common_iterator<IsConst>& other_it) const {
      return !(*this == other_it);
    }

    Node* get_pointer() const {
      return it_;
    }
  };

  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin() {
    return iterator(tail->next); // tail->next = head
  }

  iterator end() {
    return iterator(tail);
  }

  const_iterator begin() const {
    return const_iterator(tail->next); // tail->next = head
  }

  const_iterator end() const {
    return const_iterator(tail);
  }

  const_iterator cbegin() const {
    return const_iterator(tail->next); // tail->next = head
  }

  const_iterator cend() const {
    return const_iterator(tail);
  }

  reverse_iterator rbegin() {
    return reverse_iterator(end());
  }

  reverse_iterator rend() {
    return reverse_iterator(begin()); // in list: head->prev = tail
  }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(cend());
  }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(cbegin()); // in list: head->prev = tail
  }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(--cend());
  }

  const_reverse_iterator crend() const {
    return const_reverse_iterator(cend()); // in list: head->prev = tail
  }

  List();

  explicit List(size_t N){
    size_ = N;
    tail = AllocTraitsBaseNode::allocate(BaseNodeAlloc_, 1);
    AllocTraitsBaseNode::construct(BaseNodeAlloc_, tail, BaseNode());
    auto previous = tail;
    Node* new_node;
    Node* this_node;
    for (size_t i = 0; i < N; ++i) {
      try{
        new_node = AllocTraitsNode::allocate(NodeAlloc_, 1);
        try {
          AllocTraitsNode::construct(NodeAlloc_, new_node, Node());
        } catch(...) {
          AllocTraitsNode::deallocate(NodeAlloc_, new_node);
          throw;
        }
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          this_node = static_cast<Node*>(previous);
          previous = this_node->prev;
          AllocTraitsBaseNode::destroy(NodeAlloc_,this_node);
          AllocTraitsBaseNode::deallocate(NodeAlloc_, this_node);
        }
        AllocTraitsBaseNode::destroy(BaseNodeAlloc_, tail);
        AllocTraitsBaseNode::deallocate(BaseNodeAlloc_, tail);
        throw;
      }
      new_node->next = tail;
      previous->next = new_node;
      new_node->prev = previous;
      previous = new_node;
    }
    tail->prev = previous;
  }

  explicit List(size_t N, const T& value);

  explicit List(const Allocator& alloc) :
    AllocT_(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)),
    BaseNodeAlloc_(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)),
    NodeAlloc_(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    tail = AllocTraitsBaseNode::allocate(BaseNodeAlloc_, 1);
    AllocTraitsBaseNode::construct(BaseNodeAlloc_, tail, BaseNode());
  }

  explicit List(size_t N, const Allocator& alloc) :
    AllocT_(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)),
    BaseNodeAlloc_(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)),
    NodeAlloc_(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    size_ = N;
    tail = AllocTraitsBaseNode::allocate(BaseNodeAlloc_, 1);
    AllocTraitsBaseNode::construct(BaseNodeAlloc_, tail, BaseNode());
    BaseNode* previous = tail;
    Node* new_node;
    Node* this_node;
    for (size_t i = 0; i < N; ++i) {
      try{
        new_node = AllocTraitsNode::allocate(NodeAlloc_, 1);
        try {
          AllocTraitsNode::construct(NodeAlloc_, new_node, Node());
        } catch(...) {
          AllocTraitsNode::deallocate(NodeAlloc_, new_node);
          throw;
        }
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          this_node = static_cast<Node*>(previous);
          previous = this_node->prev;
          AllocTraitsBaseNode::destroy(NodeAlloc_,this_node);
          AllocTraitsBaseNode::deallocate(NodeAlloc_, this_node);
        }
        AllocTraitsBaseNode::destroy(BaseNodeAlloc_, tail);
        AllocTraitsBaseNode::deallocate(BaseNodeAlloc_, tail);
        throw;
      }
      new_node->next = tail;
      previous->next = new_node;
      new_node->prev = previous;
      previous = new_node;
    }
    tail->prev = previous;
  }

  explicit List(size_t N, const T& value, const Allocator& alloc) :
    AllocT_(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)),
    BaseNodeAlloc_(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)),
    NodeAlloc_(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    tail = AllocTraitsBaseNode::allocate(BaseNodeAlloc_, 1);
    AllocTraitsBaseNode::construct(BaseNodeAlloc_, tail, BaseNode());
    for (size_t i = 0; i < N; ++i) {
      try {
        push_back(value);
      }
      catch (...) {
        for (size_t j = 0; j < i; ++j) {
          pop_back();
        }
        AllocTraitsBaseNode::destroy(BaseNodeAlloc_, tail);
        AllocTraitsBaseNode::deallocate(BaseNodeAlloc_, tail);
        throw;
      }
    }
  }

  List(const List& other) :
    AllocT_(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.AllocT_)),
    BaseNodeAlloc_(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.BaseNodeAlloc_)),
    NodeAlloc_(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.NodeAlloc_)) {
    tail = AllocTraitsBaseNode::allocate(BaseNodeAlloc_, 1);
    AllocTraitsBaseNode::construct(BaseNodeAlloc_, tail, BaseNode());
    auto it = other.begin();
    for (size_t i = 0; i < other.size(); ++i) {
      try {
        push_back(*it);
        ++it;
      }
      catch (...) {
        for (size_t j = 0; j < i; ++j) {
          pop_back();
        }
        AllocTraitsBaseNode::destroy(BaseNodeAlloc_, tail);
        AllocTraitsBaseNode::deallocate(BaseNodeAlloc_, tail);
        throw;
      }
    }
  }

  List& operator=(const List& other) {
    if (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value) {
      AllocT_ = other.AllocT_;
      BaseNodeAlloc_ = other.BaseNodeAlloc_;
      NodeAlloc_ = other.NodeAlloc_;
    } else {
      AllocT_ = std::allocator_traits<Allocator>::select_on_container_copy_construction(other.AllocT_);
      BaseNodeAlloc_ = std::allocator_traits<Allocator>::select_on_container_copy_construction(other.BaseNodeAlloc_);
      NodeAlloc_ = std::allocator_traits<Allocator>::select_on_container_copy_construction(other.NodeAlloc_);
    }
    if (!tail) {
      tail = AllocTraitsBaseNode::allocate(BaseNodeAlloc_, 1);
      AllocTraitsBaseNode::construct(BaseNodeAlloc_, tail, BaseNode());
    }
    auto it = other.begin();
    for (size_t i = 0; i < other.size(); ++i) {
      try {
        push_back(*it);
        ++it;
      }
      catch (...) {
        for (size_t j = 0; j < i; ++j) {
          pop_back();
        }
        throw;
      }
    }
    while (size_ > other.size_) {
      pop_front();
    }
    return *this;
  }

  ~List() {
    size_t sz = size_;
    for (size_t i = 0; i < sz; ++i) {
      pop_back();
    }
    AllocTraitsBaseNode::destroy(BaseNodeAlloc_, tail);
    AllocTraitsBaseNode::deallocate(BaseNodeAlloc_, tail);
  }

  Allocator get_allocator() const {
    return AllocT_;
  }

  size_t size() const {
    return size_;
  }

  void push_back(const T& val) {
    insert(end(), val);
  }

  void push_front(const T& val) { insert(begin(), val); }

  void pop_front() { erase(begin()); }

  void pop_back() { erase(iterator(tail->prev)); }

  void insert(const const_iterator& it, const T& val) {
    Node* new_node;
    new_node = AllocTraitsNode::allocate(NodeAlloc_, 1);
    try {
      AllocTraitsNode::construct(NodeAlloc_, new_node, Node(val));
    } catch (...) {
      AllocTraitsNode::deallocate(NodeAlloc_, new_node, 1);
      throw;
    }
    Node* previous = it.get_pointer()->prev;
    Node* next = it.get_pointer();
    new_node->next = next;
    new_node->prev = previous;
    next->prev = new_node;
    previous->next = new_node;
    ++size_;
  }

  void erase(const const_iterator& it) {
    Node* this_node = static_cast<Node*>(it.get_pointer());
    this_node->prev->next = this_node->next;
    this_node->next->prev = this_node->prev;
    AllocTraitsNode::destroy( NodeAlloc_, this_node);
    AllocTraitsNode::deallocate(NodeAlloc_, this_node, 1);
    --size_;
  }
};

template <typename T, class Allocator>
List<T, Allocator>::List() {
  tail = AllocTraitsBaseNode::allocate(BaseNodeAlloc_, 1);
  AllocTraitsBaseNode::construct(BaseNodeAlloc_, tail, BaseNode());
}

template <typename T, class Allocator>
List<T, Allocator>::List(size_t N, const T& value) {
  tail = AllocTraitsBaseNode::allocate(BaseNodeAlloc_, 1);
  AllocTraitsBaseNode::construct(BaseNodeAlloc_, tail, BaseNode());
  for (size_t i = 0; i < N; ++i) {
    try {
      push_back(value);
    }
    catch (...) {
      for (size_t j = 0; j < i; ++j) pop_back();
      AllocTraitsBaseNode::destroy(BaseNodeAlloc_, tail);
      AllocTraitsBaseNode::deallocate(BaseNodeAlloc_, tail);
      throw;
    }
  }
}

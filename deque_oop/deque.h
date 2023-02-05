#include <type_traits>

template <typename T>
class Deque {
 private:
  T** buckets_ = nullptr;
  size_t bucket_begin_ = 0;
  size_t size_of_deque_ = 0;
  size_t bucket_end_ = 0;

  void clean() {
    for (size_t i = bucket_begin_; i < bucket_end_; ++i) {
      (buckets_[i / size_of_bucket] + i % size_of_bucket)->~T();
    }
    for (size_t i = 0; i < size_of_deque_ / size_of_bucket; ++i) {
      delete[] reinterpret_cast<uint8_t*>(buckets_[i]);
    }
    delete[] reinterpret_cast<uint8_t**>(buckets_);
  }

 public:
  using value_type = T;
  size_t more = 3;
  size_t size_of_bucket = 8;

  Deque() {
    buckets_ = new T* [more];
    for (size_t i = 0; i < more; ++i) {
      try { buckets_[i] = reinterpret_cast<T*>(new uint8_t[size_of_bucket * sizeof(T)]); }
      catch (...) {
        for (size_t j = 0; j < i; ++j) {
          delete[] reinterpret_cast<uint8_t*>(buckets_[j]);
        }
        delete[] reinterpret_cast<uint8_t**>(buckets_);
      }
    }
    size_of_deque_ = more * size_of_bucket;
    bucket_begin_ = size_of_bucket;
    bucket_end_ = bucket_begin_;
  }

  Deque(const Deque& deque) {
    buckets_ = new T* [deque.size_of_deque_ / deque.size_of_bucket];
    bucket_begin_ = deque.bucket_begin_;
    bucket_end_ = deque.bucket_end_;
    size_of_deque_ = deque.size_of_deque_;
    for (size_t i = 0; i < size_of_deque_ / size_of_bucket; ++i) {
      try { buckets_[i] = reinterpret_cast<T*>(new uint8_t[size_of_bucket * sizeof(T)]); }
      catch (...) {
        for (size_t j = 0; j < i; ++j) {
          delete[] reinterpret_cast<uint8_t*>(buckets_[j]);
        }
        delete[] reinterpret_cast<uint8_t**>(buckets_);
        throw;
      }
    }
    for (size_t i = bucket_begin_; i < bucket_end_; ++i) {
      try {
        new(buckets_[i / size_of_bucket] + i % size_of_bucket)
          T(*(deque.buckets_[i / size_of_bucket] + i % size_of_bucket));
      } catch (...) {
        for (size_t j = bucket_begin_; j < i; ++j) {
          (buckets_[j / size_of_bucket] + j % size_of_bucket)->~T();
        }
        for (size_t j = 0; j < size_of_deque_ / size_of_bucket; ++j) {
          delete[] reinterpret_cast<uint8_t*>(buckets_[j]);
        }
        delete[] reinterpret_cast<uint8_t**>(buckets_);
        throw;
      }
    }
  }

  Deque(size_t num) {
    size_t num_of_buckets = ((num + size_of_bucket) / size_of_bucket) * more;
    buckets_ = new T* [num_of_buckets];
    for (size_t i = 0; i < num_of_buckets; ++i) {
      try { buckets_[i] = reinterpret_cast<T*>(new uint8_t[size_of_bucket * sizeof(T)]); }
      catch (...) {
        for (size_t j = 0; j < i; ++j) {
          delete[] reinterpret_cast<uint8_t*>(buckets_[j]);
        }
        delete[] reinterpret_cast<uint8_t**>(buckets_);
        throw;
      }
    }
    size_of_deque_ = num_of_buckets * size_of_bucket;
    bucket_begin_ = (num_of_buckets / more) * size_of_bucket;
    bucket_end_ = bucket_begin_ + num;
    for (size_t i = 0; i < num; ++i) {
      try { new(buckets_[(bucket_begin_ + i) / size_of_bucket] + i % size_of_bucket) T(); }
      catch (...) {
        if (i != 0) {
          for (size_t j = 1; j < i; ++j) {
            (buckets_[(bucket_begin_ + j) / size_of_bucket] + j % size_of_bucket)->~T();
          }
          buckets_[bucket_begin_ / size_of_bucket]->~T();
        }
        for (size_t j = 0; j < size_of_deque_ / size_of_bucket; ++j) {
          delete[] reinterpret_cast<uint8_t*>(buckets_[j]);
        }
        delete[] reinterpret_cast<uint8_t**>(buckets_);
        throw;
      }
    }
  }

  Deque(size_t num, const T& el) {
    size_t num_of_buckets = ((num + size_of_bucket) / size_of_bucket) * more;
    buckets_ = new T* [num_of_buckets];
    size_of_deque_ = num_of_buckets * size_of_bucket;
    bucket_begin_ = (num_of_buckets / more) * size_of_bucket;
    bucket_end_ = bucket_begin_ + num;
    for (size_t i = 0; i < num_of_buckets; ++i) {
      try { buckets_[i] = reinterpret_cast<T*>(new uint8_t[size_of_bucket * sizeof(T)]); }
      catch (...) {
        for (size_t j = 0; j < i; ++j) {
          delete[] reinterpret_cast<uint8_t*>(buckets_[j]);
        }
        delete[] reinterpret_cast<uint8_t**>(buckets_);
        throw;
      }
    }
    for (size_t i = 0; i < num; ++i) {
      try { new(buckets_[(bucket_begin_ + i) / size_of_bucket] + i % size_of_bucket) T(el); }
      catch (...) {
        for (size_t j = 0; j < i; ++j) {
          (buckets_[(bucket_begin_ + j) / size_of_bucket] + j % size_of_bucket)->~T();
        }
        for (size_t j = 0; j < num_of_buckets; ++j) {
          delete[] reinterpret_cast<uint8_t*>(buckets_[j]);
        }
        delete[] reinterpret_cast<uint8_t**>(buckets_);
        throw;
      }
    }
  }

  Deque& operator=(const Deque& deque) {
    Deque copy_deque(deque);
    std::swap(buckets_, copy_deque.buckets_);
    std::swap(bucket_begin_, copy_deque.bucket_begin_);
    std::swap(bucket_end_, copy_deque.bucket_end_);
    std::swap(size_of_deque_, copy_deque.size_of_deque_);
    return *this;
  }

  ~Deque() {
    clean();
  }

  size_t size() const { return bucket_end_ - bucket_begin_; }

  T& operator[](size_t index) {
    return *(buckets_[(bucket_begin_ + index) / size_of_bucket] + ((bucket_begin_ + index) % size_of_bucket));
  }

  const T& operator[](size_t index) const {
    const T& val = *(buckets_[(bucket_begin_ + index) / size_of_bucket] + ((bucket_begin_ + index) % size_of_bucket));
    return val;
  }

  T& at(size_t index) {
    if (bucket_begin_ + index >= bucket_end_ || index >= bucket_end_) {
      throw std::out_of_range("You're out of range");
    } else {
      return *(buckets_[(bucket_begin_ + index) / size_of_bucket] + (bucket_begin_ + index) % size_of_bucket);
    }
  }

  const T& at(size_t index) const {
    if (bucket_begin_ + index >= bucket_end_) {
      throw std::out_of_range("You're out of range");
    } else {
      return *(buckets_[(bucket_begin_ + index) / size_of_bucket] + (bucket_begin_ + index) % size_of_bucket);
    }
  }

  void push_back(const T& el) {
    if (bucket_end_ >= size_of_deque_) { reserve(); }
    new(buckets_[bucket_end_ / size_of_bucket] + bucket_end_ % size_of_bucket) T(el);
    ++bucket_end_;
  }

  void push_front(const T& el) {
    if (bucket_begin_ == 0) { reserve(); }
    --bucket_begin_;
    try {
      new(buckets_[bucket_begin_ / size_of_bucket] + bucket_begin_ % size_of_bucket) T(el);
    } catch (...) {
      ++bucket_begin_;
      throw;
    }
  }

  void pop_front() {
    (buckets_[bucket_begin_ / size_of_bucket] + bucket_begin_ % size_of_bucket)->~T();
    ++bucket_begin_;
  }

  void pop_back() {
    --bucket_end_;
    (buckets_[bucket_end_ / size_of_bucket] + bucket_end_ % size_of_bucket)->~T();
  }

  void reserve() {
    size_t num_of_buckets = (bucket_end_ + size_of_bucket - 1) / size_of_bucket - bucket_begin_ / size_of_bucket;
    T** new_buckets_ = new T* [num_of_buckets * more];
    for (size_t i = 0; i < num_of_buckets; ++i) {
      try { new_buckets_[i] = reinterpret_cast<T*>(new uint8_t[size_of_bucket * sizeof(T)]); }
      catch (...) {
        for (size_t j = 0; j < i; ++j) {
          delete[] reinterpret_cast<uint8_t*>(new_buckets_[j]);
        }
        delete[] reinterpret_cast<uint8_t**>(new_buckets_);
        throw;
      }
    }
    size_t first_ind = bucket_begin_ / size_of_bucket;
    for (size_t i = num_of_buckets; i < 2 * num_of_buckets; ++i) {
      new_buckets_[i] = buckets_[i - num_of_buckets + first_ind];
    }
    for (size_t i = 2 * num_of_buckets; i < more * num_of_buckets; ++i) {
      try { new_buckets_[i] = reinterpret_cast<T*>(new uint8_t[size_of_bucket * sizeof(T)]); }
      catch (...) {
        for (size_t k = 0; k < num_of_buckets; ++k) {
          delete[] reinterpret_cast<uint8_t*>(new_buckets_[k]);
        }
        for (size_t j = 2 * num_of_buckets; j < i; ++j) {
          delete[] reinterpret_cast<uint8_t*>(new_buckets_[j]);
        }
        delete[] reinterpret_cast<uint8_t**>(new_buckets_);
        throw;
      }
    }
    for (size_t i = 0; i < first_ind; ++i) {
      delete[] reinterpret_cast<uint8_t*>(buckets_[i]);
    }
    for (size_t i = first_ind + num_of_buckets; i < size_of_deque_ / size_of_bucket; ++i) {
      delete[] reinterpret_cast<uint8_t*>(buckets_[i]);
    }
    delete[] reinterpret_cast<uint8_t**>(buckets_);
    buckets_ = new_buckets_;
    bucket_begin_ = num_of_buckets * size_of_bucket + (bucket_begin_ - size_of_bucket * first_ind);
    bucket_end_ = num_of_buckets * size_of_bucket + (bucket_end_ - size_of_bucket * first_ind);
    size_of_deque_ = num_of_buckets * more * size_of_bucket;
  }


  template <bool IsConst>
  class common_iterator : public std::iterator<std::random_access_iterator_tag, std::conditional_t<IsConst, const T, T>> {
   private:
    T** it_bucket_;
    size_t ind_ = 0;
   public:
    size_t size_of_buckets = 8;

    using value_type = std::conditional_t<IsConst, const T, T>;
    using pointer = std::conditional_t<IsConst, const T*, T*>;
    using reference = std::conditional_t<IsConst, const T&, T&>;
    using difference_type = int;

    common_iterator(std::conditional_t<IsConst, T** const, T**> first_data, size_t index) {
      it_bucket_ = first_data;
      ind_ = index;
    }

    common_iterator(const common_iterator<false>& it) {
      it_bucket_ = it.it_bucket_;
      ind_ = it.ind_;
    }

    std::conditional_t<IsConst, const T&, T&>& operator*() {
      return it_bucket_[ind_ / size_of_buckets][ind_ % size_of_buckets];
    }

    std::conditional_t<IsConst, T* const, T*> operator->() {
      return &it_bucket_[ind_ / size_of_buckets][ind_ % size_of_buckets];
    }

    common_iterator<IsConst>& operator++() {
      ++ind_;
      return *this;
    }

    common_iterator<IsConst> operator++(int) {
      common_iterator<IsConst> copy_it = *this;
      ++*this;
      return copy_it;
    }

    common_iterator<IsConst>& operator--() {
      --ind_;
      return *this;
    }

    common_iterator<IsConst> operator--(int) {
      common_iterator<IsConst> copy_it = *this;
      --*this;
      return copy_it;
    }

    common_iterator<IsConst>& operator+=(int n) {
      if (n > 0) { ind_ += n; }
      else if (n < 0) { (*this) -= std::abs(n); }
      return *this;
    }

    common_iterator<IsConst>& operator-=(int n) {
      if (n > 0) { ind_ -= n; }
      else if (n < 0) { (*this) += std::abs(n); }
      return *this;
    }

    common_iterator<IsConst> operator+(int n) const {
      common_iterator<IsConst> it = *this;
      it += n;
      return it;
    }

    common_iterator<IsConst> operator-(int n) const {
      common_iterator<IsConst> it = *this;
      it -= n;
      return it;
    }

    template <bool IsConst1>
    difference_type operator-(const common_iterator<IsConst1>& other_it) {
      int ans = ind_ - other_it.ind_;
      return ans;
    }

    bool operator==(const common_iterator<IsConst>& other_it) const {
      return ind_ == other_it.ind_;
    }

    bool operator!=(const common_iterator<IsConst>& other_it) const {
      return !(*this == other_it);
    }

    bool operator>(const common_iterator<IsConst>& other_it) const {
      return ind_ > other_it.ind_;
    }

    bool operator<(const common_iterator<IsConst>& other_it) const {
      return !(*this == other_it || *this > other_it);
    }

    bool operator>=(const common_iterator<IsConst>& other_it) const {
      return *this > other_it || *this == other_it;
    }

    bool operator<=(const common_iterator<IsConst>& other_it) const {
      return *this < other_it || *this == other_it;
    }

    size_t get_index() const { return ind_; }

  };

  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin() {
    return iterator(buckets_, bucket_begin_);
  }

  iterator end() {
    return iterator(buckets_, bucket_end_);
  }

  const_iterator begin() const {
    return const_iterator(buckets_, bucket_begin_);
  }

  const_iterator end() const {
    return const_iterator(buckets_, bucket_end_);
  }

  const_iterator cbegin() const {
    return const_iterator(buckets_, bucket_begin_);
  }

  const_iterator cend() const {
    return const_iterator(buckets_, bucket_end_);
  }

  reverse_iterator rbegin() {
    return reverse_iterator(end());
  }

  reverse_iterator rend() {
    return reverse_iterator(begin());
  }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(cend());
  }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(cbegin());
  }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(cend());
  }

  const_reverse_iterator crend() const {
    return const_reverse_iterator(cbegin());
  }

  void insert(iterator it, const T& el) {
    Deque<T> copy_deque(*this);
    size_t need_index = it.get_index() - bucket_begin_;
    copy_deque.push_back(el);
    need_index += copy_deque.bucket_begin_;
    size_t index = copy_deque.bucket_end_;
    while (index != need_index) {
      --index;
      copy_deque.buckets_[index / size_of_bucket][index % size_of_bucket] =
        copy_deque.buckets_[(index - 1) / size_of_bucket][(index - 1) % size_of_bucket];
    }
    copy_deque.buckets_[need_index / size_of_bucket][need_index % size_of_bucket] = el;
    std::swap(buckets_, copy_deque.buckets_);
    std::swap(bucket_begin_, copy_deque.bucket_begin_);
    std::swap(bucket_end_, copy_deque.bucket_end_);
    std::swap(size_of_deque_, copy_deque.size_of_deque_);
  }

  void erase(iterator it) {
    Deque<T> copy_deque(*this);
    size_t index = it.get_index() + 1;
    while (index < copy_deque.bucket_end_) {
      copy_deque.buckets_[(index - 1) / size_of_bucket][(index - 1) % size_of_bucket] =
        copy_deque.buckets_[index / size_of_bucket][index % size_of_bucket];
      ++index;
    }
    --index;
    copy_deque.bucket_end_ = index;
    (copy_deque.buckets_[index / size_of_bucket] + index % size_of_bucket)->~T();
    std::swap(buckets_, copy_deque.buckets_);
    std::swap(bucket_begin_, copy_deque.bucket_begin_);
    std::swap(bucket_end_, copy_deque.bucket_end_);
    std::swap(size_of_deque_, copy_deque.size_of_deque_);
  }
};
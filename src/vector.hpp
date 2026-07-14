#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <new>
#include <cstdlib>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
private:
	T* data_;
	size_t size_;
	size_t capacity_;
	
	void reallocate(size_t new_capacity) {
		T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
		for (size_t i = 0; i < size_; ++i) {
			new(new_data + i) T(data_[i]);
		}
		for (size_t i = 0; i < size_; ++i) {
			data_[i].~T();
		}
		::operator delete(data_);
		data_ = new_data;
		capacity_ = new_capacity;
	}
	
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
	friend class vector;
	private:
		T* ptr_;
		vector* parent_;
		
	public:
		iterator(T* ptr = nullptr, vector* parent = nullptr) : ptr_(ptr), parent_(parent) {}
		iterator(const iterator& other) : ptr_(other.ptr_), parent_(other.parent_) {}
		
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			return iterator(ptr_ + n, parent_);
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr_ - n, parent_);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (parent_ != rhs.parent_) {
				throw invalid_iterator();
			}
			return static_cast<int>(ptr_ - rhs.ptr_);
		}
		iterator& operator+=(const int &n)
		{
			ptr_ += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			ptr_ -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) 
		{
			iterator temp = *this;
			++ptr_;
			return temp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() 
		{
			++ptr_;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) 
		{
			iterator temp = *this;
			--ptr_;
			return temp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() 
		{
			--ptr_;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			return *ptr_;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const 
		{
			return ptr_ == rhs.ptr_;
		}
		bool operator==(const const_iterator &rhs) const 
		{
			return ptr_ == rhs.ptr_;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const 
		{
			return ptr_ != rhs.ptr_;
		}
		bool operator!=(const const_iterator &rhs) const 
		{
			return ptr_ != rhs.ptr_;
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	friend class vector;
	private:
		const T* ptr_;
		const vector* parent_;
		
	public:
		const_iterator(const T* ptr = nullptr, const vector* parent = nullptr) : ptr_(ptr), parent_(parent) {}
		const_iterator(const const_iterator& other) : ptr_(other.ptr_), parent_(other.parent_) {}
		const_iterator(const iterator& other) : ptr_(other.ptr_), parent_(other.parent_) {}
		
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		const_iterator operator+(const int &n) const
		{
			return const_iterator(ptr_ + n, parent_);
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(ptr_ - n, parent_);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const
		{
			if (parent_ != rhs.parent_) {
				throw invalid_iterator();
			}
			return static_cast<int>(ptr_ - rhs.ptr_);
		}
		const_iterator& operator+=(const int &n)
		{
			ptr_ += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			ptr_ -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) 
		{
			const_iterator temp = *this;
			++ptr_;
			return temp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() 
		{
			++ptr_;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) 
		{
			const_iterator temp = *this;
			--ptr_;
			return temp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() 
		{
			--ptr_;
			return *this;
		}
		/**
		 * TODO *it
		 */
		const T& operator*() const
		{
			return *ptr_;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const const_iterator &rhs) const 
		{
			return ptr_ == rhs.ptr_;
		}
		bool operator==(const iterator &rhs) const 
		{
			return ptr_ == rhs.ptr_;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const const_iterator &rhs) const 
		{
			return ptr_ != rhs.ptr_;
		}
		bool operator!=(const iterator &rhs) const 
		{
			return ptr_ != rhs.ptr_;
		}
	};
	
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() : data_(nullptr), size_(0), capacity_(0) {}
	vector(const vector &other) : data_(nullptr), size_(0), capacity_(0) {
		if (other.capacity_ > 0) {
			data_ = static_cast<T*>(::operator new(other.capacity_ * sizeof(T)));
			capacity_ = other.capacity_;
			size_ = other.size_;
			for (size_t i = 0; i < size_; ++i) {
				new(data_ + i) T(other.data_[i]);
			}
		}
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		clear();
		::operator delete(data_);
		data_ = nullptr;
		capacity_ = 0;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (this == &other) return *this;
		
		for (size_t i = 0; i < size_; ++i) {
			data_[i].~T();
		}
		::operator delete(data_);
		
		if (other.capacity_ > 0) {
			data_ = static_cast<T*>(::operator new(other.capacity_ * sizeof(T)));
			capacity_ = other.capacity_;
			size_ = other.size_;
			for (size_t i = 0; i < size_; ++i) {
				new(data_ + i) T(other.data_[i]);
			}
		} else {
			data_ = nullptr;
			size_ = 0;
			capacity_ = 0;
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos >= size_) {
			throw index_out_of_bound();
		}
		return data_[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos >= size_) {
			throw index_out_of_bound();
		}
		return data_[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		return at(pos);
	}
	const T & operator[](const size_t &pos) const {
		return at(pos);
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (empty()) {
			throw container_is_empty();
		}
		return data_[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (empty()) {
			throw container_is_empty();
		}
		return data_[size_ - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(data_, this);
	}
	const_iterator begin() const {
		return const_iterator(data_, this);
	}
	const_iterator cbegin() const {
		return const_iterator(data_, this);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(data_ + size_, this);
	}
	const_iterator end() const {
		return const_iterator(data_ + size_, this);
	}
	const_iterator cend() const {
		return const_iterator(data_ + size_, this);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return size_ == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return size_;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (size_t i = 0; i < size_; ++i) {
			data_[i].~T();
		}
		size_ = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		int index = pos - begin();
		if (index < 0 || index > static_cast<int>(size_)) {
			throw invalid_iterator();
		}
		
		if (size_ == capacity_) {
			reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		
		// Move elements from index to end one position to the right
		for (size_t i = size_; i > static_cast<size_t>(index); --i) {
			new(data_ + i) T(data_[i - 1]);
			data_[i - 1].~T();
		}
		
		// Insert new element
		new(data_ + index) T(value);
		++size_;
		
		return iterator(data_ + index, this);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > size_) {
			throw index_out_of_bound();
		}
		return insert(begin() + ind, value);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		int index = pos - begin();
		if (index < 0 || index >= static_cast<int>(size_)) {
			throw invalid_iterator();
		}
		
		// Destroy the element at pos
		data_[index].~T();
		
		// Move elements from index+1 to end one position to the left
		for (size_t i = index; i < size_ - 1; ++i) {
			new(data_ + i) T(data_[i + 1]);
			data_[i + 1].~T();
		}
		
		--size_;
		if (index < static_cast<int>(size_)) {
			return iterator(data_ + index, this);
		} else {
			return end();
		}
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind >= size_) {
			throw index_out_of_bound();
		}
		return erase(begin() + ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		insert(end(), value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (empty()) {
			throw container_is_empty();
		}
		erase(end() - 1);
	}
};


}

#endif

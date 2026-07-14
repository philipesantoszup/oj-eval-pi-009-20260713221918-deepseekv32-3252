#include "src/vector.hpp"
#include <iostream>
#include <cassert>

void test_exceptions() {
    sjtu::vector<int> v;
    
    // Test empty container exceptions
    try {
        v.front();
        std::cout << "ERROR: Should have thrown container_is_empty!" << std::endl;
    } catch (sjtu::container_is_empty&) {
        std::cout << "PASS: front() throws container_is_empty on empty vector" << std::endl;
    }
    
    try {
        v.back();
        std::cout << "ERROR: Should have thrown container_is_empty!" << std::endl;
    } catch (sjtu::container_is_empty&) {
        std::cout << "PASS: back() throws container_is_empty on empty vector" << std::endl;
    }
    
    try {
        v.pop_back();
        std::cout << "ERROR: Should have thrown container_is_empty!" << std::endl;
    } catch (sjtu::container_is_empty&) {
        std::cout << "PASS: pop_back() throws container_is_empty on empty vector" << std::endl;
    }
    
    // Add some elements
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    // Test index out of bounds
    try {
        v.at(10);
        std::cout << "ERROR: Should have thrown index_out_of_bound!" << std::endl;
    } catch (sjtu::index_out_of_bound&) {
        std::cout << "PASS: at() throws index_out_of_bound for out of bounds" << std::endl;
    }
    
    try {
        v.erase(10);
        std::cout << "ERROR: Should have thrown index_out_of_bound!" << std::endl;
    } catch (sjtu::index_out_of_bound&) {
        std::cout << "PASS: erase() throws index_out_of_bound for out of bounds" << std::endl;
    }
    
    try {
        v.insert(10, 42);
        std::cout << "ERROR: Should have thrown index_out_of_bound!" << std::endl;
    } catch (sjtu::index_out_of_bound&) {
        std::cout << "PASS: insert() throws index_out_of_bound for index > size" << std::endl;
    }
}

void test_iterator_invalidation() {
    sjtu::vector<int> v;
    for (int i = 0; i < 10; i++) {
        v.push_back(i);
    }
    
    // Test iterator distance between different containers
    sjtu::vector<int> v2;
    v2.push_back(1);
    
    try {
        auto d = v.begin() - v2.begin();
        std::cout << "ERROR: Should have thrown invalid_iterator!" << std::endl;
    } catch (sjtu::invalid_iterator&) {
        std::cout << "PASS: iterator distance throws invalid_iterator for different containers" << std::endl;
    }
    
    // Test valid iterator operations
    auto it1 = v.begin();
    auto it2 = v.begin() + 5;
    std::cout << "Distance between iterators: " << (it2 - it1) << " (expected 5)" << std::endl;
}

void test_copy_and_assignment() {
    std::cout << "\nTesting copy and assignment..." << std::endl;
    
    sjtu::vector<int> v1;
    for (int i = 0; i < 100; i++) {
        v1.push_back(i * i);
    }
    
    // Copy constructor
    sjtu::vector<int> v2(v1);
    assert(v1.size() == v2.size());
    for (size_t i = 0; i < v1.size(); i++) {
        assert(v1[i] == v2[i]);
    }
    std::cout << "PASS: Copy constructor works correctly" << std::endl;
    
    // Assignment operator
    sjtu::vector<int> v3;
    v3 = v1;
    assert(v1.size() == v3.size());
    for (size_t i = 0; i < v1.size(); i++) {
        assert(v1[i] == v3[i]);
    }
    std::cout << "PASS: Assignment operator works correctly" << std::endl;
    
    // Self-assignment
    v1 = v1;
    assert(v1.size() == 100);
    std::cout << "PASS: Self-assignment works correctly" << std::endl;
}

void test_insert_erase() {
    std::cout << "\nTesting insert and erase..." << std::endl;
    
    sjtu::vector<int> v;
    for (int i = 0; i < 5; i++) {
        v.push_back(i);
    }
    
    // Insert at beginning
    v.insert(v.begin(), -1);
    assert(v[0] == -1);
    assert(v.size() == 6);
    
    // Insert at end (size is allowed)
    v.insert(v.size(), 100);
    assert(v[6] == 100);
    assert(v.size() == 7);
    
    // Insert in middle using iterator
    v.insert(v.begin() + 3, 999);
    assert(v[3] == 999);
    
    // Erase from beginning
    v.erase(v.begin());
    assert(v[0] == 0);
    
    // Erase from end
    v.erase(v.size() - 1);
    assert(v.back() == 4);
    
    std::cout << "PASS: Insert and erase operations work correctly" << std::endl;
}

void test_clear_and_empty() {
    std::cout << "\nTesting clear and empty..." << std::endl;
    
    sjtu::vector<int> v;
    assert(v.empty());
    
    for (int i = 0; i < 1000; i++) {
        v.push_back(i);
    }
    assert(!v.empty());
    assert(v.size() == 1000);
    
    v.clear();
    assert(v.empty());
    assert(v.size() == 0);
    
    // Should be able to reuse after clear
    v.push_back(42);
    assert(v.size() == 1);
    assert(v[0] == 42);
    
    std::cout << "PASS: Clear and empty work correctly" << std::endl;
}

int main() {
    test_exceptions();
    test_iterator_invalidation();
    test_copy_and_assignment();
    test_insert_erase();
    test_clear_and_empty();
    
    std::cout << "\nAll edge case tests passed!" << std::endl;
    return 0;
}
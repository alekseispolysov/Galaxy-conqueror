#pragma once
#include <iostream>
#include <vector>
#include <cassert>

template <typename T>
class DynamicSparseSet
{
public:
	DynamicSparseSet() = default;
	
	void insert(int id, T& value);
    //void insert(int id, const T value);
	void erase(int id);
	bool contains(int id);
	T& get(int id);
	const std::vector<T>& getElements();
	void print();
    int size();
    void clear();




private:
	std::vector<int> sparse;  // Sparse array: maps IDs to positions in dense
	std::vector<T> dense;      // Dense array: stores actual values
    std::vector<int> ids;      // Tracks IDs for values in the dense array
	void ensureCapacity(int id);
};


// - STORING pointers/references
template<typename T>
void DynamicSparseSet<T>::insert(int id, T& value)
{
    ensureCapacity(id); // Ensure sparse array can accommodate the ID
    if (contains(id)) return; // Ignore duplicates
    sparse[id] = dense.size(); // Map sparse[id] to position in dense array
    dense.push_back(value);       // Add the ID to the dense array
    ids.push_back(id);
}

// this part of object is type function overload, so I can store objects, or pointers to an objects in DynamicSparseSet
// storing objects
//template<typename T>
//inline void DynamicSparseSet<T>::insert(int id, const T value)
//{
//    ensureCapacity(id); // Ensure sparse array can accommodate the ID
//    if (contains(id)) return; // Ignore duplicates
//    sparse[id] = dense.size(); // Map sparse[id] to position in dense array
//    dense.push_back(value);       // Add the ID to the dense array
//    ids.push_back(id);
//}

template<typename T>
void DynamicSparseSet<T>::erase(int id)
{
    if (!contains(id)) return; // Ignore if ID is not in the set
    int denseIndex = sparse[id];          // Position of the element in the dense array
    int lastID = dense.back();            // Get the last element in dense array
    dense[denseIndex] = dense.back();     // Move the last value to the deleted position
    sparse[lastID] = denseIndex;          // Update the sparse array for the moved element
    sparse[id] = -1;                      // Mark the deleted ID as removed
    dense.pop_back();                     // Remove the last value
    ids.pop_back();                       // Remove the last ID
}

template<typename T>
bool DynamicSparseSet<T>::contains(int id)
{
    return id < sparse.size() && sparse[id] != -1;
}

template<typename T>
T& DynamicSparseSet<T>::get(int id)
{
    assert(contains(id) && "ID not in the set");
    return dense[sparse[id]];
}

template<typename T>
const std::vector<T>& DynamicSparseSet<T>::getElements()
{
    return dense;
}

template<typename T>
void DynamicSparseSet<T>::print()
{
    std::cout << "Dense: ";
    for (T id : dense) {
        std::cout << id << " ";
    }
    std::cout << std::endl;
}

template<typename T>
void DynamicSparseSet<T>::ensureCapacity(int id)
{
    if (id >= sparse.size()) {
        sparse.resize(id + 1, -1); // Resize sparse array and initialize new elements to -1
    }
}

template<typename T>
int DynamicSparseSet<T>::size() {
    return dense.size();
}

template<typename T>
void DynamicSparseSet<T>::clear() {
    sparse.clear();
    dense.clear();
    ids.clear();
}
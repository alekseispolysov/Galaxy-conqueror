#include "DynamicSparseSet.h"
#include <iostream>
#include <vector>
#include <cassert>

//void insert(int id, const T& value);

//template<typename T>
//void DynamicSparseSet<T>::insert(int id, const T& value)
//{
//	ensureCapacity(id); // Ensure sparse array can accommodate the ID
//
//	if (contains(id)) return; // Ignore duplicates
//
//	sparse[id] = dense.size(); // Map sparse[id] to position in dense array
//	dense.push_back(value);       // Add the ID to the dense array
//	ids.push_back(id);
//}
//
//
//template<typename T>
//void DynamicSparseSet<T>::erase(int id)
//{
//	if (!contains(id)) return; // Ignore if ID is not in the set
//
//	int denseIndex = sparse[id];          // Position of the element in the dense array
//	int lastID = dense.back();            // Get the last element in dense array
//
//	// Move the last element to the deleted position
//	dense[denseIndex] = dense.back();
//	sparse[lastID] = denseIndex;          // Update the sparse array for the moved element
//
//	// Mark the deleted ID as removed in sparse array
//	sparse[id] = -1;
//
//	dense.pop_back();                      // Remove the last value
//	ids.pop_back();                        // Remove the last ID
//}
//
//template<typename T>
//bool DynamicSparseSet<T>::contains(int id) const
//{
//	return id < sparse.size() && sparse[id] != -1;
//}
//
//template<typename T>
//const T& DynamicSparseSet<T>::get(int id) const
//{
//	assert(contains(id) && "ID not in the set");
//    return dense[sparse[id]];
//}
//
//template<typename T>
//const std::vector<T>& DynamicSparseSet<T>::getElements() const
//{
//	return dense;
//}
//
//template<typename T>
//void DynamicSparseSet<T>::print() const
//{
//	std::cout << "Dense: ";
//	for (int id : dense) {
//		std::cout << id << " ";
//	}
//	std::cout << std::endl;
//}
//
//template<typename T>
//void DynamicSparseSet<T>::ensureCapacity(int id)
//{
//	if (id >= sparse.size()) {
//		sparse.resize(id + 1, -1); // Resize sparse array and initialize new elements to -1
//	}
//}
//
//


//template <typename T>
//class DynamicSparseSet {
//public:
//    DynamicSparseSet() = default;
//
//    // Insert an element with a given ID and value
//    void insert(int id, const T& value) {
//        ensureCapacity(id); // Ensure sparse array can accommodate the ID
//
//        if (contains(id)) return; // Ignore duplicates
//
//        sparse[id] = dense.size();    // Map sparse[id] to the position in dense
//        dense.push_back(value);       // Add value to the dense array
//        ids.push_back(id);            // Track the ID associated with this value
//    }
//
//    // Remove an element by its ID
//    void erase(int id) {
//        if (!contains(id)) return; // Ignore if the ID is not in the set
//
//        int denseIndex = sparse[id];           // Position of the element in dense
//        int lastID = ids.back();               // ID of the last element in dense
//
//        dense[denseIndex] = dense.back();      // Move the last value to the deleted position
//        ids[denseIndex] = lastID;              // Update ID tracking
//
//        sparse[lastID] = denseIndex;           // Update sparse array for moved ID
//        sparse[id] = -1;                       // Mark the deleted ID
//
//        dense.pop_back();                      // Remove the last value
//        ids.pop_back();                        // Remove the last ID
//    }
//
//    // Check if an element with the given ID exists
//    bool contains(int id) const { // const means that nothing is modified in this function, I cannot modify class variables
//        return id < sparse.size() && sparse[id] != -1;
//    }
//
//    // Access an element by its ID
//    T& get(int id) {
//        assert(contains(id) && "ID not in the set");
//        return dense[sparse[id]];
//    }
//
//    const T& get(int id) const {
//        assert(contains(id) && "ID not in the set");
//        return dense[sparse[id]];
//    }
//
//    // Print the elements
//    void print() const {
//        std::cout << "Elements:\n";
//        for (size_t i = 0; i < dense.size(); ++i) {
//            std::cout << "ID " << ids[i] << ": " << dense[i] << "\n";
//        }
//    }
//
//private:
//    std::vector<int> sparse;   // Sparse array: maps IDs to positions in dense
//    std::vector<T> dense;      // Dense array: stores actual values
//    std::vector<int> ids;      // Tracks IDs for values in the dense array
//
//    // Ensure sparse array can accommodate the given ID
//    void ensureCapacity(int id) {
//        if (id >= sparse.size()) {
//            sparse.resize(id + 1, -1); // Resize and initialize new entries to -1
//        }
//    }
//};
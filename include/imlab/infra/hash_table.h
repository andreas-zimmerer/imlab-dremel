// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_INFRA_HASH_TABLE_H_
#define INCLUDE_IMLAB_INFRA_HASH_TABLE_H_
//---------------------------------------------------------------------------
#include <algorithm>
#include <utility>
#include <vector>
#include <iterator>
#include <limits>
#include "imlab/infra/hash.h"
#include "imlab/infra/bits.h"
//---------------------------------------------------------------------------
template<typename ... T> struct IsKey : std::false_type { };
template<typename ... T> struct IsKey<Key<T...>> : std::true_type { };
//---------------------------------------------------------------------------
template<typename T> struct Tester;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
template <typename KeyT, typename ValueT>
class LazyMultiMap {
    // Check key type
    static_assert(IsKey<KeyT>::value, "The key of LazyMultiMap must be a Key<T>");

 protected:
    // Entry in the hash table
    struct Entry {
        // Pointer to the next element in the collision list
        Entry *next;
        // Key of the hash table entry
        KeyT key;
        // Value of the hash table entry
        ValueT value;

        // Constructor
        Entry(KeyT key, ValueT value)
            : next(nullptr), key(key), value(value) {}
    };

    // EqualRangeIterator for the hash table
    class EqualRangeIterator: public std::iterator<std::forward_iterator_tag, Entry> {
     public:
        // Default constructor
        EqualRangeIterator(): ptr_(nullptr), key_() {}
        // Constructor
        explicit EqualRangeIterator(Entry *ptr, KeyT key): ptr_(ptr), key_(key) {}
        // Destructor
        ~EqualRangeIterator() = default;

        // Postfix increment
        EqualRangeIterator operator++(int) {
            // TODO
        }
        // Prefix increment
        EqualRangeIterator &operator++() {
            // TODO
        }
        // Reference
        Entry &operator*() { /* TODO */ }
        // Pointer
        Entry *operator->() { /* TODO */ }
        // Equality
        bool operator==(const EqualRangeIterator &other) const { return ptr_ == other.ptr_; }
        // Inequality
        bool operator!=(const EqualRangeIterator &other) const { return ptr_ != other.ptr_; }

     protected:
        // Entry pointer
        Entry *ptr_;
        // Key that is being searched for
        KeyT key_;
    };

 public:
    // End of the equal range
    EqualRangeIterator end() { return EqualRangeIterator(); }

    // Insert an element into the hash table
    //  * Gather all entries with insert and build the hash table with finalize.
    void insert(const std::pair<KeyT, ValueT> &val) {
        // TODO
    }

    // Finalize the hash table
    //  * Get the next power of two, which is larger than the number of entries (include/imlab/infra/bits.h).
    //  * Resize the hash table to that size.
    //  * For each entry in entries_, calculate the hash and prepend it to the collision list in the hash table.
    void finalize() {
        // TODO
    }

    // To find an element, calculate the hash (Key::Hash), and search this list until you reach a nullptr;
    std::pair<EqualRangeIterator, EqualRangeIterator> equal_range(KeyT key) {
        // TODO
        return std::make_pair(EqualRangeIterator(), EqualRangeIterator());
    }

 protected:
    // Entries of the hash table.
    std::vector<Entry> entries_;
    // The hash table.
    // Use the next_ pointers in the entries to store the collision list of the hash table.
    //
    //      hash_table_     entries_
    //      +---+           +---+
    //      | * | --------> | x | --+
    //      | 0 |           |   |   |
    //      | 0 |           |   |   |
    //      | 0 |           | z | <-+
    //      +---+           +---+
    //
    std::vector<Entry*> hash_table_;
    // The hash table mask.
    uint32_t hash_table_mask_;
};
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_INFRA_HASH_TABLE_H_
//---------------------------------------------------------------------------

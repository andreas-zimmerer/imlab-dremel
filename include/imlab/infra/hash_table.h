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
            do {
                ptr_ = ptr_->next;
            } while (ptr_->key != key_);
            return *this;
        }
        // Prefix increment
        EqualRangeIterator &operator++() {
            do {
                ptr_ = ptr_->next;
            } while (ptr_->key != key_);
            return *this;
        }
        // Reference
        Entry &operator*() { return ptr_; }
        // Pointer
        Entry *operator->() { return ptr_; }
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
        entries_.emplace_back(val.first, val.second);
    }

    // Finalize the hash table
    //  * Get the next power of two, which is larger than the number of entries (include/imlab/infra/bits.h).
    //  * Resize the hash table to that size.
    //  * For each entry in entries_, calculate the hash and prepend it to the collision list in the hash table.
    void finalize() {
        auto hash_table_size = imlab::NextPow2_64(entries_.size());
        hash_table_.reserve(hash_table_size);

        hash_table_mask_ = 0;
        for (size_t i = 1; i < hash_table_size; i++)
            hash_table_mask_ = (hash_table_mask_ << 1) | 1;

        for (auto& entry : entries_) {
            auto hash = entry.key.Hash() & hash_table_mask_;

            auto next_ = hash_table_[hash];
            entry.next = next_;
            hash_table_[hash] = &entry;
        }
    }

    // To find an element, calculate the hash (Key::Hash), and search this list until you reach a nullptr;
    std::pair<EqualRangeIterator, EqualRangeIterator> equal_range(KeyT key) {
        auto hash = key.Hash() & hash_table_mask_;
        auto start_entry = hash_table_[hash];

        auto end_entry = start_entry;
        while (true) {
            auto tmp_next = end_entry;
            do {
                tmp_next = tmp_next->next;
            } while (tmp_next->key != key && tmp_next->next != nullptr);

            if (tmp_next->key == key) {
                end_entry = tmp_next;
            } else {
                break;
            }
        }

        return std::make_pair(EqualRangeIterator(start_entry, key), EqualRangeIterator(end_entry, key));
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

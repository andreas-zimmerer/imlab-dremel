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
#include <atomic>
#include "./hash.h"
#include "./bits.h"
#include "tbb/tbb.h"
#include "tbb/enumerable_thread_specific.h"
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
            } while (ptr_ != nullptr && ptr_->key != key_);
            return *this;
        }
        // Prefix increment
        EqualRangeIterator &operator++() {
            do {
                ptr_ = ptr_->next;
            } while (ptr_ != nullptr && ptr_->key != key_);
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
        // Every thread gets its own local entries vector.
        // Result: nice performance, because we don't have to lock.
        // In the end we of course need to process all of them.
        auto& entries_local = entries_.local();
        entries_local.emplace_back(val.first, val.second);
    }

    // Finalize the hash table
    //  * Get the next power of two, which is larger than the number of entries (include/imlab/infra/bits.h).
    //  * Resize the hash table to that size.
    //  * For each entry in entries_, calculate the hash and prepend it to the collision list in the hash table.
    void finalize() {
        size_t total_entry_count = 0;
        for (auto& local_entries : entries_) {
            total_entry_count += local_entries.size();
        }
        auto hash_table_size = imlab::NextPow2_64(total_entry_count);
        assert(hash_table_size > 0);
        hash_table_.resize(hash_table_size);

        hash_table_mask_ = hash_table_size - 1;

        for (auto& entries_local : entries_) {
            tbb::parallel_for(tbb::blocked_range<size_t>(0, entries_local.size()), [&](const tbb::blocked_range<size_t>& range) {
                for(size_t i = range.begin(); i != range.end(); ++i) {
                    auto entry = &entries_local[i];

                    auto hash = entry->key.Hash() & hash_table_mask_;
                    assert(hash < hash_table_.size());

                    auto hash_position = reinterpret_cast<std::atomic<Entry*>*>(&hash_table_[hash]);
                    entry->next = hash_position->load();
                    while(!std::atomic_compare_exchange_weak(
                            hash_position,
                            &entry->next,
                            entry));
                }
            });
        }
    }

    // To find an element, calculate the hash (Key::Hash), and search this list until you reach a nullptr;
    std::pair<EqualRangeIterator, EqualRangeIterator> equal_range(KeyT key) {
        auto hash = key.Hash() & hash_table_mask_;

        // Multiple keys might hash to the same bucket; so get the first element with the key we want
        auto start_entry = hash_table_[hash];
        while (start_entry != nullptr && start_entry->key != key) {
            start_entry = start_entry->next;
        }

        return std::make_pair(EqualRangeIterator(start_entry, key), EqualRangeIterator(nullptr, key));
    }

 protected:
    // Entries of the hash table.
    tbb::enumerable_thread_specific<std::vector<Entry>> entries_;
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

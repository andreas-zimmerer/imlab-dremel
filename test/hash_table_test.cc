// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "gtest/gtest.h"
#include "imlab/infra/hash_table.h"
#include "imlab/infra/types.h"

namespace {

TEST(LazyHashTableTest, InsertOneEntryAndFinalize) {
    LazyMultiMap<Key<Integer>, int> hash_map;
    hash_map.insert({Key(Integer(1)), 42});

    hash_map.finalize();
}

TEST(LazyHashTableTest, InsertOneEntryAndGet) {
    LazyMultiMap<Key<Integer>, int> hash_map;
    hash_map.insert({Key(Integer(1)), 42});

    hash_map.finalize();

    auto[it_begin, it_end] = hash_map.equal_range(Key(Integer(1)));
    ASSERT_EQ(it_begin->value, 42);
    ASSERT_EQ(++it_begin, it_end);
}

TEST(LazyHashTableTest, InsertTwoEntriesAndGet) {
    LazyMultiMap<Key<Integer>, int> hash_map;
    hash_map.insert({Key(Integer(1)), 42});
    hash_map.insert({Key(Integer(2)), 24});

    hash_map.finalize();

    auto[it_begin_1, it_end_1] = hash_map.equal_range(Key(Integer(1)));
    ASSERT_EQ(it_begin_1->value, 42);
    ASSERT_EQ(++it_begin_1, it_end_1);

    auto[it_begin_2, it_end_2] = hash_map.equal_range(Key(Integer(2)));
    ASSERT_EQ(it_begin_2->value, 24);
    ASSERT_EQ(++it_begin_2, it_end_2);
}

TEST(LazyHashTableTest, InsertSameKey) {
    LazyMultiMap<Key<Integer>, int> hash_map;
    std::set<int> values_to_insert {24, 42};
    for (auto v : values_to_insert) {
        hash_map.insert({Key(Integer(1)), v});
    }

    hash_map.finalize();

    std::set<int> values {};
    auto[it_begin, it_end] = hash_map.equal_range(Key(Integer(1)));
    for (auto it = it_begin; it != it_end; ++it) {
        values.insert(it->value);
    }
    ASSERT_EQ(values, values_to_insert);
}

TEST(LazyHashTableTest, EmptyGet) {
    LazyMultiMap<Key<Integer>, int> hash_map;
    hash_map.insert({Key(Integer(1)), 42});

    hash_map.finalize();

    auto[it_begin, it_end] = hash_map.equal_range(Key(Integer(3)));
    ASSERT_EQ(it_begin, it_end);
}

}  // namespace

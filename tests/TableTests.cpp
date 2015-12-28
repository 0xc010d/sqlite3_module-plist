#include <gtest/gtest.h>
#include "Table.hpp"

TEST(Table, EmptyJoinEmpty)
{
  Table<int> t1, t2;
  t1.join(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)0);
  ASSERT_EQ(t1.getFields().empty(), true);
}

TEST(Table, EmptyJoinNonEmpty)
{
  Table<int> t1, t2{"a", 1};
  t1.join(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)1);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)1);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
}

TEST(Table, NonEmptyJoinEmpty)
{
  Table<int> t1{"a", 1}, t2;
  t1.join(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)1);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)1);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
}

TEST(Table, NonEmptyJoinNonEmpty)
{
  Table<int> t1{"a", 1}, t2{"b", 2};
  t1.join(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)2);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)2);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "b"), fields.end());
}

TEST(Table, NonEmptyJoinNonEmptyShareField)
{
  Table<int> t1{"a", 1}, t2{"a", 2};
  t1.join(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)2);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)1);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
}

TEST(Table, ComplexNonEmptyJoinComplexNonEmpty)
{
  Table<int> t1
    {{
       {"a", {1, 4, 7}},
       {"b", {2, 5, 8}},
       {"c", {3, 6, 9}},
     }};
  Table<int> t2
    {{
       {"aa", {11, 14}},
       {"bb", {12, 15}},
       {"cc", {13, 16}},
     }};
  t1.join(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)5);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)6);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "b"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "c"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "aa"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "bb"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "cc"), fields.end());
  ASSERT_EQ(t1["a"], std::vector<int>({1, 4, 7, 0, 0}));
  ASSERT_EQ(t1["b"], std::vector<int>({2, 5, 8, 0, 0}));
  ASSERT_EQ(t1["c"], std::vector<int>({3, 6, 9, 0, 0}));
  ASSERT_EQ(t1["aa"], std::vector<int>({0, 0, 0, 11, 14}));
  ASSERT_EQ(t1["bb"], std::vector<int>({0, 0, 0, 12, 15}));
  ASSERT_EQ(t1["cc"], std::vector<int>({0, 0, 0, 13, 16}));
}

TEST(Table, ComplexNonEmptyJoinComplexNonEmptyShareField)
{
  Table<int> t1
    {{
       {"a", {1, 4, 7}},
       {"b", {2, 5, 8}},
       {"c", {3, 6, 9}},
     }};
  Table<int> t2
    {{
       {"a", {11, 14}},
       {"b", {12, 15}},
       {"aa", {13, 16}},
     }};
  t1.join(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)5);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)4);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "b"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "c"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "aa"), fields.end());
  ASSERT_EQ(t1["a"], std::vector<int>({1, 4, 7, 11, 14}));
  ASSERT_EQ(t1["b"], std::vector<int>({2, 5, 8, 12, 15}));
  ASSERT_EQ(t1["c"], std::vector<int>({3, 6, 9, 0, 0}));
  ASSERT_EQ(t1["aa"], std::vector<int>({0, 0, 0, 13, 16}));
}

TEST(Table, EmptyCombineEmpty)
{
  Table<int> t1, t2;
  t1.combine(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)0);
  ASSERT_EQ(t1.getFields().empty(), true);
}

TEST(Table, EmptyCombineNonEmpty)
{
  Table<int> t1, t2{"a", 1};
  t1.combine(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)1);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)1);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
}

TEST(Table, NonEmptyCombineEmpty)
{
  Table<int> t1{"a", 1}, t2;
  t1.combine(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)1);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)1);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
}

TEST(Table, NonEmptyCombineNonEmpty)
{
  Table<int> t1{"a", 1}, t2{"b", 2};
  t1.combine(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)1);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)2);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "b"), fields.end());
}

TEST(Table, NonEmptyCombineComplex)
{
  Table<int> t1{"a", 1};
  Table<int> t2
    {{
       {"b", {2, 3, 4}},
       {"c", {5, 6, 7}},
     }};
  t1.combine(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)3);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)3);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "b"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "c"), fields.end());
  ASSERT_EQ(t1["a"], std::vector<int>({1, 1, 1}));
  ASSERT_EQ(t1["b"], std::vector<int>({2, 3, 4}));
  ASSERT_EQ(t1["c"], std::vector<int>({5, 6, 7}));
}

TEST(Table, ComplexCombineNonEmpty)
{
  Table<int> t1
    {{
       {"a", {1, 2, 3}},
       {"b", {4, 5, 6}},
     }};
  Table<int> t2{"c", 7};
  t1.combine(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)3);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)3);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "b"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "c"), fields.end());
  ASSERT_EQ(t1["a"], std::vector<int>({1, 2, 3}));
  ASSERT_EQ(t1["b"], std::vector<int>({4, 5, 6}));
  ASSERT_EQ(t1["c"], std::vector<int>({7, 7, 7}));
}

TEST(Table, ComplexCombineComplex)
{
  Table<int> t1
    {{
       {"a", {1, 4, 7}},
       {"b", {2, 5, 8}},
       {"c", {3, 6, 9}},
     }};
  Table<int> t2
    {{
       {"aa", {11, 13}},
       {"bb", {12, 14}},
     }};
  t1.combine(t2);
  ASSERT_EQ(t1.getHeight(), (size_t)6);
  auto fields = t1.getFields();
  ASSERT_EQ(fields.size(), (size_t)5);
  ASSERT_NE(std::find(fields.begin(), fields.end(), "a"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "b"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "c"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "aa"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "bb"), fields.end());
  ASSERT_EQ(t1["a"], std::vector<int>({1, 4, 7, 1, 4, 7}));
  ASSERT_EQ(t1["b"], std::vector<int>({2, 5, 8, 2, 5, 8}));
  ASSERT_EQ(t1["c"], std::vector<int>({3, 6, 9, 3, 6, 9}));
  ASSERT_EQ(t1["aa"], std::vector<int>({11, 11, 11, 13, 13, 13}));
  ASSERT_EQ(t1["bb"], std::vector<int>({12, 12, 12, 14, 14, 14}));
}

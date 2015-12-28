#include <gtest/gtest.h>
#include "PlistTable.hpp"


TEST(PlistTable, Primitive)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <true/>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)1);
  auto fields = table.getFields();
  ASSERT_NE(std::find(fields.begin(), fields.end(), "_"), fields.end());
}

TEST(PlistTable, FieldsDictionaryEmpty)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict/>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)0);
}

TEST(PlistTable, FieldsDictionaryOneLevel)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key></key>
    <string>text</string>
    <key>key</key>
    <true/>
  </dict>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)2);
  auto fields = table.getFields();
  ASSERT_NE(std::find(fields.begin(), fields.end(), "_"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key"), fields.end());
}

TEST(PlistTable, FieldsDictionaryTwoLevels)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key></key>
    <string>text</string>
    <key>key1</key>
    <dict>
      <key></key>
      <string></string>
      <key>key2</key>
      <true/>
    </dict>
  </dict>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)3);
  auto fields = table.getFields();
  ASSERT_NE(std::find(fields.begin(), fields.end(), "_"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1.key2"), fields.end());
}

TEST(PlistTable, FieldsDictionaryThreeLevels)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key></key>
    <string>text</string>
    <key>key1</key>
    <dict>
      <key></key>
      <string></string>
      <key>key2</key>
      <dict>
        <key></key>
        <string></string>
        <key>key3</key>
        <true/>
      </dict>
    </dict>
  </dict>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)4);
  auto fields = table.getFields();
  ASSERT_NE(std::find(fields.begin(), fields.end(), "_"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1.key2"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1.key2.key3"), fields.end());
}

TEST(PlistTable, FieldsArrayEmpty)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <array/>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)0);
}

TEST(PlistTable, FieldsArrayOneLevel)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <array>
    <true/>
    <string></string>
  </array>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)1);
  auto fields = table.getFields();
  ASSERT_NE(std::find(fields.begin(), fields.end(), "_"), fields.end());
}

TEST(PlistTable, FieldsArrayTwoLevels)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <array>
    <true/>
    <string></string>
    <array>
      <true/>
      <string></string>
    </array>
  </array>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)2);
  auto fields = table.getFields();
  ASSERT_NE(std::find(fields.begin(), fields.end(), "_"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "_._"), fields.end());
}

TEST(PlistTable, FieldsArrayThreeLevels)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <array>
    <array>
      <true/>
      <array>
        <true/>
        <string></string>
      </array>
    </array>
    <true/>
    <string></string>
  </array>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)3);
  auto fields = table.getFields();
  ASSERT_NE(std::find(fields.begin(), fields.end(), "_"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "_._"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "_._._"), fields.end());
}

TEST(PlistTable, FieldsArrayOfDictionaries)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <array>
    <dict>
      <key>key1</key>
      <string>value1</string>
      <key>key3</key>
      <string>value3</string>
    </dict>
    <dict>
      <key>key1</key>
      <string>value1</string>
      <key>key2</key>
      <string>value2</string>
    </dict>
  </array>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)3);
  auto fields = table.getFields();
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key2"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key3"), fields.end());
}

TEST(PlistTable, FieldsDictionaryOfArrays)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key>key1</key>
    <array>
      <true/>
      <string></string>
    </array>
    <key>key2</key>
    <array>
      <true/>
      <string></string>
      <string></string>
    </array>
  </dict>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)2);
  auto fields = table.getFields();
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key2"), fields.end());
}

TEST(PlistTable, FieldsDictionaryOfArraysOfDictioniaries)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key>key1</key>
    <array>
      <true/>
      <string></string>
      <dict>
        <key>key2</key>
        <array>
          <true/>
          <string></string>
          <array>
            <true/>
            <false/>
            <array>
              <array/>
              <array/>
            </array>
          </array>
        </array>
      </dict>
    </array>
    <key>key3</key>
    <true/>
  </dict>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)4);
  auto fields = table.getFields();
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1.key2"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1.key2._"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key3"), fields.end());
}

TEST(PlistTable, FieldsComplexStructure)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key>key1</key>
    <array>
      <dict>
        <key></key>
        <string>value1</string>
        <key>key2</key>
        <string>value2</string>
        <key>key3</key>
        <array>
          <dict>
            <key>key4</key>
            <string>value4</string>
            <key>key5</key>
            <string>value5</string>
          </dict>
          <integer>2</integer>
          <integer>3</integer>
          <integer>4</integer>
          <integer>5</integer>
          <array>
            <integer>2</integer>
            <integer>3</integer>
          </array>
        </array>
      </dict>
      <dict>
        <key></key>
        <string>value1</string>
        <key>key2</key>
        <string>value2</string>
        <key>key3</key>
        <array>
          <dict>
            <key>key6</key>
            <string>value6</string>
          </dict>
          <integer>5</integer>
        </array>
      </dict>
    </array>
  </dict>
</plist>
)";
  PlistTable table;
  ASSERT_EQ(table.load(xml.c_str(), xml.length(), 0), true);
  ASSERT_EQ(table.getFields().size(), (size_t)7);
  auto fields = table.getFields();
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1.key2"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1.key3"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1.key3._"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1.key3.key4"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1.key3.key5"), fields.end());
  ASSERT_NE(std::find(fields.begin(), fields.end(), "key1.key3.key6"), fields.end());
}

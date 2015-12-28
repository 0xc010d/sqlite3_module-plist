#include <gtest/gtest.h>
#include <Plist.hpp>


TEST(Plist, Invalid)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict/>
)";
  auto plist = Plist::parse(xml.c_str(), xml.length());
  ASSERT_EQ(plist.isValid(), false);
}

TEST(Plist, Empty)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0"></plist>
)";
  auto plist = Plist::parse(xml.c_str(), xml.length());
  ASSERT_EQ(plist.isValid(), false);
}

TEST(Plist, DictionaryEmpty)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict/>
</plist>
)";
  auto plist = Plist::parse(xml.c_str(), xml.length());
  ASSERT_EQ(plist.isRow(), true);
  ASSERT_EQ(plist.rowValue().empty(), true);
}

TEST(Plist, DictionaryNonEmpty)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key>key1</key>
    <true/>
    <key>key2</key>
    <real>0.82</real>
    <key>key3</key>
    <integer>19</integer>
    <key>key4</key>
    <string>text</string>
  </dict>
</plist>
)";
  auto plist = Plist::parse(xml.c_str(), xml.length());
  ASSERT_EQ(plist.isRow(), true);
  ASSERT_EQ(plist.size(), (size_t)4);
  ASSERT_EQ(plist["key1"].isInteger(), true);
  ASSERT_EQ(plist["key1"].integerValue(), 1);
  ASSERT_EQ(plist["key2"].isReal(), true);
  ASSERT_EQ(plist["key2"].realValue(), 0.82);
  ASSERT_EQ(plist["key3"].isInteger(), true);
  ASSERT_EQ(plist["key3"].integerValue(), 19);
  ASSERT_EQ(plist["key4"].isText(), true);
  ASSERT_EQ(plist["key4"].textValue(), "text");
}

TEST(Plist, ArrayEmpty)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <array/>
</plist>
)";
  auto plist = Plist::parse(xml.c_str(), xml.length());
  ASSERT_EQ(plist.isColumn(), true);
  ASSERT_EQ(plist.columnValue().empty(), true);
}

TEST(Plist, ArrayNonEmpty)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <array>
    <true/>
    <string>text</string>
    <real>42</real>
    <integer>12</integer>
  </array>
</plist>
)";
  auto plist = Plist::parse(xml.c_str(), xml.length());
  ASSERT_EQ(plist.isColumn(), true);
  ASSERT_EQ(plist.size(), (size_t)4);
  ASSERT_EQ(plist[0].isInteger(), true);
  ASSERT_EQ(plist[0].integerValue(), 1);
  ASSERT_EQ(plist[1].isText(), true);
  ASSERT_EQ(plist[1].textValue(), "text");
  ASSERT_EQ(plist[2].isReal(), true);
  ASSERT_EQ(plist[2].realValue(), 42.0);
  ASSERT_EQ(plist[3].isInteger(), true);
  ASSERT_EQ(plist[3].integerValue(), 12);
}

TEST(Plist, ComplexStructure)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key></key>
    <array>
      <true/>
      <dict>
        <key></key>
        <string>text</string>
      </dict>
      <array>
        <integer>42</integer>
        <real>12.1</real>
      </array>
    </array>
  </dict>
</plist>
)";
  auto plist = Plist::parse(xml.c_str(), xml.length());
  ASSERT_EQ(plist.isRow(), true);
  ASSERT_EQ(plist.size(), (size_t)1);
  ASSERT_EQ(plist[""].isColumn(), true);
  ASSERT_EQ(plist[""].size(), (size_t)3);
  ASSERT_EQ(plist[""][0].isInteger(), true);
  ASSERT_EQ(plist[""][0].integerValue(), 1);
  ASSERT_EQ(plist[""][1].isRow(), true);
  ASSERT_EQ(plist[""][1].size(), (size_t)1);
  ASSERT_EQ(plist[""][1][""].isText(), true);
  ASSERT_EQ(plist[""][1][""].textValue(), "text");
  ASSERT_EQ(plist[""][2].isColumn(), true);
  ASSERT_EQ(plist[""][2].size(), (size_t)2);
  ASSERT_EQ(plist[""][2][0].isInteger(), true);
  ASSERT_EQ(plist[""][2][0].integerValue(), 42);
  ASSERT_EQ(plist[""][2][1].isReal(), true);
  ASSERT_EQ(plist[""][2][1].realValue(), 12.1);
}

TEST(Plist, DictionaryEmptyKeyPath)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key>key1</key>
    <dict>
      <key>key2</key>
      <string></string>
    </dict>
    <key>key3</key>
    <dict/>
  </dict>
</plist>
)";
  auto plist = Plist::parse(xml.c_str(), xml.length(), "key3");
  ASSERT_EQ(plist.isRow(), true);
  ASSERT_EQ(plist.size(), (size_t)0);
}

TEST(Plist, DictionaryNonEmptyKeyPath)
{
  std::string xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/ PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key>key1</key>
    <string></string>
    <key>key2</key>
    <true/>
    <key>key3</key>
    <dict>
      <key>key4</key>
      <string></string>
    </dict>
  </dict>
</plist>
)";
  auto plist1 = Plist::parse(xml.c_str(), xml.length(), "key1");
  ASSERT_EQ(plist1.isText(), true);
  ASSERT_EQ(plist1.textValue(), "");
  auto plist2 = Plist::parse(xml.c_str(), xml.length(), "key2");
  ASSERT_EQ(plist2.isInteger(), true);
  ASSERT_EQ(plist2.integerValue(), 1);
  auto plist3 = Plist::parse(xml.c_str(), xml.length(), "key3");
  ASSERT_EQ(plist3.isRow(), true);
  ASSERT_EQ(plist3["key4"].isText(), true);
  ASSERT_EQ(plist3["key4"].textValue(), "");
}

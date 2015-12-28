#include "Plist.hpp"
#include <cstdlib>
#import <Foundation/Foundation.h>

static CFPropertyListRef CFPropertyListCreateWithBuffer(const void *buffer, size_t size);

static CFPropertyListRef CFPropertyListCreateWithFilePath(const char *path)
{
  FILE *file = std::fopen(path, "rb");
  if (file == NULL) {
    return NULL;
  }
  std::fseek(file, 0, SEEK_END);
  size_t length = (size_t)std::ftell(file);
  std::fseek(file, 0, SEEK_SET);

  void *buffer = std::malloc(sizeof(uint8_t) * length);
  if (buffer == NULL) {
    return NULL;
  }

  if (std::fread(buffer, sizeof(uint8_t), length, file) != length) {
    std::free(buffer);
    return NULL;
  }

  CFPropertyListRef plist = CFPropertyListCreateWithBuffer(buffer, length);
  std::free(buffer);
  return plist;
}

static CFPropertyListRef CFPropertyListCreateWithBuffer(const void *buffer, size_t size)
{
  CFDataRef data = CFDataCreateWithBytesNoCopy(NULL, (UInt8 *)buffer, size, kCFAllocatorNull);
  if (data == NULL) {
    return NULL;
  }

  CFErrorRef error = NULL;
  CFPropertyListRef plist = CFPropertyListCreateWithData(kCFAllocatorDefault, data, kCFPropertyListImmutable, NULL, &error);
  CFRelease(data);
  return plist;
}

Plist Plist::parse(const std::string &path, const std::string &keyPath)
{
  CFPropertyListRef plist = CFPropertyListCreateWithFilePath(path.c_str());
  auto result = parse(plist, keyPath);
  if (plist != NULL) {
    CFRelease(plist);
  }
  return result;
}

Plist Plist::parse(const void *buffer, size_t size, const std::string &keyPath)
{
  CFPropertyListRef plist = CFPropertyListCreateWithBuffer(buffer, size);
  auto result = parse(plist, keyPath);
  if (plist != NULL) {
    CFRelease(plist);
  }
  return result;
}

Plist Plist::parse(CFPropertyListRef plist, const std::string &keyPath)
{
  if (!keyPath.empty()) {
    @autoreleasepool {
      NSString *path = [NSString stringWithUTF8String:keyPath.c_str()];
      CFPropertyListRef newPlist = (__bridge CFPropertyListRef)[(__bridge id)plist valueForKeyPath:path];
      return Cell::parse(newPlist);
    }
  }
  return Cell::parse(plist);
}

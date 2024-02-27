#include "../xupeng/xupeng.h"
#include <stdint.h>
#include <vector>

static PENG::Logger::ptr g_logger = PENG_LOG_ROOT();

#define XX(type, len, write_fun, read_fun, base_len)                           \
  {                                                                            \
    std::vector<type> vec;                                                     \
    for (int i = 0; i < len; ++i) {                                            \
      vec.push_back(rand());                                                   \
    }                                                                          \
    PENG::ByteArray::ptr ba(new PENG::ByteArray(base_len));                    \
    for (auto &i : vec) {                                                      \
      ba->write_fun(i);                                                        \
    }                                                                          \
    ba->setPosition(0);                                                        \
    for (size_t i = 0; i < vec.size(); ++i) {                                  \
      type v = ba->read_fun();                                                 \
      /*PENG_LOG_INFO(g_logger) << i << " - "                                  \
                              << " - " << vec[i];  */                          \
      PENG_ASSERT(v == vec[i]);                                                \
    }                                                                          \
    PENG_ASSERT(ba->getReadSize() == 0);                                       \
    PENG_LOG_INFO(g_logger)                                                    \
        << #write_fun "/" #read_fun " (" #type " ) len = " << len              \
        << " base_len = " << base_len << " size = " << ba->getSize();          \
  }

#define XXX(type, len, write_fun, read_fun, base_len)                          \
  {                                                                            \
    std::vector<type> vec;                                                     \
    for (int i = 0; i < len; ++i) {                                            \
      vec.push_back(rand());                                                   \
    }                                                                          \
    PENG::ByteArray::ptr ba(new PENG::ByteArray(base_len));                    \
    for (auto &i : vec) {                                                      \
      ba->write_fun(i);                                                        \
    }                                                                          \
    ba->setPosition(0);                                                        \
    for (size_t i = 0; i < vec.size(); ++i) {                                  \
      type v = ba->read_fun();                                                 \
      /*PENG_LOG_INFO(g_logger) << i << " - "                                  \
                              << " - " << vec[i];  */                          \
      PENG_ASSERT(v == vec[i]);                                                \
    }                                                                          \
    PENG_ASSERT(ba->getReadSize() == 0);                                       \
    PENG_LOG_INFO(g_logger)                                                    \
        << #write_fun "/" #read_fun " (" #type " ) len = " << len              \
        << " base_len = " << base_len << " size = " << ba->getSize();          \
    ba->setPosition(0);                                                        \
    PENG_ASSERT(ba->writeToFile("/tmp/" #type "_" #len "-" #read_fun ".dat")); \
    PENG::ByteArray::ptr ba2(new PENG::ByteArray(base_len * 2));               \
    PENG_ASSERT(                                                               \
        ba2->readFromFile("/tmp/" #type "_" #len "-" #read_fun ".dat"));       \
    ba2->setPosition(0);                                                       \
    PENG_ASSERT(ba->toString() == ba2->toString());                            \
    PENG_ASSERT(ba->getPosition() == 0);                                       \
    PENG_ASSERT(ba2->getPosition() == 0);                                      \
  }

void test() {
  // std::vector<int32_t> vec;
  // for (int i = 0; i < 10; ++i) {
  //   vec.push_back(rand());
  // }
  // PENG::ByteArray::ptr ba(new PENG::ByteArray(1));
  // for (auto &i : vec) {
  //   ba->writeFint32(i);
  // }
  // ba->setPosition(0);
  // for (size_t i = 0; i < vec.size(); ++i) {
  //   int32_t v = ba->readFint32();
  //   PENG_ASSERT(v == vec[i]);
  // }
  XX(int8_t, 100, writeFint8, readFint8, 1);
  XX(uint8_t, 100, writeFuint8, readFuint8, 1);
  XX(int16_t, 100, writeFint16, readFint16, 1);
  XX(uint16_t, 100, writeFuint16, readFuint16, 1);
  XX(int32_t, 100, writeFint32, readFint32, 1);
  XX(uint32_t, 100, writeFuint32, readFuint32, 1);
  XX(int64_t, 100, writeFint64, readFint64, 1);
  XX(uint64_t, 100, writeFuint64, readFuint64, 1);

  XX(int32_t, 100, writeInt32, readInt32, 1);
  XX(uint32_t, 100, writeUint32, readUint32, 1);
  XX(int64_t, 100, writeInt64, readInt64, 1);
  XX(uint64_t, 100, writeUint64, readUint64, 1);

  XXX(int8_t, 100, writeFint8, readFint8, 1);
  XXX(uint8_t, 100, writeFuint8, readFuint8, 1);
  XXX(int16_t, 100, writeFint16, readFint16, 1);
  XXX(uint16_t, 100, writeFuint16, readFuint16, 1);
  XXX(int32_t, 100, writeFint32, readFint32, 1);
  XXX(uint32_t, 100, writeFuint32, readFuint32, 1);
  XXX(int64_t, 100, writeFint64, readFint64, 1);
  XXX(uint64_t, 100, writeFuint64, readFuint64, 1);

  XXX(int32_t, 100, writeInt32, readInt32, 1);
  XXX(uint32_t, 100, writeUint32, readUint32, 1);
  XXX(int64_t, 100, writeInt64, readInt64, 1);
  XXX(uint64_t, 100, writeUint64, readUint64, 1);
  return;
}
#undef XX
#undef XXX

int main() {
  test();
  return 0;
}

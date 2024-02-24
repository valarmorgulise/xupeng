#ifndef __PENG_ADDRESS_H__
#define __PENG_ADDRESS_H__

#include <arpa/inet.h>
#include <cstdint>
#include <cwchar>
#include <map>
#include <memory>
#include <netinet/in.h>
#include <ostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <vector>

namespace PENG {

class IPAddress;

/**
 * @brief 网络地址的基类， 抽象类
 */
class Address {
public:
  typedef std::shared_ptr<Address> ptr;

  /**
   * @brief 通过sockaddr指针创建address
   * @param[in] addr sockaddr指针
   * @param[in] addrlen sockaddr的长度
   * @return 返回和sockaddr相匹配的Address， 失败返回nullptr
   *
   */
  static Address::ptr Create(const sockaddr *addr, socklen_t addrlen);

  /**
   * @brief 通过host地址返回对应条件的所有address
   * @param[in] result 保存满足条件的address
   * @param[in] host 域名，服务器名
   * @param[in] family 协议族(AF_INET, AF_INET6, AF_UNIX)
   * @param[in] type socket 类型SOCK_STREAM\ SOCK_DGRAM等
   * @param[in] protocol 协议，IPPROTO_TCP, IPPROTO_UDP等
   * @return 返回是否转换成功
   *
   */
  static bool Lookup(std::vector<Address::ptr> &result, const std::string &host,
                     int family = AF_INET, int type = 0, int protocol = 0);

  /**
   * @brief 通过host地址返回对应条件的任意address
   * @param[in] host 域名，服务器名
   * @param[in] family 协议族(AF_INET, AF_INET6, AF_UNIX)
   * @param[in] type socket 类型SOCK_STREAM\ SOCK_DGRAM等
   * @param[in] protocol 协议，IPPROTO_TCP, IPPROTO_UDP等
   * @return 返回满足条件的任意Address， 失败返回nullptr
   *
   */
  static Address::ptr LookupAny(const std::string &host, int family = AF_INET,
                                int type = 0, int protocol = 0);

  /**
   * @brief 通过host地址返回对应条件的任意address
   * @param[in] host 域名，服务器名
   * @param[in] family 协议族(AF_INET, AF_INET6, AF_UNIX)
   * @param[in] type socket 类型SOCK_STREAM\ SOCK_DGRAM等
   * @param[in] protocol 协议，IPPROTO_TCP, IPPROTO_UDP等
   * @return 返回满足条件的任意Address， 失败返回nullptr
   *
   */
  static std::shared_ptr<IPAddress> LookupAnyIPAddress(const std::string &host,
                                                       int family = AF_INET,
                                                       int type = 0,
                                                       int protocol = 0);

  /**
   * @brief 返回本机所有网卡的<网卡名，地址，子网掩码数量>
   * @param[out] result 保存本机所有地址
   * @param[in] family 协议族(AF_INET, AF_INET6, AF_UNIX)
   * @return 是否返回成功
   *
   */
  static bool GetInterfaceAddresses(
      std::multimap<std::string, std::pair<Address::ptr, uint32_t>> &result,
      int family = AF_INET);

  /**
   * @brief 获取指定的网卡的地址和子网掩码数量
   * @param[out] result 保存本机指定的网卡的所有地址
   * @param[in] iface 网卡名称
   * @param[in] family 协议族(AF_INET, AF_INET6, AF_UNIX)
   * @return 是否获取成功
   *
   */
  static bool
  GetInterfaceAddresses(std::vector<std::pair<Address::ptr, uint32_t>> &result,
                        const std::string &iface, int family = AF_INET);

  /**
   * @brief 虚析构函数
   */
  virtual ~Address() {}

  /**
   * @brief 返回协议族
   */
  int getFamily() const;

  /**
   * @brief 返回 sockaddr 指针， 只读
   */
  virtual const sockaddr *getAddr() const = 0;

  /**
   * @brief 返回 sockaddr 指针， 读写
   */
  virtual sockaddr *getAddr() = 0;

  /**
   * @brief 返回 sockaddr的长度
   */
  virtual socklen_t getAddrLen() const = 0;

  /**
   * @brief 返回 可读性输出地址
   */
  virtual std::ostream &insert(std::ostream &os) const;

  /**
   * @brief 返回 可读性字符串
   */
  std::string toString();

  /**
   * @brief 小于号比较函数
   */
  bool operator<(const Address &rhs) const;

  /**
   * @brief 等于号比较函数
   */
  bool operator==(const Address &rhs) const;

  /**
   * @brief 不等于比较函数
   */
  bool operator!=(const Address &rhs) const;
};

/**
 * @brief ip地址的基类
 */
class IPAddress : public Address {
public:
  typedef std::shared_ptr<IPAddress> ptr;

  /**
   * @brief 通过域名，IP， 服务器名创建IPAddress
   * @param[in] address 域名，IP，服务器名等
   * @param[in] port 端口号
   * @return  调用成功返回IPAddress， 失败返回nullptr
   *
   */
  static IPAddress::ptr Create(const char *address, uint16_t port = 0);

  /**
   * @brief 获取该地址的广播地址
   * @param[in] prefix_len 子网掩码位数
   * @return  调用成功返回IPAddress， 失败返回nullptr
   *
   */
  virtual IPAddress::ptr broadcastAddress(uint32_t prefix_len) = 0;

  /**
   * @brief 获取该地址的网段
   * @param[in] prefix_len 子网掩码位数
   * @return  调用成功返回IPAddress， 失败返回nullptr
   *
   */
  virtual IPAddress::ptr networkAddress(uint32_t prefix_len) = 0;

  /**
   * @brief 获取子网掩码地址
   * @param[in] prefix_len 子网掩码位数
   * @return  调用成功返回IPAddress， 失败返回nullptr
   *
   */
  virtual IPAddress::ptr subnetMask(uint32_t prefix_len) = 0;

  /**
   * @brief 返回端口号
   */
  virtual uint32_t getPort() const = 0;

  /**
   * @brief 设置端口号
   */
  virtual void setPort(uint16_t v) = 0;
};

/**
 * @brief IPv4地址
 */
class IPv4Address : public IPAddress {
public:
  typedef std::shared_ptr<IPv4Address> ptr;

  /**
   * @brief 利用点分十进制地址创建IPv4Address
   * @param[in] address 点分十进制地址，如192.168.1.1
   * @param[in] port 端口号
   * @return  返回IPv4Address， 失败返回nullptr
   *
   */
  static Address::ptr Create(const char *address, uint16_t port = 0);

  /**
   * @brief 通过sockaddr_in构造IPv4Address
   * @param[in] address sockaddr_in 结构体
   *
   */
  IPv4Address(const sockaddr_in &address);

  /**
   * @brief 通过二进制地址构造IPv4Address
   * @param[in] address 二进制地址结构体
   *
   */
  IPv4Address(uint32_t address = INADDR_ANY, uint16_t port = 0);

  const sockaddr *getAddr() const override;
  sockaddr *getAddr() override;
  socklen_t getAddrLen() const override;
  std::ostream &insert(std::ostream &os) const override;

  IPAddress::ptr broadcastAddress(uint32_t prefix_len) override;
  IPAddress::ptr networkAddress(uint32_t prefix_len) override;
  IPAddress::ptr subnetMask(uint32_t prefix_len) override;
  uint32_t getPort() const override;
  void setPort(uint16_t v) override;

private:
  sockaddr_in m_addr;
};

/**
 * @brief IPv6地址
 */
class IPv6Address : public IPAddress {
public:
  typedef std::shared_ptr<IPv6Address> ptr;

  /**
   * @brief 通过IPv6地址字符串构造IPv6Address
   * @param[in] address IPv6地址字符串
   * @param[in] port 端口号
   * @return  返回IPv6Address， 失败返回nullptr
   *
   */
  static Address::ptr Create(const char *address, uint16_t port = 0);

  /**
   * @brief 无参构造
   */
  IPv6Address();

  /**
   * @brief 通过sockaddr_in6构造IPv6Address
   * @param[in] address sockaddr_in6结构体
   */
  IPv6Address(const sockaddr_in6 &address);

  /**
   * @brief 通过IPv6二进制地址构造IPv6Address
   * @param[in] address IPv6二进制地址
   */
  IPv6Address(const uint8_t address[16], uint16_t port);
  // IPv6Address(uint32_t address = INADDR_ANY, uint32_t port = 0);
  const sockaddr *getAddr() const override;
  sockaddr *getAddr() override;
  socklen_t getAddrLen() const override;
  std::ostream &insert(std::ostream &os) const override;

  IPAddress::ptr broadcastAddress(uint32_t prefix_len) override;
  IPAddress::ptr networkAddress(uint32_t prefix_len) override;
  IPAddress::ptr subnetMask(uint32_t prefix_len) override;
  uint32_t getPort() const override;
  void setPort(uint16_t v) override;

private:
  sockaddr_in6 m_addr;
};

/**
 * @brief UnixSocket地址
 */
class UnixAddress : public Address {
public:
  typedef std::shared_ptr<UnixAddress> ptr;
  /**
   * @brief 无参构造函数
   */
  UnixAddress();

  /**
   * @brief 通过路径构造UnixAddress
   * @param[in] path UnixSocket路径(长度小于UNIX_PATH_MAX)
   */
  UnixAddress(const std::string &path);

  const sockaddr *getAddr() const override;
  sockaddr *getAddr() override;
  socklen_t getAddrLen() const override;
  void setAddrLen(uint32_t v);
  std::string getPath() const;
  std::ostream &insert(std::ostream &os) const override;

private:
  sockaddr_un m_addr;
  socklen_t m_length;
};

/**
 * @brief 未知地址
 */
class UnknownAddress : public Address {
public:
  typedef std::shared_ptr<UnknownAddress> ptr;
  UnknownAddress(int family);
  UnknownAddress(const sockaddr &addr);
  sockaddr *getAddr() override;
  const sockaddr *getAddr() const override;
  socklen_t getAddrLen() const override;
  std::ostream &insert(std::ostream &os) const override;

private:
  sockaddr m_addr;
};

/**
 * @brief 流式输出Address
 */
std::ostream &operator<<(std::ostream &os, const Address &addr);

} // namespace PENG

#endif // !__PENG_ADDRESS_H__

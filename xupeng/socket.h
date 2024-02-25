#ifndef __PENG_SOCKET_H__
#define __PENG_SOCKET_H__

#include "address.h"
#include "noncopyable.h"
#include <cstdint>
#include <memory>
#include <sys/socket.h>

namespace PENG {

class Socket : public std::enable_shared_from_this<Socket>, Noncopyable {
public:
  typedef std::shared_ptr<Socket> ptr;
  typedef std::weak_ptr<Socket> weak_ptr;

  /**
   * @brief Socket type
   */
  enum Type {
    /// TCP Type
    TCP = SOCK_STREAM,
    /// UDP Type
    UDP = SOCK_DGRAM
  };

  /**
   * @brief Socket Family
   */
  enum Family {
    /// IPv4 socket
    IPv4 = AF_INET,
    /// IPv6 socket
    IPv6 = AF_INET6,
    /// Unix socket
    UNIX = AF_UNIX,
  };

  /**
   * @brief Create TCP Socket
   * @param[in] address address
   */
  static Socket::ptr CreateTCP(PENG::Address::ptr address);

  /**
   * @brief Create TCP Socket
   * @param[in] address address
   */
  static Socket::ptr CreateUDP(PENG::Address::ptr address);

  /**
   * @brief Create IPV4 TCP Socket
   */
  static Socket::ptr CreateTCPSocket();

  /**
   * @brief Create IPV6 TCP Socket
   */
  static Socket::ptr CreateUDPSocket();

  /**
   * @brief Create IPV6 TCP Socket
   */
  static Socket::ptr CreateTCPSocket6();

  /**
   * @brief Create IPV6 UDP Socket
   */
  static Socket::ptr CreateUDPSocket6();

  /**
   * @brief Create UNIX TCP Socket
   */
  static Socket::ptr CreateUnixTCPSocket();

  /**
   * @brief Create UNIX UDP Socket
   */
  static Socket::ptr CreateUnixUDPSocket();

  /*
   * @brief Socket Constructor
   * @param[in] family  protocol family
   * @param[in] type    it's about type
   * @param[in] protocol  its just a protocol
   */
  Socket(int family, int type, int protocol = 0);

  /**
   * @brief Destructor
   */
  virtual ~Socket();

  /**
   * @brief Get Send Time out(ms)
   */
  int64_t getSendTimeout();

  /**
   * @brief Set Send Time out(ms)
   */
  void setSendTimeout(int64_t v);

  /**
   * @brief Get Receive Time out(ms)
   */
  int64_t getRecvTimeout();

  /**
   * @brief Set Receive Time out(ms)
   */
  void setRecvTimeout(int64_t v);

  /**
   * @brief get sockopt @see getsockopt
   */
  bool getOption(int level, int option, void *result, socklen_t *len);

  /**
   * @brief a template for sockopt @see getsockopt
   */
  template <class T> bool getOption(int level, int option, T &result) {
    socklen_t length = sizeof(T);
    return getOption(level, option, &result, &length);
  }

  /**
   * @brief set sockopt @see setsockopt
   */
  bool setOption(int level, int option, const void *result, socklen_t len);

  /**
   * @brief a template for sockopt @see setsockopt
   */
  template <class T> bool setOption(int level, int option, const T &value) {
    return setOption(level, option, &value, sizeof(T));
  }

  /**
   * @brief Receive connect
   * @return success return new socket, fail return nullptr
   * @pre Socket must be bind, listen then success
   */
  virtual Socket::ptr accept();

  /**
   * @brief bind address
   * @param[in] addr address
   * @return bind success?
   */
  virtual bool bind(const Address::ptr addr);

  /**
   * @brief connect address
   * @param[in] addr address goal
   * @return timeout_ms time out(ms)
   */
  virtual bool connect(const Address::ptr addr, uint64_t timeout_ms = -1);

  virtual bool reconnect(uint64_t timeout_ms = -1);

  /**
   * @brief listen socket
   * @param[in] backlog the max length of not connect
   * @result connect is success
   * @pre must be bind success
   */
  virtual bool listen(int backlog = SOMAXCONN);

  /**
   * @brief close socket
   */
  virtual bool close();

  /**
   * @brief send datas
   * @param[in] buffer dai fasong shuju de neicun
   * @param[in] length dai fasong shuju de changdu
   * @param[in] flags flag
   * @return
   *      @retval > 0 send success duiyingdaixiao de shuju
   *      @retval = 0 socket closed
   *      @retval < 0 socket error
   */
  virtual int send(const void *buffer, size_t length, int flags = 0);

  /**
   * @brief send datas
   * @param[in] buffer dai fasong shuju de neicun(iovec shuzu)
   * @param[in] length dai fasong shuju de changdu(iovec length)
   * @param[in] flags flag
   * @return
   *      @retval > 0 send success duiyingdaixiao de shuju
   *      @retval = 0 socket closed
   *      @retval < 0 socket error
   */
  virtual int send(const iovec *buffers, size_t length, int flags = 0);

  /**
   * @brief send datas
   * @param[in] buffer dai fasong shuju de neicun()
   * @param[in] length dai fasong shuju de changdu()
   * @param[in] to send to goal address
   * @param[in] flags flag
   * @return
   *      @retval > 0 send success duiyingdaixiao de shuju
   *      @retval = 0 socket closed
   *      @retval < 0 socket error
   */
  virtual int sendTo(const void *buffer, size_t length, const Address::ptr to,
                     int flags = 0);

  /**
   * @brief send datas
   * @param[in] buffer dai fasong shuju de neicun(iovec shuzu)
   * @param[in] length dai fasong shuju de changdu(iovec length)
   * @param[in] to send to goal address
   * @param[in] flags flag
   * @return
   *      @retval > 0 send success duiyingdaixiao de shuju
   *      @retval = 0 socket closed
   *      @retval < 0 socket error
   */
  virtual int sendTo(const iovec *buffer, size_t length, const Address::ptr to,
                     int flags = 0);

  /**
   * @brief receive datas
   * @param[in] buffer jieshou shuju de neicun()
   * @param[in] length jieshou shuju de changdu()
   * @param[in] flags flag
   * @return
   *      @retval > 0 send success duiyingdaixiao de shuju
   *      @retval = 0 socket closed
   *      @retval < 0 socket error
   */
  virtual int recv(void *buffer, size_t length, int flags = 0);

  /**
   * @brief receive datas
   * @param[in] buffer jieshou shuju de neicun(iovec shuzu)
   * @param[in] length jieshou shuju de changdu(iovec length)
   * @param[in] flags flag
   * @return
   *      @retval > 0 send success duiyingdaixiao de shuju
   *      @retval = 0 socket closed
   *      @retval < 0 socket error
   */
  virtual int recv(iovec *buffers, size_t length, int flags = 0);

  /**
   * @brief receive datas
   * @param[in] buffer jieshou shuju de neicun()
   * @param[in] length jieshou shuju de changdu()
   * @param[in] from send address
   * @param[in] flags flag
   * @return
   *      @retval > 0 send success duiyingdaixiao de shuju
   *      @retval = 0 socket closed
   *      @retval < 0 socket error
   */
  virtual int recvFrom(void *buffer, size_t length, Address::ptr from,
                       int flags = 0);

  /**
   * @brief receive datas
   * @param[in] buffer jieshou shuju de neicun(iovec shuzu)
   * @param[in] length jieshou shuju de changdu(iovec length)
   * @param[in] from send address
   * @param[in] flags flag
   * @return
   *      @retval > 0 send success duiyingdaixiao de shuju
   *      @retval = 0 socket closed
   *      @retval < 0 socket error
   */
  virtual int recvFrom(iovec *buffers, size_t length, Address::ptr from,
                       int flags = 0);

  /**
   * @brief get remote addresses
   */
  Address::ptr getRemoteAddresses();

  /**
   * @brief get localhost addresses
   */
  Address::ptr getLocalAddress();

  /**
   * @brief get protocol family
   */
  int getFamily() const { return m_family; }

  /**
   * @brief get type
   */
  int getType() const { return m_type; }

  /**
   * @brief get protocol
   */
  int getProtocol() const { return m_protocol; }

  /**
   * @brief return connect ?
   */
  bool isConnected() const { return m_isConnected; }

  /**
   * @brief return isValid
   */
  bool isValid() const;

  /**
   * @brief return Socket error
   */
  int getError();

  /**
   * @brief out ostream to stream
   */
  virtual std::ostream &dump(std::ostream &os) const;

  virtual std::string toString() const;

  /**
   * @brief return socket jubing
   */
  int getSoket() const { return m_sock; }

  /**
   * @brief cancel read
   */
  int cancelRead();

  /**
   * @brief cancel write
   */
  int cancelWrite();

  /**
   * @brief cancel accept
   */
  int cancelAccept();

  /**
   * @brief cancel all things
   */
  bool cancelAll();

protected:
  /**
   * @brief init socket
   */
  void initSock();

  /**
   * @brief create socket
   */
  void newSock();

  /**
   * @brief init sock
   */
  virtual bool init(int sock);

protected:
  /// socket jubing
  int m_sock;
  /// protocol family
  int m_family;
  /// type
  int m_type;
  /// protocol
  int m_protocol;
  /// connect ?
  bool m_isConnected;
  /// local Addressed
  Address::ptr m_localAddress;
  /// remote Addressed
  Address::ptr m_remoteAddress;
};

class SSLSocket : public Socket {
public:
  typedef std::shared_ptr<SSLSocket> ptr;

  static SSLSocket::ptr CreateTCP(PENG::Address::ptr address);
  static SSLSocket::ptr CreateTCPSocket();
  static SSLSocket::ptr CreateTCPSocket6();

private:
};
} // namespace PENG

#endif // !__PENG_SOCKET_H__

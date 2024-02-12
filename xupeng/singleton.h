#ifndef __PENG_SIGNLETON_H
#define __PENG_SIGNLETON_H

#include <memory>
namespace PENG {

namespace {

template <class T, class X, int N> T &GetInstanceX() {
  static T v;
  return v;
}

template <class T, class X, int N> std::shared_ptr<T> GetInstancePtr() {
  static std::shared_ptr<T> v(new T);
  return v;
}

} // namespace

template <class T, class X = void, int N = 0> class Singleton {
public:
  static T *GetInstance() {
    static T v;
    return &v;
  }
};

template <class T, class X = void, int N = 0> class SingletonPtr {
public:
  static std::shared_ptr<T> *GetInstance() {
    static std::shared_ptr<T> v(new T);
    return v;
  }
};

} // namespace PENG

#endif // !

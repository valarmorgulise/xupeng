#ifndef __PENG_CONFIG_H
#define __PENG_CONFIG_H

#include "log.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <cctype>
#include <cstdint>
#include <ctype.h>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/node/type.h>
#include <yaml-cpp/yaml.h>

namespace PENG {

class ConfigVarBase {
public:
  typedef std::shared_ptr<ConfigVarBase> ptr;
  ConfigVarBase(const std::string &name, const std::string &decription = "")
      : m_name(name), m_decription(decription) {
    std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
  }
  virtual ~ConfigVarBase() {}
  const std::string &getName() const { return m_name; }
  const std::string &getDecription() const { return m_decription; }
  virtual std::string toString() = 0;
  virtual bool fromString(const std::string &val) = 0;
  virtual std::string GetTypeName() const = 0;

protected:
  std::string m_name;
  std::string m_decription;
};

// F from type, T to_type
template <class F, class T> class LexicalCast {
public:
  T operator()(const F &v) { return boost::lexical_cast<T>(v); }
};

template <class T> class LexicalCast<std::string, std::vector<T>> {
public:
  std::vector<T> operator()(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    typename std::vector<T> vec;
    std::stringstream ss;
    for (size_t i = 0; i < node.size(); ++i) {
      ss.str("");
      ss << node[i];
      vec.push_back(LexicalCast<std::string, T>()(ss.str()));
    }
    return vec;
  }
};

template <class T> class LexicalCast<std::vector<T>, std::string> {
public:
  std::string operator()(const std::vector<T> &v) {
    YAML::Node node(YAML::NodeType::Sequence);
    for (auto &i : v) {
      node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
  }
};

template <class T> class LexicalCast<std::string, std::list<T>> {
public:
  std::list<T> operator()(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    typename std::list<T> list;
    std::stringstream ss;
    for (size_t i = 0; i < node.size(); ++i) {
      ss.str("");
      ss << node[i];
      list.push_back(LexicalCast<std::string, T>()(ss.str()));
    }
    return list;
  }
};

template <class T> class LexicalCast<std::list<T>, std::string> {
public:
  std::string operator()(const std::list<T> &v) {
    YAML::Node node(YAML::NodeType::Sequence);
    for (auto &i : v) {
      node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
  }
};

template <class T> class LexicalCast<std::string, std::set<T>> {
public:
  std::set<T> operator()(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    typename std::set<T> set;
    std::stringstream ss;
    for (size_t i = 0; i < node.size(); ++i) {
      ss.str("");
      ss << node[i];
      set.insert(LexicalCast<std::string, T>()(ss.str()));
    }
    return set;
  }
};

template <class T> class LexicalCast<std::set<T>, std::string> {
public:
  std::string operator()(const std::set<T> &v) {
    YAML::Node node(YAML::NodeType::Sequence);
    for (auto &i : v) {
      node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
  }
};

template <class T> class LexicalCast<std::string, std::unordered_set<T>> {
public:
  std::unordered_set<T> operator()(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    typename std::unordered_set<T> unset;
    std::stringstream ss;
    for (size_t i = 0; i < node.size(); ++i) {
      ss.str("");
      ss << node[i];
      unset.insert(LexicalCast<std::string, T>()(ss.str()));
    }
    return unset;
  }
};

template <class T> class LexicalCast<std::unordered_set<T>, std::string> {
public:
  std::string operator()(const std::unordered_set<T> &v) {
    YAML::Node node(YAML::NodeType::Sequence);
    for (auto &i : v) {
      node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
  }
};

template <class T> class LexicalCast<std::string, std::map<std::string, T>> {
public:
  std::map<std::string, T> operator()(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    typename std::map<std::string, T> map;
    std::stringstream ss;
    for (auto it = node.begin(); it != node.end(); ++it) {
      ss.str("");
      ss << it->second;
      map.insert(std::make_pair(it->first.Scalar(),
                                LexicalCast<std::string, T>()(ss.str())));
    }
    return map;
  }
};

template <class T> class LexicalCast<std::map<std::string, T>, std::string> {
public:
  std::string operator()(const std::map<std::string, T> &v) {
    YAML::Node node(YAML::NodeType::Map);
    for (auto &i : v) {
      node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
  }
};

template <class T>
class LexicalCast<std::string, std::unordered_map<std::string, T>> {
public:
  std::unordered_map<std::string, T> operator()(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    typename std::unordered_map<std::string, T> map;
    std::stringstream ss;
    for (auto it = node.begin(); it != node.end(); ++it) {
      ss.str("");
      ss << it->second;
      map.insert(std::make_pair(it->first.Scalar(),
                                LexicalCast<std::string, T>()(ss.str())));
    }
    return map;
  }
};

template <class T>
class LexicalCast<std::unordered_map<std::string, T>, std::string> {
public:
  std::string operator()(const std::unordered_map<std::string, T> &v) {
    YAML::Node node(YAML::NodeType::Map);
    for (auto &i : v) {
      node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
  }
};

// FromStr T operator() (const std::string &)
// ToStr std::string operator() (const T&)
template <class T, class FromStr = LexicalCast<std::string, T>,
          class ToStr = LexicalCast<T, std::string>>
class ConfigVar : public ConfigVarBase {
public:
  typedef std::shared_ptr<ConfigVar> ptr;
  typedef std::function<void(const T &old_value, const T &new_value)>
      on_change_cb;
  ConfigVar(const std::string &name, const T &default_value,
            const std::string &description = "")
      : ConfigVarBase(name, description), m_val(default_value) {}

  std::string toString() override {
    try {
      // return boost::lexical_cast<std::string>(m_val);
      return ToStr()(m_val);
    } catch (std::exception &e) {
      PENG_LOG_ERROR(PENG_LOG_ROOT())
          << "ConfigVar::toString exception" << e.what()
          << " convert: " << typeid(m_val).name() << " to string";
    }
    return "";
  }

  bool fromString(const std::string &val) override {
    try {
      // m_val = boost::lexical_cast<T>(val);
      SetValue(FromStr()(val));
    } catch (std::exception &e) {
      PENG_LOG_ERROR(PENG_LOG_ROOT())
          << "ConfigVar::toString exception" << e.what()
          << " convert: string to " << typeid(m_val).name() << " - " << val;
    }
    return false;
  }

  const T GetValue() const { return m_val; }

  void SetValue(const T &v) {
    // m_val = v;
    if (v == m_val) {
      return;
    }
    for (auto &i : m_cbs) {
      i.second(m_val, v);
    }
    m_val = v;
  }

  std::string GetTypeName() const override { return typeid(T).name(); }

  void AddListener(/*uint64_t key,*/ on_change_cb cb) {
    static uint64_t s_fun_id = 0;
    ++s_fun_id;
    m_cbs[s_fun_id] = cb;
    return;
  }

  void DelListener(uint64_t key) { m_cbs.erase(key); }

  on_change_cb GetListener(uint64_t key) {
    auto it = m_cbs.find(key);
    return it == m_cbs.end() ? nullptr : it->second;
  }

  void ClearListener() { m_cbs.clear(); }

private:
  T m_val;
  // 变更回调函数族, uint64_t key, 要求唯一， 一般可以用hash
  std::map<uint64_t, on_change_cb> m_cbs;
};

class Config {
public:
  typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

  template <class T>
  static typename ConfigVar<T>::ptr
  Lookup(const std::string &name, const T &default_value,
         const std::string &description = "") {
    auto it = GetDatas().find(name);
    if (it != GetDatas().end()) {
      auto tmp = std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
      if (tmp) {
        PENG_LOG_INFO(PENG_LOG_ROOT()) << "Lookup name=" << name << " exists";
        return tmp;
      } else {
        PENG_LOG_ERROR(PENG_LOG_ROOT())
            << "Lookup name=" << name << " exists but type not "
            << typeid(T).name() << " real_type = " << it->second->GetTypeName()
            << " " << it->second->toString();
        return nullptr;
      }
    }
    if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyz"
                               "._012345678") != std::string::npos) {
      PENG_LOG_ERROR(PENG_LOG_ROOT()) << "Look name invalid " << name;
      throw std::invalid_argument(name);
      return nullptr;
    }
    typename ConfigVar<T>::ptr v(
        new ConfigVar<T>(name, default_value, description));
    GetDatas()[name] = v;
    return v;
  }

  template <class T>
  static typename ConfigVar<T>::ptr Lookup(const std::string &name) {
    auto it = GetDatas().find(name);
    if (it == GetDatas().end()) {
      return nullptr;
    }
    return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
  }

  static void LoadFromYaml(const YAML::Node &root);
  static ConfigVarBase::ptr LookupBase(const std::string &name);

private:
  static ConfigVarMap &GetDatas() {
    static ConfigVarMap m_datas;
    return m_datas;
  }
};

} // namespace PENG

#endif

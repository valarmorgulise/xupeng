#include "../xupeng/config.h"
#include "../xupeng/log.h"
#include <iostream>
#include <map>
#include <string>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>

#define XX(g_var, name, prefix)                                                \
  {                                                                            \
    auto &v = g_var->GetValue();                                               \
    for (auto &i : v) {                                                        \
      PENG_LOG_INFO(PENG_LOG_ROOT()) << #prefix " " #name ": " << i;           \
    }                                                                          \
    PENG_LOG_INFO(PENG_LOG_ROOT())                                             \
        << #prefix " " #name " yaml: " << g_var->toString();                   \
  }

#define XX_M(g_var, name, prefix)                                              \
  {                                                                            \
    auto &v = g_var->GetValue();                                               \
    for (auto &i : v) {                                                        \
      PENG_LOG_INFO(PENG_LOG_ROOT())                                           \
          << #prefix " " #name ": {" << i.first << " - " << i.second << "}";   \
    }                                                                          \
    PENG_LOG_INFO(PENG_LOG_ROOT())                                             \
        << #prefix " " #name " yaml: " << g_var->toString();                   \
  }

#define XX_PM(g_var, prefix)                                                   \
  {                                                                            \
    auto m = g_var->GetValue();                                                \
    for (auto &i : m) {                                                        \
      PENG_LOG_INFO(PENG_LOG_ROOT())                                           \
          << prefix << ": " << i.first << " - " << i.second.toString();        \
    }                                                                          \
    PENG_LOG_INFO(PENG_LOG_ROOT()) << prefix << ": size=" << m.size();         \
  }

PENG::ConfigVar<int>::ptr g_int_value_config =
    PENG::Config::Lookup("system.port", (int)8080, "system port");

PENG::ConfigVar<float>::ptr g_int_valuex_config =
    PENG::Config::Lookup("system.port", (float)8080, "system port");

PENG::ConfigVar<float>::ptr g_float_value_config =
    PENG::Config::Lookup("system.value", (float)10.2f, "system value");

PENG::ConfigVar<std::vector<int>>::ptr g_int_vec_value_config =
    PENG::Config::Lookup("system.int_vec", std::vector<int>{1, 2},
                         "system int vec");
PENG::ConfigVar<std::list<int>>::ptr g_int_list_value_config =
    PENG::Config::Lookup("system.int_list", std::list<int>{1, 2},
                         "system int list");

PENG::ConfigVar<std::set<int>>::ptr g_int_set_value_config =
    PENG::Config::Lookup("system.int_set", std::set<int>{1, 2},
                         "system int set");

PENG::ConfigVar<std::unordered_set<int>>::ptr g_int_unset_value_config =
    PENG::Config::Lookup("system.int_uset", std::unordered_set<int>{1, 2},
                         "system int uset");

PENG::ConfigVar<std::map<std::string, int>>::ptr g_str_int_map_value_config =
    PENG::Config::Lookup("system.int_map", std::map<std::string, int>{{"k", 2}},
                         "system str int map");

PENG::ConfigVar<std::unordered_map<std::string, int>>::ptr
    g_str_int_umap_value_config =
        PENG::Config::Lookup("system.int_umap",
                             std::unordered_map<std::string, int>{{"k", 2}},
                             "system str int umap");
void print_yaml(const YAML::Node &node, int level) {
  if (node.IsScalar()) {
    PENG_LOG_INFO(PENG_LOG_ROOT())
        << std::string(level * 4, ' ') << node.Scalar() << " - " << node.Type()
        << " - " << level;
  } else if (node.IsNull()) {
    PENG_LOG_INFO(PENG_LOG_ROOT()) << std::string(level * 4, ' ') << "NULL - "
                                   << node.Type() << " - " << level;
  } else if (node.IsMap()) {
    for (auto it = node.begin(); it != node.end(); ++it) {
      PENG_LOG_INFO(PENG_LOG_ROOT())
          << std::string(level * 4, ' ') << it->first << " - "
          << it->second.Type() << " - " << level;
      print_yaml(it->second, level + 1);
    }
  } else if (node.IsSequence()) {
    for (size_t i = 0; i < node.size(); ++i) {
      PENG_LOG_INFO(PENG_LOG_ROOT())
          << std::string(level * 4, ' ') << i << " - " << node[i].Type()
          << " - " << level;
      print_yaml(node[i], level + 1);
    }
  }
}

void test_yaml() {
  YAML::Node root =
      YAML::LoadFile("/home/xupeng/valarmorgulis/xupeng/bin/conf/test.yml");
  // PENG_LOG_INFO(PENG_LOG_ROOT()) << root;
  print_yaml(root, 0);
  PENG_LOG_INFO(PENG_LOG_ROOT()) << root.Scalar();
}

void test_config() {
  // PENG_LOG_INFO(PENG_LOG_ROOT()) << g_int_value_config->GetValue();
  // PENG_LOG_INFO(PENG_LOG_ROOT()) << g_int_value_config->toString();

  // PENG_LOG_INFO(PENG_LOG_ROOT()) << g_float_value_config->GetValue();
  // PENG_LOG_INFO(PENG_LOG_ROOT()) << g_float_value_config->toString();
  PENG_LOG_INFO(PENG_LOG_ROOT())
      << "before: " << g_int_value_config->GetValue();
  PENG_LOG_INFO(PENG_LOG_ROOT())
      << "before: " << g_float_value_config->toString();

  // auto v = g_int_vec_value_config->GetValue();
  // for (auto &i : v) {
  //   PENG_LOG_INFO(PENG_LOG_ROOT()) << "before: int_vec : " << i;
  // }

  // auto l = g_int_list_value_config->GetValue();
  // for (auto &i : l) {
  //   PENG_LOG_INFO(PENG_LOG_ROOT()) << "before: int_list : " << i;
  // }
  XX(g_int_vec_value_config, int_vec, before);
  XX(g_int_list_value_config, int_list, before);
  XX(g_int_set_value_config, int_set, before);
  XX(g_int_unset_value_config, int_uset, before);
  XX_M(g_str_int_map_value_config, int_map, before);
  XX_M(g_str_int_umap_value_config, int_umap, before);

  YAML::Node root =
      YAML::LoadFile("/home/xupeng/valarmorgulis/xupeng/bin/conf/test.yml");
  PENG::Config::LoadFromYaml(root);

  PENG_LOG_INFO(PENG_LOG_ROOT()) << "after: " << g_int_value_config->GetValue();
  PENG_LOG_INFO(PENG_LOG_ROOT())
      << "after: " << g_float_value_config->toString();

  // v = g_int_vec_value_config->GetValue();
  // for (auto &i : v) {
  //   PENG_LOG_INFO(PENG_LOG_ROOT()) << "after: int_vec : " << i;
  // }

  // l = g_int_list_value_config->GetValue();
  // for (auto &i : l) {
  //   PENG_LOG_INFO(PENG_LOG_ROOT()) << "after: int_list : " << i;
  // }
  XX(g_int_vec_value_config, int_vec, after);
  XX(g_int_list_value_config, int_list, after);
  XX(g_int_set_value_config, int_set, after);
  XX(g_int_unset_value_config, int_uset, after);
  XX_M(g_str_int_map_value_config, int_map, after);
  XX_M(g_str_int_umap_value_config, int_umap, after);
}

class Person {
public:
  std::string toString() const {
    std::stringstream ss;
    ss << "[Person name=" << m_name << " age=" << m_age << " sex=" << m_sex
       << "]";
    return ss.str();
  }

  bool operator==(const Person &oth) const {
    return m_name == oth.m_name && m_age == oth.m_age && m_sex == oth.m_sex;
  }

  std::string m_name;
  int m_age = 0;
  bool m_sex = 0;
};

namespace PENG {

template <> class LexicalCast<std::string, Person> {
public:
  Person operator()(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    Person p;
    p.m_name = node["name"].as<std::string>();
    p.m_age = node["age"].as<int>();
    p.m_sex = node["sex"].as<bool>();
    return p;
  }
};

template <> class LexicalCast<Person, std::string> {
public:
  std::string operator()(const Person &p) {
    YAML::Node node;
    node["name"] = p.m_name;
    node["age"] = p.m_age;
    node["sex"] = p.m_sex;
    std::stringstream ss;
    ss << node;
    return ss.str();
  }
};

} // namespace PENG

PENG::ConfigVar<Person>::ptr g_person =
    PENG::Config::Lookup("class.person", Person(), "system person");

PENG::ConfigVar<std::map<std::string, Person>>::ptr g_person_map =
    PENG::Config::Lookup("class.map", std::map<std::string, Person>(),
                         "system person");

PENG::ConfigVar<std::map<std::string, std::vector<Person>>>::ptr
    g_person_vec_map =
        PENG::Config::Lookup("class.vec_map",
                             std::map<std::string, std::vector<Person>>(),
                             "system person");

void test_class() {
  PENG_LOG_INFO(PENG_LOG_ROOT())
      << "before: " << g_person->GetValue().toString() << " - "
      << g_person->toString();

  g_person->AddListener([](const Person &old_value, const Person &new_value) {
    PENG_LOG_INFO(PENG_LOG_ROOT()) << "old_value = " << old_value.toString()
                                   << " new_value = " << new_value.toString();
  });

  XX_PM(g_person_map, "class.map before");
  PENG_LOG_INFO(PENG_LOG_ROOT()) << "before: " << g_person_vec_map->toString();

  YAML::Node root =
      YAML::LoadFile("/home/xupeng/valarmorgulis/xupeng/bin/conf/test.yml");

  PENG::Config::LoadFromYaml(root);

  PENG_LOG_INFO(PENG_LOG_ROOT()) << "after: " << g_person->GetValue().toString()
                                 << " - " << g_person->toString();
  XX_PM(g_person_map, "class.map after");
  PENG_LOG_INFO(PENG_LOG_ROOT()) << "after: " << g_person_vec_map->toString();

  return;
}

void test_log() {
  static PENG::Logger::ptr system_log = PENG_LOG_NAME("system");
  PENG_LOG_INFO(system_log) << "hello system" << std::endl;
  std::cout << PENG::LoggerMgr::GetInstance()->toYamlString() << std::endl;
  YAML::Node root =
      YAML::LoadFile("/home/xupeng/valarmorgulis/xupeng/bin/conf/log.yml");

  PENG::Config::LoadFromYaml(root);
  std::cout << "===============" << std::endl;
  std::cout << PENG::LoggerMgr::GetInstance()->toYamlString() << std::endl;
  std::cout << "===============" << std::endl;
  std::cout << root << std::endl;
  PENG_LOG_INFO(system_log) << "hello system" << std::endl;

  system_log->setFormatter("%d %m%n");
  PENG_LOG_INFO(system_log) << "hello system" << std::endl;
}

int main(int argc, char **argv) {
  // test_yaml();
  // test_config();
  // test_class();
  test_log();
  return 0;
}

//
// Created by valerino on 10/11/2017.
//

#ifndef PROJECT_CCONFIGURATION_H
#define PROJECT_CCONFIGURATION_H

#include <json.hpp>
using nlohmann::json;

/**
 * handles configuration, json format
 */
class CConfiguration {
protected:
  /**
   * use static init() !
   * @param path path to the configuration file, will be created if not existing
   * @param def default configuration json, or NULL
   */
  CConfiguration(const char *path, const char* def = NULL);

  /**
   * use static init() !
   * @throws std::system_error
   * @throws json::exception
   */
  void read();

  /**
   * use static init() !
   * @param def a default configuration JSON string, configuration will be created if not existing
   * @throws json::exception on parse error
   */
  void read(const char *def);

public:
  /**
   * initializes the configuration
   * @param path path to the configuration file, will be created if not existing
   * @throws std::system_error
   * @throws json::exception
   * @return CConfiguration* singleton
   */
  static CConfiguration* init (const char* path, const char* def = NULL);

  /**
   * singleton, call init() once, then access configuration through instance() elsewhere!
   * @return
   */
  static CConfiguration* instance();

  /**
   * write back the configuration
   * @return 0 on success
   */
  int write();

  /**
   * check if the configuration is empty
   * @return bool
   */
  bool is_empty() { return m_j.empty(); }

  /**
   * set value into configuration
   * @tparam T a primitive or std:: type
   * @param key
   * @param value
   */
  template <class T> void set(const char *key, T value) { m_j[key] = value; }

  /**
   * get value from configuration
   * @tparam T a primitive or std:: type
   * @param key
   * @return T
   */
  template <class T> T get(const char *key) {
    T def = T();
    try {
      def = m_j[key];
    } catch (...) {
    }
    return def;
  }

  /**
   * get configuration path
   * @return
   */
  const char *path() { return m_path.c_str(); }

  ~CConfiguration();

private:
  static CConfiguration* g_instance;
  std::string m_path;
  json m_j;
};

#endif // PROJECT_CCONFIGURATION_H

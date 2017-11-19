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
public:
  /**
   * initializes configuration. call read() then.
   * @param path path to the configuration JSON file
   */
  CConfiguration(const char *path);

  /**
   * write back the configuration
   * @return 0 on success
   */
  int write();

  /**
   * read configuration
   * @throws std::system_error(ENOENT) when not found
   * @throws json::exception on parse error
   */
  void read();

  /**
   * read configuration or initialize with default if not found
   * @param def a default configuration JSON string
   * @return 0 on success
   */
  int read(const char *def);

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
  std::string m_path;
  json m_j;
};

#endif // PROJECT_CCONFIGURATION_H

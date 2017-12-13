/**
 * @file   CConfiguration.cpp
 * @Author valerino
 * @date   13/12/2017
 * @brief  implements JSON configuration serializer/deserializer, part of vuelib
 */

#include <string>
#include <fstream>
#include "CFile.h"
#include "CConfiguration.h"
#include "CDbg.h"

/**
 * static singleton
 */
CConfiguration* CConfiguration::g_instance = NULL;

CConfiguration::CConfiguration(const char *path, const char *def) {
  m_path = path;
  m_j = {};

  // read
  read(def ? def : "{}");
}

CConfiguration *CConfiguration::init(const char *path, const char *def) {
  if (g_instance) {
    return instance();
  }
  g_instance = new CConfiguration(path, def);
  return g_instance;
}

CConfiguration *CConfiguration::instance() {
  return g_instance;
}

void CConfiguration::read(const char *def) {
  try {
    read();
  } catch (std::system_error &e) {
    if (e.code().value() == ENOENT) {
      // initialize default configuration
      CDbg::warning(
          "configuration not found, writing default configuration: %s\n",
          path());

      // and create file
      if (CFile::from_buffer(path(), (uint8_t *)def, strlen(def)) != 0) {
        throw std::system_error(EACCES, std::generic_category(), std::string("can't write configuration file: ") + path());
      }

      // read back
      read();
    }
  } catch (json::exception &e) {
    throw std::system_error(EINVAL, std::generic_category(),  std::string("error parsing configuration: ") + path());
  }
}

void CConfiguration::read() {
  uint32_t size;
  const char *cfg = (const char *)CFile::to_buffer(m_path.c_str(), &size);
  if (!cfg) {
    throw std::system_error(ENOENT, std::generic_category(), std::string());
  }

  // strip comments
  //for (int i=0; i < size; i++) {
  //  if (cfg[i] == '/' and cfg[i[]])
  //}
  try {
    m_j = json::parse(cfg);
    delete cfg;
  } catch (json::exception &e) {
    delete cfg;
    throw e;
  }
}

int CConfiguration::write() {
  std::string s = m_j.dump();
  return CFile::from_buffer(m_path.c_str(), (const uint8_t *)s.c_str(),
                            s.length());
}

CConfiguration::~CConfiguration() {}

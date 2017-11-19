//
// Created by valerino on 10/11/2017.
//

#include <string>
#include <fstream>
#include "CFile.h"
#include "CConfiguration.h"
#include "CDbg.h"

CConfiguration::CConfiguration(const char *path) {
  m_path = std::string(path);
  m_j = {};
}

int CConfiguration::read(const char *def) {
  try {
    read();
    CDbg::notify("read configuration: %s\n", path());
  } catch (std::system_error &e) {
    if (e.code().value() == ENOENT) {
      // initialize default configuration
      CDbg::warning(
          "configuration not found, writing default configuration: %s\n",
          path());

      // and create file
      if (CFile::from_buffer(path(), (uint8_t *)def, strlen(def)) != 0) {
        CDbg::error("cannot write default configuration: %s\n", path());
        return EACCES;
      }

      // read back
      read();
    }
  } catch (json::exception &e) {
    CDbg::error("error parsing configuration: %s, %s\n", path(), e.what());
    return EINVAL;
  }

  return 0;
}

void CConfiguration::read() {
  const char *cfg = (const char *)CFile::to_buffer(m_path.c_str());
  if (!cfg) {
    throw std::system_error(ENOENT, std::generic_category(), std::string());
  }

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

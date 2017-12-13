/**
 * @file   IEmulator.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  defines the IEmulator interface, which all cores must implement
 */
#ifndef PROJECT_CEMULATOR_H
#define PROJECT_CEMULATOR_H

#include <string>

/**
 * interface for emulator cores
 */
class IEmulator {
public:
  /**
   * destructor
   */
  virtual ~IEmulator(){};

  /**
 * returns the emulator core name
 * @return
 */
  virtual std::string name() =0;

  /**
   * returns the emulator version
   * @return
   */
  virtual std::string version() =0;

  /**
   * returns the emulator description
   * @return
   */
  virtual std::string info() = 0;

  /**
   * starts the emulator
   * @param rom_path path to the rom image to be executed
   * @return 0 on success
   */
  virtual int start(const char *rom_path) =0;

  /**
   * stop the emulator
   */
  virtual void stop() =0;

  /**
   * pause the emulator
   */
  virtual void pause() = 0;

  /**
   * loads configuration (<core_name>.json) into the emulator
   */
  virtual void configure() =0;
};

/** @brief every emulator core must also export IEmulator* get_emulator_interface() */
typedef IEmulator *(*GET_EMULATOR_INTERFACE)();

#endif // PROJECT_CEMULATOR_H

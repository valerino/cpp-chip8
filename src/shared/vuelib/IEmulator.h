//
// Created by valerino on 08/11/2017.
//

#ifndef PROJECT_CEMULATOR_H
#define PROJECT_CEMULATOR_H

#include <string>

/**
 * interface for emulator plugins
 */
class IEmulator {
public:
  virtual ~IEmulator(){};

  /**
   *  get the emulator name
   *  @return
   */
  virtual std::string name() = 0;

  /**
   * get the emulator version
   * @return
   */
  virtual std::string version() = 0;

  /**
   * get the emulator description
   * @return
   */
  virtual std::string info() = 0;

  /**
   * get emulator additional options string
   * @return
   */
  virtual std::string options() = 0;

  /**
   * start emulator
   * @param rom_path path to a rom file
   * @return 0 on success
   */
  virtual int start(const char *rom_path) = 0;

  /**
   * stop emulator
   */
  virtual void stop() = 0;

  /**
   * pause emulator
   */
  virtual void pause() = 0;

  /**
   * show emulator-specific configuration options
   */
  virtual void configure() = 0;
};

/**
 * every emulator plugin must export IEmulator* get_emulator_interface()
 */
typedef IEmulator *(*GET_EMULATOR_INTERFACE)();

#endif // PROJECT_CEMULATOR_H

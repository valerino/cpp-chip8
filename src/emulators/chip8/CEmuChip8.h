/**
 * @file   CEmuChip8.h
 * @Author valerino
 * @date   13/12/2017
 * @brief  implements the chip8 emulator
 */
#ifndef PROJECT_CEMUCHIP8_H
#define PROJECT_CEMUCHIP8_H

/**
 * implements chip8 emulator, implements IEmulator interface
 */
class CEmuChip8 : public IEmulator {
public:
  /**
   * constructor
   */
  CEmuChip8();

  /**
   * destructor
   */
  ~CEmuChip8();

  /**
   * returns the emulator core name
   * @return
   */
  std::string name() override;

  /**
   * returns the emulator version
   * @return
   */
  std::string version() override;

  /**
   * returns the emulator description
   * @return
   */
  std::string info() override;

  /**
   * starts the emulator
   * @param rom_path path to the rom image to be executed
   * @return 0 on success
   */
  int start(const char *rom_path) override;

  /**
   * stop the emulator
   */
  void stop() override;

  /**
   * pause the emulator
   */
  void pause() override;

  /**
   * loads configuration (<core_name>.json) into the emulator
   */
  void configure() override;

private:
  bool m_running;
  CSound *m_sound;
  CDisplay *m_display;
  CInput *m_input;
  CMemory *m_memory;
  CCpu *m_cpu;
  bool m_paused;

  void sdl_poll();
};

#endif // PROJECT_CEMUCHIP8_H

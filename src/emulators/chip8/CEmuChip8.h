//
// Created by valerino on 09/11/2017.
//

#ifndef PROJECT_CEMUCHIP8_H
#define PROJECT_CEMUCHIP8_H

/**
 * implements chip8 emulator, implements IEmulator interface
 */
class CEmuChip8 : public IEmulator {
public:
  CEmuChip8();
  ~CEmuChip8();
  std::string name() override;
  std::string version() override;
  std::string info() override;
  std::string options() override;
  int start(const char *rom_path) override;
  void stop() override;
  void pause() override;
  void configure() override;

  /**
   * runs in the sound thread while the emulator is running
   */
  void soundthread_proc();

private:
  bool m_running;
  CSound *m_sound;
  CDisplay *m_display;
  CInput *m_input;
  CMemory *m_memory;
  CCpu *m_cpu;
  bool m_paused;

  /**
   * handle sdl events and control events
   * ESC=exit emulator
   * P=pause
   */
  void poll();
};

#endif // PROJECT_CEMUCHIP8_H

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

private:
  CDisplay *m_display;
  CInput *m_input;
  CMemory *m_memory;
  CCpu *m_cpu;
  CSound *m_sound;
  bool m_running;
  bool m_paused;
};

#endif // PROJECT_CEMUCHIP8_H

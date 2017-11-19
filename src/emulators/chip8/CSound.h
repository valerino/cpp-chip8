//
// Created by valerino on 13/11/2017.
//

#ifndef PROJECT_CSOUND_H
#define PROJECT_CSOUND_H

// implements the sound subsystem
class CSound {
public:
  /**
   * emit continuos buzz, call stop_buzz() to stop
   */
  void beep();

  /**
   * stop buzzing
   */
  void stop_buzz();
};

#endif // PROJECT_CSOUND_H

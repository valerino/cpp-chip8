//
// Created by valerino on 09/11/2017.
//

#ifndef PROJECT_CDISPLAY_H
#define PROJECT_CDISPLAY_H

class CDisplay {
public:
  CDisplay(int mode);
  ~CDisplay();

private:
  SDL_Surface* m_surface;
  SDL_Renderer* m_renderer;
  SDL_Window* m_window;
};

#endif //PROJECT_CDISPLAY_H

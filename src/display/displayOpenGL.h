#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "display/display.h"

class Memory;

class DisplayOpenGL : public Display
{
public:
  explicit DisplayOpenGL(Memory &memory);
  ~DisplayOpenGL();

  void toggleFullScreen() override;

  void saveScreenshot() const override;

  void render() override;

private:
  Memory &memory_;

  SDL_Window *window_;
  SDL_GLContext context_;

  // OpenGL
  int shader_program_;
  unsigned int quadVAO_, quadVBO_;

  const int width_{512};
  const int height_{256};

  bool isFullScreen_{false};
};
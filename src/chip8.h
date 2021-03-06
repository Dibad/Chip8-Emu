#pragma once

#include <memory>
#include <string>

#include "commander/commander.h"
#include "cpu/cpu.h"
#include "memory/memory.h"

class AbstractFactory;
class Display;
class Input;
class Sound;

// INLINE

class Chip8 {
public:
  explicit Chip8(std::unique_ptr<AbstractFactory> &factory,
                 const std::string &rom_path = "");
  ~Chip8();

  void execute();

private:
  bool quit_{false};

  Memory memory_;
  Cpu cpu_{memory_};
  std::unique_ptr<Display> display_;
  std::unique_ptr<Input> input_;
  std::unique_ptr<Sound> sound_;

  friend class Commander;
  Commander commander_{*this};
};

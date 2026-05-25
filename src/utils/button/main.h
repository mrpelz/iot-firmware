#pragma once

#ifdef IOT_NODE_BUTTONS

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <functional>
#include <vector>

#include "../../utils/log.h"

namespace IotNode::Utils::Button
{
  struct Update
  {
    bool down;
    bool downChanged;
    unsigned long prevDuration;
    unsigned char repeat;
    unsigned char longpress;
    ::std::vector<bool> pressedMap;
  };

  typedef ::std::function<void(Update update)> ChangeCallback;

  struct Config
  {
    unsigned char pin;
    bool invert;
    bool pullupEnable;
    unsigned long debounceTime;
    unsigned long repeatTime;
    unsigned long longpressTime;
    unsigned long noiseGateTime;
  };

  struct State
  {
    bool running = false;
    bool down;
    unsigned long changeTime;
    unsigned char repeat;
    unsigned char longpress;
    unsigned long longpressTime;
    unsigned long noiseGateTime;
    ChangeCallback changeCallback = [](Update update) {};
  };

  class Class
  {
  private:
    Config config;
    State state;

  public:
    Class(Config _config);
    void setChangeCallback(ChangeCallback callback);
    bool isDown();
    void start();
    void stop();
    void update();
    void update(bool insert);
  };
}

#endif

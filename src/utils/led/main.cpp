#include "./main.h"

#ifdef IOT_NODE_LED

namespace IotNode {
namespace Utils {

namespace Led {
  Class::Class(Config _config) {
    config = _config;
  }

  void Class::commit() {
    if (!state.wasInitialized) return;

    Log::debug("led", "commit");

    if (state.duty == ESP8266_PWM_MIN) {
      digitalWrite(config.pin, config.invert);

      return;
    }

    if (state.duty == ESP8266_PWM_MAX) {
      digitalWrite(config.pin, !config.invert);

      return;
    }

    analogWrite(
      config.pin,
      (config.invert ? ESP8266_PWM_MAX - state.duty : state.duty)
    );
  }

  void Class::init() {
    Log::debug("led", "init");
    Log::debug("led.pin", String(config.pin));

    pinMode(config.pin, OUTPUT);
    state.wasInitialized = true;

    commit();
  }

  bool Class::isOn() {
    return state.duty > ESP8266_PWM_MIN;
  }

  void Class::setDutyCycle(uint16_t duty) {
    Log::debug("led.set-duty", String(duty));

    if (duty > ESP8266_PWM_MAX) {
      state.duty = ESP8266_PWM_MAX;
      return;
    }

    state.duty = duty;
    commit();
  }

  void Class::setOn(bool on) {
    Log::debug("led.set-on", on ? "on" : "off");
    Log::debug("led.pin", String(config.pin));

    state.duty = ESP8266_PWM_MAX;
    commit();
  }

  void Class::toggle() {
    auto on = state.duty == ESP8266_PWM_MIN;

    Log::debug("led.toggle", on ? "on2off" : "off2on");
    Log::debug("led.pin", String(config.pin));

    state.duty = on ? ESP8266_PWM_MAX : ESP8266_PWM_MIN;
    commit();
  }
}

} // section namespace
} // project namespace

#endif

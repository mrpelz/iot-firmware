#include "./main.h"

#ifdef IOT_NODE_HMMD_MOTION

namespace IotNode::Events::HmmdMotion
{
  auto sensor = new Sensor();

  unsigned long lastReading = 0;
  unsigned short lastDistance = 0;
  auto isDetected = false;

  void initializer()
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("hmmd-motion-event: initializing sensor");
#endif

    vTaskDelay(pdMS_TO_TICKS(100));
    IOT_NODE_HMMD_MOTION_SERIAL.begin(115200, SERIAL_8N1, 6, 5);

    bool isRadarEnabled = false;
    for (int i = 0; i < 3; i++)
    {
      if (sensor->begin(IOT_NODE_HMMD_MOTION_SERIAL))
      {
        isRadarEnabled = true;
        break;
      }

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("hmmd-motion-event.init: retrying connection");
      vTaskDelay(pdMS_TO_TICKS(100));
#endif
    }

    sensor->setRadarConfigurationTargetDisappearanceDelay(1);

#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("hmmd-motion-event.init-result", isRadarEnabled ? "Ok" : "Failed");

    if (isRadarEnabled)
    {
      if (sensor->readFirmwareVersion())
      {
        Utils::Log::debug("hmmd-motion-event.firmware-version", sensor->firmwareVersion.c_str());
      }
      if (sensor->readSerialNumber())
      {
        Utils::Log::debug("hmmd-motion-event.serial-number", sensor->serialNumber.c_str());
      }

      auto config = sensor->radarConfiguration;
      Utils::Log::debug(fmt::format("hmmd-motion-event.config.gates-min: {}", config.detectionGatesMin));
      Utils::Log::debug(fmt::format("hmmd-motion-event.config.gates-max: {}", config.detectionGatesMax));
      Utils::Log::debug(fmt::format("hmmd-motion-event.config.disappearance-delay: {}", config.targetDisappearanceDelay));
    }
#endif
  }

  void event(bool isTargetDetected, short distance = 0)
  {
    ::std::vector<unsigned char> payload = {
        (unsigned char)(isTargetDetected ? 0x01 : 0x00),
    };

    auto distance_ = ::std::bit_cast<::std::array<unsigned char, sizeof distance>>(distance);
    payload.insert(
        payload.end(),
        ::std::begin(distance_),
        ::std::end(distance_));

    Utils::UDP::instance.event(ids::hmmdMotion, 0, payload);
  }

  void checkForTargets()
  {
    if (sensor->read())
    {
      auto newIsDetected = sensor->isTargetDetected;
      auto newDistance = sensor->distanceToTarget;

      if (isDetected && !newIsDetected)
      {
        event(false);

#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("hmmd-motion-event: target-lost");
#endif
      }
      else if (!isDetected && newIsDetected)
      {
        event(true, newDistance);

#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("hmmd-motion-event: target-found");
        Utils::Log::debug(fmt::format("hmmd-motion-event.distance: {}", newDistance));
#endif
      }

      isDetected = newIsDetected;

      if (isDetected && (lastDistance != newDistance))
      {
        event(true, newDistance);

#ifdef IOT_NODE_LOGGING
        Utils::Log::debug(fmt::format("hmmd-motion-event.distance: {}", newDistance));
#endif
      }

      lastDistance = newDistance;
    }

    static unsigned long nextWarning = 0;
    if (!sensor->isActive() && millis() > nextWarning)
    {
      // throttle warnings to at most once every 5 seconds
      nextWarning = millis() + 5000;

      event(false);
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("hmmd-motion-event.sensor-error: not-sending-data");
#endif
    }
  }
}

#endif

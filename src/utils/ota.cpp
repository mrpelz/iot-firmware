#include "./ota.h"

namespace IotNode
{
  namespace Utils
  {

    namespace OTA
    {
      void update()
      {
        ArduinoOTA.handle();
      }

      void setup()
      {
        ArduinoOTA.setHostname(IOT_NODE_NAME);
        ArduinoOTA.setPassword(IOT_NODE_OTA_PASSWORD);

        ArduinoOTA.onStart([]()
                           {
#ifdef IOT_NODE_LOGGING
                             Log::debug("ota", "start");
#endif
                           });

        ArduinoOTA.onEnd([]()
                         {
#ifdef IOT_NODE_LOGGING
                           Log::debug("ota", "end");
#endif
                         });

        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                              {
#ifdef IOT_NODE_LOGGING
                                Log::debug("ota.progress", String(progress / (total / 100)));
#endif
                              });

        ArduinoOTA.onError([](ota_error_t error)
                           {
#ifdef IOT_NODE_LOGGING
                             if (error == OTA_AUTH_ERROR)
                               Log::debug("ota.error", "auth");
                             else if (error == OTA_BEGIN_ERROR)
                               Log::debug("ota.error", "begin");
                             else if (error == OTA_CONNECT_ERROR)
                               Log::debug("ota.error", "connect");
                             else if (error == OTA_RECEIVE_ERROR)
                               Log::debug("ota.error", "receive");
                             else if (error == OTA_END_ERROR)
                               Log::debug("ota.error", "end");
#endif
                           });
      }
    }

  } // section namespace
} // project namespace

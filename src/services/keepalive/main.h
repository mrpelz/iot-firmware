#pragma once

#include <Arduino.h>

#include <pre.h>
#include <post.h>

#include "../../utils/keepalive/setup.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./setup.h"

namespace IotNode::Services::Keepalive
{
  Utils::UDP::RequestHandler makeHandler();
}

#pragma once

namespace IotNode
{
  namespace Events
  {

    enum ids
    {
      button = 0,   // button0 (start id)
      input = 0xa0, // input0 (start id)
      rf433 = 0xfc,
      vcc = 0xfd,
      espNowGw = 0xfe
    };

  } // section namespace
} // project namespace

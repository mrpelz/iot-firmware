#include "./main.h"

#ifdef IOT_NODE_OUTPUT_NG

namespace IotNode::Services::OutputNg
{
  Utils::UDP::Service makeService(Utils::OutputNg::Buzzer *instance, Utils::UDP::Class *udp, unsigned char index)
  {
    auto handler = [instance, index, udp](
                       Utils::UDP::Payload *request,
                       Utils::UDP::RespondCallback respond,
                       Utils::UDP::Peer peer)
    {
      Utils::OutputNg::Request<Utils::OutputNg::DimmableValue<unsigned long>> object;

      memcpy(&object.iterations, request->data(), sizeof(object.iterations));

      for (size_t i = 0; i < (request->size() - sizeof(object.iterations)) / BUZZER_ITEM_SIZE; i++)
      {
        Utils::OutputNg::SequenceItem<Utils::OutputNg::DimmableValue<unsigned long>> item;

        auto itemBegin = request->data() + sizeof(object.iterations) + (BUZZER_ITEM_SIZE * i);

        memcpy(&item.holdTime, itemBegin, sizeof(item.holdTime));
        memcpy(&item.value.rampTime, itemBegin + sizeof(item.holdTime), sizeof(item.value.rampTime));
        memcpy(&item.value.value, itemBegin + sizeof(item.holdTime) + sizeof(item.value.rampTime), sizeof(item.value.value));

        object.sequence.push_back(item);
      }

      if (false)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("indicator-service: error");

        return;
#endif
      }

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug(fmt::format(
          "output-ng-service: dezerialized:\n\
  iterations:{},\n\
  sequenceItems:{}\n\
  item0.holdTime:{}\n\
  item0.value.rampTime:{}\n\
  item0.value.value:{}",
          object.iterations,
          object.sequence.size(),
          object.sequence.size() ? object.sequence.at(0).holdTime : 0,
          object.sequence.size() ? object.sequence.at(0).value.rampTime : 0,
          object.sequence.size() ? object.sequence.at(0).value.value : 0));
#endif

      instance->setSequence(
          object,
          [index, udp](unsigned long remainingIterations)
          {
            ::std::vector<unsigned char> payload = {
                (unsigned char)true, // still iterating
            };

            auto remainingIterations_ = reinterpret_cast<unsigned char *>(&remainingIterations);
            payload.insert(
                payload.end(),
                remainingIterations_,
                remainingIterations_ + sizeof(remainingIterations));

            udp->event(Events::ids::outputNgBuzzerProgress, index, payload); }, [index, udp]()
          { udp->event(Events::ids::outputNgBuzzerProgress, index, {
                                                                       (unsigned char)false, // ended (reset)
                                                                   }); });

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("indicator-service: sending response");
#endif

      respond({});
    };

    return {
        ids::outputNgBuzzer,
        index,
        handler};
  }

  Utils::UDP::Service makeService(Utils::OutputNg::Binary *instance, Utils::UDP::Class *udp, unsigned char index)
  {
    auto handler = [instance, index, udp](
                       Utils::UDP::Payload *request,
                       Utils::UDP::RespondCallback respond,
                       Utils::UDP::Peer peer)
    {
      Utils::OutputNg::Request<bool> object;

      memcpy(&object.iterations, request->data(), sizeof(object.iterations));

      for (size_t i = 0; i < (request->size() - sizeof(object.iterations)) / BINARY_ITEM_SIZE; i++)
      {
        Utils::OutputNg::SequenceItem<bool> item;

        auto itemBegin = request->data() + sizeof(object.iterations) + (BINARY_ITEM_SIZE * i);

        memcpy(&item.holdTime, itemBegin, sizeof(item.holdTime));
        memcpy(&item.value, itemBegin + sizeof(item.holdTime), sizeof(item.value));

        object.sequence.push_back(item);
      }

      if (false)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("indicator-service: error");

        return;
#endif
      }

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug(fmt::format(
          "output-ng-service: dezerialized:\n\
  iterations:{},\n\
  sequenceItems:{}\n\
  item0.holdTime:{}\n\
  item0.value:{}",
          object.iterations,
          object.sequence.size(),
          object.sequence.size() ? object.sequence.at(0).holdTime : 0,
          object.sequence.size() && object.sequence.at(0).value ? "true" : "false"));
#endif

      instance->setSequence(
          object,
          [index, udp](unsigned long remainingIterations)
          {
            ::std::vector<unsigned char> payload = {
                (unsigned char)true, // still iterating
            };

            auto remainingIterations_ = reinterpret_cast<unsigned char *>(&remainingIterations);
            payload.insert(
                payload.end(),
                remainingIterations_,
                remainingIterations_ + sizeof(remainingIterations));

            udp->event(Events::ids::outputNgBinaryProgress, index, payload); }, [index, udp]()
          { udp->event(Events::ids::outputNgBinaryProgress, index, {
                                                                       (unsigned char)false, // ended (reset)
                                                                   }); });

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("indicator-service: sending response");
#endif

      respond({});
    };

    return {
        ids::outputNgBinary,
        index,
        handler};
  }

  Utils::UDP::Service makeService(Utils::OutputNg::Dimmable *instance, Utils::UDP::Class *udp, unsigned char index)
  {
    auto handler = [instance, index, udp](
                       Utils::UDP::Payload *request,
                       Utils::UDP::RespondCallback respond,
                       Utils::UDP::Peer peer)
    {
      Utils::OutputNg::Request<Utils::OutputNg::DimmableValue<double>> object;

      memcpy(&object.iterations, request->data(), sizeof(object.iterations));

      for (size_t i = 0; i < (request->size() - sizeof(object.iterations)) / DIMMABLE_ITEM_SIZE; i++)
      {
        Utils::OutputNg::SequenceItem<Utils::OutputNg::DimmableValue<double>> item;

        auto itemBegin = request->data() + sizeof(object.iterations) + (DIMMABLE_ITEM_SIZE * i);

        memcpy(&item.holdTime, itemBegin, sizeof(item.holdTime));
        memcpy(&item.value.rampTime, itemBegin + sizeof(item.holdTime), sizeof(item.value.rampTime));
        memcpy(&item.value.value, itemBegin + sizeof(item.holdTime) + sizeof(item.value.rampTime), sizeof(item.value.value));

        object.sequence.push_back(item);
      }

      if (false)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("indicator-service: error");

        return;
#endif
      }

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug(fmt::format(
          "output-ng-service: dezerialized:\n\
iterations:{},\n\
sequenceItems:{}\n\
item0.holdTime:{}\n\
item0.value.rampTime:{}\n\
item0.value.value:{:.2f}",
          object.iterations,
          object.sequence.size(),
          object.sequence.size() ? object.sequence.at(0).holdTime : 0,
          object.sequence.size() ? object.sequence.at(0).value.rampTime : 0,
          object.sequence.size() ? object.sequence.at(0).value.value : 0));
#endif

      instance->setSequence(
          object,
          [index, udp](unsigned long remainingIterations)
          {
            ::std::vector<unsigned char> payload = {
                (unsigned char)true, // still iterating
            };

            auto remainingIterations_ = reinterpret_cast<unsigned char *>(&remainingIterations);
            payload.insert(
                payload.end(),
                remainingIterations_,
                remainingIterations_ + sizeof(remainingIterations));

            udp->event(Events::ids::outputNgDimmableProgress, index, payload); }, [index, udp]()
          { udp->event(Events::ids::outputNgDimmableProgress, index, {
                                                                         (unsigned char)false, // ended (reset)
                                                                     }); });

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("indicator-service: sending response");
#endif

      respond({});
    };

    return {
        ids::outputNgDimmable,
        index,
        handler};
  }

  Utils::UDP::Service makeService(Utils::OutputNg::DimmableRGB *instance, Utils::UDP::Class *udp, unsigned char index)
  {
    auto handler = [instance, index, udp](
                       Utils::UDP::Payload *request,
                       Utils::UDP::RespondCallback respond,
                       Utils::UDP::Peer peer)
    {
      Utils::OutputNg::Request<Utils::OutputNg::DimmableRGBValue> object;

      memcpy(&object.iterations, request->data(), sizeof(object.iterations));

      for (size_t i = 0; i < (request->size() - sizeof(object.iterations)) / DIMMABLERGB_ITEM_SIZE; i++)
      {
        Utils::OutputNg::SequenceItem<Utils::OutputNg::DimmableRGBValue> item;

        auto itemBegin = request->data() + sizeof(object.iterations) + (DIMMABLERGB_ITEM_SIZE * i);

        memcpy(&item.holdTime, itemBegin, sizeof(item.holdTime));
        memcpy(&item.value.rampTime, itemBegin + sizeof(item.holdTime), sizeof(item.value.rampTime));
        memcpy(&item.value.r, itemBegin + sizeof(item.holdTime) + sizeof(item.value.rampTime), sizeof(item.value.r));
        memcpy(&item.value.g, itemBegin + sizeof(item.holdTime) + sizeof(item.value.rampTime) + sizeof(item.value.r), sizeof(item.value.g));
        memcpy(&item.value.b, itemBegin + sizeof(item.holdTime) + sizeof(item.value.rampTime) + sizeof(item.value.r) + sizeof(item.value.g), sizeof(item.value.b));

        object.sequence.push_back(item);
      }

      if (false)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("indicator-service: error");

        return;
#endif
      }

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug(fmt::format(
          "output-ng-service: dezerialized:\n\
iterations:{},\n\
sequenceItems:{}\n\
item0.holdTime:{}\n\
item0.value.rampTime:{}\n\
item0.value.r:{:.2f}\n\
item0.value.g:{:.2f}\n\
item0.value.b:{:.2f}\n\
item1.holdTime:{}\n\
item1.value.rampTime:{}\n\
item1.value.r:{:.2f}\n\
item1.value.g:{:.2f}\n\
item1.value.b:{:.2f}",
          object.iterations,
          object.sequence.size(),
          object.sequence.size() ? object.sequence.at(0).holdTime : 0,
          object.sequence.size() ? object.sequence.at(0).value.rampTime : 0,
          object.sequence.size() ? object.sequence.at(0).value.r : 0,
          object.sequence.size() ? object.sequence.at(0).value.g : 0,
          object.sequence.size() ? object.sequence.at(0).value.b : 0,
          object.sequence.size() > 1 ? object.sequence.at(1).holdTime : 0,
          object.sequence.size() > 1 ? object.sequence.at(1).value.rampTime : 0,
          object.sequence.size() > 1 ? object.sequence.at(1).value.r : 0,
          object.sequence.size() > 1 ? object.sequence.at(1).value.g : 0,
          object.sequence.size() > 1 ? object.sequence.at(1).value.b : 0));
#endif

      instance->setSequence(
          object,
          [index, udp](unsigned long remainingIterations)
          {
            ::std::vector<unsigned char> payload = {
                (unsigned char)true, // still iterating
            };

            auto remainingIterations_ = reinterpret_cast<unsigned char *>(&remainingIterations);
            payload.insert(
                payload.end(),
                remainingIterations_,
                remainingIterations_ + sizeof(remainingIterations));

            udp->event(Events::ids::outputNgDimmableRGBProgress, index, payload); }, [index, udp]()
          { udp->event(Events::ids::outputNgDimmableRGBProgress, index, {
                                                                            (unsigned char)false, // ended (reset)
                                                                        }); });

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("indicator-service: sending response");
#endif

      respond({});
    };

    return {
        ids::outputNgDimmableRGB,
        index,
        handler};
  }
}

#endif

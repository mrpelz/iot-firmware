#pragma once

#ifdef IOT_NODE_EQ3

#include <aes/esp_aes.h>
#include <Arduino.h>
#include <assert.h>
#include <byteswap.h>
#include <cmath>
#include <sstream>
#include <string>

namespace IotNode::Lib::EQ3
{
  ::std::string string_to_hex(const ::std::string &input);
  ::std::string hexstring_to_string(const ::std::string &hex_chars);

  // input should be padded array
  ::std::string encrypt_aes_ecb(::std::string &data, ::std::string &key);
  ::std::string xor_array(::std::string data, ::std::string xor_array, int offset);
  ::std::string compute_nonce(char msg_type_id, ::std::string session_open_nonce, uint16_t security_counter);
  int generic_ceil(int value, int step, int minimum);
  ::std::string compute_auth_value(::std::string data, char msg_type_id, ::std::string session_open_nonce, uint16_t security_counter, ::std::string key);
  ::std::string crypt_data(::std::string data, char msg_type_id, ::std::string session_open_nonce, uint16_t security_counter, ::std::string key);
}

#endif

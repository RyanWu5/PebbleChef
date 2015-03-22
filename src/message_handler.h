#pragma once

void send_value(const uint32_t key, const uint8_t value);
void register_message_handlers();
  
// Struct for reading step information.
typedef struct step_info_t {
  uint8_t sequence;
  char* title;
  char* description;
  char* duration;
} step_info_t;

extern step_info_t step_info;
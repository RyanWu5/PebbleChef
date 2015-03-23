/* Copyright by Ryan Wu 2015 */

#pragma once

void send_value(const uint32_t key, const uint8_t value);
void send_2_values(const uint32_t key1, const uint8_t value1, const uint32_t key2, const time_t value2);
void register_message_handlers();
  
// Struct for reading step information.
typedef struct step_info_t {
  uint8_t sequence;
  char* title;
  char* description;
  char* duration;
} step_info_t;

// Key values for AppMessage Dictionary
enum key_t {
	SEQUENCE_KEY,
	TITLE_KEY,
	DESCRIPTION_KEY,
	DURATION_KEY,
	MORE_KEY,
  REQUEST_STEP_KEY,
  REQUEST_PIN_KEY
};

extern step_info_t step_info;
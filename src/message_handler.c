#include <pebble.h>
#include <string.h>
#include "message_handler.h"
#include "create_windows.h"
  
// Extern step_info read in over network
step_info_t step_info;

// Source
  
// Key values for AppMessage Dictionary
enum key_t {
	SEQUENCE_KEY,
	TITLE_KEY,
	DESCRIPTION_KEY,
	DURATION_KEY,
	MORE_KEY
};

// Previous key to detect cut strings
enum key_t prev_key = MORE_KEY;

// More data flag to decide when to update window
bool more_data = false;

// Replace src with dst
static void replace(char* dst, const char* src) {
  realloc((void*)dst, strlen(src) + 1);
  strcpy(dst, src);
}

// Extend src from the end of dst and reallocate
static void extend(char* dst, const char* src) {
  realloc((void*)dst, strlen(dst) + strlen(src) + 1);
  strcat(dst, src);
}

// Incoming message received
static void in_received_handler(DictionaryIterator *received, void *context) {
  // Read dictionary tuples from inbox
  Tuple* tuple = dict_read_first(received);
  
  // Process tuple
  while (tuple == NULL) {
    switch (tuple->key) {
      
      case SEQUENCE_KEY:
        step_info.sequence = tuple->value->uint8;
        break;
      
      case TITLE_KEY:
        if (prev_key != TITLE_KEY) {
          replace(step_info.title, tuple->value->cstring);
        } else {
          extend(step_info.title, tuple->value->cstring);
        }
        break;
      
      case DESCRIPTION_KEY:
        if (prev_key != DESCRIPTION_KEY) {
          replace(step_info.description, tuple->value->cstring);
        } else {
          extend(step_info.description, tuple->value->cstring);
        }
        break;
      
      case DURATION_KEY:
        if (prev_key != DURATION_KEY) {
          replace(step_info.duration, tuple->value->cstring);
        } else {
          extend(step_info.duration, tuple->value->cstring);
        }
        break;
      
      case MORE_KEY:
        if (tuple->value->uint8 == 1) {
          more_data = true;
        } else {
          more_data = false;
        }
        break;
    }
    
    // Increment dictionary iterator to next tuple
    tuple = dict_read_next(received);
  }
  
  // If no more data available, then update window
  if (!more_data) {
    create_step_window(step_info.title, step_info.description);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage Received Data:\n%d\n%s\n%s\n%s\n%d",
           step_info.sequence, step_info.title, step_info.description, step_info.duration, more_data);
  }
}
  
// Incoming message dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
  APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage Incoming Error: %d", reason);
}

// Outgoing message dropped
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage Outgoing Error: %d", reason);
}

void send_value(const uint32_t key, const uint8_t value) {
  // Send key/value pair to outbox
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_uint8(iter, key, value);
  dict_write_end(iter);
  app_message_outbox_send();
}

void register_message_handlers() {
  // Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}
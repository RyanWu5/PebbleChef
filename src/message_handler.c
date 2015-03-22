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

// More data flag to decide when to update window
bool more_data = false;

static char *strdup(const char *str) {
  size_t len = strlen(str);
  char *ret = calloc(1, len);
  memcpy(ret, str, len);
  return ret;
}

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
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s():%d:\n", __func__, __LINE__);
  // Process tuple
  while (tuple != NULL) {
    switch (tuple->key) {
      case SEQUENCE_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "%s():%d:\n", __func__, __LINE__);
        step_info.sequence = tuple->value->uint8;
        if (step_info.title)
            free(step_info.title);
        step_info.title = NULL;
        if (step_info.duration)
            free(step_info.duration);
        step_info.duration = NULL;
        if (step_info.description)
            free(step_info.description);
        step_info.description = NULL;
        break;
      
      case TITLE_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "%s():%d:\n", __func__, __LINE__);
        if (!step_info.title)
          step_info.title = strdup(tuple->value->cstring);
        else
          extend(step_info.title, tuple->value->cstring);
        break;
      
      case DESCRIPTION_KEY:
      APP_LOG(APP_LOG_LEVEL_DEBUG, "%s():%d:\n", __func__, __LINE__);
        if (!step_info.description)
          step_info.description = strdup(tuple->value->cstring);
        else
          extend(step_info.description, tuple->value->cstring);
      break;

      case DURATION_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "%s():%d:\n", __func__, __LINE__);
        if (!step_info.duration)
          step_info.duration = strdup(tuple->value->cstring);
        else
          extend(step_info.duration, tuple->value->cstring);
        break;
      
      case MORE_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "%s():%d:\n", __func__, __LINE__);
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
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%s():%d:\n", __func__, __LINE__);
    Window *step_window = create_step_window(step_info.title, step_info.description);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage Received Data:\n%d\n%s\n%s\n%s\n%d",
           step_info.sequence, step_info.title, step_info.description, step_info.duration, more_data);
    window_stack_push(step_window, true);
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

  // Zero-out step_info structure
  memset(&step_info, 0, sizeof(step_info));
}
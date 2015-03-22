#include <pebble.h>
#include "create_windows.h"

//APP_LOG(APP_LOG_LEVEL_DEBUG, "%s():%d", __func__, __LINE__);  

Window* splash_window = NULL;
Window* top_window = NULL;

void handle_init(void) {
  // Create splash_window
  splash_window = create_splash_window();
  
  // Push splash_window to stack
  window_stack_push(splash_window, true);
}

void handle_deinit(void) {
  // Delete splash window
  window_destroy(splash_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}

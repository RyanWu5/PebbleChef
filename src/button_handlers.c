#include <pebble.h>
#include "button_handlers.h"
#include "create_windows.h"
#include "message_handler.h"
  
// Source: http://developer.getpebble.com/guides/pebble-apps/app-structure/clicks/

/* * * * * * * * * * * * * 
 * Splash Window Handler *
 * * * * * * * * * * * * */

static void up_click_splash_handler(ClickRecognizerRef recognizer, void *context) {
  // Go back one page
  
}

static void select_click_splash_handler(ClickRecognizerRef recognizer, void *context) {
  // Start/stop timer
}

static void down_click_splash_handler(ClickRecognizerRef recognizer, void *context) {
  // Go forward one page
  //send_value(0, );
}

void splash_click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_splash_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_splash_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_splash_handler);
}
  
/* * * * * * * * * * * *
 * Top Window Handler  *
 * * * * * * * * * * * */
  
static void up_click_top_handler(ClickRecognizerRef recognizer, void *context) {
  // Go back one page
  
}

static void select_click_top_handler(ClickRecognizerRef recognizer, void *context) {
  // Start/stop timer
}

static void down_click_top_handler(ClickRecognizerRef recognizer, void *context) {
  // Go forward one page
}

void top_click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_top_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_top_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_top_handler);
}

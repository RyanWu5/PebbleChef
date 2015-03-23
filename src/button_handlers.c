/* Copyright by Ryan Wu 2015 */

#include <pebble.h>
#include "button_handlers.h"
#include "create_windows.h"
#include "message_handler.h"
  
// Source: http://developer.getpebble.com/guides/pebble-apps/app-structure/clicks/
  
/* * * * * * * * * * * * * 
 * Splash Window Handler *
 * * * * * * * * * * * * */

static void up_click_splash_handler(ClickRecognizerRef recognizer, void *context) {
  // Do nothing
}

static void select_click_splash_handler(ClickRecognizerRef recognizer, void *context) {
  // Reopen page
  if (step_info.sequence > 0) {
    send_value(REQUEST_STEP_KEY, step_info.sequence);
    vibes_short_pulse();
  }
}

static void down_click_splash_handler(ClickRecognizerRef recognizer, void *context) {
  // Do nothing
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
  if (step_info.sequence > 1) {
    send_value(REQUEST_STEP_KEY, step_info.sequence - 1);
    vibes_short_pulse();
  }
}

static void select_click_top_handler(ClickRecognizerRef recognizer, void *context) {
  // Start/stop timer
  if (step_info.sequence > 0) {
    start_timer(atoi(step_info.duration));
    vibes_short_pulse();
  }
}

static void down_click_top_handler(ClickRecognizerRef recognizer, void *context) {
  // Go forward one page
  if (step_info.sequence > 0) {
    send_value(REQUEST_STEP_KEY, step_info.sequence + 1);
    vibes_short_pulse();
  }
}

void top_click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_top_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_top_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_top_handler);
}

/* * * * * * * * * *
 * Motion Handler  *
 * * * * * * * * * */

static void tap_handler(AccelAxisType axis, int32_t direction) {
  // Go forward one page
  if (step_info.sequence > 0) {
    send_value(REQUEST_STEP_KEY, step_info.sequence + 1);
    vibes_short_pulse();
  }
}

void register_tap_handler() {
  accel_tap_service_subscribe(tap_handler);
}
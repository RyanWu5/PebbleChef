#include <pebble.h>
#include "create_windows.h"
#include "button_handlers.h"
#include "message_handler.h"

#include <string.h>
  
#define TITLE_FONT FONT_KEY_BITHAM_42_LIGHT
#define DESCRIPTION_FONT FONT_KEY_GOTHIC_24
#define TIMER_FONT FONT_KEY_GOTHIC_18_BOLD

/* * * * * * * * *
 * Splash Screen *
 * * * * * * * * */
  
typedef struct splash_window_data {
  GBitmap* splash_bitmap;
  BitmapLayer* splash_layer;
} splash_window_data;

static void splash_window_load(Window* window) {

  // Create memory for pointers to children
  splash_window_data* user_data = (splash_window_data*) malloc(sizeof(splash_window_data));

  // Create bitmap and layer and attach to splash_window
  user_data->splash_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SPLASH);
  user_data->splash_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(user_data->splash_layer, user_data->splash_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(user_data->splash_layer));
  
  // Set user data to pointers to children
  window_set_user_data(window, (void*)user_data);
}

static void splash_window_unload(Window* window) {
  // Get pointers to children
  splash_window_data* user_data = (splash_window_data*) window_get_user_data(window);
  
  // Destroy children
  gbitmap_destroy(user_data->splash_bitmap);
  bitmap_layer_destroy(user_data->splash_layer);
  
  // Free user data
  free(user_data);
}

Window* create_splash_window() {
  // Create splash_window
  Window* window = window_create();
  
  // Set button handlers
  window_set_click_config_provider(window, splash_click_config_provider);
  
  // Set message handlers
  register_message_handlers();
  
  // Set splash_window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = splash_window_load,
    .unload = splash_window_unload,
  });
  
  // Return splash_window
  return window;
}
  
/* * * * *
 * Timer *
 * * * * */

char buffer[16];

time_t timer_end;
TextLayer *timer_layer;
bool timer_running = false;

static void timer_handler(struct tm *tick_time, TimeUnits units_changed) {
  // Calculate elapsed time
  time_t remaining_time = timer_end - time(NULL);
  
  // Check if timer has remaining time
  if (remaining_time > 0) {
    // Convert to localtime
    struct tm* local_time = localtime(&remaining_time);
    
    // Format timer as string
    if (remaining_time >= 3600) {
      strftime(buffer, sizeof(buffer), "%Hh:%Mm", local_time);
    } else {
      strftime(buffer, sizeof(buffer), "%Mm:%Ss", local_time);
    }
  } else {
    // No time remaining
    strncpy(buffer, "00m:00s", sizeof(buffer));
    timer_running = false;
    vibes_long_pulse();
    tick_timer_service_unsubscribe();
  }
  
  // Print to timer layer
  text_layer_set_text(timer_layer, buffer);
  
}

void start_timer(time_t duration) {
  // Check if timer is already running
  if (timer_running) {
    return;
  } else {
    timer_running = true;
  }
  
  // Set end time
  timer_end = time(NULL) + duration;
  
  // Send end time
  send_2_values(REQUEST_PIN_KEY, step_info.sequence, REQUEST_PIN_KEY, timer_end);
  
  // Register time callback function
  tick_timer_service_subscribe(SECOND_UNIT, timer_handler);
}

/* * * * * * * * *
 * Timer Window  *
 * * * * * * * * */

typedef struct timer_window_data {
  char* title;
  char* description;
  int duration;
  TextLayer* title_layer;
  TextLayer* description_layer;
} timer_window_data;
  
static void timer_window_load(Window* window) {
  // Get pointers to children
  timer_window_data* user_data = (timer_window_data*) window_get_user_data(window);
  
  // Create title_layer
  user_data->title_layer = text_layer_create(GRect(5, 0, 134, 45));
  text_layer_set_background_color(user_data->title_layer, GColorClear);
  text_layer_set_text_color(user_data->title_layer, GColorBlack);
  text_layer_set_font(user_data->title_layer, fonts_get_system_font(TITLE_FONT));  
  text_layer_set_text(user_data->title_layer, user_data->title);
  
  // Create description layer
  user_data->description_layer = text_layer_create(GRect(5, 45, 134, 100));
  text_layer_set_background_color(user_data->description_layer, GColorClear);
  text_layer_set_text_color(user_data->description_layer, GColorBlack);
  text_layer_set_font(user_data->description_layer, fonts_get_system_font(DESCRIPTION_FONT));
  text_layer_set_overflow_mode(user_data->description_layer, GTextOverflowModeWordWrap);
  text_layer_set_text(user_data->description_layer, user_data->description);
  
  // Create timer layer
  timer_layer = text_layer_create(GRect(5, 124, 134, 24));
  text_layer_set_background_color(timer_layer, GColorBlack);
  text_layer_set_text_color(timer_layer, GColorWhite);
  text_layer_set_font(timer_layer, fonts_get_system_font(TIMER_FONT));
  text_layer_set_text_alignment(timer_layer, GTextAlignmentCenter);
  
  // Set timer text
  time_t duration = user_data->duration;
  struct tm* local_time = localtime(&duration);  
  if (duration >= 3600) {
    strftime(buffer, sizeof(buffer), "%Hh:%Mm", local_time);
  } else {
    strftime(buffer, sizeof(buffer), "%Mm:%Ss", local_time);
  }
  text_layer_set_text(timer_layer, buffer);
  
  // Add layers to window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(user_data->title_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(user_data->description_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(timer_layer));
}
  
static void timer_window_unload(Window* window) {
  // Get pointers to children
  timer_window_data* user_data = (timer_window_data*) window_get_user_data(window);
  
  // Destroy children
  text_layer_destroy(user_data->title_layer);
  text_layer_destroy(user_data->description_layer);
  text_layer_destroy(timer_layer);
  
  // Free user data
  free(user_data->title);
  free(user_data->description);
  free(user_data);
}

Window* create_timer_window (char* title, char* description, time_t duration) {
  // Create step_window
  Window* window = window_create();
  
  // Create memory for pointers to children
  timer_window_data* user_data = (timer_window_data*) malloc(sizeof(timer_window_data));
  
  // Write arguments to user data
  user_data->title = (char*) malloc(strlen(title));
  strcpy(user_data->title, title);
  user_data->description = (char*) malloc(strlen(description));
  strcpy(user_data->description, description);
  user_data->duration = duration;
  
  // Set user data to pointers to children
  window_set_user_data(window, (void*)user_data);
  
  // Set button handlers
  window_set_click_config_provider(window, top_click_config_provider);
  
  // Set step_window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = timer_window_load,
    .unload = timer_window_unload,
  });
  
  // Return step_window
  return window;
}

/* * * * * * * *
 * Step Window *
 * * * * * * * */

typedef struct step_window_data {
  char* title;
  char* description;
  TextLayer* title_layer;
  TextLayer* description_layer;
} step_window_data;

void step_window_load(Window* window) {
  // Get pointers to children
  step_window_data* user_data = (step_window_data*) window_get_user_data(window);
  
  // Create title_layer
  user_data->title_layer = text_layer_create(GRect(5, 0, 134, 45));
  text_layer_set_background_color(user_data->title_layer, GColorClear);
  text_layer_set_text_color(user_data->title_layer, GColorBlack);
  text_layer_set_font(user_data->title_layer, fonts_get_system_font(TITLE_FONT));
  text_layer_set_text(user_data->title_layer, user_data->title);
  
  // Create description layer
  user_data->description_layer = text_layer_create(GRect(5, 45, 134, 100));
  text_layer_set_background_color(user_data->description_layer, GColorClear);
  text_layer_set_text_color(user_data->description_layer, GColorBlack);
  text_layer_set_font(user_data->description_layer, fonts_get_system_font(DESCRIPTION_FONT));  
  text_layer_set_overflow_mode(user_data->description_layer, GTextOverflowModeWordWrap);
  text_layer_set_text(user_data->description_layer, user_data->description);
  
  
  
  // Add layers to window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(user_data->title_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(user_data->description_layer));
}

void step_window_unload(Window* window) {
  // Get pointers to children
  step_window_data* user_data = (step_window_data*) window_get_user_data(window);

  // Destroy children
  text_layer_destroy(user_data->title_layer);
  text_layer_destroy(user_data->description_layer);
  
  // Free user data
  free(user_data->title);
  free(user_data->description);
  free(user_data);
}

Window* create_step_window(char* title, char* description) {
  // Create step_window
  Window* window = window_create();
    
  // Create memory for pointers to children
  step_window_data* user_data = (step_window_data*) malloc(sizeof(step_window_data));
  
  // Write arguments to user data
  user_data->title = (char*) malloc(strlen(title));
  strcpy(user_data->title, title);
  user_data->description = (char*) malloc(strlen(description));
  strcpy(user_data->description, description);
  
  // Set user data to pointers to children
  window_set_user_data(window, (void*)user_data);
  
  // Set button handlers
  window_set_click_config_provider(window, top_click_config_provider);
  
  // Set step_window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = step_window_load,
    .unload = step_window_unload,
  });
  
  // Return step_window
  return window;
}

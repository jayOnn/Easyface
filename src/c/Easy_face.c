#include <pebble.h>
#include "src/c/battery.h"

static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;

static TextLayer *s_date_layer;
static GFont s_date_font;

extern void battery_callback(BatteryChargeState state);
extern void battery_update_proc(Layer *layer, GContext *ctx);
extern Layer *s_battery_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
	static char s_buffer2[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

	strftime(s_buffer2, sizeof(s_buffer2), "%b %d",tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
	text_layer_set_text(s_date_layer, s_buffer2);
	
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));

	
	s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_BIT_FONT_48));
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

	
	// Date Layer
	s_date_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(88, 100), bounds.size.w, 25));
	text_layer_set_background_color(s_date_layer,GColorClear);
	text_layer_set_text_color(s_date_layer, GColorWhite);
	text_layer_set_text_alignment(s_date_layer,GTextAlignmentRight);
	text_layer_set_text(s_date_layer, "Loading...");
	
	s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_GEO_SANS_LIGHT_18));
	text_layer_set_font(s_date_layer,s_date_font);
	
	// Battery Layer
	s_battery_layer = layer_create(GRect(0,0,1,168));
	layer_set_update_proc(s_battery_layer, battery_update_proc);
	
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
	layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
	layer_add_child(window_layer, s_battery_layer);
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
	fonts_unload_custom_font(s_time_font);
	text_layer_destroy(s_date_layer);
	fonts_unload_custom_font(s_date_font);
	layer_destroy(s_battery_layer);
}


static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

	// Set background color to black
	window_set_background_color(s_main_window, GColorBlack);
	
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();
	
	// Ensure battery level is displayed from the start
	battery_callback(battery_state_service_peek());

  // Register to Service
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	battery_state_service_subscribe(battery_callback);
	
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
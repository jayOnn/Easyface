#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;

static TextLayer *s_date_layer;
static GFont s_date_font;

extern void battery_callback(BatteryChargeState state);
extern void battery_update_proc(Layer *layer, GContext *ctx);
extern Layer *s_battery_layer;

extern BitmapLayer *s_background_layer, *s_bt_layer;
extern GBitmap *s_background_bitmap, *s_bt_bitmap;
extern void bluetooth_callback(bool connected);
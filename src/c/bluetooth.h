#pragma once
#include <pebble.h>

void bluetooth_callback(bool connected);

BitmapLayer *s_background_layer, *s_bt_layer;
GBitmap *s_background_bitmap, *s_bt_bitmap;
#pragma once
#include <pebble.h>

Layer *s_battery_layer;

int s_battery_level;
void battery_callback(BatteryChargeState state);
void battery_update_proc(Layer *layer, GContext *ctx);


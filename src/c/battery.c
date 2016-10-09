#include "battery.h"

void battery_callback(BatteryChargeState state){
	layer_mark_dirty(s_battery_layer);
	s_battery_level = state.charge_percent;
}

void battery_update_proc(Layer *layer, GContext *ctx){
	GRect bounds = layer_get_bounds(layer);
	
	// height of bar
	int height = (int)(float)((1 - ((float)s_battery_level / 100.0F)) * 168.0F);
	
	//draw background
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_fill_rect(ctx, bounds, 0, GCornerNone);
	
	//Draw Bar
	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_fill_rect(ctx, GRect(0, 0, bounds.size.w, height), 0, GCornerNone);
}

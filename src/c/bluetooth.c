#include "bluetooth.h"

void bluetooth_callback(bool connected){
	layer_set_hidden(bitmap_layer_get_layer(s_bt_layer), connected);
	
	if(!connected){
		vibes_double_pulse();
	}
}


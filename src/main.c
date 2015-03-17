#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer, *s_date_layer, *s_hashtag_layer;
static GFont s_time_font, s_date_font, s_hashtag_font;

static void update_time() {

	// Create and retrieve a time structure from the curent local time
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);

	// Create a long-lived buffer
	static char buffer[] = "00:00";
	static char date[] = "Mon 01 Jan";

	// Write the current hours and minutes into the buffer
	if (clock_is_24h_style() == true) {
		// Use 24 hour format
		strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
	} else {
		// Use 12 hour format
		strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
	}

	// Write the current date into the date buffer
	strftime(date, sizeof("01/01/15"), "%m/%d/%g", tick_time);

	// Display this time and date on the TextLayers
	text_layer_set_text(s_time_layer, buffer);
	text_layer_set_text(s_date_layer, date);

}

static void main_window_load(Window *window) {

	// Initialize the window layer
	Layer *window_layer = window_get_root_layer(window);

	// Create Time Layer
	s_time_layer = text_layer_create(GRect(0, 30, 144, 50));
	text_layer_set_background_color(s_time_layer, GColorClear);
	#ifdef PBL_COLOR
		text_layer_set_text_color(s_time_layer, GColorChromeYellow);
	#else
		text_layer_set_text_color(s_time_layer, GColorWhite);
	#endif
	text_layer_set_text(s_time_layer, "00:00");

	s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PACIFICO_30));

	text_layer_set_font(s_time_layer, s_time_font);
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

	layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

	// Create Date Layer
	s_date_layer = text_layer_create(GRect(0, 65, 144, 50));
	text_layer_set_background_color(s_date_layer, GColorClear);
	#ifdef PBL_COLOR
		text_layer_set_text_color(s_date_layer, GColorChromeYellow);
	#else
		text_layer_set_text_color(s_date_layer, GColorWhite);
	#endif
	text_layer_set_text(s_date_layer, "01/01/15");

	s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PACIFICO_30));

	text_layer_set_font(s_date_layer, s_date_font);
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

	layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
	
	// Create Hashtag Layer
	s_hashtag_layer = text_layer_create(GRect(0, 105, 144, 25));
	text_layer_set_background_color(s_hashtag_layer, GColorClear);
	#ifdef PBL_COLOR
		text_layer_set_text_color(s_hashtag_layer, GColorChromeYellow);
	#else
		text_layer_set_text_color(s_hashtag_layer, GColorWhite);
	#endif
	text_layer_set_text(s_hashtag_layer, "#PebbleLove");
	
	s_hashtag_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PACIFICO_24));

	text_layer_set_font(s_hashtag_layer, s_hashtag_font);
	text_layer_set_text_alignment(s_hashtag_layer, GTextAlignmentCenter);

	layer_add_child(window_layer, text_layer_get_layer(s_hashtag_layer));

}

static void main_window_unload(Window *window) {

	// Destroy layers
	text_layer_destroy(s_time_layer);
	text_layer_destroy(s_date_layer);
	text_layer_destroy(s_hashtag_layer);

	// Destroy fonts
  	fonts_unload_custom_font(s_time_font);
  	fonts_unload_custom_font(s_date_font);
	fonts_unload_custom_font(s_hashtag_font);

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {

	// Update the time every tick
	update_time();

}

static void init() {

	// Create the window with a black background
	s_main_window = window_create();
	window_set_fullscreen(s_main_window, true);
	#ifdef PBL_COLOR
		window_set_background_color(s_main_window, GColorPictonBlue);
	#else
		window_set_background_color(s_main_window, GColorBlack);
	#endif
	
	window_set_window_handlers(s_main_window, (WindowHandlers) {
    	.load = main_window_load,
    	.unload = main_window_unload,
	});

	window_stack_push(s_main_window, true);

	// Subscribe to time updates
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

}

static void deinit() {

	// Destroy window on close
	window_destroy(s_main_window);

}

int main() {

	init();
	app_event_loop();
	deinit();

}
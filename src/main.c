#include "pebble.h"
static Window *main_window;
static Layer *window_layer;
static Layer *child_layer;

static void timer_callback(void *data) {
 ((int16_t*)(child_layer))[4] += ((int16_t*)(child_layer))[4]>144 ? -144 : 1;  // Increment child_layer.origin.x
 ((int16_t*)(child_layer))[5] += ((int16_t*)(child_layer))[5]>168 ? -168 : 1;  // Increment child_layer.origin.y
 layer_mark_dirty(window_layer); // v2
 app_timer_register(20, timer_callback, NULL); // Schedule a callback in 20ms
}

static void child_layer_update_callback(Layer *me, GContext *ctx) {
 graphics_context_set_fill_color(ctx, GColorBlack); // Draw a black pixel
 graphics_fill_rect(ctx, GRect(0,0,10,10), 0, GCornerNone);
}

static void root_layer_update_callback(Layer *me, GContext *ctx) {
 uint16_t* framebuffer = ((uint16_t*)(((GBitmap*)ctx)->addr));
 for (uint16_t i=0; i<160*168/16; i++)
   framebuffer[i] = rand(); // Draw random pixels
//  graphics_context_set_stroke_color(ctx, GColorClear); // Draw a clear pixel
//  graphics_draw_pixel(ctx, GPoint(0, 35)); // At: X = 0 Y = 35
}

int main(void) {
 // Init
 main_window = window_create();
 window_layer = window_get_root_layer(main_window);
 child_layer = layer_create(GRect(0,0,10,10));
 layer_set_update_proc(child_layer, child_layer_update_callback);
 layer_set_update_proc(window_layer, root_layer_update_callback);
 layer_add_child(window_layer, child_layer);
 window_set_fullscreen(main_window, true);
 window_stack_push(main_window, true);
 timer_callback(NULL); // Establish timer loop
  
 app_event_loop(); // Block until ready to exit
  
 // De-init
 layer_destroy(child_layer);
 window_destroy(main_window);
}
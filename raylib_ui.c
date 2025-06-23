#define UI_REMOVE_PREFIX
#include "ui.h"
#include <raylib.h>

static UI_Vector2f measure_text(UI_Font* font, const char *text, int font_size) {
	Font *raylib_font = (Font *)font;
	/*RLAPI Vector2 MeasureTextEx(Font font, const char *text, float fontSize, float spacing);    // Measure string size for Font*/

	/*UI_log_info("text: %s | font_size: %d", text, font_size);;*/
	Vector2 res = MeasureTextEx(*raylib_font, text, (float)font_size, 1.f);
	/*UI_log_info("glyphCount: %d", raylib_font->glyphCount);*/
	return UI_CLITERAL(UI_Vector2f) { res.x, res.y };
}

static UI_Vector2f get_mpos(void* data) {
	(void)data;
	Vector2 res = GetMousePosition();
	return UI_CLITERAL(UI_Vector2f) { res.x, res.y };
}

static bool mouse_button_released(UI_Mouse_button btn) {
	// NOTE: We don't have to transltate UI_Mouse_button to raylib's since they have the same value (for now)
	return IsMouseButtonReleased((int)btn);
}

static bool mouse_button_pressed(UI_Mouse_button btn) {
	// NOTE: We don't have to transltate UI_Mouse_button to raylib's since they have the same value (for now)
	return IsMouseButtonPressed((int)btn);
}

static bool mouse_button_down(UI_Mouse_button btn) {
	// NOTE: We don't have to transltate UI_Mouse_button to raylib's since they have the same value (for now)
	return IsMouseButtonDown((int)btn);
}

static Color UI_Color_to_Raylib_Color(const UI_Color color) {
	return CLITERAL(Color) {
		.r = color.r,
		.g = color.g,
		.b = color.b,
		.a = color.a,
	};
}

static void draw_box(UI_Vector2f pos, UI_Vector2f size, UI_Color fill_color, UI_Color out_color) {
	DrawRectangleV(CLITERAL(Vector2) { pos.x, pos.y }, CLITERAL(Vector2) { size.x, size.y }, UI_Color_to_Raylib_Color(fill_color));
	DrawRectangleLines((int)pos.x, (int)pos.y, (int)size.x, (int)size.y, UI_Color_to_Raylib_Color(out_color));
}

static void draw_text(UI_Font *font, const char *text, UI_Vector2f pos, int font_size, UI_Color color) {
	Font *raylib_font = (Font *)font;
	DrawTextEx(*raylib_font, text, CLITERAL(Vector2) { pos.x, pos.y }, font_size, 1.f, UI_Color_to_Raylib_Color(color));
}

void init_raylib_ui_bindings(void) {
	UI_set_measure_text(measure_text);
	UI_set_get_mpos(get_mpos);
	UI_set_mouse_button_released(mouse_button_released);
	UI_set_mouse_button_pressed(mouse_button_pressed);
	UI_set_mouse_button_down(mouse_button_down);
	UI_set_draw_box(draw_box);
	UI_set_draw_text(draw_text);
}

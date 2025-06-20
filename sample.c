#define UI_IMPLEMENTATION
#define UI_REMOVE_PREFIX
#include "ui.h"
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 600

UI_Vector2f measure_text(UI_Font* font, const char *text, int font_size) {
	Font *raylib_font = (Font *)font;
	/*RLAPI Vector2 MeasureTextEx(Font font, const char *text, float fontSize, float spacing);    // Measure string size for Font*/

	/*UI_log_info("text: %s | font_size: %d", text, font_size);;*/
	Vector2 res = MeasureTextEx(*raylib_font, text, (float)font_size, 1.f);
	/*UI_log_info("glyphCount: %d", raylib_font->glyphCount);*/
	return UI_CLITERAL(UI_Vector2f) { res.x, res.y };
}

UI_Vector2f get_mpos(void* data) {
	(void)data;
	Vector2 res = GetMousePosition();
	return UI_CLITERAL(UI_Vector2f) { res.x, res.y };
}

bool mouse_button_released(UI_Mouse_button btn) {
	// NOTE: We don't have to transltate UI_Mouse_button to raylib's since they have the same value (for now)
	return IsMouseButtonReleased((int)btn);
}

bool mouse_button_pressed(UI_Mouse_button btn) {
	// NOTE: We don't have to transltate UI_Mouse_button to raylib's since they have the same value (for now)
	return IsMouseButtonPressed((int)btn);
}

bool mouse_button_down(UI_Mouse_button btn) {
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

void draw_box(UI_Vector2f pos, UI_Vector2f size, UI_Color fill_color, UI_Color out_color) {
	DrawRectangleV(CLITERAL(Vector2) { pos.x, pos.y }, CLITERAL(Vector2) { size.x, size.y }, UI_Color_to_Raylib_Color(fill_color));
	DrawRectangleLines((int)pos.x, (int)pos.y, (int)size.x, (int)size.y, UI_Color_to_Raylib_Color(out_color));
}

void draw_text(UI_Font *font, const char *text, UI_Vector2f pos, int font_size, UI_Color color) {
	Font *raylib_font = (Font *)font;
	DrawTextEx(*raylib_font, text, CLITERAL(Vector2) { pos.x, pos.y }, font_size, 1.f, UI_Color_to_Raylib_Color(color));
}

int main(void) {

	InitWindow(WIDTH, HEIGHT, "Raylib UI sample");

	const char *font_path = "res/fonts/arial.ttf";
	Font font = LoadFont(font_path);
	if (!IsFontReady(font)) {
		UI_log_error("font '%s' is not valid!", font_path);
		CloseWindow();
		return 1;
	}

	UI_Context ctx = UI_Context_make((UI_Font *)&font, UI_CLITERAL(UI_Vector2f) { 100.f, 100.f });

	UI_set_measure_text(measure_text);
	UI_set_get_mpos(get_mpos);
	UI_set_mouse_button_released(mouse_button_released);
	UI_set_mouse_button_pressed(mouse_button_pressed);
	UI_set_mouse_button_down(mouse_button_down);
	UI_set_draw_box(draw_box);
	UI_set_draw_text(draw_text);

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(BLACK);

		UI_begin(&ctx, UI_LAYOUT_KIND_VERT);

			if (UI_button(&ctx, "Click me!", 18, UI_COLOR_WHITE)) {
				log_info("Button Clicked!");
			}

			UI_text(&ctx, "This is a long text.", 24, UI_COLOR_RED);
			UI_text(&ctx, "This too is a long text.", 18, UI_COLOR_WHITE);

		UI_end(&ctx);

		EndDrawing();
	}

	CloseWindow();

	UI_Context_free(&ctx);
	return 0;
}

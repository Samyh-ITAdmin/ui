#define UI_IMPLEMENTATION
#define UI_REMOVE_PREFIX
#include <ui.h>
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 600

UI_Vector2f measure_text(UI_Font* font, const char *text, int font_size) {
	Font *raylib_font = (Font *)font;
	/*RLAPI Vector2 MeasureTextEx(Font font, const char *text, float fontSize, float spacing);    // Measure string size for Font*/

	Vector2 res = MeasureTextEx(*raylib_font, text, (float)font_size, 0.f);
	return UI_CLITERAL(UI_Vector2f) { res.x, res.y };
}

UI_Vector2f get_mpos(void* data) {
	(void)data;
	Vector2 res = GetMousePosition();
	return UI_CLITERAL(UI_Vector2f) { res.x, res.y };
}

int main(void) {
	Font font = GetFontDefault();
	UI_Context ctx = UI_Context_make((UI_Font *)&font);
	
	InitWindow(WIDTH, HEIGHT, "Raylib UI sample");

	UI_set_measure_text(measure_text);
	UI_set_get_mpos(get_mpos);

	while (!WindowShouldClose()) {
		BeginDrawing();

		UI_begin(&ctx, UI_LAYOUT_KIND_VERT);
		
		if (UI_button(&ctx, "Click me!", 18, UI_COLOR_WHITE)) {
			printf("Button Clicked!\n");
		}

		UI_end(&ctx);

		EndDrawing();
	}

	CloseWindow();

	UI_Context_free(&ctx);
	return 0;
}

#include "raylib_ui.c"

#define UI_IMPLEMENTATION
#define UI_REMOVE_PREFIX
#include "ui.h"
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 600

int main(void) {

	InitWindow(WIDTH, HEIGHT, "Raylib UI sample");

	const char *font_path = "res/fonts/arial.ttf";
	Font font = LoadFont(font_path);
	if (!IsFontReady(font)) {
		UI_log_error("font '%s' is not valid!", font_path);
		CloseWindow();
		return 1;
	}
	init_raylib_ui_bindings();

	UI_Context ctx = UI_Context_make((UI_Font *)&font, UI_CLITERAL(UI_Vector2f) { 100.f, 100.f }, "Sample", WIDTH, HEIGHT);

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(BLACK);

		UI_begin(&ctx);

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

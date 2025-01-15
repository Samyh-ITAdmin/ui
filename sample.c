#define UI_IMPLEMENTATION
#define UI_REMOVE_PREFIX
#include <ui.h>
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 600
int main(void) {
	UI_Context ctx = UI_Context_make();
	
	InitWindow(WIDTH, HEIGHT, "Raylib UI sample");

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

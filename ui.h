#ifndef UI_H_
#define UI_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

// Remove Prefix
#ifdef UI_REMOVE_PREFIX
#define ASSERT UI_ASSERT
#define Vector2f UI_Vector2f
#define Layout_kind UI_Layout_kind
#define LAYOUT_KIND_VERT UI_LAYOUT_KIND_VERT
#define LAYOUT_KIND_HORZ UI_LAYOUT_KIND_HORZ
#define LAYOUT_KIND_COUNT UI_LAYOUT_KIND_COUNT
#define Layout UI_Layout
#define Layout_make UI_Layout_make
#define Layout_available_pos UI_Layout_available_pos
#define Layout_push_widget UI_Layout_push_widget
#define LAYOUTS_CAP UI_LAYOUTS_CAP
#define Context UI_Context
#define Context_make UI_Context_make
#define Context_free UI_Context_free
#define Context_push_layout UI_Context_push_layout
#define log_info UI_log_info
#define log_error UI_log_error
#define log_warning UI_log_warning
#endif

/*
 * Stand-Alone UI library that can work with any graphics API.
 */

// Macros

// Macro for calling callbacks
#define UI_CALL(func, ...) ({\
                UI_ASSERT(func != NULL, "Trying to call '"#func"', that points to NULL!");\
                func(__VA_ARGS__);\
        })
// NOTE: Copy-pasted from raylib.h
// This is called aggregate initialization (C++11 feature)
#if defined(__cplusplus)
    #define UI_CLITERAL(type)      type
#else
    #define UI_CLITERAL(type)      (type)
#endif

#define UI_ASSERT(condition, message) do {\
		if (!(condition)) {\
			fprintf(stderr, "%s:%u:0: ASSERTION FAILED: %s '%s'\n", __FILE__, __LINE__, #condition, message);\
			exit(1);\
		}\
	} while (0)

// Logging
#define UI_log_error(fmt, ...) do {\
		fprintf(stderr, "%s"fmt"\n", "[ERROR] ", ##__VA_ARGS__);\
	} while (0)
#define UI_log_info(fmt, ...) do {\
		fprintf(stdout, "%s"fmt"\n", "[INFO] ", ##__VA_ARGS__);\
	} while (0)
#define UI_log_warning(fmt, ...) do {\
		fprintf(stdout, "%s"fmt"\n", "[WARNING] ", ##__VA_ARGS__);\
	} while (0)
#if defined(DEBUG)
#define UI_log_debug(fmt, ...) do {\
		fprintf(stdout, "%s"fmt"\n", "[DEBUG] ", ##__VA_ARGS__);\
	} while (0)
#else
#define UI_log_debug(...)
#endif // defined(DEBUG)

// Struct: UI_Vector2f
typedef struct {
	float x, y;
} UI_Vector2f;

UI_Vector2f v2(float x, float y);
UI_Vector2f v2xx(float x);

// Struct: UI_Rect
typedef struct {
	float x, y, width, height;
} UI_Rect;

// Methods of UI_Rect
bool UI_Rect_has_point(const UI_Rect *rect, UI_Vector2f point);

typedef struct {
	uint8_t r, g, b, a;
} UI_Color;

// Predefined Colors
#define UI_COLOR_WHITE UI_CLITERAL(UI_Color) { 255, 255, 255, 255 }
#define UI_COLOR_BLACK UI_CLITERAL(UI_Color) {   0,   0,   0, 255 }
#define UI_COLOR_RED   UI_CLITERAL(UI_Color) { 255,   0,   0, 255 }
#define UI_COLOR_GREEN UI_CLITERAL(UI_Color) {   0, 255,   0, 255 }
#define UI_COLOR_BLUE  UI_CLITERAL(UI_Color) {   0,   0, 255, 255 }

// Typedef: UI_Font
typedef void* UI_Font;

// Enum: UI_Mouse_button
typedef enum {
	UI_MOUSE_BUTTON_LEFT = 0,
	UI_MOUSE_BUTTON_RIGHT,
	UI_MOUSE_BUTTON_MIDDLE,
	UI_MOUSE_BUTTON_COUNT,
} UI_Mouse_button;

// Enum: UI_Layout_kind
typedef enum {
	UI_LAYOUT_KIND_VERT,
	UI_LAYOUT_KIND_HORZ,
	UI_LAYOUT_KIND_COUNT,
} UI_Layout_kind;

// Struct: UI_Layout
typedef struct {
	UI_Layout_kind kind;
	UI_Vector2f pos, size;
} UI_Layout;

// Methods of UI_Layout
UI_Layout   UI_Layout_make(UI_Layout_kind kind);
UI_Vector2f UI_Layout_available_pos(UI_Layout* layout);
void        UI_Layout_push_widget(UI_Layout* layout, UI_Vector2f size);

typedef struct {
	UI_Vector2f pos;
#define UI_LAYOUTS_CAP 1024
	UI_Layout layouts[LAYOUTS_CAP];
	size_t    layouts_count;
	int active_id;
	int last_used_id;
	UI_Font *font;
	UI_Vector2f button_padding;
} UI_Context;

// Methods of UI_Context
UI_Context UI_Context_make(UI_Font* font, UI_Vector2f pos);
void       UI_begin(UI_Context* ctx, UI_Layout_kind kind);
void       UI_end(UI_Context* ctx);
bool       UI_button(UI_Context* ctx, const char *text, int font_size, UI_Color color);
void       UI_text(UI_Context *ctx, const char *text, int font_size, UI_Color color);
void       UI_Context_push_layout(UI_Context* ctx, UI_Layout layout);
UI_Layout  UI_Context_pop_layout(UI_Context* ctx);
UI_Layout *UI_Context_top_layout(UI_Context* ctx);
void       UI_Context_free(UI_Context* ctx);

#define DECLARE_CALLBACK(func_name, ret_type, ...) typedef ret_type(*UI_##func_name##_func)(__VA_ARGS__);\
	void UI_set_##func_name(UI_##func_name##_func callback);\
	UI_##func_name##_func UI_##func_name
#define DEFINE_CALLBACK(func_name) void UI_set_##func_name(UI_##func_name##_func callback) { UI_##func_name = callback; }

// Graphics plugin callbacks
DECLARE_CALLBACK(measure_text, UI_Vector2f, UI_Font*, const char*, int);

DECLARE_CALLBACK(get_mpos, UI_Vector2f, void*);
DECLARE_CALLBACK(mouse_button_released, bool, UI_Mouse_button);
DECLARE_CALLBACK(mouse_button_pressed, bool, UI_Mouse_button);
DECLARE_CALLBACK(mouse_button_down, bool, UI_Mouse_button);

DECLARE_CALLBACK(draw_box, void, UI_Vector2f, UI_Vector2f, UI_Color, UI_Color);
DECLARE_CALLBACK(draw_text, void, UI_Font*, const char*, UI_Vector2f, int, UI_Color);

#endif
//////////////////////////////////////////////////
#ifdef UI_IMPLEMENTATION

// Methods of UI_Vector2f

UI_Vector2f v2(float x, float y) {
	return UI_CLITERAL(UI_Vector2f) { x, y };
}

UI_Vector2f v2xx(float x) {
	return v2(x, x);
}

// Methods of UI_Rect
bool UI_Rect_has_point(const UI_Rect *rect, UI_Vector2f point) {
	UI_Rect r = *rect;
	return (point.x >= r.x && point.x <= r.x + r.width &&
	    point.y >= r.y && point.y <= r.y + r.height);
}

// Methods of UI_Layout
UI_Layout UI_Layout_make(UI_Layout_kind kind) {
	UI_Layout l = {
		.kind = kind,
	};
	return l;
}

UI_Vector2f  UI_Layout_available_pos(UI_Layout* layout) {
	switch (layout->kind) {
		case UI_LAYOUT_KIND_VERT: {
						  return (UI_Vector2f) { layout->pos.x, layout->pos.y + layout->size.y };
					  } break;
		case UI_LAYOUT_KIND_HORZ: {
						  return (UI_Vector2f) { layout->pos.x + layout->size.x, layout->pos.y };
					  } break;
		case UI_LAYOUT_KIND_COUNT:
		default: {
				 UI_ASSERT(0, "UNREACHABLE!");
			 } break;

	}
	return (UI_Vector2f) {0.f, 0.f};
}

void UI_Layout_push_widget(UI_Layout* layout, UI_Vector2f size) {
	switch (layout->kind) {
		case UI_LAYOUT_KIND_VERT: {
						  layout->size.x += size.x;
						  layout->size.y = fmaxf(layout->size.y, size.y);
					  } break;
		case UI_LAYOUT_KIND_HORZ: {
						  layout->size.x = fmaxf(layout->size.x, size.x);
						  layout->size.y += size.y;
					  } break;
		case UI_LAYOUT_KIND_COUNT:
		default: {
				 UI_ASSERT(0, "UNREACHABLE!");
			 } break;
	}
}

// Methods of UI_Context
UI_Context UI_Context_make(UI_Font* font, UI_Vector2f pos) {
	UI_Context ctx = {0};
	ctx.active_id = -1;
	ctx.layouts_count = 0;
	ctx.font = font;
	ctx.pos = pos;
	ctx.button_padding = UI_CLITERAL(UI_Vector2f) {
		.x = 10.f,
		.y = 10.f,
	};

	return ctx;
}

void UI_begin(UI_Context* ctx, UI_Layout_kind kind) {
	UI_Layout layout = {0};
	layout.pos = (UI_Vector2f) {
		.x = ctx->pos.x,
		.y = ctx->pos.y
	};
	layout.kind = kind;
	UI_Context_push_layout(ctx, layout);
}

void UI_end(UI_Context* ctx) {
	ctx->last_used_id = 0;
	UI_Context_pop_layout(ctx);
}

// NOTE: local helper
static void push_ui_widget(UI_Context* ctx, UI_Layout* layout, UI_Vector2f size) {
    (void)ctx;
	switch (layout->kind) {
		case UI_LAYOUT_KIND_HORZ: {
			// ctx->ui_rect.width += size.x;
			// ctx->ui_rect.height = fmaxf(ctx->ui_rect.height, size.y);
		} break;
		case UI_LAYOUT_KIND_VERT: {
			// ctx->ui_rect.width = fmaxf(ctx->ui_rect.width, size.x);
			// ctx->ui_rect.height += size.y;
		} break;
		case UI_LAYOUT_KIND_COUNT:
		default: UI_ASSERT(0, "Unreachable");
	}
    UI_Layout_push_widget(layout, size);
}

bool UI_button(UI_Context* ctx, const char *text, int font_size, UI_Color color) {
	int id = ctx->last_used_id++;
	UI_Layout *top = UI_Context_top_layout(ctx);
	if (top == NULL) {
		UI_log_error("This function must be used between 'begin' and 'end'!");
		return false;
	}

	const UI_Vector2f pos = UI_Layout_available_pos(top);
	UI_Vector2f size = UI_CALL(UI_measure_text, ctx->font, text, font_size);
	size.x += ctx->button_padding.x * 2.f;
	size.y += ctx->button_padding.y * 2.f;

	const UI_Rect rect = {
		.x = pos.x,
		.y = pos.y,
		.width = size.x,
		.height = size.y,
	};

	// TODO: Maybe just pass a mpos ptr to the ctx?
	bool click = false;
	UI_Vector2f mpos = UI_CALL(UI_get_mpos, NULL);

	bool hovering = UI_Rect_has_point(&rect, mpos);

	if (ctx->active_id == id) {
		bool m = UI_CALL(UI_mouse_button_released, UI_MOUSE_BUTTON_LEFT);
		if (m) {
			ctx->active_id = -1;
			if (hovering) {
				click = true;
			}
		}
	} else {
		bool m = UI_CALL(UI_mouse_button_pressed, UI_MOUSE_BUTTON_LEFT);
		if (hovering && m) {
			ctx->active_id = id;
		}
	}

	float alpha = 0.4f;
	if (hovering) {
		alpha += 0.1f;
	}

	bool btn_down = UI_CALL(UI_mouse_button_down, UI_MOUSE_BUTTON_LEFT);
	bool is_clicked = (hovering && btn_down);
	if (is_clicked) {
		alpha += 0.2f;
	}

	push_ui_widget(ctx, top, size);

	// TODO: Move drawing to post
	color.a = (uint8_t)(alpha * 255.f);
	UI_CALL(UI_draw_box, pos, size, color, UI_COLOR_WHITE);

	UI_Vector2f text_pos = UI_CLITERAL(UI_Vector2f) {
		.x = pos.x + ctx->button_padding.x,
		.y = pos.y + ctx->button_padding.y,
	};

	if (is_clicked) {
		text_pos.x += ctx->button_padding.x * 0.25f;
		text_pos.y += ctx->button_padding.y * 0.25f;
	}

	UI_CALL(UI_draw_text, ctx->font, text, text_pos, font_size, UI_COLOR_WHITE);

	/*if (!ctx->show) click = false;*/
	/*if (click) ignore_mouse_input(true);*/

	return click;
}

void UI_text(UI_Context *ctx, const char *text, int font_size, UI_Color color) {
	int id = ctx->last_used_id++;
	UI_Layout *top = UI_Context_top_layout(ctx);
	if (top == NULL) {
		UI_log_error("This function must be used between 'begin' and 'end'!");
		return;
	}

	const UI_Vector2f pos = UI_Layout_available_pos(top);
	UI_Vector2f size = UI_CALL(UI_measure_text, ctx->font, text, font_size);
	size.x += ctx->button_padding.x * 2.f;
	size.y += ctx->button_padding.y * 2.f;

	const UI_Rect rect = {
		.x = pos.x,
		.y = pos.y,
		.width = size.x,
		.height = size.y,
	};

	UI_log_debug("Rect: %f, %f,  %f, %f", rect.x, rect.y, rect.width, rect.height);

	push_ui_widget(ctx, top, size);

	UI_Vector2f text_pos = pos;
	UI_CALL(UI_draw_text, ctx->font, text, text_pos, font_size, UI_COLOR_WHITE);

}

void UI_Context_push_layout(UI_Context* ctx, UI_Layout layout) {
	UI_ASSERT(ctx->layouts_count + 1 < UI_LAYOUTS_CAP, "Layouts exhausted! Please increase the UI_LAYOUTS_CAP!");

	ctx->layouts[ctx->layouts_count++] = layout;
}

UI_Layout UI_Context_pop_layout(UI_Context* ctx) {
	UI_ASSERT(ctx->layouts_count > 0, "Popping from an empty stack!");
	return ctx->layouts[--ctx->layouts_count];
}

UI_Layout *UI_Context_top_layout(UI_Context* ctx) {
	if (ctx->layouts_count <= 0) return NULL;
	return &ctx->layouts[ctx->layouts_count-1];
}

void UI_Context_free(UI_Context* ctx) {
    (void)ctx;
}

// Graphics plugin callbacks
DEFINE_CALLBACK(measure_text);
DEFINE_CALLBACK(get_mpos);
DEFINE_CALLBACK(mouse_button_released);
DEFINE_CALLBACK(mouse_button_pressed);
DEFINE_CALLBACK(mouse_button_down);
DEFINE_CALLBACK(draw_box);
DEFINE_CALLBACK(draw_text);

#endif

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
#define UI_Vector2f Vector2f 
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
#endif

/*
 * Stand-Alone UI library that can work with any graphics API.
 */

// Macros

// Macro for calling callbacks
#define UI_CALL(func, ...) ({\
                UI_ASSERT(func != NULL, "Trying to call a NULL pointed function!");\
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
			fprintf(stderr, "%s:%u:0: ASSERTION FAILED: %s '%s'", __FILE__, __LINE__, #condition, message);\
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

// Struct: UI_Vector2f
typedef struct {
	float x, y;
} UI_Vector2f;

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
UI_Layout UI_Layout_make(UI_Layout_kind kind);
UI_Vector2f  UI_Layout_available_pos(UI_Layout* layout);
void      UI_Layout_push_widget(UI_Layout* layout, UI_Vector2f size);

typedef struct {
	UI_Vector2f pos;
#define UI_LAYOUTS_CAP 1024
	UI_Layout layouts[LAYOUTS_CAP];
	size_t    layouts_count;
	int active_id;
	int last_used_id;
	UI_Font *font;
} UI_Context;

// Methods of UI_Context
UI_Context UI_Context_make();
void       UI_begin(UI_Context* ctx, UI_Layout_kind kind);
void       UI_end(UI_Context* ctx);
bool       UI_button(UI_Context* ctx, const char *text, int font_size, UI_Color color);
void       UI_Context_push_layout(UI_Context* ctx, UI_Layout layout);
UI_Layout  UI_Context_pop_layout(UI_Context* ctx);
UI_Layout *UI_Context_top_layout(UI_Context* ctx);
void       UI_Context_free(UI_Context* ctx);

// Graphics plugin callbacks
typedef UI_Vector2f(UI_Measure_text_func)(UI_Font*, const char*, int);
void UI_set_Measure_text_callback(UI_Measure_text_func);
UI_Measure_text_func UI_Measure_text;

#endif
//////////////////////////////////////////////////
#ifdef UI_IMPLEMENTATION

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
UI_Context UI_Context_make() {
	UI_Context ctx = {0};
	ctx.active_id = -1;
	ctx.layouts_count = 0;

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

bool UI_button(UI_Context* ctx, const char *text, int font_size, UI_Color color) {
	int id = ctx->last_used_id++; UI_Layout *top = UI_Context_top_layout(ctx);
	if (top == NULL) {
		UI_log_error("This function must be used between 'begin' and 'end'!");
		return false;
	}

	const UI_Vector2f pos  = UI_Layout_available_pos(top);
	UI_Vector2f size;
	size = UI_CALL(UI_Measure_text, ctx->font, text, font_size);
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

}

#endif

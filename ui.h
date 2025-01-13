#ifndef UI_H_
#define UI_H_

/*
 * Stand-Alone UI library that can work with any graphics API.
 */

// TODO: Implement UI_UNREACHABLE
/*// Macros*/
/*#define UI_UNREACHABLE*/

// Struct: UI_Vector2f
typedef struct {
	float x, y;
} UI_Vector2f;

// Enum: UI_Layout_kind
typedef enum {
	UI_LAYOUT_KIND_HORZ,
	UI_LAYOUT_KIND_VERT,
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
#define LAYOUTS_CAP 1024
	UI_Layout layouts[LAYOUTS_CAP];
} UI_Context;

// Methods of UI_Context
UI_Context UI_Context_make(UI_Layout_kind kind);


/*
 * Remove Prefix
 */
#ifdef UI_REMOVE_PREFIX
#define Vector2f UI_Vector2f
#define UI_Layout_kind Layout_kind
#define UI_Layout Layout
#define UI_Layout_make Layout_make
#define UI_Layout_available_pos Layout_available_pos
#define UI_Layout_push_widget Layout_push_widget
#define UI_Context Context
#define UI_Context_make Context_make
#endif

#endif

//////////////////////////////////////////////////
#ifdef UI_IMPLEMENTATION

// Methods of UI_Layout
UI_Layout UI_Layout_make(UI_Layout_kind kind) {
	UI_Layout l = {
		.kind = kind;
	};
	return l;
}

// WIP
UI_Vector2f  UI_Layout_available_pos(UI_Layout* layout) {
	switch (layout->kind) {
	}
}

void      UI_Layout_push_widget(UI_Layout* layout, UI_Vector2f size);

// Methods of UI_Context
UI_Context UI_Context_make(UI_Layout_kind kind);

#endif

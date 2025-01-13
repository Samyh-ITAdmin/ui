#define UI_IMPLEMENTATION
#define UI_REMOVE_PREFIX
#include <ui.h>

int main(void) {
	UI_ASSERT(1 == 0, "Now, this should fail!");
	/*UI_Context ctx = UI_make_context(UI_LAYOUT_KIND_VERT);*/
	return 0;
}

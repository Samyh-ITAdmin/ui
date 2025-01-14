#define UI_IMPLEMENTATION
#define UI_REMOVE_PREFIX
#include <ui.h>

int main(void) {
	UI_Context ctx = UI_Context_make();

	UI_log_error("This is what an error looks like %d, %c", 10, 'A');
	UI_log_info("This is what an info looks like %d, %f", 56, 1.04554);
	size_t u = 65535;
	UI_log_warning("This is what an warning looks like %p, %zu", &ctx, u);

	UI_Context_free(&ctx);
	return 0;
}

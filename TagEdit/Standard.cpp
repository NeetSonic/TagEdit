#include "Standard.h"
CFont g_sysFont;
void SetSysFont() {
	g_sysFont.CreatePointFont(SYS_FONT_SIZE, SYS_FONT_TYPE);
}
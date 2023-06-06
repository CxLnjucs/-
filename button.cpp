#include"button.h"
Button::Button(int _x, int _y, int w, int h, COLORREF c, std::string str) {
	x = _x;
	y = _y;
	wight = w;
	height = h;
	color = c;
	int textlen = strlen(str.data()) + 1;
	button_text = new char[textlen];
	strcpy_s(button_text, textlen, str.data());
}

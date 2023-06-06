#pragma once
#include<easyx.h>
#include<string>
class Button {
public:
	int x;
	int y;
	int wight;
	int height;
	COLORREF color;
	char* button_text;
	Button(int x, int y, int w, int h, COLORREF c, std::string str);
};
void drawButton(Button temp);
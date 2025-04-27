#include<iostream>
#include"ViewLayer.h"

int main()
{
	ViewLayer v;
	initgraph(1200, 900, EW_SHOWCONSOLE);
	AddFontResourceEx(L"三极萌萌简体.ttf", FR_PRIVATE, NULL);		//加载字体资源
	v.Init();
	while (true) {
		v.draw();
	}
	RemoveFontResourceEx(L"三极萌萌简体.ttf", FR_PRIVATE, NULL);		//释放字体资源
}
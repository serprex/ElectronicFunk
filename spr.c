#include "ef.h"
struct spr spr[LSPR]={
	{0,2032,6,16},
	{30,2042,16,6},
	{30,2034,8,8},
	{0},
	{0},
};
void drawRect_(int x,int y,int w,int h,float tx,float ty,float tw,float th){
	glTexCoord2f(tx,ty);
	glVertex2i(x,y);
	glTexCoord2f(tx+tw,ty);
	glVertex2i(x+w,y);
	glTexCoord2f(tx+tw,ty+th);
	glVertex2i(x+w,y+h);
	glTexCoord2f(tx,ty+th);
	glVertex2i(x,y+h);
}
void drawRect(int x,int y,int w,int h,float tx,float ty,float tw,float th){
	glBegin(GL_QUADS);
	drawRect_(x,y,w,h,tx,ty,tw,th);
	glEnd();
}
void drawSpr(sprid s,int x,int y,int f,int h){
	drawRect(x+(h?0:spr[s].w),y,h?spr[s].w:-spr[s].w,spr[s].h,spr[s].x/2048.+spr[s].w*f/2048.,spr[s].y/2048.,spr[s].w/2048.,spr[s].h/2048.);
}
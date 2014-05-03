#include "ef.h"
static GLuint Stx;
GLFWwindow*wnd;
struct spr spr[LSPR]={
	{0,2032,6,16},
	{30,2042,16,6},
	{61,2032,6,16},
	{91,2042,16,6},
	{30,2034,8,8},
	{122,2032,16,16},
	{185,2035,12,7},
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
	x-=Wx;
	y-=Wy;
	drawRect(h?x:x+spr[s].w,y,spr[s].w*(h?1:-1),spr[s].h,spr[s].x/2048.+spr[s].w*f/2048.,spr[s].y/2048.,spr[s].w/2048.,spr[s].h/2048.);
}
void sprInit(){
	GLubyte Sask[256][1024];
	for(int y=0;y<128;y++)
		for(int x=0;x<512;x++){
			double xy=sin(M_PI*x/1024)*sin(M_PI*y/256);
			Sask[y][x]=Sask[y][1023-x]=Sask[255-y][x]=Sask[255-y][1023-x]=255-255*(xy+(1-xy*xy)/3);
		}
	glfwInit();
	wnd=glfwCreateWindow(1024,256,"",0,0);
	glfwMakeContextCurrent(wnd);
	glOrtho(0,1024,256,0,1,0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glGenTextures(2,&Stx);
	glBindTexture(GL_TEXTURE_2D,Stx+1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,1024,256,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,Sask);
	glBindTexture(GL_TEXTURE_2D,Stx);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,2048,2048,0,GL_RGBA,GL_UNSIGNED_BYTE,S);
}
void sprEnd(){
	glBindTexture(GL_TEXTURE_2D,Stx+1);
	glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	glBlendFunc(GL_ONE,GL_ONE);
	drawRect(0,0,1024,256,0,0,1,1);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glBindTexture(GL_TEXTURE_2D,Stx);
	glfwSwapBuffers(wnd);
	double gT=1./59-timesince();
	if(gT>0&&!glfwGetKey(wnd,GLFW_KEY_SPACE))sleepd(gT);
	else printf("%f\n",-gT);
	inittime(0);
	glfwPollEvents();
	if(glfwGetKey(wnd,GLFW_KEY_ESCAPE)||glfwWindowShouldClose(wnd))exit(0);
}
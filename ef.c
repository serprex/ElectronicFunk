#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <GL/glfw.h>
#include "tgen.h"
#define LEN(x) (sizeof(x)/sizeof((x)[0]))
#define SQR(x) ((x)*(x))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)>(y)?(y):(x))
#define case(x) break;case x:
#define else(x) else if(x)
#include <stdio.h>
GLuint Ts[TLEN];
int Tbg;

void bindTex(GLuint i){
	glBindTexture(GL_TEXTURE_2D,Ts[i]);
}
int makeTex(int id,const void*px,int w,int h,int f){
	bindTex(id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,f,w,h,0,f,GL_UNSIGNED_BYTE,px);
}
#define XY(X,Y){*x=(X)/8.;*y=(Y)/8.;return;}
#define TXY(C,X,Y) case C:*x=X/8.;*y=Y/8.;return;
void Tfontxy(char c,float*x,float*y){
	if(c>='0'&&c<=';')
		XY(c&3,c>>2&3)
	if(c>='a'&&c<='z')
		XY(c-'a'&7,3+(c-'a'>>3))
	if(c>='A'&&c<='Z')
		XY(c-'A'&7,3+(c-'A'>>3))
	if(c=='"')c='\'';
	else if(c==',')c='.';
	for(int i=0;i<8;i++)
		if(c=="'%&?/()."[i])
			XY(4+(i&3),(i&4)>>2)
}

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

void drawRect(int i,int x,int y,int w,int h,float tx,float ty,float tw,float th){
	bindTex(i);
	glBegin(GL_QUADS);
	drawRect_(x,y,w,h,tx,ty,tw,th);
	glEnd();
}

void drawText(int x,int y,char*s){
	int xx=x;
	glBegin(GL_QUADS);
	for(;*s;s++){
		if(!isspace(*s)){
			float tx,ty;
			Tfontxy(*s,&tx,&ty);
			drawRect_(x,y,8,8,tx,ty,1./8,1./8);
		}
		if(*s=='\n'||x>=376){
			x=xx;
			if((y+=8)>512)break;
		}else x+=8;
	}
	glEnd();
}

int main(int argc,char**argv){
	glfwInit();
	glfwDisable(GLFW_AUTO_POLL_EVENTS);
	glfwOpenWindow(384,512,0,0,0,0,0,0,GLFW_WINDOW);
	glOrtho(0,384,512,0,1,-1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glGenTextures(TLEN,Ts);
	makeTex(fontT,font,fontW,fontH,fontF);
	Tbg=-1;
	int t=0;
	srand(glfwGetTime()*10e5);
	for(;;){
		glClear(GL_COLOR_BUFFER_BIT);
		t++;
		if(t>>11!=Tbg){
			Tbg=t>>11;
			makeTex(_T,_+Tbg*196608,256,256,_F);
			makeTex(_T+1,_+(Tbg+1)*196608,256,256,_F);
			makeTex(_T+2,_+(Tbg+2)*196608,256,256,_F);
		}
		drawRect(_T,0,-(t/8&255),256,256,0,0,1,1);
		drawRect(_T+1,0,-(t/8&255)+256,256,256,0,0,1,1);
		drawRect(_T+2,0,-(t/8&255)+512,256,256,0,0,1,1);
		bindTex(fontT);
		glfwSwapBuffers();
		double gT=1./59-glfwGetTime();
		if(gT>0)glfwSleep(gT);
		else printf("%f\n",-gT);
		glfwSetTime(0);
		glfwPollEvents();
		if(glfwGetKey(GLFW_KEY_ESC)||!glfwGetWindowParam(GLFW_OPENED))return 0;
	}
}
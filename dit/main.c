#include "../ef.h"
#include <stdio.h>
qtree*getqtree();
float Wy = 100;
int Wx = 100,t;
int curspr = 0;
void saveLevel(FILE*f,qtree*q){
	for(int i=0;i<4;i++)
		if (q->q[i])saveLevel(f, q->q[i]);
	for(obj*o=q->o,*no;o;o=no){
		no=o->o;
		if(o->t == Man)fputs("P=",f);
		if (o->d[0]){
			fprintf(f,"memcpy(omake(%d,%d,%d,%d,%d,%d)->d,\"",o->d[0],o->t,o->x,o->y,o->w,o->h);
			for(int i=1;i<o->d[0];i++)fprintf(f,"\\x%.2x", o->d[i]);
			fprintf(f,"\",%d);",o->d[0]);
		}else fprintf(f,"omake(0,%d,%d,%d,%d,%d);",o->t,o->x,o->y,o->w,o->h);
	}
}
void rmatxy(qtree*q,uint16_t x, float y){
	for(int i=0;i<4;i++)
		if (q->q[i])rmatxy(q->q[i],x,y);
	for(obj*o=q->o,*no;o;o=no){
		no=o->o;
		if (o->x<x && o->x+o->w>x && o->y<y && o->y+o->h>y){
			qtrm(o);
		}
	}
}
void mousecb(GLFWwindow*wnd, int b, int a, int m){
	if (a == GLFW_PRESS){
		double mx,my;
		glfwGetCursorPos(wnd, &mx, &my);
		if (b == GLFW_MOUSE_BUTTON_LEFT){
			omake(1, curspr, Wx+mx, Wy+my, 16, 16)->d[0]=0;
			printf("%d %f %f\n", curspr, mx, my);
		} else if (b == GLFW_MOUSE_BUTTON_RIGHT){
			rmatxy(getqtree(),mx,my);
		} else if (b == GLFW_MOUSE_BUTTON_MIDDLE){
			printf("%f\t%f\n", mx, my);
		}
	}
}
void keycb(GLFWwindow*wnd, int k, int s, int a, int m){
	if (a == GLFW_PRESS){
		curspr += (k == 'E')-(k == 'Q');
		if (curspr == LLSPR) curspr = 0;
	}
}
void drawSprs(qtree*q){
	for(int i=0;i<4;i++)
		if (q->q[i])drawSprs(q->q[i]);
	for(obj*o=q->o,*no;o;o=no){
		no=o->o;
		printf("%d %d %f\n",o->t,o->x,o->y);
		drawSpr(o->t, o->x, o->y, 0, 0);
	}
}
int main(int argc,char**argv){
	sprInit();
	qtinit();
	obj*P;
	glfwSetMouseButtonCallback(wnd, mousecb);
	glfwSetKeyCallback(wnd, keycb);
	for(;;){
		t++;
		if (glfwGetKey(wnd,'P')){
			FILE*f=fopen("lv.h","w");
			saveLevel(f,getqtree());
			fclose(f);
		}
		if(Wx<0)Wx=0;
		else(Wx>2048*8)Wx=2048*8;
		Wy += (glfwGetKey(wnd,GLFW_KEY_DOWN)-glfwGetKey(wnd,GLFW_KEY_UP))*48;
		Wx += (glfwGetKey(wnd,GLFW_KEY_RIGHT)-glfwGetKey(wnd,GLFW_KEY_LEFT))*48;
		drawRect(0,0,1024,256,Wx/16384.,Wy/16384.,1./16,1./64);
		drawSprs(getqtree());
		uint16_t hudx = 0;
		for(int i=0; i<LSPR; i++){
			drawSpr(i, Wx+hudx, Wy, 0, curspr == i);
			hudx += spr[i].w;
		}
		for(int i=LSPR+1; i<LLSPR; i++){
			drawSpr(i-LSPR-1, Wx+hudx, Wy, 0, curspr == i);
			hudx += spr[i].w;
		}
		sprEnd();
	}
}
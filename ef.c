#include "ef.h"
#define LEN(x) (sizeof(x)/sizeof((x)[0]))
#define SQR(x) ((x)*(x))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)>(y)?(y):(x))
#define case(x) break;case x:
#define else(x) else if(x)
double Wx=0,Px=1800,Wy=0,Py=900,Pya=0,Pj=-1;
int t=0,Pd=1,Pjd=0,Pcrawl;
obj*P;
struct Pmask{
	uint16_t x0,x6;
	uint8_t y0,y15,y16;
}Pmask;
obj*RR;
void Pupmask(){
	memset(&Pmask,0,sizeof(Pmask));
	for(int i=0;i<6;i++){
		if(getbxy(Px+i,Py))Pmask.y0|=1<<i;
		if(getbxy(Px+i,Py+15))Pmask.y15|=1<<i;
		if(getbxy(Px+i,Py+16))Pmask.y16|=1<<i;
	}
	for(int i=0;i<16;i++){
		if(getbxy(Px,Py+i))Pmask.x0|=1<<i;
		if(getbxy(Px+6,Py+i))Pmask.x6|=1<<i;
	}
	int px=P->x,py=P->y;
	P->x=Px;
	P->y=Py;
	qthit(P);
	for(int i=0;i<P->c->n;i++){
		obj*o=P->c->o[i];
		for(int i=0;i<6;i++){
			if(pino(Px+i,Py,o))Pmask.y0|=1<<i;
			if(pino(Px+i,Py+15,o))Pmask.y15|=1<<i;
			if(pino(Px+i,Py+16,o))Pmask.y16|=1<<i;
		}
		for(int i=0;i<16;i++){
			if(pino(Px,Py+i,o))Pmask.x0|=1<<i;
			if(pino(Px+6,Py+i,o))Pmask.x6|=1<<i;
		}
	}
	P->x=px;
	P->y=py;
	qthit(P);
}
int main(int argc,char**argv){
	glfwInit();
	glfwDisable(GLFW_AUTO_POLL_EVENTS);
	glfwOpenWindow(1024,256,0,0,0,0,0,0,GLFW_WINDOW);
	glOrtho(0,1024,256,0,1,-1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	{GLuint Stx;
	glGenTextures(1,&Stx);
	glBindTexture(GL_TEXTURE_2D,Stx);}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,2048,2048,0,GL_RGBA,GL_UNSIGNED_BYTE,S);
	qtinit();
	P=omake(0,Man,Px,Py,6,16);
	obj*R=RR=omake(0,RClean,1700,900,8,8);
	srand(glfwGetTime()*10e5);
	for(;;){
		t++;
		drawRect(0,0,1024,256,Wx/16384.,Wy/16384.,1./16,1./64);
		double oPx=Px,oPy=Py;
		drawSpr(RClean,R->x-Wx,R->y-Wy,!(t&16),0);
		qthit(P);
		if(glfwGetKey(GLFW_KEY_RIGHT))Px++;
		if(glfwGetKey(GLFW_KEY_LEFT))Px--;
		Pupmask();
		if(oPx!=Px){
			Pd=Px-oPx;
			int x=Pd==1?6:0,fy=-1;
			Pcrawl=0;
			for(int y=0;y<16;y++)
				if(nthbit(x?Pmask.x6:Pmask.x0,y)){
					if(fy==-1)fy=y;
					if(!(Pcrawl=!(y>8||Pya)))Px=oPx;
					if(y>4&&fy==y)Pya=fmin(Pya,-1);
				}
		}
		if(Pcrawl){
			Px=(Px+oPx)/2;
			drawSpr(ManCrawl,Px-Wx,Py-Wy+10,!(t&32),Pd==1);
		}else{
			Pjd=0;
			if(glfwGetKey(GLFW_KEY_UP)){
				for(int xx=0;xx<6;xx+=5)
					if(nthbit(Pmask.y16,xx)){
						Pj=fmax(Pj-.125,-3);
						Pjd=1;
						break;
					}
			}
			if(Pj<-1){
				if(Pjd)Px=oPx;
				else{
					Pya=Pj;
					Pj=-1;
				}
			}
			Pya=fmin(Pya+.09375,3);
			Py+=Pya;
			Pupmask();
			for(int x=0;x<6;x+=5){
				if(Pya>0&&nthbit(Pmask.y15,x)){
					Pya=0;
					Py=ceil(Py);
					Pupmask();
					for(int x=0;x<6;x+=5)
						if(nthbit(Pmask.y15,x)){
							x=0;
							Py--;
							Pupmask();
						}
					break;
				}
				if(Pya<0&&nthbit(Pmask.y0,x)){
					Pya=0;
					Py=floor(Py);
					for(int x=0;x<6;x+=5)
						if(nthbit(Pmask.y15,x)){
							x=0;
							Py++;
							Pupmask();
						}
					break;
				}
			}
			drawSpr(Man,Px-Wx,Py-Wy,Pya>1.125?4:Pj<-1?3:oPx==Px?0:1+!(t&32),Pd==1);
		}
		qtmove(P,Px,ceil(Py));
		glfwSwapBuffers();
		double gT=1./59-glfwGetTime();
		if(gT>0)glfwSleep(gT);
		else printf("%f\n",-gT);
		glfwSetTime(0);
		glfwPollEvents();
		if(glfwGetKey(GLFW_KEY_ESC)||!glfwGetWindowParam(GLFW_OPENED))return 0;
		if(Px<Wx+512-128)Wx=Px-512+128;
		else if(Px>Wx+512+128)Wx=Px-512-128;
		if(Wx<0)Wx=0;
		else if(Wx>2048*8)Wx=2048*8;
		Wy=Py-160;
	}
}
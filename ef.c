#include "ef.h"
#define LEN(x) (sizeof(x)/sizeof(*(x)))
#define SQR(x) ((x)*(x))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)>(y)?(y):(x))
float Wy=0,Pya=0,Pj=-1;
int Wx=0,t=0,Pd=1,Pjd=0,Pcrawl;
obj*P;
struct{
	union{uint16_t x0;struct{uint8_t x0l,x0h;};};
	union{uint16_t x6;struct{uint8_t x6l,x6h;};};
	uint8_t y0,y15,y16;
}Pmask;
void Pupmask(){
	memset(&Pmask,0,sizeof(Pmask));
	float py=P->y;
	P->y=ceil(P->y);
	Pmask.y0=getbx8y(P->x,P->y)&63;
	Pmask.y15=getbx8y(P->x,P->y+15)&63;
	Pmask.y16=getbx8y(P->x,P->y+16)&63;
	for(int i=0;i<16;i++){
		if(getbxyi(P->x,P->y+i))Pmask.x0|=1<<i;
		if(getbxyi(P->x+6,P->y+i))Pmask.x6|=1<<i;
	}
	qthit(P);
	for(int i=0;i<P->c->n;i++){
		obj*o=P->c->o[i];
		for(int i=0;i<6;i++){
			if(pino(P->x+i,P->y,o))Pmask.y0|=1<<i;
			if(pino(P->x+i,P->y+15,o))Pmask.y15|=1<<i;
			if(pino(P->x+i,P->y+16,o))Pmask.y16|=1<<i;
		}
		for(int i=0;i<16;i++){
			if(pino(P->x,P->y+i,o))Pmask.x0|=1<<i;
			if(pino(P->x+6,P->y+i,o))Pmask.x6|=1<<i;
		}
	}
	P->y=py;
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
	P=omake(0,Man,1800,900,6,16);
	obj*R=omake(0,RClean,1700,900,8,8);
	srand(glfwGetTime()*10e5);
	for(;;){
		t++;
		drawRect(0,0,1024,256,Wx/16384.,Wy/16384.,1./16,1./64);
		//R->x++;
		//R->y++;
		//qtmove(R);
		drawSpr(RClean,R->x-Wx,R->y-Wy,!(t&16),0);
		float oPy=P->y;
		int oPx=P->x;
		P->x+=glfwGetKey(GLFW_KEY_RIGHT)-glfwGetKey(GLFW_KEY_LEFT);
		Pupmask();
		if(oPx!=P->x){
			Pd=P->x>oPx;
			int xm=Pd?Pmask.x6:Pmask.x0;
			if(!(xm&15)&&xm&0xFFF)Pya=fmin(Pya,-1);
		}
		Pcrawl=!Pya&&(Pmask.x0l&&!Pmask.x0h||Pmask.x6l&&!Pmask.x6h);
		while(Pcrawl?Pmask.x0h:Pmask.x0){
			P->x++;
			Pupmask();
		}
		while(Pcrawl?Pmask.x6h:Pmask.x6){
			P->x--;
			Pupmask();
		}
		if(Pcrawl){
			if(t&1)P->x=oPx;
			drawSpr(ManCrawl,P->x-Wx,P->y-Wy+10,!(t&32),Pd);
		}else{
			Pjd=0;
			if(glfwGetKey(GLFW_KEY_UP)&&Pmask.y16){
				Pj=fmax(Pj-.125,-3);
				Pjd=1;
			}
			if(Pj<-1){
				if(Pjd)P->x=oPx;
				else{
					Pya=Pj;
					Pj=-1;
				}
			}
			Pya=fmin(Pya+.09375,3);
			P->y+=Pya;
			Pupmask();
			if(Pya>0&&Pmask.y15){
				Pya=0;
				P->y=ceil(P->y);
				do Pupmask(); while(Pmask.y15&&(P->y--,1));
			}
			if(Pya<0&&Pmask.y0){
				Pya=0;
				P->y=floor(P->y);
				do Pupmask(); while(Pmask.y0&&(P->y++,1));
			}
			drawSpr(Man,P->x-Wx,P->y-Wy,Pya>1.125?4:Pj<-1?3:oPx==P->x?0:1+!(t&32),Pd);
		}
		qtmove(P);
		qtdraw();
		if(P->x<Wx+512-128)Wx=P->x-512+128;
		else(P->x>Wx+512+128)Wx=P->x-512-128;
		if(Wx<0)Wx=0;
		else(Wx>2048*8)Wx=2048*8;
		Wy=P->y-160;
		glfwSwapBuffers();
		double gT=1./59-glfwGetTime();
		if(gT>0&&!glfwGetKey(GLFW_KEY_SPACE))glfwSleep(gT);
		else printf("%f\n",-gT);
		glfwSetTime(0);
		glfwPollEvents();
		if(glfwGetKey(GLFW_KEY_ESC)||!glfwGetWindowParam(GLFW_OPENED)){
			printf("%d %f\n",P->x,P->y);
			return 0;
		}
	}
}
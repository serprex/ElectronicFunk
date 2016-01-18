#include "ef.h"
float Wy,Pya,Pj=-1;
int Wx,t,Pd=1,Pjd,Pcrawl,Pxx;
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
	for(int i=0;i<P->cn;i++){
		obj*o=P->c[i];
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
	sprInit();
	qtinit();
	P=omake(0,Man,1800,900,6,16);
	omake(0,RClean,1700,900,8,8);
	initrand();
	for(;;){
		t++;
		if(P->x<Wx+512-128)Wx=P->x-512+128;
		else(P->x>Wx+512+128)Wx=P->x-512-128;
		if(Wx<0)Wx=0;
		else(Wx>2048*8)Wx=2048*8;
		Wy=P->y-160;
		drawRect(0,0,1024,256,Wx/16384.,Wy/16384.,1./16,1./64);
		qtdraw();
		float oPy=P->y;
		int oPx=P->x;
		Pxx=glfwGetKey(wnd,GLFW_KEY_RIGHT)-glfwGetKey(wnd,GLFW_KEY_LEFT);
		P->x+=Pxx;
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
			if(glfwGetKey(wnd,GLFW_KEY_UP)&&Pmask.y16){
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
			}else if(Pya<0&&Pmask.y0){
				Pya=0;
				P->y=floor(P->y);
				do Pupmask(); while(Pmask.y0&&(P->y++,1));
			}
			drawSpr(Man,P->x,P->y,Pya>1.125?4:Pj<-1?3:oPx==P->x?0:1+!(t&32),Pd);
		}
		qtmove(P);
		qtit();
		sprEnd();
	}
}
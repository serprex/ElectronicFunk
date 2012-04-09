struct qtree;
typedef struct obj{
	struct qtree*q;
	struct obj*o,**c;
	uint8_t t,T,w,h;
	uint16_t x,cn;
	float y;
	uint8_t d[];
}obj;
typedef struct qtree{
	struct qtree*p,*q[4];
	obj*o;
	uint16_t x,y,w;
}qtree;
int getbx8y(int x,int y);
int getbxyi(int x,int y);
int getbxy(int x,float dy);
obj*omake(int sz,int t,int x,float y,int w,int h);
int ohit(obj*a,obj*b);
void qtmove(obj*o);
void qtinit();
void qthit(obj*o);
void qtit();
void qtdraw();
static inline int pino(int x,float y,obj*o){
	return x>=o->x&&x<=o->x+o->w&&y>=o->y&&y<=o->y+o->h;
}
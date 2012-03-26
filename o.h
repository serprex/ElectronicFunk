struct jray;
struct qtree;
typedef struct obj{
	uint8_t t,s,w,h;
	uint16_t x,y;
	struct qtree*q;
	struct jray*c;
	struct obj*o;
	uint8_t d[];
}obj;
typedef struct qtree{
	struct qtree*p;
	struct qtree*q[4];
	obj*o;
	uint16_t x,y,w;
}qtree;
typedef struct jray{
	int n;
	obj*o[];
}jray;
int getbxyi(int x,int y);
int getbxy(int x,double dy);
obj*omake(int sz,int t,int x,int y,int w,int h);
int ohit(obj*a,obj*b);
void qtmove(obj*o);
void qtinit();
void qthit(obj*o);
void qtdraw(int,int);
static inline int pino(int x,int y,obj*o){
	return x>=o->x&&x<=o->x+o->w&&y>=o->y&&y<=o->y+o->h;
}
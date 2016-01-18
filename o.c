#include "ef.h"
static qtree*qroot;
int getbx8y(int x,int y){
	unsigned xx=x&7;
	x>>=3;
	y=(2047-(y>>3))<<8;
	unsigned x7=x&7;
	union{int w;struct{uint8_t lo,hi;};}m;
	m.lo=-!!(mapB[y+(x>>3)]&1<<x7);
	if(!xx)return m.w;
	m.hi=-!!(mapB[y+(x+1>>3)]&1<<(x+1&7));
	return m.w>>xx;
}
int getbxyi(int x,int y)
{
	x>>=3;
	y>>=3;
	return mapB[(2047-y)*256+(x>>3)]&1<<(x&7);
}
int getbxy(int x,float dy)
{
	return getbxyi(x,ceil(dy));
}
int ohit(obj*a,obj*b){
	int ax2=a->x+a->w,ay2=a->y+a->h,bx2=b->x+b->w,by2=b->y+b->h;
	return a!=b&&(pino(a->x,a->y,b)||pino(ax2,a->y,b)||pino(a->x,ay2,b)||pino(ax2,ay2,b)||pino(b->x,b->y,a)||pino(bx2,b->y,a)||pino(b->x,by2,a)||pino(bx2,by2,a)||a->x>b->x&&a->x<bx2&&a->y>b->y&&a->y<by2||b->x>a->x&&b->x<ax2&&b->y>a->y&&b->y<ay2);
}
static void jadd(obj*o,obj*x){
	o->c=realloc(o->c,++o->cn*sizeof(obj*));
	o->c[o->cn-1]=x;
}
static qtree*qtmake(qtree*p,int x,int y,int w){
	qtree*q=calloc(1,sizeof(qtree));
	q->p=p;
	q->x=x;
	q->y=y;
	q->w=w;
	return q;
}
void qtinit(){
	qroot=qtmake(0,0,0,8192);
}
void qtdraw_(qtree*q){
	glBegin(GL_LINES);
	glVertex2f(q->x+q->w-Wx,q->y-Wy);
	glVertex2f(q->x+q->w-Wx,q->y+q->w*2-Wy);
	glVertex2f(q->x-Wx,q->y+q->w-Wy);
	glVertex2f(q->x+q->w*2-Wx,q->y+q->w-Wy);
	glVertex2f(q->x-Wx,q->y-Wy);
	glVertex2f(q->x+q->w*2-Wx,q->y+q->w*2-Wy);
	glVertex2f(q->x+q->w*2-Wx,q->y-Wy);
	glVertex2f(q->x-Wx,q->y+q->w*2-Wy);
	glEnd();
	for(int i=0;i<4;i++)
		if(q->q[i])qtdraw_(q->q[i]);
}
void qtdraw(int Wx,int Wy){
	glDisable(GL_TEXTURE_2D);
	qtdraw_(qroot);
	glEnable(GL_TEXTURE_2D);
}
static void qthit_(qtree*q,obj*o){
	unsigned xw=q->x+q->w,yh=q->y+q->w;
	for(obj*qo=q->o;qo;qo=qo->o)
		if(ohit(o,qo))jadd(o,qo);
	if(o->x<=xw&&o->y<=yh&&q->q[0])qthit_(q->q[0],o);
	if(o->x+o->w>xw&&o->y<=yh&&q->q[1])qthit_(q->q[1],o);
	if(o->x<=xw&&o->y+o->h>yh&&q->q[2])qthit_(q->q[2],o);
	if(o->x+o->w>xw&&o->y+o->h>yh&&q->q[3])qthit_(q->q[3],o);
}
void qthit(obj*o){
	free(o->c);
	o->c=0;
	o->cn=0;
	qthit_(qroot,o);
}
static void qtadd_(qtree*q,obj*o){
	unsigned xw=q->x+q->w,yh=q->y+q->w;
	if(q->w==64||o->x<=xw&&o->x+o->w>xw||o->y<=yh&&o->y+o->h>yh){
		o->o=q->o;
		q->o=o;
		o->q=q;
		return;
	}
	int qid;
	if(o->x<=xw&&o->y<=yh)qid=0;
	else(o->x>xw&&o->y<=yh)qid=1;
	else(o->x<=xw&&o->y>yh)qid=2;
	else qid=3;
	if(!q->q[qid])q->q[qid]=qtmake(q,qid==0||qid==2?q->x:xw,qid==0||qid==1?q->y:yh,q->w/2);
	qtadd_(q->q[qid],o);
}
void qtadd(obj*o){
	qtadd_(qroot,o);
}
void qtrm(obj*o){
	if(o->q->o==o)o->q->o=o->o;
	else{
		obj*qo=o->q->o;
		while(qo->o!=o)qo=qo->o;
		qo->o=o->o;
	}
}
obj*omake(int sz,int t,int x,float y,int w,int h){
	obj*o=malloc(sizeof(obj)+sz);
	o->t=t;
	o->T=t&1;
	o->x=x;
	o->cn=0;
	o->y=y;
	o->w=w;
	o->h=h;
	o->o=0;
	o->c=0;
	memset(&o->d,0,sz);
	qtadd(o);
	return o;
}
static int qtfindqid(qtree*q){
	for(int qid=0;;qid++)
		if(q==q->p->q[qid])return qid;
}
static void qtaddhint(qtree*q,obj*o){
	while(o->x<=q->x||o->y<=q->y||o->x+o->w>q->x+q->w*2||o->y+o->h>q->y+q->w*2)q=q->p;
	qtadd_(q,o);
}
static void qtgchint(qtree*q){
	while(!q->o&&!q->q[0]&&!q->q[1]&&!q->q[2]&&!q->q[3]){
		qtree*p=q->p;
		p->q[qtfindqid(q)]=0;
		free(q);
		q=p;
	}
}
void qtmove(obj*o){
	qtree*q=o->q;
	qtrm(o);
	qtaddhint(q,o);
	qtgchint(q);
	qthit(o);
}
bool mtowards(obj*o,uint16_t tx,uint16_t ty,uint16_t hs,uint16_t vs){
	if (o->x<tx){
		o->x += hs;
	}
	if (o->x>tx){
		o->x -= hs;
		if (o->x<tx) o->x=tx;
	}
	if (o->y<ty){
		o->y += vs;
	}
	if (o->y>ty){
		o->y -= vs;
		if (o->y<ty) o->y=ty;
	}
	return o->x == tx && o->y == ty;
}
void qtit_(qtree*q){
	for(int i=0;i<4;i++)
		if(q->q[i])qtit_(q->q[i]);
	for(obj*o=q->o,*no;o;o=no){
		if(o->T==(t&1))return;
		o->T=t&1;
		no=o->o;
		switch(o->t){
		case(RClean)
			if(o->x<16000&&o->y<16000){
				o->x++;
				o->y++;
				qtmove(o);
			}
			drawSpr(RClean,o->x,o->y,!(t&16),0);
		case(Plat){
			uint16_t n = getdata(o,uint16_t,0),
				hs = getdata(o,uint16_t,4+n*8),
				vs = getdata(o,uint16_t,6+n*8),
				tx = getdata(o,uint16_t,8+n*8),
				ty = getdata(o,uint16_t,10+n*8);
			if (mtowards(o, tx, ty, hs, vs)){
				setdata(o,uint16_t,0,getdata(o,uint16_t,2)==n?0:n+1);
			}
			drawSpr(RClean,o->x,o->y,!(t&16),0);
		}
		}
	}
}
void qtit(){
	qtit_(qroot);
}
qtree*getqtree(){
	// should only be used by level editor
	return qroot;
}
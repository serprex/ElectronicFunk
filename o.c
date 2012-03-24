#include "ef.h"
static qtree*qroot;
int getbxyi(int x,int y)
{
	x>>=3;
	y>>=3;
	return mapB[(2047-y)*256+(x>>3)]&1<<(x&7);
}
int getbxy(int x,double dy)
{
	return getbxyi(x,ceil(dy));
}
int ohit(obj*a,obj*b){
	int ax2=a->x+a->w,ay2=a->y+a->h,bx2=b->x+b->w,by2=b->y+b->h;
	return a!=b&&(pino(a->x,a->y,b)||pino(ax2,a->y,b)||pino(a->x,ay2,b)||pino(ax2,ay2,b)||pino(b->x,b->y,a)||pino(bx2,b->y,a)||pino(b->x,by2,a)||pino(bx2,by2,a)||a->x>b->x&&a->x<bx2&&a->y>b->y&&a->y<by2||b->x>a->x&&b->x<ax2&&b->y>a->y&&b->y<ay2);
}
void orm(obj*qo,obj*o){//UNSAFE
	for(;qo;qo=qo->o)
		if(qo->o==o){
			qo->o=o->o;
			break;
		}
}
void jadd(jray**l,obj*o){
	jray*ll;
	if(*l){
		(*l)->n++;
		ll=*l=realloc(*l,sizeof(jray)+(*l)->n*sizeof(obj*));
	}else{
		ll=*l=malloc(sizeof(jray)+sizeof(obj*));
		ll->n=1;
	}
	ll->o[ll->n-1]=o;
}
static qtree*qtmake(qtree*p){
	qtree*q=calloc(1,sizeof(qtree));
	q->p=p;
	return q;
}
void qtinit(){
	qroot=qtmake(0);
}
static void qthit_(qtree*q,obj*o,unsigned x,unsigned y,unsigned w){
	//printf("!%p %p %d %d %d\n",q,q->o,x,y,w);
	unsigned xw=x+w/2,yh=y+w/2;
	for(obj*qo=q->o;qo;qo=qo->o)
		if(ohit(o,qo))jadd(&o->c,qo);
	if(o->x<=xw&&o->y<=yh&&q->q[0])qthit_(q->q[0],o,x,y,w/2);
	if(o->x+o->w>xw&&o->y<=yh&&q->q[1])qthit_(q->q[1],o,xw,y,w/2);
	if(o->x<=xw&&o->y+o->h>yh&&q->q[2])qthit_(q->q[2],o,x,yh,w/2);
	if(o->x+o->w>xw&&o->y+o->h>yh&&q->q[3])qthit_(q->q[3],o,xw,yh,w/2);
}
void qthit(obj*o){
	o->c=realloc(o->c,sizeof(jray));
	o->c->n=0;
	qthit_(qroot,o,0,0,16384);
}
static void qtadd_(qtree*q,obj*o,unsigned x,unsigned y,unsigned w){
	//printf("\t%d %p %p:%d %d %d\n",o->t,q,q->o,x,y,w);
	unsigned xw=x+w/2,yh=y+w/2;
	if(w==1||o->x<=xw&&o->x+o->w>xw||o->y<=yh&&o->y+o->h>yh){
		//if(q->o==o)return;
		o->o=q->o;
		q->o=o;
		return;
	}
	int qid;
	if(o->x<=xw&&o->y<=yh)qid=0;
	else if(o->x>xw&&o->y<=yh)qid=1;
	else if(o->x<=xw&&o->y>yh)qid=2;
	else if(o->x>xw&&o->y>yh)qid=3;
	if(!q->q[qid])q->q[qid]=qtmake(q);
	qtadd_(q->q[qid],o,qid==0||qid==2?x:xw,qid==0||qid==1?y:yh,w/2);
}
void qtadd(obj*o){
	qtadd_(qroot,o,0,0,16384);
}
static void qtrm_(qtree*q,obj*o,unsigned x,unsigned y,unsigned w){
	unsigned xw=x+w/2,yh=y+w/2;
	if(w==1||o->x<=xw&&o->x+o->w>xw||o->y<=yh&&o->y+o->h>yh){
		if(!q->o)return;
		else if(q->o!=o)orm(q->o,o);
		else q->o=q->o->o;
		return;
	}
	int qid;
	if(o->x<=xw&&o->y<=yh)qid=0;
	else if(o->x>xw&&o->y<=yh)qid=1;
	else if(o->x<=xw&&o->y>yh)qid=2;
	else if(o->x>xw&&o->y>yh)qid=3;
	if(!q->q[qid])return;
	qtrm_(q->q[qid],o,qid==0||qid==2?x:xw,qid==0||qid==1?y:yh,w/2);
}
void qtrm(obj*o){
	qtrm_(qroot,o,0,0,16384);
}
obj*omake(int sz,int t,int x,int y,int w,int h){
	obj*o=malloc(sizeof(obj)+sz);
	o->t=t;
	o->s=sz;
	o->x=x;
	o->y=y;
	o->w=w;
	o->h=h;
	o->o=0;
	o->c=calloc(1,sizeof(jray));
	memset(&o->d,0,sz);
	qtadd(o);
	return o;
}
static void qtaddhint(qtree*q,obj*o,unsigned x,unsigned y,unsigned w){
	if(o->x<=x||o->y<=y||o->x+o->w>x+w||o->y+o->h>y+w){
		int qid=0;
		for(;qid<3;qid++)
			if(q==q->p->q[qid])break;
		qtaddhint(q->p,o,qid==1||qid==3?x-w:x,qid==2||qid==3?y-w:y,w*2);
	}else qtadd_(q,o,x,y,w);

}
static void qtmove_(qtree*q,obj*o,unsigned x,unsigned y,unsigned w,int ox,int oy){
	unsigned xw=x+w/2,yh=y+w/2;
	if(w==1||o->x<=xw&&o->x+o->w>xw||o->y<=yh&&o->y+o->h>yh){
		if(q->o==o)q->o=o->o;
		else orm(q->o,o);
		o->x=ox;
		o->y=oy;
		qtaddhint(q,o,x,y,w);
		return;
	}
	int qid;
	if(o->x<=xw&&o->y<=yh)qid=0;
	else if(o->x>xw&&o->y<=yh)qid=1;
	else if(o->x<=xw&&o->y>yh)qid=2;
	else if(o->x>xw&&o->y>yh)qid=3;
	if(!q->q[qid])return;
	qtmove_(q->q[qid],o,qid==0||qid==2?x:xw,qid==0||qid==1?y:yh,w/2,ox,oy);
}
void qtmove(obj*o,int x,int y){
	if(x!=o->x||y!=o->y)
		qtmove_(qroot,o,0,0,16384,x,y);
	qthit(o);
}
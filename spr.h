typedef enum{Boy,BoyCrawl,Man,ManCrawl,RClean,RScience,RDrill,LSPR}sprid;
extern struct spr{
	uint16_t x,y;
	uint8_t w,h;
}spr[LSPR];
void drawRect_(int x,int y,int w,int h,float tx,float ty,float tw,float th);
void drawRect(int x,int y,int w,int h,float tx,float ty,float tw,float th);
void drawSpr(sprid s,int x,int y,int f,int h);
void sprInit();
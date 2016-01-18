#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include <winix/time.h>
#include <winix/rand.h>
#include <stdio.h>
#include "tgen.h"
#include "bgen.h"
#include "spr.h"
#include "o.h"
static inline int nthbit(int x,int n){
	return x&(1<<n);
}
extern float Wy;
extern int t,Wx;
#define case(x) break;case x:;
#define else(x) else if(x)
#define LEN(x) (sizeof(x)/sizeof(*(x)))
#define SQR(x) ((x)*(x))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)>(y)?(y):(x))
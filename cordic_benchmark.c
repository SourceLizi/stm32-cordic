#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cordic.c"


#define SINCOS_RADSTEP 0.2
#define ATAN_RADSTEP 0.1

#define LOG_TEST_STEP 0.5
#define LOG_TEST_OFFSET 10


int main(int argc, char const *argv[])
{
	float a,zzd,xxd,yyd;
	float s,c,l;

	printf("\n------- CORDIC HYPER vectoring (ln)---------\n");
	for(float a=LOG_TEST_OFFSET+LOG_TEST_STEP;
		a<+LOG_TEST_OFFSET+20*LOG_TEST_STEP;a+=LOG_TEST_STEP) {
		zzd = cordic_logf(a);
		printf("%+f ln:%+f err:%+e\n",a,zzd,zzd-log(a));
	}
	printf("\n------- CORDIC CORRD vectoring (sin,cos)-------\n");
	for(float a=-M_PI; a < M_PI;a+=SINCOS_RADSTEP) {
		cordic_sin_cosf(a,&s,&c);
		printf("%+f sin:%+f err:%+e cos:%f err:%+e\n",a,s,s-sin(a),c,c-cos(a));
	}
	srand((unsigned)time(NULL));

	printf("\n------- CORDIC CORRD vectoring (atan,sqrt)-------\n");
	for(float a=-M_PI/2; a < M_PI/2;a+=ATAN_RADSTEP) {
		l = ((float)rand())/(float)RAND_MAX*2;
		s = l * sin(a);
		c = l * cos(a);
		cordic_atan_sqrtf(s,c,&xxd,&yyd);
		printf("%+f,%+f atan:%+f err:%+e sqrt:%f err:%+e\n",a,l,xxd,xxd-a,yyd,yyd-l);
	}

	return 0;
}
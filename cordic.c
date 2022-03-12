//CORDIC, 26 bits, 24 iterations
// 1.0 = 8388608.000000 multiplication factor
// A = 1.743287 convergence angle (limit is 1.7432866 = 99.9deg)
// F = 1.646760 gain (limit is 1.64676025812107)
// 1/F = 0.607253 inverse gain (limit is 0.607252935008881)
// pi = 3.141593 (3.1415926536897932384626)

#define CORDIC_A 1.743287 // CORDIC convergence angle A
#define CORDIC_F 0x00D2C90A // CORDIC gain F
#define CORDIC_1F 0x004DBA77 // CORDIC inverse gain 1/F
#define CORDIC_HALFPI 0x00C90FDB
#define CORDIC_PI 0x01921FB5
#define CORDIC_TWOPI 0x03243F6B
#define CORDIC_MUL 8388608.000000 // CORDIC multiplication factor M = 2^23
#define CORDIC_MAXITER 24

int CORDIC_ZTBL[] = {
 0x006487ED, 0x003B58CE, 0x001F5B76, 0x000FEADD, 0x0007FD57, 0x0003FFAB, 0x0001FFF5, 0x0000FFFF, 
 0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100, 
 0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001 };
/*--------------------------------------------------------------------------------------------------------*/
//CORDIC_HYPER, 26 bits, 25 iterations
// 1.0 = 8388608.000000 multiplication factor
// A = 1.118173 convergence angle (limit is 1.1181730 = 64.0deg)
// F = 0.828159 gain (limit is 0.82978162013890)
// 1/F = 1.207497 inverse gain (limit is 1.20513635844646)
// pi = 3.141593 (3.1415926536897932384626)

#define CORDIC_HYPER_A 1.118173 // CORDIC convergence angle A
#define CORDIC_HYPER_F 0x006A0120 // CORDIC gain F
#define CORDIC_HYPER_1F 0x009A8F44 // CORDIC inverse gain 1/F
#define CORDIC_HYPER_HALFPI 0x00C90FDB
#define CORDIC_HYPER_PI 0x01921FB5
#define CORDIC_HYPER_TWOPI 0x03243F6B
#define CORDIC_HYPER_MUL 8388608.000000 // CORDIC multiplication factor M = 2^23
#define CORDIC_HYPER_MAXITER 25

int CORDIC_HYPER_ZTBL[] = {
 0x80000000, 0x00464FAA, 0x0020B15E, 0x00101589, 0x000802AC, 0x00040055, 0x0002000B, 0x00010001, 
 0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100, 
 0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001, 
 0x00000001 };

/*CORDIC code for circular coordinates */
/*--------------------------------------------------------------------------------------------------------*/

// z less than convergence angle (limit is 1.7432866 = 99.9deg) multiplied by M
void CORDIC_rotation_Zto0(int x, int y, int z, int *xx, int *yy){
	int k, tx;
	for (k = 0; k < CORDIC_MAXITER; k++){
		tx = x;
		if (z >= 0)	{ x -= (y >> k); y += (tx >> k); z -= CORDIC_ZTBL[k]; }
		else 		{ x += (y >> k); y -= (tx >> k); z += CORDIC_ZTBL[k]; }
	}
	*xx = x; // x*cos(z)-y*sin(z) multiplied by M and gain F
	*yy = y; // x*sin(z)+y*cos(z) multiplied by M and gain F
}

void CORDIC_vectoring_Yto0(int x, int y, int z, int *xx, int *zz){
	int k, tx;
	for (k = 0; k < CORDIC_MAXITER; k++){
		tx = x;
		if (y <= 0)	{ x -= (y >> k); y += (tx >> k); z -= CORDIC_ZTBL[k]; }
		else 		{ x += (y >> k); y -= (tx >> k); z += CORDIC_ZTBL[k]; }
	}
	*xx = x; // sqrt(x^2+y^2) multiplied by gain F
	*zz = z; // z+atan2(y,x) multiplied by M
}

/*CORDIC code for hyperbolic coordinates */
/*--------------------------------------------------------------------------------------------------------*/

// z less than convergence angle (limit is 1.1181730 = 64.0deg) multiplied by M 

void CORDIC_HYPER_rotation_Zto0(int x, int y, int z, int *xx, int *yy) { 
	int k, k2, tx; 
	for (k=1,k2=4; k<CORDIC_HYPER_MAXITER;) { 
		tx = x; 
		if (z>=0)	{ x += (y>>k); y += (tx>>k); z -= CORDIC_HYPER_ZTBL[k]; } 
		else		{ x -= (y>>k); y -= (tx>>k); z += CORDIC_HYPER_ZTBL[k]; } 
		if(k==k2) k2=k*3+1; else k++; 
	} 
	*xx = x; // x*cosh(z)+y*sinh(z) multiplied by M and gain F 
	*yy = y; // x*sinh(z)+y*cosh(z) multiplied by M and gain F 
}

void CORDIC_HYPER_vectoring_Yto0(int x, int y, int z, int *xx, int *zz) { 
	int k, k2, tx; 
	for (k=1,k2=4; k<CORDIC_HYPER_MAXITER;) {
		tx = x; 
		if (y<=0)	{ x += (y>>k); y += (tx>>k); z -= CORDIC_HYPER_ZTBL[k]; } 
		else 		{ x -= (y>>k); y -= (tx>>k); z += CORDIC_HYPER_ZTBL[k]; } 
		if(k==k2) k2=k*3+1; else k++; 
	}
	*xx = x; // sqrt(x^2+y^2) multiplied by gain F 
	*zz = z; // z+atan2(y,x) multiplied by M
}


/* CORDIC code circular coordinates, specialized for trigonometric functions */
/*--------------------------------------------------------------------------------------------------------*/

// angle is radians multiplied by CORDIC multiplication factor M
// modulus can be set to CORDIC inverse gain 1/F to avoid post-division
void cordic_sin_cos(int a, int m, int *s, int *c)
{
	int k, tx, x = m, y = 0, z = a, fl = 0;
	if (z > +CORDIC_HALFPI)		{ fl = +1; z = (+CORDIC_PI) - z; }
	else if (z < -CORDIC_HALFPI){ fl = +1; z = (-CORDIC_PI) - z; }
	for (k = 0; k < CORDIC_MAXITER; k++){
		tx = x;
		if (z >= 0)	{ x -= (y >> k); y += (tx >> k); z -= CORDIC_ZTBL[k]; }
		else 		{ x += (y >> k); y -= (tx >> k); z += CORDIC_ZTBL[k]; }
	}
	if (fl){ x = -x; }
	*c = x; // m*cos(a) multiplied by gain F and factor M
	*s = y; // m*sin(a) multiplied by gain F and factor M
}

void cordic_atan2_sqrt(int *a, int *m, int y, int x)
{
	int k, tx, z = 0, fl = 0;
	if (x < 0)	{ fl = ((y > 0) ? +1 : -1); x = -x; y = -y; }
	for (k = 0; k < CORDIC_MAXITER; k++){
		tx = x;
		if (y <= 0)	{ x -= (y >> k); y += (tx >> k); z -= CORDIC_ZTBL[k]; }
		else 		{ x += (y >> k); y -= (tx >> k); z += CORDIC_ZTBL[k]; }
	}
	if (fl != 0){ z += fl * CORDIC_PI; }
	*a = z; // radians multiplied by factor M
	*m = x; // sqrt(x^2+y^2) multiplied by gain F
}

void cordic_atan_sqrt(int *a, int *m, int y, int x)
{
	int k, tx, z = 0;
	if (x < 0){ x = -x; y = -y;}
	for (k = 0; k < CORDIC_MAXITER; k++){
		tx = x; 
		if (y <= 0)	{ x -= (y >> k); y += (tx >> k); z -= CORDIC_ZTBL[k]; }
		else 		{ x += (y >> k); y -= (tx >> k); z += CORDIC_ZTBL[k]; }
	}
	*a = z; // radians multiplied by factor M
	*m = x; // sqrt(x^2+y^2) multiplied by gain F
}

void cordic_atan_sqrtf(float yf, float xf, float* ret_atan, float* ret_sqrt){
	int a,m;
	if(yf/xf > ((float)0x3FFFFFFF)/CORDIC_MUL){ 
		cordic_atan_sqrt(&a,&m,xf*CORDIC_MUL,yf*CORDIC_MUL);
		a = CORDIC_HALFPI - a;
	}else{
		cordic_atan_sqrt(&a,&m,yf*CORDIC_MUL,xf*CORDIC_MUL);
	}
	if(ret_atan != 0) *ret_atan = ((float)a)/CORDIC_MUL;
	if(ret_sqrt != 0) *ret_sqrt = ((float)m)/CORDIC_F;
}

void cordic_sin_cosf(float ang, float* ret_sin, float* ret_cos){
	int s,c;
	cordic_sin_cos((int)(ang*CORDIC_MUL),CORDIC_1F,&s,&c);
	if(ret_sin != 0) *ret_sin = ((float)s)/CORDIC_MUL;
	if(ret_cos != 0) *ret_cos = ((float)c)/CORDIC_MUL;
}

float cordic_logf(float e){
	#define FLOAT_EXP_MASK 0x7f800000
	#define FLOAT_MAN_MASK 0x007fffff

	#define LN2 0.69314718055994530942f
	int x,z = 0;
	union{ float f; unsigned int ui; }fi_p;

	fi_p.f = e;
	int exp = (int)((fi_p.ui & FLOAT_EXP_MASK) >> 23) - 127;
	// unsigned int man = (0x00800000 | (fi_p.ui & FLOAT_MAN_MASK));
	// CORDIC_HYPER_vectoring_Yto0(man + CORDIC_HYPER_MUL,man - CORDIC_HYPER_MUL,0,&x,&z);
	unsigned int man = (fi_p.ui & FLOAT_MAN_MASK);
	CORDIC_HYPER_vectoring_Yto0(( man | 0x01000000 ),man,0,&x,&z);
	return (2.0f*((float)z)/CORDIC_HYPER_MUL + ((float)exp)*LN2);
}

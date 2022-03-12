//CORDIC, 268501009 bits, 0 iterations
// 1.0 = 10430.378350 multiplication factor
// A = 0.000000 convergence angle (limit is 1.7432866 = 99.9deg)
// F = 1.000000 gain (limit is 1.64676025812107)
// 1/F = 1.000000 inverse gain (limit is 0.607252935008881)
// pi = 3.141593 (3.1415926536897932384626)

#define CORDIC_A 0.000000 // CORDIC convergence angle A
#define CORDIC_F 0x000028BE // CORDIC gain F
#define CORDIC_1F 0x000028BE // CORDIC inverse gain 1/F
#define CORDIC_HALFPI 0x00004000
#define CORDIC_PI 0x00008000
#define CORDIC_TWOPI 0x00010000
#define CORDIC_MUL 10430.378350 // CORDIC multiplication factor M
#define CORDIC_MAXITER 0

int CORDIC_ZTBL[] = {};


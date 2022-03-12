#include <stdio.h>
#include <math.h>
//#define M_PI 3.1415926536897932384626
int main(int argc, char **argv)
{
    FILE *f;
    char tname[50], cname[10];
    int n, n2, mp2, niter, bits, t;
    double F, A, mul, tmul; // CORDIC gain, convergence angle, multiplication factor
    printf("0)circular, 1)linear, 2)hyperbolic? ");
    scanf("%d", &t);
    switch (t)
    {
    case 0:
        sprintf(cname, "%s", "");
        break;
    case 1:
        sprintf(cname, "%s", "_LIN");
        break;
    case 2:
        sprintf(cname, "%s", "_HYPER");
        break;
    }
    sprintf(tname, "CORDICtable%s.c", cname);
    if (NULL == (f = fopen(tname, "wt")))
    {
        printf("cannot write to %s\n", tname);
        return 0;
    }
    printf("number of bits for mantissa (e.g. 30)? ");
    scanf("%d", &bits);
    printf("0) mul factor is 2^n (easier output scaling), or\n"
           "1) 2pi is 2^n (easier implementation)\n ? ");
    scanf("%d", &mp2);
    printf("suggested multiplication factor ");
    if (mp2 == 0)
    {
        tmul = (double)(1 << (bits - 3));
        printf("2^%d = %f\n", bits - 3, tmul);
    }
    else
    {
        tmul = (double)(1 << (bits - 2)) / M_PI;
        printf("2^%d/pi = %f\n", bits - 2, tmul);
    }
    printf("multiplication factor (0 for suggested)? ");
    scanf("%lf", &mul);
    if (mul < 0.1)
    {
        mul = tmul;
        printf("%f\n", mul);
    }
    else
        mp2 = -1; // custom mul factor
    switch (t)
    {
    case 0:
        for (n = 0; n < bits; n++)
            if ((int)round(atan(pow(2.0, (double)(-n))) * mul) == 0)
                break;
        break;
    case 1:
        for (n = 0; n < bits; n++)
            if ((int)round((pow(2.0, (double)(-n))) * mul) == 0)
                break;
        break;
    case 2:
        for (n = 1, n2 = 4; n < bits;)
        {
            if ((int)round(atanh(pow(2.0, (double)(-n))) * mul) == 0)
                break;
            if (n == n2)
                n2 = 3 * n + 1;
            else
                n++;
        }
        break;
    }
    printf("iterations (up to %d)? ", n);
    scanf("%d", &niter);
    F = 1.0;
    A = 0.0;
    switch (t)
    {
    case 0:
        for (n = 0; n < niter; n++)
        {
            F = F * sqrt(1 + pow(2.0, -2.0 * n));
            A += atan(pow(2.0, (double)(-n)));
        }
        break;
    case 1:
        for (n = 0; n < niter; n++)
        {
            F = F * sqrt(1);
            A += (pow(2.0, (double)(-n)));
        }
        break;
    case 2:
        for (n = 1, n2 = 4; n < niter;)
        {
            F = F * sqrt(1 - pow(2.0, -2.0 * n));
            A += atanh(pow(2.0, (double)(-n)));
            if (n == n2)
                n2 = 3 * n + 1;
            else
                n++;
        }
        break;
    }
    fprintf(f, "//CORDIC%s, %d bits, %d iterations\n", cname, bits, niter);
    fprintf(f, "// 1.0 = %f multiplication factor\n", mul);
    switch (t)
    {
    case 0:
        fprintf(f, "// A = %lf convergence angle "
                   "(limit is 1.7432866 = 99.9deg)\n",
                A);
        fprintf(f, "// F = %lf gain (limit is 1.64676025812107)\n", F);
        fprintf(f, "// 1/F = %lf inverse gain (limit is 0.607252935008881)\n", 1.0 / F);
        break;
    case 1:
        fprintf(f, "// A = %lf convergence angle (limit is 2)\n", A);
        fprintf(f, "// F = %lf gain (limit is 1.0)\n", F);
        fprintf(f, "// 1/F = %lf inverse gain (limit is 1.0)\n", 1.0 / F);
        break;
    case 2:
        fprintf(f, "// A = %lf convergence angle "
                   "(limit is 1.1181730 = 64.0deg)\n",
                A);
        fprintf(f, "// F = %lf gain (limit is 0.82978162013890)\n", F);
        fprintf(f, "// 1/F = %lf inverse gain (limit is 1.20513635844646)\n", 1.0 / F);
        break;
    }
    fprintf(f, "// pi = %lf (3.1415926536897932384626)\n", M_PI);
    fprintf(f, "\n");
    fprintf(f, "#define CORDIC%s_A %f // CORDIC convergence angle A\n", cname, A);
    fprintf(f, "#define CORDIC%s_F 0x%08X // CORDIC gain F\n",
            cname, (int)round(mul * F));
    fprintf(f, "#define CORDIC%s_1F 0x%08X // CORDIC inverse gain 1/F\n",
            cname, (int)round(mul / F));
    fprintf(f, "#define CORDIC%s_HALFPI 0x%08X\n", cname, (int)round(mul * (M_PI / 2.0)));
    fprintf(f, "#define CORDIC%s_PI 0x%08X\n", cname, (int)round(mul * (M_PI)));
    fprintf(f, "#define CORDIC%s_TWOPI 0x%08X\n", cname, (int)round(mul * (2.0 * M_PI)));
    fprintf(f, "#define CORDIC%s_MUL %f // CORDIC multiplication factor M", cname, mul);
    switch (mp2)
    {
    case 0:
        fprintf(f, " = 2^%d\n", bits - 3);
        break;
    case 1:
        fprintf(f, " = 2^%d/pi\n", bits - 2);
        break;
    default:
        fprintf(f, "\n");
        break;
    }
    fprintf(f, "#define CORDIC%s_MAXITER %d\n\n", cname, niter);
    fprintf(f, "int CORDIC%s_ZTBL[] = {", cname);
    for (n = 0; n < niter; n++)
    {
        if ((n % 8) == 0)
            fprintf(f, "\n ");
        switch (t)
        {
        case 0:
            fprintf(f, "0x%08X", (int)round(atan(pow(2.0, (double)(-n))) * mul));
            break;
        case 1:
            fprintf(f, "0x%08X", (int)round((pow(2.0, (double)(-n))) * mul));
            break;
        case 2:
            if (n == 0){
                fprintf(f, "0x00000000");
            }else fprintf(f, "0x%08X", (int)round(atanh(pow(2.0, (double)(-n))) * mul));
            break;
        }
        if (n < (niter - 1))
            fprintf(f, ", ");
        else
            fprintf(f, " ");
    }
    fprintf(f, "};\n\n");
    fclose(f);
    printf("table written to %s\n", tname);
    return 0;
}
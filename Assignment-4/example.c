// ME341A Assignment-4 
// Name: Kartikeya Gupta
// Roll: 190420


// The computer code below uses the Runge-Kutta procedure to solve velocity boundary layer
// and thermal boundary layer equations over a flat plate via the shooting technique. 
// It employs a fourth order Runge-Kutta method for solving ordinary differential equations 
// and a bisection method for finding roots.


#include <stdio.h>
#include <math.h>
// user inputs
#define L 10.0              // length_of the domain
#define N 100000            // number of grid points
#define A 0.0               // guess value of H_at eta = 0
#define B 1.0               // guess value of H_at eta = 0
#define convergence 0.00001 // convergence criteria
#define Pr 1.0
// global variables
float h = L / (N - 1); // step size
// function declaration
float shootingVelocity(float, float[], float[], float[]);
float shootingThermal(float[], float, float[], float[]);
void printVelocityBLData(float[], float[], float[]);
void printThermalBLData(float[], float[]);
// main function
int main()
{
    int iterations = 0;
    float a, b, c_old = 1.0, c, c_new, error, f_a, f_b, f_c;
    float f_array[N - 1], g_array[N - 1], h_array[N - 1], Theta[N - 1], ThetaDes[N - 1];
    printf("********** BOUNDARY LAYER SOLUTION OVER FLAT PLATE*************\n");
    printf("**************** Starting velocity BL calculations ****************\n");
    a = A; // These are guessed values of H provided above
    b = B;

    // bisection method
    do
    {
        iterations++;                                         // counter for number of iterations
        c_new = (a + b) / 2.0;                                // new guess value
        f_a = shootingVelocity(a, f_array, g_array, h_array); // solve using shooting velocity technique
        f_b = shootingVelocity(b, f_array, g_array, h_array);
        f_c = shootingVelocity(c_new, f_array, g_array, h_array);
        if (((f_a - 1.0) * (f_c - 1.0)) == 0) // check where root lies
        {
            a = c_new;
        }
        else
        {
            b = c_new;
        }
        error = fabs((c_new - c_old) / c_old); // calculate error
        c_old = c_new;                         // update guess value
        printf("Iterations = %d\th(0) = %f\n", iterations, c_new);
        // output iterations and improved guess to terminal
    } while (error > convergence);                      // iterate up to convergence
    printf("Correct guess value of h(0) =%f\n", c_new); // output correct guess value of h(0) to terminal
    printf("************* Writing velocity BL data to file **************\n");
    // print velocity boundary layer data to file
    printVelocityBLData(f_array, g_array, h_array);
    iterations = 0;
    printf("\n");
    printf("**************** Starting thermal BL calculations ****************\n");
    printf("*********************For Prandtl no =%f *****************\n", Pr);
    a = A; // Guess value of ThetaDes(0) provided
    b = B;
    c_old = 1.0;
    // bisection method
    do
    {
        iterations++;                                       // counter for number of iterations
        c_new = (a + b) / 2.0;                              // new guess value
        f_a = shootingThermal(f_array, a, Theta, ThetaDes); // solve using shoootingThermal technique
        f_c = shootingThermal(f_array, c_new, Theta, ThetaDes);
        f_b = shootingThermal(f_array, b, Theta, ThetaDes);
        if ((f_a - 1.0) * (f_c - 1.0) > 0) // check where root lies
        {
            a = c_new;
        }
        else
        {
            b = c_new;
        }
        error = fabs((c_new - c_old) / c_old);                          // calculate error
        c_old = c_new;                                                  // update old guess
        printf("Iterations =%d\tThetaDes(0) =%f\n", iterations, c_new); // output iterations and improved guess to terminal
    } while (error > convergence);
    printf("Correct guess value of ThetaDes(0) =%f\n", c_new); // output correct guess value of ThetaDes(0) to terminal
    printf("************* Writing Thermal BL data to file **************\n");
    // print Thermal boundary layer data to file
    printThermalBLData(Theta, ThetaDes);
    printf("******************* Exiting program **********************\n");
    return 0;
}
// shoots using guess h(0) value, returns g(L) value
float shootingVelocity(float h_old, float f_array[], float g_array[], float h_array[])
{
    float
        f_old,
        g_old, f_new, g_new, h_new, k1, k2, k3, k4, l1, l2, l3, l4, m1, m2, m3, m4;
    int i, j;
    f_old = 0;                     // f(0) = 0
    g_old = 0;                     // g(0) = 0
    for (i = 0; i <= (N - 1); i++) // calculate coefficients of Runge-Kutta method
    {
        k1 = h * g_old;
        l1 = h * h_old;
        m1 = h * -0.5 * f_old * h_old;
        k2 = h * (g_old + 0.5 * l1);
        l2 = h * (h_old + 0.5 * m1);
        m2 = h * -0.5 * (f_old + 0.5 * k1) * (h_old + 0.5 * m1);
        k3 = h * (g_old + 0.5 * l2);
        l3 = h * (h_old + 0.5 * m2);
        m3 = h * -0.5 * (f_old + 0.5 * k2) * (h_old + 0.5 * m2);
        k4 = h * (g_old + l3);
        l4 = h * (h_old + m3);
        m4 = h * -0.5 * (f_old + k3) * (h_old + m3);
        // values of f,g,h for next position
        f_new = f_old + (1.0 / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
        g_new = g_old + (1.0 / 6.0) * (l1 + 2.0 * l2 + 2.0 * l3 + l4);
        h_new = h_old + (1.0 / 6.0) * (m1 + 2.0 * m2 + 2.0 * m3 + m4);
        // update f,g,h
        f_old = f_new;
        g_old = g_new;
        h_old = h_new;
        // update f,g,h in array
        f_array[i] = f_new;
        g_array[i] = g_new;
        h_array[i] = h_new;
    }
    return g_new;
}
void printVelocityBLData(float f_array[], float g_array[], float h_array[])
{
    int i;
    // open output files
    FILE *out;
    out = fopen("velocityBLData.txt", "w");
    // fprintf(out,"eta\t\tf\tg\th\n"); //data file variables
    for (i = 0; i < N; i++) // write to file
    {
        fprintf(out, "%f %f %f %f \n", i * h, f_array[i], g_array[i], h_array[i]);
    }
    // close output file
    fclose(out);
}
float shootingThermal(float f_array[], float Y_in, float Theta[], float ThetaDes[])
{
    float Y_old, Y_new, Y1_old, Y1_new, k1, k2, k3, k4, l1, l2, l3, l4;
    int i;
    Y_old = Y_in;                  // Guess value for ThetaDes(0)
    Y1_old = 0.0;                  // Theta(0) = 0
    for (i = 0; i <= (N - 1); i++) // calculate coefficients of Runge-Kutta method
    {
        k1 = -0.5 * Pr * f_array[i] * Y_old;
        k2 = -0.5 * Pr * f_array[i] * (Y_old + (h / 2.0) * k1);
        k3 = -0.5 * Pr * f_array[i] * (Y_old + (h / 2.0) * k2);
        k4 = -0.5 * Pr * f_array[i] * (Y_old + h * k3);
        l1 = Y_old;
        l2 = Y_old + (h / 2.0) * l1;
        l3 = Y_old + (h / 2.0) * l2;
        l4 = Y_old + h * l3;
        // values of Theta, ThetaDes for next position
        Y_new = Y_old + (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
        Y1_new = Y1_old + (h / 6.0) * (l1 + 2.0 * l2 + 2.0 * l3 + l4);
        // update Theta, ThetaDes
        Y_old = Y_new;
        Y1_old = Y1_new;
        // update Theta, ThetaDes in array
        Theta[i] = Y1_new;
        ThetaDes[i] = Y_new;
    }
    return Y1_new;
}
void printThermalBLData(float Theta[], float ThetaDes[])
{
    int i;
    // open output files
    FILE *out;
    out = fopen("thermalBLData.txt", "w");
    // fprintf(out,"eta\tTheta\tThetaDes\n"); //data file variables
    for (i = 0; i < N; i++) // write to file
    {
        fprintf(out, "%f %f %f \n", i * h, Theta[i], ThetaDes[i]);
    }
    // close output file
    fclose(out);
}
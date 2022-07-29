#define USE_MATH_DEFINES
#include <math.h>
#include <iostream>

// Gives a random number between an inputted range
double randnum(double cmin, double cmax)
{
    int cMin = cmin*10000, cMax = cmax*10000;
    double r = rand()%(cMax - cMin+1) + cMin;
    return r /= 10000;
}

// Gives us a random Z-score to be used in our simulation
double NORMINV(double p)
{
    return sqrt(2*log(1/(p*sqrt(2*M_PI))));
}

// Calculates the stock price for each step
double Stochastic(double S, double Mu, double V, double dT)
{
    int c = 1;
    if(rand() % 2 == 0){
        c = -1;
    }

    double A = Mu - pow(V, 2)/2;
    double B = V*c*NORMINV(randnum(0.0005, 0.39));
    return S*exp(dT*(A + B));
}


int main()
{
    // Inputted parameters
    double stock = 100;
    double mu = 0.07;
    double vh = 0.25;
    double t = 3;
    double n = 100;
    double dT = t / n;

    // Simulate stock prices
    for(int i = 0; i < n; ++i){
        std::cout << stock << std::endl;
        stock = Stochastic(stock, mu, vh, dT);
    }

    return 0;
}
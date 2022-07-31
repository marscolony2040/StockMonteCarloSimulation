#define USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

double RationalApproximation(double t)
{
    // Abramowitz and Stegun formula 26.2.23.
    // The absolute value of the error should be less than 4.5 e-4.
    double c[] = {2.515517, 0.802853, 0.010328};
    double d[] = {1.432788, 0.189269, 0.001308};
    return t - ((c[2]*t + c[1])*t + c[0]) / 
               (((d[2]*t + d[1])*t + d[0])*t + 1.0);
}

double NormalCDFInverse(double p)
{
    if (p <= 0.0 || p >= 1.0)
    {
        std::stringstream os;
        os << "Invalid input argument (" << p 
           << "); must be larger than 0 but less than 1.";
        throw std::invalid_argument( os.str() );
    }

    // See article above for explanation of this section.
    if (p < 0.5)
    {
        // F^-1(p) = - G^-1(p)
        return -RationalApproximation( sqrt(-2.0*log(p)) );
    }
    else
    {
        // F^-1(p) = G^-1(1-p)
        return RationalApproximation( sqrt(-2.0*log(1-p)) );
    }
}

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
    //return sqrt(2*log(1/(p*sqrt(2*M_PI))));
    return NormalCDFInverse(p);
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

// Calculates the standard error of the simulated price
double StdErr(std::vector<double> x)
{
    double n = x.size();
    double mean, stdev;
    for(auto & k : x){
        mean += k;
    }
    mean /= n;
    for(auto & k : x){
        stdev += pow((k - mean), 2);
    }
    stdev /= (n - 1);
    return stdev / sqrt(n);
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

    // Holds simulated prices
    std::vector<double> store;

    // Simulate stock prices
    for(int i = 0; i < n; ++i){
        store.push_back(stock);
        stock = Stochastic(stock, mu, vh, dT);
    }

    // Standard Error
    double se = StdErr(store);

    // Critical Value
    double alpha = 0.05;
    double ts = NORMINV(1 - alpha/2.0);

    // Margin of Error
    double moe = ts*se;

    std::cout << "We are 95 percent confident that the stock price" << std::endl;
    std::cout << "will be between " << stock - moe << " and " << stock + moe << std::endl;


    return 0;
}

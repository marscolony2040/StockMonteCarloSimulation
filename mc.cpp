#define USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>

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
    double ts = NORMINV(0.05);

    // Margin of Error
    double moe = ts*se;

    std::cout << "We are 95 percent confident that the option price will be between " << stock - moe << " and " << stock + moe << std::endl;


    return 0;
}

#include <iostream>

double idealGasDensity(double P, double T, double R)
{
    double a = P / (R * T);
    return a;
}

double reynoldsNumber(double U, double L, double rho, double mu)
{
    return (rho * U * L) / mu;
}

int main()
{
    double pressure = 101325.0;
    double temperature = 300.0;
    double R = 287.05; // Specific gas constant for air
    double density = idealGasDensity(pressure, temperature, R);
    std::cout << "Density of air at " << temperature << " K and " 
    << pressure << " Pa is: " << density << " kg/m^3" << std::endl;

    double U=1.0;
    double L=0.1;
    double μ=1.81e-5;

    double Re = reynoldsNumber(U, L, density, μ);
    std::cout << "Reynolds number is: " << Re << std::endl;

    return 0;
}
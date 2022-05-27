#include "ParticleType.hpp"
#include <iostream>


ParticleType::ParticleType(const char* Name, double Mass, int Charge)
    : fName{Name}
    , fMass{Mass}
    , fCharge{Charge}
    {}   

const char* ParticleType::GetName() const {
    return fName;
}
double ParticleType::GetMass() const {
    return fMass;
}
int ParticleType::GetCharge() const {
    return fCharge;
}
double ParticleType::GetWidth() const {
  return 0;
}

void ParticleType::Print() const {
std::cout << "Particle name: "   << fName   << '\n'
            << "Particle mass: "   << fMass   << '\n'
            << "Particle charge: " << fCharge << '\n';
}


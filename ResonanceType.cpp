#include "ResonanceType.hpp"
#include <iostream>


// Constructor /////////////////////////////////////////////////////////////////////
ResonanceType::ResonanceType(const char* Name, double Mass, int Charge, double Width)
: ParticleType(Name, Mass, Charge)
, fWidth{Width}
{}

// Methods /////////////////////////////////////////////////////////////////////////
void ResonanceType::Print() const {
ParticleType::Print();
std::cout << "Resonance width: " << fWidth << '\n';
}

// Getters /////////////////////////////////////////////////////////////////////////
double ResonanceType::GetWidth() const {
return fWidth;
}


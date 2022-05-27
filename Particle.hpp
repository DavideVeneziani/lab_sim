#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "ResonanceType.hpp"

class Particle
{
    public:
    // Constructor /////////////////////////////////////////////////////////////////
    Particle(const char* Name, double Px = 0, double Py = 0, double Pz = 0);
    
    // Default constructor
    Particle();

    // Methods /////////////////////////////////////////////////////////////////////
    static bool AddParticleType(const char* Name, double Mass, int Charge, double Width = 0);

    static void PrintArray();

    void PrintParticle();

    double Momentum() const;
    double Momentum2() const;

    double GetMass() const;
    double GetEnergy() const;

    double InvariantMass(Particle &Particle) const;

    /*p1.Decay2body(p2, p3) simulates p1 decading into p2 and p3
    After call p2 and p3 contain final momentum of decade. Result (int) returns: 
    0 if everythink was succesful n > 1 if something went wrong*/
    int Decay2body(Particle &dau1,Particle &dau2) const;
    
    // Getters /////////////////////////////////////////////////////////////////////
    int GetIndex() const;

    double GetPx() const;
    double GetPy() const;
    double GetPz() const;

    // Setters ///////////////////////////////////////////////////////////////////
    // Manually set this particle index.
    bool SetIndex(int Index);
    bool SetIndex(const char* Name);
    
    void SetP(double px,double py,double pz);

    private:

    // Maximum number of active particles
    static const int fMaxNumParticleType = 10;

    // Number of types in fParticleType array
    static int fNParticleType;

    // Array containing current particle types
    static ParticleType* fParticleType [fMaxNumParticleType];

    // Index of this particle in fParticleTypes array
    int fIndex;

    // Momentum components
    double fPx{0};
    double fPy{0};
    double fPz{0};
    
    // Methods /////////////////////////////////////////////////////////////////////
    // Find a particle type in ParticleTypes_ array by name, return its index.
    // Returns -1 if not found.
    static int FindParticleIndex(const char* Name);

    // Used by Decay2Body to split momentum components
    void Boost(double bx, double by, double bz);

};


#endif
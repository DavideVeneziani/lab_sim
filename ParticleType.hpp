#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP

class ParticleType
{
    public:

    //constructor //////////////////////////////////////////////////////////////////
    ParticleType(const char* Name, double Mass, int Charge);

    //getters //////////////////////////////////////////////////////////////////////
    const char* GetName() const;
    double GetMass() const;
    int GetCharge() const;
    // This is needed for inheritance. Returns zero.
    virtual double GetWidth() const;

    //print function ///////////////////////////////////////////////////////////////
    virtual void Print() const;

    private:
    
    //name, mass, charge ///////////////////////////////////////////////////////////
    const char* fName;
    const double fMass;
    const int fCharge;

  
};
#endif 

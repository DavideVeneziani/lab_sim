#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"

#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>


int Particle::fNParticleType = 0;
ParticleType* Particle::fParticleType[fMaxNumParticleType] = {};

// Constructor /////////////////////////////////////////////////////////////////////
Particle::Particle(const char* Name, double Px, double Py, double Pz)
  : fPx{Px}
  , fPy{Py}
  , fPz{Pz}
    {
    // Try to find particle named Name in ParticleTypes_ array, assign index based on that.
    const int Index = FindParticleIndex(Name);
    if (Index == -1) {
    std::cout << "Error: No particle named " << '\'' << Name << '\'' << " found." << '\n';
    }
  fIndex = Index;
}

Particle::Particle() = default;

// Methods /////////////////////////////////////////////////////////////////////////
bool Particle::AddParticleType(const char* Name, double Mass, int Charge, double Width){
    if (FindParticleIndex(Name) != -1) {
        std::cout << "Warning: a particle with this name exists alredy" << '\n';
        return false;
    }
    if (fNParticleType >= fMaxNumParticleType) { 
        std::cout << "Warning: the particle array is alredy full, no more space available." << '\n';
        return false;
    }
    if (Width == 0) {
        fParticleType[fNParticleType] = new ParticleType(Name, Mass, Charge);
        fNParticleType++;
        return true;
    }
    else {
        fParticleType[fNParticleType] = new ResonanceType(Name, Mass, Charge, Width);
        fNParticleType++;
        return true;
    }

    /*else {for (int i = 0; i < fNParticleType; ++i) {
            if (fParticleType[i] == 0){
                fParticleType[i] = new ParticleType(Name, Mass, Charge);
            }
        return true;
    }*/
}

void Particle::PrintArray(){
    if (fNParticleType == 0){
        std::cout << "Particle Array is empty." << '\n';
    }
    else {
        for (int i = 0; i < fNParticleType; i++){
            fParticleType[i]->Print();
        }
    }
}

void Particle::PrintParticle (){
    std::cout << "Particle's index: " << fIndex << '\n';
    std::cout << "Particle's name: " << fParticleType[fIndex]->GetName() << '\n';
    std::cout << "Px: " << fPx << '\n'
              << "Py: " << fPy << '\n'
              << "Pz: " << fPz << '\n';
}

/*void Particle::PrintParticle (Particle mParticle){
    int i = mParticle.GetIndex();
    std::cout << "Particle's index: " << i << '\n';
    std::cout << "Particle's name: " << fParticleType[i]->GetName() << '\n';
    std::cout << "Px: " << mParticle.fPx << '\n'
              << "Py: " << mParticle.fPy << '\n'
              << "Pz: " << mParticle.fPz << '\n';
}*/


// Check if Name matches any name in fParticleType 
int Particle::FindParticleIndex(const char* Name) {
    for (int i = 0; i < fNParticleType; ++i) {
        if (strcmp(fParticleType[i]->GetName(), Name) == 0) {
        return i;
        }
    }
    // Return -1 if no match.
    return -1;
}

double Particle::Momentum() const {
    return sqrt(pow(fPx,2)+pow(fPy,2)+pow(fPz,2));
}
double Particle::Momentum2() const {
    return pow(sqrt(pow(fPx,2)+pow(fPy,2)+pow(fPz,2)),2);
}

double Particle::GetMass() const {
    return fParticleType[fIndex]->GetMass();
}
double Particle::GetEnergy() const {
    return sqrt(pow(fParticleType[fIndex]->GetMass(),2)+ Momentum2());
}

double Particle::InvariantMass(Particle &Particle) const {
  const double EnergySumSquared = pow(GetEnergy() + Particle.GetEnergy(), 2);
  const double MomentumSumSquared = pow(Momentum() + Particle.Momentum(), 2);

  return sqrt(EnergySumSquared + MomentumSumSquared);
}

int Particle::Decay2body(Particle &dau1,Particle &dau2) const {
  if(GetMass() == 0.0){
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }
  
  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if(fIndex > -1){ // add width effect

    // gaussian random numbers

    float x1, x2, w, y1, y2;
    
    double invnum = 1./RAND_MAX;
    do {
      x1 = 2.0 * rand()*invnum - 1.0;
      x2 = 2.0 * rand()*invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );
    
    w = sqrt( (-2.0 * log( w ) ) / w );
    y1 = x1 * w;
    y2 = x2 * w;

    massMot += fParticleType[fIndex]->GetWidth() * y1;

  }

  if(massMot < massDau1 + massDau2){
    printf("Decayment cannot be preformed because mass is too low in this channel\n");
    return 2;
  }
  
  double pout = sqrt((massMot*massMot - (massDau1+massDau2)*(massDau1+massDau2))*(massMot*massMot - (massDau1-massDau2)*(massDau1-massDau2)))/massMot*0.5;

  double norm = 2*M_PI/RAND_MAX;

  double phi = rand()*norm;
  double theta = rand()*norm*0.5 - M_PI/2.;
  dau1.SetP(pout*sin(theta)*cos(phi),pout*sin(theta)*sin(phi),pout*cos(theta));
  dau2.SetP(-pout*sin(theta)*cos(phi),-pout*sin(theta)*sin(phi),-pout*cos(theta));

  double energy = sqrt(fPx*fPx + fPy*fPy + fPz*fPz + massMot*massMot);

  double bx = fPx/energy;
  double by = fPy/energy;
  double bz = fPz/energy;

  dau1.Boost(bx,by,bz);
  dau2.Boost(bx,by,bz);

  return 0;
}

void Particle::Boost(double bx, double by, double bz)
{

  double energy = GetEnergy();

  //Boost this Lorentz vector
  double b2 = bx*bx + by*by + bz*bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx*fPx + by*fPy + bz*fPz;
  double gamma2 = b2 > 0 ? (gamma - 1.0)/b2 : 0.0;

  fPx += gamma2*bp*bx + gamma*bx*energy;
  fPy += gamma2*bp*by + gamma*by*energy;
  fPz += gamma2*bp*bz + gamma*bz*energy;
}

// Setters /////////////////////////////////////////////////////////////////////////
//Changes the index of a particle (Changes the type of the particle (e.g. from electron to proton))
bool Particle::SetIndex(int Index){
    if (Index < 0 || Index >= fNParticleType || Index >= fMaxNumParticleType){
        std::cout << "Invalid index." << '\n';
        return false;
    }
    else {
        fIndex = Index;
        return true;
    }
}
bool Particle::SetIndex(const char* Name){
    return SetIndex(FindParticleIndex(Name));
}

void Particle::SetP(double Px, double Py, double Pz){
    fPx = Px;
    fPy = Py;
    fPz = Pz;
}


// Getters /////////////////////////////////////////////////////////////////////
int Particle::GetIndex() const {
    return fIndex;
}

double Particle::GetPx() const {
    return fPx;
}
double Particle::GetPy() const {
    return fPy;
}
double Particle::GetPz() const {
    return fPz;
}




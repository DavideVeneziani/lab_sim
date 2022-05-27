#include "ParticleType.hpp"
#include "ResonanceType.hpp"
#include "Particle.hpp"

#include "TRandom2.h"
#include "TRandom3.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"

#include <iostream>
#include <array>
#include <math.h>
#include <random>

//PROBELMI:
//il costruttore di particle = default va bene?

int main() {
    Particle::AddParticleType("Pion+", 0.13957, 1);    //0
    Particle::AddParticleType("Pion-", 0.13957, -1);
    Particle::AddParticleType("Kaon+", 0.49367, 1);    //2
    Particle::AddParticleType("Kaon-", 0.49367, -1);
    Particle::AddParticleType("Proton+", 0.93827, 1);  //4
    Particle::AddParticleType("Proton-", 0.93827, 1);
    Particle::AddParticleType("K*", 0.89166, 0, 0.05); //6

    Particle pVector[120]={};

    Particle kplus  ("Kaon+");
    Particle kminus ("Kaon-");
    Particle pplus  ("Pion+");
    Particle pminus ("Pion-");

    TRandom3* rand = new TRandom3(0);
    TH1F* h1 = new TH1F("h1", "generated particles", 10, 0, 10);

    int counter = 0;
    
    for(int i = 0; i < 1000; ++i){
        for(int i = 0; i < 100; ++i){
            double phi = rand->Rndm()*2*M_PI;
            double theta = rand->Rndm()*M_PI;
            double p = rand->Exp(1);

            double Px=p*cos(phi)*sin(theta);
            double Py=p*sin(phi)*sin(theta);
            double Pz=p*cos(theta);
            pVector[i].SetP(Px, Py, Pz);

            double rnd1 = rand->Rndm()*1000;

            if (rnd1<=400)                  { pVector[i].SetIndex("Pion+");   }
            else if (rnd1>400 && rnd1<=800) { pVector[i].SetIndex("Pion-");   }
            else if (rnd1>800 && rnd1<=850) { pVector[i].SetIndex("Kaon+");   }
            else if (rnd1>850 && rnd1<=900) { pVector[i].SetIndex("Kaon-");   }
            else if (rnd1>900 && rnd1<=945) { pVector[i].SetIndex("Proton+"); }
            else if (rnd1>945 && rnd1<=990) { pVector[i].SetIndex("Proton-"); }
            else if (rnd1>990) { 
                pVector[i].SetIndex("K*");                                  
                if (rnd1>990 && rnd1<=995) { 
                    pVector[i].Decay2body(kminus, pplus); 
                    pVector[100+counter].SetIndex(kminus.GetIndex());
                    pVector[100+counter+1].SetIndex(pplus.GetIndex());
                    pVector[100+counter].SetP(kminus.GetPx(), kminus.GetPy(), kminus.GetPz());
                    pVector[100+counter+1].SetP(pplus.GetPx(), pplus.GetPy(), pplus.GetPz());
                    counter=counter+2;
                    }
                else if (rnd1>995 && rnd1<=1000) { 
                    pVector[i].Decay2body(kplus, pminus); 
                    pVector[100+counter].SetIndex(kplus.GetIndex());
                    pVector[100+counter+1].SetIndex(pminus.GetIndex());
                    pVector[100+counter].SetP(kplus.GetPx(), kplus.GetPy(), kplus.GetPz());
                    pVector[100+counter+1].SetP(pminus.GetPx(), pminus.GetPy(), pminus.GetPz());                
                    counter=counter+2;
                    }
                else {std::cout << "there's a spill in value filtering A" <<'\n';}
                }
                
            else {pVector[i].SetIndex("K*");}  

        }
        for (int i=0; i<(100+counter); ++i){
        int x = pVector[i].GetIndex();
        h1->Fill(x);
        }
    }
    TFile *file = new TFile("GeneratedParticles.root", "RECREATE");
    h1->Write();
    file->Close();
}
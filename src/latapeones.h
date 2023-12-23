#ifndef LATAPEONES_HPP
#define LATAPEONES_HPP


#include "grafo-escena.h"
#include "malla-ind.h"
using namespace std;


class Lata : public NodoGrafoEscena{
    public:
        Lata(const std::string & textureJPG);
};

class LataPeones : public NodoGrafoEscena{
    public:
        LataPeones();
        
};

class PeonMadera : public NodoGrafoEscena {
    public:
        PeonMadera(int nperfiles);
};

class PeonBlanco : public NodoGrafoEscena {
    public:
        PeonBlanco(int nperfiles);
};

class PeonNegro : public NodoGrafoEscena {
    public:
        PeonNegro(int nperfiles);
};

class VariasLatasPeones : public NodoGrafoEscena{
    public:
    VariasLatasPeones();
};



#endif
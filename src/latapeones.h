#ifndef LATAPEONES_HPP
#define LATAPEONES_HPP


#include "grafo-escena.h"
#include "malla-ind.h"
using namespace std;
using namespace glm;


class Lata : public NodoGrafoEscena{
    public:
        Lata(const std::string & textureJPG);
};

class LataPeones : public NodoGrafoEscena{
    public:
        LataPeones();
        
};

class PeonMadera : public NodoGrafoEscena {
    protected:
    mat4 *pm_tras=nullptr;
    public:
        PeonMadera(int nperfiles);
        virtual bool cuandoClick( const glm::vec3 & centro_wc) ;
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
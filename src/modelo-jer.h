//Nombre: María de las Angustias, Apellidos: Izquierdo García, Titulación: GIIADE


#ifndef MODELO_JER_HPP
#define MODELO_JER_HPP

#include "grafo-escena.h"
#include "malla-ind.h"
#include "ig-aux.h"
#include "malla-revol.h"


using namespace std;
using namespace glm;



class Muñeco : public NodoGrafoEscena{

    protected:
    mat4 *pm_rot_gorro=nullptr;
    mat4 *pm_rot_cuerda=nullptr;
    mat4 *pm_rot_yoyo=nullptr;
    mat4 *pm_tras_muñeco=nullptr;

    public:
    unsigned leerNumParametros() const ;
    virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
    void fijarRotacionGorro( const float n);
    void fijarRotacionCuerda( const float n);
    void fijarRotacionYoyo( const float n);
    void fijarTraslacionMuñeco( const float n);
    Muñeco();
};




#endif
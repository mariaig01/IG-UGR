//Nombre: María de las Angustias, Apellidos: Izquierdo García, Titulación: GIIADE



#include "ig-aux.h"
#include "grafo-escena.h"
#include "modelo-jer.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include <glm/gtx/transform.hpp>


using namespace std;
using namespace glm;




// ------------------------MUÑECO-----------------------
Muñeco::Muñeco(){
    ponerNombre("Muñeco");

   //Nodo raíz
    NodoGrafoEscena *muñeco=new NodoGrafoEscena();
    muñeco->ponerIdentificador(0);
    unsigned ind_tras_muñeco = this->agregar(translate(vec3{0.0, 0.0, 0.0}));

    NodoGrafoEscena *cuerpo=new NodoGrafoEscena();
    cuerpo->ponerIdentificador(1);
    cuerpo->agregar(scale(vec3{0.9,0.9,0.9}));
    cuerpo->agregar(translate(vec3{0.0,0.8,0.0}));
    Piramide *piramide=new Piramide();
    piramide->ponerColor({1.0,0.0,0.0});
    cuerpo->agregar(piramide);
    muñeco->agregar(cuerpo);

    NodoGrafoEscena *cabeza=new NodoGrafoEscena();
    cabeza->ponerIdentificador(2);
    cabeza->agregar(scale(vec3(0.5,0.5,0.5)));
    cabeza->agregar(translate(vec3(0,2.6,0)));
    EsferaRadio *esfera=new EsferaRadio(50,100,0.5);
    esfera->ponerColor({1.0,1.0,0.9});
    cabeza->agregar(esfera);
    muñeco->agregar(cabeza);

    
    NodoGrafoEscena *gorro=new NodoGrafoEscena();
    gorro->ponerIdentificador(3);
    unsigned ind_rot_gorro = gorro->agregar(rotate(0.0f, vec3{0.0, 1.0, 0.0}));
    gorro->agregar(scale(vec3(0.3,0.3,0.3)));
    gorro->agregar(translate(vec3(0,5.0,0)));
    Cono *cono=new Cono(2,30);
    cono->ponerColor({0.1,0.0,0.1});
    gorro->agregar(cono);
    

    NodoGrafoEscena *pieizq=new NodoGrafoEscena();
    pieizq->ponerIdentificador(4);
    pieizq->agregar(translate(vec3(-0.15,0.15,0)));
    pieizq->agregar(scale(vec3(0.1,0.15,0.1)));
    Cubo *cubo=new Cubo();
    cubo->ponerColor({1.0,1.0,0.9});
    pieizq->agregar(cubo);
    muñeco->agregar(pieizq);

    NodoGrafoEscena *piedcha=new NodoGrafoEscena();
    piedcha->ponerIdentificador(5);
    piedcha->agregar(translate(vec3(0.15,0.15,0)));
    piedcha->agregar(scale(vec3(0.1,0.15,0.1)));
    Cubo *cubo1=new Cubo();
    cubo1->ponerColor({1.0,1.0,0.9});
    piedcha->agregar(cubo1);
    muñeco->agregar(piedcha);

    NodoGrafoEscena *brazos=new NodoGrafoEscena();
    brazos->ponerIdentificador(6);
    brazos->agregar(translate(vec3(0,0.6,0)));
    brazos->agregar(scale(vec3(0.75,0.05,0.05)));
    Cubo *cubo2=new Cubo();
    cubo2->ponerColor({1.0,1.0,0.9});
    brazos->agregar(cubo2);
    muñeco->agregar(brazos);

    NodoGrafoEscena *cuerda=new NodoGrafoEscena();
    cuerda->ponerIdentificador(7);
    unsigned ind_rot_cuerda = cuerda->agregar(rotate(0.0f, vec3{0.0, 1.0, 0.0}));
    cuerda->agregar(translate(vec3(0.7,0.4,0)));
    cuerda->agregar(rotate(90.0f,vec3(0,1,0)));
    cuerda->agregar(scale(vec3(0.01,0.16,0.01)));
    Cubo *cubo3=new Cubo();
    cubo3->ponerColor({1.0,0.0,1.0});
    cuerda->agregar(cubo3);
    

    NodoGrafoEscena *yoyo=new NodoGrafoEscena();
    yoyo->ponerIdentificador(8);
    unsigned ind_rot_yoyo = yoyo->agregar(rotate(0.0f, vec3{0.0, 1.0, 0.0}));
    yoyo->agregar(translate(vec3(0.5,0.1,0)));
    yoyo->agregar(scale(vec3(0.3,0.3,0.3)));
    yoyo->agregar(new PiramideEstrellaZ(5));
    

   muñeco->agregar(gorro);
   pm_rot_gorro = gorro->leerPtrMatriz(ind_rot_gorro);

   muñeco->agregar(cuerda);
   pm_rot_cuerda = cuerda->leerPtrMatriz(ind_rot_cuerda);

   muñeco->agregar(yoyo);
   pm_rot_yoyo = yoyo->leerPtrMatriz(ind_rot_yoyo);

   
   this->agregar(muñeco);
   pm_tras_muñeco = leerPtrMatriz(ind_tras_muñeco);

}

void Muñeco::fijarRotacionGorro( const float n )
{
   float rot = n*360 ;
   *pm_rot_gorro = rotate(rot, vec3{0.0, 1.0, 0.0});
}

void Muñeco::fijarRotacionCuerda( const float n )
{
   float rot =  sin(2*M_PI*0.7*n)/30;
   *pm_rot_cuerda = rotate(rot, vec3{0.0, 0.0, 1.0});
}

void Muñeco::fijarRotacionYoyo( const float n )
{
   float rot=sin(2*M_PI*0.7*n)/10;
   *pm_rot_yoyo = rotate(rot, vec3{0.0, 0.0, 1.0});
}

void Muñeco::fijarTraslacionMuñeco( const float n )
{
   float rot=sin(2*M_PI*0.7*n)/10;
   *pm_tras_muñeco = translate(vec3{0.0, 0.0, rot});
}

unsigned Muñeco::leerNumParametros() const {

    return 4;
}

void Muñeco::actualizarEstadoParametro( const unsigned iParam, const float tSec ){
    assert(iParam < leerNumParametros()); 

   switch(iParam)
   {
      case 0:
         fijarTraslacionMuñeco(tSec);
         break;
      case 1:
         fijarRotacionGorro(tSec);
         break;
      case 2:
         fijarRotacionCuerda(tSec);
         break;
      case 3:
         fijarRotacionYoyo(tSec);
         break;
   }
}
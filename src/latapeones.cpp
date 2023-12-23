#include "latapeones.h"
#include "ig-aux.h"
#include "grafo-escena.h"
#include "malla-ind.h"
#include "malla-revol.h"

using namespace std;



Lata::Lata(const std::string & textureJPG){

   // Tapa superior
    NodoGrafoEscena *nodo_psup = new NodoGrafoEscena();
    Material *material_psup = new Material(0.4, 0.4, 0.8, 25.0);
    MallaRevol *psup = new MallaRevolPLY("lata-psup.ply", 50);
    psup->ponerColor({0.7, 0.7, 0.7});
    nodo_psup->agregar(material_psup);
    nodo_psup->agregar(psup);

    // Perfil parte central
    NodoGrafoEscena *nodo_pcue = new NodoGrafoEscena();
    Textura *textura_pcue = new Textura(textureJPG);
    Material *material_pcue = new Material(textura_pcue, 0.6, 0.6, 0.9, 50.0);
    MallaRevol *pcue = new MallaRevolPLY("lata-pcue.ply", 50);
    nodo_pcue->agregar(material_pcue);
    nodo_pcue->agregar(pcue);

    // Base inferior
    NodoGrafoEscena *nodo_binf = new NodoGrafoEscena();
    Material *material_binf = new Material(0.4, 0.4, 0.8, 25.0);
    MallaRevol *binf = new MallaRevolPLY("lata-pinf.ply", 50);
    binf->ponerColor({0.7, 0.7, 0.7});
    nodo_binf->agregar(material_binf);
    nodo_binf->agregar(binf);

    // Agregamos las tres partes que forman la lata
    agregar(nodo_psup);
    agregar(nodo_binf);
    agregar(nodo_pcue);

}

PeonBlanco::PeonBlanco(int nperfiles){
    NodoGrafoEscena *peon = new NodoGrafoEscena();
    peon->ponerColor({0.8,0.8,0.8});
    MallaRevolPLY * peon_blanco = new MallaRevolPLY("peon.ply",nperfiles);
    Material * material = new Material(0.1f,1.0f,0.0f,1.0f);

    peon->agregar(scale(vec3(0.2,0.2,0.2)));
    peon->agregar(translate(vec3{2.5,1.3,2.5}));
    peon->agregar( material );
    peon->agregar( peon_blanco );
    // Asignar nombre
    ponerNombre("Peón blanco");
    ponerIdentificador(4);
    agregar(peon);

}

//material especular sin apenas reflectividad difusa 
PeonNegro::PeonNegro(int nperfiles){
ponerColor({0.0,0.0,0.0});
    NodoGrafoEscena *peon = new NodoGrafoEscena();
    MallaRevolPLY * peon_negro = new MallaRevolPLY("peon.ply",nperfiles);
    Material * material = new Material(0.4f,0.1f,1.0f,5.0f);

    peon->agregar(scale(vec3(0.2,0.2,0.2)));
    peon->agregar(translate(vec3{5,1.3,2.5}));
    peon->agregar( material );
    peon->agregar( peon_negro );   

    // Asignar nombre
    peon->ponerNombre("Peón negro");
    peon->ponerIdentificador(6);
    agregar( peon );

}

/*con la textura de madera difuso-especular, usando generación automática de coordenadas 
de textura, de forma que la coordenada s de textura es proporcional a la
coordenada X de la posición, y la coordenada t a Y*/
PeonMadera::PeonMadera(int nperfiles){

    NodoGrafoEscena *peon = new NodoGrafoEscena();
    peon->ponerColor({0.8,0.8,0.8});
    MallaRevolPLY * peon_madera = new MallaRevolPLY("peon.ply",nperfiles);
    Textura * textura = new Textura("text-madera.jpg");
    Material * material = new Material(textura,0.6f, 0.5f, 1.5f, 100.0);

    peon->agregar(scale(vec3(0.2,0.2,0.2)));
    peon->agregar(translate(vec3{0,1.3,2.5}));
    peon->agregar( material );
    peon->agregar( peon_madera);
    // Asignar nombre
    peon->ponerNombre("Peón madera");
    peon->ponerIdentificador(5);
    agregar(peon);

}


LataPeones::LataPeones(){

    NodoGrafoEscena *latapeones =  new NodoGrafoEscena();
    latapeones->agregar(new Lata("lata-coke.jpg"));
    latapeones->agregar(new PeonNegro(50));
    latapeones->agregar(new PeonMadera(50));
    latapeones->agregar(new PeonBlanco(50));

    agregar(latapeones);
}
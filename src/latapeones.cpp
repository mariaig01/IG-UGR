#include "latapeones.h"
#include "ig-aux.h"
#include "grafo-escena.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include "escena.h"
#include <iostream>
#include "camara.h"

using namespace std;



Lata::Lata(const std::string & textureJPG){

   // Tapa superior
    NodoGrafoEscena *lata = new NodoGrafoEscena();
    NodoGrafoEscena *nodo_psup = new NodoGrafoEscena();
    Material *material_psup = new Material(0.4, 0.4, 0.8, 25.0);
    MallaRevol *psup = new MallaRevolPLY("lata-psup.ply", 50);
    psup->ponerColor({0.7, 0.7, 0.7});
    psup->ponerIdentificador(25);
    nodo_psup->agregar(material_psup);
    nodo_psup->agregar(psup);
    nodo_psup->ponerIdentificador(26);

    // Perfil parte central
    NodoGrafoEscena *nodo_pcue = new NodoGrafoEscena();
    Textura *textura_pcue = new Textura(textureJPG);
    Material *material_pcue = new Material(textura_pcue, 0.6, 0.6, 0.9, 50.0);
    MallaRevol *pcue = new MallaRevolPLY("lata-pcue.ply", 50);
    nodo_pcue->agregar(material_pcue);
    nodo_pcue->agregar(pcue);
    nodo_pcue->ponerIdentificador(27);

    // Base inferior
    NodoGrafoEscena *nodo_binf = new NodoGrafoEscena();
    Material *material_binf = new Material(0.4, 0.4, 0.8, 25.0);
    MallaRevol *binf = new MallaRevolPLY("lata-pinf.ply", 50);
    binf->ponerColor({0.7, 0.7, 0.7});
    binf->ponerIdentificador(28);
    nodo_binf->agregar(material_binf);
    nodo_binf->agregar(binf);
    nodo_binf->ponerIdentificador(29);


    // Agregamos las tres partes que forman la lata
    lata->agregar(nodo_psup);
    lata->agregar(nodo_binf);
    lata->agregar(nodo_pcue);
    lata->ponerIdentificador(20);
    agregar(lata);

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
    ponerIdentificador(2);
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
    peon->ponerIdentificador(3);
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
    peon->ponerIdentificador(4);
    agregar(peon);

}
/*
Se deben de implementar el método cuandoClick para los tres peones de esta escena, de forma
que cuando se haga click en un peón, se ejecute código que cambie la matriz de transformación
de dicho peón y se desplace cierta distancia en la dirección y sentido de la rama positiva del eje
Z. Para eso se debe de definir una clase específica para estos peones, que puede ser derivada de
NodoGrafoEscena, pero que tendrá esa matriz de desplazamiento y además tendrá redefinido el
método cuandoClick*/
bool PeonMadera::cuandoClick( const glm::vec3 & centro_wc){
    float rot=sin(2*M_PI*0.7)/10;
   *pm_tras = translate(vec3{0.0, 0.0, rot});

   assert( apl != nullptr );
   Escena * escena = apl->escenas[apl->ind_escena_act] ; assert( escena != nullptr );

   cout << "Ejecutando método 'cuandoClick' por defecto." << endl ;
   cout << "Click realizado sobre el objeto: '" << leerNombre() << "'" << endl ;

   // COMPLETAR: práctica 5: apuntar la cámara actual a este objeto 
   //
   // Recuperar la camara actual de la escena (puntero 'escena') con el método 'camaraActual', 
   // y hacer que apunte al punto central de este objeto (parámetro 'centro_wc'), usando 
   // el método 'mirarHacia' de esa cámara.
   escena->camaraActual()->mirarHacia(centro_wc);
   // ...

   return true ;

}


LataPeones::LataPeones(){

    NodoGrafoEscena *latapeones =  new NodoGrafoEscena();
    latapeones->agregar(new Lata("lata-coke.jpg"));
    latapeones->agregar(new PeonNegro(50));
    latapeones->agregar(new PeonMadera(50));
    latapeones->agregar(new PeonBlanco(50));

    agregar(latapeones);
}

VariasLatasPeones::VariasLatasPeones(){
    NodoGrafoEscena *variaslataspeones = new NodoGrafoEscena();
    variaslataspeones->ponerNombre("VariasLatasPeones");

    NodoGrafoEscena *latacoke = new Lata("lata-coke.jpg");
    latacoke->ponerNombre("Lata de Coca-Cola");
    latacoke->ponerIdentificador(200);
    variaslataspeones->agregar(latacoke);

    variaslataspeones->agregar(translate(vec3{0.7,0,0}));

    NodoGrafoEscena *latapepsi = new Lata("lata-pepsi.jpg");
    latapepsi->ponerNombre("Lata de Pepsi");
    latapepsi->ponerIdentificador(201);
    variaslataspeones->agregar(latapepsi);
    
    variaslataspeones->agregar(translate(vec3{0.7,0,0}));

    NodoGrafoEscena *lataugr = new Lata("window-icon.jpg");
    lataugr->ponerNombre("Lata de la UGR");
    lataugr->ponerIdentificador(202);
    variaslataspeones->agregar(lataugr);

    variaslataspeones->agregar(translate(vec3{-1.4,0,0}));

    NodoGrafoEscena *peonblanco = new PeonBlanco(100);
    peonblanco->ponerNombre("Peón blanco");
    peonblanco->ponerIdentificador(203);
    variaslataspeones->agregar(peonblanco);

    NodoGrafoEscena *peonnegro = new PeonNegro(100);
    peonnegro->ponerNombre("Peón negro");
    peonnegro->ponerIdentificador(204);
    variaslataspeones->agregar(peonnegro);

    NodoGrafoEscena *peonmadera = new PeonMadera(100);
    peonmadera->ponerNombre("Peón de madera");
    peonmadera->ponerIdentificador(205);
    variaslataspeones->agregar(peonmadera);

    agregar(variaslataspeones);

}
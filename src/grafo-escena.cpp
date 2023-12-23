// *********************************************************************
// ** Nombre: María de las Angustias, Apellidos: Izquierdo García, Titulación: GIIADE
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Gestión de grafos de escena (implementación)
// ** Copyright (C) 2016-2023 Carlos Ureña
// **
// ** Implementación de: 
// **     + Clase 'NodoGrafoEscena' (derivada de 'Objeto3D')
// **     + Clase 'EntradaNGE' (una entrada de un nodo del grafo de escena)
// **     + Tipo enumerado 'TipoEntNGE' (tipo de entradas del nodo del grafo de escena)
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "ig-aux.h"  
#include "grafo-escena.h"
#include "aplicacion-ig.h"
#include "seleccion.h"   // para 'ColorDesdeIdent' 
#include "malla-ind.h"
#include "malla-revol.h"

using namespace std;
using namespace glm;


// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != nullptr );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const glm::mat4 & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new glm::mat4() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != nullptr );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

NodoGrafoEscena::NodoGrafoEscena()
{

}

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL(  )
{
   using namespace std ;
   assert( apl != nullptr );

    // comprobar que hay un cauce y una pila de materiales y recuperarlos.
   Cauce *          cauce           = apl->cauce ;           assert( cauce != nullptr );
   PilaMateriales * pila_materiales = apl->pila_materiales ; assert( pila_materiales != nullptr );

   if (apl->iluminacion)
      pila_materiales->push();

   // COMPLETAR: práctica 3: implementar la visualización del nodo
   //
   // Se deben de recorrer las entradas y llamar recursivamente de visualizarGL, pero 
   // teniendo en cuenta que, al igual que el método visualizarGL de las mallas indexadas,
   // si el nodo tiene un color, debemos de cambiar el color del cauce (y hacer push/pop). 
   // Además, hay que hacer push/pop de la pila de modelado. 
   // Así que debemos de dar estos pasos:
   //
   // 1. Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //     - hacer push del color actual del cauce (con 'pushColor') y después
   //     - fijar el color en el cauce (con 'fijarColor'), usando el color del objeto (se lee con 'leerColor()')

   if(tieneColor()){
      cauce->pushColor();
      cauce->fijarColor(leerColor());
   }

  

   // 2. Guardar copia de la matriz de modelado (con 'pushMM'), 
   cauce->pushMM();

   // 3. Para cada entrada del vector de entradas:
   //     - si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGL'
   //     - si la entrada es de tipo transformación: componer la matriz (con 'compMM')
    for( unsigned i = 0 ; i < entradas.size() ; i++ )
      {
      switch( entradas[i].tipo )
         {
         case TipoEntNGE::objeto : // entrada objeto:
            entradas[i].objeto->visualizarGL();//llamar recursivamente a visualizarGL
         break ;
         case TipoEntNGE::transformacion : // entrada transf.:
            cauce->compMM( *(entradas[i].matriz)); // componer matriz
         break ;
         case TipoEntNGE::material : // si la entrada es de tipo ’material’
            if ( apl->iluminacion ) // y si está activada la iluminación
            pila_materiales->activar( entradas[i].material );
         break ;
      }
   }


   // 4. Restaurar la copia guardada de la matriz de modelado (con 'popMM')
   cauce->popMM();
   

   // 5. Si el objeto tiene color asignado:
   //     - restaurar el color original a la entrada (con 'popColor')
   if(tieneColor()){
      cauce->popColor();
   }



   // COMPLETAR: práctica 4: añadir gestión de los materiales cuando la iluminación está activada    
   //
   // Si 'apl->iluminacion' es 'true', se deben de gestionar los materiales:
   //
   //   1. al inicio, hacer 'push' de la pila de materiales (guarda material actual en la pila)
   //   2. si una entrada es de tipo material, activarlo usando a pila de materiales
   //   3. al finalizar, hacer 'pop' de la pila de materiales (restaura el material activo al inicio)
   if (apl->iluminacion){
      pila_materiales->pop();
   }

}

// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...

void NodoGrafoEscena::visualizarGeomGL(  )
{
   using namespace std ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce; assert( cauce != nullptr );
  
   // COMPLETAR: práctica 3: implementar la visualización del nodo (ignorando colores)
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', pero más simple,
   // Se dan estos pasos:
   //
   // 1. Guardar copia de la matriz de modelado (con 'pushMM'), 
   cauce->pushMM();


   // 2. Para cada entrada del vector de entradas:
   //         - Si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGeomGL'.
   //         - Si la entrada es de tipo transformación: componer la matriz (con 'compMM').

   for( unsigned i = 0 ; i < entradas.size() ; i++ )
      {
      switch( entradas[i].tipo )
         {
         case TipoEntNGE::objeto : // entrada objeto:
            entradas[i].objeto->visualizarGeomGL();//llamar recursivamente a visualizarGeomGL
         break ;
         case TipoEntNGE::transformacion : // entrada transf.:
            cauce->compMM( *(entradas[i].matriz)); // componer matriz
         break ;
      }
   }
   //   3. Restaurar la copia guardada de la matriz de modelado (con 'popMM')
   cauce->popMM();
   // .......

}

// -----------------------------------------------------------------------------
// Visualizar las normales de los objetos del nodo

void NodoGrafoEscena::visualizarNormalesGL(  )
{
   using namespace std ;

   // comprobar que hay un cauce 
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce; assert( cauce != nullptr );
  

   // COMPLETAR: práctica 4: visualizar las normales del nodo del grafo de escena
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', teniendo 
   // en cuenta estos puntos:
   //
   // - usar push/pop de la matriz de modelado al inicio/fin (al igual que en visualizatGL)
   // - recorrer las entradas, llamando recursivamente a 'visualizarNormalesGL' en los nodos u objetos hijos
   // - ignorar el color o identificador del nodo (se supone que el color ya está prefijado antes de la llamada)
   // - ignorar las entradas de tipo material, y la gestión de materiales (se usa sin iluminación)

    cauce->pushMM();

   
   for (size_t i=0; i < entradas.size(); i++)
   {
      switch(entradas[i].tipo)
      {
         case TipoEntNGE::objeto:
            entradas[i].objeto->visualizarNormalesGL();
            break;
         case TipoEntNGE::transformacion:
            cauce->compMM( *(entradas[i].matriz) );
            break;
         case TipoEntNGE::material:
         case TipoEntNGE::noInicializado:
            break;
      }
   }

   cauce->popMM();

   // .......

}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores 
// basados en los identificadores de los objetos
void NodoGrafoEscena::visualizarModoSeleccionGL()
{
   using namespace std ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 5: visualizar este nodo en modo selección.
   //
   // Se debe escribir código para dar estos pasos:
   // 
   // 2. Leer identificador (con 'leerIdentificador'), si el identificador no es -1 
   //      + Guardar una copia del color actual del cauce (con 'pushColor')
   //      + Fijar el color del cauce de acuerdo al identificador, (usar 'ColorDesdeIdent'). 
   // 3. Guardar una copia de la matriz de modelado (con 'pushMM')
   // 4. Recorrer la lista de nodos y procesar las entradas transformación o subobjeto:
   //      + Para las entradas subobjeto, invocar recursivamente a 'visualizarModoSeleccionGL'
   //      + Para las entradas transformación, componer la matriz (con 'compMM')
   // 5. Restaurar la matriz de modelado original (con 'popMM')   
   // 6. Si el identificador no es -1, restaurar el color previo del cauce (con 'popColor')
   //
   // ........


}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   // COMPLETAR: práctica 3: agregar la entrada al nodo
   // ........
   entradas.push_back(entrada);
   
   //devolver índice de la entrada agregada
   return entradas.size()-1;

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const glm::mat4 & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
glm::mat4 * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETAR: práctica 3: leer un puntero a una matriz en una entrada de un nodo
   //

   // Devolver el puntero a la matriz en la entrada indicada por 'indice'. 
   // Debe de dar error y abortar si: 
   //   - el índice está fuera de rango 
   //   - la entrada no es de tipo transformación
   //   - el puntero a la matriz es nulo 
   //
   // Sustituir 'return nullptr' por lo que corresponda.
   //
    assert( indice < entradas.size() );
   assert( entradas[indice].tipo == TipoEntNGE::transformacion );
   assert( entradas[indice].matriz != nullptr );
   return entradas[indice].matriz ; // sustituir por lo que corresponda ....

}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{
   using namespace std ;
   using namespace glm ;

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int          ident_busc, // identificador a buscar
   const glm::mat4 &  mmodelado,  // matriz de modelado
   Objeto3D       **  objeto,     // (salida) puntero al puntero al objeto
   glm::vec3 &        centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   using namespace std ;
   using namespace glm ;
   
   assert( 0 < ident_busc );

   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   // 1. calcula el centro del objeto, (solo la primera vez)
   // ........


   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........


   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........


   // ni este nodo ni ningún hijo es el buscado: terminar
   return false ;
}


//------------------------------EXAMEN CONVOCATORIA ORDINARIA 2020/2021 PRÁCTICA 3----------------------------------------

GrafoEstrellaX::GrafoEstrellaX( unsigned n ){

   NodoGrafoEscena *raiz = new NodoGrafoEscena();
   NodoGrafoEscena *estrella= new NodoGrafoEscena();
   EstrellaX* estrellaX = new EstrellaX(n,1,0.0,0.0); 
   estrella->agregar(estrellaX);
   raiz->agregar(estrella);

   const int grados_circulo = 360;


   float x,y, grados_int_inicial = grados_circulo/n;

   for(float grados = grados_circulo/n;grados <= grados_circulo; grados += grados_int_inicial){    
         NodoGrafoEscena *cono = new NodoGrafoEscena();                   
         x = (cos(radians(grados)));
         y = (sin(radians(grados)));

         cono->agregar(new ConoX(x,y, grados));
         raiz->agregar(cono);
   }

   unsigned rot_estrella = this->agregar(rotate(0.0f, vec3{0.0, 1.0, 0.0}));
   this->agregar(raiz);

   pm_rot = leerPtrMatriz(rot_estrella);


}

void GrafoEstrellaX::fijarRotEstrella( const float alpha ){
    *pm_rot = rotate( alpha, vec3{0, 0, 1} );
}

unsigned GrafoEstrellaX::leerNumParametros() const {
    return 1;
}

void GrafoEstrellaX::actualizarEstadoParametro( const unsigned iParam, const float tSec ){
    assert(iParam < leerNumParametros() && iParam >=0);

   float angulo= 360*tSec*2.5;

    switch(iParam)
    {
        case 0:
            fijarRotEstrella(angulo);
        break;
    }
}

ConoX::ConoX(float x, float y, float grados)
{
   agregar(translate(vec3{x,y,0.0}));
   agregar(rotate(radians(grados-90),vec3{0.0,0.0,1.0}));
   agregar(scale(vec3(0.21,0.1125,0.21)));
   agregar(new Cono(2,30));
}



//------------------------------EXAMEN CONVOCATORIA ORDINARIA 2020/2021 PRÁCTICA 3----------------------------------------

GrafoCubos::GrafoCubos(){

   // base
   NodoGrafoEscena *base = new NodoGrafoEscena();
   base->agregar(translate(vec3(-0.5,-0.5,-0.5)));
   base->agregar(new Wall(pm_rot1));
   agregar(base);

   //Perpendicular al eje X negativo
   NodoGrafoEscena *ejexneg = new NodoGrafoEscena();
   ejexneg->agregar(translate(vec3(-0.5,0.5,-0.5)));
   ejexneg->agregar(rotate(-(float)M_PI/2,vec3{0,0,1}));
   ejexneg->agregar(new Wall(pm_rot2));
   agregar(ejexneg);

   //Techo
   NodoGrafoEscena *techo = new NodoGrafoEscena();
   techo->agregar(translate(vec3(-0.5,0.5,0.5)));
   techo->agregar(rotate((float)M_PI,vec3{1,0,0}));
   techo->agregar(new Wall(pm_rot3));
   agregar(techo);

   //Perpendicular al eje X positivo
   NodoGrafoEscena *ejexpos = new NodoGrafoEscena();
   ejexpos->agregar(translate(vec3(0.5,-0.5,-0.5)));
   ejexpos->agregar(rotate((float)M_PI/2,vec3{0,0,1}));
   ejexpos->agregar(new Wall(pm_rot4));
   agregar(ejexpos);

   //Perpendicular al eje Z negativo
   NodoGrafoEscena *ejezneg = new NodoGrafoEscena();
   ejezneg->agregar(translate(vec3(0.5,-0.5,-0.5)));
   ejezneg->agregar(rotate((float)M_PI,vec3{0,1,1}));
   ejezneg->agregar(new Wall(pm_rot5));
   agregar(ejezneg);

   //Perpendicular al eje Z positivo
   NodoGrafoEscena *ejezpos = new NodoGrafoEscena();
   ejezpos->agregar(translate(vec3(0.5,-0.5,0.5)));
   ejezpos->agregar(rotate((float)M_PI/2,vec3{0,0,1}));
   ejezpos->agregar(rotate(3*(float)M_PI/2,vec3{1,0,0}));
   ejezpos->agregar(new Wall(pm_rot6));
   agregar(ejezpos);

}

Wall::Wall(mat4 * &pm_rot){

   NodoGrafoEscena  *pared = new NodoGrafoEscena();
   pared->agregar(new RejillaY(20,20));
   agregar(pared);

   NodoGrafoEscena * cubo = new NodoGrafoEscena();
   
   cubo->agregar(translate(vec3(0.5,-0.25,0.5)));
   cubo->agregar(scale(vec3(0.125,0.25,0.125)));
   unsigned ind = cubo->agregar(rotate(0.0f,vec3{0,1,0}));
   cubo->agregar(new Cubo());
   agregar(cubo);
   pm_rot = cubo->leerPtrMatriz(ind);

}

void GrafoCubos::fijarRot1( const float alpha ){
    *pm_rot1 = rotate( alpha, vec3{0, 1, 0} );
}

void GrafoCubos::fijarRot2( const float alpha ){
    *pm_rot2 = rotate( alpha, vec3{0, 1, 0} );
}

void GrafoCubos::fijarRot3( const float alpha ){
    *pm_rot3 = rotate( alpha, vec3{0, 1, 0} );
}

void GrafoCubos::fijarRot4( const float alpha ){
    *pm_rot4 = rotate( alpha, vec3{0, 1, 0} );
}

void GrafoCubos::fijarRot5( const float alpha ){
    *pm_rot5 = rotate( alpha, vec3{0, 1, 0} );
}

void GrafoCubos::fijarRot6( const float alpha ){
    *pm_rot6 = rotate( alpha, vec3{0, 1, 0} );
}

unsigned GrafoCubos::leerNumParametros() const {
   return 6;
}

void GrafoCubos::actualizarEstadoParametro( const unsigned iParam, const float tSec ){
    assert( iParam < leerNumParametros() );
    switch (iParam)
    {
    case 0:
         fijarRot1(2.5*tSec);
         fijarRot2(2.5*tSec);
         fijarRot3(2.5*tSec);
         fijarRot4(2.5*tSec);
         fijarRot5(2.5*tSec);
         fijarRot6(2.5*tSec);
      break;
    }
}




//--------------------------------------------------------------------------


//--------Ejercicio 24 del tema 2 de las diapositivas
Android::Android()
{
   NodoGrafoEscena *cabeza= new NodoGrafoEscena();

   NodoGrafoEscena *semicabeza= new NodoGrafoEscena();
   semicabeza->agregar(translate(vec3{0,1.1,0}));
   semicabeza->agregar(scale(vec3(0.5,0.5,0.5)));
   semicabeza->agregar(new SemiEsfera(100,100));
   cabeza->agregar(semicabeza);
  
   NodoGrafoEscena *oreja_izq= new NodoGrafoEscena();
   oreja_izq->agregar(translate(vec3{0.5,1.1,0}));
   //oreja_izq->agregar(scale(vec3(0.1,0.1,0.1)));
   extremidad(oreja_izq);
   cabeza->agregar(oreja_izq);

   NodoGrafoEscena *oreja_derch= new NodoGrafoEscena();
   oreja_derch->agregar(translate(vec3{-0.5,1.1,0}));
   //oreja_derch->agregar(scale(vec3(0.1,0.1,0.1)));
   extremidad(oreja_derch);
   cabeza->agregar(oreja_derch);

   

   NodoGrafoEscena *cuerpo=new NodoGrafoEscena();
   cuerpo->agregar(new Cilindro(10,10));
   cuerpo->agregar(rotate(180.0f,vec3{0,1,0}));
   agregar(cuerpo);

   NodoGrafoEscena *brazo_izq=new NodoGrafoEscena();
   unsigned int ind_rot_brazoi = brazo_izq->agregar(rotate(0.0f,vec3{0,1,0}));
   brazo_izq->agregar(translate(vec3{3.4,4.3,0}));
   extremidad(brazo_izq);
   agregar(brazo_izq);
   pm_rot_brazoi = brazo_izq->leerPtrMatriz(ind_rot_brazoi);


   NodoGrafoEscena *brazo_derch=new NodoGrafoEscena();
   unsigned int ind_rot_brazod = brazo_derch->agregar(rotate(0.0f,vec3{0,1,0}));
   brazo_derch->agregar(scale(vec3{5,5,5}));
   brazo_derch->agregar(translate(vec3{-3.4,4.3,0}));
   extremidad(brazo_derch);
   this->agregar(brazo_derch);
   pm_rot_brazod = brazo_derch->leerPtrMatriz(ind_rot_brazod);

   NodoGrafoEscena *pie_izq=new NodoGrafoEscena();
   pie_izq->agregar(scale(vec3{25,25,25}));
   pie_izq->agregar(translate(vec3{-1,0,0}));
   extremidad(pie_izq);
   agregar(pie_izq);


   NodoGrafoEscena *pie_derch=new NodoGrafoEscena();
   pie_derch->agregar(scale(vec3{125,125,125}));
   pie_derch->agregar(translate(vec3{1,0,0}));
   extremidad(pie_derch);
   agregar(pie_derch);

   unsigned int ind_rot_cabeza = cabeza->agregar(rotate(0.0f,vec3{0,1,0}));
   agregar(cabeza);
   pm_rot_cabeza = cabeza->leerPtrMatriz(ind_rot_cabeza);
   
}

void Android::fijarRotBrazos( const float alpha )
{
    float rot = -sin(2*M_PI*0.7*alpha)/10;
   *pm_rot_brazod = rotate(rot, vec3{1.0, 0.0, 0.0});
   *pm_rot_brazoi = rotate(-rot, vec3{1.0, 0.0, 0.0});
   
}

void Android::fijarRotCabeza( const float alpha )
{
   float rot = alpha * 360.0;
   *pm_rot_cabeza = rotate(rot, vec3{0.0, 1.0, 0.0});
}

unsigned Android::leerNumParametros() const
{
   return 2;
}

void Android::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros()); 

   switch(iParam)
   {
      case 0:
         fijarRotBrazos(t_sec);
         break;
      case 1: 
         fijarRotCabeza(t_sec);
         break;

   }
}

void Android::extremidad(NodoGrafoEscena* transicion){

   NodoGrafoEscena *extremidad = new NodoGrafoEscena(); 
   extremidad->agregar(scale(vec3(0.2,0.2,0.2)));
   extremidad->agregar(new SemiEsfera(100,100));

   extremidad->agregar(inverse(scale(vec3(0.2,0.2,0.2))));
   extremidad->agregar(scale(vec3(0.45,2.72,0.45)));
   extremidad->agregar(translate(vec3(0,-1,0)));
   extremidad->agregar(new Cilindro(10,30));

   extremidad->agregar(inverse(scale(vec3{0.45,2.72,0.45})));
   extremidad->agregar(scale(vec3(0.2,0.2,0.2)));
   extremidad->agregar(rotate(160.3f,vec3{0,0,1}));
   extremidad->agregar(new SemiEsfera(100,100));


   agregar(scale(vec3(0.2,0.2,0.2)));
   transicion->agregar(extremidad);

}


// ------------------------GRAFO EXAMEN 2022 PRÁCTICAS 1,2 Y 3------------------------

Coche::Coche()
{

   NodoGrafoEscena * coche = new NodoGrafoEscena();
   unsigned ind_rot_coche = this->agregar(translate(vec3{0.0, 0.0, 0.0}));

   NodoGrafoEscena * caja = new NodoGrafoEscena();

   NodoGrafoEscena * cajasup = new NodoGrafoEscena();

   cajasup->agregar( scale(vec3(0.9, 0.6, 1.25)) );
   cajasup->agregar( new Cubo() );

   NodoGrafoEscena * cajainf = new NodoGrafoEscena();

   cajainf->agregar( scale(vec3(1.1, 0.7, 2.0)) );
   cajainf->agregar( new Cubo() );

   caja->agregar(translate(vec3{0.0,-0.7, 0.0}));
   caja->agregar(cajainf);
   caja->agregar(translate(vec3{0.0, 1.3, 0.0}));
   caja->agregar(cajasup);


   NodoGrafoEscena * rueda = new NodoGrafoEscena();
   rueda->agregar(translate(vec3(-1.0, 0.7, 0.0)));
   //rueda->agregar(rotate(radians(-45.0f),vec3{0, 1, 0}));

   //Agregamos las ruedas del lado izquierdo
   NodoGrafoEscena * ruedas_izq = new NodoGrafoEscena();

   ruedas_izq->agregar(translate(vec3(0.8,-1.2,-0.7)));
   ruedas_izq->agregar(rotate(radians(180.0f),vec3{0,1,0}));
   ruedas_izq->agregar(rueda);
   ruedas_izq->agregar(translate(vec3{0.0,0.0,-2.4}));
   ruedas_izq->agregar(rueda);

   //Agregamos las ruedas del lado derecho
   NodoGrafoEscena * ruedas_der = new NodoGrafoEscena();

   ruedas_der->agregar(translate(vec3(-1.7,-1.2,-1)));
   ruedas_der->agregar(rotate(radians(180.0f),vec3{0,1,0}));
   ruedas_der->agregar(rueda);
   ruedas_der->agregar(translate(vec3{0.0,0.0,-2.4}));
   ruedas_der->agregar(rueda);
      
   rueda->agregar(translate(vec3(0.5, 0.0, 0.5)));
   
   //Con el nodo cilindro formamos un cilindro con dos SemiCilindroBarrido
   NodoGrafoEscena * cilindro = new NodoGrafoEscena();

   //HACER PRIMERO LOS TRANSLATE Y LUEGO LOS ROTATES
   cilindro->agregar(scale(vec3(3, 3, 3)));
   cilindro->agregar(translate(vec3{0.8, 0, 0}));
   cilindro->agregar( rotate(89.5f, vec3{0.0, 0.0, 1.0}) );
   cilindro->agregar(new SemiCilindroBarrido(7,15) );
   cilindro->agregar(rotate(180.0f, vec3{0.0, 1.0, 0.0}));      
   cilindro->agregar(new SemiCilindroBarrido(7,15) );

   //Formamos una rueda con el nodo cilindro y un cubo
   unsigned ind_rot_rueda = rueda->agregar(rotate(0.0f, vec3{1.0, 0.0, 0.0}));
   rueda->agregar(scale(vec3(0.25,0.25,0.25)));
   Cubo *cubo1 = new Cubo();
   cubo1->ponerColor({1.0, 0.0, 1.0});
   rueda->agregar( cubo1 );
   rueda->agregar(scale(vec3(0.5,0.5,0.5)));
   rueda->agregar( cilindro );
   rueda->agregar(scale(vec3(0.7, 0.7, 0.7)));
   rueda->agregar(translate(vec3{0.5, 1.8, 0.0}));
   
   pm_rot_rueda = rueda->leerPtrMatriz(ind_rot_rueda);

   coche->agregar(ruedas_izq);
   coche->agregar(ruedas_der);
   //Agregamos los nodos caja, ruedas izq y ruedas der que seran los nodos raíces
   
   //this->agregar(translate(vec3{-5.0, 0.0, 0.0}));
   coche->agregar(caja);
   
   this->agregar(coche);
   pm_tras_coche = leerPtrMatriz(ind_rot_coche);
   
   
}

void Coche::fijarRotacionRueda( const float n_alpha )
{
   float rot = n_alpha*360;
   *pm_rot_rueda = rotate(rot, vec3{1.0, 0.0, 0.0});
}

void Coche::fijarTraslacionCoche( const float n_alpha )
{
   
   float rot=sin(2*M_PI*0.7*n_alpha);
   *pm_tras_coche = translate(vec3{0.0, 0.0, rot});
}

unsigned Coche::leerNumParametros() const
{
   return 2;
}

void Coche::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros()); 

   switch(iParam)
   {
      case 0:
         fijarTraslacionCoche(t_sec);
         break;

      case 1:
         fijarRotacionRueda(t_sec);
         break;
   }
}


//----------------------OTRO GRAFO-------------------------

CuerpoPerro::CuerpoPerro(){
   NodoGrafoEscena * cuerpo = new NodoGrafoEscena();

   NodoGrafoEscena *partedelantera = new NodoGrafoEscena();
   partedelantera->agregar(translate(vec3(-0.7,0.5,0)));
   partedelantera->agregar(rotate((float)M_PI/2,vec3(0,0.0,1.0)));
   partedelantera->agregar(scale(vec3(0.3,0.3,0.3)));
   SemiEsfera * semiesfera = new SemiEsfera(100,100);
   semiesfera->ponerColor({0.0,0.0,0.0});
   partedelantera->agregar(semiesfera);
   cuerpo->agregar(partedelantera);

   

   NodoGrafoEscena *partetrasera = new NodoGrafoEscena();
   partetrasera->agregar(translate(vec3(0.7,0.5,0)));
   partetrasera->agregar(rotate(-(float)M_PI/2,vec3(0,0.0,1.0)));
   partetrasera->agregar(scale(vec3(0.3,0.3,0.3)));
   SemiEsfera * semiesfera2 = new SemiEsfera(100,100);
   semiesfera2->ponerColor({0.0,0.0,0.0});
   partetrasera->agregar(semiesfera2);
   cuerpo->agregar(partetrasera);

   NodoGrafoEscena *cuerpocilindro = new NodoGrafoEscena();
   cuerpocilindro->agregar(translate(vec3(-0.7,0.5,0)));
   cuerpocilindro->agregar(rotate(-(float)M_PI/2,vec3(0,0.0,1.0)));
   cuerpocilindro->agregar(scale(vec3(0.6,1.4,0.6)));
   Cilindro * cilindro = new Cilindro(2,30);
   cilindro->ponerColor({0.22, 0.12, 0});
   cuerpocilindro->agregar(cilindro);
   cuerpo->agregar(cuerpocilindro);

   this->agregar(cuerpo);

}

Cola::Cola(){

   NodoGrafoEscena * cola = new NodoGrafoEscena();

   NodoGrafoEscena * esferacola = new NodoGrafoEscena();
   esferacola->agregar(scale(vec3(0.2,0.2,0.2)));
   EsferaRadio * esfera = new EsferaRadio(50,100,0.5);
   esfera->ponerColor({0.0, 0.0, 0.0});
   esferacola->agregar(esfera);
   cola->agregar(esferacola);

   NodoGrafoEscena * cilindrocola = new NodoGrafoEscena();
   cilindrocola->agregar(translate(vec3(0.0,-0.5,0.0)));
   cilindrocola->agregar(scale(vec3(0.1,0.4,0.1)));
   Cilindro * cilindro = new Cilindro(2,30);
   cilindro->ponerColor({0.22, 0.12, 0});
   cilindrocola->agregar(cilindro);
   cola->agregar(cilindrocola);

   this->agregar(cola);

}

Perro::Perro(){

   NodoGrafoEscena * perro = new NodoGrafoEscena();

   NodoGrafoEscena * cuerpo = new NodoGrafoEscena();
   cuerpo->agregar(scale(vec3(0.8,0.8,0.8)));
   cuerpo->agregar(new CuerpoPerro());
   perro->agregar(cuerpo);

   NodoGrafoEscena * cabeza = new NodoGrafoEscena();
   unsigned ind_rot_cabeza = cabeza->agregar(rotate(0.0f, vec3{0.0, 1.0, 0.0}));;
   cabeza->agregar(scale(vec3(0.6,0.6,0.6)));
   cabeza->agregar(translate(vec3(1.3,1.4,0)));
   EsferaRadio * esfera = new EsferaRadio(50,100,0.5);
   esfera->ponerColor({0.22, 0.12, 0});
   cabeza->agregar(esfera);

   NodoGrafoEscena * oreja_izq = new NodoGrafoEscena();
   oreja_izq->agregar(translate(vec3(0.0,0.4,0.4)));
   oreja_izq->agregar(rotate((float)M_PI/2,vec3(0,0.0,1.0)));
   oreja_izq->agregar(scale(vec3(0.2,0.2,0.2)));
   SemiEsfera * semiesfera = new SemiEsfera(100,100);
   semiesfera->ponerColor({0.0,0.0,0.0});
   oreja_izq->agregar(semiesfera);
   cabeza->agregar(oreja_izq);

   NodoGrafoEscena * oreja_derch = new NodoGrafoEscena();
   oreja_derch->agregar(translate(vec3(0.0,0.4,-0.4)));
   oreja_derch->agregar(rotate((float)M_PI/2,vec3(0,0.0,1.0)));
   oreja_derch->agregar(scale(vec3(0.2,0.2,0.2)));
   SemiEsfera * semiesfera2 = new SemiEsfera(100,100);
   semiesfera2->ponerColor({0.0,0.0,0.0});
   oreja_derch->agregar(semiesfera2);
   cabeza->agregar(oreja_derch);

   //A los nodos que les voy a añadir movimiento, los añado al final de todos los nodos
   //porque si no se mueven todos los nodos declarados debajo de este
   NodoGrafoEscena * cola = new NodoGrafoEscena();
   unsigned ind_rot_cola = cola->agregar(rotate(0.0f,vec3{0.0, 1.0, 0.0}));
   cola->agregar(translate(vec3(-1,0.8,0)));
   cola->agregar(rotate((float)M_PI/3,vec3(0,0.0,1.0)));
   cola->agregar(new Cola());

   NodoGrafoEscena *pie_dizq=new NodoGrafoEscena();
   pie_dizq->agregar(translate(vec3(0.5,0,-0.1)));
   pie_dizq->agregar(rotate((float)M_PI/2,vec3(0.0,0,1)));
   pie_dizq->agregar(scale(vec3(0.2,0.3,0.2)));
   pie_dizq->agregar(new CuerpoPerro());
   perro->agregar(pie_dizq);

   NodoGrafoEscena *pie_tizq=new NodoGrafoEscena();
   pie_tizq->agregar(translate(vec3(-0.2,0,-0.1)));
   pie_tizq->agregar(rotate((float)M_PI/2,vec3(0.0,0,1)));
   pie_tizq->agregar(scale(vec3(0.2,0.3,0.2)));
   pie_tizq->agregar(new CuerpoPerro());
   perro->agregar(pie_tizq);

   NodoGrafoEscena *pie_dcho=new NodoGrafoEscena();
   pie_dcho->agregar(translate(vec3(0.5,0,0.1)));
   pie_dcho->agregar(rotate((float)M_PI/2,vec3(0.0,0,1)));
   pie_dcho->agregar(scale(vec3(0.2,0.3,0.2)));
   pie_dcho->agregar(new CuerpoPerro());
   perro->agregar(pie_dcho);

   NodoGrafoEscena *pie_tdcho=new NodoGrafoEscena();
   pie_tdcho->agregar(translate(vec3(-0.2,0,0.1)));
   pie_tdcho->agregar(rotate((float)M_PI/2,vec3(0.0,0,1)));
   pie_tdcho->agregar(scale(vec3(0.2,0.3,0.2)));
   pie_tdcho->agregar(new CuerpoPerro());
   perro->agregar(pie_tdcho);

   
   perro->agregar(cola);
   pm_rot_cola = cola->leerPtrMatriz(ind_rot_cola);

   
   perro->agregar(cabeza);
   pm_rot_cabeza = cabeza->leerPtrMatriz(ind_rot_cabeza);

   unsigned ind_tras_perro = this->agregar(rotate(0.0f,vec3{0.0, 1.0, 0.0}));
   this->agregar(perro);
   pm_tras_perro = leerPtrMatriz(ind_tras_perro);


}

void Perro::fijarRotacionCola( const float n_alpha )
{
   float rot =  sin(2*M_PI*0.7*n_alpha)/5;
   *pm_rot_cola = rotate(rot, vec3{1.0, 0.0, 0.0});
}

void Perro::fijarTraslacionPerro( const float n_alpha )
{
   float tras=sin(2*M_PI*0.7*n_alpha)/10;
   *pm_tras_perro = translate(vec3{tras, 0.0, 0.0});
}

void Perro::fijarRotacionCabeza( const float n_alpha )
{
   float rot=sin(2*M_PI*0.7*n_alpha)/10;
   *pm_rot_cabeza = rotate(rot, vec3{1.0, 0.0, 0.0});
}

unsigned Perro::leerNumParametros() const
{
   return 3;
}

void Perro::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros()); 

   switch(iParam)
   {
      case 0:
         fijarRotacionCola(t_sec);
         break;
      case 1:
         fijarTraslacionPerro(t_sec);
         break;
      case 2:
         fijarRotacionCabeza(t_sec);
      break;
      
   }
}

//-----------------------------------------------------



//----------------------OTRO GRAFO-------------------------
Muñeco2::Muñeco2(){

   NodoGrafoEscena * muñeco = new NodoGrafoEscena();
   unsigned ind_rot_muñeco = muñeco->agregar(rotate(0.0f, vec3{0.0, 1.0, 0.0}));


   NodoGrafoEscena * cabeza = new NodoGrafoEscena();
   unsigned ind_rot_cabeza = cabeza->agregar(rotate(0.0f, vec3{0.0, 1.0, 0.0}));

   NodoGrafoEscena * semiesfera = new NodoGrafoEscena();
   semiesfera->agregar(scale(vec3(0.5,0.5,0.5)));
   semiesfera->agregar(new SemiEsfera(100,100));
   //SI quiero añadir una traslacion o rotacion al nodo cabeza tengo que añadirla antes de 
   //añadir los subnodos
   cabeza->agregar(translate(vec3(0.0,1.1,0)));
   cabeza->agregar(semiesfera);

   NodoGrafoEscena * ojos = new NodoGrafoEscena();
   unsigned ind_rot_ojos = ojos->agregar(rotate(0.0f, vec3{0.0, 1.0, 0.0}));

   NodoGrafoEscena * ojoizq = new NodoGrafoEscena();
   ojoizq->agregar(translate(vec3(-0.2,0.2,0.4)));
   ojoizq->agregar(scale(vec3(0.1,0.1,0.1)));
   ojoizq->agregar(new EsferaRadio(50,100,0.5));
   ojos->agregar(ojoizq);

   NodoGrafoEscena * ojoderch = new NodoGrafoEscena();
   ojoderch->agregar(translate(vec3(0.2,0.2,0.4)));
   ojoderch->agregar(scale(vec3(0.1,0.1,0.1)));
   ojoderch->agregar(new EsferaRadio(50,100,0.5));
   ojos->agregar(ojoderch);

   ojos->ponerColor({0.0,0.0,0.0});
   pm_rot_ojos = ojos->leerPtrMatriz(ind_rot_ojos);

   cabeza->agregar(ojos);

   NodoGrafoEscena * orejadech = new NodoGrafoEscena();
   orejadech->agregar(translate(vec3(0.3,0.35,0.1)));
   orejadech->agregar(rotate(-(float)M_PI/5,vec3(0,0.0,1.0)));
   orejadech->agregar(scale(vec3(0.2,0.2,0.05)));
   orejadech->agregar(new Cono(2,30));
   cabeza->agregar(orejadech);

   NodoGrafoEscena * orejaizq = new NodoGrafoEscena();
   orejaizq->agregar(translate(vec3(-0.3,0.35,0.1)));
   orejaizq->agregar(rotate((float)M_PI/5,vec3(0,0.0,1.0)));

   orejaizq->agregar(scale(vec3(0.2,0.2,0.05)));
   orejaizq->agregar(new Cono(2,30));
   cabeza->agregar(orejaizq);

   NodoGrafoEscena * cuerpo = new NodoGrafoEscena();
   cuerpo->agregar(new Cilindro(100,100));
   muñeco->agregar(cuerpo);



   muñeco->agregar(cabeza);
   agregar(muñeco);
   pm_rot_cabeza = cabeza->leerPtrMatriz(ind_rot_cabeza);
   pm_tras_muñeco = muñeco->leerPtrMatriz(ind_rot_muñeco);
   pm_rot_muñeco = muñeco->leerPtrMatriz(ind_rot_muñeco);

}

unsigned Muñeco2::leerNumParametros() const
{
   return 3;
}

void Muñeco2::fijarRotacionCabeza( const float n_alpha )
{
   float rot =  sin(2*M_PI*0.7*n_alpha)/5;
   *pm_rot_cabeza = rotate(rot, vec3{0.0, 1.0, 0.0});
}

void Muñeco2::fijarRotacionOjos( const float n_alpha )
{
   float rot =  sin(2*M_PI*0.7*n_alpha)/5;
   *pm_rot_ojos = rotate(rot, vec3{0.0, 1.0, 0.0});
}

void Muñeco2::fijarTraslacionMuñeco( const float n_alpha )
{
   float rot =  sin(2*M_PI*0.7*n_alpha)/5;
   float rot2 =  2*M_PI*0.7*n_alpha;
   *pm_tras_muñeco = translate(vec3(rot, 0.0, 0.0));
   *pm_rot_muñeco = rotate(rot2, vec3(1.0, 0.0, 0.0));
}

void Muñeco2::actualizarEstadoParametro (const unsigned iParam, const float t_sec){
   assert(iParam < leerNumParametros()); 

   switch(iParam)
   {
      case 0:
         fijarRotacionOjos(t_sec);
         break;
      case 1:
         fijarRotacionCabeza(t_sec);
         break;
      case 2:
         fijarTraslacionMuñeco(t_sec);
      break;
   }
}



//-----------------------------------------------------


Prueba::Prueba(){

   //Cubo que gira sobre sí mismo 
   NodoGrafoEscena * cubo = new NodoGrafoEscena();
   cubo->agregar(translate(vec3(0.5,0.5,0.5)));
   cubo->agregar(scale(vec3(0.125,0.25,0.125)));
   unsigned ind = cubo->agregar(rotate(0.0f,vec3{0,1,0}));
   cubo->agregar(new Cubo());
   pm_rot1=cubo->leerPtrMatriz(ind);
   agregar(cubo);
}

void Prueba::fijarRot1( const float alpha ){
    *pm_rot1 = rotate( alpha, vec3{0, 1, 0} );
}

void Prueba::actualizarEstadoParametro( const unsigned iParam, const float tSec ){
    assert( iParam < leerNumParametros() );
    switch (iParam)
    {
    case 0:
         fijarRot1(2.5*tSec);
      break;
    }
}

unsigned Prueba::leerNumParametros() const
{
   return 1;
}








Rueda::Rueda(){
    NodoGrafoEscena * rueda = new NodoGrafoEscena();
      
   //quitamos esta traslación porque sino en la clase ruedas no se mueve sobre sí misma
   //rueda->agregar(translate(vec3(0.5, 0.0, 0.5)));
   
   //Con el nodo cilindro formamos un cilindro con dos SemiCilindroBarrido
   NodoGrafoEscena * cilindro = new NodoGrafoEscena();

   //HACER PRIMERO LOS TRANSLATE Y LUEGO LOS ROTATES
   cilindro->agregar(scale(vec3(3, 3, 3)));
   cilindro->agregar(translate(vec3{0.8, 0, 0}));
   cilindro->agregar( rotate(89.5f, vec3{0.0, 0.0, 1.0}) );
   cilindro->agregar(new SemiCilindroBarrido(7,15) );
   cilindro->agregar(rotate(180.0f, vec3{0.0, 1.0, 0.0}));      
   cilindro->agregar(new SemiCilindroBarrido(7,15) );

   //Formamos una rueda con el nodo cilindro y un cubo
   unsigned ind_rot_rueda = rueda->agregar(rotate(0.0f, vec3{1.0, 0.0, 0.0}));
   rueda->agregar(scale(vec3(0.25,0.25,0.25)));
   Cubo *cubo1 = new Cubo();
   cubo1->ponerColor({1.0, 0.0, 1.0});
   rueda->agregar( cubo1 );
   rueda->agregar(scale(vec3(0.5,0.5,0.5)));
   rueda->agregar( cilindro );
   rueda->agregar(scale(vec3(0.7, 0.7, 0.7)));
   rueda->agregar(translate(vec3{0.5, 1.8, 0.0}));
   
   

   //pm_rot = rueda->leerPtrMatriz(ind_rot_rueda);
   agregar(rueda);

}


Ruedas::Ruedas() {

   NodoGrafoEscena * cuatroruedas = new NodoGrafoEscena();
   //SI queremos mover el nodo raíz lo hacemos antes de añadirle los subnodos, porque sino no funciona
   cuatroruedas->agregar(translate (vec3(0.0,0.0,0.0)));

   NodoGrafoEscena *rueda1 = new NodoGrafoEscena();
   unsigned ind_rot_cuatroruedas = rueda1->agregar(translate(vec3{0.0, 0.0, 0.0}));
   rueda1->agregar(new Rueda());
   cuatroruedas->agregar(rueda1);
  
   NodoGrafoEscena *rueda2 = new NodoGrafoEscena();
   cuatroruedas->agregar(translate(vec3(0.0,0.0,1.0)));
   unsigned ind_rot_cuatroruedas2 = rueda2->agregar(translate(vec3{0.0, 0.0, 0.0}));
   rueda2->agregar(new Rueda());
   cuatroruedas->agregar(rueda2);

   NodoGrafoEscena *rueda3 = new NodoGrafoEscena();
   cuatroruedas->agregar(translate(vec3(1.5,0.0,0.0)));
   unsigned ind_rot_cuatroruedas3 = rueda3->agregar(translate(vec3{0.0, 0.0, 0.0}));
   rueda3->agregar(new Rueda());
   cuatroruedas->agregar(rueda3);

   NodoGrafoEscena *rueda4 = new NodoGrafoEscena();
   cuatroruedas->agregar(translate(vec3(0.15,0.0,-1.0)));
   unsigned ind_rot_cuatroruedas4 = rueda4->agregar(translate(vec3{0.0, 0.0, 0.0}));
   rueda4->agregar(new Rueda());
   cuatroruedas->agregar(rueda4);

   //Hay que utilizar una matriz para cada rueda por que si aplico la transformacion a cuatroruedas
   //directamente, no giran sobre si mismas
   pm_rot1 = rueda1->leerPtrMatriz(ind_rot_cuatroruedas);
   pm_rot2 =rueda2->leerPtrMatriz(ind_rot_cuatroruedas2);
   pm_rot3 =rueda3->leerPtrMatriz(ind_rot_cuatroruedas3);
   pm_rot4 =rueda4->leerPtrMatriz(ind_rot_cuatroruedas4);

   

   agregar(cuatroruedas);
  
}

void Ruedas::fijarRotacion( const float alpha )
{
   float rot = 20*alpha;
   *pm_rot1 = rotate(rot, vec3{1.0, 0.0, 0.0});
   *pm_rot2 = rotate(rot, vec3{1.0, 0.0, 0.0});
   *pm_rot3 = rotate(rot, vec3{1.0, 0.0, 0.0});
   *pm_rot4 = rotate(rot, vec3{1.0, 0.0, 0.0});
}

unsigned Ruedas::leerNumParametros() const
{
   return 1;
}  

void Ruedas::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros()); 

   switch(iParam)
   {
      case 0:
         fijarRotacion(t_sec);
         break;
   }
}











Casa::Casa(){

   NodoGrafoEscena * casa = new NodoGrafoEscena();

   NodoGrafoEscena * paredejex = new NodoGrafoEscena();
   paredejex->agregar(new PanelAncho());
   casa->agregar(paredejex);

   NodoGrafoEscena *paredejexpos = new NodoGrafoEscena();
   paredejexpos->agregar(translate(vec3(0,0,2)));
   paredejexpos->agregar(rotate(radians(90.0f),vec3{0,1,0}));
   paredejexpos->agregar(new PanelEstrecho());
   casa->agregar(paredejexpos);

   NodoGrafoEscena *paredejez = new NodoGrafoEscena();
   paredejez->agregar(new PanelEstrecho());
   casa->agregar(paredejez);

   NodoGrafoEscena *paredejezpos = new NodoGrafoEscena();
   paredejezpos->agregar(translate(vec3(3,0,0)));
   paredejezpos->agregar(new PanelEstrecho());
   casa->agregar(paredejezpos);

   NodoGrafoEscena *puerta = new NodoGrafoEscena();
   puerta->agregar(translate(vec3(2,0,2)));
   unsigned ind_rot_puerta =  puerta->agregar(translate(vec3{0,0,0}));
   
   puerta->agregar(scale(vec3(0.3333,1,0.5)));
   puerta->agregar(new PanelAncho());
   casa->agregar(puerta);
   pm_rot_puerta = puerta->leerPtrMatriz(ind_rot_puerta);

   NodoGrafoEscena *techo = new NodoGrafoEscena();
   techo->agregar(translate(vec3(1.5,2,0)));
   unsigned ind_rot_techo = techo->agregar(translate(vec3{0,0,0}));
   techo->agregar(rotate(radians(25.0f),vec3{1,0,0}));
   techo->agregar(scale(vec3(0.5,1,0.0)));
   techo->agregar(new PanelAncho());
   casa->agregar(techo);
   pm_rot_techo = techo->leerPtrMatriz(ind_rot_techo);


   agregar(casa);

}

void Casa::fijarRotPuerta( const float alpha )
{
   float rot = sin(2*M_PI*alpha);
   *pm_rot_puerta = rotate(rot, vec3{0.0, 1.0, 0.0});
}  

void Casa::fijarRotTecho( const float alpha )
{
   float rot = sin(M_PI*alpha);
   *pm_rot_techo = rotate(rot, vec3{1.0, 0.0, 0.0});
}

unsigned Casa::leerNumParametros() const
{
   return 2;
}  

void Casa::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros()); 

   switch(iParam)
   {
      case 0:
         fijarRotPuerta(t_sec);
         break;
      case 1:  
         fijarRotTecho(t_sec);
         break;

   }
}



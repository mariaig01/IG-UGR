// *********************************************************************
// ** Nombre: María de las Angustias, Apellidos: Izquierdo García, Titulación: GIIADE
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Mallas indexadas (implementación)
// ** Copyright (C) 2016-2023 Carlos Ureña
// **
// ** Implementación de las clases 
// **    + MallaRevol: malla indexada de triángulos obtenida por 
// **      revolución de un perfil (derivada de MallaInd)
// **    + MallaRevolPLY: malla indexada de triángulos, obtenida 
// **      por revolución de un perfil leído de un PLY (derivada de MallaRevol)
// **    + algunas clases derivadas de MallaRevol
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
#include "lector-ply.h"
#include "malla-revol.h"
#include <glm/gtx/transform.hpp>

using namespace std ;
using namespace glm;

// *****************************************************************************




// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<glm::vec3> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   // COMPLETAR: Práctica 4: crear tablas de normales 
      vector<vec3> aristas;
      vector<float> d_i;

      // Aristas
      for (int i=0; i < perfil.size()-1; i++)
      {
         aristas.push_back(perfil[i+1]-perfil[i]);
         d_i.push_back(length(aristas[i]));
      }

      float suma = 0.0;
      for (int i=0; i < d_i.size(); i++)
         suma += d_i[i];


      vector<float> t_i;
      // Calculo de las coordenadas de texturas
      float d_j;
      for (int i=0; i < perfil.size(); i++)
      {
         d_j = 0.0;
         for (int j=0; j < i; j++)
            d_j += d_i[j];
         
         t_i.push_back(d_j / suma);
      }

      vector<vec3> nor_aristas;
      // Calculo de las normales de las aristas del perfil
      for (int i=0; i < aristas.size(); i++)
      {
         vec3 nor_ari = vec3(aristas[i].y, -aristas[i].x, 0);

         if (length(nor_ari) != 0.0)
            nor_ari = normalize(nor_ari);

         nor_aristas.push_back(nor_ari);
      }

      // Cálculo de las normales de los vértices del perfil
      vector<vec3> normales_vertices;
      vec3 normal_vertice;

   
      normales_vertices.push_back(nor_aristas[0]);
      for (size_t i=1; i < perfil.size()-1; i++)
      {
         
         normal_vertice = nor_aristas[i-1]+nor_aristas[i];

         
         if (glm::length(normal_vertice) != 0.0)
            normal_vertice = glm::normalize(normal_vertice);

         normales_vertices.push_back(normal_vertice);
      }
      
      normales_vertices.push_back(nor_aristas[perfil.size()-2]);

   // COMPLETAR: Práctica 2: completar: creación de la malla....

   float x,y,z,rot;
   float x_nor, y_nor, z_nor;
   float seno, coseno;
      // creación de tabla de vertices
      for(int i=0; i<num_copias; ++i){
         float angulo=(2*M_PI*i)/(num_copias-1);
         coseno=cos(angulo);
         seno=sin(angulo);
         for(int j=0; j<perfil.size(); ++j){
            
            x=coseno*perfil.at(j)[0]+seno*perfil.at(j)[2];
            y=perfil.at(j)[1];
            z=-seno*perfil.at(j)[0]+coseno*perfil.at(j)[2];

            vertices.push_back({x,y,z});
            //añadir normales. La normal de cualquier vétice de la malla completa es igual
            //a la normal (rotada) del correspondiente vértice del perfil original

            //Rotacion de la normal
            rot = normales_vertices[j].x;
            x_nor = rot*cos(angulo);
            y_nor = normales_vertices[j].y;
            z_nor = -rot*sin(angulo);
            vec3 aux = vec3(x_nor,y_nor,z_nor);

            if (length(aux) != 0.0)
               aux = normalize(aux);

            nor_ver.push_back(aux);
            
            // Añadir coordenadas de textura
            float s = float(i) / (num_copias-1);
            float t = 1 - t_i[j];
            cc_tt_ver.push_back({s,t});
         }
      }
      // creación de triangulos
      int k=0;
      for( int i=0; i<num_copias-1; ++i){
         for( int j=0; j<perfil.size()-1; ++j){
            k=i*perfil.size() + j;
            triangulos.push_back({k,k+perfil.size(),k+perfil.size()+1});
            triangulos.push_back({k,k+perfil.size()+1,k+1});
         }
      }
}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   // ...........................
    vector<glm::vec3>  perfil;

   LeerVerticesPLY(nombre_arch,perfil);
   inicializar(perfil,nperfiles);

}

// la base tiene el centro en el origen, el radio y la altura son 1
Cilindro::Cilindro(const int num_verts_per,const unsigned nperfiles){
      ponerNombre(string("Cilindro") );
   vector<glm::vec3> perfil;
   float longitud = 1.0/(num_verts_per - 1);
   for (int i = 0; i < num_verts_per; i++)
      perfil.push_back(glm::vec3{0.5,i*longitud,0.0});
   
   inicializar(perfil, nperfiles);

}

//// la base tiene el centro en el origen, el radio y altura son 1
Cono::Cono(const int num_verts_per,const unsigned nperfiles){
         
   //el cono sería como un cilindro pero las x no son iguales, sino que cada vez se acercarían mas a 0
   //la longitud de x tiene que decrecer hasta 0 y la altura de la y tiene que crecer hasta 1

   /*La base del cono tiene un radio de 1, por lo que los vértices del perfil estarán a una distancia de 1 del origen.
La altura del cono es 1, por lo que la coordenada y de los vértices del perfil variará desde 0 hasta 1.
La coordenada x de los vértices del perfil variará desde 1 hasta 0, de forma que el cono se vaya estrechando a medida que sube*/

/*La coordenada x del vértice se calcula como el producto de 0.5 por el valor de 1 - i * longitud. El valor de 1 - i * longitud disminuye a medida que i aumenta, por lo que la coordenada x disminuye a medida que subimos por el cono.
La coordenada y del vértice se calcula como el valor de i * longitud. El valor de i * longitud aumenta a medida que i aumenta, por lo que la coordenada y aumenta a medida que subimos por el cono.

*/

   ponerNombre(string("Cono") );
   vector<glm::vec3> perfil;
   float longitud = 1/(num_verts_per-1);
   for (int i = 0; i < num_verts_per; i++)
      perfil.push_back(glm::vec3{(1-i*longitud)*0.5,i*longitud,0.0});
   
   inicializar(perfil, nperfiles); 
  
}

Cono2::Cono2(const int num_verts_per,const unsigned nperfiles, const float radio,
   const float altura){
         
   //el cono sería como un cilindro pero las x no son iguales, sino que cada vez se acercarían mas a 0
   //la longitud de x tiene que decrecer hasta 0 y la altura de la y tiene que crecer hasta 1

   /*La base del cono tiene un radio de 1, por lo que los vértices del perfil estarán a una distancia de 1 del origen.
La altura del cono es 1, por lo que la coordenada y de los vértices del perfil variará desde 0 hasta 1.
La coordenada x de los vértices del perfil variará desde 1 hasta 0, de forma que el cono se vaya estrechando a medida que sube*/

/*La coordenada x del vértice se calcula como el producto de 0.5 por el valor de 1 - i * longitud. El valor de 1 - i * longitud disminuye a medida que i aumenta, por lo que la coordenada x disminuye a medida que subimos por el cono.
La coordenada y del vértice se calcula como el valor de i * longitud. El valor de i * longitud aumenta a medida que i aumenta, por lo que la coordenada y aumenta a medida que subimos por el cono.

*/

   ponerNombre( std::string("cono") );
   vector<glm::vec3> perfil;
   float lon = 1.0/(num_verts_per - 1);
   for (int i = 0; i < num_verts_per; i++)
      perfil.push_back(glm::vec3((1-i*lon)*radio,altura*i*lon,0.0));
   inicializar(perfil, nperfiles);
  
}

// La esfera tiene el centro en el origen, el radio es la unidad
Esfera::Esfera(const int num_verts_per,const unsigned nperfiles){

/*La longitud de un arco es la distancia entre dos puntos del arco. En este caso, queremos que el arco tenga 
una longitud igual a la circunferencia de una esfera de radio 1. La circunferencia de una esfera de radio 1 es 2 * M_PI.
Por lo tanto, la longitud del arco es igual a la circunferencia de la esfera dividida por el número de vértices del 
perfil menos uno. Esto se debe a que queremos que el arco se repita num_verts_per veces para crear la malla de revolución.*/

/*El razonamiento de que las coordenadas x e y se calculan con el seno y el coseno se basa en la siguiente ecuación:

x = r * sen(θ)
y = r * cos(θ)
donde:

x es la coordenada x del punto.
y es la coordenada y del punto.
r es el radio de la esfera.
θ es el ángulo entre el punto y el eje x.
En este caso, queremos que los vértices del perfil estén a una distancia de 1 del origen. Por lo tanto, el radio es 1.

El ángulo θ es el ángulo entre el vértice y el eje x. Este ángulo aumenta de 0 a 2 * M_PI a medida que recorremos el perfil.

Por lo tanto, la coordenada x del vértice se calcula como el seno del ángulo θ. La coordenada y del vértice se calcula como el coseno del ángulo θ.*/

ponerNombre( std::string("esfera por revolución del perfil") );
   std::vector<glm::vec3> perfil;
   for (int i=0; i < num_verts_per; i++)
   {
      float longitud = -M_PI / 2.0 + float(i) / (num_verts_per -1) * M_PI;
      perfil.push_back(glm::vec3(cos(longitud), sin(longitud), 0));
   }

   inicializar(perfil, nperfiles);

}



EsferaRadio::EsferaRadio
(
   const int num_verts_per,
   const unsigned nperfiles,
   const float radio
)
{
   ponerNombre( std::string("esfera con radio") );
   vector<glm::vec3> perfil;
   float longitud = 2.0*M_PI/(num_verts_per - 1);
   for (int i = 0; i < num_verts_per; i++){
      perfil.push_back(glm::vec3(radio*sin(i*longitud),radio*cos(i*longitud),0.0));
   }
   inicializar(perfil, nperfiles);
}


CilindroRadio::CilindroRadio
(
   const int num_verts_per,
   const unsigned nperfiles,
   const float radio,
   const float altura
)
{
   ponerNombre( std::string("cilindro") );
   std::vector<glm::vec3> perfil;
   float lon = 1.0/(num_verts_per - 1);
   for (int i = 0; i < num_verts_per; i++)
      perfil.push_back(glm::vec3(radio,altura*i*lon,0.0));
   inicializar(perfil, nperfiles);
}



//------------------------------Examen 2022 Prácticas 1,2 y 3 (Práctica 2)----------------------------------------


void MallaBarrido::inicializar
(
   const std::vector<glm::vec3> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   int n = num_copias;
   int m = perfil.size();

   for(int i = 0; i < n; ++i){
      for(int j = 0; j < m; ++j){
         vertices.push_back( { perfil[j][0], perfil[j][1]+i/5.0, perfil[j][2]  } );
      }
   }

   for(int i = 0; i < (n-1); ++i){
      for(int j = 0; j< (m-1); ++j){

         unsigned int k = i * m + j;
         triangulos.push_back( glm::vec3(k, k+m, k+m+1) );
         triangulos.push_back( glm::vec3(k, k+m+1, k+1) );
      }
   }
}


SemiCilindroBarrido::SemiCilindroBarrido(const int n, const int m)
{
   vector<glm::vec3> perfil;

   double step = M_PI / m;
   double current = 0.0;
   double x, z;

   for(int i = 0; i <= m; i++)
   {
      x = sin(current) * 1.0;
      z = cos(current) * 1.0;

      perfil.push_back( glm::vec3( x, 0.0, z ) );

      current += step;
   }

   inicializar(perfil, n);
}

//----------------------------------------------------------------------


//------------------------------Examen 2022 Prácticas 1,2 y 3 (Práctica 2)----------------------------------------

CopaRevol::CopaRevol(unsigned nperfiles, unsigned nvertices){

      std::vector<glm::vec3> perfil;
      //Base de la copa
      perfil.push_back({1.25, 0.0,0.0});
      perfil.push_back({1.25, 0.25,0.0});

      //Tronco de la copa
      float r=(float)1;
      for(unsigned i=0;i<nperfiles;++i){
         float y=r*(-1+(float)1*i/(nperfiles -1));
         perfil.push_back({-sqrt(r*r - y*y)+1.25,y+1.25,0.0});
      }

      //semicilindro de la cúspide
      float r2=(float)1;
      for(unsigned i=0;i<nperfiles;++i){
         float y=r2*(-1+(float)1*i/(nperfiles -1));
         perfil.push_back({sqrt(r2*r2 - y*y)+0.25,y+2.25,0.0});
      }

   inicializar(perfil,nvertices);;
}

//----------------------------------------------------------------------


//--------Ejercicio 24 del tema 2 de las diapositivas
SemiEsfera::SemiEsfera(const int num_verts_per,const unsigned nperfiles){

ponerNombre( string("SemiEsfera") );
   vector<glm::vec3> perfil;
   float longitud = (M_PI/2)/(num_verts_per - 1); 
   for (int i = 0; i < num_verts_per; i++){
      perfil.push_back(glm::vec3(sin(i*longitud),cos(i*longitud),0.0));
   }
   inicializar(perfil, nperfiles);
  
}
//-----------------------------------

//---------------------------------Figuras inventadas--------------------------------------
ConoTruncado::ConoTruncado (const int num_verts_per,  const unsigned nperfiles, const float radio, const float altura, const float altura_corte){
   ponerNombre( std::string("cono truncado") );
   vector<glm::vec3> perfil;
   float longitud = 1.0/(num_verts_per - 1);
   for (int i = 0; i < num_verts_per; i++)
      perfil.push_back(glm::vec3((1-i*longitud)*radio + i*longitud*altura_corte,altura*i*longitud,0.0));
   inicializar(perfil, nperfiles);
  
}

DobleCono::DobleCono(const int num_verts_per, const unsigned nperfiles, const float radio, const float altura) {
    ponerNombre(std::string("doble cono"));


    vector<glm::vec3> perfil;
    float lon = 1.0 / ((num_verts_per / 2) - 1);

    // Primer cono
    for (int i = 0; i < num_verts_per / 2; i++)
        perfil.push_back(glm::vec3((1 - i * lon) * radio, altura * i * lon, 0.0));

    // Segundo cono (simétrico al primero)
    for (int i = num_verts_per / 2; i < num_verts_per; ++i)
        perfil.push_back(glm::vec3((1 - (i - num_verts_per / 2) * lon) * radio, altura * (i - num_verts_per / 2) * lon, 0.0));

    inicializar(perfil, nperfiles);
}

Anillo::Anillo(const int num_verts_per, const unsigned nperfiles, const float radioExterior, const float radioInterior, const float altura) {
    ponerNombre(std::string("anillo"));

   vector<glm::vec3> perfil;

    for (int i = 0; i < num_verts_per; ++i) {
        float theta = static_cast<float>(i) / (num_verts_per - 1) * 2 * glm::pi<float>();
        float x = glm::cos(theta);
        float y = glm::sin(theta);
        float r = radioInterior + (radioExterior - radioInterior) * static_cast<float>(i) / (num_verts_per - 1);

        perfil.push_back(glm::vec3(x * r, y * r, altura));
    }

    inicializar(perfil, nperfiles);
}

Elipsoide::Elipsoide(const int num_verts_per, const unsigned nperfiles){
    ponerNombre(std::string("elipsoide"));
    // Creación de tabla de vértices
   ponerNombre( string("Esfera") );
   vector<glm::vec3> perfil;
   float longitud = 2*M_PI/(num_verts_per - 1);
   for (int i = 0; i < num_verts_per; i++){
      perfil.push_back(glm::vec3(sin(i*longitud),2*cos(i*longitud),0.0));
   }
   inicializar(perfil, nperfiles);
}

Botella::Botella(const int num_verts_per, const unsigned nperfiles){
    ponerNombre(std::string("botella"));
 
   
   //Tronco de la botella
   vector<glm::vec3> perfil;
   float longitud = 1.0/(num_verts_per - 1);
   for (int i = 0; i < num_verts_per; i++){
      perfil.push_back(glm::vec3{0.5,i*longitud,0.0});
   }
   //Parte estrecha de la botella
   float x=0.5;
   for (int i = num_verts_per; i < 2*num_verts_per; i++){
      perfil.push_back(glm::vec3{x,i*longitud,0.0});
      x=x/1.1;
   }

   //Tapón esfera
   float radius = 0.1; // The radius of the sphere is the same as the top radius of the bottle
    for (int i = 0; i < num_verts_per; i++){
        float theta = static_cast<float>(i) / (num_verts_per - 1) * glm::pi<float>();
        float y = radius * glm::cos(theta) + 2.3; // The y-coordinate is offset by the height of the bottle
        float z = radius * glm::sin(theta);
        perfil.push_back(glm::vec3{0.0, y, z});
    }
   

   inicializar(perfil, nperfiles);
}
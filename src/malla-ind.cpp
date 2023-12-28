// *********************************************************************
// ** Nombre: María de las Angustias, Apellidos: Izquierdo García, Titulación: GIIADE
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Mallas indexadas (implementación)
// ** Copyright (C) 2016-2023 Carlos Ureña
// **
// ** Implementación de las clases 
// **        + MallaInd: malla indexada de triángulos (derivada de Objeto3D)
// **        + MallaPLY: malla indexada de triángulos, leída de un PLY (derivada de MallaInd)
// **        + algunas clases derivadas de MallaInd.
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
#include "aplicacion-ig.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"
#include "seleccion.h"   // para 'ColorDesdeIdent' 
#include <glm/glm.hpp>

using namespace glm;
using namespace std;


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }
   
   // // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // // ....
   // //MODIFICAR
   //  for (size_t i=0; i < triangulos.size(); i++)
   // {
   //    uvec3 tri = triangulos[i];
   //    unsigned int p = tri[0],
   //                 q = tri[1],
   //                 r = tri[2];

   //    vec3 a = vertices[q] - vertices[p];
   //    vec3 b = vertices[r] - vertices[p];
   //    vec3 m_c = glm::cross(a, b);
   //    vec3 n_c;

   //    if (length(m_c) != 0.0)
   //       n_c = normalize(m_c);
   //    else
   //       n_c = vec3(0, 0, 0);

   //    nor_tri.push_back(n_c);
   // }

 

   //COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   
   //  for(int i=0;i<nt;i++){
   //      uvec3 tri=triangulos.at(i);
   //      fvec3 q=vertices.at(tri[0]);
   //      fvec3 p=vertices.at(tri[1]);
   //      vec3 r=vertices.at(tri[2]);
   //      vec3 a=q-p;
   //      vec3 b=r-p;
   //      vec3 m;
   //      float m_factor;
   //      m[0]=(a[1]*b[2]-a[2]*b[1]);
   //      m[1]=(a[2]*b[0]-a[0]*b[2]);
   //      m[2]=(a[0]*b[1]-a[1]*b[0]);
   //      m_factor=std::sqrt(m[0]*m[0]+m[1]*m[1]+m[2]*m[2]);
   //      vec3 n=m/m_factor;
   //      nor_tri.push_back(n);
   //  }


    vec3 v_1,v_2,v_3;
   for(int i=0;i<triangulos.size();i++)
   {
      v_1 = vertices[triangulos.at(i)[1]] - vertices[triangulos.at(i)[0]];
      v_2 = vertices[triangulos.at(i)[2]] - vertices[triangulos.at(i)[0]];

      v_3 = cross(v_1,v_2);

      nor_tri.push_back(normalize(v_3));

   }

}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   using namespace glm ;
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......
   //MODIFICAR
   //  calcularNormalesTriangulos();

   // nor_ver.resize(vertices.size(), glm::vec3(0, 0, 0));

   // for (size_t i=0; i < triangulos.size(); i++)
   // {
   //    glm::uvec3 tri = triangulos[i];
   //    unsigned int v1 = tri[0],
   //                 v2 = tri[1],
   //                 v3 = tri[2];
                   
   //    nor_ver[v1] += nor_tri[i];
   //    nor_ver[v2] += nor_tri[i];
   //    nor_ver[v3] += nor_tri[i];
   // }

   // for (size_t i=0; i < nor_ver.size(); i++)
   //    if (glm::length(nor_ver[i]) != 0.0)
   //       nor_ver[i] = glm::normalize(nor_ver[i]);


   calcularNormalesTriangulos();
    std::vector<vec3> mv;
    for(int i=0;i<vertices.size();i++){
        vec3 t={0,0,0};
        mv.push_back(t);
    }
    for(int i=0;i<nor_tri.size();i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<3;k++){
                mv.at(triangulos.at(i)[j])[k]+=nor_tri.at(i)[k];
            }
        }
    }
    //normalizamos
    for(int i=0;i<vertices.size();i++){
        vec3 m_nor;
        vec3 m=mv.at(i);
        float m_factor=std::sqrt(m[0]*m[0]+m[1]*m[1]+m[2]*m[2]);
        m_nor=m/m_factor;
        mv.at(i)=m_nor;
    }
    nor_ver=mv;


}


// --------------------------------------------------------------------------------------------

void MallaInd::visualizarGL( )
{
   // comprobar algunas precondiciones básicas
   using namespace std ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce ; assert( cauce != nullptr );
   CError();

   // si la malla no vértices o no tiene triángulos, imprimir advertencia y salir.
   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   // COMPLETAR: práctica 1: cambiar color del cauce
   //
   // Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //    - hacer push del color actual del cauce
   //    - fijar el color en el cauce usando el color del objeto (se lee con 'leerColor()')

   if(tieneColor()){
      cauce->pushColor();
      cauce->fijarColor(leerColor());
   }


   // COMPLETAR: práctica 1: crear el descriptor de VAO, si no está creado
   //  Si el puntero 'dvao' es nulo, crear el descriptor de VAO
   //   * en primer lugar se crea el descriptor de VAO, con su constructor 
   //     (se le pasa como parámetro la tabla de posiciones y el número de atributos del cauce). 
   //   * se añade el descriptor de VBO con la tabla de índices (la tabla de triángulos),
   //   * finalmente se añaden al VAO los descriptores VBOs con tablas de atributos 
   //     que no estén vacías
   //  Si el VAO ya está creado, (dvao no nulo), no hay que hacer nada.
   //

      if(dvao == nullptr){
      /*a)*/ dvao = new DescrVAO(numero_atributos_cauce, new DescrVBOAtribs (ind_atrib_posiciones, vertices));
      /*b)*/ dvao->agregar(new DescrVBOInds (triangulos));
      /*c)*/ 
      if(cc_tt_ver.size() >0) dvao->agregar(new DescrVBOAtribs(ind_atrib_coord_text, cc_tt_ver));
      if(col_ver.size() >0) dvao->agregar(new DescrVBOAtribs(ind_atrib_colores, col_ver));
      if(nor_ver.size() >0) dvao->agregar(new DescrVBOAtribs(ind_atrib_normales, nor_ver));
         // COMPLETAR: práctica 1: visualizar el VAO usando el método 'draw' de 'DescrVAO'
 
   }

    dvao->draw(GL_TRIANGLES); //las mallas indexadas se representan con GL_TRIANGLES (DIAPOSITIVA 57 TEMA 2)
   // COMPLETAR: práctica 1: restaurar color anterior del cauce 
   //
   // Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //    - hacer 'pop' del color actual del cauce
   if(tieneColor()){
      cauce->popColor();
   }

}


// -----------------------------------------------------------------------------
// Visualizar el objeto con OpenGL
// usa las tablas de normales, colores y coordenadas de textura, si no están vacías.
      
void MallaInd::visualizarGeomGL( )
{
   // Comprobar que el descriptor de VAO ya está creado
   // (es decir, este método únicamente se podrá invocar después de que 
   // se haya llamado a 'visualizaGL')
   
   assert( dvao != nullptr );

   // COMPLETAR: práctica 1: visualizar únicamente la geometría del objeto 
   // TODO
   //    1. Desactivar todas las tablas de atributos del VAO (que no estén vacías)
   //dvao->habilitarAtrib(0,0); // El 0 no se puede deshabilitar
   if(cc_tt_ver.size() >0) dvao->habilitarAtrib(ind_atrib_coord_text,0);
   if(col_ver.size() >0)   dvao->habilitarAtrib(ind_atrib_colores,0);
   if(nor_ver.size() >0)   dvao->habilitarAtrib(ind_atrib_normales,0);
   //    2. Dibujar la malla (únicamente visualizará los triángulos)
  // glDrawElements(GL_TRIANGLES, triangulos.size() * 3, GL_UNSIGNED_INT , 0);
  dvao->draw(GL_TRIANGLES);
   //    3. Volver a activar todos los atributos para los cuales la tabla no esté vacía
   // ....
   if(cc_tt_ver.size() >0) dvao->habilitarAtrib(ind_atrib_coord_text,1);
   if(col_ver.size() >0)   dvao->habilitarAtrib(ind_atrib_colores,1);
   if(nor_ver.size() >0)   dvao->habilitarAtrib(ind_atrib_normales,1);

}

// -----------------------------------------------------------------------------
// Visualizar las normales del objeto, si no tiene tabla de normales imprime 
// advertencia y no hace nada.

void MallaInd::visualizarNormalesGL(  )
{
   using namespace std ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce ; assert( cauce != nullptr );

   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }  

   if( nor_ver.size() != vertices.size() )
   {
      cout << "Error visu. normales: tabla de normales no vacía y de tamaño distinto a la de vértices." << endl ;
      cout << "Nombre del objeto        : " << leerNombre() << endl ;
      cout << "Tamaño tabla vértices    : " << vertices.size() << endl ;
      cout << "Tamaño tabla de normales : " << nor_ver.size() << endl ;
      exit(1);
   }
   CError();

   // COMPLETAR: práctica 4: visualizar las normales del objeto MallaInd
   // 
   // *1* Si el puntero al descriptor de VAO de normales ('dvao_normales') es nulo, 
   //    debemos de crear dicho descriptor, con estos pasos:
   //
   //       * Para cada posición 'v_i' de un vértice en el vector 'vertices':
   //             - Leer la correspondiente normal 'n_i' del vector de normales ('nor_ver').
   //             - Añadir 'v_i' al vector 'segmentos_normales'.
   //             - Añadir 'v_i+a*n_i' al vector 'segmentos_normales'.
   //
   //       * Crear el objeto descriptor del VAO de normales, para ello se usa el vector 
   //          'segmentos_normales' y se tiene en cuenta que esa descriptor únicamente gestiona 
   //          una tabla de atributos de vértices (la de posiciones, ya que las otras no se 
   //          necesitan).
   
   if (dvao_normales == nullptr)
   {
      float a = 1.0f;
      for (size_t i=0; i < vertices.size(); i++)
      {
         segmentos_normales.push_back(vertices[i]);
         segmentos_normales.push_back(vertices[i]+a*nor_ver[i]);
      }
      dvao_normales = new DescrVAO( 1, new DescrVBOAtribs( ind_atrib_posiciones, segmentos_normales )); 

   }

   // *2* Visualizar el VAO de normales, usando el método 'draw' del descriptor, con el 
   //       tipo de primitiva 'GL_LINES'.
   dvao_normales->draw(GL_LINES);
   //  ..........

}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores 
// basados en los identificadores de los objetos
void MallaInd::visualizarModoSeleccionGL() 
{

   using namespace std ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 5: visualizar la malla en modo selección 
   //
   // Se debe escribir código para visualizar únicamente la geometría, pero usando el color 
   // obtenido a partir del identificador. El código da estos pasos:
   // 
   // 1. Leer el identificador del objeto (con 'leerIdentificador'). Si el objeto tiene 
   //    identificador (es decir, si su identificador no es -1)
   //       + Hacer push del color del cauce, con 'pushColor'.
   //       + Fijar el color del cauce (con 'fijarColor') usando un color obtenido a 
   //         partir del identificador (con 'ColorDesdeIdent'). 
   // 2. Invocar 'visualizarGeomGL' para visualizar la geometría.
   // 3. Si tiene identificador: hacer pop del color, con 'popColor'.
   //
   int identificador = leerIdentificador();
   if (identificador != -1)
   {
      cauce->pushColor();
      cauce->fijarColor(ColorDesdeIdent(identificador));
   }
   visualizarGeomGL();
   if (identificador != -1)
      cauce->popColor();

}


// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................
    LeerPLY(nombre_arch,vertices,triangulos);

   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................
   calcularNormales();
}

// ****************************************************************************
// Clase 'Cubo

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;
   calcularNormales();
}

Tetraedro::Tetraedro()
:  MallaInd( "tetraedro" )
{
   float a = 1.0;
   
      vertices = {{-a,0.0,-a}, {a,0.0,-a},
            {0.0,0.0,a}, {0.0,a*sqrt(3.0),0.0}};

      triangulos =
      {  
         {3, 0, 1},
        {2, 3, 1},
         {1, 0, 2},
         {2, 0, 3}
      } ;

      ponerColor({1.0f, 0.0f, 1.0f});
   calcularNormales();

}


CuboColores::CuboColores()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

      col_ver =
      {  {  0.0, 0.0, 0.0 }, // 0
         { 0.0, 0.0, +1.0 }, // 1
         { 0.0, +1.0, -0.0 }, // 2
         { 0.0, +1.0, +1.0 }, // 3
         { +1.0, 0.0, 0.0 }, // 4
         { +1.0, 0.0, +1.0 }, // 5
         { +1.0, +1.0, 0.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;
}


EstrellaZ::EstrellaZ(unsigned n)
:  MallaInd( "Estrella de n puntas" )
{

   float angulo =  M_PI / n;
   float radio = 0.5f; // radio de la estrella
   float centro_ejex = 0.5f;
   float centro_ejey = 0.5f;
   float centro_ejez = 0.0f;

   for ( int i = 0; i < 2*n; i++ ){
      if ( i % 2 == 0)
         vertices.push_back({centro_ejex + radio*cos(i*angulo), centro_ejey + radio*sin(i*angulo), centro_ejez});
      else
         vertices.push_back({centro_ejex + radio/2.0f*cos(i*angulo), centro_ejey + radio/2.0f*sin(i*angulo), centro_ejez});
   }

   vertices.push_back({centro_ejex, centro_ejey, centro_ejez});
  

  // Triángulos

    for ( int i = 0; i < 2*n+1 ; i++ )
      triangulos.push_back({i,(i+1)%(2*n),2*n});

    for ( int i = 1; i < 2*n+1; i++)
       col_ver.push_back(vertices[i]);

   col_ver.push_back({1.0,1.0,1.0});


}

EstrellaX::EstrellaX(unsigned n,float radio, float centro_ejey, float centro_ejez)
:  MallaInd( "Estrella de n puntas" )
{

   float angulo =  M_PI / n;
   float centro_ejex = 0.0f;

   for ( int i = 0; i < 2*n; i++ ){
      if ( i % 2 == 0)
         vertices.push_back({centro_ejex + radio*cos(i*angulo), centro_ejey + radio*sin(i*angulo), centro_ejez});
      else
         vertices.push_back({centro_ejex + radio/2.0f*cos(i*angulo), centro_ejey + radio/2.0f*sin(i*angulo), centro_ejez});
   }

   vertices.push_back({centro_ejex, centro_ejey, centro_ejez});
  

  // Triángulos

    for ( int i = 0; i < 2*n+1 ; i++ )
      triangulos.push_back({i,(i+1)%(2*n),2*n});

    for ( int i = 1; i < 2*n+1; i++)
       col_ver.push_back(vertices[i]);

   col_ver.push_back({1.0,1.0,1.0});


}


CasaX::CasaX()
:  MallaInd( "Casa X" )
{
   
   vertices =
      {  { 0.0, 0.0, 1.0 }, // A 0
         { 0.0, 1.0, +1.0 }, // B 1
         { 0.0, 0.0, 0.0 }, // C 2 
         { 0.0, +1.0, 0.0 }, // D 3
         { +1.5, 0.0, 1.0 }, // E 4
         { +1.5, 1.0, 1.0 }, // F 5
         { +1.5, 0.0, 0.0 }, // G 6
         { +1.5, +1.0, 0.0 }, // H 7
         {1.5,1.5,0.5}, //8
         {0,1.5,0.5} //9
      } ;

   triangulos ={
        {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Z
         {2,3,7}, {2,7,6}, // Z+ (+2)

         {5,8,7},
         {8,1,5},
         {9,1,8},
         {9,1,3},
         {9,3,7},{7,9,8}
      } ;

      col_ver =
      {  {  0.0, 0.0, 0.0 }, // 0
         { 0.0, 0.0, +1.0 }, // 1
         { 0.0, +1.0, -0.0 }, // 2
         { 0.0, +1.0, +1.0 }, // 3
         { +1.0, 0.0, 0.0 }, // 4
         { +1.0, 0.0, +1.0 }, // 5
         { +1.0, +1.0, 0.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
         { +1.0, +1.0, 0.0 }, // 6
         { +1.0, +1.0, +1.0 } // 7
      } ;
}

MallaTriangulo::MallaTriangulo()
:  MallaInd( "Malla Triangulo" )
{

   vertices =
      {  { 1, 0.0, 0.0 }, // 0
      { 0.0, 2.0, 0.0 }, // 0
      { -1.0, 0.0, 0.0 } // 0
      } ;

   triangulos ={
        {0,1,2} 
      } ;


}

MallaCuadrado::MallaCuadrado()
:  MallaInd( "Malla Cuadrado" )
{

   vertices =
      {  { 0.5, 0.0, 0.0 }, // 0
      { 0.5, 1, 0.0 }, // 1
      { -0.5, 1, 0.0 }, // 2
       { -0.5, 0.0, 0.0 } // 3
      } ;


   triangulos ={
        {0,1,2} ,
        {2,3,0}
      } ;

}

MallaPiramideL::MallaPiramideL()
:  MallaInd( "Malla Piramide" )
{


   vertices =
      {    
         //BASE L
         { +1.5, 0.0, 0.7 }, // 0
         { +1.5, 0.0, 0.0 }, // 1
         { 0.0, 0.0, 0.0 }, // 2
         { 0, 0, 0.7 }, // 3
         //CUADRADO DE LA BASE L
         {0,0,0.7}, //4
         {0,0,1.4}, //5
         {0.7,0,0.7}, //6
         {0.7,0,1.4},  //7
         {0,2,0} //8

         
      } ;


   triangulos ={
       {0,1,2},
       {2,3,0},
       {4,5,6},
       {6,5,7},
       {2,1,8}, //Triangulo eje XY
      {0,1,8},
      {5,2,8}, //Triangulo eje ZY
      {7,5,8},
      {6,0,8},
      {6,7,8}
      } ;


}








PiramideEstrellaZ::PiramideEstrellaZ(unsigned n)
:  MallaInd( "Malla Piramide" )
{

   vertices = {};
   int indice = 0;


   for(unsigned i = 0; i < n; ++i)
   {
      // Vértices lejanos al centro
      vertices.push_back(  {0.5 + (float) cos(2*i*M_PI/n) * 0.5f,
                           0.5 + (float) sin(2*i*M_PI/n) * 0.5f, 0.0f});

      // Color de los vértices lejanos
      col_ver.push_back(vertices.back());

      // Vértices cercanos al centro
      vertices.push_back(  {0.5 + (float) cos(2*(i+.5)*M_PI/n) * 0.25f,
                           0.5 + (float) sin(2*(i+0.5)*M_PI/n) * 0.25f, 0.0f});

      // Color de los vértices cercanos
      col_ver.push_back(vertices.back());

      // Triángulo par
      triangulos.push_back({2*n,  indice, (indice + 1)%(2*n)});

      // Triángulo impar
      triangulos.push_back({2*n,  (indice + 1)%(2*n), (indice+ 2)%(2*n)});

      indice+= 2;
   }


   vertices.push_back({0.5,0.5,0.0}) ;   // 2*n+1: centro
   col_ver.push_back({1.0,1.0,1.0});     // El centro es blanco

   assert(vertices.size() == 2*n + 1);
   assert(triangulos.size() == 2*n);

   // Dibujamos la pirámide

   vertices.push_back({0.5,0.5,0.5});    // 2*n+2: ápice
   col_ver.push_back({1,1,1});     // El ápice es blanco


   for(unsigned i = 0; i < 2*n-1; i += 2)
   {
      // Juntamos todo
      triangulos.push_back({i, (i+1)%(2*n), 2*n+1});
      triangulos.push_back({i, (i-1+2*n)%(2*n), 2*n+1});
   }

   assert(vertices.size() == 2*n + 2);
   assert(triangulos.size() == 4*n);

}



//Cuadrado en el ejer XY
Cuadrado::Cuadrado() : MallaInd(){
   vertices={  {0,0,0}, {0,1,0}, {1,1,0}, {1,0,0} };
   triangulos={ {0,1,3}, {3,1,2} };
   

}

//Triangulo inscrito en el cuadrado anterior, pero no cortando el cuadrado en una diagonal, sino un poco más
//hacia arriba
Triangulo::Triangulo() : MallaInd(){

vertices={ {0.5,0.5,0}, {0.5,1,0}, {1,0.5,0} };
triangulos={ {0,1,2} };

}


Piramide::Piramide(){
   ponerNombre("Piramide");
   
     vertices.push_back({+.5,-0.5,+.5});// 0
      vertices.push_back({-.5,-0.5,+.5});// 1
      vertices.push_back({-.5,-0.5,-.5});// 2
      vertices.push_back({+.5,-0.5,-.5});// 3
      vertices.push_back({0,0.42,0});//cúspide

      triangulos.push_back({0,1,3});
      triangulos.push_back({1,3,2});
      triangulos.push_back({0,1,4});
      triangulos.push_back({1,4,2});
      triangulos.push_back({2,3,4});
      triangulos.push_back({0,3,4});

      //hay que poner exactamente el mismo numero de coordenadas que de vértices
      cc_tt_ver={
         {0,0.1}, //0
         {0,0}, //1
         {0,0}, //2
         {0,0}, 
         {0,0}, 
      };

}



//--------EJERCICIO CONVOCATORIA ORDINARIA 2020-2021-------------
RejillaY::RejillaY(unsigned n, unsigned m)
    : MallaInd("Rejilla Y")
    {

         assert(n > 1 && m > 1);
         float x, z;
         float dx = 1.0f / (n - 1);
         float dz = 1.0f / (m - 1);
         for (unsigned i = 0; i < n; ++i)
         {
               x = i * dx;
               for (unsigned j = 0; j < m; ++j)
               {
                  z = j * dz;
                  vertices.push_back({x, 0.0f, z});
                  col_ver.push_back({x, 0.0f, z});
               }
         }
         for (unsigned i = 0; i < n - 1; ++i)
         {
               for (unsigned j = 0; j < m - 1; ++j)
               {
                  triangulos.push_back({i * m + j, i * m + j + 1, (i + 1) * m + j});
                  triangulos.push_back({i * m + j + 1, (i + 1) * m + j + 1, (i + 1) * m + j});
               }
         }
      
   
    
}


RejillaX::RejillaX(unsigned n, unsigned m)
    : MallaInd("Rejilla X")
    {

      assert(n > 1 && m > 1);
    float y, z;
    float dy = 1.0f / (n - 1);
    float dz = 1.0f / (m - 1);
    for (unsigned i = 0; i < n; ++i) {
      y = i * dy;
      for (unsigned j = 0; j < m; ++j) {
        z = j * dz;
        vertices.push_back({0.0f, y, z});
        col_ver.push_back({0.0f, y, z});
      }
    }
    for (unsigned i = 0; i < n - 1; ++i) {
      for (unsigned j = 0; j < m - 1; ++j) {
        triangulos.push_back({i * m + j, (i + 1) * m + j, i * m + j + 1});
        triangulos.push_back({(i + 1) * m + j, i * m + j + 1, (i + 1) * m + j + 1});
      }
    }
      
   
    
}

RejillaZ::RejillaZ(unsigned n, unsigned m)
    : MallaInd("Rejilla Z")
    {

    assert(n > 1 && m > 1);
        float x, y;
        float dx = 1.0f / (n - 1);
        float dy = 1.0f / (m - 1);
        for (unsigned i = 0; i < n; ++i)
        {
            x = i * dx;
            for (unsigned j = 0; j < m; ++j)
            {
                y = j * dy;
                vertices.push_back({x, y, 0.0f});
                col_ver.push_back({x, y, 0.0f});
            }
        }
        for (unsigned i = 0; i < n - 1; ++i)
        {
            for (unsigned j = 0; j < m - 1; ++j)
            {
                triangulos.push_back({i * m + j, (i + 1) * m + j, i * m + j + 1});
                triangulos.push_back({(i + 1) * m + j + 1, i * m + j + 1, (i + 1) * m + j});
            }
        }
      
   
    
}

Cubo24::Cubo24(){

        vertices ={
      //cara +z
      { -1.0, -1.0, 1.0 }, //0
      { 1.0, -1.0, 1.0 },  //1
      { 1.0, 1.0, 1.0 },  //2
      { -1.0, 1.0, 1.0 }, //3
      //cara -z
      { -1.0, -1.0, -1.0 }, //4
      { 1.0, -1.0, -1.0 },  //5
      { 1.0, 1.0, -1.0 },   //6
      { -1.0, 1.0, -1.0 },  //7
      //cara +y
       { -1.0, 1.0, -1.0 },   //8
      { 1.0, 1.0, -1.0 },   //9
      { 1.0, 1.0, 1.0 },  //10
      { -1.0, 1.0, 1.0 },   //11
      //cara -y
      { -1.0, -1.0, -1.0 }, //12
      { 1.0, -1.0, -1.0 },    //13
      { 1.0, -1.0, 1.0 },    //14
      { -1.0, -1.0, 1.0 },    //15
      //cara +x
      { 1.0, -1.0, -1.0 }, //16
      { 1.0, -1.0, 1.0 },  //17
      { 1.0, 1.0, 1.0 },   //18
      { 1.0, 1.0, -1.0 },  //19
      //cara -x
      { -1.0, -1.0, -1.0 }, //20
      { -1.0, -1.0, 1.0 },  //21
      { -1.0, 1.0, 1.0 },   //22
      { -1.0, 1.0, -1.0 }  //23
      } ;

      triangulos =
      {
       {1,3,0}, {1,2,3}, //cara +z
       {5,4,7}, {5,7,6}, //cara -z
      {9,8,11}, {9,11,10}, //cara +y
      {12,13,15}, {13,14,15}, //cara -y
      {17,19,16}, {17,18,19}, //cara +x
      {21,20,23}, {21,23,22}, //cara -x
      } ;
      

    

       cc_tt_ver=
      {
      {0,1}, {1,1}, {1,0}, {0,0}, //cara +Z
      {1,1}, {0,1}, {0,0}, {1,0}, //cara -Z hay que darle la vuelta a las texturas de la cara +Z
      {0,0},{1,0},{1,1},{0,1}, //cara +y
      {0,1}, {1,1}, {1,0}, {0,0},//cara -y 
      {1,1},{0,1},{0,0},{1,0}, //cara +x
      {0,1}, {1,1}, {1,0}, {0,0},//cara -x
    
      };

      

      calcularNormales();

}



MallaDiscoP4::MallaDiscoP4(){
ponerColor({1.0,1.0,1.0});
   const unsigned ni=23, nj =31;

   for(unsigned i=0; i<ni; i++){
      for(unsigned j=0; j< nj;j++){
         const float fi = float(i)/float(ni-1), fj = float(j)/float(nj-1), ai=2.0*M_PI*fi, x=fj * cos(ai), y=fj * sin(ai), z=0.0;
         vertices.push_back({x,y,z});
         cc_tt_ver.push_back({x, y}); //PARA QUE APAREZCA IGUAL QUE EN LA IMAGEN
      }
   }

   for(unsigned i=0;i<ni-1;i++){
      for(unsigned j=0; j<nj-1;j++){
         triangulos.push_back({i*nj+j,i*nj+(j+1),(i+1)*nj+(j+1)});
         triangulos.push_back({i*nj+j,(i+1)*nj+(j+1),(i+1)*nj+j});
      }

   }


}

MallaDiscoP24::MallaDiscoP24(){
ponerColor({1.0,1.0,1.0});
   const unsigned ni=23, nj =31;

   for(unsigned i=0; i<ni; i++){
      for(unsigned j=0; j< nj;j++){
         const float fi = float(i)/float(ni-1), fj = float(j)/float(nj-1), ai=2.0*M_PI*fi, x=fj * cos(ai), y=fj * sin(ai), z=0.0;
         vertices.push_back({x,y,z});
         cc_tt_ver.push_back({fi, fj}); 
      }
   }

   for(unsigned i=0;i<ni-1;i++){
      for(unsigned j=0; j<nj-1;j++){
         triangulos.push_back({i*nj+j,i*nj+(j+1),(i+1)*nj+(j+1)});
         triangulos.push_back({i*nj+j,(i+1)*nj+(j+1),(i+1)*nj+j});
      }
   }

}

MallaPrueba::MallaPrueba(){

        vertices ={
      //cara +z
      { -1.0, -1.0, 1.0 }, //0
      { 1.0, -1.0, 1.0 },  //1
      { 1.0, 1.0, 1.0 },  //2
      { -1.0, 1.0, 1.0 }, //3
      //cara -z
      { -1.0, -1.0, -1.0 }, //4
      { 1.0, -1.0, -1.0 },  //5
      { 1.0, 1.0, -1.0 },   //6
      { -1.0, 1.0, -1.0 },  //7
      //cara +y
       { -1.0, 1.0, -1.0 },   //8
      { 1.0, 1.0, -1.0 },   //9
      { 1.0, 1.0, 1.0 },  //10
      { -1.0, 1.0, 1.0 },   //11
      //cara -y
      { -1.0, -1.0, -1.0 }, //12
      { 1.0, -1.0, -1.0 },    //13
      { 1.0, -1.0, 1.0 },    //14
      { -1.0, -1.0, 1.0 },    //15
      //cara +x
      { 1.0, -1.0, -1.0 }, //16
      { 1.0, -1.0, 1.0 },  //17
      { 1.0, 1.0, 1.0 },   //18
      { 1.0, 1.0, -1.0 },  //19
      //cara -x
      { -1.0, -1.0, -1.0 }, //20
      { -1.0, -1.0, 1.0 },  //21
      { -1.0, 1.0, 1.0 },   //22
      { -1.0, 1.0, -1.0 }  //23
      } ;

      triangulos =
      {
       {1,3,0}, {1,2,3}, //cara +z
       {5,4,7}, {5,7,6}, //cara -z
      {9,8,11}, {9,11,10}, //cara +y
      {12,13,15}, {13,14,15}, //cara -y
      {17,19,16}, {17,18,19}, //cara +x
      {21,20,23}, {21,23,22}, //cara -x
      } ;
      

    

       cc_tt_ver=
      {
      {0,1}, {1,1}, {1,0}, {0,0}, //cara +Z
      {1,1}, {0,1}, {0,0}, {1,0}, //cara -Z hay que darle la vuelta a las texturas de la cara +Z
      {0,0},{1,0},{1,1},{0,1}, //cara +y
      {0,1}, {1,1}, {1,0}, {0,0},//cara -y 
      {1,1},{0,1},{0,0},{1,0}, //cara +x
      {0,1}, {1,1}, {1,0}, {0,0},//cara -x
    
      };

      

      calcularNormales();

}

// -----------------------------------------------------------------------------------------------

//----------------------FIGURAS INVENTADAS-----------------------------------

//Trapecio con centro en el origen y coordenadas del eje Y positivas
Trapecio::Trapecio():MallaInd("Trapecio"){
   vertices={
      //Base trapecio
      {1.5,0,1},//0
      {1.5,0,-1},//1
      {-1.5,0,-1},//2
      {-1.5,0,1},//3

      //Techo trapecio
      {0.7,0.7,0.7},//4
      {0.7,0.7,-0.7},//5
      {-0.7,0.7,0.7},//6
      {-0.7,0.7,-0.7}//7
   };

   triangulos={
      {0,1,2},
      {0,2,3},
      {4,5,7},
      {4,7,6},
      {0,4,6},{0,6,3},
      {1,4,0},{1,5,4},
      {1,2,7},{1,7,5},
      {2,6,7},{2,3,6}
   };

         col_ver =
      {  {  0.0, 0.0, 0.0 }, // 0
         { 0.0, 0.0, +1.0 }, // 1
         { 0.0, +1.0, -0.0 }, // 2
         { 0.0, +1.0, +1.0 }, // 3
         { +1.0, 0.0, 0.0 }, // 4
         { +1.0, 0.0, +1.0 }, // 5
         { +1.0, +1.0, 0.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;

      cc_tt_ver=
      {
      {0,3}, {0.5,0.5}, {1,0.5}, {0,0}, //cara +Z
      {0,0}, {0,0}, {0,0}, {0,0}, 
    
      
    
      };

}


TrapecioSinTecho::TrapecioSinTecho():MallaInd("Trapecio"){
   vertices={
      //Base trapecio
      {1.5,0,1},//0
      {1.5,0,-1},//1
      {-1.5,0,-1},//2
      {-1.5,0,1},//3

      //Techo trapecio
      {0.7,0.7,0.7},//4
      {0.7,0.7,-0.7},//5
      {-0.7,0.7,0.7},//6
      {-0.7,0.7,-0.7}//7
   };

   triangulos={
      {0,1,2},
      {0,2,3},
      {4,5,7},
      {4,7,6},
      {0,4,6},{0,6,3},
      {1,4,0},{1,5,4},
      {1,2,7},{1,7,5},
      {2,6,7},{2,3,6}
   };

         col_ver =
      {  {  0.0, 0.0, 0.0 }, // 0
         { 0.0, 0.0, +1.0 }, // 1
         { 0.0, +1.0, -0.0 }, // 2
         { 0.0, +1.0, +1.0 }, // 3
         { +1.0, 0.0, 0.0 }, // 4
         { +1.0, 0.0, +1.0 }, // 5
         { +1.0, +1.0, 0.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;

      cc_tt_ver=
      {
      {0,1}, {1,1}, {1,0}, {0,0}, //cara +Z
      {0,0}, {0,0}, {0,0}, {0,0}, 
    
      
    
      };

}

PoligonoRegular::PoligonoRegular(unsigned n):MallaInd(){

 const double pi = 3.14159265358979323846;
    const double angle = 2 * pi / n;

    for (unsigned int i = 0; i < n; i++) {
        double x = std::cos(i * angle);
        double y = std::sin(i * angle);
        double z = 0;
        vertices.push_back(vec3(x, y,z));
        triangulos.push_back({0,i,(i+1)%n});
    }

   
}



//Prisma pentagonal con piramide en la cuspide
PrismaPentagonal::PrismaPentagonal(unsigned altura):MallaInd(){

 for (int i = 0; i < 5; i++) {
            double x = cos(2 * M_PI * i / 5);
            double y = sin(2 * M_PI * i / 5);
            vertices.push_back(vec3(x, y, 0));
        }

        // Calculamos las coordenadas de los vértices de las caras laterales
        for (int i = 0; i < 5; i++) {
            double x = vertices[i].x;
            double y = vertices[i].y;
            vertices.push_back(vec3(x, y, altura));
        }

        // Calculamos los triángulos de la base
        for (int i = 0; i < 4; i++) {
            triangulos.push_back({0, i + 1, (i + 2) % 5});
        }

        // Calculamos los triángulos de las caras laterales
        for (int i = 0; i < 5; i++) {
            triangulos.push_back({i, (i + 1) % 5, 5 + i});
            triangulos.push_back({5 + i, (i + 1) % 5, 5 + (i + 1) % 5});
        }
    
      vertices.push_back(vec3(0,0,altura*1.25));

      // triangulos.push_back({5,6,10});
      //  triangulos.push_back({6,7,10});
      //   triangulos.push_back({7,8,10});
      //    triangulos.push_back({8,9,10});


      for (int i = 5; i < 9; i++) {
    triangulos.push_back({i, i + 1, 10});
   }

   
}

PrismaHexagonal::PrismaHexagonal(unsigned altura){

    for (int i = 0; i < 6; i++) {
        double x = cos(2 * M_PI * i / 6);
        double y = sin(2 * M_PI * i / 6);
        vertices.push_back(vec3(x, 0, y));  // Swap the y and z coordinates
    }

    // Calculamos las coordenadas de los vértices de las caras laterales
    for (int i = 0; i < 6; i++) {
        double x = vertices[i].x;
        double z = vertices[i].z;  // Swap the y and z coordinates
        vertices.push_back(vec3(x, altura, z));  // Swap the y and z coordinates
    }

    // Calculamos los triángulos de la base
    for (int i = 0; i < 5; i++) {
        triangulos.push_back({0, i + 1, (i + 2) % 6});
    }

    // Calculamos los triángulos de las caras laterales
    for (int i = 0; i < 6; i++) {
        triangulos.push_back({i, (i + 1) % 6, 6 + i});
        triangulos.push_back({6 + i, (i + 1) % 6, 6 + (i + 1) % 6});
    }

    // Calculamos los triángulos del techo
    for (int i = 0; i < 5; i++) {
        triangulos.push_back({6, 6 + i + 1, 6 + (i + 2) % 6});
    }

    vertices.push_back(vec3(0, altura * 1.25, 0));  

}



Octaedro::Octaedro():MallaInd(){

   vertices={
      {0,1,0},//0
   {1,0,0},//1
   {-1,0,0},//2
   {0,0,-1},//3
   {0,0,1},//4
   {0,-1,0},//5

   };

   triangulos={
      {0,1,3},
      {0,3,2},
      {0,2,4},
      {0,4,1},
      {5,1,4},
      {5,3,1},
      {5,2,3},
      {5,4,2}
   };

}

Prisma::Prisma()
    : MallaInd("Prisma")
{    
    vertices.push_back({0.0, 0.0, 0.0});
    vertices.push_back({1.0, 0.0, 0.0});
    vertices.push_back({1.0, 1.0, 0.0});
    vertices.push_back({1.0, 1.0, 1.0});
    vertices.push_back({1.0, 0.0, 1.0});
    vertices.push_back({0.0, 0.0, 1.0});

    triangulos.push_back({0, 1, 2});
    triangulos.push_back({0, 2, 3});
    triangulos.push_back({0, 3, 5});
    triangulos.push_back({0, 5, 4});
    triangulos.push_back({0, 4, 1});
    triangulos.push_back({5, 3, 4});
    triangulos.push_back({4, 3, 1});
    triangulos.push_back({1, 3, 2});
    
}

PiramideHexagonal::PiramideHexagonal(unsigned altura) : MallaInd("Piramide Hexagonal"){

   for (int i = 0; i < 6; i++) {
        double x = cos(2 * M_PI * i / 6);
        double y = sin(2 * M_PI * i / 6);
        vertices.push_back(vec3(x, 0, y));  // Swap the y and z coordinates
    }

   vertices.push_back(vec3(0,altura,0));

   // Calculamos los triángulos de la base
    for (int i = 0; i < 5; i++) {
        triangulos.push_back({0, i + 1, (i + 2) % 6});
    }

      // Calculamos los triángulos de las caras laterales
      for (int i = 0; i < 6; i++) {
         triangulos.push_back({i, (i + 1) % 6, 6});
      }
   

}

Paralelepipedo::Paralelepipedo() : MallaInd(){
   
      vertices={
         //Base
         {0,0,0}, //0
         {1.5,0,0}, //1
         {0,0,1}, //2
         {1.5,0,1}, //3
         //Techo
         {0.5,1,0},//4
         {2,1,0}, //5
         {0.5,1,1}, //6
         {2,1,1}, //7
      };
   
      triangulos={
       {0,1,3},
       {0,2,3},
       {4,5,7},
       {4,6,7},
       {5,7,1},
       {7,3,1},
       {6,3,7},
       {6,2,3},
       {4,2,6},
       {4,0,2},
       {4,1,0},
       {4,5,1}
      };

}

PiramideHeptagonal::PiramideHeptagonal(unsigned altura) : MallaInd(){
for (int i = 0; i < 7; i++) {
        double x = cos(2 * M_PI * i / 7);
        double y = sin(2 * M_PI * i / 7);
        vertices.push_back(vec3(x, 0, y));  // Swap the y and z coordinates
    }

   vertices.push_back(vec3(0,altura,0));

   // Calculamos los triángulos de la base
    for (int i = 0; i < 6; i++) {
        triangulos.push_back({0, i + 1, (i + 2) % 7});
    }

      // Calculamos los triángulos de las caras laterales
      for (int i = 0; i < 7; i++) {
         triangulos.push_back({i, (i + 1) % 7, 7});
      }

}

PanelAncho::PanelAncho()
:  MallaInd( "Panel ancho" )
{

   vertices =
      {  {0.0 , 0.0 , 0.0}, //0
         {3.0 , 0.0 , 0.0}, //1
         {0.0 , 2.0 , 0.0}, //2
         {3.0 , 2.0 , 0.0}  //3
      } ;



   triangulos =
      {  
         {0,1,2}, {1,3,2}
      } ;

}

PanelEstrecho::PanelEstrecho()
:  MallaInd( "Panel estrecho" )
{

   vertices =
      {  {0.0 , 0.0 , 0.0}, //0
         {0.0 , 0.0 , 2.0}, //1
         {0.0 , 2.0 , 0.0}, //2
         {0.0 , 2.0 , 2.0}  //3
      } ;



   triangulos =
      {  
         {0,1,2}, {1,3,2}
      } ;

}

//-----------------------------------------------------------------------------


//EJERCICIOS EXAMENES

//Ejercicio examen 2022 de prácticas 1,2 y 3 (Práctica 1)
CuboPuntas::CuboPuntas()
:  MallaInd( "cubo puntas" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
         {6,0,0},//8
         {0,0,-6}, //9
         {-6,0,0},//10
         {0,0,6}//11
      } ;



   triangulos =
      {
         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)
         {4,6,8},{7,6,8},{5,7,8},{4,5,8},
         {4,9,6},{6,9,2},{2,9,0},{4,9,0},
         {0,10,2},{2,10,3},{3,10,1},{0,10,1},
         {7,3,11},{3,1,11},{1,5,11},{5,7,11}

      } ;
         

   for(int i=0; i<vertices.size(); i++){
      col_ver.push_back( { vertices[i][0] / 12.0 + 0.5, (vertices[i][0] / 12.0 + 0.5)/2 + (vertices[i][2] / 12.0 + 0.5)/2, vertices[i][1] * 0.5 + 0.5});
   
   }
}

//Ejercicio examen 2022 de prácticas 1,2 y 3 (Práctica 1)
PiramideCruz::PiramideCruz()
:  MallaInd( "PiramideCruz" )
{

      vertices =
      {  
         {0,4,0}, //0
         {1,0,3}, //1
         {1,0,1}, //2
         {3,0,1}, //3
         {3,0,-1},//4
         {1,0,-1},//5
         {1,0,-3}, //6
         {-1,0,-3}, //7
         {-1,0,-1}, //8
         {-3,0,-1}, //9
         {-3,0,1}, //10
         {-1,0,1}, //11
         {-1,0,3}, //12
         {0,0,0} //13
      } ;



  triangulos =
      {  
         {0,2,1},
         {0,3,2},
         {0,4,3},
         {0,5,4}, 
         {0,6,5},
         {0,7,6},
         {0,8,7},
         {0,9,8},
         {0,10,9},
         {0,11,10},
         {0,12,11},
         {0,12,1},

         //BASE LOS TRIANGULOS DE LA BASE CONSISTEN EN QUE TODOS LOS PUNTOS DE LA BASE VAN HACIA EL PUNTO (0,0,0)
         {13,12,1},
         {13,1,2},
         {13,2,3},
         {13,3,4},
         {13,4,5},
         {13,5,6},
         {13,6,7},
         {13,7,8},
         {13,8,9},
         {13,9,10},
         {13,10,11},
         {13,11,12},
      } ;

         col_ver =
      {  { 0.5,  1,     0.5 }, // 0
         { 0.33,  0.5,     0.66 }, // 1
         { 0.0,   0.5,     0.66 }, // 2
         { 0.0,   0.5,     0.33 }, // 3
         { 0.33,  0.5,     0.33 }, // 4
         { 0.33,  0.5,     0 }, // 5
         { 0.66,  0.5,     0 }, // 6
         { 0.66,  0.5,     0.33 }, // 7
         { 1.0,   0.5,     0.33 }, // 8
         { 1.0,   0.5,     0.66 }, // 9
         { 0.66,  0.5,     0.66 }, // 10
         { 0.66,  0.5,     1 }, // 11
         { 0.5,   0.5,     0.5}, // 12
         { 0.5,   0.5,     0.5}  // 13
      } ;

}


//Ejercicio examen curso 2022 extraordinaria (Practica 2)
P2_Malla::P2_Malla(int s, int n)
{
   for(int i=1;i<=n;i++){
      //cara abajo
      vertices.push_back({ i*-1.0, -1.0, i*-1.0 }); //0
      vertices.push_back({ i*1.0, -1.0, i*-1.0 }); //1
      vertices.push_back({ i*1.0, -1.0, i*1.0 }); //2
      vertices.push_back( { i*-1.0, -1.0, i*1.0 }); //3

      //cara detrás
      vertices.push_back({ i*-1.0, -1.0, i*-1.0 }); //4
      vertices.push_back({i*1.0, -1.0, i*-1.0 }); //5
      vertices.push_back({ i*0.4, 1.0, i*-0.4 });//6
      vertices.push_back( { i*-0.4, 1.0, i*-0.4 });//7

      //cara delante
      vertices.push_back( { i*1.0, -1.0, i*1.0 });//8
      vertices.push_back({ i*1.0, -1,i*1.0 });//9
      vertices.push_back({ i*0.4, 1.0, i*0.4 });//10
      vertices.push_back( { i*-0.4, 1.0, i*0.4 });//11

      //cara izquierda
      vertices.push_back({ i*-1.0, -1.0, i*-1.0 });//12
      vertices.push_back({ i*-1.0, -1.0, i*1.0 });//13
      vertices.push_back({ i*-0.4, 1.0, i*0.4 });//14
      vertices.push_back({ i*-0.4, 1.0, i*-0.4 });//15

      //cara derecha
      vertices.push_back({ i*1.0, -1.0, i*-1});//16
      vertices.push_back({ i*1.0, -1.0, i*1.0 });//17
      vertices.push_back( { i*0.4, 1.0, i*0.4});
      vertices.push_back( { i*0.4, 1.0, i*-0.4 } );

      // Triangles
      triangulos =
      { 
         {1,3,0}, {1,2,3}, //cara abajo
         {5,4,7}, {5,7,6}, //cara arriba
         {9,8,11}, {9,11,10}, //cara detrás
         {12,13,15}, {13,14,15}, //cara delante
         {17,19,16}, {17,18,19}, //cara izquierda
         {21,20,23}, {21,23,22}, //cara derecha
         {0,7,4}, {0,3,7}, //triángulos faltantes en la cara detrás
         {2,6,7}, {2,7,3}, //triángulos faltantes en la cara detrás
         {8,11,14}, {8,14,13}, //triángulos faltantes en la cara delante
         {9,10,15}, {9,15,14}, //triángulos faltantes en la cara delante
         {16,19,18}, {16,18,17}, //triángulos faltantes en la cara izquierda
         {20,21,22}, {20,22,23} //triángulos faltantes en la cara derecha
      };
   }
}

//---------EJERCICIO CONVOCATORIA EXTRAORDINARIA 2020-2021

MallaTorre::MallaTorre(int n):MallaInd("Malla Torre"){

   unsigned int j = 0;
   vertices = {};

//Vértices de la base
   vertices.push_back({+0.5,0,+0.5});// k
   vertices.push_back({-0.5,0,+0.5});// k+1
   vertices.push_back({-0.5,0,-0.5});// k+2
   vertices.push_back({+0.5,0,-0.5});// k+3


   for(unsigned i = 1; i <= n; ++i)
   {
      j += 4;
      vertices.push_back({+0.5,i,+0.5});// k
      vertices.push_back({-0.5,i,+0.5});// k+1
      vertices.push_back({-0.5,i,-0.5});// k+2
      vertices.push_back({+0.5,i,-0.5});// k+3

      triangulos.push_back({j-4, j-3, j});
      triangulos.push_back({j-3, j+1, j});

      triangulos.push_back({j-3, j-2, j+1});
      triangulos.push_back({j-2, j+2, j+1});

      triangulos.push_back({j-2, j-1, j+2});
      triangulos.push_back({j-1, j+3, j+2});

      triangulos.push_back({j-1, j-4, j+3});
      triangulos.push_back({j-4, j, j+3});
   }


}

//EXAMEN PRÁCTICAS P4 Y P5
MallaEXP4::MallaEXP4() {
  
    vertices =
      {  { 1.0, 0.0, 0.0 }, // 0
         { 1.0, 0.0, 0.0 }, // 0,1
         { 1.0, 0.0, 0.0 }, // 0,2

         { 0.0, 1.0, 0.0 }, // 1,3
         { 0.0, 1.0, 0.0 }, // 1,4
         { 0.0, 1.0, 0.0 }, // 1,5

         { 0.0, 0.0, 1.0 }, // 2,6
         { 0.0, 0.0, 1.0 }, // 2,7
         { 0.0, 0.0, 1.0 }, // 2,8
         
         { 0.0, 0.0, 0.0 }, // 3,9
         { 0.0, 0.0, 0.0 }, // 3,10
         { 0.0, 0.0, 0.0 }, // 3,11

      } ;



   triangulos =
      {  {9,3,0}, {1,4,6},
         {10,2,7}, {11,8,5}
      } ;

   cc_tt_ver = 
      {
         {0.0,1.0},//0
         {1.0,1.0},
         {1.0,1.0},
         {0.5,0.0},//1
         {0.5,0.0},
         {0.5,0.0},
         {0.0,1.0},//2
         {0.0,1.0},
         {1.0,1.0},
         {1.0,1.0},//3
         {0.5,0.0},
         {0.0,1.0},
      };
   calcularNormales();
}
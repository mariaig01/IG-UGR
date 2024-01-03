// *********************************************************************
// ** Nombre: María de las Angustias, Apellidos: Izquierdo García, Titulación: GIIADE
// ** Gestión de materiales y texturas (implementación)
// ** Copyright (C) 2014 Carlos Ureña
// **
// ** Implementación de:
// **    + clase 'Textura' (y derivadas 'TexturaXY', 'TexturaXZ')
// **    + clase 'Material' 
// **    + clase 'FuenteLuz'
// **    + clase 'ColFuentesLuz' (y la clase derivada 'Col2Fuentes')
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

#include "aplicacion-ig.h"
#include "materiales-luces.h"

using namespace std ;

const bool trazam = false ;

// **********************************************************************
/*La clase Textura encapsula una imagen en memoria, con formato RGB, donde cada texel se 
codifica con tres bytes (3 datos de tipo unsigned char), dispuestos por filas, de abajo hacia arriba.*/
Textura::Textura( const std::string & nombreArchivoJPG )
{
   // COMPLETAR: práctica 4: cargar imagen de textura
   // (las variables de instancia están inicializadas en la decl. de la clase)
   // El nombre del archivo debe convertirse a una cadena (char *) acabada en 
   // 0 tradicional en C. Para eso debe usarse el método 'c_str' de la clase 
   // 'std::string'.
   // El nombre del archivo debe ir sin el 'path', la función 'LeerArchivoJPG' lo 
   // busca en 'materiales/imgs' y si no está se busca en 'archivos-alumno'
   // .....
   const char * Archivo = nombreArchivoJPG.c_str();
   imagen = LeerArchivoJPEG(Archivo,ancho,alto);
}

// ---------------------------------------------------------------------

//----------------------------------------------------------------------
//envia la textura a la GPU
void Textura::enviar()
{
   // COMPLETAR: práctica 4: enviar la imagen de textura a la GPU
   // y configurar parámetros de la textura (glTexParameter)
   // .......
   glGenTextures( 1, &ident_textura);
   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, ident_textura );

   glTexImage2D
   ( GL_TEXTURE_2D,           // GLenum target: tipo de textura.
      0,                         // GLint level: nivel de mipmap.
      GL_RGB,                    // GLint internalformat: formato de destino en GPU.
      ancho,                     // GLsizei width: número de columnas de texels.
      alto,                      // GLsizei height: numero de filas de texels.
      0,                         // GLint border: borde, no se usa, se pone a 0.
      GL_RGB,                    // GLenum format: formato origen en memoria apl.
      GL_UNSIGNED_BYTE,          // GLenum type: tipo valores.
      imagen                     // const void * data: puntero a texels en memoria apl.
   );

    // generar mipmaps (versiones a resolución reducida)
   glGenerateMipmap( GL_TEXTURE_2D );

   
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

   
   enviada = true;
}

//----------------------------------------------------------------------

Textura::~Textura( )
{
   using namespace std ;
   cout << "destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != nullptr )
      delete [] imagen ;

   imagen = nullptr ;
   cout << "hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0
//se encarga de activar una textura en el cauce
void Textura::activar(  )
{
   using namespace std ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 4: enviar la textura a la GPU (solo la primera vez) y activarla
   // .......
   if(!enviada) enviar();

   cauce->fijarEvalText(enviada,ident_textura);
   cauce->fijarTipoGCT(modo_gen_ct,coefs_s,coefs_t);

}
// *********************************************************************
// crea un material usando un color plano y los coeficientes de las componentes

Material::Material( const float p_k_amb, const float p_k_dif,
                    const float p_k_pse, const float p_exp_pse )
{
   textura  = nullptr ;
   k_amb    = p_k_amb ;
   k_dif    = p_k_dif ;
   k_pse    = p_k_pse ;
   exp_pse  = p_exp_pse ;
}

//----------------------------------------------------------------------
// crea un material usando una textura y los coeficientes de las componentes

Material::Material( Textura * p_textura, const float p_k_amb, const float p_k_dif,
                    const float p_k_pse, const float p_exp_pse )
{
   textura  = p_textura ;  assert( textura != nullptr );
   k_amb    = p_k_amb ;
   k_dif    = p_k_dif ;
   k_pse    = p_k_pse ;
   exp_pse  = p_exp_pse ; assert( 0.5 <= exp_pse ); 
}
//----------------------------------------------------------------------

Material::~Material()
{
   if ( textura != nullptr )
   {  delete textura ;
      textura = nullptr ;
   }
}
//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}
//----------------------------------------------------------------------

std::string Material::nombre() const
{
   return nombre_mat ;
}
//----------------------------------------------------------------------
/*La clase Material encapsula una textura (opcionalmente) y además los parámetros del modelo
de iluminación local que no está asociados a las fuentes de luz (estos son: los coeficientes de 
reflexión ambiental, difusa y especular, y el exponente de brillo, en total 4 valores de tipo float.*/
void Material::activar( )
{
   using namespace std ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 4: activar un material
   // .....

   if (textura != nullptr)
      textura->activar();
   else
      cauce->fijarEvalText(false);


   cauce->fijarParamsMIL(k_amb, k_dif, k_pse, exp_pse);

}
//**********************************************************************
/*La clase FuenteLuz encapsula los parámetros de una fuente de luz de tipo direccional. Las variables de 
instancia incluyen las coordenadas esféricas (ángulos de longitud y latitud) del vector de
dirección que apunta a la fuente de luz.*/
FuenteLuz::FuenteLuz( GLfloat p_longi_ini, GLfloat p_lati_ini, const glm::vec3 & p_color )
{
   if ( trazam )
      cout << "creando fuente de luz." <<  endl << flush ;

   // inicializar parámetros de la fuente de luz
   longi_ini = p_longi_ini ;
   lati_ini  = p_lati_ini  ;
   longi     = longi_ini ;
   lati      = lati_ini ;
   color     = p_color ;
}

//----------------------------------------------------------------------
// para fuentes diraccionales, incrementar o decrementar la longitud
// (en las puntuales no hace nada)
void FuenteLuz::actualizarLongi( const float incre )
{
   longi = longi + incre ;
   using namespace std ;
   cout << "actualizado angulo de 'longitud' de una fuente de luz, nuevo == " << longi << endl ;
}
//----------------------------------------------------------------------
// para fuentes diraccionales, incrementar o decrementar la longitud
// (en las puntuales no hace nada)
void FuenteLuz::actualizarLati( const float incre )
{
   lati = lati + incre ;
   using namespace std ;
   cout << "actualizado angulo de 'latitud' de una fuente de luz, nuevo == " << lati << endl ;
}

//**********************************************************************
/*La claseColFuentesLuzencapsula un vector (una colección) de punteros a objetos de tipoFuenteLuz.*/
ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = -1 ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );

   //pf->ind_fuente = vpf.size() ;
   vpf.push_back( pf ) ;
}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz en el cauce actual
/* Este método debe construir un vector con los vectores de dirección de las luces (calculados a partir de los 
dos ángulos de cada una), y otro con los colores, y usar ambos vectores para invocar el métodofijarFuentesLuz
del cauce en uso (el cauce es un parámetro de activar). A partir de que una colección de fuentes
se activa, las fuentes que incluye se usan para la evaluación del modelo de iluminación local.*/

void ColFuentesLuz::activar( )
{
   using namespace std ;
   using namespace glm ;
   assert( apl != nullptr );
   Cauce * cauce = apl->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 4: activar una colección de fuentes de luz
   //   - crear un 'std::vector' con los colores y otro con las posiciones/direcciones,
   //   - usar el método 'fijarFuentesLuz' del cauce para activarlas
   // .....

   std::vector<glm::vec3> colores;
   std::vector<glm::vec4> pos_dir;

   for (int i=0; i < vpf.size(); i++)
   {
      FuenteLuz * fuente = vpf.at(i);
      float latitud = fuente->lati;
      float longitud = fuente->longi;
      colores.push_back(fuente->color);
      pos_dir.push_back(vec4(cos(radians(latitud)), sin(radians(latitud))*cos(radians(longitud)),sin(radians(latitud))*sin(radians(longitud)),0.0));
   }

   cauce->fijarFuentesLuz(colores, pos_dir);


}

// ---------------------------------------------------------------------
// pasa a la siguiente fuente de luz (si d==+1, o a la anterior (si d==-1))
// aborta si 'd' no es -1 o +1

void ColFuentesLuz::sigAntFuente( int d )
{
   assert( i_fuente_actual < vpf.size()) ;
   assert( d == 1 || d== -1 );
   i_fuente_actual = unsigned((int(i_fuente_actual+vpf.size())+d) % vpf.size()) ;
   cout << "fuente actual: " << (i_fuente_actual+1) << " / " << vpf.size() << endl ;
}

// ---------------------------------------------------------------------
// devuelve un puntero a la fuente de luz actual

FuenteLuz * ColFuentesLuz::fuenteLuzActual()
{
   assert( vpf[i_fuente_actual] != nullptr );
   return vpf[i_fuente_actual] ;
}
//----------------------------------------------------------------------

ColFuentesLuz::~ColFuentesLuz()
{
   for( unsigned i = 0 ; i < vpf.size() ; i++ )
   {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}
//----------------------------------------------------------------------

void PilaMateriales::push()
{
   assert( actual != nullptr );
   vector_materiales.push_back( actual );
}
//----------------------------------------------------------------------

void PilaMateriales::activar( Material * nuevo_actual )
{
   assert( nuevo_actual != nullptr );
   if ( nuevo_actual != actual )
   {  
      actual = nuevo_actual ;
      actual->activar( ) ;
   }
}
//----------------------------------------------------------------------

void PilaMateriales::pop()
{
   const unsigned n = vector_materiales.size() ;
   assert( n > 0 );
   activar( vector_materiales[n-1] ) ;
   vector_materiales.pop_back() ;
}


//--------------------------------------------------------------------------
// actualizar una colección de fuentes en función de una tecla GLFW pulsada
// (se usa el código glfw de la tecla, se llama desde 'main.cpp' con L pulsada)
// devuelve 'true' sii se ha actualizado algo

bool ProcesaTeclaFuenteLuz( ColFuentesLuz * col_fuentes, int glfw_key )
{
   assert( col_fuentes != nullptr );

   FuenteLuz * fuente     = col_fuentes->fuenteLuzActual() ; assert( fuente != nullptr );
   bool        redib      = true ;
   const float delta_grad = 10.0 ; // incremento en grados para long. y lati.

   switch( glfw_key )
   {
      case GLFW_KEY_RIGHT_BRACKET : // tecla '+' en el teclado normal
      case GLFW_KEY_KP_ADD :
         col_fuentes->sigAntFuente( +1 );
         break ;
      case GLFW_KEY_SLASH :        // tecla con '-' y '_' en el teclado normal
      case GLFW_KEY_KP_SUBTRACT :
         col_fuentes->sigAntFuente( -1 );
         break ;
      case GLFW_KEY_LEFT :
         fuente->actualizarLongi( -delta_grad );
         break ;
      case GLFW_KEY_RIGHT :
         fuente->actualizarLongi( +delta_grad );
         break ;
      case GLFW_KEY_DOWN :
         fuente->actualizarLati( -delta_grad );
         break ;
      case GLFW_KEY_UP :
         fuente->actualizarLati( +delta_grad );
         break ;
      default :
         redib = false ;
         break ;
   }
   return redib ;
}

//-----------------------------------------------------------------------
// constructor de una colección de fuentes de luz sencilla que incluye
// dos fuentes de luz.

Col2Fuentes::Col2Fuentes()
{
   using namespace glm ;
   const float f0 = 0.7, f1 = 0.3 ;
   insertar( new FuenteLuz( +45.0, 60.0,  vec3 { f0, f0,     f0,    } ) );
   insertar( new FuenteLuz( -70.0, -30.0, vec3 { f1, f1*0.5, f1*0.5 } ) );

}
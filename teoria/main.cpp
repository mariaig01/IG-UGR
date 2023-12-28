// Ejemplo mínimo de código OpenGL, usa OpenGL 3.3 + GLSL 3.3 + GLFW



// includes de la librería estándard de C++
#include <cassert>   // 'assert' (enforce preconditions)
#include <cstring>   // 'strlen' (to compile shaders)
#include <iostream>  // 'cout' and such
#include <iomanip>   // set precision and such
#include <vector>    // 'std::vector' types

// incluir cabeceras de OpenGL y GLM
#include "glincludes.h"

// incluir cabeceras auxiliares para shaders, vaos y vbos.
#include "cauce.h"      // clase 'Cauce'
#include "vaos-vbos.h"  // clases 'DescrVAO', 'DescrVBOAtribs' y 'DescrVBOInds' 
#include <math.h>
using namespace std;
using namespace glm;

// ---------------------------------------------------------------------------------------------
// Constantes y variables globales

constexpr GLuint
    ind_atrib_posiciones = 0,      // índice del atributo de vértice con su posiciones (debe ser el índice 0, siempre)
    ind_atrib_colors    = 1,      // índice del atributo de vértice con su color RGB
    num_atribs           = 2 ;     // número de atributos que gestionan los shaders
bool
    redibujar_ventana   = true ,   // puesto a true por los gestores de eventos cuando cambia el modelo y hay que regenerar la vista
    terminar_programa   = false ;  // puesto a true en los gestores de eventos cuando hay que terminar el programa
GLFWwindow *
    ventana_glfw        = nullptr; // puntero a la ventana GLFW
int
    ancho_actual        = 512 ,    // ancho actual del framebuffer, en pixels
    alto_actual         = 512 ;    // alto actual del framebuffer, en pixels
DescrVAO
    * vao_ind          = nullptr , // identificador de VAO (vertex array object) para secuencia indexada
    * vao_no_ind       = nullptr , // identificador de VAO para secuencia de vértices no indexada
    * vao_glm          = nullptr ; // identificador de VAO para secuencia de vértices guardada en vectors de vec3
Cauce 
    * cauce            = nullptr ; // puntero al objeto de la clase 'Cauce' en uso.


// ---------------------------------------------------------------------------------------------
// función que se encarga de visualizar un triángulo relleno en modo diferido,
// no indexado, usando la clase 'DescrVAO' (declarada en 'vaos-vbos.h')
// el triángulo se dibuja en primer lugar relleno con colores, y luego las aristas en negro


void DibujarTriangulo_NoInd( )
{
    assert( glGetError() == GL_NO_ERROR );

    // la primera vez, crear e inicializar el VAO
    if ( vao_no_ind == nullptr )
    {
        // número de vértices que se van a dibujar
        constexpr unsigned num_verts = 3 ;

        // tablas de posiciones y colores de vértices (posiciones en 2D, con Z=0)
        const GLfloat
            posiciones[ num_verts*2 ] = {  -0.8, -0.8,      +0.8, -0.8,     0.0, 0.8      },
            colores   [ num_verts*3 ] = {  1.0, 0.0, 0.0,   0.0, 1.0, 0.0,  0.0, 0.0, 1.0 };

        // Crear VAO con posiciones, colores e indices
        vao_no_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, GL_FLOAT, 2, num_verts, posiciones ));
        vao_no_ind->agregar( new DescrVBOAtribs( cauce->ind_atrib_colores, GL_FLOAT, 3, num_verts, colores ));//añadimos al vao no indexado los colores
    }
    
    assert( glGetError() == GL_NO_ERROR );

    // duibujar relleno usando los colores del VAO
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    cauce->fijarUsarColorPlano( false );
    vao_no_ind->habilitarAtrib( cauce->ind_atrib_colores, true );
    vao_no_ind->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );

    // dibujar las líneas usando color negro
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    cauce->fijarUsarColorPlano( true );
    cauce->fijarColor( { 0.0, 0.0, 0.0 });
    vao_no_ind->habilitarAtrib( cauce->ind_atrib_colores, false );
    vao_no_ind->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
}

// ---------------------------------------------------------------------------------------------
// función que se encarga de visualizar un triángulo  en modo diferido,
// indexado, usando la clase  'DescrVAO' (declarada en vaos-vbos.h)
// el triángulo se dibuja en primer lugar relleno con colores, y luego las aristas en negro

void DibujarTriangulo_Ind( )
{
    assert( glGetError() == GL_NO_ERROR );

    if ( vao_ind == nullptr )
    {
         // número de vértices e índices que se van a dibujar
        constexpr unsigned num_verts = 3, num_inds  = 3 ;

        // tablas de posiciones y colores de vértices (posiciones en 2D, con Z=0)
        const GLfloat
            posiciones[ num_verts*2 ] = {  -0.4, -0.4,      +0.4, -0.4,     0.0, +0.4      },
            colores   [ num_verts*3 ] = {  1.0, 0.0, 0.0,   0.0, 1.0, 0.0,  0.0, 0.0, 1.0 } ;
        const GLuint
            indices   [ num_inds    ] = { 0, 1, 2 };

        vao_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, GL_FLOAT, 2, num_verts, posiciones) );
        vao_ind->agregar( new DescrVBOAtribs( cauce->ind_atrib_colores, GL_FLOAT, 3, num_verts, colores) ) ;
        vao_ind->agregar( new DescrVBOInds( GL_UNSIGNED_INT, num_inds, indices ));
    }
   
    assert( glGetError() == GL_NO_ERROR );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    cauce->fijarUsarColorPlano( false );
    vao_ind->habilitarAtrib( cauce->ind_atrib_colores, true );
    vao_ind->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
   
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    cauce->fijarColor( { 0.0, 0.0, 0.0 });
    vao_ind->habilitarAtrib( cauce->ind_atrib_colores, false );
    vao_ind->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
}

// ---------------------------------------------------------------------------------------------
// función que se encarga de visualizar un triángulo relleno en modo diferido,
// usando vectores con entradas de tipos GLM (vec2, vec3, uvec3)
// el triángulo se dibuja en primer lugar relleno con colores, y luego las aristas en negro

void DibujarTriangulo_glm( )
{    
    using namespace std ;
    using namespace glm ;

    assert( glGetError() == GL_NO_ERROR );

    if ( vao_glm == nullptr )
    {

        // tablas de posiciones y colores de vértices (posiciones en 2D, con Z=0)
        const vector<vec2>   posiciones = {  {-0.4, -0.4},     {+0.42, -0.47},   {0.1, +0.37}    };
        const vector<vec3>   colores    = {  {1.0, 1.0, 0.0},  {0.0, 1.0, 1.0},  {1.0, 0.0, 1.0} };
        const vector<uvec3>  indices    = {  { 0, 1, 2 }};   // (un único triángulo)      

        vao_glm = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, posiciones ));
        vao_glm->agregar( new DescrVBOAtribs( cauce->ind_atrib_colores, colores )) ;
        vao_glm->agregar( new DescrVBOInds( indices ) );

        assert( glGetError() == GL_NO_ERROR );
    }
   
    assert( glGetError() == GL_NO_ERROR );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    cauce->fijarUsarColorPlano( false );
    vao_glm->habilitarAtrib( cauce->ind_atrib_colores, true );
    vao_glm->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
   
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    cauce->fijarColor( { 0.0, 0.0, 0.0 });
    vao_glm->habilitarAtrib( cauce->ind_atrib_colores, false );
    vao_glm->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
}






//-------------------EJERCICIOS TEORÍA------------------------------------------------
/*escribe una función que genera una tabla de coordenadas de
posición de vértices con las coordenadas de los vértices de un
polígono regular de n lados o aristas (es una constante del
programa), con centro en el origen y radio unidad.
Los vértices se almacenan como flotantes de doble precisión
(double), con 2 coordenadas por vértice (usa una tabla de tipo
vector<dvec2> para esto).
Adicionalente, en esa función escribe el código que crea el
correspondiente descriptor de VAO (DescrVAO) a partir de esta
tabla (el vao queda guardado como una variable global del
programa).
El valor de n (> 2) es un parámetro (un entero sin signo). El VAO sería
la base para visualizar el polígono (únicamente las aristas),
considerando la tabla de vértices como una secuencia de vértices no
indexada.
Escribe dos variantes del código, de forma que la tabla se debe
diseñar para representar el polígono usando distintos tipos de
primitivas:
(a) tipo de primitiva GL_LINE_LOOP.
(b) tipo de primitiva GL_LINES.
En este problema y el siguiente se pide únicamente generar las
tablas y el VAO, en ningún caso se pide visualizar nada.
*/
//Dibujar polígono con una secuencia de vértices no indexada GL_LINE_LOOP
void Ejercicio1a( unsigned int nlados){
    assert( glGetError() == GL_NO_ERROR );
    const unsigned int num_verts = 2*nlados; //en no indexada hay que multiplicar por el numero de coordenadas (en este caso sólo dos porque sólo dibujamos x e y)
    /*el constructor de DescrVBOAtribs tiene tres sobrecargas:
     - a partir de un puntero a los datos y algunos metadatos:
        DescrVBOAtribs( const unsigned p_index, const GLenum p_type, const unsigned p_size, const unsigned long p_count, const void *p_data );
     - a partir de un vector (GLM) de vec3
        DescrVBOAtribs( const unsigned p_index, const std::vector<glm::vec3> & src_vec );
     - a partir de un vector (GLM) de vec2
        DescrVBOAtribs( const unsigned p_index, const std::vector<glm::vec2> & src_vec );
    */
    
    vector<vec2> posiciones;

    for(int i=0;i<num_verts;i++){
        posiciones.push_back({cos(2*M_PI*i)/num_verts,sin(2*M_PI*i)/num_verts});
    }


    // la primera vez, crear e inicializar el VAO
    if ( vao_no_ind == nullptr )
    {
        // Crear VAO con posiciones, colores e indices
        vao_no_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, posiciones ));   
    }
    
    
    assert( glGetError() == GL_NO_ERROR );

    // dibujar las líneas usando color negro
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE_LOOP );

    assert( glGetError() == GL_NO_ERROR );

}
//GL_LINES
void Ejercicio1b( unsigned int nlados){
    assert( glGetError() == GL_NO_ERROR );
    const unsigned int num_verts = nlados;
    vector<vec2> posiciones;

    for(int i=0;i<num_verts;i++){
        posiciones.push_back({cos(2*M_PI*i)/num_verts,sin(2*M_PI*i)/num_verts});
    }


    // la primera vez, crear e inicializar el VAO
    if ( vao_no_ind == nullptr )
    {
        // Crear VAO con posiciones, colores e indices
        vao_no_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, posiciones ));   
    }
    
    assert( glGetError() == GL_NO_ERROR );

    // duibujar relleno usando los colores del VAO
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDrawArrays(GL_LINES,0,num_verts);


}

/*
Escribe otra función para generar una tabla de vértices y una tabla
de índices (y el correspondiente descriptor de VAO en una variable
global), que permitiría visualizar el mismo polígono regular del
problema anterior pero ahora como un conjunto de n triángulos
iguales rellenos que comparten un vértice en el centro del polígono
(el origen). Usa ahora datos de simple precisión float para los
vértices, con tres valores por vértice, siendo la Z igual a 0 en todos
ellos.
La tabla está destinada a ser visualizada con el tipo de primitiva
GL_TRIANGLES.
Escribe dos variantes del código: una variante (a) usa una secuencia
no indexada (con 3n vértices), y otra (b) usa una secuencia indexada
(sin vértices repetidos), con n + 1 vértices y 3n índices.
*/

void DibujaCuadrado(){

assert( glGetError() == GL_NO_ERROR );

    // la primera vez, crear e inicializar el VAO
    if ( vao_no_ind == nullptr )
    {
        // número de vértices que se van a dibujar
        constexpr unsigned num_verts = 4 ;

        // tablas de posiciones y colores de vértices (posiciones en 2D, con Z=0)
        const GLfloat
            posiciones[ num_verts*2 ] = {  0, 0.5,      0.5, 0,     0.5,0.5  ,0,0.5      },
            colores   [ num_verts*3 ] = {  1.0, 0.0, 0.0,   0.0, 1.0, 0.0,  0.0, 0.0, 1.0 , 0.0, 0.0, 1.0};

        // Crear VAO con posiciones, colores e indices
        vao_no_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, GL_FLOAT, 2, num_verts, posiciones ));
        vao_no_ind->agregar( new DescrVBOAtribs( cauce->ind_atrib_colores, GL_FLOAT, 3, num_verts, colores ));//añadimos al vao no indexado los colores
    }
    
    assert( glGetError() == GL_NO_ERROR );

    // duibujar relleno usando los colores del VAO
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    cauce->fijarUsarColorPlano( false );
    vao_no_ind->habilitarAtrib( cauce->ind_atrib_colores, true );
    vao_no_ind->draw( GL_LINE_STRIP);

    assert( glGetError() == GL_NO_ERROR );

    // dibujar las líneas usando color negro
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    cauce->fijarUsarColorPlano( true );
    cauce->fijarColor( { 0.0, 0.0, 0.0 });
    vao_no_ind->habilitarAtrib( cauce->ind_atrib_colores, false );
    vao_no_ind->draw( GL_LINE_LOOP );

    assert( glGetError() == GL_NO_ERROR );

}


void Ejercicio2a(unsigned int nlados){
      assert( glGetError() == GL_NO_ERROR );
     static GLenum nombre_vao = 0;
     const unsigned int num_verts = nlados; //añadimos el vértice del centro del polígono

     //tablas de coordenadas y vértices con Z=0
     vector<GLfloat> posiciones;


     //insertamos las coordenadas de todos los vertices
     for(unsigned int i=0;i<nlados;i++){
        posiciones.push_back(0);
        const GLfloat angulo = (2*i*M_PI)/nlados;
        GLfloat x = cos(angulo);
        GLfloat y = sin(angulo);
        posiciones.push_back(x);
        posiciones.push_back(y);
     }


     GLfloat * coordenadas = posiciones.data();

     if ( vao_no_ind == nullptr )
    {
        // Crear VAO con posiciones, colores e indices
        //una llamda al método CrearVao desencadena toda la inicialización de 
        //buffers y deja al VAO en un estado listo para ser usado en visualización.
        //pero como en estas practicas no tenemos un método CrearVao, tenemos que realizarlo de esta manera:
        vao_no_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, GL_FLOAT, 2, num_verts, coordenadas ));
     
    }
    
     assert( glGetError() == GL_NO_ERROR );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    //supongo que lo de los colores no se pone ya que el enunciado no pone nada de que tengamos
    //en cuenta los colores
    // cauce->fijarUsarColorPlano( false );
    // vao_ind->habilitarAtrib( cauce->ind_atrib_colores, true );
    vao_ind->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
   
}


void Ejercicio2b(unsigned int nlados){
     assert( glGetError() == GL_NO_ERROR );
     const unsigned int num_verts = nlados+1; //añadimos el vértice del centro del polígono
     const unsigned int num_inds = 3*nlados;

     //tablas de coordenadas y vértices con Z=0
     vector<GLfloat> posiciones;
     vector<GLuint> indices;

     //en primer lugar insertamos el vértice del centro del polígono
     posiciones.push_back(0);

     //insertamos el resto de posiciones
     for(unsigned int i=0;i<nlados;i++){
        const GLfloat angulo = (2*i*M_PI)/nlados;
        GLfloat x = cos(angulo);
        GLfloat y = sin(angulo);
        posiciones.push_back(x);
        posiciones.push_back(y);

     }

    
     //inicializar la tabla de indices
     //los triangulos son de la forma {0,i,i+1}
     //el ultimo triangulo es {0,nlados,1}
     for(unsigned int i=1;i<nlados;i++){
        GLuint i2=i;
        GLuint i3=i+1;
        indices.push_back(0);
        indices.push_back(i2);
        indices.push_back(i3);
     }

     //insertar ultimo triangulo
     indices.push_back(0);
     indices.push_back(nlados);
     indices.push_back(1);

     GLfloat * coordenadas = posiciones.data();
     GLuint * ind = indices.data();

     if ( vao_ind == nullptr )
    {
        // Crear VAO con posiciones, colores e indices
        vao_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, GL_FLOAT, 2, num_verts, coordenadas ));
        vao_ind->agregar( new DescrVBOInds( GL_UNSIGNED_INT, num_inds, ind ));
    }
    
     assert( glGetError() == GL_NO_ERROR );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    //supongo que lo de los colores no se pone ya que el enunciado no pone nada de que tengamos
    //en cuenta los colores
    // cauce->fijarUsarColorPlano( false );
    // vao_ind->habilitarAtrib( cauce->ind_atrib_colores, true );
    vao_ind->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
   
}

/*
Crea una copia del repositorio opengl3-minimo, y modifica el
código de ese repositorio para incluir una nueva función para
visualizar las aristas y el relleno del polígono regular de n lados
(donde n es una constante de tu programa), usando los dos
descriptores de VAO que se mencionan en:
    - el enunciado del problema 1.1 (variante (a), con
    GL_LINE_LOOP) para las aristas,
    - el enunciado del problema 1.2 (variante (b), secuencia indexada)
    para el relleno
El polígono se verá relleno de un color plano y las aristas con otro
color (también plano), distintos ambos del color de fondo. Debes
usar un VAO para las aristas y otro para el relleno.
No uses una tabla de colores de vértices para este problema, en su
lugar usa la función glVertexAttrib para cambiar el color
antes de dibujar.
Incluye todo el código en una función de visualización (nueva), esa
función debe incluir tanto la creación de tablas y VAOs (en la
primera llamada), como la visualización (en todas las llamadas)
*/
void DibujarPoligono(unsigned int n){

    static GLenum nombre_vao_aristas = 0;
    static GLenum nombre_vao_relleno = 0;

    //Dibujamos el relleno
    vector<fvec3> posiciones2;
    vector<uvec3> indices;
    const GLfloat colores_relleno   [ n*3 ] = {  1.0, 0.0, 0.0,   0.0, 1.0, 0.0,  0.0, 0.0, 1.0 } ;

    for(unsigned int i=0;i<n;i++)
        posiciones2.push_back({float(cos((2*M_PI*i)/n)),float(sin((2*M_PI*i)/n)),0});

    posiciones2.push_back({0,0,0});

    for(unsigned int i=0;i<n;i++)
        indices.push_back({i,(i+1)%n,n});

    if(vao_ind == nullptr){
        /*La clase DescrVAO incluye el método crearVAO
            - Crea un nuevo VAO en la GPU (inicializa array)
            - Invoca crearVBO para todas las tablas de atributos y la de índices (si hay).
        */
        vao_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, posiciones2 ));
        vao_ind->agregar( new DescrVBOAtribs( cauce->ind_atrib_colores, GL_FLOAT, 3, n, colores_relleno) ) ;
        vao_ind->agregar( new DescrVBOInds( indices ));
    }
    //no hace falta ya que DescrVAO ya llama a glBindVertexArray
    //else
        //glBindVertexArray( nombre_vao_relleno );

    //glVertexAttrib3f( cauce->ind_atrib_colores, 1.0, 0.0, 0.0 );
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //glDrawElements(GL_TRIANGLES,3*indices.size(),GL_UNSIGNED_INT,0);
    vao_ind->draw(GL_TRIANGLES);

    glBindVertexArray(0); //activa un VAO ya creado

    //Dibujamos las aristas
    vector<fvec2> posiciones;
    const GLfloat colores_aristas   [ n*3 ] = {  1.0, 0.0, 0.0,   0.0, 1.0, 0.0,  0.0, 0.0, 1.0 } ;

    for(unsigned int i=0;i<n;i++)
        posiciones.push_back({float(cos((2*M_PI*i)/n)),float(sin((2*M_PI*i)/n))});

    if(vao_no_ind == nullptr){
        /*La clase DescrVAO incluye el método crearVAO
            - Crea un nuevo VAO en la GPU (inicializa array)
            - Invoca crearVBO para todas las tablas de atributos y la de índices (si hay).
        */
        vao_no_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, posiciones ));
        vao_no_ind->agregar( new DescrVBOAtribs( cauce->ind_atrib_colores, GL_FLOAT, 3, posiciones.size(), colores_aristas ));
    }
    //no hace falta ya que DescrVAO ya llama a glBindVertexArray
    // else
    //     glBindVertexArray( nombre_vao_aristas );
    vao_no_ind->draw(GL_LINE_LOOP);

}

/*
Repite el problema anterior (problema 1.3), pero ahora intenta usar
el mismo VAO para las aristas y los triángulos rellenos. Para eso
puedes usar una única tabla de n + 1 posiciones de vértices, esa
tabla sirve de base para el relleno, usando índices. Para las aristas,
puedes usar GL_LINE_LOOP, pero teniendo en cuenta únicamente
los n vértices del polígono (sin usar el vértice en el origen).
*/
void DibujarPoligonoEjercicio4(unsigned int n){

    static GLenum nombre_vao = 0;

    //Dibujamos el relleno
    vector<fvec3> posiciones;
    vector<uvec3> indices;
    const GLfloat colores   [ n*3 ] = {  1.0, 0.0, 0.0,   0.0, 1.0, 0.0,  0.0, 0.0, 1.0 } ;

    for(unsigned int i=0;i<n;i++)
        posiciones.push_back({float(cos((2*M_PI*i)/n)),float(sin((2*M_PI*i)/n)),0});

    posiciones.push_back({0,0,0});

    for(unsigned int i=0;i<n;i++)
        indices.push_back({i,(i+1)%n,n});

    if(vao_ind == nullptr){
        vao_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, posiciones ));
        vao_ind->agregar( new DescrVBOAtribs( cauce->ind_atrib_colores, GL_FLOAT, 3, n, colores) ) ;
        vao_ind->agregar( new DescrVBOInds( indices ));
    }

    // Draw fill
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    vao_ind->draw(GL_TRIANGLES);

    // Draw edges
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    vao_ind->draw(GL_LINE_LOOP);

}



/*
Modifica el código del ejemplo opengl3-minimo para que no se
introduzcan deformaciones cuando la ventana se redimensiona y el
alto queda distinto del ancho. El código original del repositorio
presenta los objetos deformados (escalados con distinta escala en
vertical y horizontal) cuando la ventana no es cuadrada, ya que
visualiza en el viewport (no cuadrado) una cara (cuadrada) del cubo
de lado 2.
Para evitar este problema, en cada cuadro se deben de leer las
variables que contienen el tamaño actual de la ventana y en función
de esas variables modificar la zona visible, que ya no será siempre un
cubo de lado 2 unidades, sino que será un ortoedro que contendrá
dicho cubo de lado 2, pero tendrá unas dimensiones superiores a 2
(en X o en Y, no en ambas), adaptadas a las proporciones de la
ventana (el ancho en X dividido por el alto en Y es un valor que debe
coincidir en el ortoedro visible y en el viewport).
*/
void Ejercicio7(){
    float r=float(ancho_actual)/(alto_actual);
    float x0,x1,y0,y1;
    float z0=-1,z1=1;
    if(r>1){
        x0=-r;
        x1=r;
        y0=-1;
        y1=1;
    }
    else{
        x0=-1;
        x1=1;
        y0=-1/r;
        y1=1/r;
    }

    float sx = 2.0f / (x1 - x0);
    float sy = 2.0f / (y1 - y0);
    float sz = 2.0f / (z1 - z0);
    float cx = (x0 + x1) / 2.0f;
    float cy = (y0 + y1) / 2.0f;
    float cz = (z0 + z1) / 2.0f;

    // fijar la matriz de transformación de posiciones de los shaders

    glm::mat4 matriz{
        sx, 0.0f, 0.0f, -cx * sx,
        0.0f, sy, 0.0f, -cy * sy,
        0.0f, 0.0f, sz, -cz * sz,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    cauce->fijarMatrizProyeccion(matriz);

}


/*
En una malla indexada, queremos añadir a la estructura de datos
una tabla de aristas. Será un vector ari, que en cada entrada tendrá
una tupla de tipo uvec2 (contiene dos unsigned) con los índices
en la tabla de vértices de los dos vértices en los extremos de la
arista. El orden en el que aparecen los vértices en una arista es
indiferente, pero cada arista debe aparecer una sola vez.
Escribe el código de una función C++ para crear y calcular la tabla de
aristas a partir de la tabla de triángulos. Intenta encontrar una
solución con la mínima complejidad en tiempo y memoria posible.
Suponer que el número de vértices adyacentes a uno cualquiera de
ellos es como mucho un valor constante k > 0, valor que no
depende del número total de vértices, que llamamos n.

Considerar dos casos:
a)1. Los triángulos se dan con orientación no coherente: esto quiere
decir que si un triángulo está formado por los vértices i, j, k,
estos tres índices pueden aparecer en cualquier orden en la
correspondiente entrada de la tabla de triángulos (puede
aparece como i, j, k o como i, k, j, o como k, j, i, etc....)
b)Los triángulos se dan con orientación coherente: esto quiere
decir que si dos triángulos comparten una arista entre los
vértices i y j, entonces en uno de los triángulos la arista aparece
como (i, j) y en el otro aparece como (j, i) (decimos que en el
triángulo a, b, c aparecen las tres aristas (a, b), (b, c) y (c, a)).
Además, asumimos que la malla es cerrada, es decir, que cada
arista es compartida por exactamente dos triángulos.
*/

void calculararistasnocoherencia(vector<vec3> triangulos, int nvertices,vector<vec3> vertices){
    // vector<vec2> ari;
    
    // for(int i=0;i<triangulos.size();i++){
    //     vec2 arista1={vertices.at(triangulos.at(i)[0]),vertices.at(triangulos.at(i)[1])};
    //     vec2 arista2={vertices.at(triangulos.at(i)[1]),vertices.at(triangulos.at(i)[2])};
    //     vec2 arista3={vertices.at(triangulos.at(i)[0]),vertices.at(triangulos.at(i)[2])};
    //     if(!ari.find(arista1)){
    //     ari.push_back(arista1);
    //     }
    //     if(!ari.find(arista2)){
    //     ari.push_back(arista2);
    //     }
    //     if(!ari.find(arista3)){
    //     ari.push_back(arista3);
    //     }
    // ari.
    // }

}

void calculararistascoherencia(vector<vec3> triangulos, int nvertices){

    int a,b;
    vector<vec2> aristas;
    for(int i=0;i<triangulos.size();i++){
        for(int j=0;j<3;j++){
            a = triangulos[i][j];
            b = triangulos[i][(j+1)%3];

            if(a<b)
                aristas.push_back({a,b});

        }

    }

}

void gancho(){

    vector<vec3> posiciones;
    posiciones.push_back({0.0,0.0,0.0});
    posiciones.push_back({1.0,0.0,0.0});
    posiciones.push_back({1.0,1.0,0.0});
    posiciones.push_back({0.0,1.0,0.0});
    posiciones.push_back({0.0,2.0,0.0});

     // la primera vez, crear e inicializar el VAO
    if ( vao_no_ind == nullptr )
    {
        // Crear VAO con posiciones, colores e indices
        vao_no_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, GL_FLOAT, 2, posiciones.size(), posiciones.data() ));
    }
    
    assert( glGetError() == GL_NO_ERROR );
    glVertexAttrib3f(cauce->ind_atrib_colores,0.1,0.1,0.1);
    // duibujar relleno usando los colores del VAO
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    vao_no_ind->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );

}

void gancho_x4(){
    /*
    La función resetMM debe llamarse al inicio del frame, de forma que
    siempre comenzamos la visualización de los objetos con la
    modelview fijada a un valor conocido.
    */
    cauce->resetMM();
    gancho();

    //COMPROBAR QUE ESTAN BIEN LOS TRANSLATE Y LOS ROTATE
    cauce->compMM(translate(vec3{0.0,2.0,0.0}));
    cauce->compMM(rotate(90.0f,vec3{0.0,0.0,1.0}));
    gancho();

    cauce->resetMM();
    cauce->compMM(translate(vec3{-2.0,2.0,0.0}));
    cauce->compMM(rotate(180.0f,vec3{0.0,0.0,1.0}));
    gancho();

    cauce->resetMM();
    cauce->compMM(translate(vec3{-2.0,0.0,0.0}));
    cauce->compMM(rotate(270.0f,vec3{0.0,0.0,1.0}));
    gancho();


}

/*
Escribe el pseudocódigo OpenGL otra función (gancho_2p) para
dibujar esa misma figura, pero escalada y rotada de forma que sus
extremos coincidan con dos puntos arbitrarios disintos p˙0 y p˙1
,
puntos cuyas coordenadas de mundo son p0 = (x0, y0, 1)
t
y
p1 = (x1
, y1
, 1)
t
. Estas coordenadas se pasan como parámetros a
dicha función (como vec3)
Escribe una solución (a) acumulando matrices de rotación, traslación
y escalado en la matriz modelview de OpenGL. Escribe otra solución
(b) en la cual la matriz modelview se calcula directamente sin
necesidad de usar funciones trigonometricas (como lo son el
arcotangente, el seno, coseno, arcoseno o arcocoseno).
*/
void gancho_2pa(vec3 p0,vec3 p1){
    vec3 v = p1 - p0;
    vec3 y = {0.0,1.0,0.0};
    float alpha = acos(float(v.dot(y)/(sqrt(y.length())*sqrt(v.length()))));
    if(v[0]>0)
        alpha = -alpha;

    cauce->resetMM();
    cauce->compMM(translate(p0));
    cauce->compMM(rotate(float((180.0f*alpha)/M_PI),vec3{0.0,0.0,1.0}));
    cauce->compMM(scale(vec3(1.0f,sqrt(v.length()),1.0)));
    cauce->compMM(scale(vec3(1.0f,0.5,1.0)));
    gancho();

    //Otra forma
    float phi=atan2(p1[0]-p0[0],p1[1]-p0[1]); //Angulo phi
    float mod=sqrt((pow(p0[0]-p1[0]),2)+(pow(p0[1]-p1[1]),2));
    float esc=mod/2.0;
    cauce->resetMM();
    cauce->compMM(translate(vec3{p0[0],p0[1],0}));
    cauce->compMM(rotate(float(-phi*180/M_PI), vec3{0,0,1}));
    cauce->compMM(scale(vec3{esc,esc,esc}));
    gancho();

}

void gancho_2pb(vec3 p0,vec3 p1){
    vec3 y_prima ={(p1[0]-p0[0])/2,(p1[1]-p0[1])/2,(p1[2]-p0[2])/2};
    vec3 x_prima = {y_prima[1],-y_prima[0],y_prima[2]};
    x_prima = x_prima.normalize();
    mat4 matriz_transformacion = mat4(1.0f);

    matriz_transformacion[0][0] = x_prima[0];
    matriz_transformacion[0][1] = y_prima[0];
    matriz_transformacion[1][0] = x_prima[1];
    matriz_transformacion[1][1] = y_prima[1];

    matriz_transformacion = translate(p0)*matriz_transformacion;

    cauce->resetMM();
    cauce->compMM(matriz_transformacion);
    gancho();
}
/*
Usa la función del problema anterior para construir estas dos nuevas
figuras, en las cuales hay un número variable de instancias de la
figura original, dispuestas en circulo (vemos los ejemplos para 5 y 8
instancias, respectivamente)
*/
void ganchos_unidos(const int unsigned n){
    float angulo_base = 2.0 * M_PI / ((float)n);
    vec3 posicion_1,posicion_2;

    for(unsigned int i=0;i<n;i++){
        float angulo = angulo_base*i - angulo_base / 2.0;
        float angulo_siguiente = angulo_base * (i+1) - angulo_base/2.0;
        posicion_1 = {(float)cos(angulo),(float)sin(angulo),0.0};
        posicion_2 = {(float)cos(angulo_siguiente),(float)sin(angulo_siguiente),0.0};
    }
    gancho_2pa(posicion_1,posicion_2);

}

/*
Escribe una función llamada FiguraSimple que dibuje con
OpenGL en modo diferido la figura que aparece aquí (un cuadrado
de lado unidad, relleno de color, con la esquina inferior izquierda en
el origen, con un triángulo inscrito, relleno del color de fondo).
*/
//CON 2 VAO (LO HACEMOS EN 2D)
void FiguraSimple2VAO(){

    vector<vec2> vertices_triangulo,vertices_cuadrado;
    vector<vec3> indices_triangulo,indices_cuadrado;

    //vertices triangulo
    vertices_triangulo.push_back({0.2,0.2}); //0
    vertices_triangulo.push_back({0.4,0.2}); //1
    vertices_triangulo.push_back({0.3,0.65}); //2

    //vertices cuadrado
    vertices_cuadrado.push_back({0,0}); //0
    vertices_cuadrado.push_back({1,0}); //1
    vertices_cuadrado.push_back({1,1}); //2
    vertices_cuadrado.push_back({0,1}); //3

    //creamos los triangulos
    vector<vec3> indices_cuadrado,indices_triangulo;
    //triangulos del cuadrado
    indices_cuadrado.push_back({0,1,2});
    indices_cuadrado.push_back({2,3,0});
    //triangulo del triangulo
    indices_triangulo.push_back({0,1,2});

    //vao cuadrado
    //aqui hay que crear un nombre_vao_cuadrado y ver si es igual a 0 y entonces hacer lo de dentro del if???
    if ( vao_ind == nullptr )
    {
        const GLfloat color_relleno_cuadrado   [ 3 ] = {  0.3, 0.7, 0.9} ;
        vao_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, GL_FLOAT, 2, vertices_cuadrado.size(), vertices_cuadrado.data()) );
        vao_ind->agregar( new DescrVBOAtribs( cauce->ind_atrib_colores, GL_FLOAT, 3, 3, color_relleno_cuadrado) ) ;
        vao_ind->agregar( new DescrVBOInds( GL_UNSIGNED_INT, indices_cuadrado.size(), indices_cuadrado.data() ));
    }

    assert( glGetError() == GL_NO_ERROR );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    cauce->fijarUsarColorPlano( false );
    vao_ind->habilitarAtrib( cauce->ind_atrib_colores, true );
    vao_ind->draw( GL_TRIANGLES ); //la funcion draw ya llama a glDrawElements (en el caso de que sea una malla indexada) y a glDrawArrays 

    

    //vao triangulo
    if ( vao_ind == nullptr )
    {
        const GLfloat color_relleno_triangulo   [ 3 ] = {  1.0, 1.0, 1.0} ;
        vao_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, GL_FLOAT, 2, vertices_triangulo.size(), vertices_triangulo.data()) );
        vao_ind->agregar( new DescrVBOAtribs( cauce->ind_atrib_colores, GL_FLOAT, 3, 3, color_relleno_triangulo) ) ;
        vao_ind->agregar( new DescrVBOInds( GL_UNSIGNED_INT, indices_triangulo.size(), indices_triangulo.data() ));
    }

    assert( glGetError() == GL_NO_ERROR );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    cauce->fijarUsarColorPlano( false );
    vao_ind->habilitarAtrib( cauce->ind_atrib_colores, true );
    vao_ind->draw( GL_TRIANGLES ); //tambien llama a glBindVertexArray( 0 );

}

//CON 1 VAO (LO HACEMOS EN 2D)
void FiguraSimple1VAO(){

    vector<vec2> posiciones;

    //vertices cuadrado
    posiciones.push_back({0,0}); //0
    posiciones.push_back({1,0}); //1
    posiciones.push_back({1,1}); //2
    posiciones.push_back({0,1}); //3

    //vertices triangulo
    posiciones.push_back({0.2,0.2}); //4
    posiciones.push_back({0.4,0.2}); //5
    posiciones.push_back({0.3,0.65}); //6

    //creamos los triangulos
    vector<vec3> indices_cuadrado,indices_triangulo;
    //triangulos del cuadrado
    indices_cuadrado.push_back({0,1,2});
    indices_cuadrado.push_back({2,3,0});
    //triangulo del triangulo
    indices_triangulo.push_back({4,5,6});

    if ( vao_ind == nullptr )
    {
        vao_ind = new DescrVAO( cauce->num_atribs, new DescrVBOAtribs( cauce->ind_atrib_posiciones, GL_FLOAT, 2, posiciones.size(), posiciones.data()) );
        vao_ind->agregar( new DescrVBOInds( GL_UNSIGNED_INT, indices_cuadrado.size(), indices_cuadrado.data() ));
        vao_ind->agregar( new DescrVBOInds( GL_UNSIGNED_INT, indices_triangulo.size(), indices_triangulo.data() ));
    }

    //no se como se haria con los colores, AVERIGUAR
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    cauce->fijarUsarColorPlano( false );
    vao_ind->habilitarAtrib( cauce->ind_atrib_colores, true );
    vao_ind->draw( GL_TRIANGLES ); //la funcion draw ya llama a glDrawElements (en el caso de que sea una malla indexada) y a glDrawArrays 


}

/*
Usando exclusivamente llamadas a la función del problema 21,
construye otra función llamada FiguraCompleja que dibuja la
figura de aquí. Para lograrlo puedes usar manipulación de la pila de
la matriz modelview (pushMM y popMM), junto con
MAT_Traslacion y MAT_Escalado:
*/
void FiguraCompleja(){
    cauce->resetMM();

    FiguraSimple1VAO();

    cauce->pushMM();
        cauce->compMM(translate(vec3{2.0,1.0,0.0}));
        cauce->compMM(rotate(float(radians(-45.0)),vec3{2.0,1.0,0.0}));
        cauce->compMM(scale(vec3{sqrt(2.0),-sqrt(2.0),sqrt(2)}));
        cauce->compMM(translate(vec3{-0.5,-0.5,0.0}));
        FiguraSimple1VAO();
    cauce->popMM();

    cauce->pushMM();
        cauce->compMM(translate(vec3{3.0,0.5,0.0}));
        cauce->compMM(scale(vec3{2.0,-1.0,1.0}));
        cauce->compMM(translate(vec3{0.0,-0.5,0.0}));
        FiguraSimple1VAO();
    cauce->popMM();

}

/*
Escribe el código OpenGL de una función (llamada Tronco) que
dibuje la figura que aparece a aquí. El código dibujará el polígono
relleno de color azul claro, y las aristas que aparecen de color azul
oscuro (ten en cuenta que no todas las aristas del polígono relleno
aparecen).
*/ 
//LO HACEMOS EN 2D
void Tronco2VAO(){
    vector<vec2> posiciones;
    posiciones.push_back({0,0});
    posiciones.push_back({1.0,0});
    posiciones.push_back({1.0,1.0});
    posiciones.push_back({2.0,2.0});
    posiciones.push_back({1.5,2.5});
    posiciones.push_back({0.5,1.5});
    posiciones.push_back({0.0,3.0});
    posiciones.push_back({-0.5,3.0});
    posiciones.push_back({0.0,1.5});

    //tenemos que crear dos tablas de indices, una para los indices de vertices y otra para los indices de aristas
    vector<vec3> 


}

void Tronco1VAO(){


}



// ---------------------------------------------------------------------------------------------
// función que se encarga de visualizar el contenido en la ventana

void VisualizarFrame( )
{
    using namespace std ;
    using namespace glm ;

    // comprobar y limpiar variable interna de error
    assert( glGetError() == GL_NO_ERROR );

    // usar (acrivar) el objeto programa (no es necesario hacerlo en 
    // cada frame si solo hay uno de estos objetos, pero se incluye 
    // para hacer explícito que el objeto programa debe estar activado)
    cauce->activar();

    // establece la zona visible (toda la ventana)
    glViewport( 0, 0, ancho_actual, alto_actual );

    // fija la matriz de transformación de posiciones de los shaders 
    // (la hace igual a la matriz identidad)
    cauce->resetMM();

    // fija la matriz de proyeccion (la hace igual a la matriz identidad)
    cauce->fijarMatrizProyeccion( glm::mat4(1.0) );

    // limpiar la ventana
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // habilitar EPO por Z-buffer (test de profundidad)
    glDisable( GL_DEPTH_TEST );

    // Dibujar un triángulo, es una secuncia de vértice no indexada.
    DibujarTriangulo_NoInd();
    
    //Ejercicio2a(3);

    // usa el color plano para el segundo triángulo
    cauce->fijarUsarColorPlano( true );

    

    // dibujar triángulo indexado (rotado y luego desplazado) 
    cauce->pushMM();
        cauce->compMM( translate( vec3{ 0.4f, 0.1f, -0.1f}  ));
        cauce->compMM( rotate(  radians(23.0f), vec3{ 0.0f, 0.0f, 1.0f}   ));
        DibujarTriangulo_Ind();     // indexado
    cauce->popMM();

    // dibujar un triángulo usando vectores de GLM
    DibujarTriangulo_glm() ;

    

    // comprobar y limpiar variable interna de error
    assert( glGetError() == GL_NO_ERROR );

    // esperar a que termine 'glDrawArrays' y entonces presentar el framebuffer actualizado
    glfwSwapBuffers( ventana_glfw );

}


// ---------------------------------------------------------------------------------------------
// función que se invoca cada vez que cambia el número de pixels del framebuffer
// (cada vez que se redimensiona la ventana)

void FGE_CambioTamano( GLFWwindow* ventana, int nuevo_ancho, int nuevo_alto )
{
    using namespace std ;
    //cout << "FGE cambio tamaño, nuevas dimensiones: " << nuevo_ancho << " x " << nuevo_alto << "." << endl ;
    ancho_actual      = nuevo_ancho ;
    alto_actual       = nuevo_alto ;
    redibujar_ventana = true ; // fuerza a redibujar la ventana
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que se pulse o levante una tecla.

void FGE_PulsarLevantarTecla( GLFWwindow* ventana, int key, int scancode, int action, int mods )
{
    using namespace std ;
    //cout << "FGE pulsar levantar tecla, número de tecla == " << key << "." << endl ;
    // si se pulsa la tecla 'ESC', acabar el programa
    if ( key == GLFW_KEY_ESCAPE )
        terminar_programa = true ;
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que se pulse o levante un botón del ratón

void FGE_PulsarLevantarBotonRaton( GLFWwindow* ventana, int button, int action, int mods )
{
    // nada, por ahora

}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que cambie la posición del puntero

void FGE_MovimientoRaton( GLFWwindow* ventana, double xpos, double ypos )
{
    // nada, por ahora
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que mueva la rueda del ratón.

void FGE_Scroll( GLFWwindow* ventana, double xoffset, double yoffset )
{
    // nada, por ahora
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cuando se produzca un error de GLFW

void ErrorGLFW ( int error, const char * description )
{
    using namespace std ;
    cerr
        << "Error en GLFW. Programa terminado" << endl
        << "Código de error : " << error << endl
        << "Descripción     : " << description << endl ;
    exit(1);
}
// ---------------------------------------------------------------------------------------------
// código de inicialización de GLFW

void InicializaGLFW( int argc, char * argv[] )
{
    using namespace std ;

    // intentar inicializar, terminar si no se puede
    glfwSetErrorCallback( ErrorGLFW );
    if ( ! glfwInit() )
    {
        cout << "Imposible inicializar GLFW. Termino." << endl ;
        exit(1) ;
    }

    // especificar versión de OpenGL y parámetros de compatibilidad que se querrán
   // (pedimos opengl 330, tipo "core" (sin compatibilidad con versiones anteriores)
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 ); 
   glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE ); // indica que tambien debe funcionar si se usa con un driver con version superior a la 3.3
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // indica que no es compatible hacia atrás con versiones previas a la 3.3

    // especificar que función se llamará ante un error de GLFW
    glfwSetErrorCallback( ErrorGLFW );

    // crear la ventana (var. global ventana_glfw), activar el rendering context
    ventana_glfw = glfwCreateWindow( 512, 512, "IG ejemplo mínimo (OpenGL 3+)", nullptr, nullptr );
    glfwMakeContextCurrent( ventana_glfw ); // necesario para OpenGL

    // leer y guardar las dimensiones del framebuffer en pixels
    glfwGetFramebufferSize( ventana_glfw, &ancho_actual, &alto_actual );

    // definir cuales son las funciones gestoras de eventos (con nombres 'FGE_....')
    glfwSetFramebufferSizeCallback( ventana_glfw, FGE_CambioTamano );
    glfwSetKeyCallback            ( ventana_glfw, FGE_PulsarLevantarTecla );
    glfwSetMouseButtonCallback    ( ventana_glfw, FGE_PulsarLevantarBotonRaton);
    glfwSetCursorPosCallback      ( ventana_glfw, FGE_MovimientoRaton );
    glfwSetScrollCallback         ( ventana_glfw, FGE_Scroll );
}

// ---------------------------------------------------------------------------------------------
// función para inicializar GLEW (necesario para las funciones de OpenGL ver 2.0 y posteriores)
// en macOS no es necesario (está vacía)

void InicializaGLEW()
{
#ifndef __APPLE__
    using namespace std ;
    GLenum codigoError = glewInit();
    if ( codigoError != GLEW_OK ) // comprobar posibles errores
    {
        cout << "Imposible inicializar ’GLEW’, mensaje recibido: " << endl
             << (const char *)glewGetErrorString( codigoError ) << endl ;
        exit(1);
    }
    else
        cout << "Librería GLEW inicializada correctamente." << endl << flush ;

#endif
}

// ---------------------------------------------------------------------------------------------

void InicializaOpenGL()
{
    using namespace std ;
    
    assert( glGetError() == GL_NO_ERROR );

    cout  << "Datos de versión e implementación de OpenGL" << endl
         << "    Implementación de : " << glGetString(GL_VENDOR)  << endl
         << "    Hardware          : " << glGetString(GL_RENDERER) << endl
         << "    Versión de OpenGL : " << glGetString(GL_VERSION) << endl
         << "    Versión de GLSL   : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl ;

    
    InicializaGLEW(); // En linux y windows, fija punteros a funciones de OpenGL version 2.0 o superiores
    
    
    assert( glGetError() == GL_NO_ERROR );
    
    glClearColor( 1.0, 1.0, 1.0, 0.0 ); // color para 'glClear' (blanco, 100% opaco)
    glDisable( GL_CULL_FACE );          // dibujar todos los triángulos independientemente de su orientación
    cauce = new Cauce() ;            // crear el objeto programa (variable global 'cauce')
    
    assert( cauce != nullptr );
    assert( glGetError() == GL_NO_ERROR );
}
// ---------------------------------------------------------------------------------------------

void BucleEventosGLFW()
{
    while ( ! terminar_programa )
    {   
        if ( redibujar_ventana )
        {   
            VisualizarFrame();
            redibujar_ventana = false; // (evita que se redibuje continuamente)
        }
        glfwWaitEvents(); // esperar evento y llamar FGE (si hay alguna)
        terminar_programa = terminar_programa || glfwWindowShouldClose( ventana_glfw );
    }
}
// ---------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------



int main( int argc, char *argv[] )
{
    using namespace std ;
    cout << "Programa mínimo de OpenGL 3.3 o superior" << endl ;

    InicializaGLFW( argc, argv ); // Crea una ventana, fija funciones gestoras de eventos
    InicializaOpenGL() ;          // Compila vertex y fragment shaders. Enlaza y activa programa. Inicializa GLEW.
    BucleEventosGLFW() ;          // Esperar eventos y procesarlos hasta que 'terminar_programa == true'
    glfwTerminate();              // Terminar GLFW (cierra la ventana)

    cout << "Programa terminado normalmente." << endl ;
}

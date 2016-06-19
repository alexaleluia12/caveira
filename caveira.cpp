#include <cstdio>
#include <GL/glut.h>
#include <cmath>
#include <cassert>


#include "comum.h"

using namespace std;// nao preciso usar std::xxxx

// flags para compilar
// -lGL -lGLU -lglut -lm

// TODO
// tecla a > abre boca
// tecla f > fecha boca


void interacao_com_teclado(unsigned char tecla, int x, int y);
void interacao_com_mouse(void);
void myInit(void);
void define_camera();
void rotacionar_camera_y(double value);
void rotacionar_camara_x(double value);
void st_rotacionar(vector<float *> & lst_vertices, float * referencia,\
                   double value, int * eixos);


// =====================================================================
#define largura  640
#define altura 620
Obj3d * bc;// baixo caveira
Obj3d * cc;// cima caveira
int passo = 15;
double angulo = 5*M_PI/180.0; // 10 graus
int angulo_camera = 50;// almentar esse valor aproxima a camera
int centro_y = altura/2.0;
int centro_x = largura/2.0;
float centro_massa[3];
int x = 0, y = 0, z = 100;// valores iniciais parecidos com o padrao
                          // do opengl
int rotaca_x[] = {1, 2};
int rotacao_y[] = {2, 0};
int rotacao_z[] = {0, 1};
float curinga[] = {1.0, 2.47292, -6.23335};

const char * local_baixo = "arquivos/baixo.obj";
const char * local_cima = "arquivos/cima.obj";

// ** fn rotacao
void st_rotacionar(vector<float *> & lst_vertices, float * referencia,
                   double value, int * eixos)
{
    float x, y;
    float * point = NULL;
    int i, max = lst_vertices.size(); //size(lst_dinamica);

    
    get_centro_massa(lst_vertices, referencia);
    desloca_centro(lst_vertices, referencia);
    
    // rotaciona
    for(i=0; i<max; i++){
        point = lst_vertices[i];
        assert(point != NULL);
        
        x = point[eixos[0]] * cos(value) - point[eixos[1]] * sin(value);
        y = point[eixos[0]] * sin(value) + point[eixos[1]] * cos(value);
        
        point[eixos[0]] = x;
        point[eixos[1]] = y;
        
        point = NULL;
    }
    
    desloca_to(lst_vertices, referencia);
    interacao_com_mouse();
    //redesenhar tudo e chamar o clear antes
    // o centro de massa deve ser o curringa
}
// **

void rotacionar_camara_x(double value)
{
    float zl, yl;
    // rotaciona
    yl = y * cos(value) - z * sin(value);
    zl = y * sin(value) + z * cos(value);

    y = round(yl);
    z = round(zl);
}


void rotacionar_camera_y(double value)
{
    float xl, zl;
    // rotaciona
    xl = z * sin(value) + x * cos(value);
    zl = z * cos(value) - x * sin(value);

    x = round(xl);
    z = round(zl);
}

void myInit(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(2.0);
    //glMatrixMode(GL_PROJECTION);// GL_PROJECTION ja eh paralelo por padrao
    //glLoadIdentity();
    //              left     right          botton      top
    glOrtho(-largura/2.0, largura/2.0, -altura/2.0, altura/2.0, -350.0, 350.0);
    //gluOrtho2D(0.0, largura/2.0, 0.0, altura/2.0);// at center

}

void define_camera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(angulo_camera, (largura/altura), 0.5, 500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, up.x, up.y, up.z);
    gluLookAt(x, y, z, 0,0,0, 0,1,0);
    // default gluLookAt(0, 0, 0, 0, 0,-1, 0, 1, 0);
}



void interacao_com_mouse(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    st_desesenhar_obj(bc);
    st_desesenhar_obj(cc);
    // ==
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3d(curinga[0], curinga[1], curinga[2]);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    // ==
    glFlush();
}


// deixar essa parte da rotacao comentada por enquanto *nao esta em comum.h

void interacao_com_teclado(unsigned char tecla, int x, int y)
{
/*    if(tecla == 'z'){*/
/*        rotacionar_em_z(angulo);*/
/*    } else if(tecla == 'x'){*/
/*        rotacionar_em_x(angulo);*/
/*    } else if(tecla == 'X'){*/
/*        rotacionar_em_x(-angulo);*/
/*    } else if(tecla == 'y'){*/
/*        rotacionar_em_y(angulo);*/
/*    } else if(tecla == 'e'){*/
/*        escalar_pontos(1.1);*/
/*    } else if(tecla == 'E'){*/
/*        escalar_pontos(.9);*/
/*    } else if(tecla == 't'){*/
/*        transladar_pontos(passo);*/
/*    } else if(tecla == 'T'){*/
/*        transladar_pontos(-passo);*/
/*    } else if(tecla == 'u'){// u para transladar em y*/
/*        transladar_pontos_y(passo);*/
/*    } else if(tecla == 'U'){*/
/*        transladar_pontos_y(-passo);*/
/*    } else */
    if(tecla == 'q'){ // afasta camera
        z += passo;
        define_camera();
        glutPostRedisplay();
    } else if(tecla == 'Q'){ // aproxima camera
        z -= passo;
        define_camera();
        glutPostRedisplay();
    } else if(tecla == 'p'){ // roda parte de cima
        // eu fico sem ver uma parte do objeto pq a camera esta rodando em
        // relacao a ela mesma
        rotacionar_camara_x(angulo);
        define_camera();
        glutPostRedisplay();
    } else if(tecla == 'P'){
        rotacionar_camara_x(-angulo);
        define_camera();
        glutPostRedisplay();
    } else if(tecla == 'r'){ // roda na lateral
        rotacionar_camera_y(angulo);
        define_camera();
        glutPostRedisplay();
    } else if(tecla == 'R'){
        rotacionar_camera_y(-angulo);
        define_camera();
        glutPostRedisplay();
    } else if(tecla == 'h' || tecla == 'H'){
        st_rotacionar(*bc->lst_vetices, bc->referencia, angulo, rotaca_x);
    }

}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(largura, altura);
    //                 largura, altura
    glutInitWindowPosition(100, 150);
    glutCreateWindow("POLIEDROS");
    myInit();

    bc = st_create_obj();
    cc = st_create_obj();

    pegar_obj_de_arquivo(*bc->lst_vetices, *bc->lst_faces, local_baixo);
    pegar_obj_de_arquivo(*cc->lst_vetices, *cc->lst_faces, local_cima);
    bc->referencia[0] = curinga[0];
    bc->referencia[1] = curinga[1];
    bc->referencia[2] = curinga[2];

    rotacionar_camera_y(15*angulo);

    // vou transladar cima -50 em y
    // * lembrando que tenho q> centro -> transforma -> original
    transladar_pontos_n(*cc->lst_vetices, -50.0, 1); // y

    glutDisplayFunc(interacao_com_mouse);
    //glutMouseFunc(my_mouse);

    glutKeyboardFunc(interacao_com_teclado);
    define_camera();

    glutMainLoop();

    st_destroy_obj(bc);
    st_destroy_obj(cc);

    return 0;
}
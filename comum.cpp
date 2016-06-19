#include <cstdio>
#include <GL/glut.h>
#include <cmath>
#include <cassert>
#include <cstring>
#include <cerrno>


#include "comum.h"

using namespace std;


// flags para compilar (linux)
// -lGL -lGLU -lglut -lm

// TODO
// implementar toda as funcoes necessario para o Obj3d
// uma delas eh o desenhar que recebe o Struct como primeiro parametro
//  ** eh NAO chama glClear(GL_COLOR_BUFFER_BIT);
//
// usar c++ nao necessaria mente eu tenho que usar clases



// =====================================================================


void die(const char *message)
{
    if(errno){
        perror(message);
    }
    else {
        printf("ERRO: %s\n", message);
        exit(1);
    }
}

void ponto2i(int x, int y)
{
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
}

void ponto3i(int x, int y, int z)
{
    glBegin(GL_POINTS);
        glVertex3i(x, y, z);
    glEnd();
    glFlush();
}


void desenhar_poligono_pontos(float * p1, float * p2, float * p3)
{

    glBegin(GL_POLYGON);
        glVertex3d(p1[0], p1[1], p1[2]);
        glVertex3d(p2[0], p2[1], p2[2]);
        glVertex3d(p3[0], p3[1], p3[2]);
    glEnd();
}

void desenhar_arestas_pontos(float * p1, float * p2, float * p3)
{

    glBegin(GL_LINE_LOOP);
        glVertex3d(p1[0], p1[1], p1[2]);
        glVertex3d(p2[0], p2[1], p2[2]);
        glVertex3d(p3[0], p3[1], p3[2]);
    glEnd();
}

inline float get_distancia(float *p1, float *p2)
{ return sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2)); }

void get_centro_massa(vector<float *> & lst, float * destiny)
{
    int i, max = lst.size();
    float x, y, z;
    float menor_x, maior_x, menor_y, maior_y, menor_z, maior_z;
    float * point_corrent = NULL;
    for(i=0; i<max; i++){
        point_corrent = lst[i];
        x = point_corrent[0];
        y = point_corrent[1];
        z = point_corrent[2];

        if(i==0 || x < menor_x){
            menor_x = x;
        }
        if(i==0 || x > maior_x){
            maior_x = x;
        }
        if(i==0 || y < menor_y){
            menor_y = y;
        }
        if(i==0 || y > maior_y){
            maior_y = y;
        }
        if(i == 0 || z > maior_z){
            maior_z = z;
        }
        if(i == 0 || z < menor_z){
            menor_z = z;
        }
    }

    // centro de massa
    destiny[0] = (menor_x + maior_x)/2;
    destiny[1] = (menor_y + maior_y)/2;
    destiny[2] = (menor_z + maior_z)/2;
}

// retorna o posicao na list do ponto mais proximo de point
int  get_mais_proximo_de(vector<float *> & lst, float *point)
{
    float valor_menor = 0.0;
    float * corrente = NULL;
    int i, max = lst.size();
    int local_menor = -1;

    for(i=0; i<max; i++){
        corrente = lst[i];
        if(i == 0 || get_distancia(corrente, point) < valor_menor){
            valor_menor = get_distancia(corrente, point);
            local_menor = i;
        }
    }

    assert(local_menor != -1);
    return local_menor;
}


void transladar_pontos_n(vector<float *> & lst, float value, int eixo)
{
    float * point = NULL;
    int i, max = lst.size();

    for(i=0; i<max; i++){
        point = lst[i];
        assert(point != NULL);

        point[eixo] += value;
        point = NULL;
    }
}


// levar para cm para origem , escalar e voltar para ponto original
void escalar_pontos(vector<float *> & lst, float value)
{
    float * point = NULL;
    int i, max = lst.size();

    // escala
    for(i=0; i<max; i++){
        point = lst[i];
        assert(point != NULL);


        point[0] = point[0] * value;
        point[1] = point[1] * value;
        point[2] = point[2] * value;

        point = NULL;
    }

}

// x - cm_x -> vai em direcao ao centro
void desloca_centro(vector<float *> & lst, float * cm_point)
{
    int i, max = lst.size();
    float x = cm_point[0], y = cm_point[1], z = cm_point[2];
    float * tmp_point;
    for(i=0; i<max; i++){
        tmp_point = lst[i];
        tmp_point[0] = tmp_point[0] - x;
        tmp_point[1] = tmp_point[1] - y;
        tmp_point[2] = tmp_point[2] - z;
    }
}

void desloca_to(vector<float *>  & lst, float * cm_point)
{
    int i, max = lst.size();
    float x = cm_point[0], y = cm_point[1], z = cm_point[2];
    float * tmp_point;
    for(i=0; i<max; i++){
        tmp_point = lst[i];
        tmp_point[0] = tmp_point[0] + x;
        tmp_point[1] = tmp_point[1] + y;
        tmp_point[2] = tmp_point[2] + z;
    }
}


int contain(char * source, char target)
{
    int i = 0;
    while(source[i] != '\0'){
        if(source[i] == target)
            return 1;
        i++;
    }
    return 0;
}

int get_at(char * source, char target)
{
    int i = 0;
    while(source[i] != '\0'){
        if(source[i] == target)
            return i;
        i++;
    }
    return -1;
}

/// e como o vetor vai desalocar as paradas
void pegar_obj_de_arquivo(vector<float *> & vertices, vector<int *> & faces, const char * local)
{
    FILE *arquivo;
    const int len_buffer = 255;
    char buffer[len_buffer];
    arquivo = fopen(local, "r");

    if(!arquivo) die("Nao foi possivel abrir o arquivo");

    // leitura
    // a face pode ter
    // f  883/8837
    // f  x/y   -> ponto con tres coordenadas, considera o z zero
    char * tmp_char;
    int a, b, c, tmpi, index;
    float d, e, f, tmpf;
    int count_faces = 0;
    int count_vetices = 0;

/*    float *p1, *p2, *p3;*/
    while(fgets(buffer, len_buffer, arquivo)){
        if(buffer[0] == '#') continue;
        if(buffer[0] == 'f' && buffer[1] == ' '){// faces
/*            printf("%s", buffer);*/

            tmp_char = strtok(buffer+1, " \n");

            while(tmp_char){
                // tmp_char pode ter '8373/83874'
                if(contain(tmp_char, '/')){
                    index = get_at(tmp_char, '/');
                    assert(index != -1);
                    tmp_char[index] = '\0';
                }

                tmpi = atoi(tmp_char);// apenas int
                tmpi = tmpi - 1;
                count_faces++;
                if(count_faces == 1){
                    a = tmpi;
                } else if(count_faces == 2){
                    b = tmpi;
                } else if(count_faces == 3){ // ponto chave
                    c = tmpi;
                    count_faces = 0;
                    // add(faces, create_lst3i(a, b, c));
                    faces.push_back(create_lst3(a, b, c));
                }
                tmp_char = strtok(NULL, " \n");
            }

        } else if(buffer[0] == 'v' && buffer[1] == ' '){// vetices
            tmp_char = strtok(buffer+1, " \n");

            while(tmp_char){
                tmpf = atof(tmp_char);

                count_vetices++;
                if(count_vetices == 1){
                    d = tmpf;
                } else if(count_vetices == 2){
                    e = tmpf;
                } else if(count_vetices == 3){// ponto chave
                    f = tmpf;
                    count_vetices = 0;
                    //add(vertices, create_lst3f(d, e, f));
                    vertices.push_back(create_lst3(d, e, f));
                }

                tmp_char = strtok(NULL, " \n");
            }

        }
    }
    fclose(arquivo);
}


int * create_lst2(int a, int b)
{
    int *nv_lst = new int[2]; //malloc(2*(sizeof(int)));
    assert(nv_lst != NULL);
    nv_lst[0] = a;
    nv_lst[1] = b;
    return nv_lst;
}

// nao chama clear
void desenhar_obj(vector<float *> & lst_vertices, vector<int *> & lst_faces)
{
    float *p1, *p2, *p3;
    int * pt_face;
    int i, max = lst_faces.size();
    for(i=0; i<max; i++){
        pt_face = lst_faces[i];//get(lst_faces, i);
        // 93 96 97
        p1 = lst_vertices[pt_face[0]];//get(lst_vertices, pt_face[0]); // primeiro valor 1 da face
        p2 = lst_vertices[pt_face[1]];//get(lst_vertices, pt_face[1]);
        p3 = lst_vertices[pt_face[2]];//get(lst_vertices, pt_face[2]);

        desenhar_arestas_pontos(p1, p2, p3);

    }
    glFlush();
    //glutSwapBuffers(); // net.c
}

// *****************
// *** nova parte **
// *****************

Obj3d * st_create_obj()
{
    Obj3d * nw_obj = new Obj3d;
    nw_obj->lst_faces = new vector<int *>;
    nw_obj->lst_vetices = new vector<float *>;

    return nw_obj;
}

// deve ser chamado no final do processo
void st_destroy_obj(Obj3d * ob)
{
    // primeiro eu tenho que deletar o conteudo
    // depois o vetor propriamente dito
    clear_lst(*ob->lst_faces);// uso * em um ponteiro pq clear_lst
                              // espera uma referencia a um objeto
    clear_lst(*ob->lst_vetices);
    delete ob->lst_faces;
    delete ob->lst_vetices;
    delete ob;
}

void st_desesenhar_obj(Obj3d * ob)
{
    // usa o desenhar_obj
    desenhar_obj(*ob->lst_vetices, *ob->lst_faces);

}


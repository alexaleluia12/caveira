#ifndef __COMUM_H__
#define __COMUM_H__

#include <vector>

using namespace std;

typedef struct {
    vector<float *> * lst_vetices;
    vector<int *> * lst_faces;
    float referencia[3];// o ponto sobre o qual vai ser realizadas as transformacoes
} Obj3d;
// ==========
// parte do objeto 3d

// alloca memaria para alguns elementos
Obj3d * st_create_obj();

void st_destroy_obj(Obj3d * ob);

// desaloca a memoria allocada em st_create_obj
void st_desesenhar_obj(Obj3d * ob);

// ==========
void die(const char *message);

void ponto2i(int x, int y);

void ponto3i(int x, int y, int z);

void desenhar_poligono_pontos(float * p1, float * p2, float * p3);

void desenhar_arestas_pontos(float * p1, float * p2, float * p3);

void get_centro_massa(vector<float *> & lst, float * destiny);

inline float get_distancia(float *p1, float *p2);

// retorna o posicao na list do ponto mais proximo de point
int  get_mais_proximo_de(vector<float *> & lst, float *point);
// ** as novas funcoes de transformacao apenas modificam o que esta em
// ** lst NAO DESENHAM NADA

// value -> quantidade que eu quero transladar
// eixo  -> eixo que vai ser tranladado
//          (deve ser uma posicao no array de vertices)
void transladar_pontos_n(vector<float *> & lst, float value, int eixo);

// assume que os array de ponto em lst tem 3 componentes (x,y,z)
void escalar_pontos(vector<float *> & lst, float value);

// assume que cm_point tem 3 elementos
void desloca_centro(vector<float *> & lst, float * cm_point);

// assume que cm_point tem 3 elementos
void desloca_to(vector<float *> & lst, float * cm_point);

// returna 1 se target esta em source, 0 caso contrario
int contain(char * source, char target);

// retorna a posicao de target em source, -1 caso nao ache
int get_at(char * source, char target);

// local - nome do arquivo
void pegar_obj_de_arquivo(vector<float *> & vertices, vector<int *> & faces, const char * local);


int * create_lst2(int a, int b);

/// **** COISAS RELACIONADAS A TEMPLATE DEVEM SER IMPLEMENTADAS NO .h ****

// chamada no final do processo limpa os conteudos do vector que foram alocados
// por mim
template<typename T>
void clear_lst(vector<T *> & lst)
{
    int i, max = lst.size();
    T * tmp;
    for(i=0; i<max; i++){
      tmp = lst[i];
      delete[] tmp;
    }
}

template<typename T>
T * create_lst3(T a, T b, T c)
{
    T *nv_lst = new T[3];//malloc(3*(sizeof(float)));
    assert(nv_lst != NULL);
    nv_lst[0] = a;
    nv_lst[1] = b;
    nv_lst[2] = c;
    return nv_lst;
}

// desenha objetro 3d baseado na lista de vetices e aresta
// assume-se que ambos tem elementos de tamanho 3
void desenhar_obj(vector<float *> & lst_vertices, vector<int *> & lst_faces);

#endif /* __COMUM_H__ */


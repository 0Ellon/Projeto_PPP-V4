#ifndef PROJECTO_PPP_H
#define PROJECTO_PPP_H
#include "Datas.h"


/* INFORMAÇÕES_PESSOAIS:

    ID_DOENTE
    NOME
    DATA
    CARTÃO_CIDADÃO
    TELEFONE
    EMAIL
*/
/* INFORMAÇÕES_MÉDICAS:

    ID_DOENTE
    DATA_REGISTRO
    TENSÃO_MÁXIMA
    TENSÃO_MÍNIMA
    PESO
    ALTURA 
*/


typedef struct Pessoais{
    int ID_Doente, Telefone;
    char *Nome, *Cartão_Cidadão, *Email;
    Data Data;
}Pessoais;

typedef struct Medicas {
    int ID_Doente, Tensao_max, Tensao_min, Peso, Altura;
    Data Data_Registo;
}Medicas;

typedef struct noLista{
    Pessoais *dados_pessoais;
    Medicas *dados_medicos;
    struct noLista * prox;
} noLista;

typedef noLista * pLista;

typedef struct {
    char *nome;
    int id;
} NomeID;

typedef struct {
    int id, tensao_max;
    char *nome;
} TensaoNome;


#endif
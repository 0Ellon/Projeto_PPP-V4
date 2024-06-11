#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Projecto_PPP.h"

// FUNÇÕES PARA INTERFACE, AJUDAR NO ESPAÇAMENTO E VISUALIZAÇÃO:
void space() {
    printf("\n-----------------------------------------------------------------------------------------\n\n");
}

void printf_para_avisos_que_tem_variavel() {
     printf("\n==========================================================================================\n");
}

void printf_para_avisos(const char *prompt) {
    printf("===========================================================================================\n");
    printf("%s", prompt);
    printf("\n==========================================================================================\n");
}
//
// FUNÇÕES PARA 'ERROS HUMANOS':
int ate_ser_inteiro(const char *prompt) {
    int valor, num_read;
    char linha[1024];
    char extra;

    do {
        printf("%s", prompt);
        // Caso a leitura falhe, tenta ler novamente
        if (!fgets(linha, sizeof(linha), stdin)) {
            printf_para_avisos("ERRO DE LEITURA. POR FAVOR, TENTE NOVAMENTE");
            continue;
        }

        linha[strcspn(linha, "\n")] = 0;

        // Tenta converter a linha para um inteiro seguido de algo mais
        num_read = sscanf(linha, "%d%c", &valor, &extra);

        // Verifica se somente um inteiro foi lido e nada mais
        if (num_read == 1) {
            break; 
        } else {
            printf_para_avisos("ENTRADA INVALIDA. POR FAVOR, DIGITE APENAS UM NÚMERO INTEIRO.");
        }

    } while (1);

    return valor;
}

int data_valida(int dia, int mes, int ano) {
    // Verifica se o ano é válido
    if (ano <= 0) return 0;
    
    // Verifica se o mês é válido
    if (mes < 1 || mes > 12) return 0;
    
    // Define o número de dias em cada mês (exceto fevereiro)
    int dias_no_mes[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Ajusta o número de dias de fevereiro se o ano for bissexto
    // (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0) Vê se o ano é bissexto
    if (mes == 2 && ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))) {
        dias_no_mes[1] = 29;
    }

    // Verifica se o dia é válido
    if (dia < 1 || dia > dias_no_mes[mes - 1]) {
        return 0;
    }

    return 1;
}

Data ate_ser_data_valida(const char *prompt) {
    Data data;
    char entrada[100];
    int resultado;

    do {
        printf("%s (DD.MM.AAAA): ", prompt);
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            printf_para_avisos("ERRO DE LEITURA. POR FAVOR, TENTE NOVAMENTE");
            continue;
        }

        entrada[strcspn(entrada, "\n")] = '\0';

        // Análise da string("%d.%d.%d") para extrair dia, mês e ano
        resultado = sscanf(entrada, "%d.%d.%d", &data.dia, &data.mes, &data.ano);

        if (resultado != 3) {
            printf_para_avisos("FORMATO INVÁLIDO. POR FAVOR, DIGITE A DATA NO FORMATO DD.MM.AAAA");
            continue;
        }

        if (!data_valida(data.dia, data.mes, data.ano)) {
            printf_para_avisos("DATA INVÁLIDA, CERTIFIQUE-SE DE QUE OS VALORES ESTÃO CORRETOS\n(DIA, MÊS, ANO)");
            resultado = 0;
        }

    } while (resultado != 3);

    return data;
}

int eh_letra(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == ' ');
}

/* Função pra ler, alocar a memoria e pra armazenar na memória */
char* ler_string(const char* prompt, int tamanho) {
    /* uma maneira de generalizar algumas partes do codigo
    p->Cartão_Cidadão = malloc(100);
    printf("DIGITE O SEU CARTÃO CIDADÃO: ");
    fgets(p->Cartão_Cidadão, 100, stdin);
    p->Cartão_Cidadão[strcspn(p->Cartão_Cidadão, "\n")] = 0; */

    char *string = malloc(tamanho);
    if (string == NULL) {
        printf_para_avisos("ERRO AO ALOCAR A MEMÓRIA");
        return NULL; 
    }

    printf("%s", prompt); 
    if (fgets(string, tamanho, stdin) == NULL) {
        free(string); 
        return NULL;
    }

    string[strcspn(string, "\n")] = '\0';

    return string;
}

/* Função pra ler se a string possui apenas letras */
char* ler_string_apenas_letras(const char *prompt, int tamanho) {
    char *entrada;
    int valido;

    do {
        entrada = ler_string(prompt, tamanho);
        if (entrada == NULL) {
            continue; 
        }

        valido = 1;  
        for (int i = 0; entrada[i] != '\0'; i++) {
            if (!eh_letra(entrada[i])) {
                printf_para_avisos("ENTRADA INVÁLIDA. POR FAVOR, DEVE APENAS CONTER LETRAS");
                valido = 0;
                break;
            }
        }
        if (valido) {
            if (entrada[0] >= 'a' && entrada[0] <= 'z') {
                entrada[0] = entrada[0] - 32;
            }
        }

        else free(entrada); // Libere a memória se a entrada for inválida
        
    } while (!valido);

    return entrada; // Retorna a entrada válida
}
//
// FUNÇÕES PARA CRIAÇÃO DA LISTA:
pLista cria() { 
    // Cria um nó cabeçalho
    pLista nova = malloc(sizeof(struct noLista));
    if (nova != NULL) {
        nova->dados_pessoais = NULL;
        nova->dados_medicos = NULL;
        nova->prox = NULL;
    }
    return nova;  
}

void insere(pLista lista, Pessoais *p) {
    pLista novo = malloc(sizeof(struct noLista));
    if (novo == NULL) {
        printf_para_avisos("FALHA AO ALOCAR A MEMÓRIA PARA O NOVO NÓ");
        return;
    }
  
    novo->dados_pessoais = p;
    novo->dados_medicos = NULL;
    novo->prox = NULL;

    // Encontra o final da lista
    pLista aux = lista;
    while (aux->prox != NULL) {
        aux = aux->prox;
    }
    aux->prox = novo;
    
}

void procura(pLista lista, int ID, pLista *ant, pLista *actual) {
    *ant = NULL;
    *actual = lista->prox;  

    while (*actual != NULL && (*actual)->dados_pessoais->ID_Doente != ID) {
        *ant = *actual;
        *actual = (*actual)->prox;
    }
    // Após o loop, *actual é o nó com o id ou NULL se não encontrado
    // *ant é o nó anterior a *actual ou NULL se *actual é o primeiro nó
}

void elimina(pLista lista) {
    int ID = ate_ser_inteiro("DIGITE O ID DO DOENTE PARA ELIMINAR: ");

    pLista ant, actual;
    procura(lista, ID, &ant, &actual);

    if (actual == NULL) {
        printf_para_avisos_que_tem_variavel();
        printf("DOENTE COM ID %d NÃO ENCONTRADO\n", ID);
        printf_para_avisos_que_tem_variavel();
        return;
    }

    // Se "ant" é NULL, então "actual" é o primeiro nó após o cabeçalho
    if (ant == NULL) {
        lista->prox = actual->prox;
    } else {
        ant->prox = actual->prox;
    }

    // Liberação de memória associada ao nó 'actual'
    if (actual->dados_pessoais) {
        free(actual->dados_pessoais->Nome);
        free(actual->dados_pessoais->Cartão_Cidadão);
        free(actual->dados_pessoais->Email);
        free(actual->dados_pessoais);
    }

    if (actual->dados_medicos) {
        free(actual->dados_medicos);
    }

    free(actual);
    printf_para_avisos_que_tem_variavel();
    printf("DOENTE COM ID %d ELIMINADO COM SUCESSO", ID);
    printf_para_avisos_que_tem_variavel();
}

void destroi(pLista lista) {
    pLista temp;

    while (lista != NULL) {
        temp = lista->prox; // Guarda o próximo nó.

        if (lista->dados_pessoais) {
            free(lista->dados_pessoais->Nome);
            free(lista->dados_pessoais->Cartão_Cidadão);
            free(lista->dados_pessoais->Email);
            free(lista->dados_pessoais);
        }

        if (lista->dados_medicos) {
            free(lista->dados_medicos);
        }

        free(lista);
        lista = temp;
    }
}

void ler_dados_Usuario(Pessoais *p, pLista lista) {
    
    if (!lista || !lista->prox) {
        p->ID_Doente = 1;  
    } else {
        pLista ultimo = lista;
        while (ultimo->prox) { 
            ultimo = ultimo->prox;
        }
        p->ID_Doente = ultimo->dados_pessoais->ID_Doente + 1; 
    }

    printf("TU ÉS O ID: %d\n", p->ID_Doente);

    p->Nome = ler_string_apenas_letras("DIGITE O SEU NOME: ", 255);
    p->Data = ate_ser_data_valida("DIGITE A SUA DATA DE NASCIMENTO");
    p->Cartão_Cidadão = ler_string("DIGITE O SEU CARTÃO CIDADÃO: ", 50);  
    p->Telefone = ate_ser_inteiro("DIGITE O SEU TELEFONE: ");
    p->Email = ler_string("DIGITE O SEU EMAIL: ", 255);

}

void ler_dados_medicos_usuario(pLista lista) {
    int ID = ate_ser_inteiro("DIGITE UM ID_DOENTE PRA PESQUISAR: ");
    pLista atual = lista->prox;
    
    while (atual && atual->dados_pessoais->ID_Doente != ID) {
        atual = atual->prox;
    }

    if (!atual) {
        printf_para_avisos_que_tem_variavel();
        printf("PACIENTE COM ID %d NÃO ENCONTRADO", ID);
        printf_para_avisos_que_tem_variavel();
        return;
    }

    if (!atual->dados_medicos) {
        atual->dados_medicos = malloc(sizeof(Medicas));
        if (!atual->dados_medicos) {
            printf_para_avisos("FALHAR AO ALOCAR MEMÓRIA PARA DADOS MÉDICOS");
            return;
        }
    }

    // Preenche os dados médicos
    atual->dados_medicos->ID_Doente = ID;
    printf("ID: %3d | NOME: %-20s\n", atual->dados_medicos->ID_Doente ,atual->dados_pessoais->Nome);
    atual->dados_medicos->Data_Registo = ate_ser_data_valida("DIGITE A DATA DE REGISTRO");
    atual->dados_medicos->Tensao_max = ate_ser_inteiro("DIGITE A TENSÃO MAX: ");
    atual->dados_medicos->Tensao_min = ate_ser_inteiro("DIGITE A TENSÃO MIN: ");
    atual->dados_medicos->Peso = ate_ser_inteiro("DIGITE O PESO (kg): ");
    atual->dados_medicos->Altura = ate_ser_inteiro("DIGITE A ALTURA (cm): ");
}

void imprime_ordenado(pLista lista) {
    pLista atual = lista->prox;

    if (!atual) {
        printf_para_avisos("LISTA VAZIA!");
        return;
    }

    // Contar o número de elementos na lista para o bubble sort
    int count = 0;

    while (atual) {
        count++;
        atual = atual->prox;
    }

    // Aloca memória para um array de estruturas NomeID
    NomeID* dados = (NomeID* ) malloc(count * sizeof(NomeID));
    if (!dados) {
        printf_para_avisos("ERRO AO ALOCAR A MEMÓRIA PARA A ORDENAÇÃO");
        return;
    }

    // Copia os nomes e IDs para o array
    atual = lista->prox;
    int i = 0;
    while (atual && i < count) {
        dados[i].nome = atual->dados_pessoais->Nome;
        dados[i].id = atual->dados_pessoais->ID_Doente;
        atual = atual->prox;
        i++;
    }
    // Ordena o array de NomeID com o Bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count -i -1; j++) {
            if (strcmp(dados[j].nome, dados[j + 1].nome) > 0) {
                NomeID temp = dados[j];
                dados[j] = dados[j+1];
                dados[j+1] = temp;
            }
        }
    }


    printf("NOMES EM ORDEM ALFABÉTICA COM IDs:\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %3d | NOME: %-30s\n", dados[i].id, dados[i].nome);
    }

    free(dados);
}

void imprime_dados_doente(pLista lista) {

    if (lista == NULL || lista->prox == NULL) {
        printf_para_avisos("A LISTA ESTÁ VAZIA");
        return;
    }

    int ID = ate_ser_inteiro("DIGITE O ID DO DOENTE PARA VER OS DADOS: ");

    pLista atual = lista->prox;
    int encontrado = 0;

    while (atual != NULL) {
        if (atual->dados_pessoais && atual->dados_pessoais->ID_Doente == ID) {
            printf("\nDADOS PESSOAIS:\n\n");
            printf("ID: %d\nNOME: %s\nDATA DE NASCIMENTO: %02d/%02d/%04d\nCARTÃO CIDADÃO: %s\nTELEFONE: %d\nEMAIL: %s\n", 
            atual->dados_pessoais->ID_Doente, 
            atual->dados_pessoais->Nome, 
            atual->dados_pessoais->Data.dia, atual->dados_pessoais->Data.mes, atual->dados_pessoais->Data.ano,
            atual->dados_pessoais->Cartão_Cidadão, 
            atual->dados_pessoais->Telefone, 
            atual->dados_pessoais->Email);
            
            if (atual->dados_medicos) {
                printf("\nDADOS MÉDICOS:\n\n");
                printf("ID: %d\nDATA DE REGISTRO: %02d/%02d/%04d\nTENSÃO MAX: %d\nTENSÃO MIN: %d\nPESO: %d kg\nALTURA: %d cm\n", 
                atual->dados_medicos->ID_Doente, 
                atual->dados_medicos->Data_Registo.dia, atual->dados_medicos->Data_Registo.mes, atual->dados_medicos->Data_Registo.ano,
                atual->dados_medicos->Tensao_max,
                atual->dados_medicos->Tensao_min,
                atual->dados_medicos->Peso,
                atual->dados_medicos->Altura);

            } else {
                printf_para_avisos("DADOS MÉDICOS AINDA NÃO REGISTRADOS");
            }

            encontrado = 1;
            break;
        }
        atual = atual->prox;
    }

    if (!encontrado) {
        printf_para_avisos_que_tem_variavel();
        printf("DOENTE COM ID %d NÃO ENCONTRADO", ID);
        printf_para_avisos_que_tem_variavel();
    }
}

void listar_doentes_com_tensoes_acima_de_um_valor(pLista lista) {

    if (lista == NULL || lista->prox == NULL) {
        printf_para_avisos("A LISTA ESTÁ VAZIA");
        return;
    }

    int valor_minimo = ate_ser_inteiro("DIGITE O VALOR DA TENSÃO MÁXIMA DESEJADA: ");

    pLista atual = lista->prox;
    int count = 0;

    while (atual != NULL) {
        if (atual->dados_medicos && atual->dados_medicos->Tensao_max > valor_minimo) {
            count++;
        }
        atual = atual->prox;
    }

    TensaoNome *doentes = malloc(count * sizeof(TensaoNome));
    if (doentes == NULL) {
        printf_para_avisos("ERRO AO ALOCAR MEMÓRIA");
        return;
    }

    atual = lista->prox;
    int i = 0;
    while (atual != NULL) {
        if (atual->dados_medicos && atual->dados_medicos->Tensao_max > valor_minimo) {
            doentes[i].tensao_max = atual->dados_medicos->Tensao_max;
            doentes[i].id = atual->dados_medicos->ID_Doente;
            doentes[i].nome = atual->dados_pessoais->Nome;
            i++;
        }
        atual = atual->prox;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count -i -1; j++) {
            if (doentes[j].tensao_max < doentes[j + 1].tensao_max) {
                TensaoNome temp = doentes[j];
                doentes[j] = doentes[j + 1];
                doentes[j + 1] = temp;
            }
        }
    }
    
    printf("LISTANDO DOENTES COM TENSÃO MÁXIMA ACIMA DE %d (MAIOR PARA O MENOR):\n\n", valor_minimo);
    for (int i = 0; i < count; i++) {
        printf("ID: %3d | NOME: %-30s | TENSÃO MÁXIMA: %d\n", doentes[i].id, doentes[i].nome, doentes[i].tensao_max);
    }

    // Liberação da memória alocada
    free(doentes);
}

void alterar_Dados_usuario(pLista lista) {
    int ID = ate_ser_inteiro("DIGITE UM ID_DOENTE PRA PESQUISAR: ");
    pLista atual = lista->prox;
    
    while (atual && atual->dados_pessoais->ID_Doente != ID) {
        atual = atual->prox;
    }

    if (!atual) {
        printf_para_avisos_que_tem_variavel();
        printf("PACIENTE COM ID %d NÃO ENCONTRADO", ID);
        printf_para_avisos_que_tem_variavel();
        return;
    }

    if (!atual->dados_pessoais) {
        atual->dados_pessoais = malloc(sizeof(Pessoais));
        if (!atual->dados_pessoais) {
            printf_para_avisos("FALHAR AO ALOCAR MEMÓRIA PARA DADOS PESSOAIS");
            return;
        }
    }

    atual->dados_pessoais->ID_Doente = ID;
    printf("\nALTERE AS INFORMAÇÕES REGISTRADAS DE:\n\nID: %3d\n", atual->dados_pessoais->ID_Doente);
    
    printf("NOME REGISTRADO: %s\n", atual->dados_pessoais->Nome);
    printf("DATA DE NASCIMENTO REGISTRADA: %d/%d/%d\n", atual->dados_pessoais->Data.dia, atual->dados_pessoais->Data.mes, atual->dados_pessoais->Data.ano);
    printf("CARTÃO CIDADÃO REGISTRADO: %s\n", atual->dados_pessoais->Cartão_Cidadão);
    printf("TELEFONE REGISTRADO: %d\n", atual->dados_pessoais->Telefone);
    printf("EMAIL REGISTRADO: %s\n", atual->dados_pessoais->Email);
    space();
    // liberar memória alocada dos ponteiros para a reescrita
    free(atual->dados_pessoais->Nome);
    free(atual->dados_pessoais->Cartão_Cidadão);
    free(atual->dados_pessoais->Email);

    atual->dados_pessoais->Nome = ler_string_apenas_letras("DIGITE O SEU NOME: ", 255);
    atual->dados_pessoais->Data = ate_ser_data_valida("DIGITE A SUA DATA DE NASCIMENTO");
    atual->dados_pessoais->Cartão_Cidadão = ler_string("DIGITE O SEU CARTÃO CIDADÃO: ", 50);  
    atual->dados_pessoais->Telefone = ate_ser_inteiro("DIGITE O SEU TELEFONE: ");
    atual->dados_pessoais->Email = ler_string("DIGITE O SEU EMAIL: ", 255);

    printf_para_avisos("ALTERAÇÕES FEITAS COM SUCESSO!");
}

void salvar_dados(pLista lista, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf_para_avisos("ERRO AO ABRIR O ARQUIVO PARA A ESCRITA");
        return;
    }

    pLista atual = lista->prox;
    while (atual != NULL) {
        fprintf(arquivo, "%d\n%s\n%d/%d/%d\n%s\n%d\n%s\n", 
                atual->dados_pessoais->ID_Doente, 
                atual->dados_pessoais->Nome,
                atual->dados_pessoais->Data.dia, 
                atual->dados_pessoais->Data.mes, 
                atual->dados_pessoais->Data.ano,
                atual->dados_pessoais->Cartão_Cidadão,
                atual->dados_pessoais->Telefone, 
                atual->dados_pessoais->Email);
        atual = atual->prox;
    }

    fclose(arquivo);
}

pLista carregar_dados(const char *nome_arquivo) {
    pLista lista = cria(); // Cria o nó cabeçalho

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("ERRO AO ABRIR O ARQUIVO PARA LEITURA");
        return lista;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {

        Pessoais *dados = (Pessoais *)malloc(sizeof(Pessoais));
        if (dados == NULL) {
            printf("FALHA AO ALOCAR A MEMÓRIA PARA DADOS PESSOAIS");
            fclose(arquivo);
            return lista;
        }

        dados->ID_Doente = 0;
        dados->Nome = NULL;
        dados->Data = (Data) {0,0,0};
        dados->Cartão_Cidadão = NULL;
        dados->Telefone = 0;
        dados->Email = NULL;

        // Ler ID_Doente
        sscanf(linha, "%d", &dados->ID_Doente);

        // Ler Nome
        fgets(linha, sizeof(linha), arquivo);
        linha[strcspn(linha, "\n")] = '\0';  // Remover nova linha
        dados->Nome = strdup(linha);

        // Ler Data
        fgets(linha, sizeof(linha), arquivo);
        sscanf(linha, "%d/%d/%d", &dados->Data.dia, &dados->Data.mes, &dados->Data.ano);
        printf("%d/%d/%d\n", dados->Data.dia, dados->Data.mes, dados->Data.ano );

        // Ler Cartao_Cidadao
        fgets(linha, sizeof(linha), arquivo);
        linha[strcspn(linha, "\n")] = '\0';  // Remover nova linha
        dados->Cartão_Cidadão = strdup(linha);

        // Ler Telefone
        fgets(linha, sizeof(linha), arquivo);
        sscanf(linha, "%d", &dados->Telefone);

        // Ler Email
        fgets(linha, sizeof(linha), arquivo);
        linha[strcspn(linha, "\n")] = '\0';  // Remover nova linha
        dados->Email = strdup(linha);

  
        insere(lista, dados);
    }

    fclose(arquivo);
    return lista;
}

void salvar_dados_medicos(pLista lista, const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf_para_avisos("ERRO AO ABRIR O ARQUIVO PARA A ESCRITA");
        return;
    }

    pLista atual = lista->prox;
    while (atual != NULL) {
        if (atual->dados_medicos != NULL) {
            fprintf(arquivo, "%d\n%d/%d/%d\n%d\n%d\n%d\n%d\n", 
                    atual->dados_medicos->ID_Doente, 
                    atual->dados_medicos->Data_Registo.dia, 
                    atual->dados_medicos->Data_Registo.mes, 
                    atual->dados_medicos->Data_Registo.ano,
                    atual->dados_medicos->Tensao_max, 
                    atual->dados_medicos->Tensao_min, 
                    atual->dados_medicos->Peso, 
                    atual->dados_medicos->Altura);
        }
        atual = atual->prox;
    }

    fclose(arquivo);
}

void carregar_dados_medicos(pLista lista, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("ERRO AO ABRIR O ARQUIVO PARA LEITURA");
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {

        // Alocar memória para os dados médicos
        Medicas *dados_medicos = (Medicas *)malloc(sizeof(Medicas));
        if (dados_medicos == NULL) {
            printf("FALHA AO ALOCAR A MEMÓRIA PARA DADOS MÉDICOS");
            fclose(arquivo);
            return;
        }

        // Ler ID_Doente
        sscanf(linha, "%d", &dados_medicos->ID_Doente);
            
        // Encontrar o nó na lista com o ID_Doente correspondente
        pLista atual = lista->prox;
        while (atual != NULL) {
            if (atual->dados_pessoais && atual->dados_pessoais->ID_Doente == dados_medicos->ID_Doente) {
                // Encontrou o paciente com o ID correspondente, então inserir os dados médicos

                // Ler a data de registro
                fgets(linha, sizeof(linha), arquivo);
                sscanf(linha, "%d/%d/%d", &dados_medicos->Data_Registo.dia, &dados_medicos->Data_Registo.mes, &dados_medicos->Data_Registo.ano);

                // Ler a tensão máxima
                fgets(linha, sizeof(linha), arquivo);
                sscanf(linha, "%d", &dados_medicos->Tensao_max);

                // Ler a tensão mínima
                fgets(linha, sizeof(linha), arquivo);
                sscanf(linha, "%d", &dados_medicos->Tensao_min);

                // Ler o peso
                fgets(linha, sizeof(linha), arquivo);
                sscanf(linha, "%d", &dados_medicos->Peso);

                // Ler a altura
                fgets(linha, sizeof(linha), arquivo);
                sscanf(linha, "%d", &dados_medicos->Altura);

                // Atribuir os dados médicos ao paciente
                atual->dados_medicos = dados_medicos;
                break;
            }
            atual = atual->prox;
        }

        // Se o ID_Doente não foi encontrado na lista, liberar memória dos dados médicos
        if (atual == NULL) {
            printf("ERRO: ID de paciente não encontrado na lista.");
            free(dados_medicos);
        }
    }

    fclose(arquivo);
}

int main() {
    pLista lista = carregar_dados("dados.txt");
    carregar_dados_medicos(lista, "dados_medicos.txt");
    int opcao = 1;
    Pessoais *pessoa;

    do {
        space();
        opcao = ate_ser_inteiro("1 - INTRODUZIR DADOS DE UM NOVO DOENTE\n2 - ELIMINAR UM DOENTE EXISTENTE\n3 - LISTAR TODOS OS DOENTES POR ORDEM ALFABÉTICA\n4 - LISTAR OS DOENTES COM TENSÕES MÁXIMAS ACIMA DE UM DETERMINADO VALOR\n5 - APRESENTAR TODA A INFORMAÇÃO DE UM DETERMINADO DOENTE\n6 - REGISTAR AS TENSÕES, O PESO E A ALTURA DE UM DETERMINADO DOENTE NUM DETERMINADO DIA\n7 - ALTERAR DADOS PESSOAIS\n0 - SAIR\n\nDIGITE QUAL OPERAÇÃO DESEJA: ");
        space();

        switch (opcao) {
            case 1: // INTRODUZIR DADOS DE UM NOVO DOENTE
                pessoa = malloc(sizeof(Pessoais));
                if (pessoa == NULL) {
                    printf_para_avisos("ERRO AO ALOCAR MEMÓRIA PARA NOVOS DADOS PESSOAIS");
                    break;
                }
                ler_dados_Usuario(pessoa, lista);
                insere(lista, pessoa);
                salvar_dados(lista, "dados.txt");
                break;
            case 2: // ELIMINAR UM DOENTE EXISTENTE
                elimina(lista);
                salvar_dados(lista, "dados.txt");
                salvar_dados_medicos(lista, "dados_medicos.txt");
                break;

            case 3: // LISTAR TODOS OS DOENTES POR ORDEM ALFABÉTICA
                imprime_ordenado(lista);
                break;
            case 4: // LISTAR OS DOENTES COM TENSÕES MÁXIMAS ACIMA DE UM DETERMINADO VALOR
                listar_doentes_com_tensoes_acima_de_um_valor(lista);
                break;
            case 5: // APRESENTAR TODA A INFORMAÇÃO DE UM DETERMINADO DOENTE
                imprime_dados_doente(lista);
                break;
            case 6: // REGISTAR AS TENSÕES, O PESO E A ALTURA DE UM DETERMINADO DOENTE NUM DETERMINADO DIA
                space();
                ler_dados_medicos_usuario(lista);
                salvar_dados_medicos(lista, "dados_medicos.txt");
                break;
            case 7: // ALTERAR DADOS PESSOAIS
                alterar_Dados_usuario(lista);
                salvar_dados(lista, "dados.txt");
                break;
            case 0: // SAIR
                /* Pode-se retirar os dois salvar dados. 
                Mas eles estão presentes para que toda e qualquer alteração que foi feita seja efetivamente salva antes de finalizar o programa */
                salvar_dados(lista, "dados.txt");
                salvar_dados_medicos(lista, "dados_medicos.txt");
                destroi(lista);
                printf_para_avisos("OBRIGADO POR UTILIZAR O CÓDIGO, VOLTE SEMPRE!"); space();
                break;
            default:
                printf_para_avisos("OPÇÃO INVÁLIDA, POR FAVOR, ESCOLHA UMA OPÇÃO DISPONÍVEL");
                break;
        }
    } while (opcao != 0);

    return 0;
}
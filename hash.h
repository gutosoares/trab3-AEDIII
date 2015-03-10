/* Cabeçalho de Dados
*
*  Alunos: Augusto Soares
*          Álvaro Cozadi
*          Uslei Marcelino
*
* Arquivo com o cabeçalho de implementação da árvore B
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definição de algumas constantes de controle */
#define NUM_INICIAL_BLOCOS 10
#define TAM_BLOCO 3
#define NAO_ENCONTROU_CHAVE -1
#define ENCONTROU_CHAVE 1
#define HASH_CHEIO -2
#define SUCESSO 2
#define TAM_CHAVE 15
#define CHAVE_VAZIA "###############\0" /* Deve conter tantos ' # ' quanto o tamanho da chave */

/* Apontadores para cada índice */
extern FILE *IndiceHash;
extern FILE *Data;
extern FILE *IndiceID;
extern FILE *IndiceMAT;

/* Estrutura responsável por armazenar informações sobre o número de posições ativas e a quantidade total de dados */
typedef struct CabecalhoDados
{
	int Total;
	int Ativos;
} CabecalhoDados;

extern CabecalhoDados *dataHeader;

/* Estrura dos índices */
typedef struct Bloco
{
	char Chaves[TAM_BLOCO][15];
	int RRNs[TAM_BLOCO];
} Bloco;

/* Estrutura referente a Aluno */
typedef struct Aluno
{
	char Nome[50];
	char Identidade[TAM_CHAVE + 1];
	char CPF[15];
	char Matricula[TAM_CHAVE + 1];
	double RSG;
} Aluno;

/* Definição das funções de todo o programa */
int Hashing (char *chave);
int BuscaBloco(char *chave, int *RRN_Ret, int *RRN_ChRemovida);
Bloco* GetBloco(int RRN);
void WriteBloco(Bloco* bloco, int RRN);
int BuscaChave(char *chave);
int InsereChave(char *chave, int RRN_Dados);
void Driver(char* filename);
void Initialize(Bloco*bloco);
void ImprimeAluno(Aluno aluno);
int RemoverChave(char *chave);
void ImprimeAlunos();
void ImprimeBlocos();
void ImprimeBloco(Bloco bloco);

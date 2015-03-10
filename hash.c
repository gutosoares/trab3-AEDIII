/* Cabeçalho de Dados
*
*  Alunos: Augusto Soares
*          Álvaro Cozadi
*          Uslei Marcelino
*
* Arquivo com as implementações da função hashing, escrita e leitura de dados
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

FILE *IndiceHash = NULL;
FILE *Data = NULL;
FILE *IndiceID = NULL;
FILE *IndiceMAT = NULL;
CabecalhoDados *dataHeader = NULL;

/* Função hashing */
int Hashing (char *chave)
{
	int chaveNumerica = atoi(chave);
	return (chaveNumerica % NUM_INICIAL_BLOCOS);
}

/*
 * chave - chave a ser inserida
 * RRN_Ret - guarda o RRN do bloco a retornar
 * RRN_ChRemovida - gurada o RRN do primeiro bloco encontrado com uma chave removida a partir da primeira chamada
 */
int BuscaBloco(char *chave, int *RRN_Bloco, int *RRN_ChRemovida)
{
	*RRN_Bloco = Hashing(chave);
	*RRN_ChRemovida = -1;

	int blocoInicial, flagHashCheio = 0;
	blocoInicial = *RRN_Bloco;
	while (1)
	{
		Bloco *bloco = GetBloco(*RRN_Bloco);

		int i;
		for (i=0; i < TAM_BLOCO; i++)
		{
			if ((bloco->RRNs[i] == -2) && (*RRN_ChRemovida == -1))
			{
				*RRN_ChRemovida = *RRN_Bloco;
			}

			if (bloco->RRNs[i] == -1)
			{
				return NAO_ENCONTROU_CHAVE;
			}

			int aux;
			if ((aux = strcmp(chave, bloco->Chaves[i])) == 0)
			{ 	/* Encontrou a chave */
				return ENCONTROU_CHAVE;
			}
		}

		(*RRN_Bloco)++;
		if (*RRN_Bloco == NUM_INICIAL_BLOCOS)
		{
			 *RRN_Bloco = 0;
			 flagHashCheio = 1;
		}

		/* Teste se o hash está cheio */
		if ((*RRN_Bloco == blocoInicial) && (flagHashCheio == 1))
			return HASH_CHEIO;
	}
}

/*
 * Busca um bloco expecifico no arquivo *
 * RRN - RRN do bloco
 * Retorna um ponteiro para um bloco especifico
 */
Bloco* GetBloco(int RRN)
{
    fseek(IndiceHash, RRN * sizeof (struct Bloco), SEEK_SET);
    Bloco *bloco = malloc(sizeof(struct Bloco));
    fread(bloco, sizeof (struct Bloco), 1, IndiceHash);
    return bloco;
}

/* Função de escita dos dados no bloco */
void WriteBloco(Bloco* bloco, int RRN)
{
    fseek(IndiceHash, RRN * sizeof (struct Bloco), SEEK_SET);
    fwrite(bloco, sizeof (struct Bloco), 1, IndiceHash);

	free(bloco);
}

/*
 * chave - chave a ser procurada no arquivo
 * Retorna o RRN do registro no arquivo de dados ou -1 caso nao encontre
 *
 */
int BuscaChave(char *chave)
{
	int RRNBloco = -1;
	int RRNChRemovida = -1;
	int aux;
	aux = BuscaBloco(chave, &RRNBloco, &RRNChRemovida);
	if (aux == HASH_CHEIO)
	{
		return HASH_CHEIO;
	}

	else if (aux == NAO_ENCONTROU_CHAVE)
	{
		return NAO_ENCONTROU_CHAVE;
	}

	else if (aux == ENCONTROU_CHAVE)
	{
		Bloco *b;
		b = GetBloco(RRNBloco);
		int i = 0;
		while (i < TAM_BLOCO)
		{
			if ((aux = strcmp(b->Chaves[i], chave)) == 0)
				return b->RRNs[i];
			++i;
		}
	}

	return -1;
}

/* Função que ralizará a inserção de uma chave */
int InsereChave(char *chave, int RRN_Dados)
{
	int RRNBloco = -1;
	int RRNChRemovida = -1;

	int aux;
	aux = BuscaBloco(chave, &RRNBloco, &RRNChRemovida);
	if (aux == HASH_CHEIO)
	{
		return HASH_CHEIO;
	}

	else if (aux == ENCONTROU_CHAVE)
	{
		return ENCONTROU_CHAVE;
	}

	else if (aux == NAO_ENCONTROU_CHAVE)
	{
		Bloco *b;
		int i;

		/* Existe um bloco com a chave removida */
		if ((RRNChRemovida != -1))
		{
			b = GetBloco(RRNChRemovida);
			i = 0;
			while (b->RRNs[i] != -2)
			{
				++i;
			}

			strcpy(b->Chaves[i], chave);
			b->RRNs[i] = RRN_Dados;
			WriteBloco(b,RRNChRemovida);

			return SUCESSO;
		}

		b = GetBloco(RRNBloco);
		i = 0;
		while (b->RRNs[i] != -1)
		{
			++i;
		}

		strcpy(b->Chaves[i], chave);
		b->RRNs[i] = RRN_Dados;
		WriteBloco(b,RRNBloco);

		return SUCESSO;
	}
	return -1;
}

/* Carregando todos os arquivos e índices */
void Driver(char* filename)
{
	int tam = strlen(filename);
    char flID[30] = "";
    char flMAT[30] = "";

    strncpy(flMAT, filename, tam - 4);
    strcat(flMAT, "_MAT.dat\0");
    strncpy(flID, filename, tam - 4);
    strcat(flID, "_ID.dat\0");

    Data = fopen(filename, "rb+");
    IndiceID = fopen(flID, "rb+");
    IndiceMAT = fopen(flMAT, "rb+");

    if (IndiceMAT == NULL && IndiceID == NULL && Data == NULL)
	{
        IndiceID = fopen(flID, "wb+");
        IndiceMAT = fopen(flMAT, "wb+");
        Data = fopen(filename, "wb+");

		dataHeader = malloc (sizeof(struct CabecalhoDados));
		dataHeader->Total = 0;
		dataHeader->Ativos = 0;

		/* Inicializa hash MAT */
        IndiceHash = IndiceMAT;
		int i;
		for (i=0; i < NUM_INICIAL_BLOCOS; i++)
		{
			Bloco *bloco = 	malloc(sizeof (struct Bloco));
			Initialize(bloco);
			WriteBloco(bloco, i);
		}

		/* Inicializa arvore ID */
		IndiceHash = IndiceID;
		for (i=0; i < NUM_INICIAL_BLOCOS; i++)
		{
			Bloco *bloco = 	malloc(sizeof (struct Bloco));
			Initialize(bloco);
			WriteBloco(bloco, i);
		}
	}

	else
	{
		dataHeader = malloc (sizeof(struct CabecalhoDados));
		fseek(Data, 0, SEEK_SET);
		fread(&(*dataHeader), sizeof (struct CabecalhoDados), 1, Data);
	}
}

/* Remover chave desejada */
int RemoverChave(char *chave)
{
	int RRNBloco = -1;
	int RRNChRemovida = -1;
	int aux;
	aux = BuscaBloco(chave, &RRNBloco, &RRNChRemovida);
	if (aux == HASH_CHEIO)
	{
		return HASH_CHEIO;
	}

	else if (aux == NAO_ENCONTROU_CHAVE)
	{
		return NAO_ENCONTROU_CHAVE;
	}

	else if (aux == ENCONTROU_CHAVE)
	{
		Bloco *b;
		b = GetBloco(RRNBloco);
		int i = 0, RRNdados;
		while (i < TAM_BLOCO)
		{
			if ((aux = strcmp(b->Chaves[i], chave)) == 0)
			{
				RRNdados = b->RRNs[i];
				b->RRNs[i] = -2;
				WriteBloco(b, RRNBloco);
				return RRNdados;
			}

			++i;
		}
	}
	return -1;
}

void Initialize(Bloco*bloco)
{
    int i = 0;
    for (i = 0; i < TAM_BLOCO; i++)
	{
        strcpy(bloco->Chaves[i], CHAVE_VAZIA);
        bloco->RRNs[i] = -1;
	}
}

/* Função para imprimir o aluno com os seus valores */
void ImprimeAluno(Aluno aluno) {

	printf("<%s> <%s> <%s> <%s> <%.2lf>\n", aluno.Nome, aluno.Identidade, aluno.CPF, aluno.Matricula, aluno.RSG);
}

/* Função que prepar a leitura de cada aluno */
void ImprimeAlunos()
{
	int i;
	rewind(Data);
	fseek(Data, (sizeof(struct CabecalhoDados)), SEEK_SET);
	Aluno aluno;
	for(i = 0; i < dataHeader->Total; i++)
	{
		fread(&aluno, (sizeof (struct Aluno)), 1, Data);
		if (aluno.Nome[1] != '#') ImprimeAluno(aluno);
	}

}

/* Função que irar imprimir a chave do bloco e depois o seu RRN */
void ImprimeBloco(Bloco bloco)
{
	printf("[");
	int i;
	for (i=0; i< TAM_BLOCO - 1; i++)
	{
		if ((bloco.RRNs[i] != -1) && (bloco.RRNs[i] != -2))
		{
			printf("%s,%d|", bloco.Chaves[i], bloco.RRNs[i]);
		}

		else
		{
			printf("%d|", bloco.RRNs[i]);
		}
	}

	if ((bloco.RRNs[i] != -1) && (bloco.RRNs[i] != -2))
	{
		printf("%s,%d", bloco.Chaves[i], bloco.RRNs[i]);
	}

	else
	{
		printf("%d", bloco.RRNs[i]);
	}

	printf("]\n");
}

/* Função que irar imprimir todos os blocos com os seu dados */
void ImprimeBlocos()
{
	int i;
	for (i = 0; i < NUM_INICIAL_BLOCOS; i++)
	{
		Bloco *bloco;
		bloco = GetBloco(i);
		ImprimeBloco(*bloco);
	}
}

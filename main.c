/* Cabeçalho de Dados
*
*  Alunos: Augusto Soares
*          Álvaro Cozadi
*          Uslei Marcelino
*
* Arquivo principal com função main e interface implementadas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/*
* Função principal main e Interface.
*/
int main(int argc, char** argv)
{

	if (argc > 1)
	{
		char *filename;
		filename = argv[1];

		Driver(filename);
		printf("Sistema de gerenciamento de alunos usando hashing.\n");

		/* Operações solicitadas na descrião do trabalho */
		char INS[4] = "INS\0";
		char REM[4] = "REM\0";
		char BUS[4] = "BUS\0";
		char ATU[4] = "ATU\0";
		char EXI[4] = "EXI\0";
		char MAT[4] = "MAT\0";
		char ALU[4] = "ALU\0";
		char ID[3] = "ID\0";
		char FIM[4] = "FIM\0";
		char opcao[4] = "NAO\0";

		do
		{
			scanf("%s", opcao);

			/* INSERIR */
			if (strcmp(INS, opcao) == 0 )
			{
				Aluno novoAluno;

				scanf("%s %s %s %s %lf", novoAluno.Nome, novoAluno.Identidade, novoAluno.CPF, novoAluno.Matricula, &novoAluno.RSG);
				int erroChave;
				IndiceHash = IndiceMAT;

				erroChave = InsereChave(novoAluno.Matricula, dataHeader->Total);
				if (erroChave == SUCESSO)
				{
					IndiceHash = IndiceID;
					erroChave = InsereChave(novoAluno.Identidade, dataHeader->Total);
					if (erroChave == SUCESSO)
					{
							dataHeader->Total++;
							dataHeader->Ativos++;
							rewind(Data);
							fwrite(dataHeader, sizeof (struct CabecalhoDados), 1, Data);
							fseek(Data, 0, SEEK_END);
							fwrite(&novoAluno, sizeof (struct Aluno), 1, Data);
							printf("SUCESSO\n");
					}

					else
					{
						IndiceHash = IndiceMAT;
						RemoverChave(novoAluno.Matricula);
						printf("ERRO\n");
					}
				}

				else
				{
					printf("ERRO\n");
				}

			/*  REMOVER  */
			} else
				if (strcmp(REM, opcao) == 0)
				{
					char op[4], chave[TAM_CHAVE + 1];
					scanf("%s %s", op, chave);

					/* Por Identidade */
					if (strcmp(ID, op) == 0)
					{
						IndiceHash = IndiceID;
						int RRNdados = RemoverChave(chave);
						if (RRNdados >= 0)
						{
							Aluno aluno;
							fseek(Data, (sizeof(struct CabecalhoDados)) + RRNdados * (sizeof (struct Aluno)), SEEK_SET);
							fread(&aluno, (sizeof (struct Aluno)), 1, Data);
							fseek(Data, (sizeof(struct CabecalhoDados)) + RRNdados * (sizeof (struct Aluno)), SEEK_SET);
							fprintf(Data, "#");
							dataHeader->Ativos--;
							rewind(Data);
							fwrite(dataHeader, sizeof (struct CabecalhoDados), 1, Data);
							IndiceHash = IndiceMAT;
							RemoverChave(aluno.Matricula);
							printf("SUCESSO\n");
						}

						else
						{
							printf("ERRO\n");
						}

					} else
						if(strcmp(MAT, op) == 0) /* Por Matricula */
						{
							IndiceHash = IndiceMAT;
							int RRNdados = RemoverChave(chave);
							if (RRNdados >= 0)
							{

								Aluno aluno;
								fseek(Data, RRNdados * (sizeof (struct Aluno)), SEEK_SET);
								fread(&aluno, (sizeof (struct Aluno)), 1, Data);
								fseek(Data, RRNdados * (sizeof (struct Aluno)), SEEK_SET);
								fprintf(Data, "#");
								dataHeader->Ativos--;
								rewind(Data);
								fwrite(dataHeader, sizeof (struct CabecalhoDados), 1, Data);
								IndiceHash = IndiceID;
								RemoverChave(aluno.Identidade);
								printf("SUCESSO\n");

							}

							else
							{
								printf("ERRO\n");
							}
						}
					/* BUSCA */
				} else
					if (strcmp(BUS, opcao) == 0)
					{

						char op[4], chave[TAM_CHAVE + 1];
						scanf("%s %s", op, chave);

						/* Por Identidade */
						if (strcmp(ID, op) == 0)
						{
							IndiceHash = IndiceID;
							int RRNdados = BuscaChave(chave);
							if (RRNdados >= 0)
							{
								Aluno aluno;
								fseek(Data, (sizeof(struct CabecalhoDados)) + RRNdados * (sizeof (struct Aluno)), SEEK_SET);
								fread(&aluno, (sizeof (struct Aluno)), 1, Data);
								ImprimeAluno(aluno);

							}

							else
							{
								printf("ERRO\n");
							}

					} else
						if(strcmp(MAT, op) == 0) /* Por Matricula */
						{
							IndiceHash = IndiceMAT;
							int RRNdados = BuscaChave(chave);
							if (RRNdados >= 0)
							{
								Aluno aluno;
								fseek(Data, (sizeof(struct CabecalhoDados)) + RRNdados * (sizeof (struct Aluno)), SEEK_SET);
								fread(&aluno, (sizeof (struct Aluno)), 1, Data);
								ImprimeAluno(aluno);
							}

							else
							{
								printf("ERRO\n");
							}
						}
					/* ATUALIZA */
					} else
						if (strcmp(ATU, opcao) == 0)
						{
							char op[4];
							char chave[TAM_CHAVE + 1];
							Aluno novoAluno;
							scanf("%s %s %s %s %s %s %lf", op, chave, novoAluno.Nome, novoAluno.Identidade, novoAluno.CPF, novoAluno.Matricula, &novoAluno.RSG);

							/* Por identidade */
							if (strcmp(ID, op) == 0)
							{

								IndiceHash = IndiceID;
								int RRNdados = BuscaChave(chave);
								if (RRNdados >= 0)
								{

									Aluno aluno;
									fseek(Data, (sizeof(struct CabecalhoDados)) + RRNdados * (sizeof (struct Aluno)), SEEK_SET);
									fread(&aluno, (sizeof (struct Aluno)), 1, Data);
									int aux;

									if ((aux = strcmp(novoAluno.Identidade, aluno.Identidade)) != 0)
									{
										RemoverChave(aluno.Identidade);
										InsereChave(novoAluno.Identidade, RRNdados);
									}

									if ((aux = strcmp(novoAluno.Matricula, aluno.Matricula)) != 0)
									{
										IndiceHash = IndiceMAT;
										RemoverChave(aluno.Matricula);
										InsereChave(novoAluno.Matricula, RRNdados);
									}

									fseek(Data, (sizeof(struct CabecalhoDados)) + RRNdados * (sizeof (struct Aluno)), SEEK_SET);
									fwrite(&novoAluno, sizeof (struct Aluno), 1, Data);
									printf("SUCESSO\n");
								}

								else
								{
									printf("ERRO\n");
								}

						} else if(strcmp(MAT, op) == 0){ /* Por Matricula */

						IndiceHash = IndiceMAT;
						int RRNdados = BuscaChave(chave);
						if (RRNdados >= 0)
						{
							Aluno aluno;
							fseek(Data, (sizeof(struct CabecalhoDados)) + RRNdados * (sizeof (struct Aluno)), SEEK_SET);
							fread(&aluno, (sizeof (struct Aluno)), 1, Data);
							int aux;

							if ((aux = strcmp(novoAluno.Matricula, aluno.Matricula)) != 0)
							{
								RemoverChave(aluno.Matricula);
								InsereChave(novoAluno.Matricula, RRNdados);
							}

							if ((aux = strcmp(novoAluno.Identidade, aluno.Identidade)) != 0)
							{
								IndiceHash = IndiceID;
								RemoverChave(aluno.Identidade);
								InsereChave(novoAluno.Identidade, RRNdados);
							}

							fseek(Data, (sizeof(struct CabecalhoDados)) + RRNdados * (sizeof (struct Aluno)), SEEK_SET);
							fwrite(&novoAluno, sizeof (struct Aluno), 1, Data);
							printf("SUCESSO\n");
						}

						else
						{
							printf("ERRO\n");
						}
				}

			} else
				if (strcmp(EXI, opcao) == 0)
				{
					char op[4];
					scanf("%s", op);

					if (strcmp(ID, op) == 0)
					{
						IndiceHash = IndiceID;
						printf("Imprimindo blocos\n");
						ImprimeBlocos();
					} else
						if (strcmp(MAT, op) == 0)
						{
							IndiceHash = IndiceMAT;
							ImprimeBlocos();
					} else
						if (strcmp(ALU, op) == 0)
						{
							ImprimeAlunos();
						}

			} else
				if (strcmp(opcao, FIM) != 0)
				{
					printf("Comando invalido\n");
				}
		} while (strcmp(opcao, FIM) != 0); /* Fim da estrutura de controle */

		/* Encerramento do arquivo de matrícula e de IDs */
		fclose(IndiceMAT);
		fclose(IndiceID);
		free(dataHeader);
		fclose(Data);
		return (EXIT_SUCCESS);
	}

	/* Mensagem informando ao usuario um possivel erro na criação do arquivo .dat */
	else
	{
		printf("\n Favor passar o nome do arquivo de dados como parametro\n Ex= SisAlunos 'nome do arquivo de dados'");
		printf("\n OBS:Lembrando, que a extensão do arquivo deve ser .dat.\n");
		return (EXIT_FAILURE);
	}
}

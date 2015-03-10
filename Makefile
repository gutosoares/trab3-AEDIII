# Comando para compilar o arquivo
all:
	gcc main.c hash.c hash.h -o SisAlunos

# Comando para excluir todos os dados existentes
clear:
	rm *.dat

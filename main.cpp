#include <stdio.h>
#include <conio2.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include  "front.h"

struct tpAutor
{
	char sobrenome[100], nome[100];
	tpAutor *prox;
};

struct tpLivroBinario
{
	char autores[5][100], nome[5][100], sobrenome[5][100], titulo[100], editora[100];
	int ano, pag, tamAutores;
};

struct pListaAutores
{
	tpAutor *pAutor;
	pListaAutores *prox;
};

struct pLivro
{
	char titulo[100];
	int ano, pag;
	pLivro *ant, *prox;
	pListaAutores *ListaAutores;
};

struct pEditora
{
	char info[100];
	pEditora *prox;
	pLivro *plivro;
};

struct tpDescritor
{
	pEditora *inicio, *fim;
	int qtde;
};

//_____[PROTOTIPOS]____________________________________________________________________________________________________ 

//Arquivo:
void CriarArquivoBin(void);
void TxtPraBin(void);
void quebraAutor(char autor[], char nome[], char sobrenome[]);
void ExibirArqBin(void);

//Inicializar:
void NovoDescr(tpDescritor &Descr);

tpAutor *NovoAutor(char nome[100], char sobrenome[100]);
pEditora *NovaEditora(char elem[100]);
pLivro *NovoLivro(char titulo[100], int ano, int pag);
pListaAutores *NovaListaAutor(void);

//Inserir:
void InserirAutor(pListaAutores *ListaAutores, char nome[100], char sobrenome[100], tpAutor *AutoresOrigin);

void InserirEditoras(tpDescritor &D);
void InserirLivros(tpDescritor D, tpAutor *AutoresOrigin);
void InserirListaAutor(pLivro *livro, char nome[5][100], char sobrenome[5][100], int tamAutores, tpAutor *AutoresOrigin);

tpAutor * InserirAutoresOrigin(tpAutor *AutoresOrigin);
tpAutor * InserirAutoresSemLivro(tpAutor *AutorSemLivro, char nome[100],char sobrenome[100]);

//Exclusoes:
void ExcluirLivrosDin(tpDescritor D);

//Relatorios:
void RelatorioAutoresLivros(tpAutor *AutoresOrigin, tpDescritor D);
void RelatorioAutoresSemLivro(tpAutor *AutoresOrigin, tpDescritor D);
void RelatorioLivrosEditoras(tpDescritor D);

//Buscas:
tpAutor *buscaAutor(tpAutor *AutoresOrigin, char nome[100], char sobrenome[100]);
pEditora *buscaEditora(tpDescritor D, char editora[100]);

//Exibir:
void ExibirAutores(pListaAutores *ListaAutores);
void ExibirLivros(pEditora *Editora);

//Menus:
void Menu(tpDescritor D);
char MenuRel(tpDescritor D);
char MenuPrincipal (void);
//_______________________________________________________________________________________________________________________

//[Arquivo]:
 /*void CriarArquivoBin(void)
{
	FILE *ptrBin = fopen("livros2.dat", "wb");
	fclose(ptrBin);
} */

void TxtPraBin(void)
{
	tpLivroBinario liv;
	FILE *ptrTxt = fopen("livros.txt", "r");
	FILE *ptrBin = fopen("livros2.dat", "wb");

	int TL, repetida;
	char letra;

	while (!feof(ptrTxt))
	{
		// Pegar Autores
		for (TL = 0; letra != '|'; TL++)
		{
			fscanf(ptrTxt, "%[^;|]%c", &liv.autores[TL], &letra);
			quebraAutor(liv.autores[TL], liv.nome[TL], liv.sobrenome[TL]);
		}
		liv.tamAutores = TL;
		letra = ' ';
		// Pegar o Resto
		fscanf(ptrTxt, "%[^|]|%[^|]|%d|%d\n", &liv.titulo, &liv.editora, &liv.ano, &liv.pag);

		// Escrever
		fwrite(&liv, sizeof(tpLivroBinario), 1, ptrBin);
	}
	fclose(ptrTxt);
	fclose(ptrBin);
}

void quebraAutor(char autor[], char nome[], char sobrenome[])
{
	int i, j;

	for (i=0,j=0; autor[i]!=',' && autor[i]!='\0';i++,j++)
		sobrenome[j] = autor[i];
	sobrenome[j] = '\0';

	for (i+=2,j=0;autor[i]!='\0';i++,j++)
		nome[j] = autor[i];
	nome[j] = '\0';
}

void ExibirArqBin(void)
{
	clrscr();
	tpLivroBinario liv;
	FILE *Ptr = fopen("livros2.dat", "rb");

	int TL;
	if (Ptr == NULL)
		printf("\nErro de Abertura!\n");
	else
	{
		printf("\n\n\n");
	
			textcolor(2);
			printf("\n================================\n");
			printf("|| >>>  ARQUIVO BINARIO <<< ||"); 
			printf("\n================================\n");
			textcolor(15);
	
	printf("\n");
		fread(&liv, sizeof(tpLivroBinario), 1, Ptr);
		while (!feof(Ptr))
		{
			for (TL = 0; TL < liv.tamAutores; TL++)
			{
				printf("%s ", liv.autores[TL]);
			}
			printf("| %s | %s | %d | %d\n\n", liv.titulo, liv.editora, liv.ano, liv.pag);
			fread(&liv, sizeof(tpLivroBinario), 1, Ptr);
		}
		 
	}
	 fclose(Ptr);

	gotoxy(2,2);
 	textcolor(4);
	printf("[ESC]");
	textcolor(15);	 
	gotoxy(7,2);
 
	 
	getch();
}

//_______________________________________________________________________________________________________________________

//[Inicializar]:
void NovoDescr(tpDescritor &Descr)
{
	Descr.inicio = Descr.fim = NULL;
	Descr.qtde = 0;
}

tpAutor *NovoAutor(char nome[100], char sobrenome[100])
{
	tpAutor *caixa = new tpAutor;

	strcpy(caixa->nome, nome);
	strcpy(caixa->sobrenome, sobrenome);
	caixa->prox = NULL;
	return caixa;
}

pEditora *NovaEditora(char elem[100])
{
	pEditora *novo = new pEditora;
	novo->prox = NULL;
	novo->plivro = NULL;
	strcpy(novo->info, elem);

	return novo;
}

pLivro *NovoLivro(char titulo[100], int ano, int pag)
{
	pLivro *caixa = new pLivro;
	 
	strcpy(caixa->titulo, titulo);
	caixa->ano = ano;
	caixa->pag = pag;
 
	
	caixa->ant = NULL;
	caixa->prox = NULL;
	caixa->ListaAutores = NULL;

	return caixa;
}

pListaAutores *NovaListaAutor(void)
{
	pListaAutores *caixa = new pListaAutores;

	caixa->pAutor = NULL;
	caixa->prox = NULL;

	return caixa;
}

//_______________________________________________________________________________________________________________________

//[Inserir]:
tpAutor * InserirAutoresOrigin(tpAutor *AutoresOrigin){
	tpLivroBinario liv;
	FILE *Ptr = fopen("livros2.dat", "r");

	tpAutor *caixa;
	int TL;
	fread(&liv, sizeof(tpLivroBinario), 1, Ptr);
	while (!feof(Ptr))
	{
		for(TL=0;TL<liv.tamAutores;TL++){
			if (buscaAutor(AutoresOrigin, liv.nome[TL],liv.sobrenome[TL]) == NULL)
			{
				caixa=NovoAutor(liv.nome[TL],liv.sobrenome[TL]);
				
				if(AutoresOrigin == NULL)
					AutoresOrigin=caixa;
				else{
					caixa->prox=AutoresOrigin;
					AutoresOrigin=caixa;
				}
			}
		}
		
		fread(&liv, sizeof(tpLivroBinario), 1, Ptr);
	}
	fclose(Ptr);
	return AutoresOrigin;
}

tpAutor * InserirAutoresSemLivro(tpAutor *AutorSemLivro, char nome[100],char sobrenome[100]){
	tpAutor *caixa;
 
	//if (buscaAutor(AutorSemLivro, nome,sobrenome) == NULL)
	//{
		caixa=NovoAutor(nome,sobrenome);
		if(AutorSemLivro == NULL){
			AutorSemLivro=caixa;
			 
		}
			
		else{
			caixa->prox=AutorSemLivro;
			AutorSemLivro=caixa;
			 
		}
	//}
	
	return AutorSemLivro;
}

void InserirAutor(pListaAutores *ListaAutores, char nome[100], char sobrenome[100], tpAutor *AutoresOrigin)
{

	tpAutor *caixa = NovoAutor(nome, sobrenome);

	if (AutoresOrigin == NULL)
		AutoresOrigin = caixa;
	else
	{
		caixa->prox = AutoresOrigin;
		AutoresOrigin = caixa;
	}
	/*if(AutoresOrigin!=NULL){
		RelatorioLivrosAutor(AutoresOrigin);
	}*/
	ListaAutores->pAutor = AutoresOrigin;
}

void InserirEditoras(tpDescritor &D)
{
	tpLivroBinario liv;
	FILE *Ptr = fopen("livros2.dat", "r");

	pEditora *atual, *anterior, *caixa;

	fread(&liv, sizeof(tpLivroBinario), 1, Ptr);
	while (!feof(Ptr))
	{

		if (buscaEditora(D, liv.editora) == NULL)
		{
			caixa = NovaEditora(liv.editora);
			if (D.qtde == 0)
				D.inicio = D.fim = caixa;
			else if (strcoll(caixa->info, D.inicio->info) < 0)
			{
				caixa->prox = D.inicio;
				D.inicio = caixa;
			}
			else if (strcoll(caixa->info, D.fim->info) > 0)
			{
				D.fim->prox = caixa;
				D.fim = caixa;
			}
			else
			{
				anterior = D.inicio;
				atual = D.inicio->prox;
				while (strcoll(caixa->info, atual->info) > 0)
				{
					anterior = atual;
					atual = atual->prox;
				}
				caixa->prox = atual;
				anterior->prox = caixa;
			}
			D.qtde++;
		}
		fread(&liv, sizeof(tpLivroBinario), 1, Ptr);
	}
	fclose(Ptr);
}

void InserirLivros(tpDescritor D, tpAutor *AutoresOrigin)
{
	tpLivroBinario liv;
	FILE *Ptr = fopen("livros2.dat", "r");

	pLivro *caixa, *atual, *anterior = NULL, *livro;
	pEditora *Editora;

	fread(&liv, sizeof(tpLivroBinario), 1, Ptr);
	while (!feof(Ptr))
	{
		Editora = buscaEditora(D, liv.editora);
		if (Editora != NULL)
		{

			atual = Editora->plivro;
			caixa = NovoLivro(liv.titulo, liv.ano, liv.pag);
			 

			if (Editora->plivro == NULL)
			{
				Editora->plivro = caixa;
				livro = Editora->plivro;
			}
			else if (strcoll(caixa->titulo, Editora->plivro->titulo) < 0)
			{
				caixa->prox = Editora->plivro;
				Editora->plivro->ant = caixa;
				Editora->plivro = caixa;
				livro = Editora->plivro;
			}
			else
			{
				while (atual != NULL && strcoll(atual->titulo, caixa->titulo) < 0)
				{
					anterior = atual;
					atual = atual->prox;
				}

				if (atual != NULL)
				{
					caixa->prox = atual;
					caixa->ant = anterior;
					atual->ant = caixa;
					anterior->prox = caixa;
				}
				else
				{
					caixa->ant = anterior;
					anterior->prox = caixa;
				}
				livro = anterior->prox;
			}
			InserirListaAutor(livro, liv.nome, liv.sobrenome, liv.tamAutores, AutoresOrigin);

			 
			
		}
		fread(&liv, sizeof(tpLivroBinario), 1, Ptr);
	}
	fclose(Ptr);
}

void InserirListaAutor(pLivro *livro, char nome[5][100], char sobrenome[5][100], int tamAutores, tpAutor *AutoresOrigin)
{
	tpAutor *Autores;

	pListaAutores *caixa;

	for (int i = 0; i < tamAutores; i++)
	{
		caixa = NovaListaAutor();
		if (livro->ListaAutores == NULL)
			livro->ListaAutores = caixa;
		else
		{
			caixa->prox = livro->ListaAutores;
			livro->ListaAutores = caixa;
		}
		InserirAutor(livro->ListaAutores, nome[i], sobrenome[i], AutoresOrigin);
	}
	
}

//_______________________________________________________________________________________________________________________
void ExcluirArqBin(char titulo[100]) {
    tpLivroBinario L;
    char aux[100];
    tpDescritor D;
 
 

    FILE *ptrBin = fopen("livros2.dat", "rb");
    if (ptrBin == NULL) {
        perror("Erro ao abrir o arquivo original");
    }
	else
	{
		FILE *ptrBinTemp = fopen("temp.dat","wb");
	    
	    fread(&L, sizeof(tpLivroBinario), 1, ptrBin);
	    while (!feof(ptrBin)) {
	        if (stricmp(L.titulo, titulo) != 0) {
	            fwrite(&L, sizeof(tpLivroBinario), 1, ptrBinTemp);
		      
	        }
	    	fread(&L, sizeof(tpLivroBinario), 1, ptrBin);
	    }
		fclose(ptrBin);
	    fclose(ptrBinTemp);
	    
		remove("livros2.dat");
		rename("temp.dat", "livros2.dat");
	}
	   
}
//[Exclusoes]:
void ExcluirLivrosDin(tpDescritor D,char titulo[100])
{
	pEditora *Editora=D.inicio;
	pLivro *livro = Editora->plivro, *ant = Editora->plivro;

	while(Editora != NULL){
		livro=Editora->plivro;
		while (livro != NULL)
		{
			if(strcoll(titulo,livro->titulo)==0){
				if(livro->ant==NULL && livro->prox==NULL){
					Editora->plivro=NULL;
					delete(livro);
				}
				else{
					if(livro->ant!=NULL){
						livro->ant->prox=NULL;
						delete(livro);
					}
					else{
						if(livro->prox!=NULL){
							Editora->plivro=livro->prox;
							livro->prox->ant=NULL;
							delete(livro);
						}
					}
				}
			}
			ant=livro;
			livro = livro->prox;
		}
		Editora=Editora->prox;
	}
	//BINARIO
	ExcluirArqBin(titulo);
	
	//----------------
	gotoxy(2,2);
 	textcolor(4);
	printf("[ESC]");
	textcolor(15);	 
	gotoxy(7,2);
 
	getch();
}
 

//_______________________________________________________________________________________________________________________

//[Relatorios]:
void  RelatorioAutoresLivros(tpAutor *AutoresOrigin, tpDescritor D ){

//Moldura2(1, 19, 95, 50, 0, 0);
clrscr();

pEditora *Editora;
	pLivro *livro;
	pListaAutores * ListaAutores;
	tpAutor * SemLivro;
	
	int cont;
	printf("\n\n\n");
	
	textcolor(2);
	printf("\n===============================\n");
	printf("|| >>>  LIVROS POR AUTOR <<< ||"); 
	printf("\n===============================\n");
	textcolor(15);
	
	printf("\n");
	while(AutoresOrigin !=NULL){
		//printf("\n%s | %s  | [&] %u\N", AutoresOrigin->nome, AutoresOrigin->sobrenome, &AutoresOrigin);
 		cont =0;
		Editora = D.inicio;
		while (Editora != NULL){
			//printf(" ### [%s]\n", Editora->info);
			
 			livro=Editora->plivro;
			while (livro != NULL)
			{


				ListaAutores = livro->ListaAutores;
 				while (ListaAutores != NULL)	{
					//printf("       |> Autor: [%s - %s]\n", ListaAutores->pAutor->nome, ListaAutores->pAutor->sobrenome );
		
					if(strcoll(ListaAutores->pAutor->nome,AutoresOrigin->nome)==0){
						
						
						if(cont>0){
							textcolor(15);
							textbackground(0);
							textcolor(2);
							printf("\n #Livro [%s]\n",Editora->plivro->titulo);
							textcolor(15);
							printf("\n   @Editora [%s]\n",Editora->info);
						}
						else{
							 
							textcolor(5);
 
							printf("\n==========================\n");
							printf("|| >>> %s %s \n",AutoresOrigin->nome,AutoresOrigin->sobrenome);
							 
							printf("==========================\n");
							textcolor(15);
							textbackground(0);
							textcolor(2);
							printf("\n #Livro [%s]\n",Editora->plivro->titulo);
							textcolor(15);
							printf("\n   @Editora [%s]\n",Editora->info);
							cont++;
						}
							
					} 
				
					ListaAutores = ListaAutores->prox; 
				}	
			 
		

				livro = livro->prox; // L NAO MUDA
			}
			//printf("\n");
			Editora = Editora->prox;
		}
		
		AutoresOrigin = AutoresOrigin -> prox; //L NAO MUDA
	}
	 
	gotoxy(2,2);
 	textcolor(4);
	printf("[ESC]");
	textcolor(15);	 
	gotoxy(7,2);
 
	getch();
 
}

void RelatorioAutoresSemLivro(tpAutor *AutoresOrigin, tpDescritor D ){
	clrscr();
	
	pEditora *Editora;
	pLivro *livro;
	pListaAutores * ListaAutores;
	tpAutor * SemLivro;
	bool semAutor= true;
	
	printf("\n\n\n");
	
	textcolor(2);
	printf("  ================================\n");
	printf("  || >>>  AUTORES SEM LIVRO <<< ||"); 
	printf("\n  ================================\n");
	textcolor(15);
	
	printf("\n");
	textcolor(15);
	while(AutoresOrigin !=NULL){ 
		Editora = D.inicio;
		while (Editora != NULL){
 
 			livro=Editora->plivro;
			while (livro != NULL)
			{
 
				ListaAutores = livro->ListaAutores;
 				while (ListaAutores != NULL)	{
 
					if(strcoll(ListaAutores->pAutor->nome,AutoresOrigin->nome)==0){
						semAutor=false;					
					}

					ListaAutores = ListaAutores->prox; 
				}	
 
				livro = livro->prox;  
			}
 
			Editora = Editora->prox;
		}
 
		if(semAutor == true){
 
 			 printf(" |> Autor: [%s - %s]\n", AutoresOrigin->nome, AutoresOrigin->sobrenome );
		}
		semAutor= true;
 
		AutoresOrigin = AutoresOrigin -> prox; //L NAO MUDA
	} 
	
	gotoxy(2,2);
 	textcolor(4);
	printf("[ESC]");
	textcolor(15);	 
	gotoxy(7,2);
 
	getch();
}

void RelatorioLivrosEditoras(tpDescritor D)
{
	clrscr();
	gotoxy(2,2);
 	textcolor(4);
	printf("[ESC]");
	textcolor(15);
	
 	pEditora *Editora = D.inicio;
 
	printf("\n\n\n");
	
	textcolor(2);
	printf("\n=================================\n");
	printf("|| >>>  LIVROS POR EDITORA <<< ||"); 
	printf("\n=================================\n");
	textcolor(15);
	
	printf("\n");
	while (Editora != NULL)
	{
		textbackground(2);
		textcolor(0);
 
		printf("\n=====================\n");
		printf("|| @@@ EDITORA @@@ ||");
		textcolor(2);
		textbackground(0);
		printf(" --->>> [%s]\n", Editora->info);
			textbackground(2);
		textcolor(0);
		printf("=====================\n");
		textcolor(15);
		textbackground(0);
		ExibirLivros(Editora);

		Editora = Editora->prox; // L NAO MUDA
		
		textcolor(8);
		printf("____________________________________________________________________________________________________\n");
		textcolor(15);
	}
	
	gotoxy(2,2);
 	textcolor(4);
	printf("[ESC]");
	textcolor(15);	 
	gotoxy(7,2);
 
	getch();
}

//_______________________________________________________________________________________________________________________

//[Buscas]:
tpAutor *buscaAutor(tpAutor *AutoresOrigin, char nome[100], char sobrenome[100])
{
	tpAutor *atual = AutoresOrigin;

	while (atual != NULL && (strcoll(nome, atual->nome) != 0 && strcoll(sobrenome, atual->nome) != 0))
		atual = atual->prox;

	return atual; // Se NULL nÃ£o tem
}

pEditora *buscaEditora(tpDescritor D, char editora[100])
{
	pEditora *atual = D.inicio, *ant;

	while (atual != NULL && strcoll(editora, atual->info) != 0)
		atual = atual->prox;

	return atual; // Se NULL nÃ£o tem
}

//_______________________________________________________________________________________________________________________

//[Exibir]
void ExibirAutores(pListaAutores *ListaAutores)
{
	pListaAutores * Autores = ListaAutores;
 	while (Autores != NULL)
	{
 
		textcolor(2);
		printf("  |> Autor: [%s - %s]\n", Autores->pAutor->nome, Autores->pAutor->sobrenome );
		Autores = Autores->prox; // L NAO MUDA
 
		textcolor(15);
	}
}

void ExibirLivros(pEditora *Editora)
{
	
	pLivro *livro = Editora->plivro;
	//printf("\n");
	while (livro != NULL)
	{
		textcolor(6);
		printf("\n  ----------------\n");
		printf("  | ## LIVRO: ## | --> %s", livro->titulo );
		printf("\n  ----------------\n");
		textcolor(15);
 
		printf("  |ANO: [%d]   |\n" ,livro->ano );
		printf("  ----------------\n");
		printf("  |PAGINAS: [%d]|\n", livro->pag);
		printf("  ----------------\n");
		
		ExibirAutores(livro->ListaAutores);
		

		livro = livro->prox; // L NAO MUDA
	}
	printf("\n");
}

//_______________________________________________________________________________________________________________________

//[Menus]
char MenuPrincipal (void)
{
	Moldura2(1, 1, 95, 60, 0, 0);
	clrscr();
	Moldura(1, 1, 52, 25, 0,2);
	Moldura(2, 2, 51, 6, 2, 0);
	
	textbackground(2);
	textcolor(0);
	gotoxy(14,4);
	printf("*** SISTEMA DE BIBLIOTECA ***");
	textbackground(0);
	textcolor(15);
	
	gotoxy(4,8);
	printf("[A] Relatorio de Livros [Por Editora]");
	gotoxy(4,10);
	printf("[B] Relatorio de Livros [Por Autor]");
	gotoxy(4,12);
	printf("[C] Exclusao de Livros [Dinamica/Arq. binario]");
	gotoxy(4,14);
	printf("[D] Relatorio de Autores sem livro");
	gotoxy(4,16);
	printf("[E] Exibir Arquivo Binario");
	textbackground(2);
	textcolor(0);
 	gotoxy(4,18);
 	printf("|============================================|");
 	textbackground(0);
	textcolor(15);
 	gotoxy(2,2);
 	textcolor(4);
	printf("[ESC]");
	textcolor(15);
	gotoxy(4,20);
	printf("@ OPCAO: ");
	textbackground(0);
	textcolor(2);
 	gotoxy(2,21);
 	printf("__________________________________________________");
 	textbackground(0);
	textcolor(15);
	
	textcolor(2);
	gotoxy(4,23);
 	printf("|MENSAGEM:");
 	textcolor(15);
 	gotoxy(13,20);
 	textcolor(15);
	return toupper(getche());
	 
}

 

//_______________________________________________________________________________________________________________________

//[EXECUTAR]
int main()
{
	//
	setlocale(LC_ALL, "");
	//CriarArquivoBin();

	//
	tpDescritor D; 	NovoDescr(D);
	tpAutor *AutoresOrigin = NULL;


	TxtPraBin();
	
	AutoresOrigin=InserirAutoresOrigin(AutoresOrigin);
	 
 
	InserirEditoras(D); 
	InserirLivros(D, AutoresOrigin);
	
	

	char titulo[100];

	//----------------------------------------------------------------------
	//Menu(D);
	
	int cont=0;
 
	  
	char op; 
	
	do
	{
		op = MenuPrincipal();
		Moldura2(1, 1, 100, 170, 0, 0);
		switch(op)
		{
			case 'A': RelatorioLivrosEditoras(D);  
					  break;
					  
			case 'B': RelatorioAutoresLivros(AutoresOrigin,D);
					  break;
					  
			case 'C': 
			
						
						
						if(cont==0){
							 fflush(stdin);
							strcpy(titulo,"Introducao a estruturas de dados: com tecnicas de programacao em C");
						
							clrscr();
	
							textcolor(2);
							printf("\n\n\n========================================\n");
							printf("|| >>>  EXCLUSAO DINAMICA/BINARIA <<< ||"); 
							printf("\n========================================\n");
							textcolor(15);
	
							printf("\n\n");
							textcolor(4);printf("[TESTE 1] Já foi excluido:\n");textcolor(15);
							printf("Introdução a estruturas de dados: com técnicas de programação em C [Elsevier]\n");
							printf("\n--------------------------------------------------------------------------------------------------\n");
						
							ExcluirLivrosDin(D,titulo);
							cont++;
						}
						
						
						clrscr();
	
						textcolor(2);
						printf("\n\n\n========================================\n");
						printf("|| >>>  EXCLUSAO DINAMICA/BINARIA <<< ||"); 
						printf("\n========================================\n");
						textcolor(15);
	
						printf("\n\n");
						
						textcolor(4);printf("[TESTE 2] Digite um livro:\n");textcolor(15);
						fflush(stdin);
						gets(titulo);
						 
						ExcluirLivrosDin(D,titulo);
					  break;
			case 'D': RelatorioAutoresSemLivro(AutoresOrigin,D); 
					  break;
			case 'E': ExibirArqBin();
						//clrscr();
					  break;
		}
	}while(op!=27);  
	
	/*fflush(stdin);
	strcpy(titulo,"Projeto de algoritmos: com implementacoes em Pascal e C");
	ExcluirArqBin(titulo);
	ExibirArqBin();*/
	
	//RelatorioLivrosEditoras(D);
	//
	
 	//RelatorioAutoresLivros(AutoresOrigin,D);
	
 	//RelatorioAutoresSemLivro(AutoresOrigin,D);
	/*
	fflush(stdin);
	strcpy(titulo,"Projeto de algoritmos: com implementações em Pascal e C");
	ExcluirLivrosDin(D,titulo);
	
	fflush(stdin);
	strcpy(titulo,"Estruturas de dados");
	ExcluirLivrosDin(D,titulo);
	
	fflush(stdin);
	strcpy(titulo,"Técnicas de programação: uma abordagem estruturada");
	ExcluirLivrosDin(D,titulo);
	
	fflush(stdin);
	strcpy(titulo,"Estruturas de dados fundamentais: conceitos e aplicações");
	ExcluirLivrosDin(D,titulo);
	
	RelatorioLivrosEditoras(D);  */
	//ExibirArqBin();
	
	
	/*gotoxy(51,3);
	getch();*/
	
	return 0;
	
}

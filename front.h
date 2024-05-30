 

void Moldura(int CI, int LI, int CF, int LF, int CorT, int CorF) {
    int i;

    textcolor(CorT);
    textbackground(CorF);
    gotoxy(CI, LI);
    printf("+" );
    gotoxy(CI, LF);
    printf("+" );
    gotoxy(CF, LI);
    printf("+" );
    gotoxy(CF, LF);
    printf("+" );

    for (i = CI + 1; i < CF; i++) {
        gotoxy(i, LI); printf("-");
        gotoxy(i, LF); printf("-");
    }

    for (i = LI + 1; i < LF; i++) {
        gotoxy(CI, i); printf("|" );
        gotoxy(CF, i); printf("|" );
    }

    textcolor(7);
    textbackground(0);
}
void Moldura2(int CI, int LI, int CF, int LF, int CorT, int CorF)
{	textbackground(0);

	for(int i=LI; i<LF; i++)
	{
 		for(int j=CI; j<CF; j++)
 		{
 			 gotoxy(j,i); printf(" ");
 		}
			
	}
	//Cores
	textcolor(0); textbackground(0);
	
	//Cantos
	gotoxy(CI,LI); printf(" ");
	gotoxy(CI,LF); printf(" ");
	gotoxy(CF,LI); printf(" ");
	gotoxy(CF,LF); printf(" ");
	
	//Contorno
	for(int i=CI+1; i<CF; i++)
	{
		gotoxy(i,LI); printf(" ");
		gotoxy(i,LF); printf(" ");
	}
	for(int i=LI+1; i<LF; i++)
	{
		gotoxy(CI,i); printf(" ");
		gotoxy(CF,i); printf(" ");
	}

	 
	textcolor(0); textbackground(0);
}
void Formulario(void) {
    system("cls");

    Moldura(1, 1, 80, 25, 10, 5);
    gotoxy(27, 3);
    textcolor(15);
    printf(" # # # T I T U L O # # #");

    Moldura(2, 2, 79, 4, 14, 1);
    Moldura(2, 5, 27, 21, 12, 7);
    // MenuPrincipal();
    Moldura(2, 22, 79, 24, 11, 4);
    gotoxy(4, 23);
    textcolor(10);
    printf("Mensagem: ");

    Moldura(28, 5, 79, 21, 9, 6);
}

 

 
 




/*   
#########################################################
# Universidade de Brasilia                              #
# Instituto de Ciencias Exatas                          #
# Departamento de Ciencia da Computacao                 #
# Algoritmos e Programação de Computadores – 1/2018     #
# Aluno(a): Pedro Luis Chaves Rocha                     #
# Matricula: 180054635                                  #
# Turma: A                                              #
# Versão do compilador: 5.4.0                           #
# Descricao: Baseado no jogo River Raid do Atari        #     
#########################################################
*/

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

	
/* Definicoes */

#ifdef _WIN32		

    #define TECLA_ENTER 13
	#define CLEAR "cls"
	#include <conio.h>

#else									

    #define TECLA_ENTER 10
	#define CLEAR "clear"
	#include<termios.h>


	int kbhit(){
	    struct termios oldt, newt;
	    int ch, oldf;
	    tcgetattr(STDIN_FILENO,&oldt);
	    newt = oldt;
	    newt.c_lflag &= ~(ICANON | ECHO);
	    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	    ch = getchar();
	    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	    fcntl(STDIN_FILENO, F_SETFL, oldf);
	    if(ch != EOF){
	        ungetc(ch,stdin);
	        return 1;
	    }
	return 0;
	}


	int getch(void) {
	    int ch;
	    struct termios oldt;
	    struct termios newt;
	    tcgetattr(STDIN_FILENO,&oldt);
	    newt = oldt;
	    newt.c_lflag &= ~(ICANON | ECHO);
	    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	    ch = getchar();
	    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	    return ch;
	}


#endif

/* Macros */


/* COR */
#define COR_VAZIO 110               
#define COR_PAREDE 43
#define COR_PERSONAGEM 96
#define COR_COMBUSTIVEL 92
#define COR_BOMBA 91
#define COR_DISPARO 95
#define COR_SNIPER 96
#define COR_TANK 93

/* Settings */
#define GANHO_DE_PONTOS_POR_KIL 50
#define GANHO_DE_PONTOS_POR_DIST 0
#define PERDA_DE_COMBUSTIVEL 1
#define GANHO_DE_COMBUSTIVEL 40
#define COMBUSTIVEL_INICIAL 400


#define CHANCE_DO_TIRO_INIMIGO 10


/* struct NPCS */

	typedef struct{

		char npc;

		int tiros;
		int vidas;

	}NPC;

/* struct RANKING*/
	typedef struct {

    int pontuacao;		
    char nome[12];
        
	}ranking;


/* Variaveis Globais */



int probX = 30;					
int probF = 30;
int probT = 4;
int probO = 1;
int tirosT = 5;
int vidasO = 10;
int velocidade = 60;
int LINHAS =12;
int COLUNAS =135;

int nivel=1;

int usleep();


/* Troca 2 char de lugar para mover os objetos */
void troca(NPC *x,NPC *y){   
    NPC aux;
    aux=*x;         
    *x=*y;
    *y=aux;
}


void cor(int cor){ 
    
    printf("\033[%dm",cor);

          /* Cores de Letra */

    /*  1 negrito = '\033[1m'    */
    /*  2 reverso = '\033[2m'    */
    /*  30 preto = '\033[30m'    */
    /*  31 vermelho = '\033[31m' */
    /*   32 verde = '\033[32m'   */
    /*   33 amarelo = '\033[33m' */
    /*   34 azul = '\033[34m'    */
    /*   35 magenta = '\033[35m' */
    /*   36 ciano = '\033[36m'   */
    /*   37 branco = '\033[37m'  */

    /*USAR O CODIGO COM 90 FICA MAIS CLARA A COR ex: 91 = vermelho mais claro */

    /* Cores de Fundo */
    /*   40 fundo preto = '\033[40m'    */
    /*   41 fundo vermelho = '\033[41m' */
    /*   42 fundo verde = '\033[42m'    */
    /*   43 fundo amarelo = '\033[43m'  */
    /*   44 fundo azul = '\033[44m'     */
    /*   45 fundo magenta = '\033[45m'  */
    /*   46 fundo ciano = '\033[46m'    */
    /*   47 fundo branco = '\033[47m'   */

    /*USAR O CODIGO COM 100 FICA MAIS CLARA A COR ex: 101 = vermelho mais claro */


}
void voltaCor(){
    printf("\033[0m");
}


void printMatriz(); /* Faco referencia a estas funcoes aqui para utilizalas nas intrucoes() e no game over() */
void printPlacar(); /* para nao dar warning na hora de compilar */


/* Imprime na tela as instrucoes */
void instrucoes(NPC mapa[LINHAS][COLUNAS]){    

    system(CLEAR);
    int a = COMBUSTIVEL_INICIAL;;
    int b = 0;

    printPlacar(&a,&b);

    printMatriz(mapa,1);

    printf("\nInstrucoes :\n\n");

    printf("Aperte 'W' e 'S' para mover o personagem e 'espaco' para atirar.\n");
    printf("Cuidado para n esbarrar em um 'X' ou acabar com o combustivel, pois ambos encerram o jogo,\n");
    printf("Colete os 'F' para ganhar conbustivel e destrua os 'X' para ganhar pontos,\n");
    printf("Caso queira pausar o jogo aperte 'q'\n");
    printf("Precione qualquer tecla para continua.");

    getch();
}

/* Printa tela de Bem vindo */
void printBemVindo(){

    system(CLEAR);

    cor(92);
    printf("\n");


    printf("  ____                  __      __            _       \n");
    printf(" |  _ \\                 \\ \\    / (_)         | |      \n");
    printf(" | |_) | ___ _ __ ___    \\ \\  / / _ _ __   __| | ___  \n");
    printf(" |  _ < / _ \\ '_ ` _ \\    \\ \\/ / | | '_ \\ / _` |/ _ \\ \n");
    printf(" | |_) |  __/ | | | | |    \\  /  | | | | | (_| | (_) |\n");
    printf(" |____/ \\___|_| |_| |_|     \\/   |_|_| |_|\\__,_|\\___/ \n");
       

    printf("\n");                                                                                              
    voltaCor();

    printf("Para navegar nos menus utilize 'w', 's' ou precione os respectivos numeros \ne aperte 'enter' para selecionar a opcao\n\n");
    printf("Precione qualquer tecla para continuar...\n");

    getch();

}



/*
#####################################################
#  Funcao que adiministra o fim de jogo mostrando   #
# mensagem de game over e gerencia o                #
# pause e o quit                                    #
#####################################################
*/

void printGameOver(int *pontos, int *combustivel,int *quit, NPC mapa[LINHAS][COLUNAS],int *gameOver){

    if(*quit==1){    /* Caso o jogo seja pausado e a pesoa deseje ou continuar sair */
 
        system(CLEAR);

        char teclaQuit;

        printPlacar(combustivel,pontos);
        printMatriz(mapa,0);

        cor(93);
        printf("\nJogo Pausado. Precione a tecla 'q' novamente para sair ou\nprecione qualquer outra tecla para continuar...\n\n");
        voltaCor();
        teclaQuit=getch();

        voltaCor();


        if(teclaQuit=='q' || teclaQuit == 'Q'){

            *gameOver = 1; /* acaba com o jogo */
            system(CLEAR);
            printPlacar(combustivel,pontos);
            printMatriz(mapa,0);
            printf("\n");

        } 

        else{

            *gameOver=0; /* continua o jogo */
            *quit=0;
        }   
            
    }



	if(*quit==0 && *gameOver==1){ /* Quando a pessoa realmente morre por perda de gasolina ou por colisao com inimigos */

        system(CLEAR);
        
        cor(91);
        printf("\n  ######       ###     ##     ##  ######## \n");
        printf(" ##    ##     ## ##    ###   ###  ##       \n");
        printf(" ##          ##   ##   #### ####  ##       \n");
        printf(" ##   ####  ##     ##  ## ### ##  ######   \n");
        printf(" ##    ##   #########  ##  #  ##  ##       \n");
        printf(" ##    ##   ##     ##  ##     ##  ##       \n");
        printf("  ######    ##     ##  ##     ##  ######## \n");
        printf("                                           \n");
        printf("  #######   ##     ##  ########  ########  \n");
        printf(" ##     ##  ##     ##  ##        ##     ## \n");
        printf(" ##     ##  ##     ##  ##        ##     ## \n");
        printf(" ##     ##  ##     ##  ######    ########  \n");
        printf(" ##     ##   ##   ##   ##        ##   ##   \n");
        printf(" ##     ##    ## ##    ##        ##    ##  \n");
        printf("  #######      ###     ########  ##     ## \n");
        printf("                                           \n\n");
        voltaCor();  

        if (*combustivel<=0){
            cor(35);
            printf("Seu Combustivel Acabou !!!\n\n");
            voltaCor();
        }
        else {
            cor(35);
            printf("Voce atingiu o inimigo !!!\n\n");
            voltaCor();
        }
        printf("Pontuacao = %d\n\n",*pontos);

        printf("Precione qualquer tecla para continuar...\n");
    	getch();

    }


}


/* Inicia todos os valores da matriz para que esla seja startada corretamente */
void zeraMatriz(NPC mapa[LINHAS][COLUNAS]){	

    int i,j;

    for(i=0;i<LINHAS;i++){

        for(j=0;j<COLUNAS;j++){

            if(i==0 || i==LINHAS-1) mapa[i][j].npc='#';

            else mapa[i][j].npc=' ';

        }
    }
    mapa[(LINHAS/2)-2][COLUNAS+1].npc='+';
}


/*
#################################################
#  Imprime o placar com os pontos gasolina e    #
# talvez as vidas, antes da matriz geralmente   #
#################################################

*/
void printPlacar(int *fuel,int *pontos){   
    int j;
    for(j=0;j<COLUNAS;j++){
        printf("-");
    }

        printf("\nCombustivel = %d |||| Pontos = %d\n",*fuel,*pontos);
        for(j=0;j<COLUNAS;j++){
            printf("-");
        }
    printf("\n");
}


/* 
#############################################################
#  Percorre e imprime a matriz na tela permitindo trocar    #
# facilmente os caracteres e as cores                       #
#############################################################
*/


void printMatriz(NPC mapa[LINHAS][COLUNAS],int vazio){	
    int i,j;
    for(i=0;i<LINHAS;i++){
        for(j=0;j<COLUNAS;j++){



            if(mapa[i][j].npc=='#') {
                cor(COR_PAREDE);
                printf("#");
                voltaCor();
            }    
            if(mapa[i][j].npc==' '){
                cor(COR_VAZIO);
                printf(" ");
                voltaCor();
            }
            if(mapa[i][j].npc=='+'){
                cor(COR_PERSONAGEM);
                printf("+");
                voltaCor();
            }

            if(vazio==0){  /* Flag para printar a matriz limpa ou com os objetos */
                if(mapa[i][j].npc=='F'){
                    cor(COR_COMBUSTIVEL);
                    printf("F");
                    voltaCor();
                }
                if(mapa[i][j].npc=='X'){
                    cor(COR_BOMBA);
                    printf("X");
                    voltaCor();
                }
                if(mapa[i][j].npc=='>'){
                    cor(COR_DISPARO);
                    printf(">");
                    voltaCor();
                }
                if(mapa[i][j].npc=='O'){
                    cor(COR_TANK);
                    printf("O");
                    voltaCor();
                }
                if(mapa[i][j].npc=='T'){
                    cor(COR_SNIPER);
                    printf("T");
                    voltaCor();
                }

                if(mapa[i][j].npc=='<'){
                    cor(COR_DISPARO);
                    printf("<");
                    voltaCor();
                }

                
            }
        }
        printf("\n");
    }
}

/* 
################################################################
#  Coleta os dados do teclado e modifica a variavel personagem #
# na main para movimenta o personagem verticalmente, tambem    #
# indica quando o jogador esta atirando ou apertou o botao de  #
# sair                                                         #
################################################################
*/


void atualizaTeclado(int *personagem,int *flagTiro,int *gameOver,int *quit){  
    char local;
    *flagTiro=0;

    if(kbhit()==1){
        local = getch();


        /* Move para cime, para baixo e atira */
        switch (local){
            case 'W':  
            case 'w':
                if(*personagem>1) *personagem=*personagem-1;
            break;
            case 'S':
            case 's':
                if(*personagem<LINHAS-2) *personagem=*personagem+1;
            break;
            case 32:
                *flagTiro=1;
            break;
            case 'Q':      /* tecla de pause e quit*/
            case 'q':
                *gameOver=1;
                *quit=1;
            break;

        }
    }

}

/* 
###############################################################
# Gera um vetor de tamanho LINHAS-2 (para excluir as bordas)  #
# com elementos de bomba e combustivel aleatorios nele de,    #
# acordo com as variaveis globais probF e probX               #						 
###############################################################
*/

void geraObjetos(NPC objetos[]){           /* LINHA -1 EX: [11]={0,0,0,X,0,0,Y,0,0,0,0,\0}  */
    int i;

    int flag=0;
    int randPosition1; /* posicao no vetor objetos do combustive aleatoria */
    int randPosition2; /*posicao no vetor das bombas aleatorio */
    int randPosition3;
    int randPosition4;

    int randT;
    int randO;
    int randX; /*aonde a probabilidade da probX é checada */
    int randF; /* aonde a probabilidade da probF é checada */


     for(i=0;i<LINHAS-1;i++){ /* zera o vetor de objetos */

        objetos[i].npc=' ';
        

    }

    /* probabilidades */

    randX=rand()%100;

     if(randX < probX && flag==0){

        randPosition2=rand() % (LINHAS-1);
        objetos[randPosition2].npc='X'; 
        flag=1;  
    }



    randF=rand()%100;


    if(randF < probF && flag==0) {

        randPosition1=rand() % (LINHAS-1);
        objetos[randPosition1].npc='F';
        
    }

    
    randO=rand()%100;

    if(randO < probO && flag==0){

        randPosition3=rand() % (LINHAS-1);
        objetos[randPosition3].npc='O'; 
       
    }


    randT=rand()%100;

    if(randT < probT && flag==0){

        randPosition4=rand() % (LINHAS-1);
        objetos[randPosition4].npc='T'; 
        
    }

   



}




/* 
##########################################################
#   Atualiza toda a matriz movendo os objetos, o         #
# personagem, os disparos e contabilizando os pontos e o #
# combustivel por meio da funcao criada troca(); utilizei#
# 2 loops de troca para mover os tiros para frente e os  #
# itens para traz                                        #
##########################################################
*/


void atualizaMatriz(int *personagem,int *flagTiro,NPC objetos[],int *pontos,int *combustivel,int *gameOver,NPC mapa[LINHAS][COLUNAS]){
    					

    /* Move o personagem de acordo com a variavel personagem passado pelo ponteiro */

	if(mapa[*personagem+1][1].npc=='+') {
		troca(&mapa[*personagem][1],&mapa[*personagem+1][1]);
		*combustivel=*combustivel-1;   
    }
    if(mapa[*personagem-1][1].npc=='+') {
    	troca(&mapa[*personagem][1],&mapa[*personagem-1][1]);																											
 		*combustivel=*combustivel-1;   
    }	

    if(*flagTiro==1 && (mapa[*personagem][2].npc!='F' && mapa[*personagem][2].npc!='X')) {

    	/* Aonde realmente sai os disparos */
    	mapa[*personagem][2].npc = '>';
    	*combustivel=*combustivel-2;								

    }

    int i,j;

    /* percorre a matriz nesse sentido para mover os tiros e adicionar os primeiros objetos
											  | 2    |	1
            								 \|/    \|/	                                 */

    for(j=COLUNAS-1;j>=0;j--){
        for(i=1;i<LINHAS-1;i++){

            if(mapa[i][j].npc=='>' && j==COLUNAS-1) mapa[i][j].npc = ' ';/* Elimina os ultimos tiros antes de fazer a torca */

            if(mapa[i][j].npc=='>' && mapa[i][j+1].npc==' ') troca(&mapa[i][j],&mapa[i][j+1]);  /* move tiro */


            if(mapa[i][j].npc=='T' && i == *personagem) { /* spawna o tiro dos inimigos */

            	int randTiro;

            	randTiro=rand() % 100;

            	if(randTiro < CHANCE_DO_TIRO_INIMIGO && mapa[i][j].tiros>0){

            		mapa[i][j-1].npc='<';
            		mapa[i][j].tiros--;
            	}
            }

            if(j==COLUNAS-1) mapa[i][j]=objetos[i];

            if(j==COLUNAS-1 && mapa[i][j].npc=='O') mapa[i][j].vidas=vidasO; /* adiciona os contadores de vidas e tiros na struct*/

            if(j==COLUNAS-1 && mapa[i][j].npc=='T') mapa[i][j].tiros=tirosT;



        }
    }

    /* percorre a matriz nesse sentido para mover os objetos e detectar as colisoes
											-->	 1 		
     						     			-->	 2 								*/



    for(i=1;i<LINHAS-1;i++){															
        for(j=0;j<COLUNAS;j++){

            if(mapa[*personagem][2].npc=='F'){ /* Colisoes com F*/
                *combustivel=*combustivel+GANHO_DE_COMBUSTIVEL;
                mapa[*personagem][2].npc=' ';
            }
            if(mapa[*personagem][2].npc=='X'){/* Colisoes com X*/
            	*gameOver=1;
                mapa[*personagem][2].npc=' ';
            }
            if(mapa[*personagem][2].npc=='O'){/* Colisoes com X*/
            	*gameOver=1;
                mapa[*personagem][2].npc=' ';
            }
            if(mapa[*personagem][2].npc=='T'){/* Colisoes com X*/
            	*gameOver=1;
                mapa[*personagem][2].npc=' ';
            }

            if(mapa[*personagem][2].npc=='<'){/* Colisoes com X*/
            	*gameOver=1;
                mapa[*personagem][2].npc=' ';
            }
            if(mapa[*personagem][3].npc=='<'){/* Colisoes com X*/
            	*gameOver=1;
                mapa[*personagem][3].npc=' ';
            }
            if(mapa[*personagem][4].npc=='<'){/* Colisoes com X*/
            	*gameOver=1;
                mapa[*personagem][4].npc=' ';
            }
            
            int flagDestruiu=0;

            if(mapa[i][j].npc=='>' && (mapa[i][j+1].npc=='F' || mapa[i][j+1].npc=='X' || mapa[i][j+1].npc=='O' || mapa[i][j+1].npc=='T' || mapa[i][j+1].npc=='<' || mapa[i][j+2].npc=='<' || mapa[i][j+3].npc=='<')) { /* Colisoed do > com F e X */

                if(mapa[i][j+1].npc=='X') {   /* Colisoes tiros e bombas*/


                	mapa[i][j+1].npc=' ';
                	*pontos=*pontos + GANHO_DE_PONTOS_POR_KIL;
                }

                if(mapa[i][j+1].npc=='F') { /* Colisoes com tiros e fules*/

                	mapa[i][j+1].npc=' ';

                }
                if(mapa[i][j+1].npc=='O'){ /* Colisoes com tiros e tanques*/

					mapa[i][j+1].vidas--;                		

					if(mapa[i][j+1].vidas<=0){

					int l,m;
					for(l=0;l<LINHAS-1;l++)
						for(m=0;m<COLUNAS;m++){

							if(mapa[l][m].npc=='X') {

								mapa[l][m].npc=' ';
								*pontos=*pontos + 10 ; /* para cada x destrudo pelo O ganha 10 pontos*/

							}


						}

					mapa[i][j+1].npc=' ';
					

					}

                }

                if(mapa[i][j+1].npc=='T'){ /* Colisoes com tiros e Snipers */

                	mapa[i][j+1].npc=' ';
                	*pontos=*pontos + 10 ;

                }

                if(mapa[i][j+1].npc == '<' && flagDestruiu ==0){ /* colisoes tiro com tiro */


                	mapa[i][j+1].npc=' ';
                	flagDestruiu =1;

                }

                if(mapa[i][j+2].npc == '<'&& flagDestruiu ==0){ /* colisoes tiro com tiro */


                	mapa[i][j+2].npc=' ';
                	flagDestruiu =1;

                }

                if(mapa[i][j+3].npc == '<'&& flagDestruiu ==0){ /* colisoes tiro com tiro */


                	mapa[i][j+3].npc=' ';
                	flagDestruiu =1;
                	
                }

                

                mapa[i][j].npc=' ';
                          
            }


            if(mapa[i][j].npc=='<' && j>0){


            	mapa[i][j].npc=' ';

				int k;
            	for(k=2;j-k>=0;k++){

            		if(mapa[i][j-k].npc==' '){

            			mapa[i][j-k].npc='<';
            			k=1000;
            		}



            	}

            		
            }


            if(mapa[i][j].npc=='F' || mapa[i][j].npc=='X' || mapa[i][j].npc=='O' || mapa[i][j].npc=='T') troca(&mapa[i][j],&mapa[i][j-1]);

            

            if(j==0) mapa[i][j].npc=' '; /* Elimina os ultimos X */

        }
    }

    *combustivel=*combustivel - PERDA_DE_COMBUSTIVEL;  /* Perda de combustivel e ganho de pontos a cada interacao do gameloop */
    *pontos=*pontos + GANHO_DE_PONTOS_POR_DIST;

    if(*combustivel<=0) *combustivel=0;

}

/*
#####################################
# Struct para armazenar a pontuacao #
#####################################
*/




/* 
#########################################################
#  Ordena por meio de um bubble sort o arquivo          #
# ranking.bin ja criado na funcoes salvarRanking() ou   #
# mostraRanking()                                       #
#########################################################
*/


void ordenaRanking(){		

    ranking rank[20];

    ranking aux;

    int i,j;

    memset(rank,0,sizeof(rank));

    FILE *arq=fopen("ranking.bin","rb");

    fread(&rank,sizeof(ranking),20,arq);

    /* bubble sort */
    for(i=0;i<20;i++){					
    	for(j=0;j<20-i-1;j++){
    		if(rank[j].pontuacao < rank[j+1].pontuacao){

    			aux=rank[j];
    			rank[j]=rank[j+1];
    			rank[j+1]=aux;

    		}
    	}
    }
    fclose(arq);

    arq=fopen("ranking.bin","wb");
    fwrite(&rank,sizeof(ranking),20,arq);
    fclose(arq);
}

/* 
#########################################################
#  Salva o ranking no arquino ranking.bin alem          #
# de atualizar os nomes repetidos e so salvar           #
# as 20 maiores pontuacoes ordenadas com o auxilio      #
# da funcao ordenaRanking()                             #
#########################################################
*/

void salvaRanking(int *pontos){
    ranking player;
    ranking rank[20];

    memset(player.nome,0,sizeof(player));
    memset(rank,0,sizeof(rank));

    player.pontuacao = *pontos;
    
    FILE *arq = fopen("ranking.bin","a+b"); /*  Impede que o prgrama tente abrir um arquivo inexistente */
    fclose(arq);

    arq = fopen("ranking.bin","rb");
    fread(&rank ,sizeof(ranking),20,arq); 
    fclose(arq);

    if(player.pontuacao > rank[19].pontuacao){ /* Checa se a pontuacao foi maior que a ultima no arquivo */

    	cor(92);
    	printf("Parabens!!!!!!!! ------- Voce Bateu o Record, insira seu nome(max de 11 caracteres)\npara entrar no ranking -->>  ");
    	voltaCor();
    	scanf("%11s", player.nome);
    	setbuf(stdin,NULL);
 
    	int nomeCont=0;

    	while(strncmp(rank[nomeCont].nome,player.nome,11)!=0 && nomeCont<19) nomeCont++; /* Checa a se o nome digitado já existe no arquivo */

    	if(nomeCont<19){

            if(rank[nomeCont].pontuacao < player.pontuacao){ /* Checa que a pontuacao antida do jogador era maior que a atual */

    		     rank[nomeCont].pontuacao=player.pontuacao;
            }   
            else{
                cor(91);
                printf("\nVoce nao bateu seu ultimo record... Tente novamente!\n");
                voltaCor();
                printf("Precione qualquer tecla para retornar ao menu...");
                getch();
            }

    	}
    	else if(nomeCont==19){

    		rank[19]=player;

    	}

    }
    /* else criado para pausar o jogo na tela de game over mesmo sem ter batido o record por conta do scanf acima */
    else {

        cor(94);
        printf("Nenhum recorde batido\nPrecione qualquer tecla para retornar ao menu...\n");
        voltaCor();
        getch();
    }

    arq = fopen("ranking.bin","wb");
    fwrite(&rank,sizeof(ranking),20,arq);
    fclose(arq);
}



void salvaConfig(){

	FILE* fd;

	fd=fopen("config.txt","w"); /* escreve as mudanças no arquivo de texto */

	fprintf(fd,"altura: %d\n",LINHAS-2);
	fprintf(fd,"largura: %d\n",COLUNAS);
	fprintf(fd,"probX: %d\n",probX);
	fprintf(fd,"probY: %d\n",probF);
	fprintf(fd,"probT: %d\n",probT);
	fprintf(fd,"probO: %d\n",probO);
	fprintf(fd,"vidasO: %d\n",vidasO);
	fprintf(fd,"tirosT: %d\n",tirosT);
	fprintf(fd,"mapVelocidade: %d\n",velocidade);

	fclose(fd);


}




/*
#############################################
#  Loop principal do jogo que une as outras #
# funcoes para que o jogo funcione          #
#############################################
*/


void mainGameLoop(int ranked){	

	if(ranked==0){

		FILE* fd;



		fd = fopen("config.txt","a+b"); /*  Impede que o prgrama tente abrir um arquivo inexistente */
   		fclose(fd);
   		

		fd=fopen("config.txt","r");



		/*carrega configs*/

		fscanf(fd,"altura: %d\n",&LINHAS);
		LINHAS=LINHAS+2; /* coreção para ele ficar com 10 de espaco entre as paredes */
		fscanf(fd,"largura: %d\n",&COLUNAS);
		fscanf(fd,"probX: %d\n",&probX);
		fscanf(fd,"probY: %d\n",&probF);
		fscanf(fd,"probT: %d\n",&probT);
		fscanf(fd,"probO: %d\n",&probO);
		fscanf(fd,"vidasO: %d\n",&vidasO);
		fscanf(fd,"tirosT: %d\n",&tirosT);
		fscanf(fd,"mapVelocidade: %d\n",&velocidade);

		fclose(fd);


	}

	if(ranked==1){

		
		LINHAS =10+2;
		COLUNAS =135;
		probX = 30;					
		probF = 30;
		probT = 4;
		probO = 1;
		vidasO = 10;
		tirosT = 5;
		velocidade = 60;
		

	}

    /* Mapa passado por referencia para permirtir sua modificacao*/
	NPC mapa[LINHAS][COLUNAS];



	srand(time(0));/* seed para o rand ser aleatorio */


    /* Variaveis importantes */
	int combustivel=COMBUSTIVEL_INICIAL;
	int pontos=0;
	int flagTiro=0;
	int personagem=(LINHAS/2)-1;
	int gameOver=0;
    int quit=0;
	NPC objetos[LINHAS-1];



	zeraMatriz(mapa);
	instrucoes(mapa);
	/* Loop principal do jogo */
	while(gameOver==0){          

		geraObjetos(objetos);
		atualizaTeclado(&personagem,&flagTiro,&gameOver,&quit);
		atualizaMatriz(&personagem,&flagTiro,objetos,&pontos,&combustivel,&gameOver,mapa);
		system(CLEAR);
		printPlacar(&combustivel,&pontos);
		printMatriz(mapa,0);
		usleep(velocidade *1000);

		if(combustivel<=0) gameOver=1;

        printGameOver(&pontos,&combustivel,&quit,mapa,&gameOver);

	}	

		if(ranked==1){
		salvaRanking(&pontos);
		ordenaRanking();
		}
}

/*
##########################################
#  Abre o arquivo ranking.bin e Imprime  #
# ele na tela ao chamar essa funcao      #
##########################################
*/

void mostraRanking(){		
	system(CLEAR);
    ranking rank[20];

    FILE *arq = fopen("ranking.bin","a+b");
    fclose(arq);

    memset(rank,0,sizeof(rank));

    arq = fopen("ranking.bin","rb");
    fread(&rank,sizeof(ranking),20,arq);
    fclose(arq);

	cor(35);
    printf("\nRanking\t\tPontos\t\tNome\n\n");
	voltaCor(35);

    int i;
    for(i=0;i<20;i++){
        printf("N.%d\t\t%d\t\t%s\t\n",i+1,rank[i].pontuacao,rank[i].nome);
    }

    printf("\nPrecione qualquer tecla para retornar ao menu!\n");
    getch();
}


void Customizado(){

/* PARTE 2 */

}


/*
###############################################
# Configuracoes do jogo por meio da alteracao #
# das variaveis globais e outros              #
###############################################
*/  


void configuracoes(){


    int config=1;
    int voltar=0;
    int tecla;


    while(voltar==0){

        system(CLEAR);

        printf("\nConfiguracoes:\n\n");

        if(config==1) cor(100); /* Highlight do menu na cor 100 da tabela ansi*/
        cor(97);
        

        cor(97);
      	printf("1 - altura --- ");
      	cor(91);
        printf("{min-0}");
        cor(97);
        printf(" --- ");
        cor(92);
        printf("{max-40}");
        cor(97);
        printf(" --> ");
        printf("{atual-(%d)}:\n",LINHAS-2);
        voltaCor();

        if(config==2) cor(100);
        cor(97);
        

        cor(97);
      	printf("2 - largura --- ");
      	cor(91);
        printf("{min-10}");
        cor(97);
        printf(" --- ");
        cor(92);
        printf("{max-160}");
        cor(97);
        printf(" --> ");
        printf("{atual-(%d)}:\n",COLUNAS);
        voltaCor();

        if(config==3) cor(100);
        cor(97);
        
        cor(97);
      	printf("3 - probX --- ");
      	cor(91);
        printf("{min-0}");
        cor(97);
        printf(" --- ");
        cor(92);
        printf("{max-50}");
        cor(97);
        printf(" --> ");
        printf("{atual-(%d)}:\n",probX);
        voltaCor();

        if(config==4) cor(100);
        cor(97);
        

        cor(97);
      	printf("4 - probF --- ");
      	cor(91);
        printf("{min-0}");
        cor(97);
        printf(" --- ");
        cor(92);
        printf("{max-50}");
        cor(97);
        printf(" --> ");
        printf("{atual-(%d)}:\n",probF);
        voltaCor();

        if(config==5) cor(100);
        cor(97);

        cor(97);
      	printf("5 - probT --- ");
      	cor(91);
        printf("{min-0}");
        cor(97);
        printf(" --- ");
        cor(92);
        printf("{max-50}");
        cor(97);
        printf(" --> ");
        printf("{atual-(%d)}:\n",probT);
        voltaCor();

        if(config==6) cor(100);
        cor(97);

        cor(97);
      	printf("6 - probO --- ");
      	cor(91);
        printf("{min-0}");
        cor(97);
        printf(" --- ");
        cor(92);
        printf("{max-50}");
        cor(97);
        printf(" --> ");
        printf("{atual-(%d)}:\n",probO);
        voltaCor();

        if(config==7) cor(100);
        cor(97);

        cor(97);
      	printf("7 - vidasO --- ");
      	cor(91);
        printf("{min-1}");
        cor(97);
        printf(" --- ");
        cor(92);
        printf("{max-30}");
        cor(97);
        printf(" --> ");
        printf("{atual-(%d)}:\n",vidasO);
        voltaCor();

        if(config==8) cor(100);
      
        cor(97);
      	printf("8 - tirosT --- ");
      	cor(91);
        printf("{min-1}");
        cor(97);
        printf(" --- ");
        cor(92);
        printf("{max-30}");
        cor(97);
        printf(" --> ");
        printf("{atual-(%d)}:\n",tirosT);
        voltaCor();


        if(config==9) cor(100);
        cor(97);

        cor(97);
      	printf("9 - velocidade --- ");
      	cor(91);
        printf("{min-1}");
        cor(97);
        printf(" --- ");
        cor(92);
        printf("{max-500}");
        cor(97);
        printf(" --> ");
        printf("{atual-(%d)}:\n",velocidade);
        voltaCor();

        if(config==10) cor(100);
        cor(97);

        cor(95);
        printf("\n0 - Pronto\n");
        voltaCor();

        	
        tecla=getch();


        /* Meios de navegar no menu */
        if((tecla=='w' || tecla=='W')&& config>1) config--;
        if((tecla=='s' || tecla=='S')&& config<10) config++;
        if(tecla=='1') config=1;
        if(tecla=='2') config=2;
        if(tecla=='3') config=3;
        if(tecla=='4') config=4;
        if(tecla=='5') config=5;
        if(tecla=='6') config=6;
        if(tecla=='7') config=7;
        if(tecla=='8') config=8;
        if(tecla=='9') config=9;
        if(tecla=='0') config=10;


        if(tecla==TECLA_ENTER){

            /* Casos do menu */
        	switch (config) {


        		case 1:  do{printf("Digite o valor desejado e valido:\n"); scanf("%d",&LINHAS);getch(); LINHAS=LINHAS+2;}while(LINHAS-2<10 || LINHAS-2>40); break; /* correcao para que tenha um espaco de 10 entre as peredes */ break;
        		case 2:  do{printf("Digite o valor desejado e valido:\n"); scanf("%d",&COLUNAS);getch(); }while(COLUNAS<10 || COLUNAS >160); break;
        		case 3:  do{printf("Digite o valor desejado e valido:\n"); scanf("%d",&probX);getch(); }while(probX<0 || probX > 50); break;
        		case 4:  do{printf("Digite o valor desejado e valido:\n"); scanf("%d",&probF);getch(); }while(probF<0 || probF > 50); break;
        		case 5:  do{printf("Digite o valor desejado e valido:\n"); scanf("%d",&probT);getch(); }while(probT<0 || probT > 50); break;
        		case 6:  do{printf("Digite o valor desejado e valido:\n"); scanf("%d",&probO);getch(); }while(probO<0 || probO > 50); break;
        		case 7:  do{printf("Digite o valor desejado e valido:\n"); scanf("%d",&vidasO);getch(); }while(vidasO<1 || vidasO>30); break;
        		case 8:  do{printf("Digite o valor desejado e valido:\n"); scanf("%d",&tirosT);getch(); }while(tirosT<1 || tirosT > 30); break;
        		case 9:  do{printf("Digite o valor desejado e valido:\n"); scanf("%d",&velocidade);getch(); }while(velocidade<1 || velocidade > 500); break;
                    

        		case 10:

        			salvaConfig();
        		
        			voltar=1;

        		break;
        	}

        	
            tecla=0;
    	}
	}
        
}






/*
#####################################################
#  Menu inicial que chama as outras funcoes         #
# principai como a mainGameLoop() a configuracoes() #
# e a mostraRanking()                               #
#####################################################
*/

int main(){

	FILE *fd;



		fd = fopen("config.txt","a+b"); /*  Impede que o prgrama tente abrir um arquivo inexistente */
   		fclose(fd);



		fd=fopen("config.txt","r");
		
		/*carrega configs*/
		fscanf(fd,"altura: %d\n",&LINHAS);
		LINHAS=LINHAS+2; /* coreção para ele ficar com 10 de espaco entre as paredes */
		fscanf(fd,"largura: %d\n",&COLUNAS);
		fscanf(fd,"probX: %d\n",&probX);
		fscanf(fd,"probY: %d\n",&probF);
		fscanf(fd,"probT: %d\n",&probT);
		fscanf(fd,"probO: %d\n",&probO);
		fscanf(fd,"vidasO: %d\n",&vidasO);
		fscanf(fd,"tirosT: %d\n",&tirosT);
		fscanf(fd,"mapVelocidade: %d\n",&velocidade);

		fclose(fd);



    int menu=1;
    int sair=0;
    char tecla=0;


    printBemVindo();
	
	while(sair==0){

		system(CLEAR);

		printf("\nMenu:\n\n");

        if(menu==1) cor(100);
		cor(92);
		printf("1 - Jogar\n");
        voltaCor();

        if(menu==2) cor(100);
		cor(93);
		printf("2 - Configuracoes\n");
        voltaCor();

        if(menu==3) cor(100);
		cor(96);
		printf("3 - Jogar (Ranqueado)\n");
        voltaCor();

        if(menu==4) cor(100);
		cor(95);
		printf("4 - Ranking\n");
        voltaCor();

        if(menu==5) cor(100);
		cor(91);
		printf("5 - Sair\n");
        voltaCor();


		voltaCor();

        tecla=getch();

        /* Meios de navegar no menu */
        if((tecla=='w' || tecla=='W') && menu>1) menu--;
        if((tecla=='s' || tecla=='S') && menu<5) menu++;
        if(tecla=='1') menu=1;
        if(tecla=='2') menu=2;
        if(tecla=='3') menu=3;
        if(tecla=='4') menu=4;
        if(tecla=='5') menu=5;

        if(tecla==TECLA_ENTER){

            /* Casos do menu */
    		switch (menu) {
    			case 1: mainGameLoop(0); break;
    			case 2: configuracoes(); break;
    			case 3: mainGameLoop(1); break;
    			case 4: mostraRanking(); break;
    			case 5: sair=1; break;
		    }

            tecla=0;
            menu=1;
        }	
	}
	return 0;
}

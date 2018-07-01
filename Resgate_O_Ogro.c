/*Jogo Resgate o Ogro implementado por Lucas Cardoso dos Santos (Cartão UFRGS: 00304243) e Paulo Gamarra Lessa Pinto (Cartão UFRGS: 00305674) */

//Endereço para a biblioteca conio no linker: C:\Program Files (x86)\CodeBlocks\MinGW\lib\libconio.a

// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <conio2.h>
#include <time.h>
#include <windows.h>
#include <locale.h>

// Menu
#define COR_QUADRO BLUE
#define COR_QUADRO_SELECIONADO GREEN
#define COR_DENTRO DARKGRAY
#define COR_DENTRO_SELECIONADO WHITE
#define COR_TEXTO WHITE
#define COR_TEXTO_SELECIONADO BLACK

// Dimensões da moldura
#define ALTURA 24
#define LARGURA 80
#define COLUNA1 1
#define LINHA1 1

// Jogador
#define TAM_NOME 11 // 50 caracteres + 1 para o '\0'
#define MOVIMENTO_JOGADOR_MORTO 3

// Paredes
#define NUM_SEGMENTOS 5
#define TAM_PAREDE 50
#define TAM_TORRE 15

// Chaves
#define NUM_CHAVE 5
#define MIN_CHAVES 3

// Guardas
#define NUM_GUARDA 5

// Teclas
#define SETAS -32
#define CIMA 72
#define BAIXO 80
#define DIREITA 77
#define ESQUERDA 75
#define ESC 27
#define ENTER 13

// Ranking
#define TAM_RAN 10

// Estruturas
typedef struct
{
    int x;
    int y;
}
COORDENADA;

typedef struct
{
    char nome[11];
    char ch; // ASCII do caracter que representa o jogador
    int chaves; // chaves coletadas
    int vida; // numero de vidas que dispoem. Inicializado com 3
    float pontos; // tempo de jogo. Inicializado com clock
    COORDENADA posicao;
}
JOGADOR;

typedef struct
{
    int direcao;

    /*
    A direção que o guarda irá se mover pode ser tanto pra cima, pra baixo, pra direita e esquerda quanto para as diagonais.
    São suas possibilidades:
    0 - para diagonal inferior esquerda
    1 - para baixo
    2 - para diagonal inferior direita
    3 - para direita
    4 - para diagonal superior direita
    5 - para cima
    6 - para diagonal superior esquerda
    7 - para esquerda
    */

    int ch; // ASCII do caracter que representa o guarda
    int passos;
    COORDENADA posicao;
    COORDENADA destino;
}
GUARDA;

typedef struct
{
    int status;

    /*
    O status da chave é expresso em 3 opções, sendo variáveis durante o jogo. São elas:
    1 - Invisível, em situações que o jogador se encontra distante da chave e ela ainda não foi capturada.
    2 - Visível, em situações que o jogador se encontra próximo da chave e ela ainda não foi capturada.
    3 - Capturada, em situações que o jogador já passou pelas exatas coordenadas da chave.
    */

    int ch; // ASCII do caracter que representa a chave
    COORDENADA posicao;
} CHAVE;

typedef struct
{
    int centesimos;
    int segundos;
    int minutos;
} TEMPO;

typedef struct
{
    // cores
    int cor_jogador;
    int cor_moldura;
    int cor_parede;
    int cor_chave;
    int cor_guardas;
    int cor_torre;
    int cor_ogro;
    int cor_cenario;

    // efeitos
    int efeitosGraficos;
} CONFIGURACOES;

typedef struct
{
    char nome[TAM_NOME];
    int chaves_pegas;
    int partidas_jogadas;
    int partidas_faceis;
    int partidas_dificeis;
    int mortes;
    int vitorias;
    int erro;
    TEMPO tempo_medio;
    TEMPO mais_rapido;
    TEMPO mais_lento;
} ESTATISTICAS;

typedef struct
{
    JOGADOR jogador;
    CHAVE listaChaves[NUM_CHAVE];
    GUARDA listaGuardas[NUM_GUARDA];
    COORDENADA paredes[TAM_PAREDE];
    COORDENADA torre[TAM_TORRE];
    COORDENADA ogro;
    TEMPO relogio;
    int num_chaves;
    int num_guardas;
    int num_paredes;
    int nivel;
    CONFIGURACOES configuracoes;
    ESTATISTICAS estatisticas;
} JOGO;

//funções
void Moldura();
// A função Moldura cria nas coordenadas especificadas pelo enunciado do trabalho uma moldura que serve como limite para a movimentação
// do personagem durante o jogo.

void TelaInicial();
// Essa função coloca na tela os escritos "Resgate o Ogro!" em ASCII art, assim como um pequeno quadro com as informações ténnicas
// e autorais sobre o jogo.

void PegaNome(JOGADOR *jogador);
// Essa função atribui a estrutura Jogador um nome digitado pelo jogador.

void IniciaJogo(JOGO *jogo);

void MovimentaJogador(JOGO *jogo, char tecla);

void ApagaElemento (int x, int y, int cor_cenario);

int TestaColisao(int x, int y, JOGO jogo);

void TestaChave(JOGO *jogo);

int PegaChave (JOGO *jogo);

void ApagaTorres(COORDENADA torre[], int cor_cenario);

int TestaGuarda (JOGADOR jogador, int num_guardas, GUARDA listaGuardas[]);

int MataJogador(JOGO *jogo, int *encurralado);

void GeraParedes (COORDENADA paredes[], int num_paredes);

void GeraGuardas(JOGO *jogo);

void GeraJogador (JOGO *jogo);

void GeraChaves (JOGO *jogo);

void GeraTorre(JOGO *jogo);

void DesenhaJogador(JOGADOR jogador, int cor_jogador);

void DesenhaGuarda(GUARDA listaGuardas[], int num_guardas, int cor_guardas);

void DesenhaParedes (COORDENADA paredes[], int num_paredes, int cor_parede);

void DesenhaTorre(COORDENADA torre[], CONFIGURACOES configuracoes);

void DesenhaOgro(COORDENADA ogro, int cor_ogro);

void DesenhaChave (CHAVE chave, int cor_chave);

void InicializaChaves (CHAVE listaChaves[], int num_chaves);

void InicializaJogo (int *num_paredes, int *num_chaves, int *num_guardas, int *nivel, COORDENADA listaParede[], CHAVE listaChave[], GUARDA listaGuardas[], JOGADOR *jogador, COORDENADA torre[], COORDENADA *ogro);

void InicializaJogador (JOGADOR *jogador);

void InicializaGuarda (GUARDA listaGuardas[], int num_guardas);

void EscolheDificuldade (int *num_paredes, int *num_chaves, int *num_guardas, int *nivel);

void TerminaJogo();

void Menu(JOGO *jogo);

void Legenda (JOGADOR jogador, CONFIGURACOES configuracoes);

void AreaDeInformacao (JOGO jogo);

int GanhaJogo (JOGADOR jogador, COORDENADA ogro);

void DesenhaRanking(TEMPO relogio[], char nomes[TAM_RAN][TAM_NOME], int posicoes[], int quantObj[], char objeto[7]);

void MoveGuarda(JOGO *jogo);

void AtualizaVidas(JOGADOR jogador);

void AtualizaTempo(TEMPO relogio);

void AtualizaChaves(JOGADOR jogador);

void Quadro (int corQuadro, int corDentro, int corTexto, int menorx, int maiorx, int menory, int maiory, char texto[16]);

void TrocaNomes (char nome1[], char nome2[]);

void TrocaNumeros (int *num1, int *num2);

void OrdenaRanking (JOGO jogo, TEMPO pontos[(TAM_RAN + 1)], char nome[][TAM_NOME], int quantObj[]);

void MostraRanking(JOGO *jogo);

void EscolherJogo(JOGO *jogo);

void Ajuda(JOGO *jogo);

void Sair(JOGO *jogo);

void GeraJogoNovo (JOGO *jogo);

void GeraOgro(COORDENADA *ogro);

void Salvando();

void CarregarPartida(JOGO *jogo);

void Carregando();

void InicializaTempo (JOGO *jogo);

void SalvarPartida(JOGO *jogo);

void DesenhaGuerreiro();

int EscolheUmOuDois(char palavra1[16], char palavra2[16], int menorx, int menory);

void RetornarJogo(JOGO *jogo);

void Ranking(JOGO *jogo);

int SalvaJogador(JOGO *jogo);

void mostraEstatisticas (ESTATISTICAS estatisticas);

void CriaEstatisticasJogador ();

void SalvarEstatistica (char nome[], ESTATISTICAS *estatisticas);

void ConvertePontoEmTempo (int pontuacao, TEMPO *tempo);

int ConverteTempoEmPonto(TEMPO tempo);

void CarregaEstatisticas (char nome[], ESTATISTICAS *estatisticas);

void AlteraEstatisticas (JOGO *jogo);

int CriaMenuPersonalizado (char botoes[][16], int menorx, int menory, int quantidade_de_botoes, int quantidade_linhas, int quantidade_colunas);

void Configuracoes(JOGO *jogo);

void PintaCenario(JOGO jogo);

void MudaCores(CONFIGURACOES *configuracoes);

void Sangue (JOGO jogo);

void AlteraEfeitosGraficos(JOGO *jogo);

int main()
{
    JOGO jogo;

    DefineConfiguracoesInicias(&jogo.configuracoes);
    jogo.nivel = 0;

    TelaInicial();
    PegaNome(&jogo.jogador);
    Menu(&jogo);
	return(0);
}

void DefineConfiguracoesInicias (CONFIGURACOES *conf)
{
    conf->cor_jogador = BLUE;
    conf->cor_moldura = LIGHTGREEN;
    conf->cor_parede = LIGHTGRAY;
    conf->cor_chave = YELLOW;
    conf->cor_guardas = WHITE;
    conf->cor_torre = DARKGRAY;
    conf->cor_ogro = GREEN;
    conf->cor_cenario = BLACK;
    conf->efeitosGraficos = 1;
}

void TelaInicial() // ok
{
    HWND resgateOgro;

    SetConsoleTitle("Resgate o Ogro!");
    resgateOgro = GetForegroundWindow();
    ShowWindow(resgateOgro, SW_MAXIMIZE);

    printf("\n");

    /* Escrito com ascii art: Resgate o Ogro e colorido com as cores do jogador, da torre e do ogro */
    printf("                                                                     __ \n");
    printf("           _____                 _                 _____            ");
    textbackground(GREEN);          printf("|  |\n");
    textbackground(BLACK);          printf("          ");
    textbackground(BLUE);           printf("| __  |");
    textbackground(BLACK);          printf("___ ___ ___ ___");
    textbackground(BLUE);           printf("| |");
    textbackground(BLACK);          printf("_ ___    ___   ");
    textbackground(GREEN);          printf("|     |");
    textbackground(BLACK);          printf("___ ___ ___");
    textbackground(GREEN);          printf("|  |\n");
    textbackground(BLACK);          printf("          ");
    textbackground(BLUE);           printf("|    -| -_|_ -| . | .'|  _| -_|");
    textbackground(BLACK);          printf("  ");
    textbackground(DARKGRAY);       printf("| . |");
    textbackground(BLACK);          printf("  ");
    textbackground(GREEN);          printf("|  |  | . |  _| . |__|\n");
    textbackground(BLACK);          printf("          ");
    textbackground(BLUE);           printf("|__|__|___|___|_  |__,|_|");
    textbackground(BLACK);          printf(" ");
    textbackground(BLUE);           printf("|___|");
    textbackground(BLACK);          printf("  ");
    textbackground(DARKGRAY);       printf("|___|");
    textbackground(BLACK);          printf("  ");
    textbackground(GREEN);          printf("|_____|_  |_|");
    textbackground(BLACK);          printf(" ");
    textbackground(GREEN);          printf("|___|__|\n");
    textbackground(BLACK);          printf("                        ");
    textbackground(BLUE);           printf("|___|");
    textbackground(BLACK);          printf("                           ");
    textbackground(GREEN);          printf("|___|\n");
    textbackground(BLACK);
    printf("\n");

    printf("                  ==============================================                  \n");
    printf("                  |                                            |                  \n");
    printf("                  |  Jogo desenvolvido por:     Cartao UFRGS:  |                  \n");
    printf("                  |                                            |                  \n");
    printf("                  |  Lucas Cardoso dos Santos   00304243       |                  \n");
    printf("                  |  Paulo Gamarra Lessa Pinto  00305674       |                  \n");
    printf("                  |                                            |                  \n");
    printf("                  |                                            |                  \n");
    printf("                  |  Para a disciplina de:                     |                  \n");
    printf("                  |  Algoritmos e Programacao (INF 01202)      |                  \n");
    printf("                  |  Professora: Mara Abel                     |                  \n");
    printf("                  |                                            |                  \n");
    printf("                  |  Semestre: 2018/1                          |                  \n");
    printf("                  |                                            |                  \n");
    printf("                  ==============================================                  \n");
    printf("\n\n");
    printf("                      Pressione ENTER para comecar o jogo...    \n");

    getch();
    system("cls");
}

void GeraJogoNovo (JOGO *jogo)
{
    srand((unsigned)time(NULL) * rand());

	GeraParedes(jogo->paredes, jogo->num_paredes);
    GeraTorre(jogo);
	GeraChaves(jogo);
    GeraGuardas(jogo);
    InicializaJogador(&jogo->jogador);
    GeraJogador(jogo);
    InicializaTempo(jogo);
}

void InicializaTempo (JOGO *jogo)
{
    jogo->relogio.minutos = 0;
    jogo->relogio.centesimos = 0;
    jogo->relogio.segundos = 0;
}

void DesenhaJogo (JOGO jogo)
{
    system("cls");

    PintaCenario(jogo);
    Moldura();
    if (jogo.jogador.chaves < MIN_CHAVES)
    {
        DesenhaTorre(jogo.torre, jogo.configuracoes);
    }
    DesenhaOgro(jogo.ogro, jogo.configuracoes.cor_ogro);
    DesenhaParedes(jogo.paredes, jogo.num_paredes, jogo.configuracoes.cor_parede);
    DesenhaGuarda(jogo.listaGuardas, jogo.num_guardas, jogo.configuracoes.cor_guardas);
    DesenhaJogador(jogo.jogador, jogo.configuracoes.cor_jogador);
}

void PintaCenario (JOGO jogo)
{
    int x, y;

    textbackground(jogo.configuracoes.cor_cenario);
    for (x = 1; x <= LARGURA; x++)
    {
        for (y = 1; y <= ALTURA; y++)
        {
            putchxy(x, y, ' ');
        }
    }
    textbackground(BLACK);
}

void AreaDeInformacao (JOGO jogo)
{
    gotoxy (COLUNA1, ALTURA+1);
    printf("Nome: %-10s  ", jogo.jogador.nome);
    printf("Vidas: %s %s %s   ", (jogo.jogador.vida > 0 ? "<3":"  "), (jogo.jogador.vida > 1 ? "<3":"  "), (jogo.jogador.vida > 2 ? "<3":"  "));
    printf("Chaves: %d/%d  ", jogo.jogador.chaves, MIN_CHAVES);
    printf("Modo: %s  ", (jogo.nivel == 1 ? "Facil":"Dificil"));
    gotoxy(jogo.jogador.posicao.x, jogo.jogador.posicao.y);
}

void PegaNome(JOGADOR *jogador)
{
    int laco;
    int tamanho;

    do
    {
        laco = 0;

        printf("Digite o seu nome:\n");
        gets(jogador->nome);

        tamanho = strlen(jogador->nome);

        system("cls");
        if (tamanho == 0)
        {
            MessageBox(0,"Voce deve digitar seu nome para poder continuar!","Nome inexistente!",MB_OK|MB_ICONEXCLAMATION);
            laco = 1;
        }
        if (tamanho > 10)
        {
            MessageBox(0, "O nome inserido ultrapassa 10 caracteres.\nTente novamente!", "Nome muito grande!", MB_OK|MB_ICONEXCLAMATION);
            laco = 1;
        }
    }
    while (laco);
}

void CarregaEstatisticas (char nome[], ESTATISTICAS *estatisticas)
{
    ESTATISTICAS estatisticas_aux;
    FILE *arq_estatisticas;
    int achou = 0;

    strcpy(estatisticas->nome, nome);

    arq_estatisticas = fopen("estatisticas.bin", "rb");
    if (arq_estatisticas == NULL)
    {
        fclose(arq_estatisticas);
        CriaEstatisticasJogador(estatisticas);
    }
    else
    {
        while (fread(&estatisticas_aux, sizeof(ESTATISTICAS), 1, arq_estatisticas))
        {
            if (strcmp(estatisticas_aux.nome, estatisticas->nome) == 0)
            {
                estatisticas->partidas_jogadas = estatisticas_aux.partidas_jogadas;
                estatisticas->chaves_pegas = estatisticas_aux.chaves_pegas;
                estatisticas->erro = estatisticas_aux.erro;
                estatisticas->mais_lento.centesimos = estatisticas_aux.mais_lento.centesimos;
                estatisticas->mais_lento.segundos = estatisticas_aux.mais_lento.segundos;
                estatisticas->mais_lento.minutos = estatisticas_aux.mais_lento.minutos;
                estatisticas->mais_rapido.centesimos = estatisticas_aux.mais_rapido.centesimos;
                estatisticas->mais_rapido.segundos = estatisticas_aux.mais_rapido.segundos;
                estatisticas->mais_rapido.minutos = estatisticas_aux.mais_rapido.minutos;
                estatisticas->tempo_medio.centesimos = estatisticas_aux.tempo_medio.centesimos;
                estatisticas->tempo_medio.segundos = estatisticas_aux.tempo_medio.segundos;
                estatisticas->tempo_medio.minutos = estatisticas_aux.tempo_medio.minutos;
                estatisticas->partidas_faceis = estatisticas_aux.partidas_faceis;
                estatisticas->partidas_dificeis = estatisticas_aux.partidas_dificeis;
                estatisticas->vitorias = estatisticas_aux.vitorias;

                achou = 1;

                fseek(arq_estatisticas, 0, SEEK_END);
            }
        }

        fclose(arq_estatisticas);

        if(!achou)
        {
            MessageBox(0, "Nao foi encontrado no arquivo um jogo salvo com seu nome para carregar!", "Erro ao achar!", MB_OK|MB_ICONEXCLAMATION);
            CriaEstatisticasJogador(estatisticas);
        }
    }
}

void CriaEstatisticasJogador(ESTATISTICAS *estatisticas)
{
    FILE *arq_estatisticas;

    estatisticas->chaves_pegas = 0;
    estatisticas->mais_lento.centesimos = 0;
    estatisticas->mais_lento.segundos = 0;
    estatisticas->mais_lento.minutos = 0;
    estatisticas->mais_rapido.centesimos = 0;
    estatisticas->mais_rapido.segundos = 0;
    estatisticas->mais_rapido.minutos = 0;
    estatisticas->partidas_dificeis = 0;
    estatisticas->partidas_faceis = 0;
    estatisticas->tempo_medio.centesimos = 0;
    estatisticas->tempo_medio.segundos = 0;
    estatisticas->tempo_medio.minutos = 0;
    estatisticas->erro = 0;
    estatisticas->mortes = 0;
    estatisticas->partidas_jogadas = 0;
    estatisticas->vitorias = 0;

    arq_estatisticas = fopen("estatisticas.bin", "ab");
    if (arq_estatisticas == NULL)
    {
        fclose(arq_estatisticas);
        MessageBox(0, "Nao foi possivel abrir ou criar um arquivo binario!\nVerifique se voce tem permissao para tal.", "Erro ao abrir!", MB_OK|MB_ICONEXCLAMATION);
        estatisticas->erro = 3;
    }
    else
    {
        if (fwrite(&estatisticas, sizeof(ESTATISTICAS), 1, arq_estatisticas))
        {
            fclose(arq_estatisticas);
        }
        else
        {
            MessageBox(0, "Nao foi possivel escrever no arquivo binario!\nCriaEstatisticaJogador", "Erro na escrita!", MB_OK|MB_ICONEXCLAMATION);
            estatisticas->erro = 1;
            fclose(arq_estatisticas);
        }
    }
}

void Menu (JOGO *jogo)
{
    int quadro;
    char botoes[9][16];

    CarregaEstatisticas(jogo->jogador.nome, &jogo->estatisticas);

    strcpy(botoes[0], "Novo jogo");
    strcpy(botoes[1], "Retornar o jogo");
    strcpy(botoes[2], "Carregar jogo");
    strcpy(botoes[3], "Salvar jogo");
    strcpy(botoes[4], "Estatisticas");
    strcpy(botoes[5], "Ranking");
    strcpy(botoes[6], "Ajuda");
    strcpy(botoes[7], "Configuracoes");
    strcpy(botoes[8], "Sair");

    system("cls");

    DesenhaGuerreiro();

    quadro = CriaMenuPersonalizado(botoes, 3, 2, 9, 5, 2);

    switch(quadro)
    {
        case 1:
            EscolherJogo(jogo);
            break;
        case 2:
            RetornarJogo(jogo);
            break;
        case 3:
            CarregarPartida(jogo);
            break;
        case 4:
            SalvarPartida(jogo);
            break;
        case 5:
            mostraEstatisticas(jogo->estatisticas);
            break;
        case 6:
            MostraRanking(jogo);
            break;
        case 7:
            Ajuda(jogo);
            break;
        case 8:
            Configuracoes(jogo);
            break;
        case 9:
            Sair(jogo);
            break;
    }

    Menu(jogo);
}

void DesenhaGuerreiro()
{
    textbackground(LIGHTGRAY);
    textcolor(BLACK);
    gotoxy(52, 3);   printf("       _,.                  ");
    gotoxy(52, 4);   printf("     ,` -.)                 ");
    gotoxy(52, 5);   printf("    ( _/-\\\\-._              ");
    gotoxy(52, 6);   printf("   /,|`--._,-^|            ,");
    gotoxy(52, 7);   printf("   \\_| |`-._/||          ,'|");
    gotoxy(52, 8);   printf("     |  `-, / |         /  /");
    gotoxy(52, 9);   printf("     |     || |        /  / ");
    gotoxy(52, 10);  printf("      `r-._||/   __   /  /  ");
    gotoxy(52, 11);  printf("  __,-<_     )`-/  `./  /   ");
    gotoxy(52, 12);  printf(" '  \\   `---'   \\   /  /    ");
    gotoxy(52, 13);  printf("     |           |./  /     ");
    gotoxy(52, 14);  printf("     /           //  /      ");
    gotoxy(52, 15);  printf(" \\_/' \\         |/  /       ");
    gotoxy(52, 16);  printf("  |    |   _,^-'/  /        ");
    gotoxy(52, 17);  printf("  |    , ``  (\\/  /_        ");
    gotoxy(52, 18);  printf("   \\,.->._    \\X-=/^        ");
    gotoxy(52, 19);  printf("   (  /   `-._//^`          ");
    gotoxy(52, 20);  printf("    `Y-.____(__}            ");
    gotoxy(52, 21);  printf("     |     {__)             ");
    gotoxy(52, 22);  printf("           ()               ");

    textbackground(YELLOW);
    for (int i = 0; i < 21; i++)
    {
        putchxy(51, 2 + i, ' ');
        putchxy(80, 2 + i, ' ');
    }
    for (int i = 0; i < 30; i++)
    {
        putchxy(51 + i, 2, ' ');
        putchxy(51 + i, 23, ' ');
    }

    textcolor(WHITE);
    textbackground(BLACK);
    putchxy(81, 9, 'L');
}

void RetornarJogo(JOGO *jogo)
{
    system("cls");

    if (jogo->nivel == 0)
    {
        MessageBox(0,"Voce precisa comecar uma partida antes de poder retornar a\num jogo ja comecado.","Jogo nao encontrado!",MB_OK|MB_ICONEXCLAMATION);
        printf("\n  Aperte qualquer botao para voltar ao Menu\n");

        getch();
    }
    else
    {
        if (jogo->jogador.vida <= 0)
        {
            MessageBox(0,"Voce perdeu! Vai ter que comecar outra!","Que espertinho!",MB_OK|MB_ICONEXCLAMATION);
            printf(" Aperte qualquer tecla para voltar ao menu");
            getch();
            system("cls");
        }
        else
        {
            DesenhaJogo(*jogo);
            IniciaJogo(jogo);
        }
    }
}

int EscolheUmOuDois(char palavra1[16], char palavra2[16], int menorx, int menory)
{
    int quadro;
    char botoes[2][16];


    strcpy(botoes[0], palavra1);
    strcpy(botoes[1], palavra2);

    quadro = CriaMenuPersonalizado(botoes, menorx, menory, 2, 1, 2);

    if (quadro == 1)
    {
        return(1);
    }
    else
    {
        return(0);
    }
}

void Sair(JOGO *jogo)
{
    system("cls");

    printf("\n");
    printf("  Deseja sair mesmo?\n");
    printf("  Qualquer jogo nao salvo sera perdido\n");

    if (EscolheUmOuDois("Sim", "Nao", 5, 5))
    {
        SalvarEstatistica(jogo->jogador.nome, &jogo->estatisticas);
        system("cls");
        exit(0);
    }
}

void SalvarPartida(JOGO *jogo)
{
    system("cls");
    printf("\n");
    printf("  Deseja realmente salvar o seu jogo atual?\n");
    printf("  Esse procedimento reescreve qualquer outro jogo antigo seu...\n");

    if (EscolheUmOuDois("Sim", "Nao", 2, 5))
    {
        JOGO jogoAux;
        FILE *jogo_salvo;
        int salvo = 0;
        int controle;
        long deslocamentoAtual = 0;

        system("cls");

        jogo_salvo = fopen("jogo_salvo.bin", "r+b");
        if (jogo_salvo == NULL)
        {
            fclose(jogo_salvo);
            MessageBox(0, "O-Oh! Isso e vergonhoso!\nNao conseguimos abrir corretamente o seu arquivo salvo!", "Arquivo nao aberto!", MB_OK|MB_ICONEXCLAMATION);
            printf("\n");
            printf("  Deseja criar um arquivo novo ou sobreescrever o antigo?\n");
            printf("  Lembrando que todos os seus dados antigos serao perdidos!\n");
            if(EscolheUmOuDois("Sim", "Nao", 2, 5))
            {
                system("cls");
                jogo_salvo = fopen("jogo_salvo.bin", "wb");
                printf("\n\n");
                if(fwrite(jogo, sizeof(JOGO), 1, jogo_salvo))
                {
                    system("cls");
                    fclose(jogo_salvo);
                    printf("  Pronto!\n");
                    printf("  Seu arquivo foi criado e o arquivo, salvo!\n");
                    printf("  Pressione qualquer tecla para voltar ao Menu!\n");
                    getch();
                }
                else
                {
                    printf("  Uau, que azar!\n");
                    printf("  Nao pudemos escrever novamente o seu jogo.\n");
                    printf("  Pressione qualquer tecla para retornar ao menu\n");
                    getch();
                    fclose(jogo_salvo);
                }
            }
            else
            {
                system("cls");
            }
        }
        else
        {
            while (fread(&jogoAux, sizeof(JOGO), 1, jogo_salvo))
            {
                if (!strcmp(jogoAux.jogador.nome, jogo->jogador.nome)) // Se o nome do jogador e o nome salvo lido são iguais, executa
                {
                    fseek(jogo_salvo, deslocamentoAtual, SEEK_SET);
                    Salvando();
                    controle = fwrite(jogo, sizeof(JOGO), 1, jogo_salvo);
                    system("cls");
                    if (controle != 1)
                    {
                        MessageBox(0, "Uma quantidade diferente de 1 foi escrita no arquivo", "Um erro ocorreu na escrita!", MB_OK|MB_ICONEXCLAMATION);
                        fclose(jogo_salvo);
                    }
                    else
                    {
                        salvo = 1;
                        printf("  Jogador atualizado!\n");
                    }

                    fseek(jogo_salvo, 0, SEEK_END);
                }
                deslocamentoAtual = ftell(jogo_salvo);
            }

            if (!salvo) // Se não foi salvo nenhuma vez
            {
                Salvando();
                fwrite(jogo, sizeof(JOGO), 1, jogo_salvo);
                printf("  Novo jogador salvo!\n");
            }
            fclose(jogo_salvo);
        }

        printf("\n  Pressione qualquer tecla para voltar ao menu\n");
        getch();
        system("cls");
    }
    else
    {
        system("cls");
    }
}

void CarregarPartida(JOGO *jogo)
{
    FILE *jogo_salvo;
    JOGO jogoAux;
    int i, carregado;

    system("cls");

    jogo_salvo = fopen("jogo_salvo.bin", "rb");
    if (jogo_salvo == NULL)
    {
        fclose(jogo_salvo);
        MessageBox(0,"Sinto muito, mas nao foi encontrado nenhum jogo salvo!\nTente jogar algum jogo para poder carrega-lo!","Sem jogos salvos!",MB_OK|MB_ICONEXCLAMATION);
        printf(" Pressione qualquer tecla para voltar ao Menu\n");
        getch();
    }

    while (fread(&jogoAux, sizeof(JOGO), 1, jogo_salvo))
    {
        if (strcmp(jogoAux.jogador.nome, jogo->jogador.nome) == 0)
        {
            Carregando();
            jogo->num_chaves = jogoAux.num_chaves;
            jogo->num_guardas = jogoAux.num_guardas;
            jogo->num_paredes = jogoAux.num_paredes;
            jogo->nivel = jogoAux.nivel;
            jogo->jogador.ch = jogoAux.jogador.ch;
            jogo->jogador.chaves = jogoAux.jogador.chaves;
            jogo->jogador.pontos = jogoAux.jogador.pontos;
            jogo->jogador.posicao.x = jogoAux.jogador.posicao.x;
            jogo->jogador.posicao.y = jogoAux.jogador.posicao.y;
            jogo->jogador.vida = jogoAux.jogador.vida;
            for (i = 0; i < jogoAux.num_chaves; i++)
            {
                jogo->listaChaves[i].ch = jogoAux.listaChaves[i].ch;
                jogo->listaChaves[i].posicao.x = jogoAux.listaChaves[i].posicao.x;
                jogo->listaChaves[i].posicao.y = jogoAux.listaChaves[i].posicao.y;
                jogo->listaChaves[i].status = jogoAux.listaChaves[i].status;
            }
            for (i = 0; i < jogoAux.num_guardas; i++)
            {
                jogo->listaGuardas[i].ch = jogoAux.listaGuardas[i].ch;
                jogo->listaGuardas[i].destino.x = jogoAux.listaGuardas[i].destino.x;
                jogo->listaGuardas[i].destino.y = jogoAux.listaGuardas[i].destino.y;
                jogo->listaGuardas[i].direcao = jogoAux.listaGuardas[i].direcao;
                jogo->listaGuardas[i].passos = jogoAux.listaGuardas[i].passos;
                jogo->listaGuardas[i].posicao.x = jogoAux.listaGuardas[i].posicao.x;
                jogo->listaGuardas[i].posicao.y = jogoAux.listaGuardas[i].posicao.y;
            }
            for (i = 0; i < jogoAux.num_paredes * NUM_SEGMENTOS; i++)
            {
                jogo->paredes[i].x = jogoAux.paredes[i].x;
                jogo->paredes[i].y = jogoAux.paredes[i].y;
            }
            for (i = 0; i < TAM_TORRE; i++)
            {
                jogo->torre[i].x = jogoAux.torre[i].x;
                jogo->torre[i].y = jogoAux.torre[i].y;
            }
            jogo->ogro.x = jogoAux.ogro.x;
            jogo->ogro.y = jogoAux.ogro.y;
            jogo->relogio.centesimos = jogoAux.relogio.centesimos;
            jogo->relogio.minutos = jogoAux.relogio.minutos;
            jogo->relogio.segundos = jogoAux.relogio.segundos;
            Carregando();

            fclose(jogo_salvo);

            system("cls");
            printf("  Jogo carregado com sucesso!\n");
            printf("  Pressione qualquer tecla para começar o seu jogo!\n");

            getch();
            system("cls");
            DesenhaJogo(*jogo);
            IniciaJogo(jogo);
        }
        else
        {
            carregado = 0;
        }
    }

    if (carregado == 0)
    {
        MessageBox(0,"Nao foi encontrado nenhum jogo com o seu nome!","Nenhum jogo encontrado!",MB_OK|MB_ICONEXCLAMATION);
        fclose(jogo_salvo);
        printf("Aperte qualquer botão para voltar ao Menu\n");
        getch();
    }
}

void Salvando()
{
    int k, j;

    for (j = 0; j < 2; j++)
    {
        for (k = 0; k < 5; k++)
        {
            gotoxy(2, 2);
            printf("Salvando... %s %s %s %s %s\n", (k >= 0 ? "*":" "), (k >= 1 ? "*":" "), (k >= 2 ? "*":" "), (k >= 3 ? "*":" "), (k >= 4 ? "*":" "));
            Sleep(200);
        }
    }
}

void Carregando()
{
    int k;

    for (k = 0; k < 5; k++)
    {
        gotoxy(2, 2);
        printf("Carregando... %s %s %s %s %s\n", (k >= 0 ? "*":" "), (k >= 1 ? "*":" "), (k >= 2 ? "*":" "), (k >= 3 ? "*":" "), (k >= 4 ? "*":" "));
        Sleep(200);
    }
}

void EscolherJogo(JOGO *jogo)
{
    int quadro;
    char botoes[3][16];

    strcpy(botoes[0], "Jogo facil");
    strcpy(botoes[1], "Jogo dificil");
    strcpy(botoes[2], "Voltar ao menu");

    system("cls");

    quadro = CriaMenuPersonalizado(botoes, 5, 3, 3, 1, 3);

    switch(quadro)
    {
        case 1:
			jogo->num_paredes = 5;
			jogo->num_chaves = 5;
			jogo->num_guardas = 2;
			jogo->nivel = 1;

            jogo->estatisticas.partidas_faceis++;
            jogo->estatisticas.partidas_jogadas++;

			GeraJogoNovo(jogo);
			DesenhaJogo(*jogo);
			IniciaJogo(jogo);
			break;
		case 2:
            jogo->num_paredes = 10;
            jogo->num_chaves = 3;
            jogo->num_guardas = 5;
            jogo->nivel = 2;

            jogo->estatisticas.partidas_dificeis++;
            jogo->estatisticas.partidas_jogadas++;
            GeraJogoNovo(jogo);

            DesenhaJogo(*jogo);
            IniciaJogo(jogo);
			break;
        case 3:
            system("cls");
            break;
    }
}

void Ajuda(JOGO *jogo)
{
    system("cls");
    printf("                                                                                    \n");
    printf("   \"Bem-vindo ao mundo do Resgate o Ogro!                                          \n");
    printf("                                                                                    \n");
    printf("   Aqui, vossa merce encontrara diversos perigos espreitando em todos os can-       \n");
    printf("   tos imaginaveis. Mas nao se acanhe, os bravos de coracao nao terao segundos      \n");
    printf("   pensamentos em enfrenta-los, uma vez que a causa nobre de natureza e. O          \n");
    printf("   bem ha de triunfar perante as mazelas desse mundo doente.                        \n");
    printf("                                                                                    \n");
    printf("   Tua missao simples e, aventurar-se nas terras adjacentes a antiga torre          \n");
    printf("   do sul abandonada, tomando muito cuidado com os guardas do maligno feiti-        \n");
    printf("   ceiro que por la vigiam e ousam zombar das pobres almas que por um acaso         \n");
    printf("   do destino ali vao parar. Mas tu nao, tu seras mais forte e rapido.              \n");
    printf("   Deves encontrar as tres chaves necessarias para abrir a porta da torre e         \n");
    printf("   de la resgatar a pobre criatura ha eras amaldicoada para que possa voltar        \n");
    printf("   e ser nosso legitimo rei, liderando-nos para o futuro prospero e justo que       \n");
    printf("   nos foi usurpado.                                                                \n");
    printf("   Para tal, escute com atenao, indomavel aventureiro. As dicas aqui pre-           \n");
    printf("   sentes ainda hao de salvar sua vida.                                             \n");
    printf("                                                                                    \n");
    printf("   Para movimentar-se, use as setas de seu teclado. Elas e apenas elas irao         \n");
    printf("   te tirar de perigos.                                                             \n");
    printf("   Perigos esses como os guardas, que vao atirar ao menor sinal de presença,        \n");
    printf("   nao importando qualquer obstaculo entre voces e sua maldade ha de triunfar.      \n");
    printf("   Portanto nao ouse aproximar-se demais, ha mais de duas casas de distancia        \n");
    printf("   nao tente chegar. Os guardas nao precisa derrotar, tolos servos do feiti-        \n");
    printf("   ceiro eles sao e sofrimento ja tiveram demais em suas vidas.                     \n");
    printf("   As chaves sao alvo de um encantamento furtivo do vil mago que ha tanto           \n");
    printf("   nos castiga. Mas tu carregas contigo o anel da visibilidade e quando che-        \n");
    printf("   gares a uma casa de distancia de qualquer uma das chaves, ela se revelara        \n");
    printf("   para voce e somente para voce. Nao pestaneje nem tenha outros pensamentos        \n");
    printf("   se nao o de capturar elas.                                                       \n");
    printf("   Quando as tres encontrar, as torres irao ruir e seus obstaculos serao in-        \n");
    printf("   fimos para salvar nosso verdadeiro lider                                         \n");
    printf("                                                                                    \n");
    printf("   Eu, por lastima, nao poderei acompanha-lo em sua empreitada, pois levei          \n");
    printf("   uma flecha no joelho, entao resta-me apenas desejar-te toda a sorte do           \n");
    printf("   do mundo e pedir para os deuses estarem do seu lado.                             \n");
    printf("                                                                                    \n");
    printf("   Boa sorte!\"                                                                     \n");
    printf("\n\n");

    Legenda(jogo->jogador, jogo->configuracoes);

    printf("\n\n");
    printf("   \"A moldura delimita a area em que voce pode andar.                           \n");
    printf("   As paredes sao aleatorias, mas voce nao pode atravessa-las. Mas cuidado,      \n");
    printf("   ela nao ira te salvar dos guardas!                                            \n");
    printf("   A torre, assim como a moldura e as paredes, nao pode ser ultrapassada, a      \n");
    printf("   nao ser que voce tenha conseguido as 3 chaves necessarias.                    \n");
    printf("   O guarda deve ser evitado ao maximo! Se chegar a duas casas de distancia      \n");
    printf("   dele, voce perdera uma vida e sera transportado para uma zona segura para     \n");
    printf("   se recuperar.                                                                 \n");
    printf("   As chaves so aparecem quando voce se aproxima dela, entao seja astuto ao      \n");
    printf("   ao procura-la!                                                                \n");
    printf("   O ogro e o seu objetivo, ao conseguir derrubar as torres, voce deve ir ate    \n");
    printf("   ele para vencer o jogo.                                                       \n");
    printf("                                                                                 \n");
    printf("   A pontuacao sera definida com base no tempo em que voce consegue salvar o     \n");
    printf("   o ogro.\"                                                                     \n");

    getch();
}

void Quadro (int corQuadro, int corDentro, int corTexto, int menorx, int maiorx, int menory, int maiory, char texto[16])
{
    int x, y;

    textbackground(corQuadro);
    for (y = 0; y < 5; y++)
    {
        putchxy(menorx, menory+y, ' ');
        putchxy(maiorx, menory+y, ' ');
    }
    for (x = 0; x < 20; x++)
    {
        putchxy(menorx+x, menory, ' ');
        putchxy(menorx+x, maiory, ' ');
    }

    textbackground(corDentro);
    menorx++;
    maiorx--;
    menory++;
    maiory--;
    for (y = 0; y < 3; y++)
    {
        putchxy(menorx, menory+y, ' ');
        putchxy(maiorx, menory+y, ' ');
    }
    for (x = 0; x < 18; x++)
    {
        putchxy(menorx+x, menory, ' ');
        putchxy(menorx+x, ((menory + maiory) / 2), ' ');
        putchxy(menorx+x, maiory, ' ');
    }

    textcolor(corTexto);

    gotoxy((menorx + ((20 - strlen(texto)) / 2)), (menory+1));
    puts(texto);

    textcolor(WHITE);
    textbackground(BLACK);
}

void Moldura(CONFIGURACOES configuracoes)
{
	int linha, coluna;

	textbackground(configuracoes.cor_moldura);
	for(linha = LINHA1; linha <= ALTURA; linha++)
	{
        putchxy(COLUNA1, linha, ' ');
        putchxy(LARGURA, linha, ' ');
    }
    for(coluna = COLUNA1; coluna <= LARGURA; coluna++)
    {
        putchxy(coluna, LINHA1, ' ');
        putchxy(coluna, ALTURA, ' ');
    }

    textbackground(BLACK);
}

void GeraTorre(JOGO *jogo)
{
    int cmpTorre = 5;
    int linha1, linha2, coluna, i;

    i = 1;
    for (linha1 = 0; linha1 < cmpTorre; linha1++)
    {
        jogo->torre[linha1].x = (LARGURA - i);
        jogo->torre[linha1].y = (ALTURA - 9);
        i++;
    }

    i = 1;
    for (linha2 = linha1; linha2 < (cmpTorre + (linha1)); linha2++)
    {
        jogo->torre[linha2].x = (LARGURA - i);
        jogo->torre[linha2].y = (LINHA1 + 9);
        i++;
    }

    i = 0;
    for (coluna = linha2; coluna < (cmpTorre + linha2 ); coluna++)
    {
        jogo->torre[coluna].x = LARGURA - cmpTorre;
        jogo->torre[coluna].y = LINHA1 + 9 + i;
        i++;
    }

    GeraOgro(&jogo->ogro);
}

void DesenhaTorre(COORDENADA torre[], CONFIGURACOES configuracoes)
{
    int i;

    textbackground(configuracoes.cor_torre);
    for (i = 0; i < TAM_TORRE; i++)
    {
        putchxy(torre[i].x, torre[i].y, ' ');
    }
    textbackground(BLACK);
}

void GeraOgro(COORDENADA *ogro)
{
    int posicaoOgro;

    posicaoOgro = rand() % 2;

    ogro->x = (LARGURA - 1);
    ogro->y = ((ALTURA / 2) + posicaoOgro);
}

void DesenhaOgro(COORDENADA ogro, int cor_ogro)
{
    textbackground(cor_ogro);
    putchxy(ogro.x, ogro.y, 'O');
    textbackground(BLACK);
}

void DesenhaParedes (COORDENADA paredes[], int num_paredes, int cor_parede)
{
    int i;

    textbackground(cor_parede);
    for(i = 0; i < num_paredes * NUM_SEGMENTOS; i++)
    {
        putchxy(paredes[i].x, paredes[i].y, ' ');
    }
    textbackground(BLACK);
}

void GeraParedes (COORDENADA paredes[], int num_paredes)
{
    int direcao;
    int i, j; // Para controlar o laço

    for (i = 0; i < num_paredes * NUM_SEGMENTOS; i = i + NUM_SEGMENTOS)
    {
        paredes[i].x = (NUM_SEGMENTOS + 1) + (rand() % ((LARGURA - 10) - (2 * (NUM_SEGMENTOS+1))));
        paredes[i].y = (NUM_SEGMENTOS + 1) + (rand() % (24 - (2 * (NUM_SEGMENTOS+1))));
        direcao = 1 + (rand() % 4);
        switch (direcao)
        {
            case 1: // Pra cima
                for (j = 1; j <= NUM_SEGMENTOS; j++)
                {
                    paredes[i+j].y = (paredes[i].y) - j;
                    paredes[i+j].x = paredes[i].x;
                }
                break;
            case 2: // Pra baixo
                for (j = 1; j <= NUM_SEGMENTOS; j++)
                {
                    paredes[i+j].y = (paredes[i].y) + j;
                    paredes[i+j].x = paredes[i].x;
                }
                break;
            case 3: // Pra esquerda
                for (j = 1; j <= NUM_SEGMENTOS; j++)
                {
                    paredes[i+j].y = paredes[i].y;
                    paredes[i+j].x = (paredes[i].x) - j;
                }
                break;
            case 4: // Pra direita
                for (j = 1; j <= NUM_SEGMENTOS; j++)
                {
                    paredes[i+j].y = paredes[i].y;
                    paredes[i+j].x  = paredes[i].x  + j;
                }
                break;
        }
    }
    for (;i < TAM_PAREDE; i++)
    {
        paredes[i].x = 0;
        paredes[i].y = 0;
    }
}

void GeraChaves (JOGO *jogo)
{
    int i;
    int laco;

    for (i = 0; i < jogo->num_chaves; i++)
    {
        do
        {
            laco = 0;
            jogo->listaChaves[i].posicao.x = (COLUNA1 + 1) + ((rand() % (LARGURA - COLUNA1 + 1)));
            jogo->listaChaves[i].posicao.y = (LINHA1 + 1) + ((rand() % (ALTURA - LINHA1 + 1)));
            if (TestaColisao(jogo->listaChaves[i].posicao.x, jogo->listaChaves[i].posicao.y, *jogo))
            {
                laco = 1;
            }
        }
        while (laco);
    }

    if (jogo->num_chaves < NUM_CHAVE)
    {
        for (i = jogo->num_chaves; i < NUM_CHAVE; i++)
        {
            jogo->listaChaves[i].posicao.x = -5;
            jogo->listaChaves[i].posicao.y = -5;
        }
    }


    InicializaChaves(jogo->listaChaves, jogo->num_chaves);
}

void InicializaChaves (CHAVE listaChaves[], int num_chaves)
{
    int i;

    for (i = 0; i < num_chaves; i++)
    {
        listaChaves[i].ch = 'C';
        listaChaves[i].status = 1;
    }
}

void DesenhaChave (CHAVE chave, int cor_chave)
{
    textbackground(cor_chave);
    textcolor(BLACK);
    putchxy((chave.posicao.x), (chave.posicao.y), chave.ch);
    textcolor(WHITE);
    textbackground(BLACK);
}

void IniciaJogo(JOGO *jogo)
{
    int ganhou = 0;
    int encurralado = 0;
    clock_t inicio, fim; // Para marcação do tempo da partida
    clock_t inicio_G, fim_G; // Para controle dos movimentos dos guardas
    clock_t inicio_C, fim_C; // Para controle da mensagem de pegar chave
    int feito_C; // Para não ficar atualizando o tempo todo as mensagens de chave pega e de vida perdida.
    char tecla;
    TEMPO relogioGuardas;

    srand((unsigned)time(NULL) * rand());

    inicio = clock();
    inicio_G = clock();
    AreaDeInformacao(*jogo);
    do
	{
	    fim_G = clock();

	    fim = clock();
        jogo->relogio.centesimos = (int) ((fim - inicio) / 10);
        if (jogo->relogio.centesimos >= 100)
        {
            jogo->relogio.segundos++;
            inicio = clock();
            jogo->relogio.centesimos = 0;
        }
        if (jogo->relogio.segundos >= 60)
        {
            jogo->relogio.minutos++;
            jogo->relogio.segundos = 0;
        }
        AtualizaTempo(jogo->relogio);

        relogioGuardas.centesimos = (int) ((fim_G - inicio_G) / 10);
        if (relogioGuardas.centesimos >= 75)
        {
            MoveGuarda(jogo);
            inicio_G = clock();
            relogioGuardas.centesimos = 0;
        }

        TestaChave(jogo);

        if (PegaChave(jogo))
        {
            inicio_C = clock() / 1000;
            gotoxy(25, (ALTURA + 4));
            textbackground(jogo->configuracoes.cor_chave);
            textcolor(BLACK);
            printf("VOCE PEGOU UMA CHAVE!");
            textbackground(BLACK);
            textcolor(WHITE);
            feito_C = 0;
        }
        fim_C = clock() / 1000;
        if ((fim_C - inicio_C) >= 3 && !feito_C)
        {
            gotoxy(25, (ALTURA + 4));
            printf("                     ");
            feito_C = 1;
        }

        MataJogador(jogo, &encurralado);

		if(kbhit())
		{
			tecla = getch();
            MovimentaJogador(jogo, tecla);
            ganhou = GanhaJogo(jogo->jogador, jogo->ogro);
		}

	}
    while(tecla != ESC && jogo->jogador.vida > 0 && !ganhou && !encurralado);

    SalvarEstatistica(jogo->jogador.nome, &jogo->estatisticas);

    if (jogo->jogador.vida <= 0)
    {
        system("cls");

        printf(" __      __                                 _            _          \n");
        printf(" \\ \\    / /                                | |          | |       \n");
        printf("  \\ \\  / /__   ___ ___   _ __   ___ _ __ __| | ___ _   _| |       \n");
        printf("   \\ \\/ / _ \\ / __/ _ \\ | '_ \\ / _ \\ '__/ _` |/ _ \\ | | | |     \n");
        printf("    \\  / (_) | (_|  __/ | |_) |  __/ | | (_| |  __/ |_| |_|        \n");
        printf("     \\/ \\___/ \\___\\___| | .__/ \\___|_|  \\__,_|\\___|\\__,_(_) \n");
        printf("                        | |                                         \n");
        printf("                        |_|                                         \n");
        printf("\n");
        printf("        Pressione qualquer tecla para voltar ao menu\n");

        Sleep(1000);
        fflush(stdin);
        getch();
    }

    if (encurralado)
    {
        system("cls");

        printf(" __      __                                 _            _          \n");
        printf(" \\ \\    / /                                | |          | |       \n");
        printf("  \\ \\  / /__   ___ ___   _ __   ___ _ __ __| | ___ _   _| |       \n");
        printf("   \\ \\/ / _ \\ / __/ _ \\ | '_ \\ / _ \\ '__/ _` |/ _ \\ | | | |     \n");
        printf("    \\  / (_) | (_|  __/ | |_) |  __/ | | (_| |  __/ |_| |_|        \n");
        printf("     \\/ \\___/ \\___\\___| | .__/ \\___|_|  \\__,_|\\___|\\__,_(_) \n");
        printf("                        | |                                         \n");
        printf("                        |_|                                         \n");
        printf("\n");
        printf("        Pressione qualquer tecla para voltar ao menu\n");

        Sleep(1000);
        fflush(stdin);
        getch();
    }

    if (tecla == ESC)
    {
        system("cls");
        printf("Voce pausou o jogo\n");
        printf("Pressione ENTER para voltar ao menu\n");

        getch();
    }
    if (ganhou)
    {
        system("cls");

        printf(" __      __                                _                 _         \n");
        printf(" \\ \\    / /                               | |               | |      \n");
        printf("  \\ \\  / /__   ___ ___    __ _  __ _ _ __ | |__   ___  _   _| |      \n");
        printf("   \\ \\/ / _ \\ / __/ _ \\  / _` |/ _` | '_ \\| '_ \\ / _ \\| | | | | \n");
        printf("    \\  / (_) | (_|  __/ | (_| | (_| | | | | | | | (_) | |_| |_|       \n");
        printf("     \\/ \\___/ \\___\\___|  \\__, |\\__,_|_| |_|_| |_|\\___/ \\__,_(_)\n");
        printf("                          __/ |                                        \n");
        printf("                         |___/                                         \n");
        printf("\n");
        printf("Aperte qualquer tecla para ir ao ranking!\n");

        AlteraEstatisticas(jogo);

        Sleep(1000);
        getch();
        system("cls");
        Ranking(jogo);
    }
}

void AlteraEstatisticas (JOGO *jogo)
{
    int tempo, tempoMedio, tempoRapido, tempoLento;

    /* Aumenta a quantidade de vitórias nas estatísticias */
    jogo->estatisticas.vitorias++;

    /* Cálculo dos tempos com base nesse último jogo */
    tempo = ConverteTempoEmPonto(jogo->relogio); // Converte o tempo em centésimos de segundo para faciliar as operações

    /* Calcula o tempo médio com base no novo tempo */
    tempoMedio = ConverteTempoEmPonto(jogo->estatisticas.tempo_medio);

    tempoMedio = (((tempoMedio * (jogo->estatisticas.vitorias - 1)) + tempo) / jogo->estatisticas.vitorias);

    ConvertePontoEmTempo(tempoMedio, &jogo->estatisticas.tempo_medio);

    /* Verifica se o tempo dessa partida é mais rápido ou mais lento */
    tempoRapido = ConverteTempoEmPonto(jogo->estatisticas.mais_rapido);
    tempoLento = ConverteTempoEmPonto(jogo->estatisticas.mais_lento);

    if (tempoRapido == 0)
    {
        ConvertePontoEmTempo(tempo, &jogo->estatisticas.mais_rapido);
    }
    if (tempo < tempoRapido)
    {
        ConvertePontoEmTempo (tempo, &jogo->estatisticas.mais_rapido);
    }
    else
    {
        if (tempo > tempoLento)
        {
            ConvertePontoEmTempo (tempo, &jogo->estatisticas.mais_lento);
        }
    }

    SalvarEstatistica(jogo->jogador.nome, &jogo->estatisticas);
}

void SalvarEstatistica (char nome[], ESTATISTICAS *estatisticas)
{
    FILE *arq_estatistica;
    ESTATISTICAS estatisticas_aux;
    int localizacaoAtual = 0;
    int salvo = 0;

    arq_estatistica = fopen("estatisticas.bin", "rb+");

    if (arq_estatistica == NULL)
    {
        fclose(arq_estatistica);

        MessageBox(0, "nao pode abrir o arquivo pra salvar", "Otario!", MB_OK|MB_ICONEXCLAMATION);

        arq_estatistica = fopen("estatisticas.bin", "wb");

        fwrite(estatisticas, sizeof(ESTATISTICAS), 1, arq_estatistica);

        fclose(arq_estatistica);
    }
    else
    {
        while (fread(&estatisticas_aux, sizeof(ESTATISTICAS), 1, arq_estatistica))
        {
            if (strcmp(estatisticas_aux.nome, nome) == 0)
            {
                fseek(arq_estatistica, sizeof(ESTATISTICAS) * localizacaoAtual, SEEK_SET);
                if(fwrite(estatisticas, sizeof(ESTATISTICAS), 1, arq_estatistica))
                {
                    salvo = 1;
                }
                else
                {
                    estatisticas->erro = 1;
                }
                fseek(arq_estatistica, 0, SEEK_END);
            }
            else
            {
                localizacaoAtual++;
            }
        }

        if (!salvo)
        {
            MessageBox(0, "Nao foi encontrado no arquivo nada salvo com o nome do jogador!", "Erro na escrita!", MB_OK|MB_ICONEXCLAMATION);
        }

        fclose(arq_estatistica);
    }
}

void AtualizaTempo(TEMPO relogio)
{
    gotoxy(66, (ALTURA+1));
    printf("Tempo: %2d:%2d:%2d", relogio.minutos, relogio.segundos, relogio.centesimos);
}

int GanhaJogo (JOGADOR jogador, COORDENADA ogro)
{
    if (jogador.posicao.x == ogro.x)
    {
        if (jogador.posicao.y == ogro.y)
        {
            return(1);
        }
    }
    return(0);
}

void Legenda (JOGADOR jogador, CONFIGURACOES configuracoes)
{
    printf("                           ||||||||||||||||||||||||||\n");
    printf("                           | Legenda:               |\n");
    printf("                           |                        |\n");
    printf("                           | ");
    textbackground(configuracoes.cor_jogador);
    printf("%c", jogador.ch);
    textbackground(BLACK);
    printf(": Jogador             |\n");
    printf("                           | ");
    textbackground(configuracoes.cor_chave);
    textcolor(BLACK);
    printf("C");
    textcolor(WHITE);
    textbackground(BLACK);
    printf(": Chave               |\n");
    printf("                           | ");
    textbackground(configuracoes.cor_guardas);
    textcolor(BLACK);
    printf("G");
    textcolor(WHITE);
    textbackground(BLACK);
    printf(": Guarda              |\n");
    printf("                           | ");
    textbackground(configuracoes.cor_parede);
    printf(" ");
    textbackground(BLACK);
    printf(": Parede              |\n");
    printf("                           | ");
    textbackground(configuracoes.cor_torre);
    printf(" ");
    textbackground(BLACK);
    printf(": Torre do Ogro       |\n");
    printf("                           | ");
    textbackground(configuracoes.cor_ogro);
    printf("O");
    textbackground(BLACK);
    printf(": Ogro                |\n");
    printf("                           | ");
    textbackground(configuracoes.cor_moldura);
    printf(" ");
    textbackground(BLACK);
    printf(": Moldura             |\n");
    printf("                           | ");
    textbackground(configuracoes.cor_cenario);
    printf(" ");
    textbackground(BLACK);
    printf(": Cenario             |\n");
    printf("                           ||||||||||||||||||||||||||\n");

}

int TestaColisao(int x, int y, JOGO jogo)
{
	int i = 0;
    int colisao = 0;

    if (x == LARGURA || x == COLUNA1 || y == ALTURA || y == LINHA1)
    {
        colisao = 1;
        i = TAM_PAREDE;
    }

    for (i = 0; i < TAM_PAREDE; i++)
    {
        if (y == jogo.paredes[i].y)
        {
            if (x == jogo.paredes[i].x)
            {
                colisao = 1;
                i = TAM_PAREDE;
            }
        }
    }

    if (colisao == 0)
    {
        i = 0;
    }

    if (jogo.jogador.chaves < MIN_CHAVES)
    {
        if (y >= (LINHA1 + 9) && y <= (ALTURA - 9))
        {
            if (x >= (LARGURA - NUM_SEGMENTOS))
            {
                colisao = 1;
            }
        }
    }

    return(colisao);
}

void ApagaTorres(COORDENADA torre[], int cor_cenario)
{
    int i;

    for (i = 0; i < TAM_TORRE; i++)
    {
        ApagaElemento(torre[i].x, torre[i].y, cor_cenario);
    }
}

void MovimentaJogador(JOGO *jogo, char tecla)
{

    if(tecla == SETAS)
   	{
        tecla = getch();
        ApagaElemento(jogo->jogador.posicao.x, jogo->jogador.posicao.y, jogo->configuracoes.cor_cenario);
        switch(tecla)
        {
            case CIMA:
                if(!(TestaColisao(jogo->jogador.posicao.x, (jogo->jogador.posicao.y - 1), *jogo)))
                    (jogo->jogador.posicao.y)--;
                break;
            case BAIXO:
                if(!(TestaColisao(jogo->jogador.posicao.x, (jogo->jogador.posicao.y + 1), *jogo)))
                    (jogo->jogador.posicao.y)++;
                break;
            case DIREITA:
                if(!(TestaColisao((jogo->jogador.posicao.x + 1), jogo->jogador.posicao.y, *jogo)))
                    (jogo->jogador.posicao.x)++;
                break;
            case ESQUERDA:
                if(!(TestaColisao((jogo->jogador.posicao.x - 1), jogo->jogador.posicao.y, *jogo)))
                    (jogo->jogador.posicao.x)--;
                break;
        }
        DesenhaJogador(jogo->jogador, jogo->configuracoes.cor_jogador);
    }
}

void MoveGuarda(JOGO *jogo)
{
    int g;
    int invalido;

    for (g = 0; g < jogo->num_guardas; g++)
    {
        invalido = 1;
        while (invalido)
        {
            switch (jogo->listaGuardas[g].direcao)
            {
                case 0:
                    jogo->listaGuardas[g].destino.x = jogo->listaGuardas[g].posicao.x - 1;
                    jogo->listaGuardas[g].destino.y = jogo->listaGuardas[g].posicao.y + 1;
                    break;
                case 1:
                    jogo->listaGuardas[g].destino.x = jogo->listaGuardas[g].posicao.x;
                    jogo->listaGuardas[g].destino.y = jogo->listaGuardas[g].posicao.y + 1;
                    break;
                case 2:
                    jogo->listaGuardas[g].destino.x = jogo->listaGuardas[g].posicao.x + 1;
                    jogo->listaGuardas[g].destino.y = jogo->listaGuardas[g].posicao.y + 1;
                    break;
                case 3:
                    jogo->listaGuardas[g].destino.x = jogo->listaGuardas[g].posicao.x + 1;
                    jogo->listaGuardas[g].destino.y = jogo->listaGuardas[g].posicao.y;
                    break;
                case 4:
                    jogo->listaGuardas[g].destino.x = jogo->listaGuardas[g].posicao.x + 1;
                    jogo->listaGuardas[g].destino.y = jogo->listaGuardas[g].posicao.y - 1;
                    break;
                case 5:
                    jogo->listaGuardas[g].destino.x = jogo->listaGuardas[g].posicao.x;
                    jogo->listaGuardas[g].destino.y = jogo->listaGuardas[g].posicao.y - 1;
                    break;
                case 6:
                    jogo->listaGuardas[g].destino.x = jogo->listaGuardas[g].posicao.x - 1;
                    jogo->listaGuardas[g].destino.y = jogo->listaGuardas[g].posicao.y - 1;
                    break;
                case 7:
                    jogo->listaGuardas[g].destino.x = jogo->listaGuardas[g].posicao.x - 1;
                    jogo->listaGuardas[g].destino.y = jogo->listaGuardas[g].posicao.y;
                    break;
            }
            if (TestaColisao(jogo->listaGuardas[g].destino.x, jogo->listaGuardas[g].destino.y, *jogo))
            {
                invalido = 1;
                jogo->listaGuardas[g].direcao = (rand() % 8);
            }
            else
            {
                invalido = 0;
                ApagaElemento(jogo->listaGuardas[g].posicao.x, jogo->listaGuardas[g].posicao.y, jogo->configuracoes.cor_cenario);
                jogo->listaGuardas[g].posicao.x = jogo->listaGuardas[g].destino.x;
                jogo->listaGuardas[g].posicao.y = jogo->listaGuardas[g].destino.y;
                jogo->listaGuardas[g].passos--;
                if(jogo->listaGuardas[g].passos == 0)
                {
                    jogo->listaGuardas[g].direcao = (rand() % 8);
                    jogo->listaGuardas[g].passos = (5 + rand() % 12);
                }
            }
        }
    }

    DesenhaGuarda(jogo->listaGuardas, jogo->num_guardas, jogo->configuracoes.cor_guardas);

    /*
    0 - para diagonal inferior esquerda
    1 - para baixo
    2 - para diagonal inferior direita
    3 - para direita
    4 - para diagonal superior direita
    5 - para cima
    6 - para diagonal superior esquerda
    7 - para esquerda
    */
}

void TestaChave(JOGO *jogo)
{
    int i, j, k;

    /*
    As variáveis i, j e k são usadas para verificar os arredores do jogador em busca de chaves.
    A variável i determina qual chave que está sendo procurado;
    A variável j determina a coordenada x do jogador, procurando entre uma casa a esquerda e uma a direita do jogador.
    A variável k determina a ordenada y do jogador, procurando entre uma casa a cima e uma a baixo do jogador.
    Assim, são buscados em uma casa de distância do jogador em busca de alguma chave, se for encontrada e tiver o status
    1 ainda, ou seja, for uma chave invisível, ela será desenhada no mapa.
    */

    for (i = 0; i < jogo->num_chaves; i++)
    {
        for (j = -1; j < 2; j++)
        {
            if ((jogo->jogador.posicao.x + j) == jogo->listaChaves[i].posicao.x)
            {
                for (k = -1; k < 2; k++)
                {
                    if ((jogo->jogador.posicao.y + k == jogo->listaChaves[i].posicao.y))
                    {
                        if (jogo->listaChaves[i].status == 1)
                        {
                            DesenhaChave(jogo->listaChaves[i], jogo->configuracoes.cor_chave);
                            jogo->listaChaves[i].status = 2;
                        }
                    }
                }
            }
        }
    }
}

int PegaChave (JOGO *jogo)
{
    int chavePega = 0;
    int i;

    for (i = 0; i < jogo->num_chaves; i++)
    {
        if (jogo->jogador.posicao.x == jogo->listaChaves[i].posicao.x)
        {
            if (jogo->jogador.posicao.y == jogo->listaChaves[i].posicao.y)
            {
                if (jogo->listaChaves[i].status == 2)
                {
                    jogo->jogador.chaves++;
                    jogo->listaChaves[i].status = 3;
                    AtualizaChaves(jogo->jogador);

                    jogo->estatisticas.chaves_pegas++;

                    chavePega = 1;

                    if (jogo->jogador.chaves >= MIN_CHAVES)
                    {
                        ApagaTorres(jogo->torre, jogo->configuracoes.cor_cenario);
                    }
                }
            }
        }
    }

    return(chavePega);
}

void AtualizaChaves(JOGADOR jogador)
{
    gotoxy(45, (ALTURA+1));
    printf("%d/%d", jogador.chaves, MIN_CHAVES);
}

int TestaGuarda (JOGADOR jogador, int num_guardas, GUARDA listaGuardas[])
{
    int i, j, k;

    /*
    As variáveis i, j e k são usadas para verificar os arredores do jogador em busca de guardas.
    A variável i determina qual o guarda que está sendo procurado;
    A variável j determina a coordenada x do jogador, procurando entre duas casas a esquerda e duas a direita do jogador.
    A variável k determina a ordenada y do jogador, procurando entre duas casas a cima e duas a baixo do jogador.
    Assim, são buscados em duas casas de distância do jogador em busca de algum guarda, se for encontrado, o jogador perde
    uma vida e é desenhado em outro lugar caso ainda tenha vidas, se não o jogo acaba.
    */

    for (i = 0; i < num_guardas; i++)
    {
        for (j = -3; j < 4; j++)
        {
            if ((jogador.posicao.x + j) == listaGuardas[i].posicao.x)
            {
                for (k = -3; k < 4; k++)
                {
                    if ((jogador.posicao.y + k == listaGuardas[i].posicao.y))
                    {
                        return(i + 1);
                    }
                }
            }
        }
    }
    return(0);
}

int MataJogador(JOGO *jogo, int *encurralado)
{
    int vidaPerdida = 0;

    if (TestaGuarda(jogo->jogador, jogo->num_guardas, jogo->listaGuardas))
    {
        jogo->jogador.vida--;
        jogo->estatisticas.mortes++;
        vidaPerdida = 1;
        AtualizaVidas(jogo->jogador);

        if (jogo->jogador.vida > 0)
        {
            char aviso[] = {"VOCE PERDEU UMA VIDA! AGUARDE ENQUANTO TE TIRAMOS DE PERTO DO GUARDA!"};

            gotoxy((LARGURA / 2) - (strlen(aviso) / 2), (ALTURA + 3));

            textbackground(WHITE);
            textcolor(BLACK);

            puts(aviso);

            textbackground(BLACK);
            textcolor(WHITE);

            if (jogo->configuracoes.efeitosGraficos)
            {
                Sangue(*jogo);
            }

            *encurralado = SalvaJogador(jogo);
        }

    }

    return(vidaPerdida);
}

void AtualizaVidas(JOGADOR jogador)
{
    gotoxy(19, (ALTURA+1));
    printf("Vidas: %s %s %s", (jogador.vida > 0 ? "<3":"  "), (jogador.vida > 1 ? "<3":"  "), (jogador.vida > 2 ? "<3":"  "));
}

void GeraGuardas(JOGO *jogo)
{
    int i;

    for (i = 0; i < jogo->num_guardas; i++)
    {
        do
        {
            jogo->listaGuardas[i].posicao.x = ((COLUNA1 + 1) + (rand() % (LARGURA - (COLUNA1 + 1) - NUM_SEGMENTOS)));
            jogo->listaGuardas[i].posicao.y = ((LINHA1 + 1) + (rand() % (ALTURA - (LINHA1 + 1))));

            // Estava acontecendo alguns bugs de alguns guardas nascerem dentro da torre do ogro, o que acabava travando
            // o jogo, como teoricamente isso já não deveria acontecer por causa do teste de Colisão que já prevê essa
            // situação, limitei a coordenada x no número de segmentos (correspondente ao tamanho da torre)
            // e portanto espero que eles não spawnem dentro da torre.
        }
        while (TestaColisao(jogo->listaGuardas[i].posicao.x, jogo->listaGuardas[i].posicao.y, *jogo));
    }

    InicializaGuarda(jogo->listaGuardas, jogo->num_guardas);
}

void DesenhaGuarda(GUARDA listaGuardas[], int num_guardas, int cor_guardas)
{
    int i;

    textbackground(cor_guardas);
    textcolor(BLACK);
    for (i = 0; i < num_guardas; i++)
    {
        putchxy(listaGuardas[i].posicao.x, listaGuardas[i].posicao.y, 'G');
    }
    textcolor(WHITE);
    textbackground(BLACK);
}

void InicializaGuarda (GUARDA listaGuardas[], int num_guardas)
{
    int i;

    for (i = 0; i < num_guardas; i++)
    {
        listaGuardas[i].ch = 'G';
        listaGuardas[i].direcao = (rand() % 8);
        listaGuardas[i].passos = (1 + rand() % 11);
    }
}

int SalvaJogador(JOGO *jogo)
{
    int i, movimento;
    COORDENADA destino;

    i = TestaGuarda(jogo->jogador, jogo->num_guardas, jogo->listaGuardas) - 1;

    for (movimento = 0; movimento < MOVIMENTO_JOGADOR_MORTO; movimento++)
    {
        if(movimento) // Se não houve nenhum movimento ainda, ou seja, se é o primeio movimento do jogador após a morte
        {
            Sleep(500);
        }

        if (jogo->jogador.posicao.x != jogo->listaGuardas[i].posicao.x)
        {
            if (jogo->jogador.posicao.x > jogo->listaGuardas[i].posicao.x)
            {
                destino.x = jogo->jogador.posicao.x + 1;
            }
            else
            {
                destino.x = jogo->jogador.posicao.x - 1;
            }
        }
        else
        {
            destino.x = jogo->jogador.posicao.x;
        }

        if (jogo->jogador.posicao.y != jogo->listaGuardas[i].posicao.y)
        {
            if (jogo->jogador.posicao.y > jogo->listaGuardas[i].posicao.y)
            {
                destino.y = jogo->jogador.posicao.y + 1;
            }
            else
            {
                destino.y = jogo->jogador.posicao.y - 1;
            }
        }
        else
        {
            destino.y = jogo->jogador.posicao.y;
        }

        if (TestaColisao(destino.x, destino.y, *jogo))
        {
            return(1);
        }
        else
        {
            ApagaElemento(jogo->jogador.posicao.x, jogo->jogador.posicao.y, jogo->configuracoes.cor_cenario);
            jogo->jogador.posicao.x = destino.x;
            jogo->jogador.posicao.y = destino.y;

            if (TestaGuarda(jogo->jogador, jogo->num_guardas, jogo->listaGuardas))
            {
                return(1);
            }
            else
            {
                DesenhaJogador(jogo->jogador, jogo->configuracoes.cor_jogador);
            }
        }
    }

    gotoxy(1, (ALTURA + 3));
    printf("                                                                                ");

    return(0);
}

void GeraJogador (JOGO *jogo)
{
    int laco;

    do
    {
        laco = 0;
        jogo->jogador.posicao.x = (COLUNA1+1) + (rand() % ((LARGURA-2) - (COLUNA1+1)));
        jogo->jogador.posicao.y = (LINHA1+1) + (rand() % ((ALTURA-2) - (LINHA1+10)));
        if (TestaColisao(jogo->jogador.posicao.x, jogo->jogador.posicao.y, *jogo))
        {
            laco = 1;
        }
        if (TestaGuarda(jogo->jogador, jogo->num_guardas, jogo->listaGuardas))
        {
            laco = 1;
        }
    }
    while (laco == 1);
}

void InicializaJogador (JOGADOR *jogador)
{
    jogador->ch = jogador->nome[0];
    jogador->chaves = 0;
    jogador->pontos = 0;
    jogador->vida = 3;
}

void DesenhaJogador (JOGADOR jogador, int cor_jogador)
{
    textbackground(cor_jogador);
    putchxy(jogador.posicao.x, jogador.posicao.y, jogador.ch);
    textbackground(BLACK);
}

void ApagaElemento (int x, int y, int cor_cenario)
{
    textbackground(cor_cenario);
    putchxy(x, y, ' ');
}

void Ranking (JOGO *jogo)
{
    FILE *ranking;
    int i;
    int posicoes[(TAM_RAN + 1)], quantObj[(TAM_RAN + 1)];
    TEMPO pontuacao[(TAM_RAN + 1)];
    char nome[(TAM_RAN + 1)][TAM_NOME];

    posicoes[10] = 11;
    pontuacao[10].centesimos = jogo->relogio.centesimos;
    pontuacao[10].segundos = jogo->relogio.segundos;
    pontuacao[10].minutos = jogo->relogio.minutos;
    strcpy(nome[10], jogo->jogador.nome);

    if (jogo->nivel == 1)
    {
        quantObj[10] = jogo->jogador.chaves;

        ranking = fopen("ranking_facil.bin", "rb");
        if (ranking == NULL)
        {
            fclose(ranking);
            MessageBox(0,"O-Oh! Encontramos um problema ao abrir o ranking!","Ranking nao encontrado!",MB_OK|MB_ICONEXCLAMATION);
            printf("\n   Podemos criar um ranking novo para resolver seu problema?\n");
            if (EscolheUmOuDois("Sim", "Nao", 2, 5))
            {
                ranking = fopen("ranking_facil.bin", "wb");
                for (i = 0; i < TAM_RAN; i++)
                {
                    posicoes[i] = (i+1);
                    strcpy(nome[i], "AAAAAAAAAA");
                    pontuacao[i].minutos = 99;
                    pontuacao[i].segundos = 59;
                    pontuacao[i].centesimos = 99;
                    quantObj[i] = 3;
                }

                OrdenaRanking(*jogo, pontuacao, nome, quantObj);

                fwrite(posicoes, sizeof(int), TAM_RAN, ranking);
                fwrite(nome, (sizeof(char) * TAM_NOME), TAM_RAN, ranking);
                fwrite(pontuacao, sizeof(TEMPO), TAM_RAN, ranking);
                fwrite(quantObj, sizeof(int), TAM_RAN, ranking);

                fclose(ranking);

                DesenhaRanking(pontuacao, nome, posicoes, quantObj, "chaves");
            }
        }
        else
        {
            fread(posicoes, sizeof(int), TAM_RAN, ranking);
            fread(nome, (sizeof(char) * TAM_NOME), TAM_RAN, ranking);
            fread(pontuacao, sizeof(TEMPO), TAM_RAN, ranking);
            fread(quantObj, sizeof(int), TAM_RAN, ranking);

            fclose(ranking);

            OrdenaRanking(*jogo, pontuacao, nome, quantObj);

            ranking = fopen("ranking_facil.bin", "wb");

            fwrite(posicoes, sizeof(int), TAM_RAN, ranking);
            fwrite(nome, (sizeof(char) * TAM_NOME), TAM_RAN, ranking);
            fwrite(pontuacao, sizeof(TEMPO), TAM_RAN, ranking);
            fwrite(quantObj, sizeof(int), TAM_RAN, ranking);

            fclose(ranking);

            DesenhaRanking(pontuacao, nome, posicoes, quantObj, "chaves");

            getch();
        }
    }
    else
    {
        quantObj[10] = jogo->jogador.vida;

        ranking = fopen("ranking_dificil.bin", "rb");
        if (ranking == NULL)
        {
            fclose(ranking);
            MessageBox(0, "O-Oh! Encontramos um problema ao abrir o ranking!", "Nao encontramos o ranking!", MB_OK|MB_ICONEXCLAMATION);
            printf("  Podemos criar um ranking novo para resolver seu problema?\n");
            if (EscolheUmOuDois("Sim", "Nao", 2, 3))
            {
                for (i = 0; i < TAM_RAN; i++)
                {
                    posicoes[i] = (i+1);
                    strcpy(nome[i], "AAAAAAAAAA");
                    pontuacao[i].minutos = 99;
                    pontuacao[i].segundos = 59;
                    pontuacao[i].centesimos = 99;
                    quantObj[i] = 0;
                }

                OrdenaRanking(*jogo, pontuacao, nome, quantObj);

                ranking = fopen("ranking_dificil.bin", "wb");

                fwrite(posicoes, sizeof(int), TAM_RAN, ranking);
                fwrite(nome, sizeof(char) * TAM_NOME, TAM_RAN, ranking);
                fwrite(pontuacao, sizeof(TEMPO), TAM_RAN, ranking);
                fwrite(quantObj, sizeof(int), TAM_RAN, ranking);

                fclose(ranking);

                system("cls");
                DesenhaRanking(pontuacao, nome, posicoes, quantObj, "vidas");
            }
            else
            {
                system("cls");
            }
        }
        else
        {
            fread(posicoes, sizeof(int), TAM_RAN, ranking);
            fread(nome, sizeof(char) * TAM_NOME, TAM_RAN, ranking);
            fread(pontuacao, sizeof(TEMPO), TAM_RAN, ranking);
            fread(quantObj, sizeof(int), TAM_RAN, ranking);

            fclose(ranking);

            OrdenaRanking(*jogo, pontuacao, nome, quantObj);

            ranking = fopen("ranking_dificil.bin", "wb");

            fwrite(posicoes, sizeof(int), TAM_RAN, ranking);
            fwrite(nome, sizeof(char) * TAM_NOME, TAM_RAN, ranking);
            fwrite(pontuacao, sizeof(TEMPO), TAM_RAN, ranking);
            fwrite(quantObj, sizeof(int), TAM_RAN, ranking);

            fclose(ranking);

            DesenhaRanking(pontuacao, nome, posicoes, quantObj, "vidas");
        }
    }
}

void DesenhaRanking(TEMPO relogio[], char nomes[TAM_RAN][TAM_NOME], int posicoes[], int quantObj[], char objeto[7])
{
    int i;

    gotoxy(1,1);
    textcolor(BLACK);
    textbackground(YELLOW);
    printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
    printf("||                _____             _    _                            ||\n");
    printf("||               |  __ \\           | |  (_)                           ||\n");
    printf("||               | |__) |__ _ _ __ | | _ _ _ __   __ _                ||\n");
    printf("||               |  _  // _` | '_ \\| |/ / | '_ \\ / _` |               ||\n");
    printf("||               | | \\ \\ (_| | | | |   <| | | | | (_| |               ||\n");
    printf("||               |_|  \\_\\__,_|_| |_|_|\\_\\_|_| |_|\\__, |               ||\n");
    printf("||                                                __/ |               ||\n");
    printf("||                                               |___/                ||\n");
    printf("||                                                                    ||\n");
    printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
    textbackground(WHITE);
    textcolor(BLACK);
    printf("||                                                                    ||\n");
    printf("||                                                                    ||\n");
    for (i = 0; i < TAM_RAN; i++)
    {
        printf("||   #%2d  %10s             %d%7s            %2d:%2d:%2d         ||\n", posicoes[i], nomes[i], quantObj[i], objeto, relogio[i].minutos, relogio[i].segundos, relogio[i].centesimos);
    }
    printf("||                                                                    ||\n");
    printf("||                                                                    ||\n");
    printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
    textcolor(WHITE);
    textbackground(BLACK);

    printf("Pressione qualquer tecla para voltar ao Menu\n");
    getch();
}

void MostraRanking(JOGO *jogo)
{
    FILE *ranking_f, *ranking_d;
    int posicoes_f[TAM_RAN], posicoes_d[TAM_RAN];
    char nomes_f[TAM_RAN][TAM_NOME], nomes_d[TAM_RAN][TAM_NOME];
    TEMPO relogio_f[TAM_RAN], relogio_d[TAM_RAN];
    int erro_f = 0, erro_d = 0, quantRankings;
    int objeto_f[TAM_RAN], objeto_d[TAM_RAN];

    ranking_f = fopen("ranking_facil.bin", "rb");
    if (ranking_f == NULL)
    {
        erro_f = 1;
        fclose(ranking_f);
    }
    ranking_d = fopen("ranking_dificil.bin", "rb");
    if (ranking_d == NULL)
    {
        erro_d = 1;
        fclose(ranking_d);
    }

    system("cls");

    quantRankings = 2 - erro_d - erro_f;

    printf("\n");
    printf("Foram encontrados %d rankings!\n", quantRankings);

    switch(quantRankings)
    {
        case 0:
            printf("\a");
            printf("Sentimos muito por isso\n\n");
            printf("Pressione qualquer tecla para voltar ao menu\n");
            getch();
            break;
        case 1:
            printf("\a");
            if (erro_f)
            {
                printf("So pudemos abrir o ranking do modo dificil!\n");
                printf("Deseja visualiza-lo?\n");
                if (EscolheUmOuDois("Sim", "Nao", 2, 6))
                {
                    system("cls");

                    fread(posicoes_d, sizeof(int), TAM_RAN, ranking_d);
                    fread(nomes_d, sizeof(char) * TAM_NOME, TAM_RAN, ranking_d);
                    fread(relogio_d, sizeof(TEMPO), TAM_RAN, ranking_d);
                    fread(objeto_d, sizeof(int), TAM_RAN, ranking_d);

                    fclose(ranking_d);
                    DesenhaRanking(relogio_d, nomes_d, posicoes_d, objeto_d, "vidas");
                }
                else
                {
                    fclose(ranking_d);
                    system("cls");
                }
            }
            else
            {
                printf("So pudemos abrir o ranking do modo facil!\n");
                printf("Deseja visualiza-lo?\n");
                if (EscolheUmOuDois("Sim", "Nao", 2, 6))
                {
                    system("cls");
                    fread(posicoes_f, sizeof(int), TAM_RAN, ranking_f);
                    fread(nomes_f, sizeof(char) * TAM_NOME, TAM_RAN, ranking_f);
                    fread(relogio_f, sizeof(TEMPO), TAM_RAN, ranking_f);
                    fread(objeto_f, sizeof(int), TAM_RAN, ranking_f);
                    fclose(ranking_f);
                    system("cls");
                    DesenhaRanking(relogio_f, nomes_f, posicoes_f, objeto_f, "chaves");
                }
                else
                {
                    fclose(ranking_f);
                    system("cls");
                }
            }
            break;
        case 2:
            printf("Qual dos rankings deseja abrir?\n");
            printf("Escolha a dificuldade do ranking que deseja abrir.\n");
            if(EscolheUmOuDois("Facil", "Dificil", 2, 6))
            {
                fclose(ranking_d);
                fread(posicoes_f, sizeof(int), TAM_RAN, ranking_f);
                fread(nomes_f, sizeof(char) * TAM_NOME, TAM_RAN, ranking_f);
                fread(relogio_f, sizeof(TEMPO), TAM_RAN, ranking_f);
                fread(objeto_f, sizeof(int), TAM_RAN, ranking_f);
                fclose(ranking_f);
                system("cls");
                DesenhaRanking(relogio_f, nomes_f, posicoes_f, objeto_f, "chaves");
            }
            else
            {
                fclose(ranking_f);

                fread(posicoes_d, sizeof(int), TAM_RAN, ranking_d);
                fread(nomes_d, sizeof(char) * TAM_NOME, TAM_RAN, ranking_d);
                fread(relogio_d, sizeof(TEMPO), TAM_RAN, ranking_d);
                fread(objeto_d, sizeof(int), TAM_RAN, ranking_d);

                fclose(ranking_d);
                system("cls");
                DesenhaRanking(relogio_d, nomes_d, posicoes_d, objeto_d, "vidas");
            }
    }

}

void OrdenaRanking (JOGO jogo, TEMPO pontos[(TAM_RAN + 1)], char nome[][TAM_NOME], int quantObj[])
{
    int trocado, i;
    int pontuacao[(TAM_RAN + 1)];
    float porcentagem = 0.25;

    for (i = 0; i < (TAM_RAN + 1); i++)
    {
        pontuacao[i] = ConverteTempoEmPonto(pontos[i]);
    }

    if (jogo.nivel == 1)
    {
        if (jogo.jogador.chaves == 5)
        {
            pontuacao[10] -= (pontuacao[10] * porcentagem);
        }
    }

    if (jogo.nivel == 2)
    {
        if (jogo.jogador.vida == 3)
        {
            pontuacao[10] -= (pontuacao[10] * porcentagem);
        }
    }

    do
    {
        trocado = 0;
        for (i = 1; i <= TAM_RAN; i++)
        {
            if (pontuacao[(i - 1)] > pontuacao[i])
            {
                TrocaNumeros(&pontuacao[(i - 1)], &pontuacao[i]);
                TrocaNumeros(&quantObj[(i - 1)], &quantObj[i]);
                TrocaNomes(nome[(i - 1)], nome[i]);
                trocado = 1;
            }
        }
    }
    while (trocado);

    for (i = 0; i < TAM_RAN; i++)
    {
        ConvertePontoEmTempo(pontuacao[i], &pontos[i]);
    }
}

int ConverteTempoEmPonto(TEMPO tempo)
{
    int pontos;

    pontos = tempo.centesimos + tempo.segundos * 100 + tempo.minutos * 60 * 100;

    return(pontos);
}

void ConvertePontoEmTempo (int pontuacao, TEMPO *tempo)
{
    tempo->centesimos = pontuacao % 100;
    pontuacao -= tempo->centesimos;
    tempo->segundos = pontuacao / 100;
    tempo->minutos = tempo->segundos / 60;
    pontuacao -= (tempo->minutos * 6000);
    tempo->segundos = (pontuacao / 100);
}

void TrocaNumeros (int *num1, int *num2)
{
    int aux;

    aux = *num1;
    *num1 = *num2;
    *num2 = aux;
}

void TrocaNomes (char nome1[], char nome2[])
{
    char nome[TAM_NOME];

    strcpy(nome, nome1);
    strcpy(nome1, nome2);
    strcpy(nome2, nome);
}

void mostraEstatisticas (ESTATISTICAS estatisticas)
{
    float mediaChaves;
    int percentual;

    system("cls");

    if (estatisticas.partidas_jogadas != 0)
    {
        percentual = (estatisticas.vitorias * 100) / estatisticas.partidas_jogadas;
        mediaChaves = estatisticas.chaves_pegas / (float) estatisticas.partidas_jogadas;
    }
    else
    {
        mediaChaves = 0;
        percentual = 0;
    }

    if(!estatisticas.erro)
    {
        printf("\n");
        printf("  "); textbackground(GREEN); printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n"); textbackground(BLACK);
        printf("  "); textbackground(GREEN); printf("||             _____     _        _   _     _   _                           ||\n"); textbackground(BLACK);
        printf("  "); textbackground(GREEN); printf("||            | ____|___| |_ __ _| |_(_)___| |_(_) ___ __ _ ___             ||\n"); textbackground(BLACK);
        printf("  "); textbackground(GREEN); printf("||            |  _| / __| __/ _` | __| / __| __| |/ __/ _` / __|            ||\n"); textbackground(BLACK);
        printf("  "); textbackground(GREEN); printf("||            | |___\\__ \\ || (_| | |_| \\__ \\ |_| | (_| (_| \\__ \\            ||\n"); textbackground(BLACK);
        printf("  "); textbackground(GREEN); printf("||            |_____|___/\\__\\__,_|\\__|_|___/\\__|_|\\___\\__,_|___/            ||\n"); textbackground(BLACK);
        printf("  "); textbackground(GREEN); printf("||                                                                          ||\n"); textbackground(BLACK);
        printf("  "); textbackground(GREEN); printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n"); textbackground(BLACK);
        textcolor(BLACK);
        printf("  "); textbackground(WHITE); printf("||                                                                          ||\n"); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Partidas jogadas:                                       %8d     ||\n", estatisticas.partidas_jogadas); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Partidas faceis:                                        %8d     ||\n", estatisticas.partidas_faceis); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Partidas dificeis:                                      %8d     ||\n", estatisticas.partidas_dificeis); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Vitorias:                                               %8d     ||\n", estatisticas.vitorias); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Derrotas:                                               %8d     ||\n", (estatisticas.partidas_jogadas - estatisticas.vitorias)); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Percentual de sucesso:                                  %8d     ||\n", percentual); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Tempo medio:                                            %2d:%2d:%2d     ||\n", estatisticas.tempo_medio.minutos, estatisticas.tempo_medio.segundos, estatisticas.tempo_medio.centesimos); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Tempo mais lento:                                       %2d:%2d:%2d     ||\n", estatisticas.mais_lento.minutos, estatisticas.mais_lento.segundos, estatisticas.mais_lento.centesimos); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Tempo mais rapido:                                      %2d:%2d:%2d     ||\n", estatisticas.mais_rapido.minutos, estatisticas.mais_rapido.segundos, estatisticas.mais_rapido.centesimos); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Total de chaves:                                        %8d     ||\n", estatisticas.chaves_pegas); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Media de chaves:                                        %8.1f     ||\n", mediaChaves); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Mortes:                                                 %8d     ||\n", estatisticas.mortes); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||     Modalidade preferida:                                   %8s     ||\n", (estatisticas.partidas_faceis) > estatisticas.partidas_dificeis ? "Facil":"Dificil"); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||                                                                          ||\n"); textbackground(BLACK);
        printf("  "); textbackground(WHITE); printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||");
        textbackground(BLACK);
        textcolor(WHITE);

        putchxy(81, 1, 'L');
    }
    else
    {
        printf("\n\n");
        printf("  Nao foi possivel abrir as estatisticas!\n");
    }

    getch();
}

void Configuracoes(JOGO *jogo)
{
    /* Botões possíveis para configurações: mudar nome, mudar cores */
    char botoes[4][16];
    int botao;

    system("cls");

    strcpy(botoes[0], "Mudar nome");
    strcpy(botoes[1], "Mudar cores");
    strcpy(botoes[2], "Efeitos");
    strcpy(botoes[3], "Voltar ao menu");

    printf("\n   Selecione o que voce quer fazer:");

    botao = CriaMenuPersonalizado(botoes, 5, 4, 4, 2, 2);

    switch(botao)
    {
        case 1:
            system("cls");
            PegaNome(&jogo->jogador);
            break;
        case 2:
            MudaCores(&jogo->configuracoes);
            break;
        case 3:
            AlteraEfeitosGraficos(jogo);
            break;
    }
}

void AlteraEfeitosGraficos(JOGO *jogo)
{
    char botoes[2][16];
    int quadro;

    strcpy(botoes[0], "Alterar");
    strcpy(botoes[1], "Voltar ao menu");

    system("cls");

    printf("\n\n  Status dos efeitos graficos: %s\n", (jogo->configuracoes.efeitosGraficos == 1 ? "Ligado":"Desligado"));

    quadro = CriaMenuPersonalizado(botoes, 5, 7, 2, 1, 2);

    if (quadro == 1)
    {
        if (jogo->configuracoes.efeitosGraficos == 1)
        {
            jogo->configuracoes.efeitosGraficos = 0;
        }
        else
        {
            jogo->configuracoes.efeitosGraficos = 1;
        }
        AlteraEfeitosGraficos(jogo);
    }
}

void MudaCores(CONFIGURACOES *configuracoes)
{
    char objetos[9][16], cores[18][16];
    int quadro1, quadro2;
    int corEscolhida;

    system("cls");

    printf("  Escolha o objeto do jogo que deseja alterar a cor:");

    strcpy(objetos[0], "Jogador");
    strcpy(objetos[1], "Moldura");
    strcpy(objetos[2], "Parede");
    strcpy(objetos[3], "Chave");
    strcpy(objetos[4], "Guardas");
    strcpy(objetos[5], "Torre");
    strcpy(objetos[6], "Ogro");
    strcpy(objetos[7], "Cenario");
    strcpy(objetos[8], "Cancelar");

    strcpy(cores[0], "Preto");
    strcpy(cores[1], "Azul");
    strcpy(cores[2], "Verde");
    strcpy(cores[3], "Ciano");
    strcpy(cores[4], "Vermelho");
    strcpy(cores[5], "Magenta");
    strcpy(cores[6], "Marrom");
    strcpy(cores[7], "Cinza claro");
    strcpy(cores[8], "Cinza escuro");
    strcpy(cores[9], "Azul claro");
    strcpy(cores[10], "Verde claro");
    strcpy(cores[11], "Ciano claro");
    strcpy(cores[12], "Vermelho claro");
    strcpy(cores[13], "Magenta claro");
    strcpy(cores[14], "Amarelo");
    strcpy(cores[15], "Branco");
    strcpy(cores[16], "Voltar");
    strcpy(cores[17], "Cancelar");

    quadro1 = CriaMenuPersonalizado(objetos, 5, 5, 9, 3, 3);

    system("cls");

    if (quadro1 != 9)
    {
        printf("Selecione agora a cor que deseja para ele:");

        quadro2 = CriaMenuPersonalizado(cores, 5, 5, 18, 6, 3);

        system("cls");

        switch(quadro2)
        {
            case 1:
                corEscolhida = BLACK;
                break;
            case 2:
                corEscolhida = BLUE;
                break;
            case 3:
                corEscolhida = GREEN;
                break;
            case 4:
                corEscolhida = CYAN;
                break;
            case 5:
                corEscolhida = RED;
                break;
            case 6:
                corEscolhida = MAGENTA;
                break;
            case 7:
                corEscolhida = BROWN;
                break;
            case 8:
                corEscolhida = LIGHTGRAY;
                break;
            case 9:
                corEscolhida = DARKGRAY;
                break;
            case 10:
                corEscolhida = LIGHTBLUE;
                break;
            case 11:
                corEscolhida = LIGHTGREEN;
                break;
            case 12:
                corEscolhida = LIGHTCYAN;
                break;
            case 13:
                corEscolhida = LIGHTRED;
                break;
            case 14:
                corEscolhida = LIGHTMAGENTA;
                break;
            case 15:
                corEscolhida = YELLOW;
                break;
            case 16:
                corEscolhida = WHITE;
                break;
            case 17:
                MudaCores(configuracoes);
                break;
        }

        switch(quadro1)
        {
            case 1:
                configuracoes->cor_jogador = corEscolhida;
                break;
            case 2:
                configuracoes->cor_moldura = corEscolhida;
                break;
            case 3:
                configuracoes->cor_parede = corEscolhida;
                break;
            case 4:
                configuracoes->cor_chave = corEscolhida;
                break;
            case 5:
                configuracoes->cor_guardas = corEscolhida;
                break;
            case 6:
                configuracoes->cor_torre = corEscolhida;
                break;
            case 7:
                configuracoes->cor_ogro = corEscolhida;
                break;
            case 8:
                configuracoes->cor_cenario = corEscolhida;
                break;
        }
    }
}

int CriaMenuPersonalizado (char botoes[][16], int menorx, int menory, int quantidade_de_botoes, int quantidade_linhas, int quantidade_colunas)
{
    int i, j;
    int duplica_menorx, duplica_menory;
    int botao = 1;
    int v_menorx[quantidade_de_botoes], v_menory[quantidade_de_botoes];
    int menorx_salvo, menory_salvo;
    int sobra;
    char texto_salvo[16];
    char tecla;

    sobra = (quantidade_linhas * quantidade_colunas) - quantidade_de_botoes;

    menorx_salvo = menorx;
    menory_salvo = menory;
    strcpy(texto_salvo, botoes[0]);

    duplica_menorx = menorx;
    duplica_menory = menory;

    for (i = 0; i < quantidade_linhas; i++)
    {
        for (j = 0; j < quantidade_colunas; j++)
        {
            if (botao <= quantidade_de_botoes)
            {
                if (botao == 1)
                {
                    Quadro(COR_QUADRO_SELECIONADO, COR_DENTRO_SELECIONADO, COR_TEXTO_SELECIONADO, menorx, (menorx + 20), menory, (menory + 4), botoes[botao - 1]);
                    v_menorx[(botao - 1)] = menorx;
                    v_menory[(botao - 1)] = menory;
                }
                else
                {
                    Quadro(COR_QUADRO, COR_DENTRO, COR_TEXTO, menorx, (menorx + 20), menory, (menory + 4), botoes[botao - 1]);
                    v_menorx[(botao - 1)] = menorx;
                    v_menory[(botao - 1)] = menory;
                }
                botao++;
                menorx += 25;
            }
        }

        menorx = duplica_menorx;
        menory += 7;
    }

    menorx = duplica_menorx;
    menory = duplica_menory;
    botao = 1;

    do
    {
        tecla = getch();
        if (tecla == SETAS)
        {
            tecla = getch();

            switch(tecla)
            {
                case CIMA:
                    if (botao <= quantidade_colunas)
                    {
                        botao +=  (quantidade_colunas * (quantidade_linhas - 1));
                        if (botao > quantidade_de_botoes)
                        {
                            botao -= quantidade_colunas;
                        }
                    }
                    else
                    {
                        botao -= quantidade_colunas;
                    }
                    break;
                case ESQUERDA:
                    botao--;
                    if (botao == 0)
                    {
                        botao = quantidade_de_botoes;
                    }

                    break;
                case DIREITA:
                    botao++;
                    if (botao > quantidade_de_botoes)
                    {
                        botao = 1;
                    }
                    break;
                case BAIXO:
                    if (botao > (quantidade_linhas - 1) * quantidade_colunas)
                    {
                        botao -= (quantidade_linhas - 1) * quantidade_colunas;
                    }
                    else
                    {
                        botao += quantidade_colunas;
                        if (botao > quantidade_de_botoes)
                        {
                            botao = (quantidade_colunas + 1) - (botao - quantidade_de_botoes);
                        }
                    }
                    break;
            }

            Quadro(COR_QUADRO, COR_DENTRO, COR_TEXTO, menorx_salvo, menorx_salvo + 20, menory_salvo, menory_salvo + 4, texto_salvo);
            Quadro(COR_QUADRO_SELECIONADO, COR_DENTRO_SELECIONADO, COR_TEXTO_SELECIONADO, v_menorx[(botao - 1)], (v_menorx[(botao - 1)] + 20), v_menory[(botao - 1)], (v_menory[(botao - 1)] + 4), botoes[(botao - 1)]);

            menorx_salvo = v_menorx[botao - 1];
            menory_salvo = v_menory[botao - 1];
            strcpy(texto_salvo, botoes[botao - 1]);
        }
    }
    while (tecla != ENTER);

    return(botao);
}

void Sangue (JOGO jogo)
{
    int guarda;
    int x_maior, y_maior;
    int acrescimo_x[3], acrescimo_y[3];

    guarda = (TestaGuarda(jogo.jogador, jogo.num_guardas, jogo.listaGuardas) - 1);

    if (jogo.jogador.posicao.x > jogo.listaGuardas[guarda].posicao.x)
    {
        x_maior = 1;
    }
    else
    {
        if (jogo.jogador.posicao.x < jogo.listaGuardas[guarda].posicao.x)
        {
            x_maior = -1;
        }
        else
        {
            x_maior = 0;
        }
    }

    if (jogo.jogador.posicao.y > jogo.listaGuardas[guarda].posicao.y)
    {
        y_maior = 1;
    }
    else
    {
        if (jogo.jogador.posicao.y < jogo.listaGuardas[guarda].posicao.y)
        {
            y_maior = -1;
        }
        else
        {
            y_maior = 0;
        }
    }

    textbackground(RED);

    switch(x_maior)
    {
        case -1:
            switch(y_maior)
            {
                case -1:
                    acrescimo_x[0] = -1;
                    acrescimo_x[1] = -1;
                    acrescimo_x[2] = 0;

                    acrescimo_y[0] = 0;
                    acrescimo_y[1] = -1;
                    acrescimo_y[2] = -1;
                    break;
                case 0:
                    acrescimo_x[0] = -1;
                    acrescimo_x[1] = -1;
                    acrescimo_x[2] = -1;

                    acrescimo_y[0] = -1;
                    acrescimo_y[1] = 0;
                    acrescimo_y[2] = 1;
                    break;
                case 1:
                    acrescimo_x[0] = -1;
                    acrescimo_x[1] = -1;
                    acrescimo_x[2] = 0;

                    acrescimo_y[0] = 0;
                    acrescimo_y[1] = 1;
                    acrescimo_y[2] = 1;
                    break;
            }
            break;
        case 0:
            switch(y_maior)
            {
                case -1:
                    acrescimo_x[0] = -1;
                    acrescimo_x[1] = 0;
                    acrescimo_x[2] = 1;

                    acrescimo_y[0] = 1;
                    acrescimo_y[1] = 1;
                    acrescimo_y[2] = 1;
                    break;
                case 1:
                    acrescimo_x[0] = -1;
                    acrescimo_x[1] = 0;
                    acrescimo_x[2] = 1;

                    acrescimo_y[0] = -1;
                    acrescimo_y[1] = -1;
                    acrescimo_y[2] = -1;
                    break;
            }
            break;
        case 1:
            switch(y_maior)
            {
                case -1:
                    acrescimo_x[0] = 0;
                    acrescimo_x[1] = 1;
                    acrescimo_x[2] = 1;

                    acrescimo_y[0] = -1;
                    acrescimo_y[1] = -1;
                    acrescimo_y[2] = 0;
                    break;
                case 0:
                    acrescimo_x[0] = 1;
                    acrescimo_x[1] = 1;
                    acrescimo_x[2] = 1;

                    acrescimo_y[0] = -1;
                    acrescimo_y[1] = 0;
                    acrescimo_y[2] = 1;
                    break;
                case 1:
                    acrescimo_x[0] = 0;
                    acrescimo_x[1] = 1;
                    acrescimo_x[2] = 1;

                    acrescimo_y[0] = 1;
                    acrescimo_y[1] = 1;
                    acrescimo_y[2] = 0;
                    break;
            }
            break;
    }

    for (int i = 0; i < 3; i++)
    {
        putchxy(jogo.jogador.posicao.x + acrescimo_x[i], jogo.jogador.posicao.y + acrescimo_y[i], ' ');
    }

    Sleep(2000);

    for (int i = 0; i < 3; i++)
    {
        ApagaElemento(jogo.jogador.posicao.x + acrescimo_x[i], jogo.jogador.posicao.y + acrescimo_y[i], jogo.configuracoes.cor_cenario);
    }
}

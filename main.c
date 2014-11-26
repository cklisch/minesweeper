#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#define TILE 55
#define FIELD_SIZE_X 20
#define FIELD_SIZE_Y 20
#define MINES_NR 20
#define EMPTY 0
#define MINE 9
#define EXP_MINE 10
#define FLAG 11
#define HIDDEN 12
#define SELECT 13
#define SHOW 1


int FIELD_MODEL[FIELD_SIZE_X][FIELD_SIZE_Y];
int FIELD_VIEW[FIELD_SIZE_X][FIELD_SIZE_Y];
SDL_Surface* symbols[13]; /* Datenstruktur für 4 Symbole (Flagge, etc.) */
SDL_Window* window; /* Globaler Zeiger (pointer) zum SDL2 Hauptfenster */
TTF_Font* font; /* Globaler Zeiger zur genutzten Schriftart (font) */


void initialize_view(){
 /* Ausführen der notwendigen Initialisierungsfunktionen */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization error: %s\n", SDL_GetError());
        exit(-1);
    }
    if(TTF_Init() < 0) {
        printf("SDL_TTF initialization error: %s\n", TTF_GetError());
        exit(-1);
    }

    /* Hier wird die Fenstergröße eingestellt */
    window = SDL_CreateWindow("SDL Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, FIELD_SIZE_X*TILE, FIELD_SIZE_Y*TILE+100, 0);
    if(window == NULL) {
        printf("SDL window creation error: %s\n", SDL_GetError());
        exit(-1);
    }

    /* Laden der Bilder */
    symbols[0] = SDL_LoadBMP("icons/empty.bmp");
    if(symbols[0] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[1] = SDL_LoadBMP("icons/1.bmp");
    if(symbols[1] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[2] = SDL_LoadBMP("icons/2.bmp");
    if(symbols[2] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[3] = SDL_LoadBMP("icons/3.bmp");
    if(symbols[3] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[4] = SDL_LoadBMP("icons/4.bmp");
    if(symbols[4] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[5] = SDL_LoadBMP("icons/5.bmp");
    if(symbols[5] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[6] = SDL_LoadBMP("icons/6.bmp");
    if(symbols[6] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[7] = SDL_LoadBMP("icons/7.bmp");
    if(symbols[7] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[8] = SDL_LoadBMP("icons/8.bmp");
    if(symbols[8] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[9] = SDL_LoadBMP("icons/mine.bmp");
    if(symbols[9] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[10] = SDL_LoadBMP("icons/explode.bmp");
    if(symbols[10] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[11] = SDL_LoadBMP("icons/flag.bmp");
    if(symbols[11] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    symbols[12] = SDL_LoadBMP("icons/hidden.bmp");
    if(symbols[12] == NULL) {
        printf("SDL cannot load bitmap: %s\n", SDL_GetError());
        exit(-1);
    }
    /* Laden der Schriftart */
    font = TTF_OpenFont("bloodlust.ttf", 50);
    if(!font) {
        printf("SDL_TTF cannot open font: %s\n", TTF_GetError());
        exit(-1);
    }
}

void initialize_model(void){
    int x,y,i,j,k;

    srand((unsigned) time(NULL));
    for (i=0; i<FIELD_SIZE_X; i++){
        for (j=0; j<FIELD_SIZE_Y; j++) {
            FIELD_MODEL[i][j]=EMPTY;
            FIELD_VIEW[i][j]=HIDDEN;
        }
    }
    for (i=0; i<MINES_NR; i++){
        x=rand()%(FIELD_SIZE_X);
        y=rand()%(FIELD_SIZE_Y);
        if (FIELD_MODEL[x][y]==MINE) {
            i--;
        }
        else {
            FIELD_MODEL[x][y]=MINE;
            for (j=-1; j<=1; j++) {
                for (k=-1; k<=1; k++) {

                    if (x+k>=0 && x+k<FIELD_SIZE_X && y+j>=0 && y+j<FIELD_SIZE_Y && FIELD_MODEL[x+k][y+j] != MINE) {
                        FIELD_MODEL[x+k][y+j]++;
                    }
                }
            }
        }
    }
}

void update_view(int x, int y, int command, int mode){
    int i,j;
    SDL_Rect offset; /* Mit dem Offset (Versatz) wird ein Abstand erzeugt. */
    SDL_Color color_white = {255,255,255,255}; /* Definiere die Farbe weiß */
    SDL_Surface* symbol;
    SDL_Surface* text = NULL;
    SDL_Surface* screen;

    screen = SDL_GetWindowSurface(window);

    if (x<0 || x>=FIELD_SIZE_X || y<0 || y>=FIELD_SIZE_Y) {
        if (mode==SHOW) printf("outside scope\n");
        return;
    }
    if (command==SELECT) {
        FIELD_VIEW[x][y]=FIELD_MODEL[x][y];
        if (FIELD_VIEW[x][y]==EMPTY ) {    // recusive opening of sourounding fields
            for (i=-1; i<=1; i++) {
                for (j=-1; j<=1; j++) {
                    if (i==0 && j==0);
                    else if(FIELD_VIEW[x+i][y+j]!=EMPTY && x+i>=0 && x+i<FIELD_SIZE_X && y+j>=0 && y+j<FIELD_SIZE_Y) update_view(x+i, y+j, SELECT, HIDDEN);
                }
            }
        }
    }
    else if (command==FLAG) {
        FIELD_VIEW[x][y]=FLAG;
    }
    else{
        if (command!=0)printf("wrong command\n");
    }

    if (mode==SHOW) {
        check_win();
        for (i=0; i<FIELD_SIZE_Y; i++) {
            for (j=0; j<FIELD_SIZE_X; j++) {
                offset.x = j*TILE;
                offset.y = i*TILE;

                symbol = symbols[FIELD_VIEW[j][i]];
                SDL_BlitSurface(symbol, NULL, screen, &offset);
            }
        }



        if (check_win()<0){
            text = TTF_RenderText_Solid(font, "Looser!", color_white);

        }
        if (check_win()>0){
            text = TTF_RenderText_Solid(font, "Winner!", color_white);
        }
        offset.x = 0;
        offset.y = TILE*FIELD_SIZE_Y;
        SDL_BlitSurface(text, NULL, screen, &offset);
        SDL_UpdateWindowSurface(window);
    }
}

void copy(){
    int i, j;
    for (i=0; i<FIELD_SIZE_X; i++) {
        for (j=0; j<FIELD_SIZE_Y; j++) {
            FIELD_VIEW[i][j] = FIELD_MODEL[i][j];
        }
    }
}


int check_win(void){
    int i,j,accuracy=0, misses=0, mines=0;
    for (i=0; i<FIELD_SIZE_X; i++) {
        for (j=0; j<FIELD_SIZE_Y; j++) {
            if (FIELD_VIEW[i][j]==FLAG && FIELD_MODEL[i][j]==MINE) {
                accuracy++;
            }
            if (FIELD_VIEW[i][j]==FLAG && FIELD_MODEL[i][j]!=MINE) {
                misses++;
            }
            if (FIELD_MODEL[i][j]==MINE) {
                mines++;
            }
            if (FIELD_VIEW[i][j]==MINE || FIELD_VIEW[i][j]==EXP_MINE) {
                FIELD_MODEL[i][j]=EXP_MINE;
                copy();
                return -1;
            }
        }
    }
    if (accuracy==mines && misses==0) {
        return 1;
    }
    return 0;
}

int position(int input){
    int output;
    for (output = 1; output*TILE<=input; output++);
    return (output - 1);
}

int main(void){
    SDL_Event e;
    char *input;
    int quit = 0,
        command,
        i, x, y = 0;
    input = malloc(4*sizeof(char));
    if (input==NULL) {
        printf("malloc error");
        return 0;
    }
    initialize_model();
    initialize_view();
    update_view(0, 0, 0, SHOW);

    /* MAIN loop - ähnlich wie in Übungsblatt 5 */
    while (!quit) {

        /* Behandle die Benutzer-Events */
        while (SDL_PollEvent(&e)) {
            /*
             * Benutzer möchte das Programm beenden, z.B. durch das Drücken auf
             * das X-Symbol des Fensters.
             */
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                if (e.button.button == SDL_BUTTON_LEFT){
                    command = SELECT;
                }
                else if (e.button.button == SDL_BUTTON_RIGHT){
                    command = FLAG;
                }
                x = position(e.button.x);
                y = position(e.button.y);
                if (y >= FIELD_SIZE_Y){
                    initialize_model();
                    SDL_DestroyWindow(window);
                    main();
                    break;
                }
                update_view(x, y, command, SHOW);

            }
        }
        SDL_Delay (10); /* Eine kurze Wartezeit reduziert die Prozessorlast. */
    }

    for (i = 0; i <= 12 ; i++) {
        SDL_FreeSurface(symbols[i]);
    }

    /* Notwendige Speicherfreigaben für SDL2 */
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 1;
}

#include <stdio.h>
#include <stdlib.h>

#define FIELD_SIZE_X 10
#define FIELD_SIZE_Y 10
#define MINES_NR 2
#define EMPTY 0
#define MINE 9
#define EXP_MINE 10
#define FLAG 11
#define HIDDEN 12
#define SELECT 13
#define SHOW 1

int FIELD_MODEL[FIELD_SIZE_X][FIELD_SIZE_Y];
int FIELD_VIEW[FIELD_SIZE_X][FIELD_SIZE_Y];
char GRAPHIC[] = {' ','1','2','3','4','5','6','7','8','M','X','F','#','S'};

int initialize_model(void){
    int x,y,i,j,k;
    time_t t;
    srand((unsigned) t+4);
    for (i=0; i<FIELD_SIZE_X; i++){
        for (j=0; j<FIELD_SIZE_Y; j++) {
            FIELD_MODEL[i][j]=EMPTY;
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
    
    
    return 1;
}


int initialize_view(void){
    int i,j;
    
    for (i=0; i<FIELD_SIZE_X; i++){
        for (j=0; j<FIELD_SIZE_Y; j++) {
            FIELD_VIEW[i][j]=HIDDEN;
        }
    }
    
    return 1;
}

int update_view(int x, int y, int command, int mode){
    int i,j;
    if (x<0 || x>=FIELD_SIZE_X || y<0 || y>=FIELD_SIZE_Y) {
        if (mode==SHOW) printf("outside scope\n");
        return 0;
    }
    if (command==SELECT) {
        FIELD_VIEW[x][y]=FIELD_MODEL[x][y];
        if (FIELD_VIEW[x][y]==EMPTY ) {
            for (i=-1; i<=1; i++) {
                for (j=-1; j<=1; j++) {
                    if (i==0 && j==0) {
                        
                    }
                    else if(FIELD_VIEW[x+i][y+j]!=EMPTY) update_view(x+i, y+j, SELECT, HIDDEN);
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
        for (i=-1; i<FIELD_SIZE_Y; i++) {
            for (j=-1; j<FIELD_SIZE_X; j++) {
                if (i==-1) {
                    if (j==-1){
                        printf("  ");
                    }
                    else{
                        printf("%2.0d ",j+1); //prints x coordinate
                    }
                }
                else if (j==-1){
                    printf("%c ",'a'+i); //prints y coordinate
                }
                else{
                    printf(" %c ",GRAPHIC[FIELD_VIEW[j][i]]);
                }
                if (j==FIELD_SIZE_X-1) {
                    printf("\n");
                }
            }
        }

    }
    
        return 1;
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
            if (FIELD_VIEW[i][j]==MINE) {
                printf("you selected a mine. you lose!\n");
                return 2;
            }
        }
    }

    if (accuracy==mines && misses==0) {
        printf("you win!\n");
        return 1;
    }
    
    return 0;
}

int main(void){
    
    char *input;
    int command=0,x,y;
    input = malloc(4*sizeof(char));
    if (input==NULL) {
        printf("malloc error");
        return 0;
    }
    initialize_model();
    initialize_view();
    update_view(0, 0, 0, SHOW); printf("\n");
    do{
        printf("enter command: ");
        scanf("%s",input);
        x=input[1]-'1';
        y=input[0]-'a';
        if (input[2]=='s') {
            command=SELECT;
        }
        else if (input[2]=='f') {
            command=FLAG;
        }
        else {
            x=(x+1)*10+(input[2]-'1');
            
            if (input[3]=='s') {
                command=SELECT;
            }
            else if (input[3]=='f') {
                command=FLAG;
            }
        }
        update_view( x, y, command, SHOW); printf("\n");
        if (check_win()>=1) {
            while (input[0]!='q') {
                printf("\n play again y/n : ");
                scanf("%s",input);
                if(input[0]=='y') {
                    main();
                }
                if (input[0]=='n') {
                    return 1;
                }
            }
            
        }
        
    }while (input[0]!='q');
    return 1;
}

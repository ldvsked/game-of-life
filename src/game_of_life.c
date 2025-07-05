#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>

#define N 25
#define M 80

void start_position(int **matrix);
void memory_for_matrix(int ***matrix);
void clean_memory_for_matrix(int ***matrix);

void otrisovka(int **matrix);
void change_position(int **matrix);
int alive(int x,int y, int **matrix);
int check_count_alive(int x, int y, int **matrix);

void change_flag(char input, int *flag);
void client_input(char *input); //то что пользователь ввел
void change_speed(int *delay, char input);

int main(void) {
    int **matrix = NULL;
    memory_for_matrix(&matrix);
    start_position(matrix);
    //initscr();
    if (freopen("/dev/tty", "r", stdin)) initscr();
    
    nodelay(stdscr, TRUE); //неблокирующий режим (не останавливает работу программы пока гетч ждет)
    cbreak(); //оставляем буферизацию строк (все отправляется программе сразу после нажатия без энтер)
    noecho(); //отключаем вывод нажатых строк

    int flag = 0;  //пробел и наличие живых клеток
    char input;
    int delay = 100;
    while (flag == 0) {
        change_position(matrix);
        otrisovka(matrix);
        refresh();
        client_input(&input);
        change_flag(input, &flag);
        napms(delay);//задержка чтобы слишком много выводов не было
    }
    clean_memory_for_matrix(&matrix);
    endwin();
}

void memory_for_matrix(int ***matrix) {
    *matrix = malloc(sizeof(int *) * N);
    int *values_array = malloc(sizeof(int ) * N * M);
    for (int i = 0; i < N; i++) {
        (*matrix)[i] = values_array + i * M;
    }

}

void clean_memory_for_matrix(int ***matrix) {
    if (*matrix != NULL) {
        if ((*matrix)[0] != NULL) {
        free((*matrix)[0]);
        }
        free(*matrix);
    }
    
}

void start_position(int **matrix) {
    char c;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            scanf("%c",&c);
            if (c == '0') {
                matrix[i][j] = 0;
            } else if (c == '1') {
                matrix[i][j] = 1;
            }
        }
        scanf("%c", &c);
    }
}

void otrisovka(int **matrix) {
    clear();
    for (int j = 0; j < M + 2; j ++) {
        printw("_");
    }
    printw("\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M + 2; j++) {
            if (j == 0 || j == M + 1) {
                printw("|");
            } else if ( matrix[i][j - 1] == 1) {
                printw("O");
            } else {
                printw(" ");
            } 
        }
        printw("\n");
    }
    for (int j = 0; j < M + 2; j ++) {
        printw("_");
    }
    printw("\n");
}

void client_input(char *input) {
    int ch = getch();
    if (ch != ERR) {
        *input = (char)ch;
    }
}

void change_flag(char input, int *flag) {
    if (input == ' ') {
        *flag = 1;
    }
}

void change_speed(int *delay, char input) {
    if (input == 'A' && *delay - 100 >= 100) {
        *delay -= 500;
    }
    if (input == 'Z') {
        *delay += 500;
    }
}

int alive(int x,int y, int **matrix){
    int check = 0;
    x = (x + N) % N;
    y = (y + M) % M;

    if (matrix[x][y] == 1) {
        check = 1;
    }
    
    return check;
}

int check_count_alive(int x, int y, int **matrix){
    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;  
            if (alive(x + dx, y + dy, matrix)) {
                count++;
            }
        }
    }
    return count;
}

void change_position(int **matrix){
    int **matrix_memory = NULL;
    memory_for_matrix(&matrix_memory);

    for (int i = 0;i<N;i++){
        for(int j = 0;j<M;j++){
            int count = check_count_alive(i, j,matrix);
            if ((count<2 || count>3) && matrix[i][j]==1){
                matrix_memory[i][j] = 0;
            }
            if (count == 3 && matrix[i][j]==0){
                matrix_memory[i][j]=1;
            }
            if ((count==2 || count == 3)&&matrix[i][j]==1){
                matrix_memory[i][j] = 1;
            }
        }
    }
    for (int i = 0;i<N;i++){
        for(int j = 0;j<M;j++){
            matrix[i][j] = matrix_memory[i][j];
        }
    }
    clean_memory_for_matrix(&matrix_memory);

}

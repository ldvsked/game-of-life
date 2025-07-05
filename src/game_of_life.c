#include <stdio.h>
#include <ncurses.h>
#define N 25
#define M 80

void start_position(int **matrix);
void memory_for_matrix(int ***matrix);
void clean_memory_for_matrix(int ****matrix);
void otrisovka(int **matrix);
void change_position(int **matrix);
void change_flag(int **matrix, char input, int *flag);
void client_input(char *input); //то что пользователь ввел

int main(void) {
    initscr();
    int **matrix = NULL;
    memory_for_matrix(&matrix);
    start_position(matrix);
    otrisovka(matrix);
    int flag = 0;  //пробел и наличие живых клеток
    char input;
    while (flag == 0) {
        change_position(matrix);
        otrisovka(matrix);
        refresh();
        client_input(&input);
        change_flag(matrix, input, &flag);
    }
    
    endwin();
}

void memory_for_matrix(int ***matrix) {
    *matrix = malloc(sizeof(int *) * N);
    int values_array = malloc(sizeof(int ) * N * M);
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
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            char input = getchar();
            if (input != '\n') {
                matrix[i][j] = input;
            }
        }
    }
}

void otrisovka(int **matrix) {
    for (int i = 0; i < N + 2; i++) {
        for (int j = 0; j < M + 2; j++) {
            if (j == 0 || j == M + 1)  {
                printw("|");
            }
        }
    }
}
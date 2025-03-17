#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>



#define COLORS " bgcrmyw-BGCRMYW"
#define MAX_SNAKE_LEN 100
#define SNAKE_COLOR 'B'
#define FOOD_COLOR 'G'

struct winsize w;
int WIDTH, HEIGHT;
COORD snake[MAX_SNAKE_LEN];
int snake_size;
COORD food;
enum {
    L,
    R,
    U,
    D
} direction;
typedef char Char[3];
HANDLE handle;
COORD ScreenSize, coord_zero = {0, 0};
char *(*(*BOARD));  //TODO interactive board size
CONSOLE_SCREEN_BUFFER_INFO info;
int gameOver = 0;
void consoleColor(char fgc, char bgc) {
    int fg,bg;
    for(fg=0;COLORS[fg] != fgc;fg++);
    for(bg=0;COLORS[bg] != bgc;bg++);
    SetConsoleTextAttribute(handle, fg+bg*16);
}

void clearBoard(Char ch) {
    for(int x = 0;x < WIDTH;x++)
        for(int y = 0;y < HEIGHT;y++)
            for(int a = 0; a < 3;a++)
                BOARD[x][y][a] = ch[a];
}
void clearBoardCh(char ch, char fg, char bg) {
    Char spec = {ch, fg, bg};
    clearBoard(spec);
}
void setPixel(int x, int y, char col) {
    int ydx = y / 2;
    int yelt = 1-(y % 2);
    BOARD[x][ydx][0] = (char)220;
    BOARD[x][ydx][1 + yelt] = col;
}
void displayBoard() {
    //FillConsoleOutputCharacter(handle, ' ', ScreenSize.X*ScreenSize.Y,coord_zero, 0);
    SetConsoleCursorPosition(handle, coord_zero);
    consoleColor('w', ' ');
    printf("%c", 201);
    for(int i = 0;i < WIDTH;i++) {
        printf("%c", 205);
    }
    printf("%c\n", 187);
    int n_spaces = (WIDTH - 10)/2;
    printf("%c", 186);
    for(int i = 0; i < n_spaces;i++) printf("%c", ' ');
    printf("Snake Game");
    if(n_spaces * 2 < WIDTH - 10) n_spaces++;
    for(int i = 0; i < n_spaces;i++) printf("%c", ' ');
    printf("%c\n", 186);
    printf("%c", 199);
    for(int i = 0;i < WIDTH;i++) {
        printf("%c", 196);
    }
    printf("%c\n", 182);
    for(int y = 0;y < HEIGHT;y++) {
        consoleColor('w', ' ');
        printf("%c", 186);
        for(int x = 0;x < WIDTH;x++) {
            consoleColor(BOARD[x][y][1], BOARD[x][y][2]);
            printf("%c", BOARD[x][y][0]);
        }
        consoleColor('w', ' ');
        printf("%c\n", 186);

    }
    consoleColor('w', ' ');
    printf("%c", 200);
    for(int i = 0;i < WIDTH;i++) {
        printf("%c", 205);
    }
    printf("%c\n", 188);

}
void drawSnakeNFood() {
    for(int i = 0; i < snake_size; i++) {
        setPixel(snake[i].X, snake[i].Y, SNAKE_COLOR);
    }
    setPixel(food.X, food.Y, FOOD_COLOR);
}
void moveSnake() {
    for(int i = snake_size - 1; i > 0; i--) {
            snake[i].X = snake[i - 1].X;
            snake[i].Y = snake[i - 1].Y;
        }
    switch(direction) {
    case U:
        if(snake[0].Y > 0)
            snake[0].Y -= 1;
        break;
    case D:
        if(snake[0].Y < HEIGHT - 1)
            snake[0].Y += 1;
        break;
    case L:
        if(snake[0].X > 0)
            snake[0].X -= 1;
        break;
    case R:
        if(snake[0].X < WIDTH - 1)
            snake[0].X += 1;
        break;
    }
}
void place_food() {
    food.X = rand() % WIDTH;
    food.Y = rand() % HEIGHT;
}
int main()
{
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    snake[0].X = 2;
    snake[0].Y = 2;
    snake[1].X = 1;
    snake[1].Y = 2;
    place_food();
    direction = R;
    snake_size = 2;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(handle, &info);
    ScreenSize.X = info.dwSize.X;
    ScreenSize.Y = info.dwSize.Y;

    clearBoardCh((char)220, ' ',' ');
    drawSnakeNFood();
    displayBoard();

    while(1) {
        Sleep(200);
        while(kbhit()) {
            char ch = getch();
            if((int)ch == -32) {
                ch = getch();
                consoleColor('W', ' ');
                switch(ch) {
                case 'H':
                    direction = U;
                    break;
                case 'K':
                    direction = L;
                    break;
                case 'M':
                    direction = R;
                    break;
                case 'P':
                    direction = D;
                    break;
                }
            };

        }
        if(gameOver) {

        }
        moveSnake();
        if(snake[0].X == food.X && snake[0].Y == food.Y) {
            snake[snake_size].X = snake[snake_size - 1].X;
            snake[snake_size].Y = snake[snake_size - 1].Y;
            snake_size++;
            place_food();
        }
        clearBoardCh((char)220, ' ',' ');
        drawSnakeNFood();
        displayBoard();
    }

    return 0;
}

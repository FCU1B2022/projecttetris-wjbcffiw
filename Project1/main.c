#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

#define LEFT_KEY 0x25     // The key to move left, default = 0x25 (left arrow)
#define RIGHT_KEY 0x27    // The key to move right, default = 0x27 (right arrow)
#define ROTATE_KEY 0x26   // The key to rotate, default = 0x26 (up arrow)
#define DOWN_KEY 0x28     // The key to move down, default = 0x28 (down arrow)
#define FALL_KEY 0x20     // The key to fall, default = 0x20 (spacebar)
#define E_KEY 0x45
#define W_KEY 0x57
#define A_KEY 0x41
#define S_KEY 0x53
#define D_KEY 0x44
#define SHIFT_KEY 0x10

#define FALL_DELAY 500    // The delay between each fall, default = 500
#define RENDER_DELAY 100  // The delay between each frame, default = 100

#define LEFT_FUNC() GetAsyncKeyState(LEFT_KEY) & 0x8000
#define RIGHT_FUNC() GetAsyncKeyState(RIGHT_KEY) & 0x8000
#define ROTATE_FUNC() GetAsyncKeyState(ROTATE_KEY) & 0x8000
#define DOWN_FUNC() GetAsyncKeyState(DOWN_KEY) & 0x8000
#define FALL_FUNC() GetAsyncKeyState(FALL_KEY) & 0x8000
#define E_FUNC() GetAsyncKeyState(E_KEY) & 0x8000
#define W_FUNC() GetAsyncKeyState(W_KEY) & 0x8000
#define A_FUNC() GetAsyncKeyState(A_KEY) & 0x8000
#define S_FUNC() GetAsyncKeyState(S_KEY) & 0x8000
#define D_FUNC() GetAsyncKeyState(D_KEY) & 0x8000
#define SHIFT_FUNC() GetAsyncKeyState(SHIFT_KEY) & 0x8000

#define CANVAS_WIDTH 10
#define CANVAS_HEIGHT 20

typedef enum {
    RED = 41,
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,
    CYAN,
    WHITE,
    BLACK = 0,
}Color;

typedef enum {
    EMPTY = -1,
    I,
    J,
    L,
    O,
    S,
    T,
    Z
}ShapeId;

typedef struct {
    ShapeId shape;
    Color color;
    int size;
    char rotates[4][4][4];
}Shape;

typedef struct
{
    int x;
    int y;
    int score;
    int rotate;
    int fallTime;
    int arrow_place_x;
    int arrow_place_y;
    int choose_xy_arrow;  //0 is x, 1 is y 
    int E_status;
    int E_coldtime;
    ShapeId queue[4];
}State;

typedef struct {
    Color color;
    ShapeId shape;
    bool current;
}Block;

Shape shapes[7] = {
    {
        .shape = I,
        .color = CYAN,
        .size = 4,
        .rotates =
        {
            {
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0}
            },
            {
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0}
            },
            {
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0}
            }
        }
    },
    {
        .shape = J,
        .color = BLUE,
        .size = 3,
        .rotates =
        {
            {
                {1, 0, 0},
                {1, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 1, 1},
                {0, 1, 0},
                {0, 1, 0}
            },
            {
                {0, 0, 0},
                {1, 1, 1},
                {0, 0, 1}
            },
            {
                {0, 1, 0},
                {0, 1, 0},
                {1, 1, 0}
            }
        }
    },
    {
        .shape = L,
        .color = YELLOW,
        .size = 3,
        .rotates =
        {
            {
                {0, 0, 1},
                {1, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 1, 0},
                {0, 1, 0},
                {0, 1, 1}
            },
            {
                {0, 0, 0},
                {1, 1, 1},
                {1, 0, 0}
            },
            {
                {1, 1, 0},
                {0, 1, 0},
                {0, 1, 0}
            }
        }
    },
    {
        .shape = O,
        .color = WHITE,
        .size = 2,
        .rotates =
        {
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            }
        }
    },
    {
        .shape = S,
        .color = GREEN,
        .size = 3,
        .rotates =
        {
            {
                {0, 1, 1},
                {1, 1, 0},
                {0, 0, 0}
            },
            {
                {0, 1, 0},
                {0, 1, 1},
                {0, 0, 1}
            },
            {
                {0, 0, 0},
                {0, 1, 1},
                {1, 1, 0}
            },
            {
                {1, 0, 0},
                {1, 1, 0},
                {0, 1, 0}
            }
        }
    },
    {
        .shape = T,
        .color = PURPLE,
        .size = 3,
        .rotates =
        {
            {
                {0, 1, 0},
                {1, 1, 1},
                {0, 0, 0}
            },

                {{0, 1, 0},
                {0, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 0, 0},
                {1, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 1, 0},
                {1, 1, 0},
                {0, 1, 0}
            }
        }
    },
    {
        .shape = Z,
        .color = RED,
        .size = 3,
        .rotates =
        {
            {
                {1, 1, 0},
                {0, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 0, 1},
                {0, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 0, 0},
                {1, 1, 0},
                {0, 1, 1}
            },
            {
                {0, 1, 0},
                {1, 1, 0},
                {1, 0, 0}
            }
        }
    },
};

void setBlock(Block* block, Color color, ShapeId shape, bool current)
{
    block->color = color;
    block->shape = shape;
    block->current = current;
}

void resetBlock(Block* block)
{
    block->color = BLACK;
    block->shape = EMPTY;
    block->current = false;
}

bool move(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int originalX, int originalY, int originalRotate, int newX, int newY, int newRotate, ShapeId shapeId) {
    Shape shapeData = shapes[shapeId];
    int size = shapeData.size;

    // check if the new position is valid to place the block
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (shapeData.rotates[newRotate][i][j]) {
                if (newX + j < 0 || newX + j >= CANVAS_WIDTH || newY + i < 0 || newY + i >= CANVAS_HEIGHT) {
                    return false;
                }
                if (!canvas[newY + i][newX + j].current && canvas[newY + i][newX + j].shape != EMPTY) {
                    return false;
                }
            }
        }
    }

    // remove the old position
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (shapeData.rotates[originalRotate][i][j]) {
                resetBlock(&canvas[originalY + i][originalX + j]);
            }
        }
    }

    // move the block
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (shapeData.rotates[newRotate][i][j]) {
                setBlock(&canvas[newY + i][newX + j], shapeData.color, shapeId, true);
            }
        }
    }

    return true;
}

void printCanvas(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH], State* state)
{
    printf("\033[0;0H\n");
    printf("   ");
    for (int i = 0; i < CANVAS_WIDTH+1; i++) {
        if (state->E_status == 1 && state->choose_xy_arrow == 1) {
            if (i == state->arrow_place_y) {
                printf("|");
            }
            else {
                printf("  ");
            }
        }
        else {
            printf("  ");
        }
    }
    printf("\n");
    printf("   ");
    for (int i = 0; i < CANVAS_WIDTH+1; i++) {
        if (state->E_status == 1 && state->choose_xy_arrow == 1) {
            if (i == state->arrow_place_y) {
                printf("v");
            }
            else {
                printf("  ");
            }
        }
        else {
            printf("  ");
        }
    }
    printf("\n");

    for (int i = 0; i < CANVAS_HEIGHT; i++) {

        if (state->E_status == 1 && state->choose_xy_arrow==0) {
            if (i == state->arrow_place_x) {
                printf("-->");
            }
            else {
                printf("   ");
            }
        }
        else {
            printf("   ");
        }
        
        printf("|");
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            printf("\033[%dm\u3000", canvas[i][j].color);
        }
        printf("\033[0m|\n");
    }
    printf("\n");
    
    if (state->E_coldtime>0) {
        printf("    ");
        printf("\033[31m (E) cold time : %d      \n\033[0m",state->E_coldtime);
        printf("                               ");
    }
    else if (state->E_coldtime==0 && state->E_status==0) {
        
        printf("        ");
        printf("(E) is ready   \n");
        printf("                               ");
    }
    else if (state->E_status==1) {
        printf("  ");
        printf("Use WASD to move the arrow\n");
        printf("  ");
        printf("Press (LFT shift) to confirm\n");
    }

    Shape shapeData = shapes[state->queue[1]];
    printf("\033[%d;%dHNext:", 3, CANVAS_WIDTH * 2 + 8);
    for (int i = 1; i <= 3; i++)
    {
        shapeData = shapes[state->queue[i]];
        for (int j = 0; j < 4; j++) {
            printf("\033[%d;%dH", i * 4 + j, CANVAS_WIDTH * 2 + 15);
            for (int k = 0; k < 4; k++) {
                if (j < shapeData.size && k < shapeData.size && shapeData.rotates[0][j][k]) {
                    printf("\x1b[%dm  ", shapeData.color);
                }
                else {
                    printf("\x1b[0m  ");
                }
            }
        }
    }
    return;
}

int clearLine(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH]) {
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            if (canvas[i][j].current) {
                canvas[i][j].current = false;
            }
        }
    }

    int linesCleared = 0;

    for (int i = CANVAS_HEIGHT - 1; i >= 0; i--)
    {
        bool isFull = true;
        for (int j = 0; j < CANVAS_WIDTH; j++)
        {
            if (canvas[i][j].shape == EMPTY) {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            linesCleared += 1;

            for (int j = i; j > 0; j--)
            {
                for (int k = 0; k < CANVAS_WIDTH; k++)
                {
                    setBlock(&canvas[j][k], canvas[j - 1][k].color, canvas[j - 1][k].shape, false);
                    resetBlock(&canvas[j - 1][k]);
                }
            }
            i++;
        }
    }


    return linesCleared;
}

void logic(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH], State* state)
{
    if (ROTATE_FUNC()) {
        int newRotate = (state->rotate + 1) % 4;
        if (move(canvas, state->x, state->y, state->rotate, state->x, state->y, newRotate, state->queue[0]))
        {
            state->rotate = newRotate;
        }
    }
    else if (LEFT_FUNC()) {
        if (move(canvas, state->x, state->y, state->rotate, state->x - 1, state->y, state->rotate, state->queue[0]))
        {
            if (state->E_status!=1) {
                state->x -= 1;
            }
        }
    }
    else if (RIGHT_FUNC()) {
        if (move(canvas, state->x, state->y, state->rotate, state->x + 1, state->y, state->rotate, state->queue[0]))
        {
            if (state->E_status != 1) {
                state->x += 1;
            }
        }
    }
    else if (DOWN_FUNC()) {
        state->fallTime = FALL_DELAY;
    }
    else if (FALL_FUNC()) {
        state->fallTime += FALL_DELAY * CANVAS_HEIGHT;
    }
    else if (E_FUNC()) {
        state->fallTime += FALL_DELAY * CANVAS_HEIGHT;
        if (state->E_status == 1) {
            state->E_status = 0;
        }
        else if (state->E_status == 0) {
            state->E_status = 1;
        }
    }
    else if (W_FUNC()) {
        state->choose_xy_arrow = 0;
        if (state->arrow_place_x>0) {
            state->arrow_place_x--;
        }
    }
    else if (A_FUNC()) {
        state->choose_xy_arrow = 1;
        if (state->arrow_place_y > 1) {
            state->arrow_place_y--;
        }
    }
    else if (S_FUNC()) {
        state->choose_xy_arrow = 0;
        if (state->arrow_place_x < CANVAS_HEIGHT-1) {
            state->arrow_place_x++;
        }
    }
    else if (D_FUNC()) {
        state->choose_xy_arrow = 1;
        if (state->arrow_place_y <  CANVAS_WIDTH) {
            state->arrow_place_y++;
        }
    }
    else if (SHIFT_FUNC()) {
        if (state->E_status == 1) {
            if (state->choose_xy_arrow == 0) {
                for (int j = state->arrow_place_x; j > 0; j--)
                {
                    for (int k = 0; k < CANVAS_WIDTH; k++)
                    {
                        setBlock(&canvas[j][k], canvas[j - 1][k].color, canvas[j - 1][k].shape, false);
                        resetBlock(&canvas[j - 1][k]);
                    }
                }
                
            }
            else {
                for (int i = 0; i < CANVAS_HEIGHT; i++) {
                    resetBlock(&canvas[i][state->arrow_place_y - 1]);
                }
                
                
            }
            state->E_status = 0;
            state->E_coldtime = 5;
            state->arrow_place_x = 0;
            state->arrow_place_y = 1;
            state->choose_xy_arrow = 0;
        }
    }
    

    state->fallTime += RENDER_DELAY;

    while (state->fallTime >= FALL_DELAY) {
        state->fallTime -= FALL_DELAY;
        

        if (move(canvas, state->x, state->y, state->rotate, state->x, state->y + 1, state->rotate, state->queue[0])) {
            state->y++;
        }
        else {
            if (state->E_status!=1) {
                state->score += clearLine(canvas);

                state->x = CANVAS_WIDTH / 2;
                state->y = 0;
                state->rotate = 0;
                state->fallTime = 0;
                state->queue[0] = state->queue[1];
                state->queue[1] = state->queue[2];
                state->queue[2] = state->queue[3];
                state->queue[3] = rand() % 7;
                state->E_coldtime--;
            }

            if (!move(canvas, state->x, state->y, state->rotate, state->x, state->y, state->rotate, state->queue[0]))
            {
                printf("\033[%d;%dH\x1b[41m GAME OVER \x1b[0m\033[%d;%dH", CANVAS_HEIGHT - 3, CANVAS_WIDTH * 2 + 5, CANVAS_HEIGHT + 5, 0);
                exit(0);
            }
        }
    }
    return;
}


int main()
{
    srand(time(NULL));
    State state = {
        .x = CANVAS_WIDTH / 2,
        .y = 0,
        .score = 0,
        .rotate = 0,
        .fallTime = 0
    };
    state.arrow_place_x = 0;
    state.arrow_place_y = 0;
    state.choose_xy_arrow = 0;
    state.E_status = 0;
    state.E_coldtime = 5;

    for (int i = 0; i < 4; i++)
    {
        state.queue[i] = rand() % 7;
    }

    Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
    for (int i = 0; i < CANVAS_HEIGHT; i++)
    {
        for (int j = 0; j < CANVAS_WIDTH; j++)
        {
            resetBlock(&canvas[i][j]);
        }
    }

    system("cls");
    // printf("\e[?25l"); // hide cursor

    move(canvas, state.x, state.y, state.rotate, state.x, state.y, state.rotate, state.queue[0]);

    while (1)
    {
        printCanvas(canvas, &state);
        logic(canvas, &state);
        
        Sleep(100);
    }

}

/***************************************************************************************
*    Title: campo minato
*    Author: Luca B.
*    Date: 20/12/23
*    Code version: C++11
*
***************************************************************************************/


#ifndef CAMPO_MINATO_H
#define CAMPO_MINATO_H

#include <string.h>
#include <conio.h>
#include <iostream>
#include ".\settings_manager.h"

using namespace std;

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

#define FIND_BOMBS_BOX_DIAMETER 3


struct campo_minato_styles{
    char index[0];
    char grass = '_';
    char bomb = '+';
    char mark = '#';
    char blank = ' ';
    char cursor = '*';
};

struct campo_minato_keywords{
    char index[0];
    char forward = 'w';
    char backward = 's';
    char left = 'a';
    char right = 'd';
    char mark = ' ';
    char dig = 13;
};

struct campo_minato_margin{
    char index[0];
    char top_left = '/';
    char top_right = '\\';
    char bottom_left = '\\';
    char bottom_right = '/';
    char top = '_';
    char bottom = '_';
    char left = '|';
    char right = '|';
};


const char campo_minato_margin_names[sizeof(campo_minato_margin)][ELEMENT_NAME_LENGTH]{
    "top_left",
    "top_right",
    "bottom_left",
    "bottom_right",
    "top",
    "bottom",
    "left",
    "right"
};

const char campo_minato_styles_names[sizeof(campo_minato_styles)][ELEMENT_NAME_LENGTH] = {
    "Grass",
    "Bomb",
    "Mark",
    "Blank",
    "Cursor"
};

const char campo_minato_keywords_names[sizeof(campo_minato_keywords)][ELEMENT_NAME_LENGTH]{
    "Forward",
    "Backward",
    "Left",
    "Right",
    "Mark",
    "Dig"
};

class campo_minato{
    public:

        int start_game();

        void setup_keywords(campo_minato_keywords custom_keywords);
        void setup_styles(campo_minato_styles custom_styles);
        void setup_margins(campo_minato_margin custom_margins);

        campo_minato(int width, int height, int bomb_percent);

    private:
        campo_minato_styles feild_styles;
        campo_minato_keywords keywords;
        campo_minato_margin margin;

        int feild_width;
        int feild_height;
        int feild_bomb_percent;

        int tot_bomb_placed = 0;

        char feild[MAX_HEIGHT][MAX_WIDTH];
        char bomb[MAX_HEIGHT][MAX_WIDTH];

        inline void init_feild();
        inline void init_bomb();

        void render(char matrix[MAX_HEIGHT][MAX_WIDTH]);
        void render_with_cursor(char matrix[MAX_HEIGHT][MAX_WIDTH], int x_pos, int y_pos);

        int find_around_bombs(int x_pos, int y_pos);
        int dig_the_feild(int x_pos, int y_pos);

        int toggle_mark(int x_pos, int y_pos);    
};

//Init setup
//width: used for the width of the game table
//height: used for the height of the game table
//bomb_percent: used for the percent of the bombs on the table
campo_minato::campo_minato(int width, int height, int bomb_percent){
    feild_width = width;
    feild_height = height;

    feild_bomb_percent = bomb_percent;
}


//########################
//         SETUP
//########################

//Update the keywords
//custom_keywords: used for updateing the keywords
void campo_minato::setup_keywords(campo_minato_keywords custom_keywords){
    keywords = custom_keywords;
}

//Update the general styles
//custom_styles: used for updateing the general styles
void campo_minato::setup_styles(campo_minato_styles custom_styles){
    feild_styles = custom_styles;
}

//Update the margins
//custom_margin: used for updateing the margins
void campo_minato::setup_margins(campo_minato_margin custom_margin){
    margin = custom_margin;
}


//########################
//          INIT
//########################

//Init the feild with grass characters
//No args
void campo_minato::init_feild(){
    for (int h = 0; h < feild_height; h++){
        for (int w = 0; w < feild_width; w++){
            feild[h][w] = feild_styles.grass;
        }
    }    
}

//Init the bomb feild by placing in random position bombs character
//percent: it indicates the percentage of bombs placed in the bomb feild
void campo_minato::init_bomb(){
    for (int h = 0; h < feild_height; h++){
        for (int w = 0; w < feild_width; w++){
            if(rand() % 100 <= feild_bomb_percent){
                bomb[h][w] = feild_styles.bomb;
                tot_bomb_placed++;
            }else{
                bomb[h][w] = feild_styles.blank;
            }
        }
    }
}


//########################
//        RENDER
//########################

//Print on the console a matrix
//matrix: used for printnig the original matrix
void campo_minato::render(char matrix[MAX_HEIGHT][MAX_WIDTH]){
    render_with_cursor(matrix, -1, -1);
}

//Print on the console a matrix and a cursor
//matrix: used for printnig the original matrix
//x_pos and y_pos: used for the position of the cursor on the matrix
void campo_minato::render_with_cursor(char matrix[MAX_HEIGHT][MAX_WIDTH], int x_pos, int y_pos){
    printf("%c", margin.top_left);
    for(int i = 0; i < feild_width; i++){
        printf("%c", margin.top);
    }
    printf("%c\n", margin.top_right);

    for (int h = 0; h < feild_height; h++){
        printf("%c", margin.left);

        for (int w = 0; w < feild_width; w++){
            if(h == y_pos && w == x_pos){
                printf("%c", feild_styles.cursor);
            }else{
                printf("%c", matrix[h][w]);
            }
        }

        printf("%c", margin.right);

        printf("\n");
    }

    printf("%c", margin.bottom_left);
    for(int i = 0; i < feild_width; i++){
        printf("%c", margin.bottom);
    }
    printf("%c\n", margin.bottom_right);
}


//########################
//       BOMB DIG
//########################

//Search in the square of FIND_BOMBS_BOX_DIAMETER from the center any bomb
//x_pos and y_pos: used for the center of the box
//return: returns the number of bombs in the square
int campo_minato::find_around_bombs(int x_pos, int y_pos){
    int around_bombs = 0;

    int x_pos_0 = x_pos;
    int y_pos_0 = y_pos;
    
    int width = FIND_BOMBS_BOX_DIAMETER;
    int height = FIND_BOMBS_BOX_DIAMETER;

    if(x_pos > 0 && x_pos < feild_width){
        x_pos_0 -= (FIND_BOMBS_BOX_DIAMETER - 1) / 2;
    }else{
        width = FIND_BOMBS_BOX_DIAMETER - (FIND_BOMBS_BOX_DIAMETER - 1) / 2;
    }
 
    if(y_pos > 0 && y_pos < feild_height){
        y_pos_0 -= (FIND_BOMBS_BOX_DIAMETER - 1) / 2;
    }else{
        height = FIND_BOMBS_BOX_DIAMETER - (FIND_BOMBS_BOX_DIAMETER - 1) / 2;
    }

    for(int h = 0; h < height; h++){
        for (int w = 0; w < width; w++){
            if (bomb[y_pos_0 + h][x_pos_0 + w] == feild_styles.bomb){
                around_bombs++;
            }
        } 
    }

    return around_bombs;
}

//It search bombs until it does not found any black box
//x_pos and y_pos: used for the starting point of the search
//return: returns -1 it cach a bomb or 0 if it dosen't
int campo_minato::dig_the_feild(int x_pos, int y_pos){
    if(bomb[y_pos][x_pos] == feild_styles.bomb){
       return -1; 
    }    

    int around_bombs = find_around_bombs(x_pos, y_pos); 

    if(around_bombs == 0){
        feild[y_pos][x_pos] = feild_styles.blank;

        int x_pos_0 = x_pos;
        int y_pos_0 = y_pos;
        
        int width = FIND_BOMBS_BOX_DIAMETER;
        int height = FIND_BOMBS_BOX_DIAMETER;

        if(x_pos > 0 && x_pos < feild_width){
            x_pos_0 -= (FIND_BOMBS_BOX_DIAMETER - 1) / 2;
        }
    
        if(y_pos > 0 && y_pos < feild_height){
            y_pos_0 -= (FIND_BOMBS_BOX_DIAMETER - 1) / 2;
        }

        for(int h = 0; h < FIND_BOMBS_BOX_DIAMETER; h++){
            for (int w = 0; w < FIND_BOMBS_BOX_DIAMETER; w++){
                if(feild[h + y_pos_0][w + x_pos_0] == feild_styles.grass){//skip the blank box
                    dig_the_feild(w + x_pos_0, h + y_pos_0);
                }
            } 
        }
    }else{
        feild[y_pos][x_pos] = around_bombs + '0';//conversion from int to char number from 0 to 9
    }

    return 0;
}


//########################
//         MARKS
//########################

//Toggle a mark
//x_pos and y_pos: used for the position of the mark to toggle on the feild
//return: returns 1 if the mark ha been added or -1 if removed
int campo_minato::toggle_mark(int x_pos, int y_pos){
    if(feild[y_pos][x_pos] == feild_styles.mark){
        feild[y_pos][x_pos] = feild_styles.grass;

        return -1;
    }else if(feild[y_pos][x_pos] == feild_styles.grass){
        feild[y_pos][x_pos] = feild_styles.mark;

        return 1;
    }

    return 0;
}


//########################
//          GAME
//########################

//Run the game
//return: returns 1 if the player has win or -1 if lost
int campo_minato::start_game(){
    init_feild();
    init_bomb();

    int lose = 0;
    int win = 0;

    int x = 0;
    int y = 0;

    int correct_marked_bombs = 0;

    render_with_cursor(feild, x, y);

    while (!lose && !win){
        while (!kbhit());

        char key = getch();  

        int valid = 0;

        if(key == keywords.forward){
            if(y == 0){
                y = feild_height - 1;
            }else{
                y--;
            }

            valid = 1;
        }else if(key == keywords.left){
            if(x == 0){
                x = feild_width - 1;
            }else{
                x--;
            }

            valid = 1;
        }else  if(key == keywords.backward){
            if(y == feild_height - 1){
                y = 0;
            }else{
                y++;
            }

            valid = 1;
        }else if(key == keywords.right){
            if(x == feild_width - 1){
                x = 0;
            }else{
                x++;
            }

            valid = 1;
        }else if(key == keywords.mark){
            
            int placeing = toggle_mark(x,y);

            if(bomb[y][x] == feild_styles.bomb){
                correct_marked_bombs += placeing;
            }   

            if(correct_marked_bombs == tot_bomb_placed){
                win = 1;
            }

            valid = 1;
        }else if(key == keywords.dig){
            if(feild[y][x] == feild_styles.mark){
                toggle_mark(x,y);
            }else{
                lose = dig_the_feild(x,y);
            }

            valid = 1;
        }

        if(valid){
            system("cls"); 
            render_with_cursor(feild, x, y);
        }
        
    }

    render(bomb);

    if(win){
        return 1;
    }else{
        return -1;
    }
    
}

const char str_credits[4][22] = {
    "3  *   _  +  _  **   ",
    ". +  Campo Minato _ #",
    "  By Luca Briguglio .",
    " 1 _    #  5   .  +__"
};

extern void campo_minato_credits(){
    for (int i = 0; i < 4; i++){
        printf("%s\n", str_credits[i]);
    }
}

#endif
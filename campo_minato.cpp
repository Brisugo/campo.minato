#include <iostream>
#include <string.h>
#include <conio.h>
#include <Windows.h>

#include ".\campo_minato.h"
#include ".\settings_manager.h"

using namespace std;

int go_to_home_page = 0;

campo_minato_styles my_style;
campo_minato_keywords my_keywords;
campo_minato_margin my_margin;

int width = 0;
int height = 0;

int bomb_percent = 0;    

void start_a_game(){
    if(width != 0 && height != 0 && bomb_percent != 0){
        campo_minato new_game(width, height, bomb_percent);

        new_game.setup_styles(my_style);
        new_game.setup_keywords(my_keywords);
        new_game.setup_margins(my_margin);

        system("cls");

        int status = new_game.start_game();    


        if (status == 1){
            printf("Congratulations. You win!!\n"); 
        }else{
            printf("You lose. Try again.\n");
        }
        
        printf("Press 'c' to continue...");

        while (getch() != 'c');

        system("cls");

        go_to_home_page = 1;
    }else{
        system("cls");
        printf("Wrong values");
        Sleep(3*1000);
    }
}

void easy_mode(){
    width = 10;
    height = 10;

    bomb_percent = 10;

    start_a_game();
}

void medium_mode(){
    width = 20;
    height = 20;

    bomb_percent = 20;

    start_a_game();
}

void hard_mode(){
    width = 25;
    height = 25;

    bomb_percent = 25;

    start_a_game();
}


int main(){
    //settings set;

    Page main_page("Main");
        Page custom_styles_page("Styles", &default_go_back);

            Page custom_general_styles_page("General", &default_go_back);
            element var_general_styles[sizeof(campo_minato_styles)] = {};
            custom_general_styles_page.append(sizeof(campo_minato_styles), var_general_styles);


            Page custom_margin_styles_page("Margin", &default_go_back);
            element var_margin_styles[sizeof(campo_minato_margin)] = {};
            custom_margin_styles_page.append(sizeof(campo_minato_margin), var_margin_styles);

        custom_styles_page.append(2,
            custom_general_styles_page.as_page_element(),
            custom_margin_styles_page.as_page_element()
        );

        Page custom_keywords_page("Keywords", &default_go_back);
        element var_keywords[sizeof(campo_minato_keywords)] = {};
        custom_keywords_page.append(sizeof(campo_minato_keywords), var_keywords);


        Page game_setup_page("Play", &default_go_back);
        element game_setup[3] = {};

            Page custom_game_page("Custom", &default_go_back);
            element custom_game_settings[4] = {};
            custom_game_page.append(sizeof(custom_game_settings) / sizeof(element), custom_game_settings);
        
        game_setup_page.append(sizeof(game_setup) / sizeof(element), game_setup);
        game_setup_page.append(custom_game_page.as_page_element());

    main_page.append(3,
        custom_styles_page.as_page_element(), 
        custom_keywords_page.as_page_element(),
        game_setup_page.as_page_element()
    );



    for (int i = 0; i < sizeof(campo_minato_styles); i++){
        var_general_styles[i].type = Element_Types::Variable_Char;

        strcpy(var_general_styles[i].name, campo_minato_styles_names[i]);

        var_general_styles[i].ptr = &my_style.index[i];
    }

    for (int i = 0; i < sizeof(campo_minato_margin); i++){
        var_margin_styles[i].type = Element_Types::Variable_Char;

        strcpy(var_margin_styles[i].name, campo_minato_margin_names[i]);

        var_margin_styles[i].ptr = &my_margin.index[i];
    }

    for (int i = 0; i < sizeof(campo_minato_keywords); i++){
        var_keywords[i].type = Element_Types::Variable_Char;
        
        strcpy(var_keywords[i].name, campo_minato_keywords_names[i]);

        var_keywords[i].ptr = &my_keywords.index[i];
    }

    
    custom_game_settings[0] = (element){Element_Types::Variable_Int, "Width", &width};
    custom_game_settings[1] = (element){Element_Types::Variable_Int, "Height", &height};
    custom_game_settings[2] = (element){Element_Types::Variable_Int, "Bomb Percent", &bomb_percent};
    custom_game_settings[3] = (element){Element_Types::Button, "Start the game!", (void*)&start_a_game};

    game_setup[0] = (element){Element_Types::Button, "Easy", (void*)&easy_mode};
    game_setup[1] = (element){Element_Types::Button, "Medium", (void*)&medium_mode};
    game_setup[2] = (element){Element_Types::Button, "Hard", (void*)&hard_mode};


    campo_minato_credits();
    main_page.print();
    
    while(1){
        while (!kbhit());
        
        int el = getch() - '0';

        if(el >= 0 && el <= main_page.get_length()){
            main_page.interact(el);

            if(go_to_home_page){
                main_page.goto_parent();
                go_to_home_page = 0;
            }

            system("cls");

            campo_minato_credits();
            main_page.print();
        }
    }
    
    return 0;
}


// Page main_page;
//         Page custom_styles_page;
//             Page custom_general_styles_page;
//             Page custom_margin_styles_page;
//         Page custom_keywords_page;

//         Page game_setup_page;
//             Page custom_game_page;

//     custom_styles_page.append(&default_go_back);
//         custom_general_styles_page.append(&default_go_back);
//         custom_margin_styles_page.append(&default_go_back);

//     custom_keywords_page.append(&default_go_back);

//     game_setup_page.append(&default_go_back);
//         custom_game_page.append(&default_go_back);

//     // Main
//     element custom_styles = {Element_Types::Page, "Styles", (void*)&custom_styles_page};
//     element custom_keywords = {Element_Types::Page, "Keywords", (void*)&custom_keywords_page};
//     element play = {Element_Types::Page, "Play", (void*)&game_setup_page};

//     // custom_general_styles && custom_margin_styles_page && custom_keywords
//     element var_general_styles[sizeof(campo_minato_styles) + 1] = {};
//     element var_margin_styles[sizeof(campo_minato_margin) + 1] = {};
//     element var_keywords[sizeof(campo_minato_keywords) + 1] = {};

//     for (int i = 0; i < sizeof(campo_minato_styles); i++){
//         var_general_styles[i].type = Element_Types::Variable_Char;

//         strcpy(var_general_styles[i].name, campo_minato_styles_names[i]);

//         var_general_styles[i].ptr = &my_style.index[i];
//     }

//     for (int i = 0; i < sizeof(campo_minato_styles); i++){
//         var_general_styles[i].type = Element_Types::Variable_Char;

//         strcpy(var_general_styles[i].name, campo_minato_styles_names[i]);

//         var_general_styles[i].ptr = &my_style.index[i];
//     }

//     for (int i = 0; i < sizeof(campo_minato_keywords); i++){
//         var_keywords[i].type = Element_Types::Variable_Char;
        
//         strcpy(var_keywords[i].name, campo_minato_keywords_names[i]);

//         var_keywords[i].ptr = &my_keywords.index[i];
//     }

//     // game_setup
//     element game_setup[4] = {
//         {Element_Types::Button, "Easy", (void*)&easy_mode},
//         {Element_Types::Button, "Medium", (void*)&medium_mode},
//         {Element_Types::Button, "Hard", (void*)&hard_mode},
//         {Element_Types::Page, "Custom", (void*)&custom_game_page},
//     };

//     // custom_game
//     element custom_game_settings[4] = {
//         {Element_Types::Variable_Int, "Width", &width},
//         {Element_Types::Variable_Int, "Height", &height},
//         {Element_Types::Variable_Int, "Bomb Percent", &bomb_percent},
//         {Element_Types::Button, "Start the game!", (void*)&start_a_game},
//     };

//     // Main
//     main_page.append(3, custom_styles, custom_keywords, play);

//     // custom_styles && custom_keywords
//     custom_styles_page.append(sizeof(campo_minato_styles), var_general_styles);
//     custom_keywords_page.append(sizeof(campo_minato_keywords), var_keywords);

//     // game_setup
//     game_setup_page.append(sizeof(game_setup) / sizeof(element), game_setup);

//     // custom_game_page 
//     custom_game_page.append(sizeof(custom_game_settings) / sizeof(element), custom_game_settings);


//     campo_minato_credits();
//     main_page.print();
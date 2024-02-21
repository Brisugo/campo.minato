/***************************************************************************************
*    Title: settings manager
*    Author: Luca B.
*    Date: 20/12/23
*    Code version: C++11
*
***************************************************************************************/


#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <iostream>
#include <string.h>
#include <cstdarg>

using namespace std;

enum class Element_Types : char{Variable_Int = 'i', Variable_Float = 'f', Variable_Char = 'c', Variable_String = 's', Page = 'p', Button = 'b', Go_Back = 'g'};

#define PAGE_ELEMENT_LENGTH 10
#define ELEMENT_NAME_LENGTH 30

struct element{
    Element_Types type;
    char name[ELEMENT_NAME_LENGTH];

    void *ptr;
};

element default_go_back = {Element_Types::Go_Back, "Back", 0};

class Page{
    public:
        // Setup
        Page(char name[ELEMENT_NAME_LENGTH]){ strcpy(as_page.name, name); }
        Page(char name[ELEMENT_NAME_LENGTH], element *new_el) { 
            strcpy(as_page.name, name); 
            append(new_el); 
        }

        void append(element *new_el);
        void append(int el_length, element *elements);
        void append(int el_length, ...);

        // Page functions

        void print(){
            if(current_page == this){
                current_page->__self_print__();
            }else{
                current_page->print();
            }
        }

        void interact(int el){
            if(current_page == this){
                current_page->__self_interact__(el);
            }else{
                current_page->interact(el);
            }
        }
    
        void goto_parent(){
            current_page->__goto_parent__();
        }


        // Self functions

        void __self_print__();

        void __self_interact__(int el);

        void __goto_parent__();


        //Other functions

        int get_length(){
            if(current_page == this){
                return current_page->length;
            }else{
                return current_page->get_length();
            }
        }

        element *as_page_element(){return &as_page; }

    private:
        element *vars_ptr[PAGE_ELEMENT_LENGTH];
        int length = 0;

        Page *current_page = this;
        Page *parent_page = this;

        element as_page = {Element_Types::Page, " ",(void*)this};
        void change_var(int el);
};


// ############
//    Setup
// ############

void Page::append(element *new_el){
    if(length < PAGE_ELEMENT_LENGTH){
        vars_ptr[length] = new_el;
        length++;
    }
}

void Page::append(int el_length, element *elements){
    for (int i = 0; i < el_length; i++){
       append(&elements[i]);
    }
}

void Page::append(int el_length, ...){
    int index = 0;

    va_list args;
    va_start(args, index);

    for (int i = 0; i < el_length; i++){
        append(va_arg(args, element*));
    }

    va_end(args);
}

// ############
//  __self__
// ############


void Page::__self_print__(){
    for (int i = 0; i < length; i++){
        printf("%d) %s", i, vars_ptr[i]->name);

        void *this_var_ptr = vars_ptr[i]->ptr;
        Element_Types type = vars_ptr[i]->type;

        if(type == Element_Types::Variable_Int){
            printf(": ");

            int *var = (int*)this_var_ptr;
            printf("%d", *var);

        }else if(type == Element_Types::Variable_Float){
            printf(": ");

            float *var = (float*)this_var_ptr;
            printf("%f", *var);

        }else if(type == Element_Types::Variable_Char){
            printf(": ");

            char *var = (char*)this_var_ptr;
            printf("%c", *var);

        }else if(type == Element_Types::Variable_String){
            printf(": ");

            char *var = (char*)this_var_ptr;
            printf("%s", var);

        }

        printf("\n");
    }
}

void Page::__self_interact__(int el){
    Element_Types el_type = vars_ptr[el]->type;

    if(el_type == Element_Types::Variable_Int || el_type == Element_Types::Variable_Float || el_type == Element_Types::Variable_Char || el_type == Element_Types::Variable_String){

        change_var(el);

    }else if(el_type == Element_Types::Button){
        void (*func_to_call)(); 

        func_to_call = (void(*)())vars_ptr[el]->ptr;

        func_to_call();
    }else if(el_type == Element_Types::Page){
        Page *page_ptr = (Page*)vars_ptr[el]->ptr;

        page_ptr->parent_page = current_page;

        current_page = page_ptr;

    }else if(el_type == Element_Types::Go_Back){

        current_page->goto_parent();

    }
}

void Page::__goto_parent__(){
    parent_page->current_page = parent_page;
}



// ############
//   Private
// ############


void Page::change_var(int el){
    fflush(stdin);

    void *this_var_ptr = vars_ptr[el]->ptr;
    Element_Types type = vars_ptr[el]->type;

    printf("Insert the new value: ");

    if(type == Element_Types::Variable_Int){

        int *var = (int*)this_var_ptr;
        scanf("%d", &*var);

    }else if(type == Element_Types::Variable_Float){

        float *var = (float*)this_var_ptr;
        scanf("%f", &*var);

    }else if(type == Element_Types::Variable_Char){

        char *var = (char*)this_var_ptr;
        scanf("%c", &*var);

    }else if(type == Element_Types::Variable_String){

        char *var = (char*)this_var_ptr;
        gets(var);

    }
}

#endif
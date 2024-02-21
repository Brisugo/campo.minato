#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <iostream>
#include <string.h>

using namespace std;

//folder
    //folder
    //page
        //variable
        //variable
    //page
        //variable
        //button
#define SETTINGS_VARIABLE_NAME_LENGTH 10
#define SETTINGS_PAGE_NAME_LENGTH 10
#define SETTINGS_PAGE_VARIABLES 20
#define SETTINGS_PAGE_PAGES 10
#define SETTINGS_PAGE_CURSOR "<"

#define SETTINGS_INDENT_STRING "  "

void indent(int indentation){
    for (int spaces = 0; spaces < indentation; spaces++){
        printf(SETTINGS_INDENT_STRING);
    }
}

class settings{

    public:        

        class variable{

            public:
                class assign{
                    public:
                        virtual void print_var(){
                            printf("undefined");
                        }
                };

                struct type{

                    class Integer : public assign{
                        public:
                            Integer(int *var){
                                var_ptr = var;
                            }

                            void print_var(){
                                printf("%d", *var_ptr);
                            }

                        private:
                            int *var_ptr;
                    };

                    class Float : public assign{
                        public:
                            Float(float *var){
                                var_ptr = var;
                            }

                            void print_var(){
                                printf("%f", *var_ptr);
                            }

                        private:
                            float *var_ptr;
                    };

                    class Char : public assign{
                        public:
                            Char(char *var){
                                var_ptr = var;
                            }

                            void print_var(){
                                printf("%c", *var_ptr);
                            }

                        private:
                            char *var_ptr;
                    };

                    class String : public assign{
                        public:
                            String(char *var){
                                var_ptr = var;
                            }

                            void print_var(){
                                printf("%s", var_ptr);
                            }

                        private:
                            char *var_ptr;
                    };

                };

                variable(assign *var, char name[SETTINGS_VARIABLE_NAME_LENGTH]){
                    strcpy(var_name, name);

                    int len = strlen(name);

                    memset(var_name + len, ' ', (SETTINGS_VARIABLE_NAME_LENGTH - len) - 1);

                    var_name[SETTINGS_VARIABLE_NAME_LENGTH - 1] = '\0';


                    var_ptr = var;
                }

                void print(){
                    printf("%s", var_name);

                    printf(" ");

                    var_ptr->print_var();
                }


                private:
                    assign *var_ptr;
                    char var_name[SETTINGS_VARIABLE_NAME_LENGTH] = "undefined";
        };

        class page{
            public:
                page(char name[SETTINGS_PAGE_NAME_LENGTH]){
                    strcpy(page_name, name);
                }


                void append(variable *var){
                    vars[vars_length] = var;
                    vars_length++;
                }

                void print(int indentation){
                    printf("%s", page_name);

                    printf("\n");

                    for (int i = 0; i < vars_length; i++){
                        indent(indentation);

                        vars[i]->print();

                        if(cursor == i){
                            printf(SETTINGS_PAGE_CURSOR);
                        }
                        printf("\n");
                    }
                }

                void cursor_set(int cursor_value){
                    cursor = cursor_value;
                }

                void cursor_next(){
                    cursor++;
                    if(cursor >= vars_length){
                        cursor = 0;
                    }
                }

                void cursor_previus(){
                    cursor--;
                    if(cursor < 0){
                        cursor = vars_length - 1;
                    }
                }

                void cursor_this(){
                    
                }

            private:
                int cursor = -1;

                int vars_length = 0;
                variable *vars[SETTINGS_PAGE_VARIABLES];

                char page_name[SETTINGS_PAGE_NAME_LENGTH] = "undefined";
        };

        class folder{
            public:

        }        
};

/*
classe astratta Dynamic {
    public:
        String format();
}

class Integer(Dynamic) {
    int value;

    String format() {
        return "${value}"
    }
}

class Data(Dynamic) {
    uint8_t giorno;
    uint8_t mese;
    uint16_t anno;

    void aggiuniAnni(uint16_t x);

    String format() {
        return "${anno}/${mese}/${giorno}"
    }
}


Dynamic valori[] = {new Integer(10), new Data(1928373)};

for (Dynamic val : valori) {
    val.format();
}*/

#endif
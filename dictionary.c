#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct definition definition;
typedef struct firstborn_letter_1 firstborn_letter_1;
typedef struct firstborn_letter_2 firstborn_letter_2;
typedef struct firstborn_word firstborn_word;
typedef struct layer_1 layer_1;
typedef struct layer_2 layer_2;
typedef struct layer_3 layer_3;
typedef struct word word;
typedef struct root_firstborn root_firstborn;
typedef struct root_layer root_layer;

void prepend_word(firstborn_word* head, word entry);
root_layer* build_dictionary(word entry/*change this later to read from a file*/);
void kill_word_list(word* head);
void kill_tree(root_layer* root);
void dictionary_template(FILE* words, root_layer* root);
root_layer* build_root();
layer_1* build_layer_1(root_firstborn* rootFirstborn, char letter);
layer_2* build_layer_2(firstborn_letter_1* firstbornLetter1, char letter);
layer_3* build_layer_3(firstborn_letter_2* firstbornLetter2, char letter);
void search_word(char keyword[], root_layer* root);

typedef struct layer_1
{
    char letter;
    firstborn_letter_1* letter_head;
    layer_1* next_layer_1;
}layer_1;

typedef struct layer_2
{
    char letter;
    firstborn_letter_2* letter_head;
    layer_2* next_layer_2;
}layer_2;

typedef struct layer_3
{
    char letter;
    firstborn_word* letter_head;
    layer_3* next_layer_3;
}layer_3;

typedef struct word
{
    char entry[21];
    definition* def;
    word* next_word;
}word;

typedef struct firstborn_letter_1
{
    int children;
    layer_2*  first_second_letter;
}firstborn_letter_1;

typedef struct firstborn_letter_2
{
    int children;
    layer_3*  first_third_letter;
}firstborn_letter_2;

typedef struct firstborn_word
{
    int children;
    word* first_word;
}firstborn_word;

typedef struct root_firstborn
{
    int children;
    layer_1* first_first_letter;
}root_firstborn;


typedef struct root_layer
{
    root_layer* root_address;
    root_firstborn* root_firstborn; 
}root_layer;

int main(void){
    FILE* input_file = fopen("common.txt", "r");
    if(input_file == NULL){
        printf("Error: Could not open words.txt\n");
        return 1;
    }
    root_layer* root = calloc(1, sizeof(root_layer));
    root->root_firstborn = calloc(1, sizeof(root_firstborn));
    root->root_firstborn->children = 0;
    root->root_firstborn->first_first_letter = NULL;
    dictionary_template(input_file, root);
    char keywords[21];
    char* searchword;
    printf("Enter the word to be searched\n");
    scanf("%s", keywords);
    search_word(keywords, root);
    kill_tree(root);
    fclose(input_file);
    return 0;
}

void prepend_word(firstborn_word* head, word entry){
    word* new_word = calloc(1, sizeof(word));
    *new_word = entry;
    new_word->next_word = NULL;
    
    if(head->children == 0){
        head->first_word = new_word;
    }else{
        new_word->next_word = head->first_word;
        head->first_word = new_word;
    } 
    head->children++;
}

void kill_tree(root_layer* root){
    layer_1* tmp1 = root->root_firstborn->first_first_letter;
    while(tmp1 != NULL){
        layer_1* ptr1 = tmp1->next_layer_1;
        layer_2* tmp2 = tmp1->letter_head->first_second_letter;
        while(tmp2 != NULL){
            layer_2* ptr2 = tmp2->next_layer_2;
            layer_3* tmp3 = tmp2->letter_head->first_third_letter;
            while(tmp3 != NULL){
                layer_3* ptr3 = tmp3->next_layer_3;
                word* wordtmp = tmp3->letter_head->first_word;
                while(wordtmp != NULL){
                    word* wordptr = wordtmp->next_word;
                    free(wordtmp);
                    wordtmp = wordptr;
                }
                free(tmp3->letter_head);
                free(tmp3);
                tmp3 = ptr3;
            }
            free(tmp2->letter_head);
            free(tmp2);
            tmp2 = ptr2;
        }
        free(tmp1->letter_head);
        free(tmp1);
        tmp1 = ptr1;
    }
    free(root->root_firstborn);
    free(root);
}

void dictionary_template(FILE* words, root_layer* root){
    char current_word[21] = {0};
    layer_1* l1;
    layer_2* l2;
    layer_3* l3;
    while(fscanf(words, "%20s", current_word) == 1){
        switch(strlen(current_word)){
            case 1:
                memset(current_word + 1, ' ', 2);
                break;
            case 2:
                memset(current_word + 2, ' ', 1);
                break;
            default:
                if(root->root_firstborn->children == 0){//build the first layer
                    l1 = build_layer_1(root->root_firstborn, current_word[0]);
                }else{
                    layer_1* tmp = root->root_firstborn->first_first_letter;
                    int found = 0;
                    while(tmp != NULL){
                        if(tmp->letter == current_word[0]){
                            found++;
                            break;
                        }
                        tmp = tmp->next_layer_1;
                    }
                    if(!found){
                        l1 = root->root_firstborn->first_first_letter;
                        layer_1* ptr = l1;
                        l1 = build_layer_1(root->root_firstborn, current_word[0]);
                        l1->next_layer_1 = ptr;
                        root->root_firstborn->first_first_letter = l1;
                    }
                }
                if(l1->letter_head->children == 0){//build the second layer
                    l2 = l1->letter_head->first_second_letter;
                    l2 = build_layer_2(l1->letter_head, current_word[1]);
                    l1->letter_head->first_second_letter = l2;
                }else{
                    layer_2* tmp = l1->letter_head->first_second_letter;
                    int found = 0;
                    while(tmp != NULL){
                        if(tmp->letter == current_word[1]){
                            found++;
                            break;
                        }
                        tmp = tmp->next_layer_2;
                    }
                    if(!found){
                        l2 = l1->letter_head->first_second_letter;
                        layer_2* ptr = l2;
                        l2 = build_layer_2(l1->letter_head, current_word[1]);
                        l2->next_layer_2 = ptr;
                        l1->letter_head->first_second_letter = l2;
                    }
                }
                if(l2->letter_head->children == 0){//building the third layer
                    l3 = l2->letter_head->first_third_letter;
                    l3 = build_layer_3(l2->letter_head, current_word[2]);
                    l2->letter_head->first_third_letter = l3;
                }else{
                    layer_3* tmp = l2->letter_head->first_third_letter;
                    int found = 0;
                    while(tmp != NULL){
                        if(tmp->letter == current_word[2]){
                            found++;
                            break;
                        }
                        tmp = tmp->next_layer_3;
                    }
                    if(!found){
                        l3 = l2->letter_head->first_third_letter;
                        layer_3* ptr = l3;
                        l3 = build_layer_3(l2->letter_head, current_word[2]);
                        l3->next_layer_3 = ptr;
                        l2->letter_head->first_third_letter = l3;
                    }
                }
                word entry;
                entry.def = NULL;
                memset(entry.entry, 0, sizeof(current_word));
                strcpy(entry.entry, current_word);
                prepend_word(l3->letter_head, entry);
            //and with that this brainchild is technically out of the womb
        }
    }
}

root_layer* build_root(){
    root_layer* root = calloc(1, sizeof(root_layer));
    root->root_firstborn = calloc(1, sizeof(root_firstborn));
    root->root_firstborn->children = 0;
    root->root_firstborn->children++;  
    return root; 
}

layer_1* build_layer_1(root_firstborn* rootFirstborn, char letter){
    rootFirstborn->first_first_letter = calloc(1, sizeof(layer_1));
    rootFirstborn->first_first_letter->letter = letter;
    rootFirstborn->first_first_letter->next_layer_1 = NULL;
    rootFirstborn->first_first_letter->letter_head = calloc(1, sizeof(firstborn_letter_1));
    rootFirstborn->first_first_letter->letter_head->children = 0;
    rootFirstborn->children++;//change this to incriment the number of children in the root firstborn
    return rootFirstborn->first_first_letter;
}

layer_2* build_layer_2(firstborn_letter_1* firstbornLetter1, char letter){
    firstbornLetter1->first_second_letter = calloc(1, sizeof(layer_2));
    firstbornLetter1->first_second_letter->letter = letter;
    firstbornLetter1->first_second_letter->next_layer_2 = NULL;
    firstbornLetter1->first_second_letter->letter_head = calloc(1, sizeof(firstborn_letter_2));
    firstbornLetter1->first_second_letter->letter_head->children = 0;
    firstbornLetter1->children++;
    return firstbornLetter1->first_second_letter;
}

layer_3* build_layer_3(firstborn_letter_2* firstbornLetter2, char letter){
    firstbornLetter2->first_third_letter = calloc(1, sizeof(layer_3));
    firstbornLetter2->first_third_letter->letter = letter;
    firstbornLetter2->first_third_letter->next_layer_3 = NULL;
    firstbornLetter2->first_third_letter->letter_head = calloc(1, sizeof(firstborn_word));
    firstbornLetter2->first_third_letter->letter_head->children = 0;
    firstbornLetter2->children++;
    return firstbornLetter2->first_third_letter;
}

void search_word(char keyword[], root_layer* root){
    layer_1* tmp1 = root->root_firstborn->first_first_letter;
    while(tmp1->letter != keyword[0]){
        if(tmp1->next_layer_1 == NULL){
            printf("Failed at first layer\n");
            return;
        }
        tmp1 = tmp1->next_layer_1;
    }
    layer_2* tmp2 = tmp1->letter_head->first_second_letter;
    char x = keyword[1];
    while(tmp2->letter != x){
        if(tmp2->next_layer_2 == NULL){
            printf("Failed at second layer\n");
            return;
        }
        tmp2 = tmp2->next_layer_2;
    }
    layer_3* tmp3 = tmp2->letter_head->first_third_letter;
    while(tmp3->letter != keyword[2]){
        if(tmp3->next_layer_3 == NULL){
            printf("Failed at third layer\n");
            return;
        }
        tmp3 = tmp3->next_layer_3;
    }
    word *wordtmp = tmp3->letter_head->first_word;
    while(strcmp(wordtmp->entry, keyword)){
        if(wordtmp->next_word == NULL){
            printf("Failed at word level\n");
            return;
        }
        wordtmp = wordtmp->next_word; 
    }
    printf("Word found\n");
}
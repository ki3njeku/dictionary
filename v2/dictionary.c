#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct layer{
    char letter;
    bool end;
    struct layer* next_layers[26];
    int children;
    char* word;
}layer;

layer* build_layer(char letter, layer* parent);
void insert_word(char* word, layer* node);
void build_word(char* word, layer* root);
void kill_tree(layer* root);
void build_dictionary(FILE* file, layer* root);

int main (void){
    layer* root = calloc(1, sizeof(layer));
    root->letter = '!';
    FILE* input = fopen("common.txt", "r");
    build_dictionary(input, root);
    fclose(input);
    kill_tree(root);
}

//Function to make a new layer
layer* build_layer(char letter, layer* parent){
    layer* location = calloc(1, sizeof(layer));
    location->letter = letter;
    parent->next_layers[letter - 'a'] = location;
    parent->children++;
    return location;
}
//Function to insert word
void insert_word(char* word, layer* node){
    node->word = malloc(strlen(word) + 1);
    strcpy(node->word, word);
    node->end = true;
}
//Function to iterate over the words and create the layers based on each letter
void build_dictionary(FILE* file, layer* root){
    char word[21] = {0};
    while(fscanf(file, "%s", word) == 1){
        build_word(word, root);
        memset(word, 0, sizeof(word));
    }
}

void build_word(char* word, layer* root){
    int len = strlen(word);
    layer* tmp = root;
    layer* ptr = {0};
    for(int i = 0; i < len; i++){
        if(tmp->next_layers[(word[i] - 'a')] == NULL){
            ptr = build_layer(word[i], tmp);
            if(i == len - 1){
                insert_word(word, tmp->next_layers[(word[i] - 'a')]);
            }
            tmp->next_layers[(word[i] - 'a')] = ptr; 
            tmp = ptr;
        }else{
            tmp = tmp->next_layers[(word[i] - 'a')];
    
        }
    }
    
}
//Function to destroy the whole tree 
void kill_tree(layer* root){
    if(root != NULL){
        for(int i = 0; i < 26; i++){
            kill_tree(root->next_layers[i]);
        }
        if(root->end){
            free(root->word);
        }
        free(root);
    }   
}

//Function to search for words
void search_word(char keyword[], layer* root){
    int len = strlen(keyword);
    for(int i = 0; i < len; i++){
        
    }
}

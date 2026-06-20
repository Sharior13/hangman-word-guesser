#include "input.h"
#include "raylib.h"

//check is alphabet or no
int isAlphabet(int c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void capitalizeInput(int *c){
    if(*c >= 'a' && *c <= 'z'){
		*c -= ('a' - 'A');
	}
}

//factory function to initialize text input
TextInput createTextInput(){
    TextInput input;
    input.buffer[0] = '\0';
    input.length = 0;
    input.backspaceHeldTime = 0.0f;
    return input;
}

//reset input buffer
void clearInput(TextInput *input){
    input->length = 0;
    input->buffer[0] = '\0';
}

//add character to input buffer
void appendChar(TextInput *input, char c){
    if(input->length < INPUT_MAX){
        input->buffer[input->length++] = c;
        input->buffer[input->length] = '\0';
    }
}

//remove last character entered by user
void removeLastChar(TextInput *input){
    if(input->length > 0){
        input->buffer[--input->length] = '\0';
    }
}

//handle backspace being held down
void updateBackspace(TextInput *input){
    if(IsKeyPressed(KEY_BACKSPACE)){
        removeLastChar(input);
        input->backspaceHeldTime = 0.0f;
    }
    else if(IsKeyDown(KEY_BACKSPACE)){
        input->backspaceHeldTime += GetFrameTime();
        if(input->backspaceHeldTime >= BACKSPACE_INITIAL_DELAY){
            input->backspaceHeldTime -= BACKSPACE_REPEAT_RATE;
            removeLastChar(input);
        }
    } else {
        input->backspaceHeldTime = 0.0f;
    }
}

void updateInput(TextInput *input){
    //letter keys
    int key = GetCharPressed();
    while(key > 0){
        if(isAlphabet(key)){
        	capitalizeInput(&key);
            appendChar(input, key);
        }
        key = GetCharPressed();
    }

    //handle backspace
    updateBackspace(input);
}
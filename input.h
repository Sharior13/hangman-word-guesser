#ifndef INPUT_H
#define INPUT_H

//max characters the text input can hold
#define INPUT_MAX 30

//backspace key-repeat timing
//seconds before repeat starts
#define BACKSPACE_INITIAL_DELAY 0.4f
//seconds between repeats once held
#define BACKSPACE_REPEAT_RATE 0.04f

//text input field
typedef struct{
    char buffer[INPUT_MAX + 1];
    int length;
    float backspaceHeldTime;
} TextInput;

//factory function to create and return empty text input
TextInput createTextInput();

void clearInput(TextInput *input);
void appendChar(TextInput *input, char c);
void removeLastChar(TextInput *input);
void updateInput(TextInput *input);

#endif
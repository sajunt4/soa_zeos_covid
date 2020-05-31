//
// Created by abel on 31/5/20.
//

#include <menu.h>

char buffer[NUM_ROWS][NUM_COLUMNS];
char menuOptions[2][40] = {
        "1. Execute tests",
        "2. Play game"
};

char title[51] = "SOA SPACE INVADERS (by Sergi Miralles y Abel Reyes)";

void clearBuffer() {
    for (int x = 0; x < NUM_COLUMNS; x++) {
        for (int y = 0; y < NUM_ROWS; y++) {
            buffer[y][x] = ' ';
        }
    }
}

int selectedOption = 0;

int updateMenu()
{
    clearBuffer();

    for(int j = 0; j < 51; j++){
        buffer[NUM_ROWS / 2 - 4][(NUM_COLUMNS - 51) / 2 + j] = title[j];
    }

    for(int i = 0; i < 2; i++)
    {
        char * text = menuOptions[i];
        int textSize = 40;
        for(int j = 0; j < textSize; j++){
            buffer[NUM_ROWS/2 + (i * 2)][(NUM_COLUMNS - textSize) / 2 + j] = menuOptions[i][j];
        }

        if(selectedOption == i){
            buffer[NUM_ROWS/2 + (i * 2)][(NUM_COLUMNS - textSize) / 2 - 1] = '[';
            buffer[NUM_ROWS/2 + (i * 2)][NUM_COLUMNS - (textSize / 2) + 1] = ']';
        }
    }

    char input;
    if(get_key(&input) >= 0){
        if(input == 0x48){
            selectedOption--;
            if(selectedOption < 0) selectedOption = 0;
        }
        if(input == 0x50){
            selectedOption++;
            if(selectedOption > 1) selectedOption = 1;
        }
        if(input == 0x1C){        //TODO: Accept key
            return selectedOption;
        }
    }

    put_screen(buffer);

    return -1;
}


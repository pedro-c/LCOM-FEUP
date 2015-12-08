#include "MainMenuState.h"
#include "keyboard.h"
#include "interface.h"
#include "mouse.h"

/*
Rectangle* newRectangle(int x1, int y1, int x2, int y2){
	Rectangle* rect=(Rectangle*)malloc(sizeof(Rectangle));

	rect->x1=x1;
	rect->y1=y1;
	rect->x2=x2;
	rect->y2=y2;

	return rect;
}

void deleteRectangle(Rectangle* rect){
	free(rect);
}


MainMenuState* newMainMenuState() {
    MainMenuState* state = (MainMenuState*) malloc(sizeof(MainMenuState));

    state->done = 0;
    state->background = loadBitmap(getImagePath("menu-background"));

    // these numbers are just meant to create the buttons boundaries
    double w = .075, hi = .44, hf = hi + .12;
    int x1 = getHorResolution() / 2 - getHorResolution() * w;
    int x2 = getHorResolution() / 2 + getHorResolution() * w;
    int y1 = getVerResolution() * hi;
    int y2 = getVerResolution() * hf;
    state->playButton = newRectangle(x1, y1, x2, y2);
    state->mouseOnPlay = 0;

    hi = .64, hf = hi + .12;
    y1 = getVerResolution() * hi;
    y2 = getVerResolution() * hf;
    state->exitButton = newRectangle(x1, y1, x2, y2);
    state->mouseOnExit = 0;

    return state;
}

int updateMainMenuState(MainMenuState* state, unsigned long scancode) {
    int draw = 0;

    // if ESC has been pressed, quit
    if (scancode == KEY_DOWN(KEY_ESC)) {
        state->action = 1;
        state->done = 1;
    }

    // if mouse is inside the play button rectangle (boundaries)
    if (mouseInsideRect(state->playButton))
        state->mouseOnPlay = 1;
    else
        state->mouseOnPlay = 0;

    // if mouse is inside the exit button rectangle (boundaries)
    if (mouseInsideRect(state->exitButton)) {
        state->mouseOnExit = 1;

        // and left mouse button has been released
        if (getMouse()->leftButtonReleased) {
            state->action = 1;
            state->done = 1;
        }
    } else
        state->mouseOnExit = 0;

    return draw;
}

void drawMainMenuState(MainMenuState* state) {
    drawBitmap(state->background, 0, 0, ALIGN_LEFT);

    if (state->mouseOnPlay)
        drawRect(state->playButton, YELLOW);
    else if (state->mouseOnExit)
        drawRect(state->exitButton, YELLOW);
}

void deleteMainMenuState(MainMenuState* state) {
    deleteBitmap(state->background);
    deleteRectangle(state->playButton);
    deleteRectangle(state->exitButton);

    free(state);
}

*/


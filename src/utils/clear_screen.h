#ifndef CLEAR_SCREEN_H
#define CLEAR_SCREEN_H

/**
 * Clears the terminal screen in a cross-platform manner.
 * On Windows, it executes system("cls").
 * On non-Windows platforms, it executes system("clear").
 */
void clear_screen(void);

#endif // CLEAR_SCREEN_H

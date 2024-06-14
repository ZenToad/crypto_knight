#include <stdio.h>
#include "stb_textedit.h"

typedef struct ass_t {
    char *str;
    int count;
} ass_t;

typedef ass_t STB_TEXTEDIT_STRING; // the type of object representing a string being edited,
                                    
#define STB_TEXTEDIT_STRINGLEN(obj)      textedit_stringlen(obj)
#define STB_TEXTEDIT_LAYOUTROW(r,obj,n)         textedit_layoutrow(r, obj, n) // returns the results of laying out a line of characters
#define STB_TEXTEDIT_GETWIDTH(obj,n,i)  textedit_getwidth(obj,n,i)
#define STB_TEXTEDIT_GETCHAR(obj,i)     textedit_getchar(obj,i)
#define STB_TEXTEDIT_NEWLINE    '\n'
#define STB_TEXTEDIT_DELETECHARS(obj,i,n) textedit_deletechars(obj,i,n)
#define STB_TEXTEDIT_INSERTCHARS(obj,i,c,n) textedit_insertchars(obj,i,c,n)
#define STB_TEXTEDIT_KEYTOTEXT(k) textedit_keytotext(k)

#define STB_TEXTEDIT_K_SHIFT                    0xF0 // a power of two that is or'd in to a keyboard input to represent the shift key
#define STB_TEXTEDIT_K_LEFT                     1 // keyboard input to move cursor left
#define STB_TEXTEDIT_K_RIGHT                    2 // keyboard input to move cursor right
#define STB_TEXTEDIT_K_UP                       3 // keyboard input to move cursor up
#define STB_TEXTEDIT_K_DOWN                     4 // keyboard input to move cursor down
#define STB_TEXTEDIT_K_PGUP                     5 // keyboard input to move cursor up a page
#define STB_TEXTEDIT_K_PGDOWN                   6 // keyboard input to move cursor down a page
#define STB_TEXTEDIT_K_LINESTART                7 // keyboard input to move cursor to start of line  // e.g. HOME
#define STB_TEXTEDIT_K_LINEEND                  8 // keyboard input to move cursor to end of line    // e.g. END
#define STB_TEXTEDIT_K_TEXTSTART                9 // keyboard input to move cursor to start of text  // e.g. ctrl-HOME
#define STB_TEXTEDIT_K_TEXTEND                  10 // keyboard input to move cursor to end of text    // e.g. ctrl-END
#define STB_TEXTEDIT_K_DELETE                   11 // keyboard input to delete selection or character under cursor
#define STB_TEXTEDIT_K_BACKSPACE                12 // keyboard input to delete selection or character left of cursor
#define STB_TEXTEDIT_K_UNDO                     13 // keyboard input to perform undo
#define STB_TEXTEDIT_K_REDO                     14 // keyboard input to perform redo

int textedit_keytotext(int k) {
    printf("textedit_keytotext\n");
    return 0;
}

int textedit_stringlen(ass_t *str) {
    printf("textedit_stringlen\n");
    return 0;
}

void textedit_layoutrow(StbTexteditRow *r, ass_t *str, int n) {
    printf("textedit_layoutrow\n");
}

float textedit_getwidth(ass_t *str, int i, int k) {
    printf("textedit_layoutrow\n");
    return 0.0f;
}

int textedit_getchar(ass_t *str, int i) {
    printf("textedit_getchar\n");
    return 0;
}

void textedit_deletechars(ass_t *str, int i, int n) {
    printf("textedit_deletechars\n");
}

int textedit_insertchars(ass_t *str, int i, STB_TEXTEDIT_POSITIONTYPE *c, int len) {
    printf("textedit_insertchars\n");
    return 1;
}

#define STB_TEXTEDIT_IMPLEMENTATION
#include "stb_textedit.h"
 
int main() {
    return 0;
}


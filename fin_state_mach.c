#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

// https://yakking.branchable.com/posts/state-machines-in-c/

///////////////////////////////////////////

#define SEM_CASE_EXT

///////////////////////////////////////////

enum states {
    START,
    LOOP,
    END,
} state;

enum events {
    START_LOOPING,
    PRINT_HELLO,
    STOP_LOOPING,
};

///////////////////////////////////////////

#ifndef SEM_CASE_EXT

void step_state(enum events event) {
    switch(state) {
    case START:
        switch(event) {
        case START_LOOPING:
            state = LOOP;
            break;
        default:
            exit(1);
            break;
        }       
        break;
    case LOOP:
        switch(event) {
        case PRINT_HELLO:
            printf("Hello World!\n");
            break;
        case STOP_LOOPING:
            state = END;
            break;
        default:
            exit(1);
            break;
        }
        break;
    case END:
        exit(1);
        break;
    }
}

#else

typedef enum states (*event_handler)(enum states, enum events);

enum states start_looping(enum states state, enum events event) {
    return LOOP;
}

enum states print_hello(enum states state, enum events event) {
    printf("Hello World!\n");
    return LOOP;
}

enum states stop_looping(enum states state, enum events event) {
    return END;
}

event_handler transitions[STOP_LOOPING+1][END+1] = {
    [START] = { [START_LOOPING] = start_looping, },
    [LOOP] = { [PRINT_HELLO] = print_hello,
               [STOP_LOOPING] = stop_looping, },
};

void step_state(enum events event) {
    event_handler handler = transitions[event][state];
    if (!handler)
        exit(1);
    state = handler(state, event);
}

#endif

///////////////////////////////////////////

int main(void) {
    #ifndef SEM_CASE_EXT
        printf("\n\tNot SEM_CASE_EXT\n\n");
    #else
        printf("\n\tSEM_CASE_EXT\n\n");
    #endif
    step_state(START_LOOPING);
    step_state(PRINT_HELLO);
    step_state(PRINT_HELLO);
    step_state(STOP_LOOPING);
    return 0;
}

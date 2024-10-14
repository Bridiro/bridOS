#include "commands.h"

static char command[30];
static char args[226];
static char *p = NULL_POINTER;

void execute_command(char *input) {
    int i=0, j=0;

    for(int k=string_length(input); k<256; k++) {
        input[k] = '\0';
    }

    command[0] = '\0';
    args[0] = '\0';

    while(input[i]!=' ' && input[i]!='\0') {
        command[i] = input[i];
        i++;
    }

    command[i] = '\0';
    i++;

    while(input[i]!='\0') {
        args[j] = input[i];
        i++;
        j++;
    }

    args[j] = '\0';

    if (compare_string(string_to_lowercase(command), "shutdown") == 0) {
        print_string("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    else if(compare_string(string_to_lowercase(command), "clear") == 0) {
        clear_screen();
        print_string("> ");
    }
    else if(compare_string(string_to_lowercase(command), "echo") == 0) {
        print_string(args);
        print_string("\n> ");
    }
    else if(compare_string(string_to_lowercase(command), "alloc") == 0) {
        int size = string_length(args);
        if(size>0){
            p = (char *) mem_alloc(sizeof(char) * (size + 1));
            for(int i=0; i<size; i++) {
                p[i] = args[i];
            }
            p[size] = '\0';
            print_string("Memory allocated!\n> ");
        }
        else {
            print_string("Can't allocate less than 1 byte!\n> ");
        }
    }
    else if(compare_string(string_to_lowercase(command), "allocated") == 0) {
        print_string(p);
        print_string("\n> ");
    }
    else if(compare_string(string_to_lowercase(command), "clearalloc") == 0) {
        int size = string_length(p);
        for(int i=0; i<size; i++) {
            p[i] = '\0';
        }
        mem_free(p);
        print_string("Allocation cleared successfully!\n> ");
    }
    else {
        print_string("Unknown command: ");
        print_string(command);
        print_string("\n> ");
    }
}

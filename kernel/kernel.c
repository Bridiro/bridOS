#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"

#include "util.h"
#include "mem.h"

static char command[30];
static char args[226];
static char *p = NULL_POINTER;

void* alloc(int n) {
    int *ptr = (int *) mem_alloc(n * sizeof(int));
    if (ptr == NULL_POINTER) {
        print_string("Memory not allocated.\n");
    } 
    else {
        // Get the elements of the array
        for (int i = 0; i < n; ++i) {
            ptr[i] = i + 1; // shorthand for *(ptr + i)
        }

        for (int i = 0; i < n; ++i) {
            char str[256];
            int_to_string(ptr[i], str);
            print_string(str);
        }
        print_nl();
    }
    return ptr;
}

void start_kernel() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing keyboard (IRQ 1).\n");
    init_keyboard();

    print_string("Initializing dynamic memory.\n");
    init_dynamic_mem();

    print_string("> ");
}

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

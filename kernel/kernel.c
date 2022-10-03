#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"

#include "util.h"
#include "mem.h"

static char command[30];
static char args[226];

void* alloc(int n) {
    int *ptr = (int *) mem_alloc(n * sizeof(int));
    if (ptr == NULL_POINTER) {
        print_string("Memory not allocated.\n");
    } else {
        // Get the elements of the array
        for (int i = 0; i < n; ++i) {
//            ptr[i] = i + 1; // shorthand for *(ptr + i)
        }

        for (int i = 0; i < n; ++i) {
//            char str[256];
//            int_to_string(ptr[i], str);
//            print_string(str);
        }
//        print_nl();
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

    clear_screen();

    print_string("init_dynamic_mem()\n");
    print_dynamic_node_size();
    print_dynamic_mem();
    print_nl();

    int *ptr1 = alloc(5);
    print_string("int *ptr1 = alloc(5)\n");
    print_dynamic_mem();
    print_nl();

    int *ptr2 = alloc(10);
    print_string("int *ptr2 = alloc(10)\n");
    print_dynamic_mem();
    print_nl();

    mem_free(ptr1);
    print_string("mem_free(ptr1)\n");
    print_dynamic_mem();
    print_nl();

    int *ptr3 = alloc(2);
    print_string("int *ptr3 = alloc(2)\n");
    print_dynamic_mem();
    print_nl();

    mem_free(ptr2);
    print_string("mem_free(ptr2)\n");
    print_dynamic_mem();
    print_nl();

    mem_free(ptr3);
    print_string("mem_free(ptr3)\n");
    print_dynamic_mem();
    print_nl();
    

    print_string("> ");
}

void execute_command(char *input) {

    int i=0, j=0;

    command[0] = '\0';
    args[0] = '\0';

    while(input[i]!=' ') {
        command[i] = input[i];
        i++;
    }

    command[i] = '\0';

    while(input[i]!='\0') {
        args[j] = input[i];
        i++;
        j++;
    }

    args[j] = '\0';

    if (compare_string(string_to_lowercase(command), "exit") == 0) {
        print_string("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    else if(compare_string(string_to_lowercase(command), "clear") == 0) {
        clear_screen();
        print_string("> ");
    }
    else if(compare_string(string_to_lowercase(command), "helloworld") == 0) {
        print_string("Hello World!!!\n> ");
    }
    else {
        print_string("Unknown command: ");
        print_string(command);
        print_string("\n> ");
    }
}

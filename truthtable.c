#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "truthtable.h"

void get_truthtable(gate* head, int input_length, int output_length, var *params) {
    int row_length = pow(2, input_length);
    for (int i = 0; i < row_length; ++i) {                  
        assign_values(params, input_length, row_length, i); // assign values to inputs

        run_circuit(head, params);                          // run through gates to get outputs

        print_row(params, input_length, output_length);     // print one row at a time
    }
}

void assign_values(var *params, int input_length, int row_length, int i) {
    var *temp = params;
    for (int j = 0; j < input_length; j++) {
        if (j == input_length-1) {                  // last element 
            if (i%2 == 0) {
                temp->value = 0;
            }
            else {
                temp->value = 1;
            }
            continue;
        }
        if (j == 0) {                               // first element
            if (i >= (0.5*row_length)) {
                temp->value = 1;
            }
            else {
                temp->value = 0;
            }
            temp = temp->next;
            continue;
        }
        int distance = input_length - 1 - j;        // between elements
        int spacing = pow(2, distance);
        int val1 = 0, val2 = 1;
        if ((i - spacing) > 0 && (i/spacing) % 2 == 0) {
            swap(&val1, &val2);
        }
        if ( (i - spacing) >= 0) {
            temp->value = val2;
        }
        else {
            temp->value = val1;
        }
        temp = temp->next;
    }
}

void run_circuit(gate *head, var *params) {
    gate *temp = head;
    while (temp != NULL) {
        if (temp->kind == 0) {
            int in1 = get_value(params, temp->inputs->name);
            int in2 = get_value(params, temp->inputs->next->name);
            int out = ANDg(in1, in2);
            store_value(params, temp->outputs->name, out);
        }
        else if (temp->kind == 1) {
            int in1 = get_value(params, temp->inputs->name); 
            int in2 = get_value(params, temp->inputs->next->name);
            int out = ORg(in1, in2);
            store_value(params, temp->outputs->name, out);
        }
        else if (temp->kind == 2) {
            int in1 = get_value(params, temp->inputs->name);
            int in2 = get_value(params, temp->inputs->next->name);
            int out = NANDg(in1, in2);
            store_value(params, temp->outputs->name, out);
        }
        else if (temp->kind == 3) {
            int in1 = get_value(params, temp->inputs->name);
            int in2 = get_value(params, temp->inputs->next->name);
            int out = NORg(in1, in2);
            store_value(params, temp->outputs->name, out);
        }
        else if (temp->kind == 4) {
            int in1 = get_value(params, temp->inputs->name);
            int in2 = get_value(params, temp->inputs->next->name);
            int out = XORg(in1, in2);
            store_value(params, temp->outputs->name, out);
        }
        else if (temp->kind == 5) {
            int in = get_value(params, temp->inputs->name);
            int out = NOTg(in);
            store_value(params, temp->outputs->name, out);
        }
        else if (temp->kind == 6) {
            int in = get_value(params, temp->inputs->name);
            int out = PASSg(in);
            store_value(params, temp->outputs->name, out);
        }
        else if (temp->kind == 7) {
            DECg(temp->size, temp->inputs, temp->outputs, params);
        }
        else if (temp->kind == 8) {
            int out = MULg(temp->size, temp->inputs, params);
            temp->outputs->value = out;
            store_value(params, temp->outputs->name, out);
        }
        temp = temp->next;
    }
}

void print_row(var *params, int input_length, int output_length) {
    var *temp = params;
        for (int j = 0; j < input_length; ++j) {
            printf("%d ", temp->value);
            temp = temp->next;
        }
        printf("| ");
        for (int j = 0; j < output_length; ++j) {
            if (j == output_length-1) {
                printf("%d", temp->value);
            }
            else {
                printf("%d ", temp->value);
            }
            temp = temp->next;
        }
        printf("\n");
}

gate* order_gates(gate* head, gate *curr) {
    gate *ptr = head;
    gate *target = NULL;
    int check_in = 0, check_out = 0, check1 = 0, check2 = 0;

    var* temp = curr->inputs;                                   // get length of curr inputs
    int length_curr_inputs = 0;
    while (temp != NULL) {                                      
        length_curr_inputs++;
        temp = temp->next;
    }

    temp = curr->outputs;                                       // get length of curr inputs
    int length_curr_outputs = 0;
    while (temp != NULL) {                                      
        length_curr_outputs++;
        temp = temp->next;
    }

    while (ptr != NULL) {
        if (share_name(curr->outputs, ptr->inputs) == 1) {      // if outputs of curr = inputs of previously seen gates
            check1 = 1;
            target = ptr;
            check_out++;
        }
        else if (share_name(curr->inputs, ptr->outputs) == 1) { // if inputs of curr = outputs of previously seen gates
            check2 = 1;
            target = ptr;
            check_in++;
        }
        ptr = ptr->next;
    }

    if (check1 == 1 && check_out != length_curr_outputs) {
        return head = insert(head, curr);
    }
    else if (check1 == 1) {
        return head = insert_before(head, target, curr);
    }
    else if (check2 == 1 && check_in != length_curr_inputs) {
        return head = insert(head, curr);
    }
    else if (check2 == 1) {
        return head = insert_after(head, target, curr);
    }
    else {
        return head = insert(head, curr);
    }
}

int share_name(var* names1, var* names2) {
    var *temp = names1;
    while (temp != NULL) {
        var *temp2 = names2;
        while (temp2 != NULL) {
            if (strcmp(temp->name, temp2->name) == 0) {
                return 1;
            }
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    return 0;
}

gate* insert(gate *head, gate *new_gate) {
    if (head == NULL) {
        head = new_gate;
    }
    gate* ptr = head;
    while(ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = new_gate;
    new_gate->next = NULL;  
    return head;
}

gate* insert_after(gate *head, gate *index, gate *new_gate) {
    gate *ptr = head;
    while (ptr != NULL) {           // may need to change
        if (ptr == index) {
            gate *temp = ptr->next;
            ptr->next = new_gate;
            new_gate->next = temp;
            return head;
        }
        ptr = ptr->next;
    }
    return head;
}

gate* insert_before(gate *head, gate *index, gate *new_gate) {
    if (head == index) {            // if index is first in list
        gate *temp = head;
        head = new_gate;
        new_gate->next = temp;
        return head;
    }
    gate *ptr = head;
    while (ptr != NULL) {           // may need to change
        if (ptr->next == index) {
            gate *temp = ptr->next;;
            ptr->next = new_gate;
            new_gate->next = temp;
            return head;
        }
        ptr = ptr->next;
    }
    return head;
}

var* insert_var(var *head, var *new_var) {
    if (head == NULL) {
        head = new_var;
    }
    var* ptr = head;
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = new_var;
    new_var->next = NULL;
    return head;
}

int is_in_var_list(var *head, char *name) {
    var *temp = head;
    while (temp != NULL) {
        if (strcmp(name, temp->name) == 0) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

int get_value(var *head, char *name) {
    var *temp = head;
    while (temp != NULL) {
        if (strcmp(name, temp->name) == 0) {
            return temp->value;
        }
        temp = temp->next;
    }
    return -1;
}

void store_value(var *head, char *name, int val) {
    var *temp = head;
    while (temp != NULL) {
        if (strcmp(name, temp->name) == 0) {
            temp->value = val;
            return;
        }
        temp = temp->next;
    }
}

void swap (int *num1, int *num2) {
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

int kind(char* token) {
    if (strcmp(token, "AND") == 0) {                                  
        return 0;
    }
    else if (strcmp(token, "OR") == 0) {
        return 1;
    }
    else if (strcmp(token, "NAND") == 0) {
        return 2;
    }
    else if (strcmp(token, "NOR") == 0) {
        return 3;
    }
    else if (strcmp(token, "XOR") == 0) {
        return 4;
    }
    else if (strcmp(token, "NOT") == 0) {
        return 5;
    }
    else if (strcmp(token, "PASS") == 0) {
        return 6;
    }
    else if (strcmp(token, "DECODER") == 0) {
        return 7;
    }
    else if (strcmp(token, "MULTIPLEXER") == 0) {
        return 8;
    }
    return -1;
}

int ORg(int a, int b) {
    if (a == 1 || b == 1) { return 1; } else { return 0; }
}

int NORg(int a, int b) {
    if (a == 1 || b == 1) { return 0; } else { return 1; }
}

int XORg(int a, int b) {
    if ( (a == 1 && b == 0) || (a == 0 && b==1) ) { return 1; } else { return 0; }
}

int ANDg(int a, int b) {
    if (a == 1 && b == 1) { return 1; } else { return 0; }
}

int NANDg(int a, int b) {
    if (a == 1 && b == 1) { return 0; } else { return 1; }
}

int NOTg(int a) {
    if (a == 1) { return 0; } else { return 1; } 
}

int PASSg(int a) {
    return a;
}

int MULg(int size, var *inputs, var *params) {
    int selector = 0, power = size - 1;
    int input_length = pow(2,size);
    var *temp = inputs;
    for (int i = 0; i < input_length; ++i) {                            // traverse list of inputs till start of selectors  
        temp = temp->next;
    }
    for (int i = 0; i < size; ++i) {                                    // compute value of selector
        int value = -1;
        value = get_value(params, temp->name);                          // get one bit of of selector from params
        if (value == 1) {
            selector += pow(2,power);
        }
        power--;
        temp = temp->next;
    }

    var *temp2 = inputs;
    for (int i = 0; i < input_length; ++i) {                            // use binary value to get output index in inputs
        if (i == selector) {
            int value = -2;
            if (is_in_var_list(params, temp2->name) == 1) {
                value = get_value(params, temp2->name);
            }
            else {
                value = temp2->value;
            }
            return value;
        }
        temp2 = temp2->next;
    }
    return -1;                                             
}

void DECg(int size, var* inputs, var *outputs, var* params) {
    int index = 0, power = size - 1;   
    var *temp = inputs;                                     
    for (int i = 0; i < size; ++i) {                                    // compute value of index
        int value = -1;
        value = get_value(params, temp->name);
        if (value == 1) {
            index += pow(2, power);
        }
        power--;
        temp = temp->next;
    }
    var* temp2 = outputs;
    for (int i = 0; i < pow(2, size); ++i) {                            // assign outputs to 0 except index
        if (i == index) {
            store_value(params, temp2->name, 1);
            temp2->value = 1;
        }
        else {
            store_value(params, temp2->name, 0);
            temp2->value = 0;
        }
        temp2 = temp2->next;
    }
}

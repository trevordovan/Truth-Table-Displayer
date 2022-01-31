/* 
 * Author:       Trevor Dovan
 * Start Date:   11/26/2021
 * End Date:     11/30/2021
 * Extra Credit:  
 * 
 * change DECg and MULg to work with linked list of ins/outs instead of arrays
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum { AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER } kind_t;

typedef struct var {
    char name[17];                  // variable name
    int value;                      // 0 | 1
    struct var* next;               // pointer to next var
} var;

typedef struct gate {
    kind_t kind;                    // kind of gate (int)
    int size;                       // indicates size of DECODER and MULTIPLEXER
    var *inputs;                    // head of linked list of inputs
    var *outputs;                   // head of linked list of inputs
    struct gate* next;              // pointer to next gate
} gate;

gate* order_gates(gate* head, gate *curr);
gate* insert(gate *head, gate *new_gate);
gate* insert_after(gate *head, gate *index, gate *new_gate);
gate* insert_before(gate *head, gate *index, gate *new_gate);
var* insert_var(var *head, var *new_var);
void get_truthtable(gate* head, int input_length, int output_length, var *params);
void print_row(var *params, int input_length, int output_length);
void assign_values(var *params, int input_length, int row_length, int i);
void store_value(var *head, char *name, int val);
void run_circuit(gate *head, var *params);
void swap (int *num1, int *num2);
int share_name(var* names1, var* names2);
int is_in_var_list(var *head, char *name);
int get_value(var *head, char *name);
int kind(char* token);
int ORg(int a, int b);
int NORg(int a, int b);
int XORg(int a, int b);
int ANDg(int a, int b);
int NANDg(int a, int b);
int NOTg(int a);
int PASSg(int a);
int MULg(int size, var *inputs, var *params);
void DECg(int size, var* inputs, var *outputs, var* params);

/* 
 * Prints a truthtable of inputs and outputs based on a specified 
 * circuit provided by a text file. 
 */
int main(int argc, char **argv) {

    FILE *file = fopen(argv[1], "r");                                       // open file
    char *token = malloc(sizeof(char) * 17);
    fscanf(file, "%16s", token);

    int input_length, check;                                                // get input length
    check = fscanf(file, "%d", &input_length);                              

    var *params = NULL;
    for(int i = 0; i < input_length; ++i) {                                 // store inputs in params
        fscanf(file, "%16s", token);
        var *temp = malloc(sizeof(var));
        strcpy(temp->name, token);
        temp->next = NULL;
        params = insert_var(params, temp);
    }
    fscanf(file, "%16s", token);

    int output_length;                                                      // get output length  
    fscanf(file, "%d", &output_length);                                                       
     for(int i = 0; i < output_length; ++i) {                               // store outputs in params
        fscanf(file, "%16s", token);
        var *temp = malloc(sizeof(var));
        strcpy(temp->name, token);
        temp->next = NULL;
        params = insert_var(params, temp);
    }

    gate *head = NULL;                                                      // read gates                                               
    while (fscanf(file, "%16s", token) != EOF) {    
        gate *temp;                        
        temp = malloc(sizeof(gate));
        temp->next = NULL;
        temp->kind = kind(token);                                           // type of gate

        if (temp->kind == 0 || temp->kind == 1 || temp->kind == 2 ||        // store inputs and outputs in gate
            temp->kind == 3 || temp->kind == 4) {
            // 2 in and 1 out
            // inputs
            var *head_inputs = NULL;
            temp->inputs = head_inputs;
            for (int i = 0; i < 2; ++i) {
                var *temp2 = malloc(sizeof(var));
                temp2->next = NULL;
                fscanf(file, "%16s", token);
                if (strcmp(token, "0") == 0) {
                    strcpy(temp2->name, token);
                    temp2->value = 0;

                    if (is_in_var_list(params, token) == 0) {               // store temp vars in params
                        var *temp = malloc(sizeof(var));
                        strcpy(temp->name, token);
                        temp->value = 0;
                        temp->next = NULL;
                        params = insert_var(params, temp);
                    }
                }
                else if (strcmp(token, "1") == 0) {
                    strcpy(temp2->name, token);
                    temp2->value = 1;

                     if (is_in_var_list(params, token) == 0) {              // store temp vars in params
                        var *temp = malloc(sizeof(var));
                        strcpy(temp->name, token);
                        temp->value = 1;
                        temp->next = NULL;
                        params = insert_var(params, temp);
                    }
                }
                else {
                    strcpy(temp2->name, token);
                }
                temp->inputs = insert_var(temp->inputs, temp2);
            }
            // outputs
            var *head_outputs = NULL;
            temp->outputs = head_outputs;
            var *temp2 = malloc(sizeof(var));
            temp2->next = NULL;
            fscanf(file, "%16s", token);
            strcpy(temp2->name, token);

            if (is_in_var_list(params, token) == 0) {                       // store temp vars in params
                var *temp = malloc(sizeof(var));
                strcpy(temp->name, token);
                temp->next = NULL;
                params = insert_var(params, temp);
            }
            temp->outputs = insert_var(temp->outputs, temp2);
        }
        else if (temp->kind == 5 || temp->kind == 6) {
            // 1 in 1 out
            // inputs
            var *head_inputs = NULL;
            temp->inputs = head_inputs;
            var *temp2 = malloc(sizeof(var));
            temp2->next = NULL;
            fscanf(file, "%16s", token);
            if (strcmp(token, "0") == 0) {
                    strcpy(temp2->name, token);
                    temp2->value = 0;

                    if (is_in_var_list(params, token) == 0) {               // store temp vars in params
                        var *temp = malloc(sizeof(var));
                        strcpy(temp->name, token);
                        temp->value = 0;
                        temp->next = NULL;
                        params = insert_var(params, temp);
                    }
                }
                else if (strcmp(token, "1") == 0) {
                    strcpy(temp2->name, token);
                    temp2->value = 1;

                     if (is_in_var_list(params, token) == 0) {              // store temp vars in params
                        var *temp = malloc(sizeof(var));
                        strcpy(temp->name, token);
                        temp->value = 1;
                        temp->next = NULL;
                        params = insert_var(params, temp);
                    }
                }
                else {
                    strcpy(temp2->name, token);
                }
            temp->inputs = insert_var(temp->inputs, temp2);
            // output
            var *head_outputs = NULL;
            temp->outputs = head_outputs;
            var *temp3 = malloc(sizeof(var));
            temp3->next = NULL;
            fscanf(file, "%16s", token);
            strcpy(temp3->name, token);

            if (is_in_var_list(params, token) == 0) {                       // store temp vars in params
                var *temp = malloc(sizeof(var));
                strcpy(temp->name, token);
                temp->next = NULL;
                params = insert_var(params, temp);
            }
            temp->outputs = insert_var(temp->outputs, temp3);
        }
        else if (temp->kind == 7){                                          // decoder (n in 2^n out)
            fscanf(file, "%d", &(temp->size));
            // inputs
            int in_size = temp->size;
            var *head_inputs = NULL;
            temp->inputs = head_inputs;
            for (int i = 0; i < in_size; ++i) {
                fscanf(file, "%16s", token);
                var *temp2 = malloc(sizeof(var));
                temp2->next = NULL;
                if (strcmp(token, "0") == 0) {
                    strcpy(temp2->name, token);
                    temp2->value = 0;
                }
                else if (strcmp(token, "1") == 0) {
                    strcpy(temp2->name, token);
                    temp2->value = 1;
                }
                else {
                    strcpy(temp2->name, token);
                }
                temp->inputs = insert_var(temp->inputs, temp2);
            }
            //outputs
            int out_size = pow(2, temp->size);
            var *head_outputs = NULL;
            temp->outputs = head_outputs;
            for (int i = 0; i < out_size; ++i) {
                fscanf(file, "%16s", token);
                var *temp2 = malloc(sizeof(var));
                temp2->next = NULL;
                strcpy(temp2->name, token);
                temp->outputs = insert_var(temp->outputs, temp2);

                if (is_in_var_list(params, token) == 0) {                   // store temp vars in params
                    var *temp = malloc(sizeof(var));
                    strcpy(temp->name, token);
                    temp->next = NULL;
                    params = insert_var(params, temp);
                }
            }
        }  
        else if (temp->kind == 8) {                                         // muliplexer (variable in 1 out)
            fscanf(file, "%d", &(temp->size));
            int in_size = pow(2,temp->size) + temp->size; 
            var *head_inputs = NULL;
            temp->inputs = head_inputs;
            // inputs (ins and selectors)
            for (int i = 0; i < in_size; ++i) {
                fscanf(file, "%16s", token);
                var *temp2 = malloc(sizeof(var));
                temp2->next = NULL;
                if (strcmp(token, "0") == 0) {
                    strcpy(temp2->name, token);
                    temp2->value = 0;
                }
                else if (strcmp(token, "1") == 0) {
                    strcpy(temp2->name, token);
                    temp2->value = 1;
                }
                else {
                    strcpy(temp2->name, token);
                }
                temp->inputs = insert_var(temp->inputs, temp2);
            }
            // output
            var *head_outputs = NULL;
            temp->outputs = head_outputs;
            var *temp2 = malloc(sizeof(var));
            temp2->next = NULL;
            fscanf(file, "%16s", token);
            strcpy(temp2->name, token);

            if (is_in_var_list(params, token) == 0) {                       // store temp vars in params
                var *temp = malloc(sizeof(var));
                strcpy(temp->name, token);
                temp->next = NULL;
                params = insert_var(params, temp);
            }
            temp->outputs = insert_var(temp->outputs, temp2);
        }
        head = order_gates(head, temp);
    }

    // print gates (test purposes)
    // gate *temp = head;
    // while (temp != NULL) { 
    //     printf("%d\n", temp->kind);
    //     temp = temp->next;
    // }                                        

    fclose(file);                                                           // close file

    get_truthtable(head, input_length, output_length, params);              // go through gates and print values

    while (head != NULL) {                                                  // free (gates, vars)
        gate *temp = head;

        var* temp_in = head->inputs;
        while (temp_in != NULL) {
            var *temp2 = temp_in;
            temp_in = temp_in->next;
            free(temp2);
        }
        free(temp_in);

        var* temp_out = head->outputs;
        while (temp_out != NULL) {
            var *temp2 = temp_out;
            temp_out = temp_out->next;
            free(temp2);
        }

        head = head->next;
        free(temp);
    }
    while (params != NULL) {                                             
        var *temp = params;
        params = params->next;
        free(temp);
    }
    free(head);
    free(params);
    free(token);

    return EXIT_SUCCESS;
}

/*
 * Prints a truthtable of specified inputs and outputs based on provided cirucit.
 * @param head head of list containing gates that make up circuit (ordered)
 * @param input_length length of inputs array
 * @param out_length  length of outputs array
 * @param params head of the list of parameters
 */
void get_truthtable(gate* head, int input_length, int output_length, var *params) {
    int row_length = pow(2, input_length);
    for (int i = 0; i < row_length; ++i) {                  
        assign_values(params, input_length, row_length, i); // assign values to inputs

        run_circuit(head, params);                          // run through gates to get outputs

        print_row(params, input_length, output_length);     // print one row at a time
    }
}

/*
 * Assigns truthtable values to inputs in parameter list 
 * based on a specified row of the truthtable
 * @param params head of parameter list
 * @param input_length amount of inputs
 * @param row_length length of row of entire truthtable
 * @param i index of row from entire truthtable
 */
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

/*
 * Traveres through list of gates comparing inputs/outputs of gates to 
 * param list to get and store values based on the circuit.
 * @param head head of the gate list
 * @param params head of the list of inputs/outputs/tempVariables
 */
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

/*
 * Prints one row of a truthtable
 * @param params head of parameter list
 * @param input_length length of inputs
 * @param output_length length of outputs
 */
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

/*
 * Inserts gate at correct index in gate list so it is ordered.
 * @param head head of the gate list
 * @param curr the current gate being inserted into ordered list
 * @return head head of the gate list
 */
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

    temp = curr->outputs;                                   // get length of curr inputs
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

/*
 * Returns 0 | 1 if there is a common element in the list.
 * @param names1 list 1
 * @param names2 list 2
 * @return 0 | 1
 */
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

/*
 * Inserts a gate at the end of the list.
 * @param head head of the list
 * @param new_gate new gate to be added at end of list
 * @return head head of the list after insertion
 */
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

/*
 * Inserts a gate after a specified gate.
 * @param head head of the list
 * @param index specified gate
 * @param new_gate new gate to be added after index
 * @return head head of the list after insertion
 */
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

/*
 * Inserts a gate before a specified gate.
 * @param head head of the list
 * @param index specified gate
 * @param new_gate new gate to be added before index
 * @return head head of the list after insertion
 */
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

/*
 * Inserts a var at the end of the list.
 * @param head head of the list
 * @param new_var new var to be added at end of list
 * @return head head of the list after insertion
 */
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

/*
 * Given a variable name returns 0 or 1 if is in variable list.
 * @param head head of the var list
 * @param name variable name to be checked for
 * @return 0 : not in list 1 : is in list
 */
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

/*
 * Returns the value associated with a given variable name in a var list.
 * @param head head of the var list
 * @param name name of the given variable
 * @return value associated with variable, returns -1 if variable is not in list
 */
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

/*
 * Assigns a value to a given variable name in a var list.
 * @param head head of the var list
 * @param name name of variable
 * @param val value to be assigned
 */
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

/*
 * Swaps the value sof two given integers.
 * @param num1 address of first integer
 * @param num2 address of second integer
 */
void swap (int *num1, int *num2) {
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

/*
 * Returns number form of gate given a specified gate in form char*.
 * @param token char array to be compared to gates
 * @return number form of gate
 */
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

/******************** gates ********************/

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

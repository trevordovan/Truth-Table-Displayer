#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "truthtable.h"

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
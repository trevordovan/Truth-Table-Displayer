#ifndef truthtable
#define truthtable

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

/*
 * Inserts gate at correct index in gate list so it is ordered.
 * @param head head of the gate list
 * @param curr the current gate being inserted into ordered list
 * @return head head of the gate list
 */
gate* order_gates(gate* head, gate *curr);

/*
 * Inserts a gate at the end of the list.
 * @param head head of the list
 * @param new_gate new gate to be added at end of list
 * @return head head of the list after insertion
 */
gate* insert(gate *head, gate *new_gate);

/*
 * Inserts a gate after a specified gate.
 * @param head head of the list
 * @param index specified gate
 * @param new_gate new gate to be added after index
 * @return head head of the list after insertion
 */
gate* insert_after(gate *head, gate *index, gate *new_gate);

/*
 * Inserts a gate before a specified gate.
 * @param head head of the list
 * @param index specified gate
 * @param new_gate new gate to be added before index
 * @return head head of the list after insertion
 */
gate* insert_before(gate *head, gate *index, gate *new_gate);

/*
 * Inserts a var at the end of the list.
 * @param head head of the list
 * @param new_var new var to be added at end of list
 * @return head head of the list after insertion
 */
var* insert_var(var *head, var *new_var);

/*
 * Prints a truthtable of specified inputs and outputs based on provided cirucit.
 * @param head head of list containing gates that make up circuit (ordered)
 * @param input_length length of inputs array
 * @param out_length  length of outputs array
 * @param params head of the list of parameters
 */
void get_truthtable(gate* head, int input_length, int output_length, var *params);

/*
 * Prints one row of a truthtable
 * @param params head of parameter list
 * @param input_length length of inputs
 * @param output_length length of outputs
 */
void print_row(var *params, int input_length, int output_length);

/*
 * Assigns truthtable values to inputs in parameter list 
 * based on a specified row of the truthtable
 * @param params head of parameter list
 * @param input_length amount of inputs
 * @param row_length length of row of entire truthtable
 * @param i index of row from entire truthtable
 */
void assign_values(var *params, int input_length, int row_length, int i);

/*
 * Assigns a value to a given variable name in a var list.
 * @param head head of the var list
 * @param name name of variable
 * @param val value to be assigned
 */
void store_value(var *head, char *name, int val);

/*
 * Traveres through list of gates comparing inputs/outputs of gates to 
 * param list to get and store values based on the circuit.
 * @param head head of the gate list
 * @param params head of the list of inputs/outputs/tempVariables
 */
void run_circuit(gate *head, var *params);

/*
 * Swaps the value sof two given integers.
 * @param num1 address of first integer
 * @param num2 address of second integer
 */
void swap (int *num1, int *num2);

/*
 * Returns 0 | 1 if there is a common element in the list.
 * @param names1 list 1
 * @param names2 list 2
 * @return 0 | 1
 */
int share_name(var* names1, var* names2);

/*
 * Given a variable name returns 0 or 1 if is in variable list.
 * @param head head of the var list
 * @param name variable name to be checked for
 * @return 0 : not in list 1 : is in list
 */
int is_in_var_list(var *head, char *name);

/*
 * Returns the value associated with a given variable name in a var list.
 * @param head head of the var list
 * @param name name of the given variable
 * @return value associated with variable, returns -1 if variable is not in list
 */
int get_value(var *head, char *name);

/*
 * Returns number form of gate given a specified gate in form char*.
 * @param token char array to be compared to gates
 * @return number form of gate
 */
int kind(char* token);

/******************** gates ********************/

int ORg(int a, int b);
int NORg(int a, int b);
int XORg(int a, int b);
int ANDg(int a, int b);
int NANDg(int a, int b);
int NOTg(int a);
int PASSg(int a);
int MULg(int size, var *inputs, var *params);
void DECg(int size, var* inputs, var *outputs, var* params);

#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <inttypes.h>

#define MAX_VARS 100
#define MAX_INPUT_LEN 8192
#define MAX_TOKENS 500
#define MAX_INSTR 4096

// MIPS64 Opcodes
#define OPCODE_SPECIAL 0x00
#define OPCODE_DADDIU  0x19
#define OPCODE_LD      0x37
#define OPCODE_SD      0x3F
#define FUNCT_DADDU    0x2D
#define FUNCT_DSUBU    0x2F
#define FUNCT_DMULT    0x1C
#define FUNCT_DDIV     0x1E
#define FUNCT_MFLO     0x12

// Variable structure
typedef struct {
    char name[32];
    int64_t value;
    int mem_offset;
    bool is_declared;
} Variable;

// Instruction structure
typedef struct {
    char assembly[128];
    char binary[33];
    char hex[9];
} Instr;

// Token types for expression parsing
typedef enum {
    TOK_NUMBER,
    TOK_VARIABLE,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MULT,
    TOK_DIV,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_END
} TokenType;

typedef struct {
    TokenType type;
    char str[64];
    int64_t num_value;
} Token;

// Global state
Variable variables[MAX_VARS];
int var_count = 0;
int next_mem_offset = 8;
Instr instrs[MAX_INSTR];
int instr_count = 0;
int error_count = 0;

// Tokens for expression parsing
Token tokens[MAX_TOKENS];
int token_count = 0;

// RPN (Reverse Polish Notation) output queue
Token rpn_queue[MAX_TOKENS];
int rpn_count = 0;

// Temporary register management
int temp_reg_counter = 1;

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void report_error(const char *msg, const char *context) {
    if (context) {
        fprintf(stderr, "Error: %s (in '%s')\n", msg, context);
    } else {
        fprintf(stderr, "Error: %s\n", msg);
    }
    error_count++;
}

void int_to_bin_32(uint32_t val, char *out) {
    for (int i = 31; i >= 0; i--) {
        out[31-i] = ((val >> i) & 1) ? '1' : '0';
    }
    out[32] = '\0';
}

// ============================================================================
// INSTRUCTION EMISSION
// ============================================================================

void emit_instr(const char *assembly, uint32_t encoding) {
    if (instr_count >= MAX_INSTR) return;
    
    strcpy(instrs[instr_count].assembly, assembly);
    int_to_bin_32(encoding, instrs[instr_count].binary);
    sprintf(instrs[instr_count].hex, "%08X", encoding);
    instr_count++;
}

void emit_daddiu(int rt, int rs, int imm) {
    char buf[128];
    sprintf(buf, "DADDIU r%d, r%d, %d", rt, rs, imm);
    uint32_t enc = (OPCODE_DADDIU << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF);
    emit_instr(buf, enc);
}

void emit_daddu(int rd, int rs, int rt) {
    char buf[128];
    sprintf(buf, "DADDU r%d, r%d, r%d", rd, rs, rt);
    uint32_t enc = (OPCODE_SPECIAL << 26) | (rs << 21) | (rt << 16) | (rd << 11) | FUNCT_DADDU;
    emit_instr(buf, enc);
}

void emit_dsubu(int rd, int rs, int rt) {
    char buf[128];
    sprintf(buf, "DSUBU r%d, r%d, r%d", rd, rs, rt);
    uint32_t enc = (OPCODE_SPECIAL << 26) | (rs << 21) | (rt << 16) | (rd << 11) | FUNCT_DSUBU;
    emit_instr(buf, enc);
}

void emit_dmult(int rs, int rt, int rd) {
    char buf[128];
    sprintf(buf, "DMULT r%d, r%d", rs, rt);
    uint32_t enc = (OPCODE_SPECIAL << 26) | (rs << 21) | (rt << 16) | FUNCT_DMULT;
    emit_instr(buf, enc);
    
    sprintf(buf, "MFLO r%d", rd);
    enc = (OPCODE_SPECIAL << 26) | (rd << 11) | FUNCT_MFLO;
    emit_instr(buf, enc);
}

void emit_ddiv(int rs, int rt, int rd) {
    char buf[128];
    sprintf(buf, "DDIV r%d, r%d", rs, rt);
    uint32_t enc = (OPCODE_SPECIAL << 26) | (rs << 21) | (rt << 16) | FUNCT_DDIV;
    emit_instr(buf, enc);
    
    sprintf(buf, "MFLO r%d", rd);
    enc = (OPCODE_SPECIAL << 26) | (rd << 11) | FUNCT_MFLO;
    emit_instr(buf, enc);
}

void emit_ld(int rt, const char *var_name, int offset) {
    char buf[128];
    sprintf(buf, "LD r%d, %s(r29)", rt, var_name);
    uint32_t enc = (OPCODE_LD << 26) | (29 << 21) | (rt << 16) | (offset & 0xFFFF);
    emit_instr(buf, enc);
}

void emit_sd(int rt, const char *var_name, int offset) {
    char buf[128];
    sprintf(buf, "SD r%d, %s(r29)", rt, var_name);
    uint32_t enc = (OPCODE_SD << 26) | (29 << 21) | (rt << 16) | (offset & 0xFFFF);
    emit_instr(buf, enc);
}

// ============================================================================
// VARIABLE MANAGEMENT
// ============================================================================

Variable* get_variable(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return NULL;
}

void declare_variable(const char *name) {
    if (get_variable(name)) return; // Already exists
    
    strcpy(variables[var_count].name, name);
    variables[var_count].value = 0;
    variables[var_count].mem_offset = next_mem_offset;
    variables[var_count].is_declared = true;
    
    next_mem_offset += 8;
    var_count++;
}

// ============================================================================
// TOKENIZER
// ============================================================================

void tokenize_expression(const char *expr) {
    token_count = 0;
    int i = 0;
    int len = strlen(expr);
    
    while (i < len) {
        // Skip whitespace
        while (i < len && isspace(expr[i])) i++;
        if (i >= len) break;
        
        // Numbers (including negative)
        if (isdigit(expr[i]) || (expr[i] == '-' && i+1 < len && isdigit(expr[i+1]))) {
            int start = i;
            if (expr[i] == '-') i++;
            while (i < len && isdigit(expr[i])) i++;
            
            int num_len = i - start;
            strncpy(tokens[token_count].str, expr + start, num_len);
            tokens[token_count].str[num_len] = '\0';
            tokens[token_count].type = TOK_NUMBER;
            tokens[token_count].num_value = atoll(tokens[token_count].str);
            token_count++;
        }
        // Variables
        else if (isalpha(expr[i]) || expr[i] == '_') {
            int start = i;
            while (i < len && (isalnum(expr[i]) || expr[i] == '_')) i++;
            
            int var_len = i - start;
            strncpy(tokens[token_count].str, expr + start, var_len);
            tokens[token_count].str[var_len] = '\0';
            tokens[token_count].type = TOK_VARIABLE;
            token_count++;
        }
        // Operators and parentheses
        else if (expr[i] == '+') {
            tokens[token_count].type = TOK_PLUS;
            tokens[token_count].str[0] = '+';
            tokens[token_count].str[1] = '\0';
            token_count++;
            i++;
        }
        else if (expr[i] == '-') {
            tokens[token_count].type = TOK_MINUS;
            tokens[token_count].str[0] = '-';
            tokens[token_count].str[1] = '\0';
            token_count++;
            i++;
        }
        else if (expr[i] == '*') {
            tokens[token_count].type = TOK_MULT;
            tokens[token_count].str[0] = '*';
            tokens[token_count].str[1] = '\0';
            token_count++;
            i++;
        }
        else if (expr[i] == '/') {
            tokens[token_count].type = TOK_DIV;
            tokens[token_count].str[0] = '/';
            tokens[token_count].str[1] = '\0';
            token_count++;
            i++;
        }
        else if (expr[i] == '(') {
            tokens[token_count].type = TOK_LPAREN;
            tokens[token_count].str[0] = '(';
            tokens[token_count].str[1] = '\0';
            token_count++;
            i++;
        }
        else if (expr[i] == ')') {
            tokens[token_count].type = TOK_RPAREN;
            tokens[token_count].str[0] = ')';
            tokens[token_count].str[1] = '\0';
            token_count++;
            i++;
        }
        else {
            report_error("Unknown character in expression", NULL);
            i++;
        }
    }
    
    tokens[token_count].type = TOK_END;
}

// ============================================================================
// SHUNTING YARD ALGORITHM (Infix to RPN)
// ============================================================================

int get_precedence(TokenType op) {
    if (op == TOK_PLUS || op == TOK_MINUS) return 1;
    if (op == TOK_MULT || op == TOK_DIV) return 2;
    return 0;
}

void convert_to_rpn() {
    Token op_stack[MAX_TOKENS];
    int op_count = 0;
    rpn_count = 0;
    
    for (int i = 0; i < token_count; i++) {
        Token tok = tokens[i];
        
        if (tok.type == TOK_NUMBER || tok.type == TOK_VARIABLE) {
            // Operands go directly to output
            rpn_queue[rpn_count++] = tok;
        }
        else if (tok.type == TOK_LPAREN) {
            // Left paren goes to operator stack
            op_stack[op_count++] = tok;
        }
        else if (tok.type == TOK_RPAREN) {
            // Pop until left paren
            while (op_count > 0 && op_stack[op_count-1].type != TOK_LPAREN) {
                rpn_queue[rpn_count++] = op_stack[--op_count];
            }
            if (op_count > 0) op_count--; // Remove left paren
        }
        else if (tok.type == TOK_PLUS || tok.type == TOK_MINUS || 
                 tok.type == TOK_MULT || tok.type == TOK_DIV) {
            // Pop operators with higher or equal precedence
            while (op_count > 0 && 
                   op_stack[op_count-1].type != TOK_LPAREN &&
                   get_precedence(op_stack[op_count-1].type) >= get_precedence(tok.type)) {
                rpn_queue[rpn_count++] = op_stack[--op_count];
            }
            op_stack[op_count++] = tok;
        }
    }
    
    // Pop remaining operators
    while (op_count > 0) {
        rpn_queue[rpn_count++] = op_stack[--op_count];
    }
}

// ============================================================================
// EXPRESSION EVALUATION WITH CODE GENERATION
// ============================================================================

int evaluate_rpn(int64_t *result_value) {
    int reg_stack[MAX_TOKENS];
    int64_t val_stack[MAX_TOKENS];
    int stack_count = 0;
    
    for (int i = 0; i < rpn_count; i++) {
        Token tok = rpn_queue[i];
        
        if (tok.type == TOK_NUMBER) {
            // Load immediate value
            int reg = temp_reg_counter++;
            emit_daddiu(reg, 0, tok.num_value);
            reg_stack[stack_count] = reg;
            val_stack[stack_count] = tok.num_value;
            stack_count++;
        }
        else if (tok.type == TOK_VARIABLE) {
            // Check if variable exists
            Variable *var = get_variable(tok.str);
            if (!var) {
                report_error("Variable not declared", tok.str);
                return temp_reg_counter++;
            }
            
            // Load variable from memory
            int reg = temp_reg_counter++;
            emit_ld(reg, tok.str, var->mem_offset);
            reg_stack[stack_count] = reg;
            val_stack[stack_count] = var->value;
            stack_count++;
        }
        else if (tok.type == TOK_PLUS || tok.type == TOK_MINUS || 
                 tok.type == TOK_MULT || tok.type == TOK_DIV) {
            if (stack_count < 2) {
                report_error("Invalid expression", NULL);
                return temp_reg_counter++;
            }
            
            int right_reg = reg_stack[--stack_count];
            int left_reg = reg_stack[--stack_count];
            int64_t right_val = val_stack[stack_count];
            int64_t left_val = val_stack[stack_count-1];
            
            int result_reg = temp_reg_counter++;
            int64_t result = 0;
            
            if (tok.type == TOK_PLUS) {
                emit_daddu(result_reg, left_reg, right_reg);
                result = left_val + right_val;
            }
            else if (tok.type == TOK_MINUS) {
                emit_dsubu(result_reg, left_reg, right_reg);
                result = left_val - right_val;
            }
            else if (tok.type == TOK_MULT) {
                emit_dmult(left_reg, right_reg, result_reg);
                result = left_val * right_val;
            }
            else if (tok.type == TOK_DIV) {
                emit_ddiv(left_reg, right_reg, result_reg);
                result = (right_val != 0) ? (left_val / right_val) : 0;
            }
            
            reg_stack[stack_count] = result_reg;
            val_stack[stack_count] = result;
            stack_count++;
        }
    }
    
    if (stack_count != 1) {
        report_error("Invalid expression evaluation", NULL);
        return temp_reg_counter++;
    }
    
    *result_value = val_stack[0];
    return reg_stack[0];
}

// ============================================================================
// STATEMENT PROCESSING
// ============================================================================

void process_statement(char *stmt) {
    // Skip whitespace
    while (isspace(*stmt)) stmt++;
    if (*stmt == '\0') return; // Empty statement
    
    // Check if this is a declaration or expression-only statement
    if (strncmp(stmt, "int ", 4) != 0) {
        // Not a declaration - treat as expression-only statement
        // In C, statements like "3+1;" or "a+b;" are valid but do nothing
        
        // Try to evaluate it as an expression
        tokenize_expression(stmt);
        convert_to_rpn();
        
        int64_t dummy_result = 0;
        temp_reg_counter = 1;
        int dummy_reg = evaluate_rpn(&dummy_result);
        
        // Expression is valid, but we don't store the result anywhere
        // This is just like C - the expression is evaluated and discarded
        return;
    }
    
    stmt += 4; // Skip "int "
    
    // Find '='
    char *eq = strchr(stmt, '=');
    if (!eq) {
        report_error("Missing '=' in declaration", stmt);
        return;
    }
    
    // Extract variable name
    char name[32];
    int name_len = 0;
    char *p = stmt;
    while (p < eq && !isspace(*p) && name_len < 31) {
        name[name_len++] = *p++;
    }
    name[name_len] = '\0';
    
    // Validate variable name
    if (!isalpha(name[0]) && name[0] != '_') {
        report_error("Invalid variable name", name);
        return;
    }
    
    // Declare variable
    declare_variable(name);
    
    // Extract expression
    char *expr = eq + 1;
    while (isspace(*expr)) expr++;
    
    // Parse and evaluate expression
    tokenize_expression(expr);
    convert_to_rpn();
    
    int64_t result_value = 0;
    temp_reg_counter = 1; // Reset temp registers
    int result_reg = evaluate_rpn(&result_value);
    
    // Store result
    Variable *var = get_variable(name);
    if (var) {
        var->value = result_value;
        
        // Move result to r3 for consistency
        if (result_reg != 3) {
            emit_daddu(3, result_reg, 0);
        }
        emit_sd(3, name, var->mem_offset);
    }
}

// ============================================================================
// INPUT PROCESSING
// ============================================================================

void check_semicolons(const char *input) {
    bool has_content = false;
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isspace(input[i])) {
            has_content = true;
        }
        if (input[i] == ';') {
            has_content = false;
        }
    }
    if (has_content) {
        report_error("Missing semicolon at end of statement", NULL);
    }
}

void normalize_input(char *s) {
    int i = 0, j = 0;
    bool space = false;
    
    while (s[i]) {
        if (isspace(s[i])) {
            space = true;
        } else {
            if (space && j > 0) s[j++] = ' ';
            s[j++] = s[i];
            space = false;
        }
        i++;
    }
    s[j] = '\0';
}

// ============================================================================
// MAIN
// ============================================================================

int main(void) {
    char program[MAX_INPUT_LEN] = "";
    
    // Read input file
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        fp = fopen("input.txt", "w");
        fprintf(fp, "int a = -50 ;\nint b = a + 10;\nint c = b * 2 + 5;\n");
        fclose(fp);
        fp = fopen("input.txt", "r");
    }
    
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        if (strlen(program) + strlen(line) < MAX_INPUT_LEN) {
            strcat(program, line);
        }
    }
    fclose(fp);
    
    printf("Input Code:\n");
    printf("===========\n%s\n", program);
    printf("===========\n\n");
    
    // Check for missing semicolons
    check_semicolons(program);
    
    // Normalize input
    normalize_input(program);
    
    // Process statements
    char *stmt = strtok(program, ";");
    while (stmt != NULL) {
        process_statement(stmt);
        stmt = strtok(NULL, ";");
    }
    
    // Output results
    if (error_count > 0) {
        printf("\n[BUILD FAILED] %d error(s) found.\n", error_count);
        return 1;
    }
    
    printf("MIPS64 ASSEMBLY OUTPUT:\n");
    printf("=======================\n\n");
    
    printf(".data\n");
    for (int i = 0; i < var_count; i++) {
        printf("%s offset --> %d\n", variables[i].name, variables[i].mem_offset);
    }
    printf("\n");
    
    for (int i = 0; i < instr_count; i++) {
        printf(" Assembly: %s\n", instrs[i].assembly);
        printf(" Binary:   %.6s %.5s %.5s %.5s %.5s %.6s\n",
               &instrs[i].binary[0], &instrs[i].binary[6],
               &instrs[i].binary[11], &instrs[i].binary[16],
               &instrs[i].binary[21], &instrs[i].binary[26]);
        printf("           [Opcode][ rs ][ rt ][ rd ][shmt][funct]\n");
        printf("            Hex: 0x%s\n\n", instrs[i].hex);
    }
    
    printf("FINAL VARIABLE VALUES:\n");
    printf("======================\n");
    for (int i = 0; i < var_count; i++) {
        printf(" %s = %" PRId64 "\n", variables[i].name, variables[i].value);
    }
    
    return 0;
}
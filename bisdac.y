%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* --- Type Definitions --- */
typedef enum { 
    TYPE_NONE = 0, 
    TYPE_INT, 
    TYPE_STR, 
    TYPE_CHAR 
} VarType;

/* --- Lexer Interface --- */
extern int yylineno;
extern char *yytext;
int yylex(void);
void yyerror(const char *s);

/* --- Global Error Counter --- */
int error_count = 0;

/* --- MIPS64 Opcode Constants --- */
#define OPCODE_SPECIAL   0x00
#define OPCODE_DADDIU    0x19
#define OPCODE_LD        0x37
#define OPCODE_SD        0x3F
#define FUNCT_DADDU      0x2D
#define FUNCT_DSUBU      0x2F
#define FUNCT_DMULT      0x1C
#define FUNCT_DDIV       0x1E
#define FUNCT_MFLO       0x12

/* --- Symbol Table --- */
typedef struct {
    char *name;
    VarType type;
    int64_t value;
    int mem_offset;
    bool is_declared;
} Symbol;

#define MAX_SYMBOLS 1024
static Symbol symtab[MAX_SYMBOLS];
static int sym_count = 0;
static int next_offset = 8;

/* --- String Constants Table --- */
typedef struct {
    char *label;
    char *value;
} StringConst;

#define MAX_STRINGS 256
static StringConst str_consts[MAX_STRINGS];
static int str_count = 0;

/* --- Instruction Buffer --- */
typedef struct {
    char assembly[128];
    char binary[33];
    char hex[9];
} Instruction;

#define MAX_INSTRUCTIONS 4096
static Instruction instructions[MAX_INSTRUCTIONS];
static int instr_count = 0;

/* --- Output Buffer --- */
static char output_buffer[8192] = "";

/* --- Register Allocation Variables --- */
static bool reg_used[32] = {false};
static int last_reg_idx = 0; // Tracks the last allocated register for cycling

/* --- Helper Function Prototypes --- */
static Symbol* sym_lookup(const char *name);
static Symbol* sym_declare(const char *name, VarType type);

static void emit_instruction(const char *asm_text, uint32_t encoding);
static void emit_daddiu(int rt, int rs, int imm);
static void emit_daddu(int rd, int rs, int rt);
static void emit_dsubu(int rd, int rs, int rt);
static void emit_dmult(int rs, int rt, int rd);
static void emit_ddiv(int rs, int rt, int rd);
static void emit_ld(int rt, const char *var_name, int offset);
static void emit_sd(int rt, const char *var_name, int offset);

static int alloc_register(void);
static void free_register(int reg);
static void int_to_binary(uint32_t val, char *out);
static char* clean_string(const char *str);
static void append_output(const char *str);

static void print_results(void);
%}

/* ADDED: is_const boolean flag to expr_info */
%union {
    int num;
    char *str;
    struct {
        int reg;
        long long value;
        int type;
        bool is_const; 
    } expr_info;
}

%token NUMERO IPAGAWAS LITIR
%token ADDEQ SUBEQ MULEQ DIVEQ
%token <num> NUMBER
%token <str> IDEN STR CHR
%token ADD SUB MUL DIV EQL END OPEN CLOSE

%type <expr_info> expr

%left ADD SUB
%left MUL DIV
%right UMINUS

%%

program:
      /* empty */
    | program statement
    ;

statement:
      /* --- Error Recovery --- */
      error END {
          yyerrok;
      }

      /* --- Declarations --- */
    | NUMERO IDEN EQL expr END {
          if ($4.type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): 'numero' requires integer expression\n", yylineno);
              error_count++;
          }
          Symbol *sym = sym_declare($2, TYPE_INT);
          if (sym) {
            sym->value = $4.value;
            emit_sd($4.reg, $2, sym->mem_offset);
          }
          free_register($4.reg);
          free($2);
      }
    | NUMERO IDEN END {
          Symbol *sym = sym_declare($2, TYPE_INT);
          if (sym) {
            sym->value = 0;
            int r = alloc_register();
            emit_daddiu(r, 0, 0);
            emit_sd(r, $2, sym->mem_offset);
            free_register(r);
          }
          free($2);
      }
    | LITIR IDEN EQL STR END {
          Symbol *sym = sym_declare($2, TYPE_STR);
          if (sym) {
            char *cleaned = clean_string($4);
            str_consts[str_count].label = malloc(64);
            sprintf(str_consts[str_count].label, "str_%d", str_count);
            str_consts[str_count].value = strdup(cleaned);
            sym->mem_offset = str_count;
            str_count++;
            free(cleaned);
          }
          free($2);
          free($4);
      }
    | LITIR IDEN EQL CHR END {
          Symbol *sym = sym_declare($2, TYPE_CHAR);
          if (sym) {
            char ch = $4[1];
            sym->value = (int64_t)ch;
            int r = alloc_register();
            emit_daddiu(r, 0, (int)ch);
            emit_sd(r, $2, sym->mem_offset);
            free_register(r);
          }
          free($2);
          free($4);
      }
    | LITIR IDEN END {
          Symbol *sym = sym_declare($2, TYPE_STR);
          if (sym) {
            str_consts[str_count].label = malloc(64);
            sprintf(str_consts[str_count].label, "str_%d", str_count);
            str_consts[str_count].value = strdup("");
            sym->mem_offset = str_count;
            str_count++;
          }
          free($2);
      }
      
      /* --- Assignments --- */
    | IDEN EQL expr END {
          Symbol *sym = sym_lookup($1);
          if (!sym) {
              fprintf(stderr, "Error (line %d): Variable '%s' not declared\n", yylineno, $1);
              error_count++;
          } else {
              if (sym->type == TYPE_INT && $3.type != TYPE_INT) {
                  fprintf(stderr, "Error (line %d): Cannot assign non-integer to numero variable\n", yylineno);
                  error_count++;
              }
              sym->value = $3.value;
              emit_sd($3.reg, $1, sym->mem_offset);
          }
          free_register($3.reg);
          free($1);
      }
    | IDEN ADDEQ expr END {
          Symbol *sym = sym_lookup($1);
          if (!sym || sym->type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Variable '%s' must be numero for +=\n", yylineno, $1);
              error_count++;
          } else {
              sym->value += $3.value;
              int r1 = alloc_register();
              int r2 = alloc_register();
              emit_ld(r1, $1, sym->mem_offset);
              emit_daddu(r2, r1, $3.reg);
              emit_sd(r2, $1, sym->mem_offset);
              free_register(r1); free_register(r2);
          }
          free_register($3.reg); free($1);
      }
    | IDEN SUBEQ expr END {
          Symbol *sym = sym_lookup($1);
          if (!sym || sym->type != TYPE_INT) {
             fprintf(stderr, "Error (line %d): Variable '%s' must be numero for -=\n", yylineno, $1);
             error_count++;
          } else {
             sym->value -= $3.value;
             int r1 = alloc_register();
             int r2 = alloc_register();
             emit_ld(r1, $1, sym->mem_offset);
             emit_dsubu(r2, r1, $3.reg);
             emit_sd(r2, $1, sym->mem_offset);
             free_register(r1); free_register(r2);
          }
          free_register($3.reg); free($1);
    }
    | IDEN MULEQ expr END {
          Symbol *sym = sym_lookup($1);
          if (!sym || sym->type != TYPE_INT) {
             fprintf(stderr, "Error (line %d): Variable '%s' must be numero for *=\n", yylineno, $1);
             error_count++;
          } else {
             sym->value *= $3.value;
             int r1 = alloc_register();
             int r2 = alloc_register();
             emit_ld(r1, $1, sym->mem_offset);
             emit_dmult(r1, $3.reg, r2);
             emit_sd(r2, $1, sym->mem_offset);
             free_register(r1); free_register(r2);
          }
          free_register($3.reg); free($1);
    }
    | IDEN DIVEQ expr END {
          Symbol *sym = sym_lookup($1);
          if (!sym || sym->type != TYPE_INT) {
             fprintf(stderr, "Error (line %d): Variable '%s' must be numero for /=\n", yylineno, $1);
             error_count++;
          } else {
             if ($3.value != 0) sym->value /= $3.value;
             int r1 = alloc_register();
             int r2 = alloc_register();
             emit_ld(r1, $1, sym->mem_offset);
             emit_ddiv(r1, $3.reg, r2);
             emit_sd(r2, $1, sym->mem_offset);
             free_register(r1); free_register(r2);
          }
          free_register($3.reg); free($1);
    }

      /* --- PRINT STATEMENT --- */
    | IPAGAWAS print_list END {
          append_output("\n");
      }
    ;

print_list:
      print_item
    | print_list print_item
    ;

print_item:
      STR {
          char *cleaned = clean_string($1);
          append_output(cleaned);
          free(cleaned);
          free($1);
      }
    | IDEN {
          Symbol *sym = sym_lookup($1);
          if (!sym) {
              fprintf(stderr, "Error (line %d): Variable '%s' not declared\n", yylineno, $1);
              error_count++;
          } else {
              char buf[256];
              if (sym->type == TYPE_INT) sprintf(buf, "%lld", (long long)sym->value);
              else if (sym->type == TYPE_CHAR) sprintf(buf, "%c", (char)sym->value);
              else if (sym->type == TYPE_STR) sprintf(buf, "%s", str_consts[sym->mem_offset].value);
              append_output(buf);
          }
          free($1);
      }
    | NUMBER {
          char buf[64];
          sprintf(buf, "%d", $1);
          append_output(buf);
    }
    | OPEN expr CLOSE {
          if ($2.type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Expression in ipakita must be integer\n", yylineno);
              error_count++;
          }
          char buf[64];
          sprintf(buf, "%lld", (long long)$2.value);
          append_output(buf);
          free_register($2.reg);
    }
    ;

/* --- Expressions --- */
expr:
      expr ADD expr {
          if ($1.type != TYPE_INT || $3.type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Arithmetic requires integers\n", yylineno);
              error_count++;
          }
          int result_reg = alloc_register();
          emit_daddu(result_reg, $1.reg, $3.reg);
          free_register($1.reg); free_register($3.reg);
          $$.reg = result_reg;
          $$.value = $1.value + $3.value;
          $$.type = TYPE_INT;
          $$.is_const = false;
      }
    | expr SUB expr {
          if ($1.type != TYPE_INT || $3.type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Arithmetic requires integers\n", yylineno);
              error_count++;
          }
          int result_reg = alloc_register();
          emit_dsubu(result_reg, $1.reg, $3.reg);
          free_register($1.reg); free_register($3.reg);
          $$.reg = result_reg;
          $$.value = $1.value - $3.value;
          $$.type = TYPE_INT;
          $$.is_const = false;
      }
    | expr MUL expr {
          if ($1.type != TYPE_INT || $3.type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Arithmetic requires integers\n", yylineno);
              error_count++;
          }
          int result_reg = alloc_register();
          emit_dmult($1.reg, $3.reg, result_reg);
          free_register($1.reg); free_register($3.reg);
          $$.reg = result_reg;
          $$.value = $1.value * $3.value;
          $$.type = TYPE_INT;
          $$.is_const = false;
      }
    | expr DIV expr {
          if ($1.type != TYPE_INT || $3.type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Arithmetic requires integers\n", yylineno);
              error_count++;
          }
          int result_reg = alloc_register();
          if ($3.value == 0 && error_count == 0) {
               fprintf(stderr, "Error (line %d): Division by zero\n", yylineno);
               error_count++;
          }
          emit_ddiv($1.reg, $3.reg, result_reg);
          free_register($1.reg); free_register($3.reg);
          $$.reg = result_reg;
          if ($3.value != 0) $$.value = $1.value / $3.value; else $$.value = 0;
          $$.type = TYPE_INT;
          $$.is_const = false;
      }
      
      /* --- OPTIMIZED UNARY MINUS --- */
    | SUB expr %prec UMINUS {
          if ($2.type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Unary minus requires integer\n", yylineno);
              error_count++;
          }
          
          if ($2.is_const) {
              // OPTIMIZATION: If it's a pure number, delete the old positive instruction 
              // and replace it with a single negative instruction.
              instr_count--; 
              emit_daddiu($2.reg, 0, -($2.value));
              
              $$.reg = $2.reg;
              $$.value = -($2.value);
              $$.type = TYPE_INT;
              $$.is_const = true; // Still a constant!
          } else {
              // Variable or complex expression minus (e.g. -x)
              int zero_reg = alloc_register();
              int result_reg = alloc_register();
              emit_daddiu(zero_reg, 0, 0);
              emit_dsubu(result_reg, zero_reg, $2.reg);
              free_register(zero_reg); free_register($2.reg);
              
              $$.reg = result_reg;
              $$.value = -($2.value);
              $$.type = TYPE_INT;
              $$.is_const = false;
          }
      }
      
    | OPEN expr CLOSE { 
          $$ = $2; 
      }
    | NUMBER {
          int reg = alloc_register();
          emit_daddiu(reg, 0, $1);
          $$.reg = reg;
          $$.value = $1;
          $$.type = TYPE_INT;
          $$.is_const = true; // This flags it as a pure constant!
      }
    | IDEN {
          Symbol *sym = sym_lookup($1);
          int reg = alloc_register();
          $$.reg = reg;
          $$.type = TYPE_INT;
          $$.value = 0;
          $$.is_const = false; // Variables are not constants
          if (!sym) {
              fprintf(stderr, "Error (line %d): Variable '%s' not declared\n", yylineno, $1);
              error_count++;
          } else if (sym->type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Variable '%s' is not numero\n", yylineno, $1);
              error_count++;
          } else {
              emit_ld(reg, $1, sym->mem_offset);
              $$.value = sym->value;
          }
          free($1);
      }
    ;
%%

/* ========================================================================== */
/* HELPER FUNCTION IMPLEMENTATIONS */
/* ========================================================================== */

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error (line %d): %s\n", yylineno, s);
    error_count++;
}

static Symbol* sym_lookup(const char *name) {
    for (int i = 0; i < sym_count; i++) {
        if (strcmp(symtab[i].name, name) == 0) return &symtab[i];
    }
    return NULL;
}

static Symbol* sym_declare(const char *name, VarType type) {
    Symbol *existing = sym_lookup(name);
    if (existing) {
        fprintf(stderr, "Error (line %d): Variable '%s' already declared\n", yylineno, name);
        error_count++;
        return NULL; 
    }
    if (sym_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Error: Symbol table full\n");
        exit(1);
    }
    symtab[sym_count].name = strdup(name);
    symtab[sym_count].type = type;
    symtab[sym_count].value = 0;
    symtab[sym_count].mem_offset = next_offset;
    symtab[sym_count].is_declared = true;
    next_offset += 8;
    return &symtab[sym_count++];
}

static void int_to_binary(uint32_t val, char *out) {
    for (int i = 31; i >= 0; i--) {
        out[31-i] = ((val >> i) & 1) ? '1' : '0';
    }
    out[32] = '\0';
}

static void emit_instruction(const char *asm_text, uint32_t encoding) {
    if (instr_count >= MAX_INSTRUCTIONS) return;
    strcpy(instructions[instr_count].assembly, asm_text);
    int_to_binary(encoding, instructions[instr_count].binary);
    sprintf(instructions[instr_count].hex, "%08X", encoding);
    instr_count++;
}

/* (Functions emit_daddiu, emit_daddu, etc... are standard) */
static void emit_daddiu(int rt, int rs, int imm) {
    char buf[128];
    sprintf(buf, "DADDIU r%d, r%d, %d", rt, rs, imm);
    uint32_t enc = (OPCODE_DADDIU << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF);
    emit_instruction(buf, enc);
}
static void emit_daddu(int rd, int rs, int rt) {
    char buf[128];
    sprintf(buf, "DADDU r%d, r%d, r%d", rd, rs, rt);
    uint32_t enc = (OPCODE_SPECIAL << 26) | (rs << 21) | (rt << 16) | (rd << 11) | FUNCT_DADDU;
    emit_instruction(buf, enc);
}
static void emit_dsubu(int rd, int rs, int rt) {
    char buf[128];
    sprintf(buf, "DSUBU r%d, r%d, r%d", rd, rs, rt);
    uint32_t enc = (OPCODE_SPECIAL << 26) | (rs << 21) | (rt << 16) | (rd << 11) | FUNCT_DSUBU;
    emit_instruction(buf, enc);
}
static void emit_dmult(int rs, int rt, int rd) {
    char buf[128];
    sprintf(buf, "DMULT r%d, r%d", rs, rt);
    uint32_t enc = (OPCODE_SPECIAL << 26) | (rs << 21) | (rt << 16) | FUNCT_DMULT;
    emit_instruction(buf, enc);
    sprintf(buf, "MFLO r%d", rd);
    enc = (OPCODE_SPECIAL << 26) | (rd << 11) | FUNCT_MFLO;
    emit_instruction(buf, enc);
}
static void emit_ddiv(int rs, int rt, int rd) {
    char buf[128];
    sprintf(buf, "DDIV r%d, r%d", rs, rt);
    uint32_t enc = (OPCODE_SPECIAL << 26) | (rs << 21) | (rt << 16) | FUNCT_DDIV;
    emit_instruction(buf, enc);
    sprintf(buf, "MFLO r%d", rd);
    enc = (OPCODE_SPECIAL << 26) | (rd << 11) | FUNCT_MFLO;
    emit_instruction(buf, enc);
}
static void emit_ld(int rt, const char *var_name, int offset) {
    char buf[128];
    sprintf(buf, "LD r%d, %s(r29)", rt, var_name);
    uint32_t enc = (OPCODE_LD << 26) | (29 << 21) | (rt << 16) | (offset & 0xFFFF);
    emit_instruction(buf, enc);
}
static void emit_sd(int rt, const char *var_name, int offset) {
    char buf[128];
    sprintf(buf, "SD r%d, %s(r29)", rt, var_name);
    uint32_t enc = (OPCODE_SD << 26) | (29 << 21) | (rt << 16) | (offset & 0xFFFF);
    emit_instruction(buf, enc);
}

/* --- REGISTER ALLOCATOR (Cyclic / Round-Robin) --- */
static int alloc_register(void) {
    int count = 0;
    while (count < 25) {
        last_reg_idx++;
        if (last_reg_idx > 25) last_reg_idx = 1; // Wrap around to 1

        if (!reg_used[last_reg_idx]) {
            reg_used[last_reg_idx] = true;
            return last_reg_idx;
        }
        count++;
    }
    fprintf(stderr, "Error: Out of registers\n");
    return 1;
}

static void free_register(int reg) {
    if (reg > 0 && reg < 32) reg_used[reg] = false;
}

static char* clean_string(const char *str) {
    size_t len = strlen(str);
    if (len < 2) return strdup("");
    char *result = malloc(len - 1);
    strncpy(result, str + 1, len - 2);
    result[len - 2] = '\0';
    return result;
}

static void append_output(const char *str) {
    strcat(output_buffer, str);
}

static void print_results(void) {
    if (strlen(output_buffer) > 0) {
        printf("%s", output_buffer);
    }
    for (int i = 0; i < instr_count; i++) {
        printf("Assembly: %s\n", instructions[i].assembly);
    }
    for (int i = 0; i < instr_count; i++) {
        printf("Binary:   %.6s %.5s %.5s %.5s %.5s %.6s\n",
               &instructions[i].binary[0], &instructions[i].binary[6],
               &instructions[i].binary[11], &instructions[i].binary[16],
               &instructions[i].binary[21], &instructions[i].binary[26]);
        printf("Hex:      0x%s\n", instructions[i].hex);
    }
}

int main(void) {
    yyparse();
    if (error_count == 0) {
        print_results();
    }
    return 0;
}
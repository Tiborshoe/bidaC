/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "bisdac.y"

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

/* Line 371 of yacc.c  */
#line 167 "bisdac.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "bisdac.tab.h".  */
#ifndef YY_YY_BISDAC_TAB_H_INCLUDED
# define YY_YY_BISDAC_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMERO = 258,
     IPAGAWAS = 259,
     LITIR = 260,
     ADDEQ = 261,
     SUBEQ = 262,
     MULEQ = 263,
     DIVEQ = 264,
     NUMBER = 265,
     IDEN = 266,
     STR = 267,
     CHR = 268,
     ADD = 269,
     SUB = 270,
     MUL = 271,
     DIV = 272,
     EQL = 273,
     END = 274,
     OPEN = 275,
     CLOSE = 276,
     UMINUS = 277
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 101 "bisdac.y"

    int num;
    char *str;
    struct {
        int reg;
        long long value;
        int type;
        bool is_const; 
    } expr_info;


/* Line 387 of yacc.c  */
#line 244 "bisdac.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_BISDAC_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 272 "bisdac.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   94

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  23
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  6
/* YYNRULES -- Number of rules.  */
#define YYNRULES  29
/* YYNRULES -- Number of states.  */
#define YYNSTATES  62

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   277

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    16,    20,    26,    32,
      36,    41,    46,    51,    56,    61,    65,    67,    70,    72,
      74,    76,    80,    84,    88,    92,    96,    99,   103,   105
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      24,     0,    -1,    -1,    24,    25,    -1,     1,    19,    -1,
       3,    11,    18,    28,    19,    -1,     3,    11,    19,    -1,
       5,    11,    18,    12,    19,    -1,     5,    11,    18,    13,
      19,    -1,     5,    11,    19,    -1,    11,    18,    28,    19,
      -1,    11,     6,    28,    19,    -1,    11,     7,    28,    19,
      -1,    11,     8,    28,    19,    -1,    11,     9,    28,    19,
      -1,     4,    26,    19,    -1,    27,    -1,    26,    27,    -1,
      12,    -1,    11,    -1,    10,    -1,    20,    28,    21,    -1,
      28,    14,    28,    -1,    28,    15,    28,    -1,    28,    16,
      28,    -1,    28,    17,    28,    -1,    15,    28,    -1,    20,
      28,    21,    -1,    10,    -1,    11,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   126,   126,   128,   133,   138,   153,   163,   177,   189,
     202,   220,   236,   252,   268,   286,   292,   293,   297,   303,
     317,   322,   336,   349,   362,   375,   394,   424,   427,   435
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUMERO", "IPAGAWAS", "LITIR", "ADDEQ",
  "SUBEQ", "MULEQ", "DIVEQ", "NUMBER", "IDEN", "STR", "CHR", "ADD", "SUB",
  "MUL", "DIV", "EQL", "END", "OPEN", "CLOSE", "UMINUS", "$accept",
  "program", "statement", "print_list", "print_item", "expr", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    23,    24,    24,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    26,    26,    27,    27,
      27,    27,    28,    28,    28,    28,    28,    28,    28,    28
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     5,     3,     5,     5,     3,
       4,     4,     4,     4,     4,     3,     1,     2,     1,     1,
       1,     3,     3,     3,     3,     3,     2,     3,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     3,     4,
       0,    20,    19,    18,     0,     0,    16,     0,     0,     0,
       0,     0,     0,     0,     6,    28,    29,     0,     0,     0,
      15,    17,     0,     9,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    21,     0,     0,    11,
      12,    13,    14,    10,     5,    27,    22,    23,    24,    25,
       7,     8
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     8,    15,    16,    29
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -19
static const yytype_int8 yypact[] =
{
     -19,    17,   -19,     0,    25,    19,    27,     5,   -19,   -19,
      16,   -19,   -19,   -19,    22,    -4,   -19,    30,    22,    22,
      22,    22,    22,    22,   -19,   -19,   -19,    22,    22,    29,
     -19,   -19,    28,   -19,    45,    51,    57,    63,    69,    75,
     -19,    37,    22,    22,    22,    22,   -19,    38,    44,   -19,
     -19,   -19,   -19,   -19,   -19,   -19,    39,    39,   -19,   -19,
     -19,   -19
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -19,   -19,   -19,   -19,    32,   -18
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      34,    35,    36,    37,    38,    39,    11,    12,    13,    40,
      41,    18,    19,    20,    21,    30,    14,     2,     3,     9,
       4,     5,     6,    22,    56,    57,    58,    59,     7,    11,
      12,    13,    25,    26,    23,    24,    10,    27,    17,    14,
      47,    48,    28,    42,    43,    44,    45,    31,    32,    33,
      46,    42,    43,    44,    45,    44,    45,    60,    55,    42,
      43,    44,    45,    61,    49,    42,    43,    44,    45,     0,
      50,    42,    43,    44,    45,     0,    51,    42,    43,    44,
      45,     0,    52,    42,    43,    44,    45,     0,    53,    42,
      43,    44,    45,     0,    54
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-19)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
      18,    19,    20,    21,    22,    23,    10,    11,    12,    27,
      28,     6,     7,     8,     9,    19,    20,     0,     1,    19,
       3,     4,     5,    18,    42,    43,    44,    45,    11,    10,
      11,    12,    10,    11,    18,    19,    11,    15,    11,    20,
      12,    13,    20,    14,    15,    16,    17,    15,    18,    19,
      21,    14,    15,    16,    17,    16,    17,    19,    21,    14,
      15,    16,    17,    19,    19,    14,    15,    16,    17,    -1,
      19,    14,    15,    16,    17,    -1,    19,    14,    15,    16,
      17,    -1,    19,    14,    15,    16,    17,    -1,    19,    14,
      15,    16,    17,    -1,    19
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    24,     0,     1,     3,     4,     5,    11,    25,    19,
      11,    10,    11,    12,    20,    26,    27,    11,     6,     7,
       8,     9,    18,    18,    19,    10,    11,    15,    20,    28,
      19,    27,    18,    19,    28,    28,    28,    28,    28,    28,
      28,    28,    14,    15,    16,    17,    21,    12,    13,    19,
      19,    19,    19,    19,    19,    21,    28,    28,    28,    28,
      19,    19
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
/* Line 1792 of yacc.c  */
#line 133 "bisdac.y"
    {
          yyerrok;
      }
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 138 "bisdac.y"
    {
          if ((yyvsp[(4) - (5)].expr_info).type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): 'numero' requires integer expression\n", yylineno);
              error_count++;
          }
          Symbol *sym = sym_declare((yyvsp[(2) - (5)].str), TYPE_INT);
          if (sym) {
            sym->value = (yyvsp[(4) - (5)].expr_info).value;
            // Move result into r3, then store r3 to memory
            emit_daddu(3, 0, (yyvsp[(4) - (5)].expr_info).reg);
            emit_sd(3, (yyvsp[(2) - (5)].str), sym->mem_offset);
          }
          free_register((yyvsp[(4) - (5)].expr_info).reg);
          free((yyvsp[(2) - (5)].str));
      }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 153 "bisdac.y"
    {
          Symbol *sym = sym_declare((yyvsp[(2) - (3)].str), TYPE_INT);
          if (sym) {
            sym->value = 0;
            // Load 0 into r3, then store r3 to memory
            emit_daddiu(3, 0, 0);
            emit_sd(3, (yyvsp[(2) - (3)].str), sym->mem_offset);
          }
          free((yyvsp[(2) - (3)].str));
      }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 163 "bisdac.y"
    {
          Symbol *sym = sym_declare((yyvsp[(2) - (5)].str), TYPE_STR);
          if (sym) {
            char *cleaned = clean_string((yyvsp[(4) - (5)].str));
            str_consts[str_count].label = malloc(64);
            sprintf(str_consts[str_count].label, "str_%d", str_count);
            str_consts[str_count].value = strdup(cleaned);
            sym->mem_offset = str_count;
            str_count++;
            free(cleaned);
          }
          free((yyvsp[(2) - (5)].str));
          free((yyvsp[(4) - (5)].str));
      }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 177 "bisdac.y"
    {
          Symbol *sym = sym_declare((yyvsp[(2) - (5)].str), TYPE_CHAR);
          if (sym) {
            char ch = (yyvsp[(4) - (5)].str)[1];
            sym->value = (int64_t)ch;
            // Load char into r3, then store r3 to memory
            emit_daddiu(3, 0, (int)ch);
            emit_sd(3, (yyvsp[(2) - (5)].str), sym->mem_offset);
          }
          free((yyvsp[(2) - (5)].str));
          free((yyvsp[(4) - (5)].str));
      }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 189 "bisdac.y"
    {
          Symbol *sym = sym_declare((yyvsp[(2) - (3)].str), TYPE_STR);
          if (sym) {
            str_consts[str_count].label = malloc(64);
            sprintf(str_consts[str_count].label, "str_%d", str_count);
            str_consts[str_count].value = strdup("");
            sym->mem_offset = str_count;
            str_count++;
          }
          free((yyvsp[(2) - (3)].str));
      }
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 202 "bisdac.y"
    {
          Symbol *sym = sym_lookup((yyvsp[(1) - (4)].str));
          if (!sym) {
              fprintf(stderr, "Error (line %d): Variable '%s' not declared\n", yylineno, (yyvsp[(1) - (4)].str));
              error_count++;
          } else {
              if (sym->type == TYPE_INT && (yyvsp[(3) - (4)].expr_info).type != TYPE_INT) {
                  fprintf(stderr, "Error (line %d): Cannot assign non-integer to numero variable\n", yylineno);
                  error_count++;
              }
              sym->value = (yyvsp[(3) - (4)].expr_info).value;
              // Move result into r3, then store r3 to memory
              emit_daddu(3, 0, (yyvsp[(3) - (4)].expr_info).reg);
              emit_sd(3, (yyvsp[(1) - (4)].str), sym->mem_offset);
          }
          free_register((yyvsp[(3) - (4)].expr_info).reg);
          free((yyvsp[(1) - (4)].str));
      }
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 220 "bisdac.y"
    {
          Symbol *sym = sym_lookup((yyvsp[(1) - (4)].str));
          if (!sym || sym->type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Variable '%s' must be numero for +=\n", yylineno, (yyvsp[(1) - (4)].str));
              error_count++;
          } else {
              sym->value += (yyvsp[(3) - (4)].expr_info).value;
              int r1 = alloc_register();
              emit_ld(r1, (yyvsp[(1) - (4)].str), sym->mem_offset);
              // Calculate and put result directly in r3
              emit_daddu(3, r1, (yyvsp[(3) - (4)].expr_info).reg);
              emit_sd(3, (yyvsp[(1) - (4)].str), sym->mem_offset);
              free_register(r1); 
          }
          free_register((yyvsp[(3) - (4)].expr_info).reg); free((yyvsp[(1) - (4)].str));
      }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 236 "bisdac.y"
    {
          Symbol *sym = sym_lookup((yyvsp[(1) - (4)].str));
          if (!sym || sym->type != TYPE_INT) {
             fprintf(stderr, "Error (line %d): Variable '%s' must be numero for -=\n", yylineno, (yyvsp[(1) - (4)].str));
             error_count++;
          } else {
             sym->value -= (yyvsp[(3) - (4)].expr_info).value;
             int r1 = alloc_register();
             emit_ld(r1, (yyvsp[(1) - (4)].str), sym->mem_offset);
             // Calculate and put result directly in r3
             emit_dsubu(3, r1, (yyvsp[(3) - (4)].expr_info).reg);
             emit_sd(3, (yyvsp[(1) - (4)].str), sym->mem_offset);
             free_register(r1);
          }
          free_register((yyvsp[(3) - (4)].expr_info).reg); free((yyvsp[(1) - (4)].str));
    }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 252 "bisdac.y"
    {
          Symbol *sym = sym_lookup((yyvsp[(1) - (4)].str));
          if (!sym || sym->type != TYPE_INT) {
             fprintf(stderr, "Error (line %d): Variable '%s' must be numero for *=\n", yylineno, (yyvsp[(1) - (4)].str));
             error_count++;
          } else {
             sym->value *= (yyvsp[(3) - (4)].expr_info).value;
             int r1 = alloc_register();
             emit_ld(r1, (yyvsp[(1) - (4)].str), sym->mem_offset);
             // Calculate and put result directly in r3 (MFLO r3)
             emit_dmult(r1, (yyvsp[(3) - (4)].expr_info).reg, 3);
             emit_sd(3, (yyvsp[(1) - (4)].str), sym->mem_offset);
             free_register(r1);
          }
          free_register((yyvsp[(3) - (4)].expr_info).reg); free((yyvsp[(1) - (4)].str));
    }
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 268 "bisdac.y"
    {
          Symbol *sym = sym_lookup((yyvsp[(1) - (4)].str));
          if (!sym || sym->type != TYPE_INT) {
             fprintf(stderr, "Error (line %d): Variable '%s' must be numero for /=\n", yylineno, (yyvsp[(1) - (4)].str));
             error_count++;
          } else {
             if ((yyvsp[(3) - (4)].expr_info).value != 0) sym->value /= (yyvsp[(3) - (4)].expr_info).value;
             int r1 = alloc_register();
             emit_ld(r1, (yyvsp[(1) - (4)].str), sym->mem_offset);
             // Calculate and put result directly in r3 (MFLO r3)
             emit_ddiv(r1, (yyvsp[(3) - (4)].expr_info).reg, 3);
             emit_sd(3, (yyvsp[(1) - (4)].str), sym->mem_offset);
             free_register(r1);
          }
          free_register((yyvsp[(3) - (4)].expr_info).reg); free((yyvsp[(1) - (4)].str));
    }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 286 "bisdac.y"
    {
          append_output("\n");
      }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 297 "bisdac.y"
    {
          char *cleaned = clean_string((yyvsp[(1) - (1)].str));
          append_output(cleaned);
          free(cleaned);
          free((yyvsp[(1) - (1)].str));
      }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 303 "bisdac.y"
    {
          Symbol *sym = sym_lookup((yyvsp[(1) - (1)].str));
          if (!sym) {
              fprintf(stderr, "Error (line %d): Variable '%s' not declared\n", yylineno, (yyvsp[(1) - (1)].str));
              error_count++;
          } else {
              char buf[256];
              if (sym->type == TYPE_INT) sprintf(buf, "%lld", (long long)sym->value);
              else if (sym->type == TYPE_CHAR) sprintf(buf, "%c", (char)sym->value);
              else if (sym->type == TYPE_STR) sprintf(buf, "%s", str_consts[sym->mem_offset].value);
              append_output(buf);
          }
          free((yyvsp[(1) - (1)].str));
      }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 317 "bisdac.y"
    {
          char buf[64];
          sprintf(buf, "%d", (yyvsp[(1) - (1)].num));
          append_output(buf);
    }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 322 "bisdac.y"
    {
          if ((yyvsp[(2) - (3)].expr_info).type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Expression in ipakita must be integer\n", yylineno);
              error_count++;
          }
          char buf[64];
          sprintf(buf, "%lld", (long long)(yyvsp[(2) - (3)].expr_info).value);
          append_output(buf);
          free_register((yyvsp[(2) - (3)].expr_info).reg);
    }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 336 "bisdac.y"
    {
          if ((yyvsp[(1) - (3)].expr_info).type != TYPE_INT || (yyvsp[(3) - (3)].expr_info).type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Arithmetic requires integers\n", yylineno);
              error_count++;
          }
          int result_reg = alloc_register();
          emit_daddu(result_reg, (yyvsp[(1) - (3)].expr_info).reg, (yyvsp[(3) - (3)].expr_info).reg);
          free_register((yyvsp[(1) - (3)].expr_info).reg); free_register((yyvsp[(3) - (3)].expr_info).reg);
          (yyval.expr_info).reg = result_reg;
          (yyval.expr_info).value = (yyvsp[(1) - (3)].expr_info).value + (yyvsp[(3) - (3)].expr_info).value;
          (yyval.expr_info).type = TYPE_INT;
          (yyval.expr_info).is_const = false;
      }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 349 "bisdac.y"
    {
          if ((yyvsp[(1) - (3)].expr_info).type != TYPE_INT || (yyvsp[(3) - (3)].expr_info).type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Arithmetic requires integers\n", yylineno);
              error_count++;
          }
          int result_reg = alloc_register();
          emit_dsubu(result_reg, (yyvsp[(1) - (3)].expr_info).reg, (yyvsp[(3) - (3)].expr_info).reg);
          free_register((yyvsp[(1) - (3)].expr_info).reg); free_register((yyvsp[(3) - (3)].expr_info).reg);
          (yyval.expr_info).reg = result_reg;
          (yyval.expr_info).value = (yyvsp[(1) - (3)].expr_info).value - (yyvsp[(3) - (3)].expr_info).value;
          (yyval.expr_info).type = TYPE_INT;
          (yyval.expr_info).is_const = false;
      }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 362 "bisdac.y"
    {
          if ((yyvsp[(1) - (3)].expr_info).type != TYPE_INT || (yyvsp[(3) - (3)].expr_info).type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Arithmetic requires integers\n", yylineno);
              error_count++;
          }
          int result_reg = alloc_register();
          emit_dmult((yyvsp[(1) - (3)].expr_info).reg, (yyvsp[(3) - (3)].expr_info).reg, result_reg);
          free_register((yyvsp[(1) - (3)].expr_info).reg); free_register((yyvsp[(3) - (3)].expr_info).reg);
          (yyval.expr_info).reg = result_reg;
          (yyval.expr_info).value = (yyvsp[(1) - (3)].expr_info).value * (yyvsp[(3) - (3)].expr_info).value;
          (yyval.expr_info).type = TYPE_INT;
          (yyval.expr_info).is_const = false;
      }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 375 "bisdac.y"
    {
          if ((yyvsp[(1) - (3)].expr_info).type != TYPE_INT || (yyvsp[(3) - (3)].expr_info).type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Arithmetic requires integers\n", yylineno);
              error_count++;
          }
          int result_reg = alloc_register();
          if ((yyvsp[(3) - (3)].expr_info).value == 0 && error_count == 0) {
               fprintf(stderr, "Error (line %d): Division by zero\n", yylineno);
               error_count++;
          }
          emit_ddiv((yyvsp[(1) - (3)].expr_info).reg, (yyvsp[(3) - (3)].expr_info).reg, result_reg);
          free_register((yyvsp[(1) - (3)].expr_info).reg); free_register((yyvsp[(3) - (3)].expr_info).reg);
          (yyval.expr_info).reg = result_reg;
          if ((yyvsp[(3) - (3)].expr_info).value != 0) (yyval.expr_info).value = (yyvsp[(1) - (3)].expr_info).value / (yyvsp[(3) - (3)].expr_info).value; else (yyval.expr_info).value = 0;
          (yyval.expr_info).type = TYPE_INT;
          (yyval.expr_info).is_const = false;
      }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 394 "bisdac.y"
    {
          if ((yyvsp[(2) - (2)].expr_info).type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Unary minus requires integer\n", yylineno);
              error_count++;
          }
          
          if ((yyvsp[(2) - (2)].expr_info).is_const) {
              // OPTIMIZATION: If it's a pure number, delete the old positive instruction 
              // and replace it with a single negative instruction.
              instr_count--; 
              emit_daddiu((yyvsp[(2) - (2)].expr_info).reg, 0, -((yyvsp[(2) - (2)].expr_info).value));
              
              (yyval.expr_info).reg = (yyvsp[(2) - (2)].expr_info).reg;
              (yyval.expr_info).value = -((yyvsp[(2) - (2)].expr_info).value);
              (yyval.expr_info).type = TYPE_INT;
              (yyval.expr_info).is_const = true;
          } else {
              int zero_reg = alloc_register();
              int result_reg = alloc_register();
              emit_daddiu(zero_reg, 0, 0);
              emit_dsubu(result_reg, zero_reg, (yyvsp[(2) - (2)].expr_info).reg);
              free_register(zero_reg); free_register((yyvsp[(2) - (2)].expr_info).reg);
              
              (yyval.expr_info).reg = result_reg;
              (yyval.expr_info).value = -((yyvsp[(2) - (2)].expr_info).value);
              (yyval.expr_info).type = TYPE_INT;
              (yyval.expr_info).is_const = false;
          }
      }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 424 "bisdac.y"
    { 
          (yyval.expr_info) = (yyvsp[(2) - (3)].expr_info); 
      }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 427 "bisdac.y"
    {
          int reg = alloc_register();
          emit_daddiu(reg, 0, (yyvsp[(1) - (1)].num));
          (yyval.expr_info).reg = reg;
          (yyval.expr_info).value = (yyvsp[(1) - (1)].num);
          (yyval.expr_info).type = TYPE_INT;
          (yyval.expr_info).is_const = true; // Flags it as a pure constant
      }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 435 "bisdac.y"
    {
          Symbol *sym = sym_lookup((yyvsp[(1) - (1)].str));
          int reg = alloc_register();
          (yyval.expr_info).reg = reg;
          (yyval.expr_info).type = TYPE_INT;
          (yyval.expr_info).value = 0;
          (yyval.expr_info).is_const = false; // Variables are not constants
          if (!sym) {
              fprintf(stderr, "Error (line %d): Variable '%s' not declared\n", yylineno, (yyvsp[(1) - (1)].str));
              error_count++;
          } else if (sym->type != TYPE_INT) {
              fprintf(stderr, "Error (line %d): Variable '%s' is not numero\n", yylineno, (yyvsp[(1) - (1)].str));
              error_count++;
          } else {
              emit_ld(reg, (yyvsp[(1) - (1)].str), sym->mem_offset);
              (yyval.expr_info).value = sym->value;
          }
          free((yyvsp[(1) - (1)].str));
      }
    break;


/* Line 1792 of yacc.c  */
#line 1927 "bisdac.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 455 "bisdac.y"


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

        // CRITICAL CHANGE: Always skip r3 so it is purely reserved for SD logic
        if (last_reg_idx == 3) {
            count++;
            continue; 
        }

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
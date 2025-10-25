/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "testlang.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern int yylex();
extern FILE *yyin;

FILE *out;

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
/* Escape a DSL string (including quotes) so it can be safely embedded
   inside a Java string literal. Escapes backslashes and double quotes. */
static char* escape_java_literal(const char* s) {
    size_t len = strlen(s);
    /* Worst case every char needs a backslash prefix */
    char* buf = (char*)malloc(len * 2 + 1);
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        char c = s[i];
        if (c == '"' || c == '\\') {
            buf[j++] = '\\';
        }
        buf[j++] = c;
    }
    buf[j] = '\0';
    return buf;
}
/* Return a newly allocated copy of s with a single pair of leading/trailing
   double quotes removed if present. */
static char* strip_outer_quotes(const char* s) {
    size_t len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        size_t n = len - 2;
        char* out = (char*)malloc(n + 1);
        memcpy(out, s + 1, n);
        out[n] = '\0';
        return out;
    }
    /* No surrounding quotes; return a duplicate */
    char* out = (char*)malloc(len + 1);
    memcpy(out, s, len + 1);
    return out;
}
/* Buffer for deferred assertions emitted in assert_section */
static char assert_buffer[65536];
static void asserts_reset() { assert_buffer[0] = '\0'; }
static void asserts_append(const char* s) {
    size_t cur = strlen(assert_buffer);
    size_t add = strlen(s);
    if (cur + add + 1 < sizeof(assert_buffer)) {
        memcpy(assert_buffer + cur, s, add + 1);
    }
}
/* Buffer for default headers from config */
static char default_headers[8192];
static void headers_append(const char* s) {
    size_t cur = strlen(default_headers);
    size_t add = strlen(s);
    if (cur + add + 1 < sizeof(default_headers)) {
        memcpy(default_headers + cur, s, add + 1);
    }
}

/* Build a Java expression for URL from a DSL string that may include $vars.
   If the path is relative (starts with '/'), prefix with BASE_URL. */
static char* build_url_expr_from_string(const char* yylval_str) {
    const char* s = yylval_str;
    size_t len = strlen(s);
    const char* p = s;
    const char* end = s + len;
    if (len >= 2 && s[0] == '"' && s[len-1] == '"') {
        p = s + 1;
        end = s + len - 1;
    }
    char buf[4096];
    size_t pos = 0;
    int is_abs = ((end - p) >= 4 && strncmp(p, "http", 4) == 0);
    if (!is_abs) {
        pos += snprintf(buf + pos, sizeof(buf) - pos, "BASE_URL + ");
    }
    int in_quote = 0;
    pos += snprintf(buf + pos, sizeof(buf) - pos, "\"");
    in_quote = 1;
    while (p < end && pos < sizeof(buf) - 1) {
        if (*p == '$') {
            if (in_quote) {
                pos += snprintf(buf + pos, sizeof(buf) - pos, "\" + ");
                in_quote = 0;
            }
            p++;
            const char* start = p;
            while (p < end && (isalnum((unsigned char)*p) || *p == '_')) p++;
            size_t n = (size_t)(p - start);
            char var[128];
            if (n > sizeof(var) - 1) n = sizeof(var) - 1;
            memcpy(var, start, n);
            var[n] = '\0';
            pos += snprintf(buf + pos, sizeof(buf) - pos, "%s + ", var);
            pos += snprintf(buf + pos, sizeof(buf) - pos, "\"");
            in_quote = 1;
            continue;
        }
        if (*p == '\\' || *p == '"') {
            pos += snprintf(buf + pos, sizeof(buf) - pos, "\\%c", *p);
        } else {
            pos += snprintf(buf + pos, sizeof(buf) - pos, "%c", *p);
        }
        p++;
    }
    if (in_quote) {
        pos += snprintf(buf + pos, sizeof(buf) - pos, "\"");
    }
    char* out = (char*)malloc(pos + 1);
    memcpy(out, buf, pos);
    out[pos] = '\0';
    return out;
}

#line 194 "testlang.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "testlang.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TEST = 3,                       /* TEST  */
  YYSYMBOL_REQUEST = 4,                    /* REQUEST  */
  YYSYMBOL_ASSERT = 5,                     /* ASSERT  */
  YYSYMBOL_METHOD = 6,                     /* METHOD  */
  YYSYMBOL_URL = 7,                        /* URL  */
  YYSYMBOL_HEADERS = 8,                    /* HEADERS  */
  YYSYMBOL_BODY = 9,                       /* BODY  */
  YYSYMBOL_STRING = 10,                    /* STRING  */
  YYSYMBOL_IDENTIFIER = 11,                /* IDENTIFIER  */
  YYSYMBOL_HTTP_METHOD = 12,               /* HTTP_METHOD  */
  YYSYMBOL_VARREF = 13,                    /* VARREF  */
  YYSYMBOL_NUMBER = 14,                    /* NUMBER  */
  YYSYMBOL_EQ = 15,                        /* EQ  */
  YYSYMBOL_CONTAINS = 16,                  /* CONTAINS  */
  YYSYMBOL_ASSIGN = 17,                    /* ASSIGN  */
  YYSYMBOL_SEMICOLON = 18,                 /* SEMICOLON  */
  YYSYMBOL_CONFIG = 19,                    /* CONFIG  */
  YYSYMBOL_BASE_URL = 20,                  /* BASE_URL  */
  YYSYMBOL_HEADER = 21,                    /* HEADER  */
  YYSYMBOL_LET = 22,                       /* LET  */
  YYSYMBOL_LBRACE = 23,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 24,                    /* RBRACE  */
  YYSYMBOL_COLON = 25,                     /* COLON  */
  YYSYMBOL_DOT = 26,                       /* DOT  */
  YYSYMBOL_YYACCEPT = 27,                  /* $accept  */
  YYSYMBOL_program = 28,                   /* program  */
  YYSYMBOL_config_opt = 29,                /* config_opt  */
  YYSYMBOL_config_body = 30,               /* config_body  */
  YYSYMBOL_config_item = 31,               /* config_item  */
  YYSYMBOL_var_list_opt = 32,              /* var_list_opt  */
  YYSYMBOL_var_list = 33,                  /* var_list  */
  YYSYMBOL_var_decl = 34,                  /* var_decl  */
  YYSYMBOL_value = 35,                     /* value  */
  YYSYMBOL_test_list = 36,                 /* test_list  */
  YYSYMBOL_test = 37,                      /* test  */
  YYSYMBOL_38_1 = 38,                      /* $@1  */
  YYSYMBOL_test_body = 39,                 /* test_body  */
  YYSYMBOL_request_section = 40,           /* request_section  */
  YYSYMBOL_method_decl = 41,               /* method_decl  */
  YYSYMBOL_url_decl = 42,                  /* url_decl  */
  YYSYMBOL_headers_opt = 43,               /* headers_opt  */
  YYSYMBOL_header_list = 44,               /* header_list  */
  YYSYMBOL_header = 45,                    /* header  */
  YYSYMBOL_body_opt = 46,                  /* body_opt  */
  YYSYMBOL_assert_section = 47,            /* assert_section  */
  YYSYMBOL_assert_list = 48,               /* assert_list  */
  YYSYMBOL_assert = 49                     /* assert  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   88

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  27
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  37
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  82

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   281


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   145,   145,   149,   151,   155,   156,   160,   163,   171,
     173,   177,   178,   182,   188,   189,   198,   199,   203,   203,
     232,   237,   241,   247,   252,   257,   259,   263,   264,   268,
     274,   276,   283,   287,   288,   292,   300,   318
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TEST", "REQUEST",
  "ASSERT", "METHOD", "URL", "HEADERS", "BODY", "STRING", "IDENTIFIER",
  "HTTP_METHOD", "VARREF", "NUMBER", "EQ", "CONTAINS", "ASSIGN",
  "SEMICOLON", "CONFIG", "BASE_URL", "HEADER", "LET", "LBRACE", "RBRACE",
  "COLON", "DOT", "$accept", "program", "config_opt", "config_body",
  "config_item", "var_list_opt", "var_list", "var_decl", "value",
  "test_list", "test", "$@1", "test_body", "request_section",
  "method_decl", "url_decl", "headers_opt", "header_list", "header",
  "body_opt", "assert_section", "assert_list", "assert", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-18)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -13,    -3,    10,    -1,    -4,   -18,     2,    19,    -1,   -18,
       6,    14,   -17,   -18,     8,    15,    19,   -18,   -18,    17,
      11,   -18,   -18,    -5,     7,   -18,    13,    22,   -18,   -18,
      16,   -18,   -18,    18,   -18,    25,   -18,    12,     9,    32,
      33,   -18,    20,   -18,    21,    31,    29,    30,    23,    36,
     -14,    -9,   -18,   -18,    -2,    24,    40,    27,    34,   -18,
     -18,   -18,   -18,    41,    28,    26,   -18,     3,    35,   -10,
     -18,    42,   -18,    44,    45,    46,   -18,   -18,   -18,   -18,
     -18,   -18
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     9,     0,     1,     0,     0,    10,    11,
       0,     0,     0,     5,     0,     0,     2,    16,    12,     0,
       0,     4,     6,     0,     0,    17,     0,     0,    14,    15,
       0,    18,     7,     0,    13,     0,     8,     0,     0,     0,
       0,    19,     0,    20,     0,     0,     0,     0,     0,    25,
       0,     0,    33,    22,     0,     0,    30,     0,     0,    32,
      34,    23,    24,     0,     0,     0,    35,     0,     0,     0,
      27,     0,    21,     0,     0,     0,    26,    28,    31,    36,
      37,    29
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -18,   -18,   -18,   -18,    47,   -18,   -18,    49,   -18,   -18,
      48,   -18,   -18,   -18,   -18,   -18,   -18,   -18,   -11,   -18,
     -18,   -18,    37
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,    12,    13,     7,     8,     9,    30,    16,
      17,    35,    38,    39,    45,    49,    56,    69,    70,    65,
      43,    51,    52
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      68,    57,    50,    10,    11,    28,     1,    21,    61,    29,
       5,    62,    58,    14,    76,    59,    10,    11,    73,    74,
       4,     6,    15,    19,    20,    23,    24,    26,    27,    37,
      31,    32,    33,    41,    34,    40,    36,    42,    48,    44,
      50,    66,    53,    46,    55,    67,    47,    63,    54,    64,
      72,    68,    78,    71,    79,    80,    81,    18,    77,    22,
      75,     0,     0,     0,    25,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60
};

static const yytype_int8 yycheck[] =
{
      10,    15,    11,    20,    21,    10,    19,    24,    10,    14,
       0,    13,    26,    11,    24,    24,    20,    21,    15,    16,
      23,    22,     3,    17,    10,    17,    11,    10,    17,     4,
      23,    18,    10,    24,    18,    23,    18,     5,     7,     6,
      11,    14,    12,    23,     8,    11,    25,    23,    25,     9,
      24,    10,    10,    25,    10,    10,    10,     8,    69,    12,
      25,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    19,    28,    29,    23,     0,    22,    32,    33,    34,
      20,    21,    30,    31,    11,     3,    36,    37,    34,    17,
      10,    24,    31,    17,    11,    37,    10,    17,    10,    14,
      35,    23,    18,    10,    18,    38,    18,     4,    39,    40,
      23,    24,     5,    47,     6,    41,    23,    25,     7,    42,
      11,    48,    49,    12,    25,     8,    43,    15,    26,    24,
      49,    10,    13,    23,     9,    46,    14,    11,    10,    44,
      45,    25,    24,    15,    16,    25,    24,    45,    10,    10,
      10,    10
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    27,    28,    29,    29,    30,    30,    31,    31,    32,
      32,    33,    33,    34,    35,    35,    36,    36,    38,    37,
      39,    40,    41,    42,    42,    43,    43,    44,    44,    45,
      46,    46,    47,    48,    48,    49,    49,    49
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     0,     4,     1,     2,     4,     5,     0,
       1,     1,     2,     5,     1,     1,     1,     2,     0,     6,
       2,     7,     3,     3,     3,     0,     4,     1,     2,     3,
       0,     3,     4,     1,     2,     3,     5,     5
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: config_opt var_list_opt test_list  */
#line 145 "testlang.y"
                                      { fprintf(out, "}\n"); }
#line 1279 "testlang.tab.c"
    break;

  case 7: /* config_item: BASE_URL ASSIGN STRING SEMICOLON  */
#line 160 "testlang.y"
                                     {
        fprintf(out, "    private static final String BASE_URL = %s;\n", (yyvsp[-1].str));
    }
#line 1287 "testlang.tab.c"
    break;

  case 8: /* config_item: HEADER STRING ASSIGN STRING SEMICOLON  */
#line 163 "testlang.y"
                                          {
        char hbuf[512];
        snprintf(hbuf, sizeof(hbuf), "        builder.header(%s, %s);\n", (yyvsp[-3].str), (yyvsp[-1].str));
        headers_append(hbuf);
    }
#line 1297 "testlang.tab.c"
    break;

  case 13: /* var_decl: LET IDENTIFIER ASSIGN value SEMICOLON  */
#line 182 "testlang.y"
                                          {
        fprintf(out, "    private static final String %s = %s;\n", (yyvsp[-3].str), (yyvsp[-1].str));
    }
#line 1305 "testlang.tab.c"
    break;

  case 14: /* value: STRING  */
#line 188 "testlang.y"
           { (yyval.str) = (yyvsp[0].str); }
#line 1311 "testlang.tab.c"
    break;

  case 15: /* value: NUMBER  */
#line 189 "testlang.y"
           {
        char buf[32];
        sprintf(buf, "\"%d\"", (yyvsp[0].num));
        (yyval.str) = strdup(buf);
    }
#line 1321 "testlang.tab.c"
    break;

  case 18: /* $@1: %empty  */
#line 203 "testlang.y"
                           {
        fprintf(out, "\n    @Test\n");
        fprintf(out, "    public void %s() throws Exception {\n", (yyvsp[-1].str));
        fprintf(out, "        String method = null;\n");
        fprintf(out, "        String url = null;\n");
        fprintf(out, "        String body = null;\n");
        fprintf(out, "        int expectedStatus = 200;\n");
        fprintf(out, "        HttpRequest.Builder builder = HttpRequest.newBuilder();\n");
        fprintf(out, "%s", default_headers);
        asserts_reset();
    }
#line 1337 "testlang.tab.c"
    break;

  case 19: /* test: TEST IDENTIFIER LBRACE $@1 test_body RBRACE  */
#line 213 "testlang.y"
                       {
        fprintf(out, "        if (url != null) builder.uri(URI.create(url));\n");
        fprintf(out, "        if (\"GET\".equalsIgnoreCase(method)) {\n");
        fprintf(out, "            builder = builder.GET();\n");
        fprintf(out, "        } else if (body != null) {\n");
        fprintf(out, "            builder = builder.method(method, HttpRequest.BodyPublishers.ofString(body));\n");
        fprintf(out, "        } else {\n");
        fprintf(out, "            builder = builder.method(method, HttpRequest.BodyPublishers.noBody());\n");
        fprintf(out, "        }\n");
        fprintf(out, "        HttpRequest request = builder.build();\n");
        fprintf(out, "        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());\n");
        fprintf(out, "        Assertions.assertEquals(expectedStatus, response.statusCode());\n");
        fprintf(out, "        String bodyStr = response.body();\n");
        fprintf(out, "%s", assert_buffer);
        fprintf(out, "    }\n");
    }
#line 1358 "testlang.tab.c"
    break;

  case 22: /* method_decl: METHOD COLON HTTP_METHOD  */
#line 241 "testlang.y"
                             {
        fprintf(out, "        method = \"%s\";\n", (yyvsp[0].str));
    }
#line 1366 "testlang.tab.c"
    break;

  case 23: /* url_decl: URL COLON STRING  */
#line 247 "testlang.y"
                     {
        char* expr = build_url_expr_from_string((yyvsp[0].str));
        fprintf(out, "        url = %s;\n", expr);
        free(expr);
    }
#line 1376 "testlang.tab.c"
    break;

  case 24: /* url_decl: URL COLON VARREF  */
#line 252 "testlang.y"
                     {
        fprintf(out, "        url = %s;\n", (yyvsp[0].str));
    }
#line 1384 "testlang.tab.c"
    break;

  case 29: /* header: STRING COLON STRING  */
#line 268 "testlang.y"
                        {
        fprintf(out, "        builder.header(%s, %s);\n", (yyvsp[-2].str), (yyvsp[0].str));
    }
#line 1392 "testlang.tab.c"
    break;

  case 31: /* body_opt: BODY COLON STRING  */
#line 276 "testlang.y"
                      {
        fprintf(out, "        body = %s;\n", (yyvsp[0].str));
    }
#line 1400 "testlang.tab.c"
    break;

  case 35: /* assert: IDENTIFIER EQ NUMBER  */
#line 292 "testlang.y"
                         {
        if (strcmp((yyvsp[-2].str), "status") == 0) {
            fprintf(out, "        expectedStatus = %d;\n", (yyvsp[0].num));
        } else {
            /* Unknown identifier in status position; emit comment */
            fprintf(out, "        // unsupported assert: %s == %d\n", (yyvsp[-2].str), (yyvsp[0].num));
        }
    }
#line 1413 "testlang.tab.c"
    break;

  case 36: /* assert: IDENTIFIER DOT IDENTIFIER EQ STRING  */
#line 300 "testlang.y"
                                        {
        if (strcmp((yyvsp[-4].str), "body") == 0) {
            char buf[512];
            char* esc = escape_java_literal((yyvsp[0].str));
            /* For human-friendly message, show value without outer quotes */
            char* msg_raw = strip_outer_quotes((yyvsp[0].str));
            char* msg_esc = escape_java_literal(msg_raw);
            snprintf(buf, sizeof(buf),
                     "        Assertions.assertTrue(bodyStr.contains(\"\\\"%s\\\":%s\"), \"body.%s == '%s'\");\n",
                     (yyvsp[-2].str), esc, (yyvsp[-2].str), msg_esc);
            free(esc);
            free(msg_esc);
            free(msg_raw);
            asserts_append(buf);
        } else {
            fprintf(out, "        // unsupported assert scope: %s.%s == %s\n", (yyvsp[-4].str), (yyvsp[-2].str), (yyvsp[0].str));
        }
    }
#line 1436 "testlang.tab.c"
    break;

  case 37: /* assert: IDENTIFIER DOT IDENTIFIER CONTAINS STRING  */
#line 318 "testlang.y"
                                              {
        if (strcmp((yyvsp[-4].str), "body") == 0) {
            char buf[512];
            snprintf(buf, sizeof(buf),
                     "        Assertions.assertTrue(bodyStr.contains(\"\\\"%s\\\"\"), \"body has field %s\");\n",
                     (yyvsp[-2].str), (yyvsp[-2].str));
            asserts_append(buf);
            char* esc2 = escape_java_literal((yyvsp[0].str));
            /* Prepare cleaner value for message by stripping outer quotes */
            char* msg2_raw = strip_outer_quotes((yyvsp[0].str));
            char* msg2_esc = escape_java_literal(msg2_raw);
            snprintf(buf, sizeof(buf),
                     "        Assertions.assertTrue(bodyStr.contains(%s), \"body.%s contains '%s'\");\n",
                     (yyvsp[0].str), (yyvsp[-2].str), msg2_esc);
            asserts_append(buf);
            free(esc2);
            free(msg2_esc);
            free(msg2_raw);
        } else {
            fprintf(out, "        // unsupported assert scope: %s.%s contains %s\n", (yyvsp[-4].str), (yyvsp[-2].str), (yyvsp[0].str));
        }
    }
#line 1463 "testlang.tab.c"
    break;


#line 1467 "testlang.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 342 "testlang.y"


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename.test>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("File not found");
        return 1;
    }

    out = fopen("Samples/src/GeneratedTests.java", "w");
    if (!out) {
        perror("Cannot open output file");
        return 1;
    }

    fprintf(out, "import org.junit.jupiter.api.*;\n");
    fprintf(out, "import java.net.http.*;\n");
    fprintf(out, "import java.net.URI;\n");
    fprintf(out, "\npublic class GeneratedTests {\n");
    fprintf(out, "    private static final HttpClient client = HttpClient.newHttpClient();\n");

    yyin = file;
    printf("Parsing %s...\n", argv[1]);
    yyparse();
    printf("Parsing complete. Generated: Samples/src/GeneratedTests.java\n");

    fclose(file);
    fclose(out);
    return 0;
}


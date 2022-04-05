#ifndef X_STRING_H
#define X_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 
=====================================================
>>>>>>>>>>>>>>>>>>>>>> XSTRING >>>>>>>>>>>>>>>>>>>>>>
-----------------------------------------------------

Small lightweight C string library for handling UTF-8
and useful operations.

================================= @Eugenio Arteaga A.

*/

#include <stddef.h>
#include <stdbool.h>

#define NULLSTR(str) (!(str) || !*(str))
#define MIN(a, b) ((a) * ((a) <= (b)) + (b) * ((b) < (a)))
#define MAX(a, b) ((a) * ((a) >= (b)) + (b) * ((b) > (a)))
#define getcount(arr) (sizeof(arr) / sizeof(arr[0]))

#define funcdef(x) do { x } while(0)
#define eatchar(str, c, i)          \
funcdef                             \
(                                   \
    size_t bytes;                   \
    while (str[i] == c) {           \
        bytes = x_utf_bytes(str[i]);\
        i += bytes;                 \
    }                               \
    i -= bytes;                     \
)


/* main string functions and operations */

bool x_strcmp(const char* str1, const char* str2);
char* x_strend(const char* str);
char* x_strdup(const char* str);
char* x_strdup_num(size_t num);
char* x_strdup_range(const char* start, const char* end);
char* x_strdup_size(const char* str, const size_t size);
char* x_strcat(const char* str1, const char* str2);
char* x_strcat_spaced(const char* str1, const char* str2);
size_t x_strnum(const char* str);
size_t x_strnum_first(const char* str);
size_t x_strstr(const char* big, const char* little);
size_t x_strchr(const char* str, const char* ch);
size_t x_strcnt(const char* search, const char* find);
size_t x_strmatch(const char* str, char** keywords, const size_t count);
size_t x_strmatch_sub(const char* str, char** keywords, const size_t count);

/* string extraction, splitting, stripping */

char* x_strget_num(const char* str);
char* x_strget_reverse(const char* str);
char* x_strget_strip(const char* str);
char* x_strget_sweep(const char* str);
char* x_strget_compose(char** strs, const size_t count);

char** x_strget_split(const char* str, size_t* count, const char* c);
char** x_strget_words(const char* str, size_t* count);
char** x_strget_symbols(const char* str, size_t* count);
char** x_strget_tokens(const char* str, size_t* count);
char** x_strget_variations(const char* str, size_t* count);
char** x_strget_decompose(const char* str, size_t* count);
char** x_strget_divide(const char* str, size_t* count, char** symbols, const size_t symbol_count);
char** x_strget_merge(char** strs1, char** strs2, const size_t count1, const size_t count2, size_t* out_count);
char** x_strget_group(const char* str, size_t* count, char** words, char** symbols, const size_t word_count, const size_t symbol_count);
void x_strget_free(char** strs, const size_t count);

/* string operatons */

void x_strop_strip(char* str);
void x_strop_upper(char* str);
void x_strop_lower(char* str);
void x_strop_remove(char* str, const char* remove);
void x_strop_remove_at(char* str, const char* remove);
void x_strop_remove_num(char* str);
void x_strop_replace_at(char* str, const char* dest);
void x_strop_replace(char* str, const char* src, const char* dest);
void x_strop_normalize(char* str);
void x_strop_capitalize(char* str);
void x_strop_capitalize_phrase(char* str);

/* string boolean functions */

bool x_strbool_utf(const char* str);
bool x_strbool_num(const char* str);
bool x_strbool_num_has(const char* str);
bool x_strbool_space_has(const char* str);
bool x_strbool_letter(const char* str);
bool x_strbool_letter_has(const char* str);
bool x_strbool_symbol(const char* str);
bool x_strbool_symbol_has(const char* str);
bool x_strbool_alphanum(const char* str);
bool x_strbool_alphanum_has(const char* str);
bool x_strbool_alphaspace(const char* str);
bool x_strbool_upper(const char* str);
bool x_strbool_lower(const char* str);
bool x_strbool_upper_alphanum(const char* str);
bool x_strbool_lower_alphanum(const char* str);
bool x_strbool_capital_token(const char* token);
bool x_strbool_capital_phrase(const char* phrase);
bool x_strbool_normal(const char* str);
bool x_strbool_match(const char* str1, const char* str2);
bool x_strbool_match_pattern(const char* str, const char* ref);

/* char operations */

bool x_chrbool_digit(const char c);
bool x_chrbool_upper_ascii(const char c);
bool x_chrbool_lower_ascii(const char c);
bool x_chrbool_upper_utf(const char* ch);
bool x_chrbool_lower_utf(const char* ch);
bool x_chrbool_upper(const char* ch);
bool x_chrbool_lower(const char* ch);
bool x_chrbool_letter(const char* ch);
bool x_chrbool_alphaspace(const char* ch);
bool x_chrbool_alphanum(const char* ch);
bool x_chrbool_symbol(const char* ch);

/* UTF-8 handling */

bool x_utf_check(const char c);
size_t x_utf_bytes(const char c);
char* x_utf_next(const char* str);

/* string tables (spanish) */

char* x_strtable_upper_utf();
char* x_strtable_lower_utf();
char* x_strtable_normal_utf();
char* x_strtable_normal_ascii();

#ifdef __cplusplus
}
#endif
#endif

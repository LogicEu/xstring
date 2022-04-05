#include <xstring.h>
#include <stdlib.h>
#include <string.h>

static char* x_strdup_if(const char* restrict src)
{
    if (!src) return NULL;

    const size_t size = strlen(src);
    if (!size) return NULL;

    char* buff = calloc(size + 1, sizeof(char));
    memcpy(buff, src, size);
    return buff;
}

static char* x_strtok_symbol(const char* restrict str, const size_t pos, const size_t len)
{
    if (NULLSTR(str) || pos > len) return NULL;

    char* start = (char*)(size_t)str + pos;
    char* end = start;
    
    if (pos > 0) {
        size_t spos = pos;
        while (spos > 0 && *(start - 1) == ' ') { 
            --start, --spos;
        } 
    }

    if (pos < len) {
        size_t bytes = 0;
        while (*end && !x_chrbool_alphanum(end)) {
            bytes = x_utf_bytes(*end);
            end += bytes;
        }
        end -= bytes;
    }

    const size_t size = end - start + x_utf_bytes(*end);
    char* ret = calloc(size + 1, sizeof(char));
    memcpy(ret, start, size);
    return ret;
}

static char* x_strtok_phrase(const char* restrict str, const size_t pos, const size_t len)
{
    if (NULLSTR(str) || pos > len) return NULL;

    char* start = (char*)(size_t)str + pos;
    char* end = start;

    if (pos < len) {
        size_t bytes = 0;
        while (*end && x_chrbool_alphaspace(end)) {
            bytes = x_utf_bytes(*end);
            end += bytes;
        }
        end -= bytes;
    }

    const size_t size = end - start + x_utf_bytes(*end);
    char* ret = calloc(size + 1, sizeof(char));
    memcpy(ret, start, size);
    if (ret[size - 1] == ' ') ret[size - 1] = 0;
    
    return ret;
}

static char* x_strtok_word(const char* restrict str, const size_t pos, const size_t len)
{
    if (NULLSTR(str) || pos > len) return NULL;

    char* start = (char*)(size_t)str + pos;
    char* end = start;

    if (pos < len) {
        size_t bytes = 0;
        while (*end && x_chrbool_alphanum(end)) {
            bytes = x_utf_bytes(*end);
            end += bytes;
        }
        end -= bytes;
    }

    const size_t size = end - start + x_utf_bytes(*end);
    char* ret = calloc(size + 1, sizeof(char));
    memcpy(ret, start, size);
    return ret;
}

char* x_strget_reverse(const char* restrict str)
{
    const size_t len = strlen(str) - 1;
    char* reverse = x_strdup(str);
    size_t bytes;
    for (size_t i = 0; str[i]; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        memcpy(reverse + len - i, str + i, bytes);
    }
    return reverse;
}

char* x_strget_num(const char* restrict str)
{
    const size_t len = strlen(str);
    char buff[len];
    memset(buff, 0, len);

    size_t bytes, index = 0;
    for (size_t i = 0; str[i]; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (x_chrbool_digit(str[i])) {
            memcpy(buff + index, str + i, bytes);
            ++index;
        }
    }

    return x_strdup(buff);
}

char* x_strget_strip(const char* restrict str)
{
    char* strip = x_strdup(str);
    if (NULLSTR(strip)) return NULL;
    
    x_strop_strip(strip);
    return strip;
}

char* x_strget_sweep(const char* restrict str)
{
    char* sweep = x_strdup(str);
    if (!sweep) return NULL;
    
    size_t bytes;
    for (size_t i = 0; sweep[i] != 0; i += bytes) {
        if (!x_chrbool_alphanum(sweep + i)) {
            x_strop_replace_at(sweep + i, " ");
        }
        bytes = x_utf_bytes(sweep[i]);
    }
    return sweep;
}

char* x_strget_compose(char** restrict strs, const size_t count)
{
    size_t total_size = 0;
    for (size_t i = 0; i < count; ++i) {
        total_size += strlen(strs[i]);
    }

    const size_t tsize = total_size;
    if (!tsize) return NULL;
    char* str = calloc(tsize + count + 1, sizeof(char));

    bool prev = NULL, this = NULL;
    for (size_t i = 0; i < count; ++i) {
        this = x_strbool_alphaspace(strs[i]);
        if (this && prev) strcat(str, " ");
        strcat(str, strs[i]);
        prev = this;
    }
    return str;
}

/* pass string already trimmed, and NULL terminated separator */
char** x_strget_split(const char* restrict str, size_t* size, const char* restrict ch)
{
    if (!size) return NULL;
    if (NULLSTR(str) || NULLSTR(ch)) {
        *size = 0;
        return NULL;
    }

    const size_t count = x_strcnt(str, ch) + x_strbool_alphanum_has(str);
    *size = count;

    if (!count) return NULL;

    char** tokens = malloc(count * sizeof(char*));
    
    const size_t chlen = strlen(ch);
    const size_t len = strlen(str);

    char buffer[len + 1];
    memset(buffer, 0, len + 1);

    size_t token_index = 0, token_char = 0, bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (!memcmp(str + i, ch, chlen)) {
            
            buffer[token_char] = 0;
            token_char = 0;
         
            tokens[token_index++] = x_strdup_if(buffer);
            memset(buffer, 0, len + 1);
            bytes = chlen;

        } else {

            memcpy(buffer + token_char, str + i, bytes);
            token_char += bytes;

        }
    }

    tokens[token_index++] = x_strdup_if(buffer);
    return tokens;
}

char** x_strget_words(const char* restrict str, size_t* size)
{
    if (!size) return NULL;
    if (NULLSTR(str)) {
        *size = 0;
        return NULL;
    }

    const size_t len = strlen(str);
    size_t indexes[len], count = 0, bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (x_chrbool_alphanum(str + i)) {
            indexes[count++] = i;
            while (str[i] != 0 && x_chrbool_alphanum(str + i)) { 
                i += x_utf_bytes(str[i]);
                bytes = 0;
            }
        }
    }

    *size = count;
    if (!count) return NULL;
    
    const size_t cnt = count;
    char** strs = malloc(cnt * sizeof(char*));
    for (size_t i = 0; i < cnt; ++i) {
        strs[i] = x_strtok_word(str, indexes[i], len);
    }
    return strs;
}

char** x_strget_deconstruct(const char* restrict str, size_t* size)
{
    if (!size) return NULL;
    if (NULLSTR(str)) {
        *size = 0;
        return NULL;
    }

    const size_t len = strlen(str);
    size_t indexes[len], count = 0, bytes;

    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (!x_chrbool_alphaspace(str + i)) {
            indexes[count++] = i;
            while (str[i] != 0 && !x_chrbool_alphaspace(str + i)) { 
                i += x_utf_bytes(str[i]);
                bytes = 0;
            }
        }
    }

    *size = count;
    if (!count) return NULL;

    const size_t cnt = count;
    char** strs = malloc(cnt * sizeof(char*));
    for (size_t i = 0; i < cnt; ++i) {
        strs[i] = x_strtok_symbol(str, indexes[i], len);
    }
    return strs;
}

char** x_strget_symbols(const char* restrict str, size_t* size)
{
    if (!size) return NULL;
    if (NULLSTR(str)) {
        *size = 0;
        return NULL;
    }

    const size_t len = strlen(str);
    size_t indexes[len], count = 0, bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (!x_chrbool_alphaspace(str + i)) {
            indexes[count++] = i;
            while (str[i] != 0 && !x_chrbool_alphaspace(str + i)) { 
                i += x_utf_bytes(str[i]);
                bytes = 0;
            }
        }
    }

    *size = count;
    if (!count) return NULL;

    const size_t cnt = count;
    char** strs = malloc(cnt * sizeof(char*));
    for (size_t i = 0; i < cnt; ++i) {
        strs[i] = x_strtok_symbol(str, indexes[i], len);
    }
    return strs;
}

char** x_strget_tokens(const char* restrict str, size_t* size)
{
    if (!size) return NULL;
    if (NULLSTR(str)) {
        *size = 0;
        return NULL;
    }

    const size_t len = strlen(str);
    size_t indexes[len], count = 0, bytes;
    
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (str[i] == ' ') continue;

        indexes[count++] = i;
        
        if (!x_chrbool_alphanum(str + i)) {
            while (str[i] != 0 && str[i] != ' ' && !x_chrbool_alphanum(str + i)) { 
                i += x_utf_bytes(str[i]);
                bytes = 0;
            }
        } else {
            while (str[i] != 0 && str[i] != ' ' && x_chrbool_alphanum(str + i)) { 
                i += x_utf_bytes(str[i]);
                bytes = 0;
            }
        }
    }

    *size = count;
    if (!count) return NULL;
    
    const size_t total_count = count;
    char** strs = malloc(total_count * sizeof(char*));
    
    for (size_t i = 0; i < total_count; ++i) {
        if (x_chrbool_alphanum(str + indexes[i])) {
            strs[i] = x_strtok_word(str, indexes[i], len);
        } else {
            strs[i] = x_strtok_symbol(str, indexes[i], len);
        }
    }
    return strs;
}

char** x_strget_variations(const char* restrict str, size_t* count)
{
    if (NULLSTR(str)) return NULL;

    const size_t variation_count = 3 * (!x_strbool_normal(str) + 1);

    char** keywords = malloc(variation_count * sizeof(char*));
    *count = variation_count;
    
    for (size_t i = 0; i < 3; ++i) {
        keywords[i] = x_strdup(str);
    }

    x_strop_capitalize_phrase(keywords[0]);
    x_strop_lower(keywords[1]);
    x_strop_upper(keywords[2]);

    for (size_t i = 3; i < variation_count; i++) {
        keywords[i] = x_strdup(keywords[i - 3]);
        x_strop_normalize(keywords[i]);
    }

    return keywords;
}

char** x_strget_decompose(const char* restrict str, size_t* size)
{
    if (!size) return NULL;
    if (NULLSTR(str)) {
        *size = 0;
        return NULL;
    }

    const size_t len = strlen(str);
    size_t indexes[len], count = 0, bytes;
    
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (str[i] == ' ') continue;

        indexes[count++] = i;
        if (x_chrbool_alphaspace(str + i)) {
            while (str[i] != 0 && x_chrbool_alphaspace(str + i)) { 
                i += x_utf_bytes(str[i]);
                bytes = 0;
            }
        } else {
            while (str[i] != 0 && !x_chrbool_alphanum(str + i)) { 
                i += x_utf_bytes(str[i]);
                bytes = 0;
            }
        }
    }

    *size = count;
    if (!count) return NULL;
    
    const size_t total_count = count;
    char** strs = malloc(total_count * sizeof(char*));

    for (size_t i = 0; i < total_count; ++i) {
        if (x_chrbool_alphanum(str + indexes[i])) {
            strs[i] = x_strtok_phrase(str, indexes[i], len);
        } else {
            strs[i] = x_strtok_symbol(str, indexes[i], len);
        }
    }

    return strs;
}

/* pass matched string (returns NULL if does not match) */
char** x_strget_divide(const char* restrict str, size_t* count, char** restrict symbols, const size_t symbol_count)
{
    if (!count) return NULL;
    *count = 0;

    if (NULLSTR(str)) return NULL;

    char* divs[symbol_count + 1];
    char* s = (char*)(size_t)str, *mark;
    size_t index = 0;

    for (size_t i = 0; i < symbol_count; ++i) {

        mark = strstr(s, symbols[i]);
        if (!mark) return NULL;

        char* div = x_strdup_range(s, mark);
        if (div) divs[index++] = div;
        
        s = mark + strlen(symbols[i]);
    }
    
    if (*s) divs[index++] = x_strdup(s);

    const size_t divcount = index;
    *count = divcount;
    if (!divcount) return NULL;

    char** divitions = malloc(divcount * sizeof(char*));
    for (size_t i = 0; i < divcount; ++i) {
        divitions[i] = divs[i];
    }

    return divitions; 
}

char** x_strget_merge(char** strs1, char** strs2, const size_t count1, const size_t count2, size_t* out_count)
{
    if (!strs1 || !strs2) return NULL;
    
    const size_t count = count1 + count2;
    *out_count = count;
    if (!count) return NULL;

    char** strs = malloc(count * sizeof(char*));

    for (size_t i = 0; i < count1; ++i) strs[i] = strs1[i];
    for (size_t i = 0; i < count2; ++i) strs[i + count1] = strs2[i];

    free(strs1);
    free(strs2);

    return strs;
}

/* pass matched collections */
char** x_strget_group(const char* restrict str, size_t* out_size, char** restrict words, char** restrict symbols, const size_t word_count, const size_t symbol_count)
{
    if (!out_size) return NULL;
    if (NULLSTR(str)) {
        *out_size = 0;
        return NULL;
    }

    const size_t count = word_count + symbol_count;
    *out_size = count;
    if (!count) return NULL;

    char** unite = malloc(count * sizeof(char*));

    char* mark = (char*)(size_t)str;
    char* word = mark, *w;
    char* sym = mark, *s;

    size_t wordcount = 0, worddif;
    size_t symcount = 0, symdif;

    for (size_t i = 0; i < count; ++i) {

        s = w = NULL;
        
        if (wordcount < word_count) {
            w = words[wordcount];
            word = strstr(mark, w);
            worddif = word - mark;
        }
        
        if (symcount < symbol_count) {
            s = symbols[symcount];
            sym = strstr(mark, s);
            symdif = sym - mark;
        }
        
        if (w && (!s || (worddif < symdif))) {
            unite[i] = x_strdup(words[wordcount]);
            mark = ++word;
            ++wordcount;
        } 

        if (s && (!w || (symdif < worddif))) {
            unite[i] = x_strdup(symbols[symcount]);
            mark = ++sym;
            ++symcount;
        }
    }

    return unite;
}

void x_strget_free(char** strs, const size_t count)
{
    if (!strs) return;
    for (size_t i = 0; i < count; ++i) {
        if (strs[i]) free(strs[i]);
    }
    free(strs);
}
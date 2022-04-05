#include <string.h>

char* x_strtable_upper_utf()
{
    static const char* table = "ÑÁÉÍÓÚ";
    return (char*)(size_t)table;
}

char* x_strtable_lower_utf()
{
    static const char* table = "ñáéíóú";
    return (char*)(size_t)table;
}

char* x_strtable_normal_utf()
{
    static const char table[] = "áéíóúÁÉÍÓÚ";
    return (char*)(size_t)table;
}

char* x_strtable_normal_ascii()
{
    static const char table[] = "aeiouAEIOU";
    return (char*)(size_t)table;
}
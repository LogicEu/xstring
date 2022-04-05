#include <xstring.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

typedef struct {
	char mask;    	/* char data will be bitwise AND with this */
	char lead;    	/* start bytes of current char in utf-8 encoded character */
	uint32_t beg; 	/* beginning of codepoint range */
	uint32_t end; 	/* end of codepoint range */
	int bits_stored;/* the number of bits from the codepoint that fits in char */
} utf_t;
 
static utf_t* utf[] = {
	/*             mask        lead        beg      end       bits */
	[0] = &(utf_t){0b00111111, 0b10000000, 0,       0,        6    },
	[1] = &(utf_t){0b01111111, 0b00000000, 0000,    0177,     7    },
	[2] = &(utf_t){0b00011111, 0b11000000, 0200,    03777,    5    },
	[3] = &(utf_t){0b00001111, 0b11100000, 04000,   0177777,  4    },
	[4] = &(utf_t){0b00000111, 0b11110000, 0200000, 04177777, 3    },
	      &(utf_t){0},
};

bool x_utf_check(const char ch)
{
    return ((unsigned char)ch >= 0xC0);
}

size_t x_utf_bytes(const char ch)
{
	size_t len = 0;
    for (utf_t **u = utf; *u; ++u, ++len) {
		if ((ch & ~(*u)->mask) == (*u)->lead) {
			break;
		}
	}
	return MAX(len, 1);
}

char* x_utf_next(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i]; i += bytes) {
        if ((bytes = x_utf_bytes(str[i])) > 1) {
            return (char*)(size_t)(str + i);
        }
    }
    return NULL;
}
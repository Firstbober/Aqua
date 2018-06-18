#include <kernel/include/string.h>
#include <stdarg.h>

#define INT_MAX 2147483647

void atoi(char *str, int* a)
{
    int k = 0;
    while(*str)
    {
        k = (k<<3)+(k<<1)+(*str)-'0';
        str++;
    }
    *a = k;
}
/*
void d2s(double d, char* string, size_t size, char dot) {
    if(size < 0) return;
    char isMinus = 0; if(d < 0) isMinus = 1;
    char isFloat = 0; if(d-(int64_t)d>0) isFloat = 1;
    size_t tab_n = 0; string[tab_n] = '\0';
    if(isMinus) {string[tab_n] = '-'; tab_n++; d=-d;}
    int64_t dot_pos = d;
    do {
        d/=10;
    }
    while(d>0);
    int64_t temp = 0;
    char dotSet = 0;
    while(d != (int64_t)d) {
        if(tab_n >= size) return;
        else if(tab_n < size-1) {string[tab_n] = '\0'; return;}
        if((int64_t)d == dot_pos && !dotSet) {
            string[tab_n] = dot;
            ++tab_n;
            dotSet = 1;
        }
        d*=10;
        string[tab_n] = '0' + ( (int64_t)d - temp*10 );
        ++tab_n;
    }
}
*/

#define DRAW_STRING
char* getStringn(char* ptr, unsigned int ptr_size) {
    if(ptr_size > 0) {
        for(int i = 0; i<ptr_size-1; i++) {
            ptr[i] = _getch(def_keyboard,_SC_US, 1);
            #ifdef DRAW_STRING
                putchar(ptr[i]);
            #endif
        }
        #ifdef DRAW_STRING
            putchar('\n');
        #endif
        ptr[ptr_size-1] = '\0';
    }
    return ptr;
}
char* getStringc(char* ptr, unsigned int ptr_size, char CHAR) {
    if(ptr_size > 0) {
        for(int i = 0; i<ptr_size-1; i++) {
            char ch = _getch(def_keyboard, _SC_US, 1);
            if(ch != CHAR) {
                ptr[i] = ch;
                #ifdef DRAW_STRING
                    putchar(ptr[i]);
                #endif
            }
            else {
                ptr_size = i+1;
                break;
            }
        }
        #ifdef DRAW_STRING
            putchar('\n');
        #endif
        ptr[ptr_size-1] = '\0';
    }
    return ptr;
}
int strcmp(const char* a, const char* b) {
    for(int i = 0;;i++) {
        if(a[i]=='\0' && b[i]!='\0') return -1;
        if(a[i]!='\0' && b[i]=='\0') return 1;
        if(a[i]!=b[i]) return 1;
        if(a[i]=='\0' && b[i]=='\0') break;
    }
    return 0;

}
bool reverse(void* _ptr, size_t size) {
    if(size==0) return 0;
    for(size_t i = 0; i < size/2; i++) {

        char* c_1 = (char*)(_ptr + i);
        char* c_2 = (char*)(_ptr + size - i - 1);

        char D = *c_1;
        char C = *c_2;

        *c_1 = C;
        *c_2 = D;
    }
    return 1;
}

bool itos(int32_t integer, char* _ptr) {
    if(integer < 0) {
        integer = -integer;
        *_ptr = '-';
        _ptr++;
    }
    int iter = -1;
    do {
        iter++;
        _ptr[iter] = (integer % 10) + 48;
        integer/=10;
    } while(integer > 0);
    reverse(_ptr,iter+1);
    _ptr[iter+1] = '\0';
    return 1;

}

void htoa(uint32_t h, char* t) {
    t[0] = '0';
    t[1] = 'x';
    t+=2;

    uint32_t i = 0;

    do {
        char d = h % 0x10;
        if(d < 10) d += 0x30;
        else d += 0x41-0x0A;
        t[i] = d;
        i++;
    } while(h /= 0x10);

    t[i] = '\0';

    reverse(t,i);
}

void itoa(int32_t h, char* t) {
  if(h < 0) {
    *t = '-';
    h = -h;
    t++;
  }

  uint32_t i = 0;
  do {
    t[i] = h % 0xA + 0x30;
    i++;
  } while(h /= 0xA);

  t[i] = '\0';
  
  reverse(t,i);
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (*str++) len++;
    return len;
}

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (bytes[i] == "\0") {
			return false;
        } else {
            putchar(bytes[i]);
        }
	return true;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
 
	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
 
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;
 
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
        } else if (*format == 'i') {
			format++;
			int numstr = va_arg(parameters, int);
            char str[sizeof(int)];
            itoa(numstr, str);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
 
	va_end(parameters);
	return written;
}

char *strcpy(char *src, char *dest)
{
   char *save = dest;
   while(*dest++ = *src++);
   return save;
}

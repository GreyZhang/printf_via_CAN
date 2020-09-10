#include "string.h"

/*
* putc will append a char to the queue instaed of tansmitting.
*/
void _putchar(char character)
{
    debug_lld_can_queue_append(character);
}


#include <common.h>
/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
void * memcpy(void * dest,const void *src,size_t count)
{
	char *tmp = (char *) dest, *s = (char *) src;
	while (count--)
		*tmp++ = *s++;
	return dest;
}
/**
 * @brief __assert_fail 
 *
 * @param __assertion
 * @param __file
 * @param __line
 * @param __function
 */
void __assert_fail (__const char *__assertion, __const char *__file,
			   unsigned int __line, __const char *__function)
{
	printf("assert fail :%s:%s:%d:%s\n",\
				__assertion,\
				__file,\
				__line,\
				__function\
		  );
}

void * memset(void * s,int c,size_t count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}

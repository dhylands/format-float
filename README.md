This repository provides a function for formatting a 32-bit floating point
number as a string.

It does most of the work required for printf suport. In keeping with the
flavor of embeded systems, it allows you to provide the buffer and the size
of the buffer for formatting.

It deals with most of what printf needs to do, with the exception of width
and zero padding. This support can be added outside of the ```format_float```
routine.

Prototype:
```
int format_float(float f, char *buf, size_t buf_size, char fmt, int prec, char sign);
```

```fmt``` should be one of 'e', 'E', 'f', 'F', 'g', or 'G'.

```prec``` is the precision (as specified in printf)

```sign``` should be '\0', '+', or ' '  ('\0' is the normal one - only print
           a sign if ```f``` is negative. Anything else is printed as the
           sign character for positive nubers.

To test:
```
make
```

will compile and build a program called float which runs a number of tests
to compare ```format-float``` with that of the builtin printf.

It also performs a number of tests to verify that no buffer overflows occur.

For the 'f' format, if the number won't fit in the buffer, then it will be
converted to 'e' format.

For any of the formats, precision will be reduced if the result will still
fit in the buffer.


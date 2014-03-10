all: float
	./float

float: float.o format-float.o
	gcc -o $@ -g $^

%.o: %.c
	gcc -c -Wall -Werror -ansi -std=gnu99  -g -O0 -I . -I ../stm -I ../py $<

clean:
	rm -f float *.o

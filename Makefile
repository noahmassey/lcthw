CFLAGS=-Wall -g

ex2_s="ex2/ex2.c"
ex2_o="ex2/ex2"

ex3_s="ex3/ex3.c"
ex3_o="ex3/ex3"

ex4_s="ex4/ex4.c"
ex4_o="ex4/ex4"

all: example2 example3 example4

example2:
	cc $(ex2_s) -o $(ex2_o) $(CFLAGS)

example3:
	cc $(ex3_s) -o $(ex3_o) $(CFLAGS)

example4:
	cc $(ex4_s) -o $(ex4_o) $(CFLAGS)

clean:
	rm -f $(ex2_o)
	rm -f $(ex3_o)
	rm -f $(ex4_o)

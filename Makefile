CFLAGS=-Wall -g

ex2_s="ex2/ex2.c"
ex2_o="ex2/ex2"

ex3_s="ex3/ex3.c"
ex3_o="ex3/ex3"

ex4_s="ex4/ex4.c"
ex4_o="ex4/ex4"

ex5_s="ex5/ex5.c"
ex5_o="ex5/ex5"

ex6_s="ex6/ex6.c"
ex6_o="ex6/ex6"

all: example2 example3 example4 example5 example6

example2:
	cc $(ex2_s) -o $(ex2_o) $(CFLAGS)

example3:
	cc $(ex3_s) -o $(ex3_o) $(CFLAGS)

example4:
	cc $(ex4_s) -o $(ex4_o) $(CFLAGS)

example5:
	cc $(ex5_s) -o $(ex5_o) $(CFLAGS)

example6:
	cc $(ex6_s) -o $(ex6_o) $(CFLAGS)

clean:
	rm -f $(ex2_o)
	rm -f $(ex3_o)
	rm -f $(ex4_o)
	rm -f $(ex5_o)
	rm -f $(ex6_o)

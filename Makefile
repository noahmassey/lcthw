CFLAGS=-Wall -g

ex2_s="ex2/ex2.c"
ex2_o="bin/ex2"

ex3_s="ex3/ex3.c"
ex3_o="bin/ex3"

ex4_s="ex4/ex4.c"
ex4_o="bin/ex4"

ex5_s="ex5/ex5.c"
ex5_o="bin/ex5"

ex6_s="ex6/ex6.c"
ex6_o="bin/ex6"

ex7_s="ex7/ex7.c"
ex7_o="bin/ex7"

ex8_s="ex8/ex8.c"
ex8_o="bin/ex8"

ex9_s="ex9/ex9.c"
ex9_o="bin/ex9"

ex10_s="ex10/ex10.c"
ex10_o="bin/ex10"

ex11_s="ex11/ex11.c"
ex11_o="bin/ex11"

ex12_s="ex12/ex12.c"
ex12_o="bin/ex12"

ex13_s="ex13/ex13.c"
ex13_o="bin/ex13"

ex13a_s="ex13/ex13a.c"
ex13a_o="bin/ex13a"

all: example2 example3 example4 example5 example6 example7 example8 \
	example9 example10 example11 example12 example13

pre:
	if [ ! -d "bin" ];then mkdir bin; fi;

example2: pre
	cc $(ex2_s) -o $(ex2_o) $(CFLAGS)

example3: pre
	cc $(ex3_s) -o $(ex3_o) $(CFLAGS)

example4: pre
	cc $(ex4_s) -o $(ex4_o) $(CFLAGS)

example5: pre
	cc $(ex5_s) -o $(ex5_o) $(CFLAGS)

example6: pre
	cc $(ex6_s) -o $(ex6_o) $(CFLAGS)

example7: pre
	cc $(ex7_s) -o $(ex7_o) $(CFLAGS)

example8: pre
	cc $(ex8_s) -o $(ex8_o) $(CFLAGS)

example9: pre
	cc $(ex9_s) -o $(ex9_o) $(CFLAGS)

example10: pre
	cc $(ex10_s) -o $(ex10_o) $(CFLAGS)

example11: pre
	cc $(ex11_s) -o $(ex11_o) $(CFLAGS)

example12: pre
	cc $(ex12_s) -o $(ex12_o) $(CFLAGS)

example13: pre
	cc $(ex13_s) -o $(ex13_o) $(CFLAGS)
	cc $(ex13a_s) -o $(ex13a_o) $(CFLAGS)

clean:
	rm -f $(ex2_o)
	rm -f $(ex3_o)
	rm -f $(ex4_o)
	rm -f $(ex5_o)
	rm -f $(ex6_o)
	rm -f $(ex7_o)
	rm -f $(ex8_o)
	rm -f $(ex9_o)
	rm -f $(ex10_o)
	rm -f $(ex11_o)
	rm -f $(ex12_o)
	rm -f $(ex13_o)
	rm -f $(ex13a_o)

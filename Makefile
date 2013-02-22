CFLAGS=-Wall -g

ex2_s=ex2/ex2.c
ex2_o=bin/ex2

BINARIES += $(ex2_o)

ex3_s=ex3/ex3.c
ex3_o=bin/ex3

BINARIES += $(ex3_o)

ex4_s=ex4/ex4.c
ex4_o=bin/ex4

BINARIES += $(ex4_o)

ex5_s=ex5/ex5.c
ex5_o=bin/ex5

BINARIES += $(ex5_o)

ex6_s=ex6/ex6.c
ex6_o=bin/ex6

BINARIES += $(ex6_o)

ex7_s=ex7/ex7.c
ex7_o=bin/ex7

BINARIES += $(ex7_o)

ex8_s=ex8/ex8.c
ex8_o=bin/ex8

BINARIES += $(ex8_o)

ex9_s=ex9/ex9.c
ex9_o=bin/ex9

BINARIES += $(ex9_o)

ex10_s=ex10/ex10.c
ex10_o=bin/ex10

BINARIES += $(ex10_o)

ex11_s=ex11/ex11.c
ex11_o=bin/ex11

BINARIES += $(ex11_o)

ex12_s=ex12/ex12.c
ex12_o=bin/ex12

BINARIES += $(ex12_o)

ex13_s=ex13/ex13.c
ex13_o=bin/ex13

BINARIES += $(ex13_o)

ex13a_s=ex13/ex13a.c
ex13a_o=bin/ex13a

BINARIES += $(ex13a_o)

ex14_s=ex14/ex14.c
ex14_o=bin/ex14

BINARIES += $(ex14_o)

ex15_s=ex15/ex15.c
ex15_o=bin/ex15

BINARIES += $(ex15_o)

ex16_s=ex16/ex16.c
ex16_o=bin/ex16

BINARIES += $(ex16_o)

ex17_s=ex17/ex17.c
ex17_o=bin/ex17

BINARIES += $(ex17_o)

all: bin $(BINARIES)

bin:
	mkdir bin

$(ex2_o): $(ex2_s)
	cc $< -o $@ $(CFLAGS)
$(ex3_o): $(ex3_s)
	cc $< -o $@ $(CFLAGS)
$(ex4_o): $(ex4_s)
	cc $< -o $@ $(CFLAGS)
$(ex5_o): $(ex5_s)
	cc $< -o $@ $(CFLAGS)
$(ex6_o): $(ex6_s)
	cc $< -o $@ $(CFLAGS)
$(ex7_o): $(ex7_s)
	cc $< -o $@ $(CFLAGS)
$(ex8_o): $(ex8_s)
	cc $< -o $@ $(CFLAGS)
$(ex9_o): $(ex9_s)
	cc $< -o $@ $(CFLAGS)
$(ex10_o): $(ex10_s)
	cc $< -o $@ $(CFLAGS)
$(ex11_o): $(ex11_s)
	cc $< -o $@ $(CFLAGS)
$(ex12_o): $(ex12_s)
	cc $< -o $@ $(CFLAGS)
$(ex13_o): $(ex13_s)
	cc $< -o $@ $(CFLAGS)
$(ex13a_o): $(ex13a_s)
	cc $< -o $@ $(CFLAGS)
$(ex14_o): $(ex14_s)
	cc $< -o $@ $(CFLAGS)
$(ex15_o): $(ex15_s)
	cc $< -o $@ $(CFLAGS)
$(ex16_o): $(ex16_s)
	cc $< -o $@ $(CFLAGS)
$(ex17_o): $(ex17_s)
	cc $< -o $@ $(CFLAGS)


example2: bin $(ex2_o)
example3: bin $(ex3_o)
example4: bin $(ex4_o)
example5: bin $(ex5_o)
example6: bin $(ex6_o)
example7: bin $(ex7_o)
example8: bin $(ex8_o)
example9: bin $(ex9_o)
example10: bin $(ex10_o)
example11: bin $(ex11_o)
example12: bin $(ex12_o)
example13: bin $(ex13_o)
example13a: bin $(ex13a_o)
example14: bin $(ex14_o)
example15: bin $(ex15_o)
example16: bin $(ex16_o)
example17: bin $(ex17_o)

clean:
	rm -f $(BINARIES)
	rmdir bin

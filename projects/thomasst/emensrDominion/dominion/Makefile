CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

unitTest1: unittest1.c dominion.o rngs.o
	gcc -o unitTest1 -g unittest1.c unittest_helper.c dominion.o rngs.o $(CFLAGS)

unitTest2: unittest2.c dominion.o rngs.o
	gcc -o unitTest2 -g unittest2.c unittest_helper.c dominion.o rngs.o $(CFLAGS)

unitTest3: unittest3.c dominion.o rngs.o
	gcc -o unitTest3 -g unittest3.c unittest_helper.c dominion.o rngs.o $(CFLAGS)

unitTest4: unittest4.c dominion.o rngs.o
	gcc -o unitTest4 -g unittest4.c unittest_helper.c dominion.o rngs.o $(CFLAGS)

unitTest5: unittest5.c dominion.o rngs.o
	gcc -o unitTest5 -g unittest5.c unittest_helper.c dominion.o rngs.o $(CFLAGS)

unittestresults: unitTest1 unitTest2 unitTest3 unitTest4 unitTest5
	./unittest1 >> unittestresults.out
	./unittest2 >> unittestresults.out
	./unitTest3 >> unittestresults.out
	./unitTest4 >> unittestresults.out
	./unitTest5 >> unittestresults.out

randomTest1: randomtestcard1.c dominion.o rngs.o
	gcc -o randomTest1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)

randomTest2: randomtestcard2.c dominion.o rngs.o
	gcc -o randomTest2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)

randomTest3: randomtestcard3.c dominion.o rngs.o
	gcc -o randomTest3 -g randomtestcard3.c dominion.o rngs.o $(CFLAGS)

randomtestresults: randomTest1 randomTest2 randomTest3
	./randomTest1 >> randomtestresults.out
	./randomTest2 >> randomtestresults.out
	./randomTest3 >> randomtestresults.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe

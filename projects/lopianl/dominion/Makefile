CFLAGS= -Wall -fpic -coverage -lm -std=c99

default: curseTest

deckCountTest: tributeDeckCountUnitTests.c dominion.o rngs.o
	echo "tributeDeckCountUnitTests.c:" &> tests.out
	gcc -o tributeDeckCountUnitTests -g tributeDeckCountUnitTests.c dominion.o rngs.o $(CFLAGS)
	tributeDeckCountUnitTests >> tests.out

actionTest: tributeActionsUnitTests.c dominion.o rngs.o
	echo "tributeActionsUnitTests.c:" &> tests.out
	gcc -o tributeActionsUnitTests -g tributeActionsUnitTests.c dominion.o rngs.o $(CFLAGS)
	tributeActionsUnitTests >> tests.out

curseTest: tributeCurseUnitTests.c dominion.o rngs.o
	echo "tributeCurseUnitTests.c:" &> tests.out
	gcc -o tributeCurseUnitTests -g tributeCurseUnitTests.c dominion.o rngs.o $(CFLAGS)
	tributeCurseUnitTests >> tests.out

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)


#########

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


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o tributeDeckCountUnitTests tributeActionsUnitTests tributeCurseUnitTests playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe

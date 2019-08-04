#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int testNum = 0;
int choice1;
int estateCard;
int estateCardIdx;
int estateSupply;
struct gameState G;
int initHandCount = 5;
int initCoins = 8;
int initDiscardCount = 0;
int initNumBuys = 0;
int expNumBuys, expHandCount, expCoins, expDiscardCount, expEstateSupply;

void testCard();
int baronEffect(struct gameState *state, int choice1, int currentPlayer);

void asserttrue(int actual, int expected) {
	if(actual == expected) {
		printf("CORRECT\n");
	}
	else {
		printf("INCORRECT\n");
	}
}

void checkResults() {
	printf("G.numBuys == %d, expected == %d -> ", G.numBuys, expNumBuys);
	asserttrue(G.numBuys, expNumBuys);

	printf("G.handCount[player] == %d, expected == %d -> ", G.handCount[0], expHandCount);
	asserttrue(G.handCount[0], expHandCount);

	printf("G.coins == %d, expected == %d -> ", G.coins, expCoins);
	asserttrue(G.coins, expCoins);

	printf("G.discardCount[player] == %d, expected == %d -> ", G.discardCount[0], expDiscardCount);
	asserttrue(G.discardCount[0], expDiscardCount);

	printf("G.supplyCount[estate] == %d, expected == %d -> ", G.supplyCount[estate], expEstateSupply);
	asserttrue(G.supplyCount[estate], expEstateSupply);

	printf("********************************************************\n");
}

void printInfo() {
	printf("\n********************************************************\n");
	printf("Testing with the following values:\n");
	printf("choice1 == %d\n", choice1);
	printf("G.handCount[player] == %d\n", initHandCount);
	if(estateCard == 1) {
		printf("G.hand[player][%d] == estate\n", estateCardIdx);
	}
	else {
		printf("No estate card in player's hand\n");
	}
	printf("G.supplyCount[estate] == %d\n\n", estateSupply);
}

void setGame() {
	int seed = 1000;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int numPlayers = 2;
	int cards[MAX_HAND];

	for(int i = 0; i < MAX_HAND; i++) {
		if(i == estateCardIdx) {
			cards[i] = estate;
		}
		else {
			cards[i] = baron;
		}	
	}	

	memset(&G, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G);

	G.handCount[0] = initHandCount;
	memcpy(G.hand[0], cards, sizeof(int) * initHandCount);
	G.coins = initCoins;
	G.discardCount[0] = initDiscardCount;
	G.numBuys = initNumBuys;
	G.supplyCount[estate] = estateSupply;
}

void setExpectedValues() {
	expNumBuys = initNumBuys + 1;

	if(choice1 > 0) {
		if(estateCard >= 1) {
			expHandCount = initHandCount - 2;
			expCoins = initCoins + 4;
			expDiscardCount = initDiscardCount + 2;
			expEstateSupply = estateSupply;

			return;
		}
	}

	if(estateSupply > 0) {
		expHandCount = initHandCount;
		expCoins = initCoins;
		expDiscardCount = initDiscardCount + 1;
		expEstateSupply = estateSupply - 1;
	}
	else {
		expHandCount = initHandCount;
		expCoins = initCoins;
		expDiscardCount = initDiscardCount;
		expEstateSupply = estateSupply;
	}
}

void setValues() {
	initHandCount = rand() % MAX_HAND;

	if(testNum % 10 == 0) {
		choice1 = -1;
	}
	else if(testNum % 5 == 0) {
		choice1 = 0;
	}
	else {
		choice1 = rand() % 10 + 1;
	}

	estateCard = rand() % 3;

	if(estateCard >= 1) {
		if(testNum % 3 == 0) {
			estateCardIdx = 0;
		}
		else if (testNum % 5 == 0) {
			estateCardIdx = initHandCount-1;
		}
		else {
			estateCardIdx = rand() % initHandCount;
		}
	}

	if(testNum % 15 == 0) {
		estateSupply = 0;
	}
	else {
		estateSupply = rand() % 10;
	}
}

void testCard() {
	while(testNum <= 45 ) {
		setValues();
		setExpectedValues();
		printInfo();
		setGame();

		baronEffect(&G, choice1, 0);

		checkResults();

		testNum++;
	}
}

int main() {
	clock_t start, end;
	double execTime;

	srand(time(NULL));

	printf("TESTING baronEffect():\n\n");

	start = clock();
	testCard();
	end = clock();

	execTime = ((double) (end-start)) / CLOCKS_PER_SEC;

	printf("\nExecution time: %f\n\n\n", execTime);
	return 0;
}
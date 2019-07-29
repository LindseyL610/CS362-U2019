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
struct gameState G;
int numPlayers = 3;
int initNumActions = 0;
int initDiscardCount = 0;
int initCoins = 8;
int initDeckCount = 6;
int initHandCount[3];
int expNumActions, expCoins;
int expDiscardCount[3];
int expDeckCount[3];
int expHandCount[3];
int minionCard, minionCardIdx, choice1, choice2;

int playMinion(struct gameState*, int, int, int, int);

void asserttrue(int actual, int expected) {
	if(actual == expected) {
		printf("CORRECT\n");
	}
	else {
		printf("INCORRECT\n");
	}
}

void checkResults() {
	for(int i = 0; i < numPlayers; i++) {
		printf("G.discardCount[%d] == %d, expected == %d -> ", i, G.discardCount[i], expDiscardCount[i]);
		asserttrue(G.discardCount[i], expDiscardCount[i]);

		printf("G.deckCount[%d] == %d, expected == %d -> ", i, G.deckCount[i], expDeckCount[i]);
		asserttrue(G.deckCount[i], expDeckCount[i]);

		printf("G.handCount[%d] == %d, expected == %d -> ", i, G.handCount[i], expHandCount[i]);
		asserttrue(G.handCount[i], expHandCount[i]);
	}

	printf("\nG.numActions == %d, expected == %d -> ", G.numActions, expNumActions);
	asserttrue(G.numActions, expNumActions);

	printf("G.coins == %d, expected == %d -> ", G.coins, expCoins);
	asserttrue(G.coins, expCoins);
	
	printf("********************************************************\n");
}

void printInfo() {
	printf("\n********************************************************\n");
	printf("Testing with the following values:\n");
	if(minionCard >= 1) {
		printf("G.hand[currentPlayer][%d] == minion\n", minionCardIdx);
	}
	else {
		printf("No minion card in player's hand\n");
	}
	printf("choice1 == %d\n", choice1);
	printf("choice2 == %d\n\n", choice2);
}

void setGame() {
	int seed = 1000;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int cards[MAX_HAND];

	for(int i = 0; i < MAX_HAND; i++) {
		if((minionCard >= 1) && (i == minionCardIdx)) {
			cards[i] = minion;
		}
		else {
			cards[i] = baron;
		}	
	}	

	memset(&G, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G);

	for(int i = 0; i < numPlayers; i++) {
		G.handCount[i] = initHandCount[i];
		G.deckCount[i] = initDeckCount;
		G.discardCount[i] = initDiscardCount;
		memcpy(G.hand[i], cards, sizeof(int) * initHandCount[i]);
		memcpy(G.deck[i], cards, sizeof(int) * initDeckCount);
		memcpy(G.discard[i], cards, sizeof(int) * initDiscardCount);
	}

	G.coins = initCoins;
	G.numActions = initNumActions;
}

void setExpectedValues() {
	if(minionCard < 1 || (choice1 < 1 && choice2 < 1)) {
		for(int i = 0; i < numPlayers; i++) {
			expDiscardCount[i] = initDiscardCount;
			expDeckCount[i] = initDeckCount;
			expHandCount[i] = initHandCount[i];
		}
		expNumActions = initNumActions;
		expCoins = initCoins;
	}
	else if(choice1 > 0) {
		expDiscardCount[0] = initDiscardCount+1;
		expDeckCount[0] = initDeckCount;
		expHandCount[0] = initHandCount[0] - 1;

		for(int i = 1; i < numPlayers; i++) {
			expDiscardCount[i] = initDiscardCount;
			expDeckCount[i] = initDeckCount;
			expHandCount[i] = initHandCount[i];
		}

		expNumActions = initNumActions + 1;
		expCoins = initCoins + 2;
	}
	else if(choice2 > 0) {
		expDiscardCount[0] = initDiscardCount + initHandCount[0];
		expDeckCount[0] = initDeckCount - 4;
		expHandCount[0] = 4;

		for(int i = 1; i < numPlayers; i++) {
			if(initHandCount[i] > 4) {
				expDiscardCount[i] = initDiscardCount + initHandCount[i];
				expDeckCount[i] = initDeckCount - 4;
				expHandCount[i] = 4;
			}
			else {
				expDiscardCount[i] = initDiscardCount;
				expDeckCount[i] = initDeckCount;
				expHandCount[i] = initHandCount[i];
			}
		}

		expNumActions = initNumActions + 1;
		expCoins = initCoins;
	}
}

void setValues() {
	int minCards;

	if(testNum % 5 == 0) {
		minCards = 5;
	}
	else {
		minCards = 1;
	}

	for(int i = 0; i < numPlayers; i++) {
		initHandCount[i] = rand() % 15 + minCards;
	}

	minionCard = rand() % 3;

	if(minionCard >= 1) {
		minionCardIdx = rand() % initHandCount[0];
	}
	else {
		minionCardIdx = 0;
	}

	choice1 = rand() % 4 - 1;
	choice2 = rand() % 4 - 1;
}

void testCard() {
	while(testNum <= 30) {
		setValues();
		setExpectedValues();
		printInfo();
		setGame();

		playMinion(&G, minionCardIdx, 0, choice1, choice2);

		checkResults();

		testNum++;
	}
}

int main() {
	srand(time(NULL));
	
	printf("TESTING playMinion():\n\n");

	testCard();

	return 0;
}

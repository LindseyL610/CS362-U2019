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
int tributeCard, tributeCardIdx, initHandCount, initDeckCount, initDiscardCount, initCoins, initNumActions, 
	currentPlayer, nextPlayer, card1, card2;
int expHandCount1, expHandCount2, expDeckCount1, expDeckCount2, expDiscardCount1, expDiscardCount2, expCoins, expNumActions;
int type[26] = {6, 2, 2, 2, 1, 1, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int playTribute(struct gameState*, int, int);

void asserttrue(int actual, int expected) {
	if(actual == expected) {
		printf("CORRECT\n");
	}
	else {
		printf("INCORRECT\n");
	}
}

void checkResults() {
	int hold;
	if(currentPlayer == 1) {
		hold = expHandCount1;
		expHandCount1 = expHandCount2;
		expHandCount2 = hold;

		hold = expDeckCount1;
		expDeckCount1 = expDeckCount2;
		expDeckCount2 = hold;

		hold = expDiscardCount1;
		expDiscardCount1 = expDiscardCount2;
		expDeckCount2 = hold;
	}
	printf("G.handCount[0] == %d, expected == %d -> ", G.handCount[0], expHandCount1);
	asserttrue(G.handCount[0], expHandCount1);
	printf("G.deckCount[0] == %d, expected == %d -> ", G.deckCount[0], expDeckCount1);
	asserttrue(G.deckCount[0], expDeckCount1);
	printf("G.discardCount[0] == %d, expected == %d -> ", G.discardCount[0], expDiscardCount1);
	asserttrue(G.discardCount[0], expDiscardCount1);

	printf("G.handCount[1] == %d, expected == %d -> ", G.handCount[1], expHandCount2);
	asserttrue(G.handCount[1], expHandCount2);
	printf("G.deckCount[1] == %d, expected == %d -> ", G.deckCount[1], expDeckCount2);
	asserttrue(G.deckCount[1], expDeckCount2);
	printf("G.discardCount[1] == %d, expected == %d -> ", G.discardCount[1], expDiscardCount2);
	asserttrue(G.discardCount[1], expDiscardCount2);

	printf("G.coins == %d, expected == %d -> ", G.coins, expCoins);
	asserttrue(G.coins, expCoins);
	printf("G.numActions == %d, expected == %d -> ", G.numActions, expNumActions);
	asserttrue(G.numActions, expNumActions);

	printf("********************************************************\n");
}

void printCardType(int c) {
	if(c == 0) {
		printf("action\n");
	}
	else if(c == 1) {
		printf("treasure\n");
	}
	else if(c == 2) {
		printf("victory\n");
	}
	else if(c == 4) {
		printf("action + victory\n");
	}
	else if(c == 6) {
		printf("curse\n");
	}
}

void printInfo() {
	printf("\n********************************************************\n");
	printf("Testing with the following values:\n");
	printf("Index of current player == %d\n", currentPlayer);
	printf("Index of next player == %d\n", nextPlayer);
	printf("Card 1 == %d, card 1 type == ", card1);
	printCardType(type[card1]);
	printf("Card 2 == %d, card 2 type == ", card2);
	printCardType(type[card2]);
	printf("\n");
}

void setGame() {
	int seed = 1000;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int numPlayers = 2;
	int hand[MAX_HAND];
	int discard[MAX_HAND];
	int deck[MAX_DECK];
	int card1Set = 0;
	int card2Set = 0;

	for(int i = 0; i < MAX_HAND; i++) {
		if(i == tributeCardIdx) {
			hand[i] = tribute;
		}
		else {
			hand[i] = baron;
		}
	}

	for(int i = 0; i < MAX_DECK; i++) {
		if((card1Set == 0) && (i < initDeckCount)) {
			deck[i] == card1;
			card1Set = 1;
		}
		else if((card2Set == 0) && (i < initDeckCount)) {
			deck[i] = card2;
			card2Set = 1;
		}
		else {
			deck[i] = baron;
		}
	}

	for(int i = 0; i < MAX_HAND; i++) {
		if((card1Set == 0) && (i < initDiscardCount)) {
			discard[i] = card1;
			card1Set = 1;
		}
		else if((card2Set == 0) && (i < initDiscardCount)) {
			discard[i] = card2;
			card2Set = 1;
		}
		else {
			discard[i] = baron;
		}
	}

	memset(&G, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G);

	for(int i = 0; i < numPlayers; i++) {
		G.handCount[i] = initHandCount;
		G.deckCount[i] = initDeckCount;
		G.discardCount[i] = initDiscardCount;
		memcpy(G.hand[i], hand, sizeof(int) * initHandCount);
		memcpy(G.deck[i], deck, sizeof(int) * initDeckCount);
		memcpy(G.discard[i], discard, sizeof(int) * initDiscardCount);
	}

	G.coins = initCoins;
	G.numActions = initNumActions;
}

void cardEf(int t) {
	if(t == 0) {
		expNumActions += 2;
	}
	else if(t == 1) {
		expCoins += 2;
	}
	else if(t == 2) {
		expHandCount1 += 2;
	}
	else if(t == 4) {
		expNumActions += 2;
		expHandCount1 += 2;
	}
}

void setExpectedValues() {
	expHandCount1 = initHandCount - 1;
	expHandCount2 = initHandCount;
	expDeckCount1 = initDeckCount;
	expDeckCount2 = initDeckCount;
	expDiscardCount1 = initDiscardCount;
	expDiscardCount2 = initDiscardCount;
	expCoins = initCoins;
	expNumActions = initNumActions;

	int bothCards, type1, type2;

	type1 = type[card1];
	type2 = type[card2];

	if(tributeCard < 1) {
		expHandCount1 = initHandCount;
		return;
	}

	if(card1 != card2) {
		bothCards = 1;
	}
	else {
		bothCards = 0;
	}

	if(initDiscardCount + initDeckCount <= 1) {
		if(initDeckCount > 0) {
			bothCards = 0;
			expDeckCount2 = initDeckCount - 1;
		}
		else if(initDiscardCount > 0) {
			bothCards = 0;
		}
		else {
			bothCards = -1;
		}
	}
	else {
		if(initDeckCount > 1) {
			expDeckCount2 = initDeckCount - 2;
		}
		else if(initDeckCount == 1) {
			expDeckCount2 = initDeckCount - 1;
		}
	}

	switch(bothCards) {
		case -1:
		return;
		break;

		case 0:
		cardEf(type1);
		break;

		case 1:
		cardEf(type1);
		cardEf(type2);
		break;
	}
}

void setValues() {
	initHandCount = rand() % 20 + 1;
	initDeckCount = rand() % 20;
	initDiscardCount = rand() % 20;

	tributeCard = rand() % 4;

	if(tributeCard >= 1) {
		tributeCardIdx = rand() % initHandCount;
	}
	else {
		tributeCardIdx = -1;
	}

	currentPlayer = rand() % 2;

	if(currentPlayer == 0) {
		nextPlayer = 1;
	}
	else {
		nextPlayer = 0;
	}

	initCoins = rand() % 50;
	initNumActions = rand() % 50;

	card1 = rand() % 26;
	card2 = rand() % 26;
}

void testCard() {
	while(testNum < 750) {
		setValues();
		setExpectedValues();
		printInfo();
		setGame();

		playTribute(&G, nextPlayer, currentPlayer);

		checkResults();

		testNum++;
	}
}

int main() {
	srand(time(NULL));

	printf("TESTING playTribute():\n\n");

	testCard();

	return 0;
}

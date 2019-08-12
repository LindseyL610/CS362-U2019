#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

void assertTrue(int actual, int expected) {
	if(actual == expected) {
		printf("CORRECT\n");
	}
	else {
		printf("INCORRECT\n");
	}
}

void checkValues(int actDeckCount, int expDeckCount) {
	printf("\nG.deckCount[1] == %d, expected == %d -> ", actDeckCount, expDeckCount);
	assertTrue(actDeckCount, expDeckCount);
}

void printInfo(int deckCount) {
	printf("\n**********************************************************\n");
	printf("Playing a tribute card with the following starting values:\n");
	printf("G.deckCount[1] == %d\n", deckCount);
}

int main() {
	int seed = 1000;
	int startCoins = 8;
	int numPlayers = 2;
	int r;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int cards[MAX_HAND];
	int deck[MAX_HAND];
	int discard[MAX_HAND];
	struct gameState G;
	int handCount = 5;
	int deckCount;
	int discardCount = 5;
	int expDeckCount;


	for(int i = 0; i < MAX_HAND; i++) {
		cards[i] = tribute; 
		deck[i] = baron;
		discard[i] = baron;
	}

	for(int run = 0; run < 5; run++) {
		switch(run) {
		case 0:
			deckCount = 73;
			break;

		case 1:
			deckCount = 33;
			break;

		case 2:
			deckCount = 21;
			break;

		case 3:
			deckCount = 101;
			break;

		case 4:
			deckCount = 6;
			break;
		}

		expDeckCount = deckCount - 2;

		memset(&G, 23, sizeof(struct gameState));
		r = initializeGame(numPlayers, k, seed, &G);

		G.handCount[0] = handCount;
		memcpy(G.hand[0], cards, sizeof(int) * handCount);
		G.handCount[1] = handCount;
		memcpy(G.hand[1], cards, sizeof(int) * handCount);
		G.deckCount[1] = deckCount;
		memcpy(G.deck[1], deck, sizeof(int) * deckCount);
		G.discardCount[1] = discardCount;
		memcpy(G.discard[1], discard, sizeof(int) * discardCount);

		G.phase = 0;
		G.whoseTurn = 0;

		printInfo(deckCount);

		playCard(0, 0, 0, 0, &G);

		checkValues(G.deckCount[1], expDeckCount);
	}
}

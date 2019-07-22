#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <signal.h>
#include <stdlib.h>

void asserttrue(int actual, int expected) {
	if(actual != expected) {
		printf("This result is incorrect\n");
	}
	else {
		printf("This result is correct\n");
	}
}

int main() {
	int seed = 1000;
	int startCoins = 8;
	int numPlayers = 2;
	int r;
	int handCount = 5;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int cards[5] = {estate, adventurer, council_room, feast, gardens};
	struct gameState G;

	printf("\n\nTESTING drawCard():\n");

	for(int i = 0; i < 3; i++) {
		int deckCount;
		if(i == 0) {
			deckCount == -1;
		}
		else if(i == 1) {
			deckCount = 0;
		}
		else {
			deckCount = 5;
		}

		memset(&G, 23, sizeof(struct gameState));
		r = initializeGame(numPlayers, k, seed, &G);
		G.handCount[0] = handCount;
		memcpy(G.hand[0], cards, sizeof(int) * handCount);
		G.deckCount[0] = deckCount;
		memcpy(G.deck[0], cards, sizeof(int) * deckCount);
		G.discardCount[0] = handCount;
		memcpy(G.discard[0], cards, sizeof(int) * handCount);

		printf("**************************************************\n");
		printf("Values before calling drawCard():\n");
		printf("G.discardCount[0] == %d\n", G.discardCount[0]);
		printf("G.deckCount[0] == %d\n", G.deckCount[0]);
		printf("G.handCount[0] == %d\n", G.handCount[0]);

		drawCard(0, &G);

		int expDiscCount;

		if(deckCount <= 0) {
			expDiscCount = 0;
		}
		else {
			expDiscCount = handCount;
		}

		printf("\nValues after calling drawCard():\n");
		printf("G.discardCount[0] == %d -> ", G.discardCount[0]);
		asserttrue(G.discardCount[0], expDiscCount);
		printf("G.deckCount[0] == %d -> ", G.deckCount[0]);
		asserttrue(G.deckCount[0], handCount-1);
		printf("G.handCount[0] == %d -> ", G.handCount[0]);
		asserttrue(G.handCount[0], handCount+1);
		printf("G.hand[0]: ");
		for(int j = 0; j < G.handCount[0]; j++) {
			printf("%d", G.hand[0][j]);

			if(j < G.handCount[0]-1) {
				printf(", ");
			}
		}
		printf(" - > ");

		int correctHand = 0;

		for(int j = 0; j < handCount; j++) {
			if(G.hand[0][handCount] == cards[j]) {
				correctHand = 1;
			}
		}

		asserttrue(correctHand, 1);

		printf("**************************************************\n\n");
	}

	return 0;
}

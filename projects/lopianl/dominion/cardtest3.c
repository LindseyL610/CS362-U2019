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
	int match, nextPlayer;

	printf("TESTING endTurn():\n");

	for(int i = 0; i < 2; i++) {
		memset(&G, 23, sizeof(struct gameState));
		r = initializeGame(numPlayers, k, seed, &G);
		G.whoseTurn = i;

		if(i == 1) {
			nextPlayer = 0;
		}
		else {
			nextPlayer = 1;
		}

		G.handCount[i] = handCount;
		memcpy(G.hand[i], cards, sizeof(int) * handCount);
		G.discardCount[i] = 0;

		printf("**************************************************\n");
		printf("Testing for player %d out of %d total players\n", i+1, numPlayers);

		printf("G.handCount[%d] before calling endTurn(): %d\n", i, G.handCount[i]);
		printf("G.hand[%d] before calling endTurn(): ", i);
		for(int j = 0; j < G.handCount[i]; j++) {
			printf("%d", G.hand[i][j]);

			if(j < G.handCount[i]-1) {
				printf(", ");
			}
		}
		printf("\nG.discardCount[%d] before calling endTurn(): %d\n", i, G.discardCount[i]);

		printf("Next player's hand count before calling endTurn(): %d\n\n", G.handCount[nextPlayer]);

		endTurn(&G);

		printf("G.handCount[%d] after calling endTurn(): %d - > ", i, G.handCount[i]);
		asserttrue(G.handCount[i], 0);
		printf("G.discardCount[%d] after calling endTurn(): %d -> ", i, G.discardCount[i]);
		asserttrue(G.discardCount[i], handCount);
		printf("G.discard[%d] after calling endTurn(): ", i);

		for(int x = 0; x < G.discardCount[i]; x++) {
			printf("%d", G.discard[i][x]);

			if(x < G.discardCount[i]-1) {
				printf(", ");
			}
		}

		printf(" -> ");

		for(int x = 0; x < G.discardCount[i]; x++) {
			match = 0;

			for(int y = 0; y < handCount; y++) {
				if(G.discard[i][x] == cards[y]) {
					match = 1;
				}
			}

			if(match != 1) {
				break;
			}
		}

		asserttrue(match, 1);

		printf("Next player's hand count after calling endTurn(): %d -> ", G.handCount[nextPlayer]);
		asserttrue(G.handCount[nextPlayer], handCount);

		printf("**************************************************\n\n");
	}

	return 0;
}

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <signal.h>
#include <stdlib.h>

int asserttrue(int actual, int expected) {
	if(actual != expected) {
		printf("This result is incorrect\n");
		return -1;
	}
	else {
		printf("This result is correct\n");
		return 0;
	}
}

int main() {
	int seed = 1000;
	int startCoins = 8;
	int numPlayers = 2;
	int r, res;
	int choice1, playerEstates, supplyEstate = 3;
	int expHandCount, expDiscCount, expSupplyCount, expCoins;
	int deckCount;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int cards[5] = {estate, adventurer, council_room, feast, gardens};
	int maxCards[MAX_DECK+1];
	struct gameState G;

	for(int i = 0; i < MAX_DECK+1; i++) {
		maxCards[i] = estate;
	}

	printf("TESTING shuffle():\n");

	for(int i = 0; i <= 5; i++) {
		printf("\n**************************************************\n");
		printf("Testing with the following cards in the player's deck: ");

		for(int j = 0; j < i; j++) {
			printf("%d", cards[j]);

			if(j < i-1) {
				printf(", ");
			}
		}
		printf("\n");

		deckCount = i;

		memset(&G, 23, sizeof(struct gameState));
		r = initializeGame(numPlayers, k, seed, &G);

		G.deckCount[0] = deckCount;
		memcpy(G.deck[0], cards, sizeof(int) * deckCount);

		res = shuffle(0, &G);

		if(res != 0) {
			printf("shuffle() was not successful\n");
			printf("Player had %d card(s) in their deck\n", i);

			if(i < 1) {
				printf("Player must have at least 1 card in their deck\n");
				printf("shuffle() SHOULD NOT have been successful\n");
			}
			else {
				printf("Player had at least 1 card in their deck\n");
				printf("shuffle() SHOULD have been successful\n");
			}
		}
		else {
			printf("G.deckCount[0] == %d, expected == %d -> ", G.deckCount[0], deckCount);
			res = asserttrue(G.deckCount[0], deckCount);

			if(res == 0) {
				printf("The shuffled deck contains the following cards: ");
				for(int k = 0; k < i; k++) {
					printf("%d", G.deck[0][k]);

					if(k < i-1) {
						printf(", ");
					}
					else {
						printf(" -> ");
					}
				}

				for(int x = 0; x < G.deckCount[0]; x++) {
					int match = 0;

					for(int y = 0; y < deckCount; y++) {
						if(G.deck[0][x] == cards[y]) {
							match = 1;
						}
					}

					if(match != 1) {
						res = -1;
						break;
					}
				}

				asserttrue(res, 0);
			}
		}
		printf("**************************************************\n\n");
	}

	printf("\n**************************************************\n");
	printf("Testing with %d cards in the player's deck, more than the maximum of %d:\n", MAX_DECK+1, MAX_DECK);

	memset(&G, 23, sizeof(struct gameState));
	r = initializeGame(numPlayers, k, seed, &G);

	G.deckCount[0] = MAX_DECK + 1;
	memcpy(G.deck[0], maxCards, sizeof(int) * (MAX_DECK+1));

	res = shuffle(0, &G);

	if(res != -1) {
		printf("shuffle() was successful\n");
		printf("shuffle() SHOULD NOT have succeeded\n");
	}
	else {
		printf("shuffle() was not successful\n");
		printf("shuffle() SHOULD NOT have succeeded\n");
	}
	printf("**************************************************\n\n");

	return 0;
}

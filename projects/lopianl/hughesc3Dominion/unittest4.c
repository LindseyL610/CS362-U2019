#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int tributeEffect(struct gameState *state, int currentPlayer, int nextPlayer);

int main() {
	int seed = 1000;
	int startCoins = 8;
	int numPlayers = 2;
	int r;
	int handCount = 5;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int cards[MAX_HAND];
	int deck[MAX_HAND];
	int discard[MAX_HAND];
	struct gameState G;
	int discCount, deckCount, card1, card2;

	for(int i = 0; i < MAX_HAND; i++) {
		cards[i] = tribute;
	}

	printf("TESTING tributeEffect():\n\n");

	for(int i = 1; i <= 14; i++) {
		memset(&G, 23, sizeof(struct gameState));
		r = initializeGame(numPlayers, k, seed, &G);

		if(i <= 9) {
			discCount = 0;
			deckCount = 2;
		}
		else if(i <= 13) {
			discCount = 1;
			deckCount = 1;
		}
		else {
			discCount = 0;
			deckCount = 2;
		}

		switch(i) {
			case 1:
			card1 = steward;
			card2 = steward;
			break;

			case 2:
			card1 = steward;
			card2 = tribute;
			break;

			case 3:
			card1 = silver;
			card2 = silver;
			break;

			case 4:
			card1 = silver;
			card2 = gold;
			break;

			case 5:
			card1 = estate;
			card2 = estate;
			break;

			case 6:
			card1 = estate;
			card2 = province;
			break;

			case 7:
			card1 = steward;
			card2 = silver;
			break;

			case 8:
			card1 = steward;
			card2 = province;
			break;

			case 9:
			card1 = silver;
			card2 = great_hall;
			break;

			case 10:
			card1 = curse;
			card2 = curse;
			break;

			case 11:
			card1 = steward;
			card2 = gold;
			break;

			case 12:
			card1 = silver;
			card2 = gold;
			break;

			case 13:
			card1 = province;
			card2 = gold;
			break;

			case 14:
			for(int j = 0; j < MAX_HAND; j++) {
				cards[j] = silver;
			}
			card1 = province;
			card2 = gold;
			break;
		}

		G.handCount[0] = handCount;
		memcpy(G.hand[0], cards, sizeof(int) * handCount);
		G.handCount[1] = handCount;
		memcpy(G.hand[1], cards, sizeof(int) * handCount);

		if(discCount == 0) {
			for(int j = 0; j < MAX_HAND; j++) {
				discard[j] = -1;
			}
		}
		else {
			for(int j = 0; j < MAX_HAND; j++) {
				discard[j] = card2;
			}
		}

		G.discardCount[1] = discCount;
		memcpy(G.discard[1], discard, sizeof(int) * discCount);

		if(deckCount == 2) {
			for(int j = 0; j < MAX_HAND; j++) {
				if(j == 0) {
					deck[j] = card1;
				}
				else {
					deck[j] = card2;
				}
			}
		}
		else {
			for(int j = 0; j < MAX_HAND; j++) {
				deck[j] = card1;
			}
		}

		G.deckCount[1] = deckCount;
		memcpy(G.deck[1], deck, sizeof(int) * deckCount);

		tributeEffect(&G, 0, 1);
	}

	return 0;
}
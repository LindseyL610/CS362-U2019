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

void checkValues(int actNumActions, int expNumActions) {
	printf("\nG.numActions == %d, expected == %d -> ", actNumActions, expNumActions);
	assertTrue(actNumActions, expNumActions);
}

void printInfo(int card1, int card2, int deckCount, int discardCount) {
	int cardNum = 1;

	printf("\n**********************************************************\n");
	printf("Playing a tribute card with the following starting values:\n");
	printf("G.deckCount[1] == %d\n", deckCount);
	printf("G.discardCount[1] == %d\n", discardCount);
	if(card1 != -1) {
		printf("Tribute card #%d: ", cardNum);

		if(card1 == baron) {
			printf("baron\n");
		}
		else if(card1 == minion) {
			printf("minion\n");
		}
		else if(card1 == copper) {
			printf("copper\n");
		}
		else {
			printf("estate\n");
		}

		cardNum++;
	}
	if(card2 != -1) {
		printf("Tribute card #%d: ", cardNum);

		if(card2 == baron) {
			printf("baron\n");
		}
		else if(card2 == minion) {
			printf("minion\n");
		}
		else if(card2 == copper) {
			printf("copper\n");
		}
		else {
			printf("estate\n");
		}

		cardNum++;
	}
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
	int p1DeckCount, p1DiscardCount, p0ExpActionsCount, card1, card2;

	for(int i = 0; i < MAX_HAND; i++) {
		cards[i] = tribute; 
	}

	for(int run = 0; run < 9; run++) {
		switch(run) {
			case 0:
			p1DeckCount = 2;
			p1DiscardCount = 0;
			card1 = baron;
			card2 = minion;
			p0ExpActionsCount = 4;
			break;

			case 1:
			p1DeckCount = 2;
			p1DiscardCount = 0;
			card1 = baron;
			card2 = minion;
			p0ExpActionsCount = 4;
			break;

			case 2:
			p1DeckCount = 2;
			p1DiscardCount = 0;
			card1 = copper;
			card2 = baron;
			p0ExpActionsCount = 2;
			break;

			case 3:
			p1DeckCount = 2;
			p1DiscardCount = 0;
			card1 = baron;
			card2 = estate;
			p0ExpActionsCount = 2;
			break;

			case 4:
			p1DeckCount = 2;
			p1DiscardCount = 0;
			card1 = estate;
			card2 = copper;
			p0ExpActionsCount = 0;
			break;

			case 5:
			p1DeckCount = 2;
			p1DiscardCount = 0;
			card1 = baron;
			card2 = baron;
			p0ExpActionsCount = 2;
			break;

			case 6:
			p1DeckCount = 1;
			p1DiscardCount = 0;
			card1 = baron;
			card2 = -1;
			p0ExpActionsCount = 2;
			break;

			case 7:
			p1DeckCount = 1;
			p1DiscardCount = 0;
			card1 = copper;
			card2 = -1;
			p0ExpActionsCount = 0;
			break;

			case 8:
			p1DeckCount = 0;
			p1DiscardCount = 0;
			card1 = -1;
			card2 = -1;
			p0ExpActionsCount = 0;
		}

		for(int i = 0; i < MAX_DECK; i++) {
			if(i == 0) {
				if(p1DeckCount == 1) {	
					deck[0] = card1;
				}
				if(p1DiscardCount == 1) {
					discard[0] = card2;
				}
			}
			if(p1DeckCount == 2) {
				if(i == 0) {
					deck[0] = card1;
				}
				else if(i == 1) {
					deck[1] = card2;
				}
				else {
					deck[i] = baron;
				}

				discard[i] = baron;
			}
			if(p1DiscardCount == 2) {
				if(i == 0) {
					discard[0] = card1;
				}
				else if(i == 1) {
					discard[1] = card2;
				}
				else {
					discard[i] = baron;
				}

				deck[i] = baron;
			}
		}

		memset(&G, 23, sizeof(struct gameState));
		r = initializeGame(numPlayers, k, seed, &G);

		G.handCount[0] = handCount;
		memcpy(G.hand[0], cards, sizeof(int) * handCount);
		G.handCount[1] = handCount;
		memcpy(G.hand[1], cards, sizeof(int) * handCount);
		G.deckCount[1] = p1DeckCount;
		memcpy(G.deck[1], deck, sizeof(int) * p1DeckCount);
		G.discardCount[1] = p1DiscardCount;
		memcpy(G.discard[1], discard, sizeof(int) * p1DiscardCount);

		G.phase = 0;
		G.whoseTurn = 0;

		printInfo(card1, card2, p1DeckCount, p1DiscardCount);

		playCard(0, 0, 0, 0, &G);

		checkValues(G.numActions, p0ExpActionsCount);
	}
}

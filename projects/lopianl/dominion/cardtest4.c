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
	int numPlayers = 3;
	int r;
	int handCount = 5;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int cards[5] = {curse, estate, estate, estate, gardens};
	int winningHand[5] = {estate, duchy, province, great_hall, gardens};
	struct gameState G;
	int numwinners, correctWinners;
	int winIdx[3];
	int players[MAX_PLAYERS];

	for(int i = 0; i < MAX_PLAYERS; i++) {
		players[i] = i;
	}

	printf("TESTING endTurn():\n");
	for(int i = 0; i < 6; i++) {
		printf("******************************************************************************\n");

		memset(&G, 23, sizeof(struct gameState));
		r = initializeGame(numPlayers, k, seed, &G);
		G.numPlayers = numPlayers;

		for(int j = 0; j < numPlayers; j++) {
			G.handCount[j] = handCount;
			G.discardCount[j] = handCount;
		}

		switch(i) {
			case 0:
			printf("Testing for Player 0 winning, with all players having an equal number of turns:\n");
			numwinners = 1;
			for(int j = 0; j < numPlayers; j++) {
				winIdx[j] = i;
			}
			memcpy(G.hand[0], winningHand, sizeof(int) * handCount);
			memcpy(G.discard[0], winningHand, sizeof(int) * handCount);
			memcpy(G.hand[1], cards, sizeof(int) * handCount);
			memcpy(G.discard[1], cards, sizeof(int) * handCount);
			memcpy(G.hand[2], cards, sizeof(int) * handCount);
			memcpy(G.discard[2], cards, sizeof(int) * handCount);
			G.whoseTurn = 2;
			break;

			case 1:
			printf("Testing for Player 1 winning, with all players having an equal number of turns:\n");
			numwinners = 1;
			for(int j = 0; j < numPlayers; j++) {
				winIdx[j] = i;
			}
			memcpy(G.hand[0], cards, sizeof(int) * handCount);
			memcpy(G.discard[0], cards, sizeof(int) * handCount);
			memcpy(G.hand[1], winningHand, sizeof(int) * handCount);
			memcpy(G.discard[1], winningHand, sizeof(int) * handCount);
			memcpy(G.hand[2], cards, sizeof(int) * handCount);
			memcpy(G.discard[2], cards, sizeof(int) * handCount);
			G.whoseTurn = 2;
			break;

			case 2:
			printf("Testing for Player 2 winning, with all players having an equal number of turns:\n");
			numwinners = 1;
			for(int j = 0; j < numPlayers; j++) {
				winIdx[j] = i;
			}
			memcpy(G.hand[0], cards, sizeof(int) * handCount);
			memcpy(G.discard[0], cards, sizeof(int) * handCount);
			memcpy(G.hand[1], cards, sizeof(int) * handCount);
			memcpy(G.discard[1], cards, sizeof(int) * handCount);
			memcpy(G.hand[2], winningHand, sizeof(int) * handCount);
			memcpy(G.discard[2], winningHand, sizeof(int) * handCount);
			G.whoseTurn = 2;
			break;

			case 3:
			printf("Testing for Player 0 and Player 2 both winning, with all players having an equal number of turns:\n");
			numwinners = 2;
			winIdx[0] = 0;
			winIdx[1] = 1;
			winIdx[2] = 1;
			memcpy(G.hand[0], winningHand, sizeof(int) * handCount);
			memcpy(G.discard[0], winningHand, sizeof(int) * handCount);
			memcpy(G.hand[1], cards, sizeof(int) * handCount);
			memcpy(G.discard[1], cards, sizeof(int) * handCount);
			memcpy(G.hand[2], winningHand, sizeof(int) * handCount);
			memcpy(G.discard[2], winningHand, sizeof(int) * handCount);
			G.whoseTurn = 2;
			break;

			case 4:
			printf("Testing for all 3 players winning, with all players having an equal number of turns:\n");
			numwinners = 3;
			for(int j = 0; j < numPlayers; j++) {
				winIdx[j] = j;
			}
			memcpy(G.hand[0], winningHand, sizeof(int) * handCount);
			memcpy(G.discard[0], winningHand, sizeof(int) * handCount);
			memcpy(G.hand[1], winningHand, sizeof(int) * handCount);
			memcpy(G.discard[1], winningHand, sizeof(int) * handCount);
			memcpy(G.hand[2], winningHand, sizeof(int) * handCount);
			memcpy(G.discard[2], winningHand, sizeof(int) * handCount);
			G.whoseTurn = 2;
			break;

			case 5:
			printf("Testing for Player 2 winning, with Player 2 having had 1 less turn than Player 0 and Player 1 and all players having\n");
			printf("the same score before accounting for Player 2 having had fewer turns:\n");
			numwinners = 1;
			for(int j = 0; j < numPlayers; j++) {
				winIdx[j] = 2;
			}
			memcpy(G.hand[0], winningHand, sizeof(int) * handCount);
			memcpy(G.discard[0], winningHand, sizeof(int) * handCount);
			memcpy(G.hand[1], winningHand, sizeof(int) * handCount);
			memcpy(G.discard[1], winningHand, sizeof(int) * handCount);
			memcpy(G.hand[2], winningHand, sizeof(int) * handCount);
			memcpy(G.discard[2], winningHand, sizeof(int) * handCount);
			G.whoseTurn = 1;
			break;
		}

		correctWinners = 1;

		printf("Winner(s) after calling getWinners(): ");

		for(int j = 0; j < numPlayers; j++) {
			if(players[j] == 1) {
				printf("%d ", j);
			}
		}

		printf(" -> ");

		int num = 0;

		for(int j = 0; j < numPlayers; j++) {
			correctWinners = 1;
			if(players[j] == 1) {
				correctWinners = 0;
				for(int k = 0; k < numwinners; k++) {
					if(winIdx[k] == j) {
						correctWinners = 1;
						num++;
					}
				}
			}
			if(correctWinners == 0) {
				break;
			}
		}

		if((correctWinners == 1) && (num < numwinners)) {
			correctWinners = 0;
		}

		asserttrue(correctWinners, 1);

		printf("******************************************************************************\n\n");
	}

	return 0;
}

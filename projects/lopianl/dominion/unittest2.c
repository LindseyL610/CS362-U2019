#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

void printInfo(int choice1, int choice2, int minCard, int minPos) {
	printf("**************************************************\n");
	printf("Testing with the following values:\n");
	printf("choice1 == %d\n", choice1);
	printf("choice2 == %d\n", choice2);
	if(minCard == 0) {
		printf("No minion card in player's hand\n");
	}
	else {
		printf("Minion card in player's hand\n");
	}
	printf("minPos == %d\n", minPos);
}

void assertTrue(int actNumAction, int expNumActions, int p0Count, int p0ExpHandCount, int p1Count, int p1ExpHandCount,
				int p2Count, int p2ExpHandCount, int p3Count, int p3ExpHandCount, int p4Count, int p4ExpHandCount,
				int actPlayedCount, int expPlayedCount, int actCoins, int expCoins) {

	printf("\nAfter calling runMinion():\n");
	printf("G.numActions == %d, expected == %d\n", actNumAction, expNumActions);
	printf("G.handCount[0] == %d, expected == %d\n", p0Count, p0ExpHandCount);
	printf("G.handCount[1] == %d, expected == %d\n", p1Count, p1ExpHandCount);
	printf("G.handCount[2] == %d, expected == %d\n", p2Count, p2ExpHandCount);
	printf("G.handCount[3] == %d, expected == %d\n", p3Count, p3ExpHandCount);
	printf("G.handCount[4] == %d, expected == %d\n", p4Count, p4ExpHandCount);
	printf("G.playedCardCount == %d, expected == %d\n", actPlayedCount, expPlayedCount);
	printf("G.coins == %d, expected == %d\n", actCoins, expCoins);
	printf("**************************************************\n\n");
}

int main() {
	int choice1, choice2,  minPos,  minCard;
	int seed = 1000;
	int startCoins = 8;
	int numPlayers = 3;
	int r;
	int handCount = 5;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int cards[MAX_HAND];
	struct gameState G;
	int expNumActions, p0ExpHandCount, p1ExpHandCount, p2ExpHandCount, p3ExpHandCount, p4ExpHandCount, expPlayedCount, expCoins;

	printf("TESTING playMinion():\n\n");

	for(int run = 1; run <= 3; run++) {
		for(int choiceComb = 1; choiceComb <= 3; choiceComb++) {
			switch(choiceComb) {
				case 1:
				choice1 = 1;
				choice2 = 0;
				break;
				case 2:
				choice1 = 0;
				choice2 = 1;
				break;
				case 3:
				choice1 = 0;
				choice2 = 0;
			}

			memset(&G, 500, sizeof(struct gameState));
			r = initializeGame(numPlayers, k, seed, &G);

			G.coins = startCoins; 
			G.numActions = 0;

			for(int i = 0; i < MAX_HAND; i++) {
				if((run == 1) && (i == 0)) {
					cards[i] = minion;
					minPos = i;
					minCard = 1;
				}
				else if((run == 2) && (i == 3)) {
					cards[i] == minion;
					minPos = i;
					minCard = 1;
				}

				cards[i] = duchy;
			}

			if(run == 3) {
				minPos = 0;
				minCard = 0;
			}

			for(int j = 0; j < numPlayers; j++) {
				if(j == 2) {
					G.handCount[j] = handCount - 2;
					memcpy(G.hand[j], cards, sizeof(int) * (handCount-2));
				}
				else {
					G.handCount[j] = handCount;
					memcpy(G.hand[j], cards, sizeof(int) * handCount);
				}
			}

			printInfo(choice1, choice2, minCard, minPos);

			//playMinion(&G, minPos, 0, choice1, choice2);

			expNumActions = 1;

			if((run == 1) && (choiceComb == 1)) {
				p0ExpHandCount = handCount - 1;
				p1ExpHandCount = handCount;
				p2ExpHandCount = handCount-2;
				p3ExpHandCount = handCount;
				p4ExpHandCount = handCount;
				expPlayedCount = 1;
				expCoins = startCoins + 2;

				assertTrue(G.numActions, expNumActions, G.handCount[0], p0ExpHandCount, G.handCount[1], p1ExpHandCount,
						G.handCount[2], p2ExpHandCount, G.handCount[3], p3ExpHandCount, G.handCount[4], p4ExpHandCount,
						G.playedCardCount, expPlayedCount, G.coins, expCoins);

			}
		}
	}

	return 0;
}

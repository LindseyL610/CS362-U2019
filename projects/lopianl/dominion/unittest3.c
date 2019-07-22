#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

void printInfo(int choice1, int choice2, int handPos, int ambCard, int handCount) {
	printf("**************************************************\n");
	printf("Testing with the following values:\n");
	printf("choice1 == %d\n", choice1);
	printf("choice2 == %d\n", choice2);
	if((choice2 < 0) || (choice2 > 2)) {
		printf("choice2 is too high or too low\n");
	}
	printf("handPos == %d\n", handPos);
	if(ambCard == 1) {
		printf("handPos contains an ambassador card\n");
	}
	else {
		printf("handPos does not contain an ambassador card\n");
	}

	if(choice1 == handPos) {
		printf("choice1 cannot be equal to handPos\n");
	}
	printf("number of cards in hand == %d\n", handCount);
}

void asserttrue(int p0ActCount, int p0ExpHandCount, int p1ActCount, int p1ExpHandCount, int actSupplyCount, int expSupplyCount) {
	printf("\nAfter calling runAmbassador():\n");
	printf("G.handCount[0] == %d, expected == %d\n", p0ActCount, p0ExpHandCount);
	printf("G.handCount[1] == %d, expected == %d\n", p1ActCount, p1ExpHandCount);
	printf("G.supplyCount[estate] == %d, expected == %d\n", actSupplyCount, expSupplyCount);
    printf("**************************************************\n\n");
}

int main() {
	int seed = 1000;
	int startCoins = 8;
	int numPlayers = 2;
	int r;
	int handCount;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int cards[MAX_HAND];
	struct gameState G;
	int handPos, choice2, ambCard;
	int supplyEstate = 2;
	int choice1 = 0;
	int p0ExpHandCount, p1ExpHandCount, expSupplyCount;

	printf("Testing playAmbassador():\n");

	for(int run = 1; run <= 5; run++) {
		handPos = 0;
		ambCard = 0;

		memset(&G, 23, sizeof(struct gameState));
		r = initializeGame(numPlayers, k, seed, &G);

		G.supplyCount[estate] = supplyEstate;

		for(int i = 0; i < MAX_HAND; i++) {
			if((run > 1) && (i == 1)) {
				cards[i] = ambassador;
				handPos = 1;
				ambCard = 1;
			}
			else {
				cards[i] = estate;
			}
		}

		if(run == 1) {
			handPos = 1;
		}
		else if(run == 5) {
			handPos = 0;
		}

		if(run == 3) {
			handCount = 2;
		}
		else {
			handCount = 5;
		}

		G.handCount[0] = handCount;
		memcpy(G.hand[0], cards, sizeof(int) * handCount);

		G.handCount[1] = handCount;
		memcpy(G.hand[1], cards, sizeof(int) * handCount);

		if(run == 1) {
			choice2 = 1;

			p0ExpHandCount = handCount;
			p1ExpHandCount = handCount;
			expSupplyCount = supplyEstate;
		}
		else if(run == 2) {
			choice2 = -1;

			p0ExpHandCount = handCount;
			p1ExpHandCount = handCount;
			expSupplyCount = supplyEstate;
		}
		else if(run == 3) {
			choice2 = 2;

			p0ExpHandCount = handCount;
			p1ExpHandCount = handCount;
			expSupplyCount = supplyEstate;
		}
		else if(run == 4) {
			choice2 = 2;

			p0ExpHandCount = handCount - choice2 - 1;
			p1ExpHandCount = handCount + 1;
			expSupplyCount = supplyEstate + choice2 - 1;
		}
		else {
			choice2 = 1;

			p0ExpHandCount = handCount;
			p1ExpHandCount = handCount;
			expSupplyCount = supplyEstate;
		}

		printInfo(choice1, choice2, handPos, ambCard, handCount);

		playAmbassador(choice1, choice2, handPos, &G, 0);

		asserttrue(G.handCount[0], p0ExpHandCount, G.handCount[1], p1ExpHandCount, G.supplyCount[estate], expSupplyCount);
	}

	return 0;
}

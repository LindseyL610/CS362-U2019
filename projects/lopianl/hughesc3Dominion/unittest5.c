#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int mineEffect(struct gameState *state, int choice1, int choice2, int currentPlayer, int handPos);

void printInfo(int choice1, int choice2, int handPos) {
	printf("**************************************************\n");
	printf("Testing with the following values:\n");
	printf("choice1 == %d\n", choice1);
	printf("choice2 == %d\n", choice2);
	printf("handPos == %d\n", handPos);
}

void asserttrue(int actPlayedCount, int expPlayedCount, int actHandCount, int expHandCount,
				int actSupplyCount, int expSupplyCount) {
	printf("\nAfter calling mineEffect():\n");
	printf("G.playedCardCount == %d, expected %d\n", actPlayedCount, expPlayedCount);
	printf("G.handCount[player] == %d, expected %d\n", actHandCount, expHandCount);
	printf("G.supplyCount[choice2] == %d, expected %d\n", actSupplyCount, expSupplyCount);
}

int main() {
	int choice1 = 0;
	int choice2;
	int handPos = 1;
	int seed = 1000;
	int startCoins = 8;
	int numPlayers = 2;
	int r;
	int handCount = 5;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, copper, silver, gold};
	int cards[MAX_HAND];
	struct gameState G;
	int supplyCopper = 3;
	int supplySilver = 3;
	int supplyGold = 3;
	int supplyEstate = 3;

	int expPlayedCount, expHandCount, expSupplyCount;

	printf("TESTING mineEffect():\n\n");

	for(int run = 1; run <= 9; run++) {
		memset(&G, 23, sizeof(struct gameState));
		r = initializeGame(numPlayers, k, seed, &G);

		G.supplyCount[copper] = supplyCopper;
		G.supplyCount[silver] = supplySilver;
		G.supplyCount[gold] = supplyGold;
		G.supplyCount[estate] = supplyEstate;

		for(int i = 0; i < MAX_HAND; i++) {
			if(i == 0) {
				switch(run) {
					case 1:
					cards[i] = estate;
					choice2 = silver;
					expPlayedCount = 0;
					expHandCount = handCount;
					expSupplyCount = supplySilver;
					break;

					case 2:
					cards[i] = copper;
					choice2 = estate;
					expPlayedCount = 0;
					expHandCount = handCount;
					expSupplyCount = supplyEstate;
					break;

					case 3:
					cards[i] = copper;
					choice2 = gold;
					expPlayedCount = 0;
					expHandCount = handCount;
					expSupplyCount = supplyGold;
					break;

					case 4:
					cards[i] = copper;
					choice2 = silver;
					expPlayedCount = 0;
					expHandCount = handCount;
					expSupplyCount = supplySilver;
					break;

					case 5:
					cards[i] = silver;
					choice2 = gold;
					expPlayedCount = 1;
					expHandCount = handCount - 1;
					expSupplyCount = supplyGold - 1;
					break;

					case 6:
					cards[i] = silver;
					choice2 = silver;
					expPlayedCount = 1;
					expHandCount = handCount - 1;
					expSupplyCount = supplySilver - 1;
					break;

					case 7:
					cards[i] = gold;
					choice2 = copper;
					expPlayedCount = 1;
					expHandCount = handCount - 1;
					expSupplyCount = supplyCopper - 1;
					break;

					case 8:
					cards[i] = estate;
					choice2 = silver;
					expPlayedCount = 1;
					expHandCount = handCount - 1;
					expSupplyCount = supplySilver - 1;
					break;

					case 9:
					cards[i] = copper;
					choice2 = silver;
					G.supplyCount[silver] = 0;
					expPlayedCount = 0;
					expHandCount = handCount;
					expSupplyCount = 0;
					break;
				}
			}
			else if((i == 1) && (run != 4)) {
				cards[i] = mine;
			}
			else if((i == 4) && (run == 8)) {
				cards[i] = copper;
				choice1 = i;
			}
			else {
				cards[i] = estate;
			}
		}

		G.handCount[0] = handCount;
		memcpy(G.hand[0], cards, sizeof(int) * handCount);

		printInfo(choice1, choice2, handPos);

		mineEffect(&G, choice1, choice2, 0, handPos);

		asserttrue(G.playedCardCount, expPlayedCount, G.handCount[0], expHandCount, G.supplyCount[choice2], expSupplyCount);

		if((run >= 5) && (run <= 8)) {
			if(G.hand[0][3] == choice2) {
				printf("The correct card was added to the player's hand.\n");
			}
			else {
				printf("The correct card was not added to the player's hand.\n");
			}
		}

		printf("**************************************************\n\n");
	}
}

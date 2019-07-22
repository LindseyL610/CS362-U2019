#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <signal.h>
#include <stdlib.h>

void printMessage(int actual, int expected) {
	if(actual != expected) {
		printf("This value is incorrect\n");
	}
	else {
		printf("This value is correct\n");
	}
}

void asserttrue(int actNumPlayers, int correctNumPlayers, int actEstate, int actDuchy,
				int actProvince, int correctNumVictCards, int actCurse, int correctNumCurseCards,
				int correctEmbargoTokens, int actCopper, int actSilver, int actGold,
				int handCount) {
	int correctCopper = 60 - (7 * correctNumPlayers);
	int correctSilver = 40;
	int correctGold = 30;
	
	printf("\nAfter calling initializeGame():\n");
	printf("G.numPlayers == %d, expected == %d -> ", actNumPlayers, correctNumPlayers);
	printMessage(actNumPlayers, correctNumPlayers);
	printf("G.supplyCount[estate] == %d, expected == %d -> ", actEstate, correctNumVictCards);
	printMessage(actEstate, correctNumVictCards);
	printf("G.supplyCount[duchy] == %d, expected == %d -> ", actDuchy, correctNumVictCards);
	printMessage(actDuchy, correctNumVictCards);
	printf("G.supplyCount[province] == %d, expected == %d -> ", actProvince, correctNumVictCards);
	printMessage(actProvince, correctNumVictCards);
	printf("G.supplyCount[curse] == %d, expected == %d -> ", actCurse, correctNumCurseCards);
	printMessage(actCurse, correctNumCurseCards);
	if(correctEmbargoTokens == 1) {
		printf("Number of embargo tokens is correct\n");
	}
	else {
		printf("Number of embargo tokens is incorrect\n");
	}
	printf("G.supplyCount[copper] == %d, expected == %d -> ", actCopper, correctCopper);
	printMessage(actCopper, correctCopper);
	printf("G.supplyCount[silver] == %d, expected == %d -> ", actSilver, correctSilver);
	printMessage(actSilver, correctSilver);
	printf("G.supplyCount[gold] == %d, expected == %d -> ", actGold, correctGold);
	printMessage(actGold, correctGold);
	printf("G.handCount[0] == %d, expected == 5 -> ", handCount);
	printMessage(handCount, 5);
	printf("*****************************************************\n");
}

int main() {
	int seed = 1000;
	int r;
	int choice1, playerEstates, supplyEstate = 3;
	int expHandCount, expDiscCount, expSupplyCount, expCoins;
	int handCount = 5;
	int k[10] = {embargo, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int cards[MAX_HAND];
	struct gameState G;
	int correctNumPlayers, correctNumVictCards, correctNumCurseCards, correctKingdomCards, correctEmbargoTokens, equalHands;
	int basicSupply[7] = {curse, copper, silver, gold, estate, duchy, province};

	printf("TESTING initializeGame():\n");

	for(int i = 1; i <= 5; i++) {
		printf("\n*****************************************************\n");
		printf("Attemping to initialize game with %d player(s)\n", i);

		memset(&G, 23, sizeof(struct gameState));
		r = initializeGame(i, k, seed, &G);

		if(r == -1) {
			printf("Initialization was unsuccessful\n");

			if(i < 2) {
				printf("%d is less than 2, the minimum number of players\n", i);
				printf("Initialization SHOULD NOT have succeeded\n");
			}
			else if(i > MAX_PLAYERS) {
				printf("%d is greater than %d, the maximum number of players\n", i, MAX_PLAYERS);
				printf("Initialization SHOULD NOT have succeeded\n");
			}
			else {
				printf("%d is an acceptable number of players\n", i);
				printf("Initialization SHOULD have been successful\n");
			}
			printf("*****************************************************\n");
		}
		else {
			correctNumPlayers = i;

			if(i == 2) {
				correctNumVictCards = 8;
			}
			else {
				correctNumVictCards = 12;
			}

			correctNumCurseCards = 10 * i;

			correctKingdomCards = 1;
			correctEmbargoTokens = 1;

			for(int j = 0; j < 10; j++) {
				if(G.supplyCount[k[j]] != 1) {
					correctKingdomCards = 0;
				}

				if(G.embargoTokens[k[j]] != 0) {
					correctEmbargoTokens = 0;
				}
			}

			for(int j = 0; j < 7; j++) {
				if(G.embargoTokens[basicSupply[j]] != 0) {
					correctEmbargoTokens = 0;
				}
			}

			asserttrue(G.numPlayers, i, G.supplyCount[estate], G.supplyCount[duchy],
						G.supplyCount[province], correctNumVictCards, G.supplyCount[curse],
						correctNumCurseCards, correctEmbargoTokens, G.supplyCount[copper],
						G.supplyCount[silver], G.supplyCount[gold], G.handCount[0]);
		}
	}

	int kc[10] = {embargo, embargo, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};

	printf("\n*****************************************************\n");
	printf("Attemping to initialize game with multiple copies of the same kingdom card\n");

	memset(&G, 23, sizeof(struct gameState));
	r = initializeGame(2, kc, seed, &G);

	if(r != -1) {
		printf("Initialization was successful\n");
		printf("Initialization SHOULD NOT have succeeded\n");
	}
	else {
		printf("Initialization was not successful\n");
		printf("Initialization SHOULD NOT have succeeded\n");
	}
	printf("*****************************************************\n");

	return 0;
}

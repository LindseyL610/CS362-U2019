#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <signal.h>
#include <stdlib.h>

void exitfunc(int sig) {
	printf("\nTimed out after 120 seconds.\n");
	printf("Check for infinite loop.\n");
        printf("**************************************************\n\n");
	exit(0);
}

void printInfo(int choice1, int handCount, int playerEstate, int supplyEstate) {
	printf("**************************************************\n");
	printf("Testing with the following values:\n");
	printf("Choice1 == %d\n", choice1);
	printf("G.handCount[player] == %d\n", handCount);	
	printf("Estate cards in player's hand:  %d\n", playerEstate);
	printf("G.supplyCount[estate] == %d\n", supplyEstate);
}

void asserttrue(int actHandCount, int expHandCount, int actDiscCount, int expDiscCount, int actSupplyCount, int expSupplyCount, int actCoins, int expCoins) {
	printf("\nAfter calling runBaron():\n");
	printf("G.handCount[player] == %d, expected ==  %d\n", actHandCount, expHandCount);
	printf("G.discardCount[player] == %d, expected == %d\n", actDiscCount, expDiscCount);
	printf("G.supplyCount[estate] == %d, expected == %d\n", actSupplyCount, expSupplyCount);
	printf("G.coins == %d, expected == %d\n", actCoins, expCoins);
        printf("**************************************************\n\n");
}

int main() {
	signal(SIGALRM, exitfunc);

	int seed = 1000;
	int startCoins = 8;
	int numPlayers = 2;
	int r;
	int choice1, playerEstates, supplyEstate = 3;
	int expHandCount, expDiscCount, expSupplyCount, expCoins;
	int handCount = 5;
	int k[10] = {estate, adventurer, council_room, feast, gardens,
				 mine, remodel, smithy, village, baron};
	int cards[MAX_HAND];
	struct gameState G;

	for(int i = 0; i < MAX_HAND; i++) {
		cards[i] = estate;
	}

	playerEstates = handCount;

	printf("TESTING playBaron():\n\n");

	for(int run = 1; run <= 4; run++) {
		alarm(120);

		memset(&G, 23, sizeof(struct gameState));
		r = initializeGame(numPlayers, k, seed, &G);
		G.handCount[0] = handCount;
		memcpy(G.hand[0], cards, sizeof(int) * handCount);
		G.supplyCount[estate] = supplyEstate;
		G.coins = startCoins;
		G.discardCount[0] = 0;	
	
		if(run == 1) {
			choice1 = 0;
			printInfo(choice1, handCount, playerEstates, supplyEstate);
			
			playBaron(&G, choice1, 0);
		
			expHandCount = handCount + 1;
			expDiscCount = 0;
			expSupplyCount = supplyEstate - 1;
			expCoins = startCoins;
			asserttrue(G.handCount[0], expHandCount, G.discardCount[0], expDiscCount, G.supplyCount[estate], expSupplyCount, G.coins, expCoins);
		}
		else if(run == 2) {	
			choice1 = -1;
			printInfo(choice1, handCount, playerEstates, supplyEstate);
			
			playBaron(&G, choice1, 0);

			expHandCount = handCount + 1;
			expDiscCount = 0;
			expSupplyCount = supplyEstate - 1;
			expCoins = startCoins;
			asserttrue(G.handCount[0], expHandCount, G.discardCount[0], expDiscCount, G.supplyCount[estate], expSupplyCount, G.coins, expCoins);
		}
		else if(run == 3) {
			choice1 = 1;
			printInfo(choice1, handCount, playerEstates, supplyEstate);

			playBaron(&G, choice1, 0);

			expHandCount = handCount - 1;
			expDiscCount = 1;
			expSupplyCount = supplyEstate + 1;
			expCoins = startCoins + 4;
			asserttrue(G.handCount[0], expHandCount, G.discardCount[0], expDiscCount, G.supplyCount[estate], expSupplyCount, G.coins, expCoins);

			handCount = 1;
			for(int j = 0; j < MAX_HAND; j++) {
				cards[j] = duchy;
			}
			playerEstates = 0;
		}
		else if(run == 4) {
			choice1 = 1;
			printInfo(choice1, handCount, playerEstates, supplyEstate);

			playBaron(&G, choice1, 0);
		}
	}

	return 0;
}

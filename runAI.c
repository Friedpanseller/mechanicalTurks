// Created by Leo Liu
// 19 May 2015

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "Game.h"
#include "mechanicalTurk.h"

// Game aspects
#define UNI_CHAR_NAME ('A' - UNI_A)
#define WINNING_KPI 150
#define DICE_AMOUNT 2

// runGame defaults
#define INVALID -1
#define DICE_FACES 6

#define NUM_DISCIPLINES 6

// Action:
#define ACTION_NAMES \
   { "Pass", "Build Campus", "Build GO8", "Obtain ARC", \
     "Start Spinoff", "", "", "Retrain Student" }

#define DISCIPLE_NAMES \
   { "ThD", "BPS", "BQN", "MJobs", "MTV", "M$" }

#define SCREEN_WIDTH 50
#define LINE_BREAK_SEPARATOR '-'

   
#define CYAN STUDENT_BQN
#define PURP STUDENT_MMONEY
#define YELL STUDENT_MJ
#define RED STUDENT_BPS
#define GREE STUDENT_MTV 
#define BLUE STUDENT_THD

#define MAX_PASS 9000

typedef struct _testAI {
	int rounds;
	int turns;
	int aWin;
	int bWin;
	int cWin;
	int highestWin;
	int lowestWin;
	int averageWin;
	int maxGO8;
	int averageGO8;
	int maxCampus;
	int averageCampus;
	int maxArc;
	int averageArc;
	int maxPublication;
	int averagePublication;
	int maxIP;
	int averageIP;
	int maxTHD;
	int averageTHD;
	int maxBPS;
	int averageBPS;
	int maxBQN;
	int averageBQN;
	int maxMJ;
	int averageMJ;
	int maxMTV;
	int averageMTV;
	int maxMMONEY;
	int averageMMONEY;
} testAI;

void randomDisciplines(int disciplines[]);
void randomDice(int dice[]);
void rigBoard(int disciplines[], int dice[]);
int rollDice(void);
int checkForWinner(Game g);

int main(int argc, char *argv[]) {
   // miscellaneous
   /*int disciplines[NUM_REGIONS];
   int dice[NUM_REGIONS];*/
   Game g;
	testAI test;
   test.rounds = 0;
	test.turns = 0;
	test.aWin = 0;
	test.bWin = 0;
	test.cWin = 0;
	test.highestWin = 0;
	test.lowestWin = 8999;
	test.averageWin = 0;
	test.maxGO8 = 0;
	test.averageGO8 = 0;
	test.maxCampus = 0;
	test.averageCampus = 0;
	test.maxArc = 0;
	test.averageArc = 0;
	test.maxPublication = 0;
	test.averagePublication = 0;
	test.maxIP = 0;
	test.averageIP = 0;
	test.maxTHD = 0;
	test.averageTHD = 0;
	test.maxBPS = 0;
	test.averageBPS = 0;
	test.maxBQN = 0;
	test.averageBQN = 0;
	test.maxMJ = 0;
	test.averageMJ = 0;
	test.maxMTV = 0;
	test.averageMTV = 0;
	test.maxMMONEY = 0;
	test.averageMMONEY = 0;
	
   // store the winner of each game
   int winner;
   
   // store game states within the game
   
   int turnFinished;
   int diceRollAmount;
   
   // random
   //char *actions[] = ACTION_NAMES;
   int diceRoll;
   
   int passedTurns = 0;
	int roundsToPlay = atoi(argv[1]);
            
	int count = 0;			
   // seed rand!
   srand(time(NULL));
   
   // while the game is wanting to be played, create new game, etc.
   while (test.rounds < roundsToPlay) {
		
		printf("Round %d...\n", test.rounds);
      // create the game
      //randomDisciplines(disciplines);
      //randomDice(dice);
      
      // you can change this to randomiseDisciplines() and randomiseDice() later
      int disciplines[NUM_REGIONS] = {CYAN,PURP,YELL,PURP,YELL,RED ,GREE,GREE, RED ,GREE,CYAN,YELL,CYAN,BLUE,YELL,PURP,GREE,CYAN,RED };
      int dice[NUM_REGIONS] = {9,10,8,12,6,5,3,7,3,11,4,6,4,9,9,2,8,10,5};

      // rig board like the real game
      rigBoard(disciplines, dice);
      g = newGame(disciplines, dice);
		test.turns = 0;
		
      
      //printf("Game created! Now playing...\n");
     
      // start the game with noone as the winner
      winner = NO_ONE;
      while (winner == NO_ONE) {
			test.turns++;
         //printLineBreak();
         // start new turn by setting turnFinished to false then
         // rolling the dice
         
         diceRollAmount = 0;
         diceRoll = 0;
         while (diceRollAmount < DICE_AMOUNT) {
            diceRoll += rollDice();
            diceRollAmount++;
         }
         
         throwDice(g, diceRoll);
         
         // new turn means new line break!
         /*printf("[Turn %d] The turn now belongs to University %c!\n", 
            getTurnNumber(g),
            getWhoseTurn(g) + UNI_CHAR_NAME);
         printf("The dice has casted a %d!\n", diceRoll);
         
         printf("\n");*/
         
         
         // keep going through the player's turn until
         // he/she decided to pass and finish the turn
         turnFinished = FALSE;
         while (turnFinished == FALSE && passedTurns < MAX_PASS) {
            // processes requests and all subrequests for a move and
            // checks if they are legal. only gives a move within the
            // scope of the defined actionCodes that is legal
            int turnPerson = getWhoseTurn(g);
				/*printf("turnPerson %d\n", getWhoseTurn(g));
            printf("Stats for %c:\n", turnPerson + UNI_CHAR_NAME);
            printf("KPIs: %d\n", getKPIpoints(g, turnPerson));
            printf("ARCs: %d\n", getARCs(g, turnPerson));
            printf("Campuses: %d\n", getCampuses(g, turnPerson));
            printf("GO8s: %d\n", getGO8s(g, turnPerson));
            printf("Publications: %d\n", 
               getPublications(g, turnPerson));
            printf("Patents: %d\n", 
               getIPs(g, turnPerson));
					*/
            /*int discipleIndex = 0;
            char *discipleNames[] = DISCIPLE_NAMES;
            while (discipleIndex < NUM_DISCIPLINES) {
               printf("Type %s: %d\n", discipleNames[discipleIndex],
                  getStudents(g, turnPerson, discipleIndex));
               discipleIndex++;
            } 
            printf("\n");*/
            
            action a = decideAction(g);
            
            // if not passing, make the move; otherwise end the turn
            if (a.actionCode == PASS) {
               turnFinished = TRUE;
               //printf("You have passed onto the next person.\n");
            } else {
              
               // write what the player did, for a logs sake.
               /*printf("The action '%s' has being completed.\n", 
                       actions[a.actionCode]);*/
               if (a.actionCode == BUILD_CAMPUS 
                   || a.actionCode == OBTAIN_ARC 
                   || a.actionCode == BUILD_GO8) {
                  //printf(" -> Destination: %s\n", a.destination);
               } else if (a.actionCode == RETRAIN_STUDENTS) {
                  //printf(" -> DisciplineTo: %d\n", a.disciplineTo);
                  //printf(" -> DisciplineFrom: %d\n", a.disciplineFrom);
               }

               assert(isLegalAction(g, a));                                   
            
               // break this and the code dies. trololol!
               if (a.actionCode == START_SPINOFF) {
						//printf("Starting Spinoff\n");
                  if (rand() % 3 <= 1) {
                     a.actionCode = OBTAIN_PUBLICATION;
                  } else {
                     a.actionCode = OBTAIN_IP_PATENT;               
                  }
               }
					
               makeAction(g, a);
               
               if (a.actionCode == PASS) {
                  passedTurns++;
               } else {
                  passedTurns = 0;
               }
               
               if (passedTurns >= MAX_PASS || getKPIpoints(g, turnPerson) >= WINNING_KPI) {
                  turnFinished = TRUE;
                  
               }
            }
            
            // if there is not a winner or pass, add a seperating line
            // to seperate actions being clumped together
            if (turnFinished == FALSE) {
               //printf("\n");
            }
         }
         
         // check if there is a winner
			//printf("checking for winner\n");
         winner = checkForWinner(g);
			if(winner == 1) {
				test.aWin++;
			} else if(winner == 2) {
				test.bWin++;
			} else if(winner == 3) {
				test.cWin++;
			}
			//printf("done checking for winner\n");
      }
      
		//printf("checking for highest wins\n");
		if(test.highestWin < test.turns) {
			test.highestWin = test.turns;
		} else if(test.lowestWin > test.turns) {
			test.lowestWin = test.turns;
		} 
		//printf("Test.Turns = %d", test.turns);
		test.averageWin = test.averageWin + test.turns;
		//printf("Test.averageWin = %d\n", test.averageWin);
		count = 1;
		while(count <= NUM_UNIS) {
			if(getGO8s(g, count) > test.maxGO8) {
				test.maxGO8 = getGO8s(g, count);
			}
			if(getCampuses(g, count) > test.maxCampus) {
				test.maxCampus = getCampuses(g, count);
			}
			if(getARCs(g, count) > test.maxArc) {
				test.maxArc = getARCs(g, count);
			}
			if(getPublications(g, count) > test.maxPublication) {
				test.maxPublication = getPublications(g, count);
			}
			if(getIPs(g, count) > test.maxIP) {
				test.maxIP = getIPs(g, count);
			}
			if(getStudents(g, count, STUDENT_THD) > test.maxTHD) {
				test.maxTHD = getStudents(g, count, STUDENT_THD);
			}
			if(getStudents(g, count, STUDENT_BPS) > test.maxBPS) {
				test.maxBPS = getStudents(g, count, STUDENT_BPS);
			}
			if(getStudents(g, count, STUDENT_BQN) > test.maxBQN) {
				test.maxBQN = getStudents(g, count, STUDENT_BQN);
			}
			if(getStudents(g, count, STUDENT_MJ) > test.maxMJ) {
				test.maxMJ = getStudents(g, count, STUDENT_MJ);
			}
			if(getStudents(g, count, STUDENT_MTV) > test.maxMTV) {
				test.maxMTV = getStudents(g, count, STUDENT_MTV);
			}
			if(getStudents(g, count, STUDENT_MMONEY) > test.maxMMONEY) {
				test.maxMMONEY = getStudents(g, count, STUDENT_MMONEY);
			}
			//printf("Get Arc = %d ",getARCs(g, count));
			test.averageGO8 = test.averageGO8 + getGO8s(g, count);
			test.averageCampus = test.averageCampus + getCampuses(g, count);
			test.averageArc = test.averageArc + getARCs(g, count);
			//printf("Test Arc = %d \n",test.averageArc);
			test.averagePublication = test.averagePublication + getPublications(g, count);
			test.averageIP = test.averageIP + getIPs(g, count);
			test.maxTHD = test.maxTHD + getStudents(g, count, STUDENT_THD);
			test.maxBPS = test.maxBPS + getStudents(g, count, STUDENT_BPS);
			test.maxBQN = test.maxBQN + getStudents(g, count, STUDENT_BQN);
			test.maxMJ = test.maxMJ + getStudents(g, count, STUDENT_MJ);
			test.maxMTV = test.maxMTV + getStudents(g, count, STUDENT_MTV);
			test.maxMMONEY = test.maxMMONEY + getStudents(g, count, STUDENT_MMONEY);
			count++;
		}
	
      if (passedTurns >= MAX_PASS) {
         printf("AI passes too much.\n");
         return EXIT_FAILURE;
      }
      
      //printLineBreak();
      //printf("GAME OVER!\n");
      //printf("Vice Chanceller %c Won in %d Turns!!\n", 
      //        winner + UNI_CHAR_NAME,
      //      getTurnNumber(g));
             
      /*printf("\n");
      int counter = UNI_A;
      while (counter < NUM_UNIS + UNI_A) {
         printf("Uni %c scored %d KPIs\n", counter + UNI_CHAR_NAME,
                getKPIpoints(g, counter));
         counter++;
      }*/
      //printLineBreak();
      disposeGame(g);
		test.rounds++;
   }     
   printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nOnly trust stats marked with a #\n");
	printf("#Num Rounds Tested: %d\n", test.rounds);
	printf("#A Win: %d\n", test.aWin);
	printf("#B Win: %d\n", test.bWin);
	printf("#C Win: %d\n", test.cWin);
	printf("#Highest Win: %d\n", test.highestWin);
	printf("#Lowest Win: %d\n", test.lowestWin);
	printf("#Average Win: %d\n", (test.averageWin / test.rounds));
	printf("Max GO8s: %d\n", test.maxGO8);
	printf("Average GO8s: %d\n", (test.averageGO8 / (test.rounds * NUM_UNIS)));
	printf("Max Campuses: %d\n", test.maxCampus);
	printf("Average Campuses: %d\n", (test.averageCampus / (test.rounds * NUM_UNIS)));
	printf("Max Arcs: %d\n", test.maxArc);
	printf("Average Arcs: %d\n", (test.averageArc / (test.rounds * NUM_UNIS)));
	printf("Max Publications: %d\n", test.maxPublication);
	printf("Average Publications: %d\n", (test.averagePublication / (test.rounds * NUM_UNIS)));
	printf("Max IPs: %d\n", test.maxIP);
	printf("Average IPs: %d\n", (test.averageIP / test.rounds));
	printf("Max THDs: %d\n", test.maxTHD);
	printf("Average THDs: %d\n", (test.averageTHD / test.rounds));
	printf("Max BPSs: %d\n", test.maxBPS);
	printf("Average BPSs: %d\n", (test.averageBPS / test.rounds));
	printf("Max BQNs: %d\n", test.maxBQN);
	printf("Average BQNs: %d\n", (test.averageBQN / test.rounds));
	printf("Max MJs: %d\n", test.maxMJ);
	printf("Average MJs: %d\n", (test.averageMJ / test.rounds));
	printf("Max MTVs: %d\n", test.maxMTV);
	printf("Average MTVs: %d\n",  (test.averageMTV / test.rounds));
	printf("Max MMONEYs: %d\n", test.maxMMONEY);
	printf("Average MMONEYs: %d\n",(test.averageMMONEY / test.rounds));
	
   return EXIT_SUCCESS;
}

// ----- game creation -----

void rigBoard(int disciplines[], int dice[]) {
  disciplines[0] = disciplines[2] = disciplines[7] = STUDENT_BPS;
  disciplines[11] = disciplines[16] = disciplines[18] = STUDENT_BQN;
}

// Allocates a set of random disciplines inside disciplines[]
void randomDisciplines(int disciplines[]) {
   int disciplineIndex;
   
   disciplineIndex = 0;
   while (disciplineIndex < NUM_REGIONS) {
      // allocate each discipline with a random one
      disciplines[disciplineIndex] = rand() % NUM_DISCIPLINES;
      disciplineIndex++;
   }
}

// Allocates a set of random dice inside disciplines[]
void randomDice(int dice[]) {
   int diceIndex;
   int diceRolled;
   int totalRoll;
   
   diceIndex = 0;
   while (diceIndex < NUM_REGIONS) {
      totalRoll = 0;

      // roll a dice DICE_AMOUNT and add the total
      diceRolled = 0;
      while (diceRolled < DICE_AMOUNT) {
         totalRoll += rollDice();
         diceRolled++;
      }
      
      // allocate the totalRoll
      dice[diceIndex] = totalRoll;
      diceIndex++;
   }
}

// return a number between 1...DICE_FACES 
int rollDice(void) {
   // modding returns between 0...(DICE_FACES-1), so add 1
   return (rand() % DICE_FACES) + 1;
}

// ----- game actions -----

// check all players' KPI and returns the winner (if any)
int checkForWinner(Game g) {
   int winner = NO_ONE;
   int playerIndex;
   
   playerIndex = UNI_A;
   while (playerIndex < NUM_UNIS + UNI_A) {
      // check if the player is over or equal the WINNING_KPI
      if (getKPIpoints(g, playerIndex) >= WINNING_KPI) {
         winner = playerIndex;
      }
      playerIndex++;
   }
   
   return winner;
}


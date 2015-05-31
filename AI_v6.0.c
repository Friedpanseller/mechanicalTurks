/*
 *  mechanicalTurk.  Brain the size of a sun!
 *
 *  Proundly Created by tony tang && leo liu
 *  Share Freely Creative Commons SA-BY-NC 3.0.  yeah nah
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include "Game.h"
#include "mechanicalTurk.h"

#define GET_BPS								(getStudents (g, getWhoseTurn(g), STUDENT_BPS))
#define GET_BQN								(getStudents (g, getWhoseTurn(g), STUDENT_BQN))
#define GET_MJ								(getStudents (g, getWhoseTurn(g), STUDENT_MJ))
#define GET_MTV								(getStudents (g, getWhoseTurn(g), STUDENT_MTV))
#define GET_MMONEY							(getStudents (g, getWhoseTurn(g), STUDENT_MMONEY))
#define GET_CAMPUS							(getCampuses (g, getWhoseTurn(g)))
#define GET_GO8								(getGO8s (g, getWhoseTurn(g)))

#define ENOUGH_STUDENTS_FOR_ARC			(GET_BPS != 1 && GET_BQN != 1)
#define ENOUGH_STUDENTS_FOR_CAMPUS		(GET_BPS > 0 && GET_BQN > 0 && GET_MJ > 0 && GET_MTV > 0)
#define ENOUGH_STUDENTS_FOR_GO8			(GET_MJ >= 2 && GET_MMONEY >= 3)
#define ENOUGH_STUDENTS_FOR_SPINOFF		(GET_MJ != 0 && GET_MTV != 0 && GET_MMONEY != 0)

// Following code is to check if 1 off students required for building
// If it has RT_??? at the start - it is a special case for retraining students which are also used for building the buildings

// MMONEY - 3x GO8, 1x Spinoff
#define NEED_MMONEY_FOR_GO8 				((GET_MMONEY == 2) && (GET_MJ >= 2) && (GET_CAMPUS >= 1) && (GET_GO8 <= 7) && (cannotBuildGO8 == FALSE))
#define RT_MJ_MMONEY_FOR_GO8 				((GET_MMONEY == 2) && (GET_MJ >= 5) && (GET_CAMPUS >= 1) && (GET_GO8 <= 7) && (cannotBuildGO8 == FALSE))
#define NEED_MMONEY_FOR_SPINOFF 			((GET_MMONEY == 0) && (GET_MTV >= 1) && (GET_MJ >= 1))
#define RT_MJ_MMONEY_FOR_SPINOFF 		    ((GET_MMONEY == 2) && (GET_MTV >= 1) && (GET_MJ >= 4))
#define RT_MTV_MMONEY_FOR_SPINOFF 		    ((GET_MMONEY == 0) && (GET_MTV >= 4) && (GET_MJ >= 1))

// MJ - 2x GO8, 1x Campus, 1x Spinoff
#define NEED_MJ_FOR_GO8 					((GET_MJ == 1) && (GET_MMONEY >= 3) && (GET_CAMPUS >= 1) && (GET_GO8 <= 7) && (cannotBuildGO8 == FALSE))
#define RT_MMONEY_MJ_FOR_GO8 				((GET_MJ == 1) && (GET_MMONEY >= 6) && (GET_CAMPUS >= 1) && (GET_GO8 <= 7) && (cannotBuildGO8 == FALSE))
#define NEED_MJ_FOR_CAMPUS 				    ((GET_MJ == 0) && (GET_BPS >= 1) && (GET_BQN >= 1) && (GET_MTV >= 1) && (cannotBuildCampus == FALSE))
#define RT_MTV_MJ_FOR_CAMPUS 				((GET_MJ == 0) && (GET_BPS >= 1) && (GET_BQN >= 1) && (GET_MTV >= 4) && (cannotBuildCampus == FALSE))
#define RT_BPS_MJ_FOR_CAMPUS 				((GET_MJ == 0) && (GET_BPS >= 4) && (GET_BQN >= 1) && (GET_MTV >= 1) && (cannotBuildCampus == FALSE))
#define RT_BQN_MJ_FOR_CAMPUS 				((GET_MJ == 0) && (GET_BPS >= 1) && (GET_BQN >= 4) && (GET_MTV >= 1) && (cannotBuildCampus == FALSE))
#define NEED_MJ_FOR_SPINOFF 				((GET_MJ == 0) && (GET_MTV >= 1) && (GET_MMONEY >= 1))
#define RT_MTV_MJ_FOR_SPINOFF 			    ((GET_MJ == 0) && (GET_MTV >= 4) && (GET_MMONEY >= 1))
#define RT_MMONEY_MJ_FOR_SPINOFF 		    ((GET_MJ == 0) && (GET_MTV >= 1) && (GET_MMONEY >= 4))

// BPS - 1x Campus, 1x ARC
#define NEED_BPS_FOR_CAMPUS 				((GET_BPS == 0) && (GET_BQN >= 1) && (GET_MJ >= 1) && (GET_MTV >= 1) && (cannotBuildCampus == FALSE))
#define RT_MTV_BPS_FOR_CAMPUS 			    ((GET_BPS == 0) && (GET_BQN >= 1) && (GET_MJ >= 1) && (GET_MTV >= 4) && (cannotBuildCampus == FALSE))
#define RT_MJ_BPS_FOR_CAMPUS 				((GET_BPS == 0) && (GET_BQN >= 1) && (GET_MJ >= 4) && (GET_MTV >= 1) && (cannotBuildCampus == FALSE))
#define RT_BQN_BPS_FOR_CAMPUS 			    ((GET_BPS == 0) && (GET_BQN >= 4) && (GET_MJ >= 1) && (GET_MTV >= 1) && (cannotBuildCampus == FALSE))
#define NEED_BPS_FOR_ARC 					((GET_BPS == 0) && (GET_BQN >= 1) && (cannotBuildArc == FALSE))
#define RT_BQN_BPS_FOR_ARC 				    ((GET_BPS == 0) && (GET_BQN >= 4) && (cannotBuildArc == FALSE))

// BQN - 1x Campus, 1x ARC
#define NEED_BQN_FOR_CAMPUS 				((GET_BQN == 0) && (GET_BPS >= 1) && (GET_MJ >= 1) && (GET_MTV >= 1) && (cannotBuildCampus == FALSE))
#define RT_MTV_BQN_FOR_CAMPUS 			    ((GET_BQN == 0) && (GET_BPS >= 1) && (GET_MJ >= 1) && (GET_MTV >= 4) && (cannotBuildCampus == FALSE))
#define RT_MJ_BQN_FOR_CAMPUS 				((GET_BQN == 0) && (GET_BPS >= 1) && (GET_MJ >= 4) && (GET_MTV >= 1) && (cannotBuildCampus == FALSE))
#define RT_BPS_BQN_FOR_CAMPUS 			    ((GET_BQN == 0) && (GET_BPS >= 4) && (GET_MJ >= 1) && (GET_MTV >= 1) && (cannotBuildCampus == FALSE))
#define NEED_BQN_FOR_ARC 					((GET_BQN == 0) && (GET_BPS >= 1) && (cannotBuildArc == FALSE))
#define RT_BPS_BQN_FOR_ARC 				    ((GET_BQN == 0) && (GET_BPS >= 4) && (cannotBuildArc == FALSE))

// MTV - 1x Campus, 1x Spinoff
#define NEED_MTV_FOR_CAMPUS 				((GET_MTV == 0) && (GET_BQN >= 1) && (GET_BPS >= 1) && (GET_MJ >= 1) && (cannotBuildCampus == FALSE))
#define RT_MJ_MTV_FOR_CAMPUS 				((GET_MTV == 0) && (GET_BQN >= 1) && (GET_BPS >= 1) && (GET_MJ >= 4) && (cannotBuildCampus == FALSE))
#define RT_BPS_MTV_FOR_CAMPUS 			    ((GET_MTV == 0) && (GET_BQN >= 1) && (GET_BPS >= 4) && (GET_MJ >= 1) && (cannotBuildCampus == FALSE))
#define RT_BQN_MTV_FOR_CAMPUS 			    ((GET_MTV == 0) && (GET_BQN >= 4) && (GET_BPS >= 1) && (GET_MJ >= 1) && (cannotBuildCampus == FALSE))
#define NEED_MTV_FOR_SPINOFF 				((GET_MTV == 0) && (GET_MJ >= 1) && (GET_MMONEY >= 1))
#define RT_MJ_MTV_FOR_SPINOFF 			    ((GET_MTV == 0) && (GET_MJ >= 4) && (GET_MMONEY >= 1))
#define RT_MMONEY_MTV_FOR_SPINOFF 		    ((GET_MTV == 0) && (GET_MJ >= 1) && (GET_MMONEY >= 4))

action testPath(Game g, action nextAction, char * pathingCount);

action decideAction (Game g) {

    action nextAction;
    int myUni = getWhoseTurn(g);
    nextAction.actionCode = PASS;
    char pathing[PATH_LIMIT];
    memset(pathing, 0, sizeof(pathing));
	 int pathingCount = 0;
	 int cannotBuildArc = 0;
	 int cannotBuildCampus = 0;
	 int cannotBuildGO8 = 0;
	 
	 //TEST ARC TEST ARC TEST ARC TEST ARC TEST ARC TEST ARC
   nextAction.actionCode = OBTAIN_ARC;
   memset(nextAction.destination, 0, sizeof(nextAction.destination));
   if(myUni == 1) {
		nextAction = testPath(g, nextAction, pathing);
		if(isLegalAction(g, nextAction) == FALSE) {
			memset(pathing, 0, sizeof(pathing));
			memset(nextAction.destination, 0, sizeof(nextAction.destination));
			pathingCount = 0;
			strcpy(pathing, "RLRLRLRLRLL");
			pathingCount += 11;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				//printf("while1\n");
				if((pathingCount == 11) ||
					(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 18 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 20 && pathing[pathingCount-1] == 'B')) {
					pathing[pathingCount-1] = 'B';
				} else {
					pathing[pathingCount] = 'L';
				}
				strcpy(nextAction.destination, pathing);
				pathingCount++;
				if(isLegalAction(g, nextAction) == FALSE) {
					if((pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 19 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 21 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount] = 'B';
					} else {
						pathing[pathingCount-1] = 'R';
					}
					strcpy(nextAction.destination, pathing);
				}
			}
		}
	} else if(myUni == 2) {
		strcpy(pathing, "RRLRL");
		pathingCount += 5;
		while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
			//printf("while2\n");
			 if((pathingCount == 5) ||
			(pathingCount == 7 && pathing[pathingCount-1] == 'B')) {
				  pathing[pathingCount] = 'B';
			 } else {
				  pathing[pathingCount] = 'L';
			 }
			 strcpy(nextAction.destination, pathing);
			 pathingCount++;
			 if(isLegalAction(g, nextAction) == FALSE) {
				  if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
				(pathingCount == 8 && pathing[pathingCount-1] == 'B') ||
				(pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
				  } else {
						pathing[pathingCount-1] = 'R';
				  }
				  strcpy(nextAction.destination, pathing);
			 }
		}
		if(isLegalAction(g, nextAction) == FALSE) {
			memset(pathing, 0, sizeof(pathing));
			memset(nextAction.destination, 0, sizeof(nextAction.destination));
			pathingCount = 0;
			strcpy(pathing, "LRLRLRRLRL");
			pathingCount += 10;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				//printf("while3\n");
				if((pathingCount == 10) ||
					(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
					pathing[pathingCount-1] = 'B';
				} else {
					pathing[pathingCount] = 'L';
				}
				strcpy(nextAction.destination, pathing);
				pathingCount++;
				if(isLegalAction(g, nextAction) == FALSE) {
					if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount] = 'B';
					} else {
						pathing[pathingCount-1] = 'R';
					}
					strcpy(nextAction.destination, pathing);
				}
			}
		}
   } else if(myUni == 3) {
		strcpy(pathing, "LRLRL");
		pathingCount += 5;
		while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
			//printf("while4\n");
			if((pathingCount == 5) ||
				(pathingCount == 7 && pathing[pathingCount-1] == 'B') ||
				(pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
				  pathing[pathingCount] = 'B';
			 } else {
				  pathing[pathingCount] = 'L';
			 }
			 strcpy(nextAction.destination, pathing);
			 pathingCount++;
			 if(isLegalAction(g, nextAction) == FALSE) {
				  if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
				(pathingCount == 8 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
				  } else {
						pathing[pathingCount-1] = 'R';
				  }
				  strcpy(nextAction.destination, pathing);
			 }
		}
		if(isLegalAction(g, nextAction) == FALSE) {
			memset(pathing, 0, sizeof(pathing));
			memset(nextAction.destination, 0, sizeof(nextAction.destination));
			pathingCount = 0;
			strcpy(pathing, "RLRLRLRRLR");
			pathingCount += 10;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				//printf("while5\n");
				if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
					pathing[pathingCount] = 'B';
				} else {
					pathing[pathingCount] = 'L';
				}
				strcpy(nextAction.destination, pathing);
				pathingCount++;
				if(isLegalAction(g, nextAction) == FALSE) {
					if((pathingCount == 10) ||
						(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
					} else {
						pathing[pathingCount-1] = 'R';
					}
					strcpy(nextAction.destination, pathing);
				}
			}
		}
	}
	if(isLegalAction(g, nextAction) == FALSE) {
		cannotBuildArc = TRUE;
		nextAction.actionCode = PASS;
	} else {
		cannotBuildArc = FALSE;
	}
	 
	//TEST CAMP TEST CAMP TEST CAMP TEST CAMP TEST CAMP TEST CAMP TEST CAMP
	nextAction.actionCode = BUILD_CAMPUS;
	memset(nextAction.destination, 0, sizeof(nextAction.destination));
	memset(pathing, 0, sizeof(pathing));
	if(myUni == 1) {
		nextAction = testPath(g, nextAction, pathing);
		if(isLegalAction(g, nextAction) == FALSE) {
			memset(pathing, 0, sizeof(pathing));
			memset(nextAction.destination, 0, sizeof(nextAction.destination));
			pathingCount = 0;
			strcpy(pathing, "RLRLRLRLRLL");
			pathingCount += 11;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				if((pathingCount == 11) ||
					(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 18 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 20 && pathing[pathingCount-1] == 'B')) {
					pathing[pathingCount-1] = 'B';
				} else {
					pathing[pathingCount] = 'L';
				}
				strcpy(nextAction.destination, pathing);
				pathingCount++;
				if(isLegalAction(g, nextAction) == FALSE) {
					if((pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 19 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 21 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount] = 'B';
					} else {
						pathing[pathingCount-1] = 'R';
					}
					strcpy(nextAction.destination, pathing);
				}
			}
		}
	} else if(myUni == 2) {
		strcpy(pathing, "RRLRL");
		pathingCount += 5;
		while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
			 if((pathingCount == 5) || (pathingCount == 7 && pathing[pathingCount-1] == 'B')) {
				  pathing[pathingCount] = 'B';
			 } else {
				  pathing[pathingCount] = 'L';
			 }
			 strcpy(nextAction.destination, pathing);
			 pathingCount++;
			 if(isLegalAction(g, nextAction) == FALSE) {
				  if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
				(pathingCount == 8 && pathing[pathingCount-1] == 'B') ||
				(pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
				  } else {
						pathing[pathingCount-1] = 'R';
				  }
				  strcpy(nextAction.destination, pathing);
			 }
		}
		if(isLegalAction(g, nextAction) == FALSE) {
			memset(pathing, 0, sizeof(pathing));
			memset(nextAction.destination, 0, sizeof(nextAction.destination));
			pathingCount = 0;
			strcpy(pathing, "LRLRLRRLRL");
			pathingCount += 10;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				if((pathingCount == 10) ||
					(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
					pathing[pathingCount-1] = 'B';
				} else {
					pathing[pathingCount] = 'L';
				}
				strcpy(nextAction.destination, pathing);
				pathingCount++;
				if(isLegalAction(g, nextAction) == FALSE) {
					if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount] = 'B';
					} else {
						pathing[pathingCount-1] = 'R';
					}
					strcpy(nextAction.destination, pathing);
				}
			}
		}
	} else if(myUni == 3) {
		strcpy(pathing, "LRLRL");
		pathingCount += 5;
		while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
			 if((pathingCount == 5) ||
			(pathingCount == 7 && pathing[pathingCount-1] == 'B') ||
			(pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
				  pathing[pathingCount] = 'B';
			 } else {
				  pathing[pathingCount] = 'L';
			 }
			 strcpy(nextAction.destination, pathing);
			 pathingCount++;
			 if(isLegalAction(g, nextAction) == FALSE) {
				  if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
				(pathingCount == 8 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
				  } else {
						pathing[pathingCount-1] = 'R';
				  }
				  strcpy(nextAction.destination, pathing);
			 }
		}
		if(isLegalAction(g, nextAction) == FALSE) {
			memset(pathing, 0, sizeof(pathing));
			memset(nextAction.destination, 0, sizeof(nextAction.destination));
			pathingCount = 0;
			strcpy(pathing, "RLRLRLRRLR");
			pathingCount += 10;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
					pathing[pathingCount] = 'B';
				} else {
					pathing[pathingCount] = 'L';
				}
				strcpy(nextAction.destination, pathing);
				pathingCount++;
				if(isLegalAction(g, nextAction) == FALSE) {
					if((pathingCount == 10) ||
						(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
					} else {
						pathing[pathingCount-1] = 'R';
					}
					strcpy(nextAction.destination, pathing);
				} 
			}
		}
	}
	if(isLegalAction(g, nextAction) == FALSE) {
		cannotBuildCampus = TRUE;
		nextAction.actionCode = PASS;
	} else {
		cannotBuildCampus = FALSE;
	}
	
	//TEST GO8 TEST GO8 TEST GO8 TEST GO8 TEST GO8 TEST GO8 TEST GO8
	nextAction.actionCode = BUILD_GO8;
	memset(nextAction.destination, 0, sizeof(nextAction.destination));
	memset(pathing, 0, sizeof(pathing));
	if(myUni == 1) {
		nextAction = testPath(g, nextAction, pathing);
		if(isLegalAction(g, nextAction) == FALSE) {
			memset(pathing, 0, sizeof(pathing));
			memset(nextAction.destination, 0, sizeof(nextAction.destination));
			pathingCount = 0;
			strcpy(pathing, "RLRLRLRLRLL");
			pathingCount += 11;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				if((pathingCount == 11) ||
					(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 18 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 20 && pathing[pathingCount-1] == 'B')) {
					pathing[pathingCount-1] = 'B';
				} else {
					pathing[pathingCount] = 'L';
				}
				strcpy(nextAction.destination, pathing);
				pathingCount++;
				if(isLegalAction(g, nextAction) == FALSE) {
					if((pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 19 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 21 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount] = 'B';
					} else {
						pathing[pathingCount-1] = 'R';
					}
					strcpy(nextAction.destination, pathing);
				}
			}
		}
	} else if(myUni == 2) {
		strcpy(pathing, "RRLRL");
		pathingCount += 5;
		while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
			if((pathingCount == 5) || (pathingCount == 7 && pathing[pathingCount-1] == 'B')) {
			  pathing[pathingCount] = 'B';
			} else {
			  pathing[pathingCount] = 'L';
			}
			strcpy(nextAction.destination, pathing);
			pathingCount++;
			if(isLegalAction(g, nextAction) == FALSE) {
				if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 8 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
					pathing[pathingCount-1] = 'B';
				} else {
					pathing[pathingCount-1] = 'R';
				}
				strcpy(nextAction.destination, pathing);
			}
		}
		if(isLegalAction(g, nextAction) == FALSE) {
			memset(pathing, 0, sizeof(pathing));
			memset(nextAction.destination, 0, sizeof(nextAction.destination));
			pathingCount = 0;
			strcpy(pathing, "LRLRLRRLRL");
			pathingCount += 10;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				if((pathingCount == 10) ||
					(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
					pathing[pathingCount-1] = 'B';
				} else {
					pathing[pathingCount] = 'L';
				}
				strcpy(nextAction.destination, pathing);
				pathingCount++;
				if(isLegalAction(g, nextAction) == FALSE) {
					if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount] = 'B';
					} else {
						pathing[pathingCount-1] = 'R';
					}
					strcpy(nextAction.destination, pathing);
				}
			}
		}
	} else if(myUni == 3) {
		strcpy(pathing, "LRLRL");
		pathingCount += 5;
		while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
			 if((pathingCount == 5) ||
			(pathingCount == 7 && pathing[pathingCount-1] == 'B') ||
			(pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
				  pathing[pathingCount] = 'B';
			 } else {
				  pathing[pathingCount] = 'L';
			 }
			 strcpy(nextAction.destination, pathing);
			 pathingCount++;
			 if(isLegalAction(g, nextAction) == FALSE) {
				  if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
				(pathingCount == 8 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
				  } else {
						pathing[pathingCount-1] = 'R';
				  }
				  strcpy(nextAction.destination, pathing);
			 }
		}
		if(isLegalAction(g, nextAction) == FALSE) {
			memset(pathing, 0, sizeof(pathing));
			memset(nextAction.destination, 0, sizeof(nextAction.destination));
			pathingCount = 0;
			strcpy(pathing, "RLRLRLRRLR");
			pathingCount += 10;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
					pathing[pathingCount] = 'B';
				} else {
					pathing[pathingCount] = 'L';
				}
				strcpy(nextAction.destination, pathing);
				pathingCount++;
				if(isLegalAction(g, nextAction) == FALSE) {
					if((pathingCount == 10) ||
						(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
					} else {
						pathing[pathingCount-1] = 'R';
					}
					strcpy(nextAction.destination, pathing);
				}
			}
		}
	}
	if(isLegalAction(g, nextAction) == FALSE) {
		cannotBuildGO8 = TRUE;
		nextAction.actionCode = PASS;
	} else {
		cannotBuildGO8 = FALSE;
	}
	 
	// -------------------------
	// ----- SMART RETRAIN -----
    // -------------------------

    // Retrain MTVs - green
    if ((getStudents (g, getWhoseTurn(g), STUDENT_MTV)) >= 3) {
    	// Retrain to MMONEY
		if (NEED_MMONEY_FOR_GO8 || RT_MTV_MMONEY_FOR_SPINOFF) {
			nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_MTV;
			nextAction.disciplineTo = STUDENT_MMONEY;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
        // Retrain to MJ
        else if (NEED_MJ_FOR_GO8 || RT_MTV_MJ_FOR_CAMPUS || RT_MTV_MJ_FOR_SPINOFF) {
            nextAction.actionCode = RETRAIN_STUDENTS;
    		nextAction.disciplineFrom = STUDENT_MTV;
			nextAction.disciplineTo = STUDENT_MJ;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
        }
        // Retrain to BPS
        else if (RT_MTV_BPS_FOR_CAMPUS || NEED_BPS_FOR_ARC) {
            nextAction.actionCode = RETRAIN_STUDENTS;
        	nextAction.disciplineFrom = STUDENT_MTV;
			nextAction.disciplineTo = STUDENT_BPS;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
        }
        // Retrain to BQN
        else if (RT_MTV_BQN_FOR_CAMPUS || NEED_BQN_FOR_ARC) {
            nextAction.actionCode = RETRAIN_STUDENTS;
            nextAction.disciplineFrom = STUDENT_MTV;
			nextAction.disciplineTo = STUDENT_BQN;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
        }
        // Pseudo-random retraining IF the MTV's cannot be used - prioritises MJ & MMONEY, never gets MTV
        else {
            nextAction.actionCode = START_SPINOFF;
            if (isLegalAction (g, nextAction) == 0) {
                nextAction.actionCode = BUILD_CAMPUS;
                if ((isLegalAction (g, nextAction) == 0)) {
                    int randomStudent = ((rand() % 3) + 3); // sets it in the range between 3-5
                    while (randomStudent == 4) {
                        randomStudent = ((rand() % 5) + 1); // sets it in the range between 1-5
                    }
                    nextAction.actionCode = RETRAIN_STUDENTS;
                    nextAction.disciplineFrom = STUDENT_MTV;
                    nextAction.disciplineTo = randomStudent;
                    //printf ("RANDOM Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
                    return nextAction;
                }
            }
        }
    }

	// Retrain BPSs - red
    if ((getStudents (g, getWhoseTurn(g), STUDENT_BPS)) >= 3) {
		// Retrain to MMONEY
		if (NEED_MMONEY_FOR_GO8 || NEED_MMONEY_FOR_SPINOFF) {
			nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_BPS;
			nextAction.disciplineTo = STUDENT_MMONEY;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to MJ
		else if (NEED_MJ_FOR_GO8 || RT_BPS_MJ_FOR_CAMPUS || NEED_MJ_FOR_SPINOFF) {
			nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_BPS;
			nextAction.disciplineTo = STUDENT_MJ;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to BQN
        else if (RT_BPS_BQN_FOR_CAMPUS || RT_BPS_BQN_FOR_ARC) {
			nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_BPS;
			nextAction.disciplineTo = STUDENT_BQN;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to MTV
		else if (RT_BPS_MTV_FOR_CAMPUS || NEED_MTV_FOR_SPINOFF) {
            nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_BPS;
			nextAction.disciplineTo = STUDENT_MTV;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		else {
            nextAction.actionCode = BUILD_CAMPUS;
            if (isLegalAction (g, nextAction) == 0) {
                nextAction.actionCode = OBTAIN_ARC;
                if ((isLegalAction (g, nextAction) == 0)) {
                    int randomStudent = ((rand() % 3) + 3); // sets it in the range between 3-5
                    while (randomStudent == 4 && randomStudent == 1) {
                        randomStudent = ((rand() % 3) + 3); // sets it in the range between 1-5
                    }
                    nextAction.actionCode = RETRAIN_STUDENTS;
                    nextAction.disciplineFrom = STUDENT_BPS;
                    nextAction.disciplineTo = randomStudent;
                    //printf ("RANDOM Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
                    return nextAction;
                }
            }
        }
    }

    // Retrain BQNs - blue
    if ((getStudents (g, getWhoseTurn(g), STUDENT_BQN)) >= 3) {
		// Retrain to MMONEY
		if (NEED_MMONEY_FOR_GO8 || NEED_MMONEY_FOR_SPINOFF) {
			nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_BQN;
			nextAction.disciplineTo = STUDENT_MMONEY;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to MJ
		else if (NEED_MJ_FOR_GO8 || RT_BQN_MJ_FOR_CAMPUS || NEED_MJ_FOR_SPINOFF) {
            nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_BQN;
			nextAction.disciplineTo = STUDENT_MJ;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to BPS
		else if (RT_BQN_BPS_FOR_CAMPUS || RT_BQN_BPS_FOR_ARC) {
            nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_BQN;
			nextAction.disciplineTo = STUDENT_BPS;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to MTV
		else if (RT_BQN_MTV_FOR_CAMPUS || NEED_MTV_FOR_SPINOFF) {
            nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_BQN;
			nextAction.disciplineTo = STUDENT_MTV;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		else {
            nextAction.actionCode = START_SPINOFF;
            if (isLegalAction (g, nextAction) == 0) {
                nextAction.actionCode = BUILD_CAMPUS;
                if ((isLegalAction (g, nextAction) == 0)) {
                    int randomStudent = ((rand() % 3) + 3); // sets it in the range between 3-5
                    while (randomStudent == 4) {
                        randomStudent = ((rand() % 3) + 3); // sets it in the range between 1-5
                    }
                    nextAction.actionCode = RETRAIN_STUDENTS;
                    nextAction.disciplineFrom = STUDENT_BQN;
                    nextAction.disciplineTo = randomStudent;
                    //printf ("RANDOM Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
                    return nextAction;
                }
            }
        }
    }

    // Retrain MJs - yellow
    if ((getStudents (g, getWhoseTurn(g), STUDENT_MJ)) >= 3) {
        // Retrain to MMONEY
		if (RT_MJ_MMONEY_FOR_GO8) { // || RT_MJ_MMONEY_FOR_SPINOFF
			nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_MJ;
			nextAction.disciplineTo = STUDENT_MMONEY;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to BPS
		else if (RT_MJ_BPS_FOR_CAMPUS || NEED_BPS_FOR_ARC) {
            nextAction.actionCode = RETRAIN_STUDENTS;
            nextAction.disciplineFrom = STUDENT_MJ;
			nextAction.disciplineTo = STUDENT_BPS;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to BQN
		else if (RT_MJ_BQN_FOR_CAMPUS || NEED_BQN_FOR_ARC) {
            nextAction.actionCode = RETRAIN_STUDENTS;
            nextAction.disciplineFrom = STUDENT_MJ;
			nextAction.disciplineTo = STUDENT_BQN;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to MTV
		else if (RT_MJ_MTV_FOR_CAMPUS) { // || RT_MJ_MTV_FOR_SPINOFF
            nextAction.actionCode = RETRAIN_STUDENTS;
            nextAction.disciplineFrom = STUDENT_MJ;
			nextAction.disciplineTo = STUDENT_MTV;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		else {
		    /*
			int randomStudent = ((rand() % 3) + 3) // sets it in the range between 3-5
            while (randomStudent == 4 || randomStudent == 3) {
                randomStudent = ((rand() % 5) + 1) // sets it in the range between 1-5
            }
			nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_MJ;
			nextAction.disciplineTo = randomStudent;;
			return nextAction;
			*/
		}
    }

    // Retrain MMONEYs - purple
    if ((getStudents (g, getWhoseTurn(g), STUDENT_MMONEY)) >= 3) {
    	// Retrain to MJ
		if (RT_MMONEY_MJ_FOR_GO8 || NEED_MJ_FOR_CAMPUS) { // || RT_MMONEY_MJ_FOR_SPINOFF
			nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_MMONEY;
			nextAction.disciplineTo = STUDENT_MJ;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to BPS
		else if (NEED_BPS_FOR_CAMPUS || NEED_BPS_FOR_ARC) {
            nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_MMONEY;
			nextAction.disciplineTo = STUDENT_BPS;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to BQN
		else if (NEED_BQN_FOR_CAMPUS || NEED_BQN_FOR_ARC) {
            nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_MMONEY;
			nextAction.disciplineTo = STUDENT_BQN;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		// Retrain to MTV
		else if (NEED_MTV_FOR_CAMPUS) { // || RT_MMONEY_MTV_FOR_SPINOFF
            nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_MMONEY;
			nextAction.disciplineTo = STUDENT_MTV;
			//printf ("Retraining students from %d to %d\n", nextAction.disciplineFrom, nextAction.disciplineTo);
			return nextAction;
		}
		else {
		    /*
			int randomStudent = ((rand() % 3) + 3) // sets it in the range between 3-5
            while (randomStudent == 4 || randomStudent == 5) {
                randomStudent = ((rand() % 5) + 1) // sets it in the range between 1-5
            }
			nextAction.actionCode = RETRAIN_STUDENTS;
			nextAction.disciplineFrom = STUDENT_MMONEY;
			nextAction.disciplineTo = randomStudent;
			return nextAction;
			*/
		}
    }

    //  -------------------
    //  -----BUILDINGS-----
    //  -------------------

    //GO8 GO8 GO8 GO8 GO8 GO8 GO8 GO8 GO8 GO8 GO8 GO8
	if(ENOUGH_STUDENTS_FOR_GO8) {
		nextAction.actionCode = BUILD_GO8;
      memset(nextAction.destination, 0, sizeof(nextAction.destination));
		memset(pathing, 0, sizeof(pathing));
		if(myUni == 1) {
			nextAction = testPath(g, nextAction, pathing);
			if(isLegalAction(g, nextAction) == FALSE) {
				memset(pathing, 0, sizeof(pathing));
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "RLRLRLRLRLL");
				pathingCount += 11;
				while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
					if((pathingCount == 11) ||
						(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 18 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 20 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
					} else {
						pathing[pathingCount] = 'L';
					}
					strcpy(nextAction.destination, pathing);
					pathingCount++;
					if(isLegalAction(g, nextAction) == FALSE) {
						if((pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 19 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 21 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount] = 'B';
						} else {
							pathing[pathingCount-1] = 'R';
						}
						strcpy(nextAction.destination, pathing);
					}
				}
			}
		} else if(myUni == 2) {
			strcpy(pathing, "RRLRL");
			pathingCount += 5;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				if((pathingCount == 5) || (pathingCount == 7 && pathing[pathingCount-1] == 'B')) {
				  pathing[pathingCount] = 'B';
				} else {
				  pathing[pathingCount] = 'L';
				}
				strcpy(nextAction.destination, pathing);
				pathingCount++;
				if(isLegalAction(g, nextAction) == FALSE) {
				   if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 8 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
				   } else {
						pathing[pathingCount-1] = 'R';
				   }
				   strcpy(nextAction.destination, pathing);
				}
			}
			if(isLegalAction(g, nextAction) == FALSE) {
				memset(pathing, 0, sizeof(pathing));
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "LRLRLRRLRL");
				pathingCount += 10;
				while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
					if((pathingCount == 10) ||
						(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
					} else {
						pathing[pathingCount] = 'L';
					}
					strcpy(nextAction.destination, pathing);
					pathingCount++;
					if(isLegalAction(g, nextAction) == FALSE) {
						if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount] = 'B';
						} else {
							pathing[pathingCount-1] = 'R';
						}
						strcpy(nextAction.destination, pathing);
					}
				}
			}
	   } else if(myUni == 3) {
			strcpy(pathing, "LRLRL");
			pathingCount += 5;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				 if((pathingCount == 5) ||
				(pathingCount == 7 && pathing[pathingCount-1] == 'B') ||
				(pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
					  pathing[pathingCount] = 'B';
				 } else {
					  pathing[pathingCount] = 'L';
				 }
				 strcpy(nextAction.destination, pathing);
				 pathingCount++;
				 if(isLegalAction(g, nextAction) == FALSE) {
					  if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 8 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount-1] = 'B';
					  } else {
							pathing[pathingCount-1] = 'R';
					  }
					  strcpy(nextAction.destination, pathing);
				 }
			}
			if(isLegalAction(g, nextAction) == FALSE) {
				memset(pathing, 0, sizeof(pathing));
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "RLRLRLRRLR");
				pathingCount += 10;
				while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
					if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount] = 'B';
					} else {
						pathing[pathingCount] = 'L';
					}
					strcpy(nextAction.destination, pathing);
					pathingCount++;
					if(isLegalAction(g, nextAction) == FALSE) {
						if((pathingCount == 10) ||
							(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount-1] = 'B';
						} else {
							pathing[pathingCount-1] = 'R';
						}
						strcpy(nextAction.destination, pathing);
					}
				}
			}
		}
		if(isLegalAction(g, nextAction) == FALSE) {
			nextAction.actionCode = PASS;
		}
	}

    //CAMPUS CAMPUS CAMPUS CAMPUS CAMPUS CAMPUS
	if(ENOUGH_STUDENTS_FOR_CAMPUS) {
		nextAction.actionCode = BUILD_CAMPUS;
      memset(nextAction.destination, 0, sizeof(nextAction.destination));
		memset(pathing, 0, sizeof(pathing));
		if(myUni == 1) {
			nextAction = testPath(g, nextAction, pathing);
			if(isLegalAction(g, nextAction) == FALSE) {
				memset(pathing, 0, sizeof(pathing));
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "RLRLRLRLRLL");
				pathingCount += 11;
				while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
					if((pathingCount == 11) ||
						(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 18 && pathing[pathingCount-1] == 'B') ||
						(pathingCount == 20 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
					} else {
						pathing[pathingCount] = 'L';
					}
					strcpy(nextAction.destination, pathing);
					pathingCount++;
					if(isLegalAction(g, nextAction) == FALSE) {
						if((pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 19 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 21 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount] = 'B';
						} else {
							pathing[pathingCount-1] = 'R';
						}
						strcpy(nextAction.destination, pathing);
					}
				}
			}
		} else if(myUni == 2) {
			strcpy(pathing, "RRLRL");
			pathingCount += 5;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				 if((pathingCount == 5) || (pathingCount == 7 && pathing[pathingCount-1] == 'B')) {
					  pathing[pathingCount] = 'B';
				 } else {
					  pathing[pathingCount] = 'L';
				 }
				 strcpy(nextAction.destination, pathing);
				 pathingCount++;
				 if(isLegalAction(g, nextAction) == FALSE) {
					  if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 8 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount-1] = 'B';
					  } else {
							pathing[pathingCount-1] = 'R';
					  }
					  strcpy(nextAction.destination, pathing);
				 }
			}
			if(isLegalAction(g, nextAction) == FALSE) {
				memset(pathing, 0, sizeof(pathing));
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "LRLRLRRLRL");
				pathingCount += 10;
				while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
					if((pathingCount == 10) ||
					   (pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount-1] = 'B';
					} else {
						pathing[pathingCount] = 'L';
					}
					strcpy(nextAction.destination, pathing);
					pathingCount++;
					if(isLegalAction(g, nextAction) == FALSE) {
						if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount] = 'B';
						} else {
							pathing[pathingCount-1] = 'R';
						}
						strcpy(nextAction.destination, pathing);
					}
				}
			}
      } else if(myUni == 3) {
			strcpy(pathing, "LRLRL");
			pathingCount += 5;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				 if((pathingCount == 5) ||
				(pathingCount == 7 && pathing[pathingCount-1] == 'B') ||
				(pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
					  pathing[pathingCount] = 'B';
				 } else {
					  pathing[pathingCount] = 'L';
				 }
				 strcpy(nextAction.destination, pathing);
				 pathingCount++;
				 if(isLegalAction(g, nextAction) == FALSE) {
					  if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 8 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount-1] = 'B';
					  } else {
							pathing[pathingCount-1] = 'R';
					  }
					  strcpy(nextAction.destination, pathing);
				 }
			}
			if(isLegalAction(g, nextAction) == FALSE) {
				memset(pathing, 0, sizeof(pathing));
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "RLRLRLRRLR");
				pathingCount += 10;
				while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
					if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount] = 'B';
					} else {
						pathing[pathingCount] = 'L';
					}
					strcpy(nextAction.destination, pathing);
					pathingCount++;
					if(isLegalAction(g, nextAction) == FALSE) {
						if((pathingCount == 10) ||
						   (pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
						   (pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
						   (pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
						   (pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
						   (pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount-1] = 'B';
						} else {
							pathing[pathingCount-1] = 'R';
						}
						strcpy(nextAction.destination, pathing);
					} 
				}
			}
      }
		if(isLegalAction(g, nextAction) == FALSE) {
			nextAction.actionCode = PASS;
		}
	}

    //Build ARC Build ARC Build ARC Build ARC Build ARC Build ARC
    if(ENOUGH_STUDENTS_FOR_ARC) {
        nextAction.actionCode = OBTAIN_ARC;
        memset(nextAction.destination, 0, sizeof(nextAction.destination));
        if(myUni == 1) {
            nextAction = testPath(g, nextAction, pathing);
				if(isLegalAction(g, nextAction) == FALSE) {
					memset(pathing, 0, sizeof(pathing));
					memset(nextAction.destination, 0, sizeof(nextAction.destination));
					pathingCount = 0;
					strcpy(pathing, "RLRLRLRLRLL");
					pathingCount += 11;
					while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
						if((pathingCount == 11) ||
							(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 18 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 20 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount-1] = 'B';
						} else {
							pathing[pathingCount] = 'L';
						}
						strcpy(nextAction.destination, pathing);
						pathingCount++;
						if(isLegalAction(g, nextAction) == FALSE) {
							if((pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
								(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
								(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
								(pathingCount == 19 && pathing[pathingCount-1] == 'B') ||
								(pathingCount == 21 && pathing[pathingCount-1] == 'B')) {
								pathing[pathingCount] = 'B';
							} else {
								pathing[pathingCount-1] = 'R';
							}
							strcpy(nextAction.destination, pathing);
						}
					}
				}
         } else if(myUni == 2) {
            strcpy(pathing, "RRLRL");
            pathingCount += 5;
            while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
                if((pathingCount == 5) ||
				   (pathingCount == 7 && pathing[pathingCount-1] == 'B')) {
                    pathing[pathingCount] = 'B';
                } else {
                    pathing[pathingCount] = 'L';
                }
                strcpy(nextAction.destination, pathing);
                pathingCount++;
                if(isLegalAction(g, nextAction) == FALSE) {
                    if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 8 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
                        pathing[pathingCount-1] = 'B';
                    } else {
                        pathing[pathingCount-1] = 'R';
                    }
                    strcpy(nextAction.destination, pathing);
                }
            }
				if(isLegalAction(g, nextAction) == FALSE) {
					memset(pathing, 0, sizeof(pathing));
					memset(nextAction.destination, 0, sizeof(nextAction.destination));
					pathingCount = 0;
					strcpy(pathing, "LRLRLRRLRL");
					pathingCount += 10;
					while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
						if((pathingCount == 10) ||
							(pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
							(pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount-1] = 'B';
						} else {
							pathing[pathingCount] = 'L';
						}
						strcpy(nextAction.destination, pathing);
						pathingCount++;
						if(isLegalAction(g, nextAction) == FALSE) {
							if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
								(pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
								(pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
								(pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
								pathing[pathingCount] = 'B';
							} else {
								pathing[pathingCount-1] = 'R';
							}
							strcpy(nextAction.destination, pathing);
						}
					}
				}
        } else if(myUni == 3) {
			strcpy(pathing, "LRLRL");
			pathingCount += 5;
			while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
				 if((pathingCount == 5) ||
				(pathingCount == 7 && pathing[pathingCount-1] == 'B') ||
				(pathingCount == 9 && pathing[pathingCount-1] == 'B')) {
					  pathing[pathingCount] = 'B';
				 } else {
					  pathing[pathingCount] = 'L';
				 }
				 strcpy(nextAction.destination, pathing);
				 pathingCount++;
				 if(isLegalAction(g, nextAction) == FALSE) {
					  if((pathingCount == 6 && pathing[pathingCount-1] == 'B') ||
					(pathingCount == 8 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount-1] = 'B';
					  } else {
							pathing[pathingCount-1] = 'R';
					  }
					  strcpy(nextAction.destination, pathing);
				 }
			}
			if(isLegalAction(g, nextAction) == FALSE) {
				memset(pathing, 0, sizeof(pathing));
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "RLRLRLRRLR");
				pathingCount += 10;
				while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
					if((pathingCount == 11 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 14 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 16 && pathing[pathingCount-1] == 'B') ||
					   (pathingCount == 18 && pathing[pathingCount-1] == 'B')) {
						pathing[pathingCount] = 'B';
					} else {
						pathing[pathingCount] = 'L';
					}
					strcpy(nextAction.destination, pathing);
					pathingCount++;
					if(isLegalAction(g, nextAction) == FALSE) {
						if((pathingCount == 10) ||
						   (pathingCount == 12 && pathing[pathingCount-1] == 'B') ||
						   (pathingCount == 13 && pathing[pathingCount-1] == 'B') ||
						   (pathingCount == 15 && pathing[pathingCount-1] == 'B') ||
						   (pathingCount == 17 && pathing[pathingCount-1] == 'B') ||
						   (pathingCount == 19 && pathing[pathingCount-1] == 'B')) {
							pathing[pathingCount-1] = 'B';
						} else {
							pathing[pathingCount-1] = 'R';
						}
						strcpy(nextAction.destination, pathing);
					}
				}
			}
      }
		if(isLegalAction(g, nextAction) == FALSE) {
			cannotBuildArc = TRUE;
			nextAction.actionCode = PASS;
		} else {
			cannotBuildArc = FALSE;
		}
   }

    // Spinoff
    if(ENOUGH_STUDENTS_FOR_SPINOFF) {
		nextAction.actionCode = START_SPINOFF;
    }

   return nextAction;
}

action testPath(Game g, action nextAction, char * pathing) {
	int pathingCount = 0;
	while(isLegalAction(g, nextAction) == FALSE && pathingCount < PATH_LIMIT) {
		pathing[pathingCount] = 'L';
		strcpy(nextAction.destination, pathing);
		pathingCount++;
		if(isLegalAction(g, nextAction) == FALSE) {
				pathing[pathingCount-1] = 'R';
				strcpy(nextAction.destination, pathing);
		}
	}
	return nextAction;
}
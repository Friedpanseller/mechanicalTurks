/*
 *  mechanicalTurk.  Brain the size of a sun!
 *
 *  Proundly Created by tony tang && leo liu
 *  Share Freely Creative Commons SA-BY-NC 3.0.  yeah nah
 *
 */
 
 nuh
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
 
#include "Game.h"
#include "mechanicalTurk.h"

#define ENOUGH_STUDENTS_FOR_ARC 	getStudents (g, getWhoseTurn(g), STUDENT_BPS) != 1 && getStudents (g, getWhoseTurn(g), STUDENT_BQN) != 1
#define ENOUGH_STUDENTS_FOR_CAMPUS	getStudents (g, getWhoseTurn(g), STUDENT_BPS) > 0 && getStudents (g, getWhoseTurn(g), STUDENT_BQN) > 0 && getStudents (g, getWhoseTurn(g), STUDENT_MJ) > 0 && getStudents (g, getWhoseTurn(g), STUDENT_MTV) > 0
#define ENOUGH_STUDENTS_FOR_GO8		getStudents (g, getWhoseTurn(g), STUDENT_MJ) >= 2 && getStudents (g, getWhoseTurn(g), STUDENT_MMONEY) >= 3

#define GET_BPS								(getStudents (g, getWhoseTurn(g), STUDENT_BPS))
#define GET_BQN								(getStudents (g, getWhoseTurn(g), STUDENT_BQN))
#define GET_MJ								(getStudents (g, getWhoseTurn(g), STUDENT_MJ))
#define GET_MTV								(getStudents (g, getWhoseTurn(g), STUDENT_MTV))
#define GET_MMONEY							(getStudents (g, getWhoseTurn(g), STUDENT_MMONEY))
#define GET_CAMPUS							(getCampuses (g, getWhoseTurn(g)))
#define GET_GO8								(getGO8s (g, getWhoseTurn(g)))

action decideAction (Game g) {
 
    action nextAction;
    int myUni = getWhoseTurn(g);
    nextAction.actionCode = PASS;
    char pathing[PATH_LIMIT];
    memset(pathing, 0, sizeof(pathing));
    int pathingCount = 0;
	int isLegalArc = 0;
	int isLegalVertex = 0;
	int count = 0;
    //Build ARC Build ARC Build ARC Build ARC Build ARC Build ARC
    if(ENOUGH_STUDENTS_FOR_ARC) {
        nextAction.actionCode = OBTAIN_ARC;
        memset(nextAction.destination, 0, sizeof(nextAction.destination));
		count = 0;
        if(myUni == 1) {
            while(isLegalAction(g, nextAction) == FALSE && count < 100) {
                pathing[pathingCount] = 'L';
                strcpy(nextAction.destination, pathing);
                pathingCount++;
                if(isLegalAction(g, nextAction) == FALSE) {
                    pathing[pathingCount-1] = 'R';
                    strcpy(nextAction.destination, pathing);
					if(isLegalAction(g, nextAction) == TRUE) {
						isLegalArc++;
					}
                } else {
                    isLegalArc++;
                }
				count++;
            }
			if(isLegalArc == 0) {
				memset(pathing, 0, sizeof(pathing));
				count = 0;
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "RLRLRLRLRLL");
				pathingCount += 11;
				while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
						if(isLegalAction(g, nextAction) == TRUE) {
							isLegalArc++;
						}
					} else {
						isLegalArc++;
					}
					count++;
				}
			}
        } else if(myUni == 2) {
            strcpy(pathing, "RRLRL");
            pathingCount += 5;
            while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
					if(isLegalAction(g, nextAction) == TRUE) {
						isLegalArc++;
					}
                } else {
                    isLegalArc++;
                }
				count++;
            }
			if(isLegalArc == 0) {
				memset(pathing, 0, sizeof(pathing));
				count = 0;
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "LRLRLRRLRL");
				pathingCount += 10;
				while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
						if(isLegalAction(g, nextAction) == TRUE) {
							isLegalArc++;
						}
					} else {
						isLegalArc++;
					}
					count++;
				}
			}
			if(isLegalAction(g, nextAction) == FALSE) {
			 printf("Build Arc Failed :(\n");
			 printf("BQN = %d, BPS = %d\n", GET_BQN, GET_BQN);
		 } else {
			 printf("Build Arc Success!\n");
		 }
        } else if(myUni == 3) {
			strcpy(pathing, "LRLRL");
            pathingCount += 5;
            while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
					if(isLegalAction(g, nextAction) == TRUE) {
						isLegalArc++;
					}
                } else {
                    isLegalArc++;
                }
				count++;
            }
			if(isLegalArc == 0) {
				memset(pathing, 0, sizeof(pathing));
				count = 0;
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "RLRLRLRRLR");
				pathingCount += 10;
				while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
						if(isLegalAction(g, nextAction) == TRUE) {
							isLegalArc++;
						}
					} else {
						isLegalArc++;
					}
					count++;
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
		count = 0;
		memset(pathing, 0, sizeof(pathing));
        if(myUni == 1) {
            while(isLegalAction(g, nextAction) == FALSE && count < 100) {
                pathing[pathingCount] = 'L';
                strcpy(nextAction.destination, pathing);
                pathingCount++;
                if(isLegalAction(g, nextAction) == FALSE) {
                    pathing[pathingCount-1] = 'R';
                    strcpy(nextAction.destination, pathing);
					if(isLegalAction(g, nextAction) == TRUE) {
						isLegalVertex++;
					}
                } else {
                    isLegalVertex++;
                }
				count++;
            }
			if(isLegalVertex == 0) {
				memset(pathing, 0, sizeof(pathing));
				count = 0;
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "RLRLRLRLRLL");
				pathingCount += 11;
				while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
						if(isLegalAction(g, nextAction) == TRUE) {
							isLegalVertex++;
						}
					} else {
						isLegalVertex++;
					}
					count++;
				}
			}
        } else if(myUni == 2) {
            strcpy(pathing, "RRLRL");
            pathingCount += 5;
            while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
					if(isLegalAction(g, nextAction) == TRUE) {
						isLegalVertex++;
					}
                } else {
                    isLegalVertex++;
                }
				count++;
            }
			if(isLegalVertex == 0) {
				memset(pathing, 0, sizeof(pathing));
				count = 0;
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "LRLRLRRLRL");
				pathingCount += 10;
				while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
						if(isLegalAction(g, nextAction) == TRUE) {
							isLegalVertex++;
						}
					} else {
						isLegalVertex++;
					}
					count++;
				}
			}
        } else if(myUni == 3) {
			strcpy(pathing, "LRLRL");
            pathingCount += 5;
            while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
					if(isLegalAction(g, nextAction) == TRUE) {
						isLegalVertex++;
					}
                } else {
                    isLegalVertex++;
                }
				count++;
            }
			if(isLegalVertex == 0) {
				memset(pathing, 0, sizeof(pathing));
				count = 0;
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "RLRLRLRRLR");
				pathingCount += 10;
				while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
						if(isLegalAction(g, nextAction) == TRUE) {
							isLegalVertex++;
						}
					} else {
						isLegalVertex++;
					}
					count++;
				}
			}
        }
		if(isLegalAction(g, nextAction) == FALSE) {
			nextAction.actionCode = PASS;
		}
	}
	
	//GO8 GO8 GO8 GO8 GO8 GO8 GO8 GO8 GO8 GO8 GO8 GO8
	if(ENOUGH_STUDENTS_FOR_GO8) {
		nextAction.actionCode = BUILD_GO8;
        memset(nextAction.destination, 0, sizeof(nextAction.destination));
		count = 0;
		isLegalVertex = 0;
		memset(pathing, 0, sizeof(pathing));
        if(myUni == 1) {
            while(isLegalAction(g, nextAction) == FALSE && count < 100) {
                pathing[pathingCount] = 'L';
                strcpy(nextAction.destination, pathing);
                pathingCount++;
                if(isLegalAction(g, nextAction) == FALSE) {
                    pathing[pathingCount-1] = 'R';
                    strcpy(nextAction.destination, pathing);
					if(isLegalAction(g, nextAction) == TRUE) {
						isLegalVertex++;
					}
                } else {
                    isLegalVertex++;
                }
				count++;
            }
			if(isLegalVertex == 0) {
				memset(pathing, 0, sizeof(pathing));
				count = 0;
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "RLRLRLRLRLL");
				pathingCount += 11;
				while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
						if(isLegalAction(g, nextAction) == TRUE) {
							isLegalVertex++;
						}
					} else {
						isLegalVertex++;
					}
					count++;
				}
			}
        } else if(myUni == 2) {
            strcpy(pathing, "RRLRL");
            pathingCount += 5;
            while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
					if(isLegalAction(g, nextAction) == TRUE) {
						isLegalVertex++;
					}
                } else {
                    isLegalVertex++;
                }
				count++;
            }
			if(isLegalVertex == 0) {
				memset(pathing, 0, sizeof(pathing));
				count = 0;
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "LRLRLRRLRL");
				pathingCount += 10;
				while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
						if(isLegalAction(g, nextAction) == TRUE) {
							isLegalVertex++;
						}
					} else {
						isLegalVertex++;
					}
					count++;
				}
			}
        } else if(myUni == 3) {
			strcpy(pathing, "LRLRL");
            pathingCount += 5;
            while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
					if(isLegalAction(g, nextAction) == TRUE) {
						isLegalVertex++;
					}
                } else {
                    isLegalVertex++;
                }
				count++;
            }
			if(isLegalVertex == 0) {
				memset(pathing, 0, sizeof(pathing));
				count = 0;
				memset(nextAction.destination, 0, sizeof(nextAction.destination));
				pathingCount = 0;
				strcpy(pathing, "RLRLRLRRLR");
				pathingCount += 10;
				while(isLegalAction(g, nextAction) == FALSE && count < 100) {
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
						if(isLegalAction(g, nextAction) == TRUE) {
							isLegalVertex++;
						}
					} else {
						isLegalVertex++;
					}
					count++;
				}
			}
        }
		if(isLegalAction(g, nextAction) == FALSE) {
			nextAction.actionCode = PASS;
		}
	}
	
    //Retrain BPS
    if((getStudents (g, getWhoseTurn(g), STUDENT_BPS)) >= 3) {
        nextAction.actionCode = RETRAIN_STUDENTS;
        nextAction.disciplineFrom = STUDENT_BPS;
      nextAction.disciplineTo = (rand() % 3 + 3);
    } 

    //Retrain BQN
    if((getStudents (g, getWhoseTurn(g), STUDENT_BQN)) >= 3) {
      nextAction.actionCode = RETRAIN_STUDENTS;
      nextAction.disciplineFrom = STUDENT_BQN;
      nextAction.disciplineTo = (rand() % 3 + 3);
    } 
    
    // Retrain MJ
    if((getStudents (g, getWhoseTurn(g), STUDENT_MJ)) >= 4) {
      nextAction.actionCode = RETRAIN_STUDENTS;
        nextAction.disciplineFrom = STUDENT_MJ;
      nextAction.disciplineTo = (rand() % 3 + 3);
    } 

    // Spinoff
    if((getStudents (g, getWhoseTurn(g), STUDENT_MJ)) != 0 && 
       (getStudents (g, getWhoseTurn(g), STUDENT_MTV)) != 0 && 
       (getStudents (g, getWhoseTurn(g), STUDENT_MMONEY)) != 0) {
		nextAction.actionCode = START_SPINOFF;
    }
	  
   return nextAction;
}

//int whatToBuild (Game g) {
   // Build GO8 - best building
   // STILL HAVE TO IMPLEMENT TESTING IF THERE IS A NORMAL CAMPUS
   // TO BE CONVERTED INTO A GO8
   //int nextAction = PASS;
   //if((getStudents (g, getWhoseTurn(g), STUDENT_MJ)) >= 2 && 
   //   (getStudents (g, getWhoseTurn(g), STUDENT_MMONEY)) >= 3) {
   //   nextAction = BUILD_GO8;
   //} else 
   //if((getStudents (g, getWhoseTurn(g), STUDENT_MJ)) != 0 && 
   //        (getStudents (g, getWhoseTurn(g), STUDENT_MTV)) != 0 && 
   //        (getStudents (g, getWhoseTurn(g), STUDENT_MMONEY)) != 0) {
   //   nextAction = START_SPINOFF;
   //}
   //return nextAction;
//}
//========================================================================//
//========================================================================//
#include <time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <deque>

#include "game.cpp"
using namespace std;
//========================================================================//
//========================================================================//
// MAKE YOUR TYPE DATA A GLOBAL DEFINITION
// IT DOESN'T CREATE A VARIABLE,
// JUST A DEFINITION OF DATA TYPE
// AS DIFFERENT CLASSES HAS TO COMMUNICATE EASILY
struct data_domino {
	int right, left, available;
};
//========================================================================//
//========================================================================//
class CRandom {
public:
	CRandom() {}  // constructor
	~CRandom() {} // destructor

	int getRandomPublic(int rangeLow, int rangeHigh) {
		int myRand_scaled;

		myRand_scaled = getRandomPrivate(rangeLow, rangeHigh);

		return myRand_scaled;
	}
private:
	// Generates uniform distribution between rangeLow and rangeHigh
	int getRandomPrivate(int rangeLow, int rangeHigh) {
		double myRand = rand() / (1.0 + RAND_MAX);
		int range = rangeHigh - rangeLow + 1;
		int myRand_scaled = (myRand * range) + rangeLow;
		return myRand_scaled;
	}
protected:
	// Generates uniform distribution between rangeLow and rangeHigh
	int getRandomProtected(int rangeLow, int rangeHigh) {
		double myRand = rand() / (1.0 + RAND_MAX);
		int range = rangeHigh - rangeLow + 1;
		int myRand_scaled = (myRand * range) + rangeLow;
		return myRand_scaled;
	}

};
//========================================================================//
//========================================================================//
class CDomino {
public:
	CDomino() {}  // constructor
	~CDomino() {} // destructor
	deque<data_domino> myDomino;
	void API(void) {
		init();
	}
	data_domino getPiece(int pieceID) {
		data_domino mypiece = myDomino.at(pieceID);
		cout << "[" << mypiece.left << "|" << mypiece.right << "]"
			<< " available = " << mypiece.available << endl;

		myDomino.at(pieceID) = mypiece;
		return(mypiece);
	}
private:
	void init(void) {
		data_domino mypiece;
		for (int right = 0; right < 7; right++) {
			for (int left = right; left < 7; left++) {
				mypiece.right = right;
				mypiece.left = left;
				mypiece.available = 1;
				cout << "[" << mypiece.left << "|" << mypiece.right << "]" << "  ";
				myDomino.push_back(mypiece);
			}
			cout << endl;
		}
		cout << "myDomino stores " << (int)myDomino.size() << " pieces.\n";
	}
};
//========================================================================//
//========================================================================//
class CPlayer : public CRandom {
public:
	CPlayer() {}  // constructor
	~CPlayer() {} // destructor
	deque<data_domino> gotHand;
	class CDomino *player_pDominoOBJ = NULL;
	// PASSING OBJECT AS POINTER - FOR DIFFERENT CLASSES INTERFACE
	void API(CDomino *receive_dominoPointerOBJ) {
		int pieceID;
		player_pDominoOBJ = receive_dominoPointerOBJ;
	}
	int takePiece(int pieceNo) {
		int playerID;
		int counter = 0, max = 14, number_wasAvailable = 0;

		data_domino takenPiece;
		takenPiece = player_pDominoOBJ->getPiece(pieceNo);

		if (takenPiece.available) {
			number_wasAvailable = takenPiece.available;
			takenPiece.available = 0; // no longer available from the dominoes pile
			player_pDominoOBJ->myDomino.at(pieceNo) = takenPiece;
			cout << " [" << takenPiece.left
				<< "|" << takenPiece.right << "]"
				<< " just taken - no longer available from pile = "
				<< takenPiece.available << endl;
			takenPiece.available = 1; // available on Player's hand
			gotHand.push_back(takenPiece);
		}
		else {
			cout << "NOT AVAILABLE" << endl;
		}
		return (number_wasAvailable);
	}
};
//========================================================================//
//========================================================================//
class CTable {
public:
	CTable() {}  // constructor
	~CTable() {} // destructor
	class CPlayer *playerOBJ = NULL;

	void selecting_pieces() {
		int pieceNO, piece_wasAvailable, totalPlayer = 2;
		cout << "selecting pieces and giving 10 pieces to each player" << endl;

		for (int playerID = 0; playerID < totalPlayer; playerID++) {
			for (int i = 0; i < 10; i++) {
				pieceNO = playerOBJ[playerID].getRandomPublic(0, 27); //get random piece
				cout << " pieceNO : " << pieceNO << endl;
				piece_wasAvailable = playerOBJ[playerID].takePiece(pieceNO);

				if (piece_wasAvailable) {
					cout << "piece available" << endl;
				}
				else {
					cout << "////////////////////////////////////////////////" << endl;
					cout << "piece not available - try to take a piece again" << endl;
					i--;
				}
			}
		}
	}
	void showPlayerHand(void) {
		data_domino showpiece;
		int totalPlayer = 2;
		for (int playerID = 0; playerID < totalPlayer; playerID++) {
			cout << "playerID = " << playerID << " stores " <<
				playerOBJ[playerID].gotHand.size() << " pieces.\n";
			for (int pieceNo = 0; pieceNo < playerOBJ[playerID].gotHand.size(); pieceNo++) {
				showpiece = playerOBJ[playerID].gotHand.at(pieceNo);
				cout << "[" << showpiece.left << "|" << showpiece.right << "]"
					<< " available = " << showpiece.available << endl;
			}
		}
	}
	void API(CPlayer *receive_playersOBJ) {
		playerOBJ = receive_playersOBJ;

		selecting_pieces();
		showPlayerHand();
	}
}; // END CTable
//========================================================================//
//========================================================================//


class Game {
private:
	CPlayer *playerOBJ;
	data_domino playOn;
	deque<int> gameBoard;
	int pile = 0;
	int turn = 0;
	bool pass1 = false;
	bool pass2 = false;
public:

	Game(CPlayer* playOBJ) {
		playerOBJ = playOBJ;
		pile = 28 - playerOBJ[0].gotHand.size() - playerOBJ[1].gotHand.size();
		getStartingTurn();
	}

	void getStartingTurn() {
		int high1 = highestDoublePos(0);
		int high2 = highestDoublePos(1);
		if (playerOBJ[0].gotHand[high1].left < playerOBJ[1].gotHand[high2].left) {
			playOn.left = playerOBJ[1].gotHand[high2].left;
			playOn.right = playerOBJ[1].gotHand[high2].left;
			gameBoard.push_front(playOn.left);
			gameBoard.push_front(playOn.left);
			playerOBJ[1].gotHand.erase(playerOBJ[1].gotHand.begin() + high2);
			turn = 0;
		}
		else {
			playOn.left = playerOBJ[0].gotHand[high1].left;
			playOn.right = playerOBJ[0].gotHand[high1].left;
			gameBoard.push_front(playOn.left);
			gameBoard.push_front(playOn.left);
			playerOBJ[0].gotHand.erase(playerOBJ[1].gotHand.begin() + high1);
			turn = 0;
		}
		
	}

	bool runGame() {
		if (playerOBJ[turn].gotHand.size() == 0) {
			if (turn == 0) {
				//END GAME: player 1 wins
				cout << "player 1 has won the game!" << endl;
			} else {
				//END GAME: player 2 wins
				cout << "player 2 has won the game!" << endl;
			}
			return false;
		}
		else if (pass1 && pass2) {
			//check for draw
			cout << "The game ends in a draw!" << endl;
			return false;
		}
		else {
			if (turn == 0) {
				pass1 = !findPieceMatch();
				turn = 1;
			}
			else if (turn == 1) {
				pass2 = !findPieceMatch();
				turn = 0;
			}

			// Game still running, return true
			return true;
		}
	}
	//return int value of biggest double in hand
	int highestDoublePos(int player) {
		int highDouble = -1;
		int highDoublePos = 0;
		for (int i = 0; i < playerOBJ[player].gotHand.size(); i++) {
			if (playerOBJ[player].gotHand[i].left == playerOBJ[player].gotHand[i].right) {
				if (highDouble < playerOBJ[player].gotHand[i].left) {
					highDouble = playerOBJ[player].gotHand[i].left;
					highDoublePos = i;
					}
			}
		}
		return highDoublePos;
	}
	//return false then player passes
	bool findPieceMatch() {
		bool canPlay = false;
		int pieceNo;
		for (pieceNo = 0; (pieceNo < playerOBJ[turn].gotHand.size()) && (canPlay == false); pieceNo++) {
			canPlay = canPlayPiece(playerOBJ[turn].gotHand[pieceNo]);
		}
		//draw
		while (canPlay == false) {
			bool pass = draw();
			pile = pile - 1;
			if (pass) {
				return false;
			}
			else {
				pieceNo = playerOBJ[turn].gotHand.size() - 1;
				canPlay = canPlayPiece(playerOBJ[turn].gotHand[pieceNo]);
			}
		}
		playerOBJ[turn].gotHand.erase(playerOBJ[turn].gotHand.begin() + pieceNo - 1);
		return true;
	}

	bool canPlayPiece(data_domino poss_piece) {
		bool found = false;
			if (poss_piece.left == playOn.left) {
				gameBoard.push_front(poss_piece.left);
				gameBoard.push_front(poss_piece.right);
				playOn.left = poss_piece.right;
				found = true;
			}
			else if (poss_piece.right == playOn.left) {
				gameBoard.push_front(poss_piece.right);
				gameBoard.push_front(poss_piece.left);
				playOn.left = poss_piece.left;
				found = true;
			}
			else if (poss_piece.left == playOn.right) {
				gameBoard.push_back(poss_piece.left);
				gameBoard.push_back(poss_piece.right);
				playOn.right = poss_piece.right;
				found = true;
			}
			else if (poss_piece.right == playOn.right) {
				gameBoard.push_back(poss_piece.right);
				gameBoard.push_back(poss_piece.left);
				playOn.right = poss_piece.left;
				found = true;
			}
		return found;
	}

	bool draw() {
		bool drawAPiece = true;
		int pieceNO = 0, piece_wasAvailable = 0;
		if (pile <= 0) {
			return true;
		}
		while (drawAPiece) {
			drawAPiece = false;
			pieceNO = playerOBJ[turn].getRandomPublic(0, 27); //get random piece
			piece_wasAvailable = playerOBJ[turn].takePiece(pieceNO);
			if (!piece_wasAvailable) {
				cout << "////////////////////////////////////////////////" << endl;
				cout << "piece not available - try to take a piece again" << endl;
				drawAPiece = true;
			}
		}
		return false;
	}
	void printGame() {
		int rowEnd = 0;
		for (int i = 0; i < gameBoard.size(); i += 2) {
			cout << "[" << gameBoard.at(i) << "|" << gameBoard.at(i+1) << "]";
		}
		cout << endl;
	}

};
//========================================================================//
//========================================================================//
int main(void) {

	srand(time(NULL));
	
	// create dominoes
	CDomino dominoOBJ, *dominoPointer;
	dominoOBJ.API();
	dominoPointer = &dominoOBJ;

	// create players
	CPlayer *playerOBJ;
	playerOBJ = new CPlayer[2];
	playerOBJ[0].API(dominoPointer);
	playerOBJ[1].API(dominoPointer);

	//draw dominoes
	CTable myTableObj;
	myTableObj.API(playerOBJ);

	//whos first
	Game game(playerOBJ);
	bool isRunning = true;
	while (isRunning) {
		isRunning = game.runGame();
	}
	game.printGame();
	delete[]playerOBJ;
	system("pause");
	return EXIT_SUCCESS;
}
//========================================================================//
//========================================================================//
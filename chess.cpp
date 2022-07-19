#include <iostream>
#include <stdlib.h>
#include <stack>
#include <string>
using namespace std;

static int count = 0;
static int person;

class Game {
public:
	char winner;
	int whiteScore = 0;
	int blackScore = 0;
	stack <char> whitePieces;
	stack <char> blackPieces;
	stack <char> temp;

	void printWhitePieces() {
		while(!whitePieces.empty()) {
			 cout<<" W"<<whitePieces.top();
			 temp.push(whitePieces.top());
			 whitePieces.pop();
		}
		while(!temp.empty()) {
			whitePieces.push(temp.top());
			temp.pop();
		}
	}
	
	void printBlackPieces() {
		while(!blackPieces.empty()) {
			 cout<<" B"<<blackPieces.top()<<" ";
			 temp.push(blackPieces.top());
			 blackPieces.pop();
		}
		while(!temp.empty()) {
			blackPieces.push(temp.top());
			temp.pop();
		}
	}
	
	int score(char pieceType) {
		switch(pieceType) {
			case 'P':
				return 1;
			case 'N':
				return 3;
			case 'B':
				return 3;
			case 'R':
				return 5;
			case 'Q':
				return 9;
		}
		return 0;
	}
};

class Piece {
private:
    char mPieceColor;
    virtual bool areSquaresLegal(int startRow, int startColumn, int endRow, int endColumn, Piece *gameBoard[8][8]) = 0;
public:
    Piece(char pieceColor) : mPieceColor(pieceColor) {}
    ~Piece() {}
    virtual char getPieceType() = 0;
    char getPieceColor() {
        return mPieceColor;
    }
    bool isLegalMove(int startRow, int startColumn, int endRow, int endColumn, Piece *gameBoard[8][8]) {
        Piece* destLocation = gameBoard[endRow][endColumn];
        if((destLocation == 0) || (mPieceColor != destLocation->getPieceColor())) {
            return areSquaresLegal(startRow, startColumn, endRow, endColumn, gameBoard);
        }
        return false;
    }
};

class PawnPiece : public Piece {
public:
    PawnPiece(char pieceColor) : Piece(pieceColor) {}
    ~PawnPiece() {}
private:
    virtual char getPieceType() {
        return 'P';
    }
    bool areSquaresLegal(int startRow, int startColumn, int endRow, int endColumn, Piece *gameBoard[8][8]) {
        Piece *destLocation = gameBoard[endRow][endColumn];
        if(destLocation == 0) {
            if(startColumn == endColumn) {
                if(getPieceColor() == 'W') {
                	if((endRow == startRow + 2) && (startRow == 1)) {
                		return true;
                	}
                    if(endRow == startRow + 1) {
                        return true;
                    }
                } 
                else{
                	if((endRow == startRow -2) && (startRow == 6)) {
                		return true;
                	}
                    if(endRow == startRow - 1) {
                        return true;
                    }
                }
            }
        } 
        else{
            if((startColumn == endColumn + 1) || (startColumn == endColumn - 1)) {
                if(getPieceColor() == 'W') {
                    if(endRow == startRow + 1) {
                        return true;
                    }
                } 
                else{
                    if(endRow == startRow - 1) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

class KnightPiece : public Piece {
public:
    KnightPiece(char pieceColor) : Piece(pieceColor) {}
    ~KnightPiece() {}
private:
    virtual char getPieceType() {
        return 'N';
    }
    bool areSquaresLegal(int startRow, int startColumn, int endRow, int endColumn, Piece *gameBoard[8][8]) {
        if((startColumn == endColumn + 1) || (startColumn == endColumn - 1)) {
            if((startRow == endRow + 2) || (startRow == endRow - 2)) {
                return true;
            }
        }
        if((startColumn == endColumn + 2) || (startColumn == endColumn - 2)) {
            if((startRow == endRow + 1) || (startRow == endRow - 1)) {
                return true;
            }
        }
        return false;
    }
};

class BishopPiece : public Piece {
public:
    BishopPiece(char pieceColor) : Piece(pieceColor) {}
    ~BishopPiece() {}
private:
    virtual char getPieceType() {
        return 'B';
    }
    bool areSquaresLegal(int startRow, int startColumn, int endRow, int endColumn, Piece *gameBoard[8][8]) {
        if((endColumn - startColumn == endRow - startRow) || (endColumn - startColumn == startRow - endRow)) {
            int rowOffset = (endRow - startRow > 0) ? 1 : -1;
            int columnOffset = (endColumn - startColumn > 0) ? 1 : -1;
            int checkRow;
            int checkCol;
            for(checkRow = startRow + rowOffset, checkCol = startColumn + columnOffset;
                checkRow !=  endRow;
                checkRow = checkRow + rowOffset, checkCol = checkCol + columnOffset)
            {
                if(gameBoard[checkRow][checkCol] != 0) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class RookPiece : public Piece
{
public:
    RookPiece(char pieceColor) : Piece(pieceColor) {}
    ~RookPiece() {}
private:
    virtual char getPieceType() {
        return 'R';
    }
    bool areSquaresLegal(int startRow, int startColumn, int endRow, int endColumn, Piece* gameBoard[8][8]) {
        if(startRow == endRow) {
            int columnOffset = (endColumn - startColumn > 0) ? 1 : -1;
            for(int checkCol = startColumn + columnOffset; checkCol !=  endColumn; checkCol = checkCol + columnOffset) {
                if(gameBoard[startRow][checkCol] != 0) {
                    return false;
                }
            }
            return true;
        } else if(endColumn == startColumn) {
            int rowOffset = (endRow - startRow > 0) ? 1 : -1;
            for(int checkRow = startRow + rowOffset; checkRow !=  endRow; checkRow = checkRow + rowOffset) {
                if(gameBoard[checkRow][startColumn] != 0) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class QueenPiece : public Piece
{
public:
    QueenPiece(char pieceColor) : Piece(pieceColor) {}
    ~QueenPiece() {}
private:
    virtual char getPieceType() {
        return 'Q';
    }
    bool areSquaresLegal(int startRow, int startColumn, int endRow, int endColumn, Piece* gameBoard[8][8]) {
        if(startRow == endRow) {
            int columnOffset = (endColumn - startColumn > 0) ? 1 : -1;
            for(int checkCol = startColumn + columnOffset; checkCol !=  endColumn; checkCol = checkCol + columnOffset) {
                if(gameBoard[startRow][checkCol] != 0) {
                    return false;
                }
            }
            return true;
        } 
        else if(endColumn == startColumn) {
            int rowOffset = (endRow - startRow > 0) ? 1 : -1;
            for(int checkRow = startRow + rowOffset; checkRow !=  endRow; checkRow = checkRow + rowOffset) {
                if(gameBoard[checkRow][startColumn] != 0) {
                    return false;
                }
            }
            return true;
        } 
        else if((endColumn - startColumn == endRow - startRow) || (endColumn - startColumn == startRow - endRow)) {
            int rowOffset = (endRow - startRow > 0) ? 1 : -1;
            int columnOffset = (endColumn - startColumn > 0) ? 1 : -1;
            int checkRow;
            int checkCol;
            for(checkRow = startRow + rowOffset, checkCol = startColumn + columnOffset;
                checkRow !=  endRow;
                checkRow = checkRow + rowOffset, checkCol = checkCol + columnOffset)
            {
                if(gameBoard[checkRow][checkCol] != 0) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class KingPiece : public Piece {
public:
    KingPiece(char pieceColor) : Piece(pieceColor) {}
    ~KingPiece() {}
private:
    virtual char getPieceType() {
        return 'K';
    }
    bool areSquaresLegal(int startRow, int startColumn, int endRow, int endColumn, Piece* gameBoard[8][8]) {
        int rowDelta = endRow - startRow;
        int columnDelta = endColumn - startColumn;
        if(((rowDelta >= -1) && (rowDelta <= 1)) &&
            ((columnDelta >= -1) && (columnDelta <= 1)))
        {
            return true;
        }
        return false;
    }
};

class CreateBoard {
public:
	Piece *mainGameBoard[8][8];
	CreateBoard() {
		for(int iRow = 0; iRow < 8; iRow++) {
			for(int iColumn = 0; iColumn < 8; iColumn++) {
				mainGameBoard[iRow][iColumn] = 0;
			}
		}
		for(int iColumn = 0; iColumn < 8; iColumn++) {
			mainGameBoard[6][iColumn] = new PawnPiece('B');
		}
		mainGameBoard[7][0] = new RookPiece('B');
        mainGameBoard[7][1] = new KnightPiece('B');
        mainGameBoard[7][2] = new BishopPiece('B');
        mainGameBoard[7][3] = new KingPiece('B');
        mainGameBoard[7][4] = new QueenPiece('B');
        mainGameBoard[7][5] = new BishopPiece('B');
        mainGameBoard[7][6] = new KnightPiece('B');
        mainGameBoard[7][7] = new RookPiece('B');
        for(int iColumn = 0; iColumn < 8; iColumn++) {
			mainGameBoard[1][iColumn] = new PawnPiece('W');
		}
		mainGameBoard[0][0] = new RookPiece('W');
        mainGameBoard[0][1] = new KnightPiece('W');
        mainGameBoard[0][2] = new BishopPiece('W');
        mainGameBoard[0][3] = new KingPiece('W');
        mainGameBoard[0][4] = new QueenPiece('W');
        mainGameBoard[0][5] = new BishopPiece('W');
        mainGameBoard[0][6] = new KnightPiece('W');
        mainGameBoard[0][7] = new RookPiece('W');
	}

	~CreateBoard() {
		for(int iRow = 0; iRow < 8; iRow++) {
			for(int iColumn = 0; iColumn < 8; iColumn++) {
				delete mainGameBoard[iRow][iColumn];
				mainGameBoard[iRow][iColumn] = 0;
			}
		}
	}

	void print() {
		const int iSquareWidth = 6;
        const int iSquareHeight = 3;
        for(int iRow = 0; iRow < 8 * iSquareHeight; iRow++) {
        	cout<<"\t\t   ";
        	int iSquareRow = iRow / iSquareHeight;
        	if(iRow % 3 == 1) {
        		cout<<"--0"<<(char)('1' + 7 - iSquareRow)<<"--";
        	}
        	else {
        		cout<<"------";
        	}
        	for(int iColumn = 0; iColumn < 8 * iSquareWidth; iColumn++) {
        		int iSquareColumn = iColumn / iSquareWidth;
        		if(iRow % 3 == 1 && ((iColumn % 6 == 2) || (iColumn % 6 == 3)) && mainGameBoard[7 - iSquareRow][iSquareColumn]) {
        			if ((iColumn % 6) == 2) {
                        cout<<mainGameBoard[7-iSquareRow][iSquareColumn]->getPieceColor();
                    } 
                    else {
                        cout<<mainGameBoard[7-iSquareRow][iSquareColumn]->getPieceType();
                    }
        		}
        		else {
        			if ((iSquareRow + iSquareColumn) % 2 == 1) {
                        cout << '*';
                    } 
                    else {
                        cout << ' ';
                    }
        		}
        	}
        	cout<<endl;
        }
        for(int iRow = 0; iRow < iSquareHeight; iRow++) {
        	cout<<"\t\t   ";
        	if(iRow % 3 == 1) {
        		cout<<"-----";
        		for(int iColumn = 0; iColumn < 8*iSquareWidth; iColumn++) {
        			int iSquareColumn = iColumn / iSquareWidth;
        			if((iColumn % 6) == 3) {
        				cout<<"0";
        			}
        			else if((iColumn % 6) == 4) {
        				cout<<(iSquareColumn + 1);
        			}
        			else {
        				cout<<"-";
        			}
        		}
        	}
        	else {
        		for(int iColumn = 1; iColumn < 9*iSquareWidth; iColumn++) {
        			cout<<"-";
        		}
        	}
        	cout<<"-"<<endl;
        }
	}

	bool isInCheck(char pieceColor) {
		int KingRow;
        int KingColumn;
        for(int iRow = 0; iRow < 8; iRow++) {
            for(int iColumn = 0; iColumn < 8; iColumn++) {
                if(mainGameBoard[iRow][iColumn] != 0) {
                    if(mainGameBoard[iRow][iColumn]->getPieceColor() == pieceColor) {
                        if(mainGameBoard[iRow][iColumn]->getPieceType() == 'K') {
                            KingRow = iRow;
                            KingColumn = iColumn;
                        }
                    }
                }
            }
        }
        for(int iRow = 0; iRow < 8; iRow++) {
            for(int iColumn = 0; iColumn < 8; iColumn++) {
                if(mainGameBoard[iRow][iColumn] != 0) {
                    if(mainGameBoard[iRow][iColumn]->getPieceColor() != pieceColor) {
                        if(mainGameBoard[iRow][iColumn]->isLegalMove(iRow, iColumn, KingRow, KingColumn, mainGameBoard)) {
                            return true;
                        }
                    }
                }
            }
        }
		return false;
	}

	bool canMove(char pieceColor) {
		for(int iRow = 0; iRow < 8; iRow++) {
            for(int iColumn = 0; iColumn < 8; iColumn++) {
                if(mainGameBoard[iRow][iColumn] != 0) {
                    if(mainGameBoard[iRow][iColumn]->getPieceColor() == pieceColor) {
                        for(int iMoveRow = 0; iMoveRow < 8; ++iMoveRow) {
                            for(int iMoveCol = 0; iMoveCol < 8; ++iMoveCol) {
                                if(mainGameBoard[iRow][iColumn]->isLegalMove(iRow, iColumn, iMoveRow, iMoveCol, mainGameBoard)) {
                                    Piece* temp = mainGameBoard[iMoveRow][iMoveCol];
                                    mainGameBoard[iMoveRow][iMoveCol] = mainGameBoard[iRow][iColumn];
                                    mainGameBoard[iRow][iColumn] = 0;
                                    bool beCanMove = !isInCheck(pieceColor);  
                                    mainGameBoard[iRow][iColumn] = mainGameBoard[iMoveRow][iMoveCol];
                                    mainGameBoard[iMoveRow][iMoveCol] = temp;
                                    if(beCanMove) {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
		return false;
	}
};

class ChessBoard : public Game {
private:
	CreateBoard newGameBoard;
	char playerTurn;
public:
	ChessBoard() : playerTurn('W') {}
	
	~ChessBoard() {}
	
	void start() {
		cout<<endl<<"\t\t\t\t     Welcome to Chess Game"<<endl;
		cout<<endl<<"\t\t\t\t             Rules        "<<endl<<endl;
			  cout<<"\t\t\t\t       *       -  white space"<<endl<<endl;
        	  cout<<"\t\t\t\t  Blank space  -  black space"<<endl<<endl;
        	  cout<<"\t\t\t    WP - White pawn      BP - Black pawn"<<endl;
        	  cout<<"\t\t\t    WN - White Knight    BN - Black Knight"<<endl;
        	  cout<<"\t\t\t    WB - White Bishop    BB - Black Bishop"<<endl;
        	  cout<<"\t\t\t    WR - White Rook      BR - Black Rook"<<endl;
        	  cout<<"\t\t\t    WQ - White Queen     BQ - Black Queen"<<endl;
        	  cout<<"\t\t\t    WK - White King      BK - Black King"<<endl;
        cout<<endl<<"\tMove by selecting row & column to another valid location using row & column"<<endl<<endl;
		do {
			getMove(newGameBoard.mainGameBoard);
			alternateTurn();
		}while(!gameOver());
		newGameBoard.print();
		if(!whitePieces.empty()) {
			cout<<endl;
			cout<<"\tBlack Score: "<<blackScore<<endl;
			cout<<"\tWhite Pieces acquired bu Black: ";
			printWhitePieces();
			cout<<endl;
		}
		if(!blackPieces.empty()) {
			cout<<endl;
			cout<<"\twhiteScore: "<<whiteScore<<endl;
			cout<<"\tBlack Pieces acquired by white: ";
			printBlackPieces();
			cout<<endl;
		}
	}

	void getMove(Piece *gameBoard[8][8]) {
		bool beValidMove = false;
		do {
			newGameBoard.print();
			int startMove, endMove;
			cout<<endl;
			cout<<"\t"<<playerTurn<<"'s Move: ";
			cin>>startMove;
			int startRow = (startMove / 10) - 1;
			int startColumn = (startMove % 10) - 1;
			cout<<"\tTo: ";
			cin>>endMove;
			int endRow = (endMove / 10) - 1;
			int endColumn = (endMove % 10) - 1;
			cout<<endl;
			if((startRow >= 0 && startRow <= 7) && (startColumn >= 0 && startColumn <= 7) && (endRow >= 0 && endRow <= 7) && (endColumn >= 0 && endColumn <= 7)) {
				Piece *currentPiece = gameBoard[startRow][startColumn];
				if((currentPiece != 0) && (currentPiece->getPieceColor() == playerTurn)) {
					if(currentPiece->isLegalMove(startRow, startColumn, endRow, endColumn, gameBoard)) {
						Piece *temp = gameBoard[endRow][endColumn];
						gameBoard[endRow][endColumn] = gameBoard[startRow][startColumn];
						gameBoard[startRow][startColumn] = 0;
						if(!newGameBoard.isInCheck(playerTurn)) {
							if(temp != 0) {
								if(temp->getPieceColor() == 'W') {
									blackScore += score(temp->getPieceType());
									whitePieces.push(temp->getPieceType()); 
								}
								else {
									whiteScore += score(temp->getPieceType());
									blackPieces.push(temp->getPieceType());
								}
							}
							delete temp;
							beValidMove = true;
						}
						else {
							gameBoard[startRow][startColumn] = gameBoard[endRow][endColumn];
							gameBoard[endRow][endColumn] = temp;
						}
					}
				}
			}
			if(!beValidMove) {
				cout<<"\tInvalid Move!"<<endl<<endl;
			}
		} while(!beValidMove);
	}

	void alternateTurn() {
		playerTurn = (playerTurn == 'W') ? 'B' : 'W';
	}

	bool gameOver() {
		bool beCanMove(false);
        beCanMove = newGameBoard.canMove(playerTurn);
        if(!beCanMove) {
            if(newGameBoard.isInCheck(playerTurn)) {
                alternateTurn();
                winner = playerTurn;
            } 
            else{
                cout<<"Stalemate!"<<std::endl;
            }
        }
        return !beCanMove;
	}
};

class playerEntry {
public:
	struct details {
		string name, email, password;
		int wins = 0, scores = 0, gamesPlayed = 0;
	}d[1000], temp;

	void signUp() {
		cout<<"Name: ";
		cin>>d[count].name;
		cout<<"Email: ";
		cin>>d[count].email;
		cout<<"Password: ";
		cin>>d[count].password;
		count++;
	}

	int login(string name, string password) {
		for(int i = 0; i < count; i++) {
			if(!(this->d[i].name.compare(name)) && !(this->d[i].password.compare(password))) {
				return i;
			}
		}
		return -1;
	}

	void detail(int person) {
		cout<<"*****Details*****"<<endl;
		cout<<"Name: "<<d[person].name<<endl;
		cout<<"Wins: "<<d[person].wins<<endl;
		cout<<"Scores: "<<d[person].scores<<endl;
		cout<<"Games Played: "<<d[person].gamesPlayed<<endl;
	}

	void playGame(int p1, int p2) {
		ChessBoard newGame;
		newGame.start();
		if(newGame.winner == 'W') {
			d[p1].wins += 1;
			d[p1].scores += newGame.whiteScore;
		}
		else {
			d[p2].wins += 1;
			d[p2].scores += newGame.blackScore;
		}
        d[p1].gamesPlayed += 1;
        d[p2].gamesPlayed += 1;
	}

	friend void winnerLeaderBoard(playerEntry pe);
    friend void scoreLeaderBoard(playerEntry pe);

};

void winnerLeaderBoard(playerEntry pe) {
    for(int i = 0; i < count; i++) {
        for(int j = i + 1; j < count ; j++ ){
            if(pe.d[j].wins > pe.d[i].wins) {
                pe.temp = pe.d[i];
                pe.d[i] = pe.d[j];
                pe.d[j] = pe.temp;
            }
        }
    }
    for(int i = 0; i < count; i++) {
        cout<<i + 1<<". "<<pe.d[i].name<<" - "<<pe.d[i].wins<<endl;
    }
}

void scoreLeaderBoard(playerEntry pe) {
    for(int i = 0; i < count; i++) {
        for(int j = i + 1; j < count ; j++ ){
            if(pe.d[j].scores > pe.d[i].scores) {
                pe.temp = pe.d[i];
                pe.d[i] = pe.d[j];
                pe.d[j] = pe.temp;
            }
        }
    }
    for(int i = 0; i < count; i++) {
        cout<<i + 1<<". "<<pe.d[i].name<<" - "<<pe.d[i].scores<<endl;
    }
}


int main() {
	int ch, p1, p2;
	string name, email, password;
	playerEntry pe;
	do {
		cout<<"\n0. Exit\n1. Register\n2. Login\n3. New Match\n4. Winner Leader Board\n5. Score Leader Board\nchoice(0-5): ";
		cin>>ch;
		cout<<endl;
		switch(ch) {
			case 0:
				cout<<"Exit";
				break;
			case 1:
				cout<<"*****Register*****"<<endl;
				pe.signUp();
				break;
			case 2:
				cout<<"*****Login*****"<<endl;
				cout<<"Name: ";
				cin>>name;
				cout<<"Password: ";
				cin>>password;
				if(pe.login(name, password) == -1) {
					cout<<"Register!";
                    cout<<endl;
				}
				else {
					pe.detail(pe.login(name, password));
				}
				break;
			case 3:
				cout<<"*****New Match*****"<<endl;
				do {
					cout<<"White Player Login: "<<endl;
					cout<<"Name: ";
					cin>>name;
					cout<<"Password: ";
					cin>>password;
				}while(pe.login(name, password) == -1);
				p1 = pe.login(name, password);
				do {
					cout<<"Black Player Login: "<<endl;
					cout<<"Name: ";
					cin>>name;
					cout<<"Password: ";
					cin>>password;
				}while(pe.login(name, password) == -1);
				p2 = pe.login(name, password);
				pe.playGame(p1, p2);
				break;
			case 4:
				cout<<"*****Winner Leader Board*****"<<endl;
				winnerLeaderBoard(pe);
				break;
			case 5:
				cout<<"*****Score Leader Board*****"<<endl;
				scoreLeaderBoard(pe);
		}
	}while(ch != 0);
}
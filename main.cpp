#include <iostream>
#include <math.h>
#include <string>
#include <intrin.h>
#include <vector>

using namespace std;

#pragma intrinsic(_BitScanReverse)

/*Same idea as above but we're going to split the task in half*/
unsigned int msb32(unsigned long x)
{
	unsigned long index;
	return _BitScanReverse(&index, x) + 1;
}
/*Abstract directions as a series of choices on what? Our Block*/
class Choice{
private:
	int what;
	int selection;
public:
	Choice(int w,int s){
		what = w;
		selection = s;
	}
};

class Path{
private:
	vector<Choice> Selections;
public:
	Path(){
		Selections.clear();
	}
	void makeChoice(Choice a){
		Selections.push_back(a);
	}
	void undoChoice(){
		Selections.pop_back();
	}
};

class State{
private:
	long totalSteps;
	long currentState;
public:
	State(long steps,long data){
		totalSteps = steps;
		currentState = data; 
	}
	int step(int hops=0){
		totalSteps += hops;
		return totalSteps;
	}
	int operator == (const State &b) const
	{
		return b.currentState == currentState;
	}
	int operator != (const State &b) const
	{
		return b.currentState != currentState;
	}
	int operator > (const State &b) const
	{
		return b.totalSteps > totalSteps;
	}
	int operator < (const State &b) const
	{
		return b.totalSteps < totalSteps;
	}
	int operator <= (const State &b) const
	{
		if(b.currentState!=currentState){
			return -1;
		} else {
			return b.totalSteps <= totalSteps;
		}
	}
	int operator >= (const State &b) const
	{
	if(b.currentState!=currentState){
			return -1;
		} else {
			return b.totalSteps >= totalSteps;
		}
	}
};

class Piece{
private:
	int codeLength;
protected:
	int width;
	int height;
	int code;
public:
	int getCode(){
		return code;
	}
	int getWidth(){
		return width;
	}
	int getHeight(){
		return height;
	}
	int getCodeLength(){
		return codeLength;
	}
	void setCode(int c){
		code = c;
		codeLength = msb32(c); /*this'll be used a lot so we're memomlizing access*/
	}
	void setDimensions(int w,int h){
		width = w;
		height = h;
	}
	Piece(int c=0,int w=1,int h=1){
		setCode(c);
		setDimensions(w,h);
	}
};

vector<State> boardOverTime;
vector<Path> choicesOverTime;

class Board: public Piece{
private:
	vector<Piece> pieces;
	vector<int> placements;
	vector<Path> choices;
	vector< vector<int> > gameBoard;
	int index;
	int coordx;
	int coordy;
public:
	Board(int w=4,int h=4){
		moveIndex(0);
		pieces = vector<Piece>(0);/*w*h);*/
		width = w;
		height = h;
		placements = vector<int>(0);/*vector<int>(w*h,0);*/
		gameBoard = vector< vector<int> >(w,h);
		vector<int> blank(w,0);
		fill( gameBoard.begin() , gameBoard.end() , blank );
	}

	int moveIndex(int i){
		if(i < width*height){
			index = i;
			coordy = index / width;
			coordx = index % width;
			return 1;
		}
		return 0;
	}

	int moveIndex(int x,int y){
		if(x < width && y < height) {
			coordx = x;
			coordy = y;
			index = y*width + x;
			return 1;
		}
		return 0;
	}

	int getXfromIndex(int i){
		return i % width;
	}

	int getYfromIndex(int i){
		return i / width;
	}

	int pushPiece(Piece a){
		/*Is the board clear at this point?*/
		int w = a.getWidth();
		int h = a.getHeight();
		for(int x=0; x<w; x++){
			for(int y=0; y<h; y++){
				/*Nope...can't add this piece*/
				if(gameBoard[x+coordx][y+coordy] != 0){
					return 0;
				}
			}
		}
		placements.push_back(index);
		pieces.push_back(a);
		for(int x=0; x<w; x++){
			for(int y=0; y<h; y++){
				/*Checking off this section of the board*/
				gameBoard[x+coordx][y+coordy] = 1;
			}
		}
		/*We have to at least move over the index over by the width*/
		displayIndex();
		moveIndex(index+w);

		/*Make sure to find the next empty spot w/o walking off the edge*/
		while(gameBoard[coordx][coordy]!=0 && index<(width*height - 1)){
			moveIndex(index+1);
			displayIndex();
		}
		cout << "Now" << endl;
		displayIndex();
		return 1;
	}
	int countPieces(){
		return pieces.size();
	}
	void popPiece(){
		int lastIndex = placements.back();
		int delx = getXfromIndex(lastIndex);
		int dely = getYfromIndex(lastIndex);
		Piece lastPiece = pieces.back();
		int w = lastPiece.getWidth();
		int h = lastPiece.getHeight();
		for(int x=0; x<w; x++){
			for(int y=0; y<h; y++){
				/*Remove the piece from the game board*/
				gameBoard[x+delx][y+dely] = 0;
			}
		}
		pieces.pop_back();
		placements.pop_back();
	}
	void swap(int index, int index2){
		Piece temp = pieces[index];
		pieces[index] = pieces[index2];
		pieces[index2] = temp;
	}
	int manipulate(int index, int direction){
		/*00 up 01 right 11 down 10 left*/
		switch (direction){
			case 0:

				break;
			case 1:

				break;
			case 2:

				break;
			case 3:

				break;
			default:

				break;
		}
	}

	State save(){
		long code = 0;
		for each(Piece boardPiece in pieces){
			code += boardPiece.getCode();
			code << boardPiece.getCodeLength();
		}
		State nState(0,code);
		return nState;
	}

	void display(){
		for(int y=0;y<height;y++){
			for(int x=0;x<width;x++){
				cout << gameBoard[x][y];
			}
			cout << endl;
		}
	}
	void displayIndex(){
		cout << "[" << coordx << "][" << coordy << "]" << endl;
	}
	void displayPlacements(){
		int size = placements.size();
		for(int i=0; i<size; i++){
			cout << "[" << placements[i] << "]<" << pieces[i].getCode() << ">" << endl; 
		}
	}
};

int main(void)
{
	/* Empty */
	Piece A(5,1,1);
	/* Other */
	Piece B(8,2,2);
	Piece C(9,1,2);
	Piece D(3,1,1);
	Piece E(0,2,1);
	Board testBoard(5,4);
	/* Puzzle from PDF */
	
	cout << "Pushing E: " << testBoard.pushPiece(E) << endl;
	testBoard.display();

	cout << "Pushing E: " << testBoard.pushPiece(E) << endl;
	testBoard.display();

	cout << "Pushing D: " << testBoard.pushPiece(D) << endl;
	testBoard.display();

	cout << "Pushing B: " << testBoard.pushPiece(B) << endl;
	testBoard.display();

	cout << "Pushing C: " << testBoard.pushPiece(C) << endl;
	testBoard.display();

	cout << "Pushing D: " << testBoard.pushPiece(D) << endl;
	testBoard.display();

	cout << "Pushing A: " << testBoard.pushPiece(A) << endl;
	testBoard.display();

	cout << "Pushing D: " << testBoard.pushPiece(D) << endl;
	testBoard.display();

	cout << "Pushing A: " << testBoard.pushPiece(A) << endl;
	testBoard.display();

	cout << "Pushing E: " << testBoard.pushPiece(E) << endl;
	testBoard.display();

	cout << "Pushing E: " << testBoard.pushPiece(E) << endl;
	testBoard.display();

	cout << "Pushing D: " << testBoard.pushPiece(D) << endl;
	testBoard.display();
	testBoard.displayPlacements();

	while(testBoard.countPieces() > 0){
		cout << "Removing Last Piece" << endl;
		testBoard.popPiece();
		testBoard.display();
	}
	system("pause");
}
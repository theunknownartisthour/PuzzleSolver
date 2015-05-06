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

class FinancialAid{
private:

public:

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

class Board: public Piece{
private:
	vector<Piece> Pieces;
	vector<int> EffectiveIndex;
public:
	Board(){
		Pieces.clear();
		EffectiveIndex.clear();
		EffectiveIndex.push_back(0);
	}

	State save(){
		long code = 0;
		for each(Piece boardPiece in Pieces){
			code += boardPiece.getCode();
			code << boardPiece.getCodeLength();
		}
		State nState(0,code);
		return nState;
	}
	void pushPiece(Piece a){
		Pieces.push_back(a);
	}
	void popPiece(){
		Pieces.pop_back();
	}
	void swap(int index, int index2){
		Piece temp = Pieces[index];
		Pieces[index] = Pieces[index2];
		Pieces[index2] = temp;
	}
};

vector<State> boardOverTime;

int main(void)
{
	/* Empty */
	Piece A(5,1,1);
	/* Other */
	Piece B(8,4,4);
	Piece C(9,1,2);
	Piece D(3,1,1);
	Piece E(0,2,1);
	Board testBoard;
}
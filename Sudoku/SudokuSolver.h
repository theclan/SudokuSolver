#pragma once

#include <vector>
#include <utility> //std::pair
#include <set>
#include <stack>

typedef std::pair<int, int>				PAIROFINTS;
typedef std::vector<int>				ARRAYOFINTS;
typedef std::vector <ARRAYOFINTS>		DOUBLEDIMINTARRAY;
typedef std::set<int>					UNIQUEINTS;
typedef std::stack<PAIROFINTS>			BACKTRACKINGSTACK;

class SudokuSolver
{
public:
	virtual bool Read(const char* szFileName, char szDelimiter) = 0;
	virtual bool Solve() = 0;
	virtual bool Write(const char* szFileName) = 0;
	virtual void print(std::ostream &os) {};
	virtual ~SudokuSolver() {};

};
//Implements Sudoku using backtacking algorithm. Would work for 4x4 and 9x9 version of Sudoku
class SudokuBackTrackingSolver : public SudokuSolver
{
public:
	SudokuBackTrackingSolver(int);
	virtual ~SudokuBackTrackingSolver() {}
	//Read would read the csv input file and populate grid_ with initial values. 
	//Before calling Read, SudokuBackTrackingSolver would initialize all array with UNINITIALIZED(0) values 
	bool Read(const char* szFileName,char szDelimiter);
	//Solve would apply the algorithm to solve given Sudoku input
	bool Solve();
	//Write would write the values to a csv file. 
	bool Write(const char* szOutFileName);
	//Print is just for debugging purpose that would dump the values of grid_ array
	virtual void print(std::ostream &os);
private:
	SudokuBackTrackingSolver(); //This should be private. Caller must give a initial Dimension value 
private:
	//Set values with all the numbers 
	void SetAllAssignableValues(UNIQUEINTS &values);
	//Iterates to find unassigned cell in grid_
	bool GetUnAssignedCell(PAIROFINTS &currentIndex);
	//When we are assigning a number for any unassigned cell we would 
	//have to find set of unique numbers that are not in the row, column or in the box
	//e.g. FindUnusedNumbersInRow for C2 would return C1,C3,C4,C5,C7,C8 and C9
	//C2 | | C1 C2 C3 | C4 C5 C6 | C7 C8 C9
	void FindUnusedNumbersInRow(int currentValue, int currentRows, UNIQUEINTS &possibleValues);
	//e.g. FindUnusedNumbersInCol for C2 would return A2,B2,D2,E2,F2,G2,H2 and I2
	//A2 
	//B2 
	//C2 
	//D2 
	//E2 
	//F2 
	//G2 
	//H2 
	//I2 

	void FindUnusedNumbersInCol(int currentValue, int currentCols, UNIQUEINTS &possibleValues);
	//e.g. FindUnusedNumbersInBox for C2 would return all the below values other than C2
	//| A1 A2 A3 | |
	//| B1 B2 B3 | |
	//| C1 C2 C3 | |
	void FindUnusedNumbersInBox(int assignedValue, PAIROFINTS boxStartingIndex, UNIQUEINTS &possibleValues);
	//FindUnsuedNumbers would call FindUnusedNumbersInRow,FindUnusedNumbersInCol and FindUnusedNumbersInBox
	void FindUnsuedNumbers(const PAIROFINTS &currentIndex, UNIQUEINTS &values);
	bool IsIndexValid(const PAIROFINTS &currentIndex) const; 
	//HandleBackTracking is a helper method that would handle the backtracking logic when there is a conflict 
	//in assigned number. We would have to pop the last value from the stack and redo the iteration. 
	void HandleBackTracking(PAIROFINTS&, BACKTRACKINGSTACK&, UNIQUEINTS&);

private:
	DOUBLEDIMINTARRAY	grid_;
	int					dimension_;
};

inline bool  SudokuBackTrackingSolver::IsIndexValid(const PAIROFINTS &currentIndex) const
{
	return ((currentIndex.first >= 0   || currentIndex.first < dimension_) &&
		    (currentIndex.second >= 0  || currentIndex.second < dimension_)) ? true : false;

}
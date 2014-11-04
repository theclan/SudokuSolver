#include "stdafx.h"
#include "SudokuSolver.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
static const int						UNASSIGNED = 0;

SudokuBackTrackingSolver::SudokuBackTrackingSolver(int nSize) :dimension_(nSize)
{
	grid_.resize(dimension_, ARRAYOFINTS(dimension_,UNASSIGNED));
}
SudokuBackTrackingSolver::SudokuBackTrackingSolver() : dimension_(0)
{}

void SudokuBackTrackingSolver::print(std::ostream &os)
{
	if (os.good())
	{
		os << "-------------------------------------------------------------" << std::endl;
		for (int i = 0; i < dimension_; i++)
		{
			for (int j = 0; j < dimension_; j++)
			{
				os << grid_[i][j] << "|";
			}
			os << std::endl;
		}
		os << "-------------------------------------------------------------" << std::endl;
	}
}

bool SudokuBackTrackingSolver::Read(const char* szFileName,char szValueDelimiter)
{
	if (nullptr == szFileName) 
	{
		std::cout << "Error: Empty string passed as filename";
		return false;
	}
	std::ifstream istream(szFileName);
	if (istream.good())	
	{
		std::string		strLine;
		int				nRowIndex = 0;
		while (std::getline(istream, strLine) && 
			  nRowIndex < dimension_) 
		{
			//We got the current line
			int nColumnIndex = 0;
			std::stringstream lineStream(strLine);
			std::string		  strSingleValue;
			while (std::getline(lineStream, strSingleValue, szValueDelimiter) &&
				  nColumnIndex < dimension_)	
			{
				int value = atoi(strSingleValue.c_str());
				grid_[nRowIndex][nColumnIndex++] = value;
			}
			nRowIndex++;
		}
	}
	return true;
}

bool SudokuBackTrackingSolver::Write(const char* szOutFileName)
{
	if (nullptr == szOutFileName)
		return false;

	std::ofstream os(szOutFileName);
	if (os.good())
	{
		for (int i = 0; i < dimension_; i++)
		{
			for (int j = 0; j < dimension_-1; j++)
			{
				os << grid_[i][j] << ",";
			}
			os << grid_[i][dimension_ - 1];
			os << std::endl;
		}
	}

	return true;
}

void SudokuBackTrackingSolver::HandleBackTracking(PAIROFINTS &currentCell, BACKTRACKINGSTACK &btStack, 
																		UNIQUEINTS &possibleValues)
{
	//Pop the last value from the stack. Since in the last iteration we have already look at the number
	//before currentValue, we would removes those values from possibleValues and reiterate the grid_
	currentCell = btStack.top();
	auto currentValue = grid_[currentCell.first][currentCell.second];
	SetAllAssignableValues(possibleValues);
	auto iterCurrentValue = possibleValues.find(currentValue);
	if (iterCurrentValue != possibleValues.end())
	{
		possibleValues.erase(possibleValues.begin(), iterCurrentValue);
		possibleValues.erase(iterCurrentValue);
	}
	grid_[currentCell.first][currentCell.second] = UNASSIGNED;
	btStack.pop();
}

bool SudokuBackTrackingSolver::Solve()
{
	BACKTRACKINGSTACK btStack;
	auto currCordinate = std::make_pair<int, int>(0, 0);
	bool backTrack = false;
	UNIQUEINTS possibleValues;
	SetAllAssignableValues(possibleValues);

	while (backTrack || GetUnAssignedCell(currCordinate))
	{
		FindUnsuedNumbers(currCordinate, possibleValues);
		if (possibleValues.size() == 0)
		{
			if (btStack.empty())
			{
				std::cout << "In backtack mode we should not have an empty stack. Please check input \n";
				return false;
			}
			HandleBackTracking(currCordinate, btStack, possibleValues);
			backTrack = true;
		}
		else
		{
			auto firstValueInSet = possibleValues.begin();
			grid_[currCordinate.first][currCordinate.second] = *firstValueInSet;
			btStack.push(currCordinate);
			currCordinate = std::make_pair<int, int>(0, 0);
			possibleValues.clear();
			SetAllAssignableValues(possibleValues);
			backTrack = false;
		}
	}
	return true;
}

/////////////////////Private methods
bool SudokuBackTrackingSolver::GetUnAssignedCell(PAIROFINTS &currentIndex)
{
	for (int i = 0; i < dimension_; i++)	
	{
		for (int j = 0; j < dimension_; j++) 
		{
			auto currentValue = grid_[i][j];
			if (currentValue == UNASSIGNED) 
			{
				currentIndex = { i, j };
				//std::cout << "Found unassigned grid Row:" << currentIndex.first<<"Column:"<<currentIndex.second<<std::endl;
				return true;
			}
		}
	}
	std::cout << "Unable to find any unassigned cell in grid\n";
	return false;
}

void SudokuBackTrackingSolver::SetAllAssignableValues(UNIQUEINTS &assignableValues)
{
	for (int i = 1; i <= dimension_; i++)
	{
		assignableValues.emplace(i);
	}
}

void SudokuBackTrackingSolver::FindUnsuedNumbers(const PAIROFINTS &currentIndex, UNIQUEINTS &values)
{
	auto currentValue = grid_[currentIndex.first][currentIndex.second];
	auto iterCurrentValue = values.find(currentValue);
	if (iterCurrentValue != values.end())
		values.erase(iterCurrentValue);

	auto boxLimit = (int) std::sqrt(dimension_);
	//boxStartingIndex is the the boundary of the box. Since for a 9x9 the box boundaries are 3, I've used 
	//sqrt to calculate the square root. 
	auto boxStartingIndex = std::make_pair<int,int>(currentIndex.first - currentIndex.first % boxLimit,
												currentIndex.second - currentIndex.second % boxLimit);
	FindUnusedNumbersInBox(currentValue, boxStartingIndex, values);
	FindUnusedNumbersInRow(currentValue, currentIndex.first, values);
	FindUnusedNumbersInCol(currentValue, currentIndex.second, values);
}

void SudokuBackTrackingSolver::FindUnusedNumbersInRow(int currentValue, int currentRows, UNIQUEINTS &possibleValues)
{
	for (int i = 0; i < dimension_; i++)
	{
		auto value = grid_[currentRows][i];
		if (currentValue == value)
			continue;
		auto iterCurValue = possibleValues.find(value);
		if (iterCurValue != possibleValues.end())
			possibleValues.erase(iterCurValue);
	}
}
void SudokuBackTrackingSolver::FindUnusedNumbersInCol(int currentValue, int currentCols, UNIQUEINTS &possibleValues)
{
	for (int i = 0; i < dimension_; i++)
	{
		auto value = grid_[i][currentCols];
		if (currentValue == value)
			continue;
		auto iterCurValue = possibleValues.find(value);
		if (iterCurValue != possibleValues.end())
			possibleValues.erase(iterCurValue);
	}
}

void SudokuBackTrackingSolver::FindUnusedNumbersInBox(int assignedValue, PAIROFINTS boxStartingIndex, UNIQUEINTS &possibleValues)
{
	auto boxBoundary = static_cast<int>(std::sqrt(dimension_));
	for (int i = 0; i < boxBoundary; i++)
	{
		for (int j = 0; j < boxBoundary; j++)
		{
			auto currentValue = grid_[i + boxStartingIndex.first][j + boxStartingIndex.second];
			if (assignedValue != currentValue)
			{
				auto iterCurValue = possibleValues.find(currentValue);
				if (iterCurValue != possibleValues.end())
					possibleValues.erase(iterCurValue);
			}
		}
	}
}

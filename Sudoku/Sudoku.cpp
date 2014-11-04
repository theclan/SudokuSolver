// Sudoku.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SudokuSolver.h"
#include <memory>
#include <iostream>

void PrintUsage()
{
	std::cout << "Error in Usage. You would need to pass a input and output file" << std::endl;
	std::cout << "Sudoku.exe input.csv output.csv"<<std::endl;
}
int _tmain(int argc, char* argv[])
{
	if (argc < 3)
	{
		PrintUsage();
		return -1;
	}
	std::unique_ptr<SudokuSolver> spSudako(new SudokuBackTrackingSolver(9));
	if (spSudako->Read(argv[1],','))
	{ 
		spSudako->print(std::cout);
		if (spSudako->Solve())
		{
			std::cout << "Solved the puzzle !!! \n";
			spSudako->print(std::cout);
			if (spSudako->Write(argv[2]))
			{
				std::cout << "Written to the File:"<<argv[2] << std::endl;
			}
		}
	}
	return 0;
}


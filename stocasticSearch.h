/******************************************************************************

   PROJECT:			StocasticSearch
   FILE:			stocasticSearch.h
   AUTHOR:			UTKARSH DWIVEDI
   DATE:			1/03/2014
   DESCRIPTION:		Minimising y = x1^2 + x2^2 in [-10, 10]

******************************************************************************/

/*************************** OTHER INCLUDE FILES *****************************/

// Include Standard C/C++ Header Files Here
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

// Namspace declarations
using namespace std;

// Declare public variables here
typedef vector< vector<double> > list_vec;
typedef vector<double> double_vec;

int elements, generations;
unsigned int seedBase = 100;
double maximumX, minimumX, alpha, radius;

list_vec* X = new list_vec[1];
list_vec* Y = new list_vec[1];
list_vec* U = new list_vec[1];
list_vec* childProbability = new list_vec[1];

/*****************************************************************************/

//	Functional prototypes here
int GetGreatestInteger(double num);
double GetRandNumber(double range, double min);
void RandomVectorList(list_vec *vecList, int rows);
void InitalizeY(list_vec* y, list_vec* x);
void LowestY(list_vec* y, list_vec* u);
void GetChildrenProbalility(list_vec* u, list_vec* p);
list_vec* GetChildrenSet(list_vec* childP);
list_vec* GetChildPoint(list_vec* center, double radius, double alpha);
list_vec* GetRandomLinearCombination(list_vec* y, list_vec* x);
list_vec* GetRandomReInitalization(list_vec* x);
list_vec* GetBestSolution(list_vec* x, list_vec* rlc, list_vec* rri);

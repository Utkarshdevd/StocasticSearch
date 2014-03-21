/******************************************************************************

   PROJECT:			StocasticSearch
   FILE:			stocasticSearch.cpp
   AUTHOR:			UTKARSH DWIVEDI
   DATE:			1/03/2014
   DESCRIPTION:		Minimising y = x1^2 + x2^2 in [-10, 10]

******************************************************************************/

/*************************** OTHER INCLUDE FILES *****************************/

#include "stocasticSearch.h"

/*****************************************************************************/

int main(int argc, char *argv[])
{
	
	//Get number of elements and number of generations from parameter file
	// Opening the Parameter File for Reading
	FILE* fp;
	if(argc==2)
	{
		fp = fopen( argv[1] , "r" );
	}
	else
	{
		cout<<"Parameter file missing :: ./a.out [Filename]"<<endl;
		return 0;
	}
	
	// Reading the Parameter File : Search Parameters Information

	char paramNameString[100] , eqString[3] , headerString[100];
	fscanf( fp , "%s" , headerString );
	
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &elements );
	int generations;
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &generations );

	fscanf( fp , "%s%s%lf" , paramNameString , eqString , &maximumX );

	fscanf( fp , "%s%s%lf" , paramNameString , eqString , &minimumX );

	fscanf( fp , "%s%s%lf" , paramNameString , eqString , &alpha );
	
	fscanf( fp , "%s%s%lf" , paramNameString , eqString , &radius );
		
	// Closing Parameter File
	fclose( fp );

	//initalize number of vectors
	X->resize(elements);
	Y->resize(elements);
	U->resize(elements);
	childProbability->resize(elements);

	//initalize random elements
	RandomVectorList(X, 2);
	RandomVectorList(Y, 1);
	RandomVectorList(U, 1);
	RandomVectorList(childProbability, 1);

	InitalizeY(Y, X);

	LowestY(Y, U);
	
	GetChildrenProbalility(U, childProbability);
	
	for(int i = 0; i<elements; i++)
	{
		cout<<(*X)[i][0]<<"  "<<(*X)[i][1]<<endl;
	}
	cout<<"------------Y------------"<<endl;
	for(int i = 0; i<elements; i++)
	{
		cout<< (*Y)[i][0] <<endl;
	}
	cout<<"------------U------------"<<endl;
	for(int i = 0; i<elements; i++)
	{
		cout<< (*U)[i][0] <<endl;
	}
	cout<<"------------childProbability------------"<<endl;
	for(int i = 0; i<elements; i++)
	{
		cout<< (*childProbability)[i][0] <<endl;
	}
	cout<<"------------childrenSet------------"<<endl;
	GetChildrenSet(childProbability);
	for(int i = 0; i<elements; i++)
	{
		cout<< (*childProbability)[i][0] <<endl;
	}

	return 0;
}

double GetRandNumber(double range, double min)
{
	srand(unsigned(time(NULL))%seedBase++);
	double num = (double)(((double)(rand()%1000)/1000)*range + min);

	return num;
}

int GetGreatestInteger(double num)
{
	int decimal = num - (int)num;
	
	if(decimal >= 0.5)
	{
		return num + 1;
	}
	
	return num;
}

void RandomVectorList(list_vec* vecList, int rows)
{
	double range = maximumX - minimumX;
	for(int xnum = 0; xnum<elements; xnum++)
	{
		(*vecList)[xnum].resize(rows);
		for (int rw = 0; rw<rows; rw++)
		{
			(*vecList)[xnum][rw] = GetRandNumber(range, minimumX);
		}
	}
}

void InitalizeY(list_vec* y, list_vec* x)
{
	for(int xnum = 0; xnum<elements; xnum++)
	{
		(*y)[xnum][0] = -1*(*x)[xnum][0]*(*x)[xnum][0] - (*x)[xnum][1]*(*x)[xnum][1];
	}
}

void LowestY(list_vec* y, list_vec* u)
{
	double lowest = 200;
	for(int index = 0; index<elements; index++)
	{
		if((*y)[index][0]<lowest)
		{
			lowest = (*y)[index][0];
		}
	}
	
	//Initalize U with Y - lowest(Y)	
	for(int index = 0; index<elements; index++)
	{
		(*u)[index][0] = (*y)[index][0] - lowest;
	}
}

void GetChildrenProbalility(list_vec* u, list_vec* p)
{
	double sumU = 0;
	
	//Summation of U
	for(int index = 0; index<elements; index++)
	{
		sumU += (*u)[index][0];
	}
	
	//Probability set
	for(int index = 0; index<elements; index++)
	{
		(*p)[index][0] = (*u)[index][0]/sumU;
	}
}
//==============adadad=====================================//
//======================sdasdsadsada=============================//
//====================================asdsadasdsad===============//
void GetChildrenSet(list_vec* childP)
{
	list_vec* childrenSet = new list_vec[1];
	
	//childP now has number of children
	for(int elem = 0; elem < elements; elem++)
	{
		(*childP)[elem][0] *= elements;
	}
	
	//now generate children and store in childrenSet
	for(int solNumber = 0; solNumber<childP->size(); solNumber++)
	{
		//for all children generate child(X1, X2), solution space is..
		//hypercircle of radius, alpha[0, 1], at center(X1, X2) of child
		list_vec* center = new list_vec[1];
		(*center)[0].resize(2);
		(*center)[0][0] = (*X)[solNumber][0];
		(*center)[0][1] = (*X)[solNumber][1];
		
		list_vec* childVector = new list_vec[1];
		(*childVector)[0].resize(2);
		
		for(int child = 0; child<GetGreatestInteger((*childP)[solNumber][0]); child++)
		{
			alpha = GetRandNumber(1, 0);
			childVector = GetChildPoint(center, radius, alpha);
		}
	}
}

list_vec* GetChildPoint(list_vec* center, double radius, double alpha)
{
	//get random unit vector
	list_vec* unitVector = new list_vec[1];
	(*unitVector)[0].resize(2);
	(*unitVector)[0][0] = GetRandNumber(1, 0);
	(*unitVector)[0][1] = sqrt(1 - (*unitVector)[0][0] * (*unitVector)[0][0]);
	
	
	//get new vector, adding center to alpha*radius*unitVector
	list_vec* childVector = new list_vec[1];
	(*childVector)[0].resize(2);
	
	(*childVector)[0][0] = (*center)[0][0] + alpha*radius*(*unitVector)[0][0];
	(*childVector)[0][1] = (*center)[0][1] + alpha*radius*(*unitVector)[0][1];
	return childVector;
}

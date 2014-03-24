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

	for(int genNo = 0; genNo<generations; genNo++)
	{
		cout<<"=============="<<"GEN : "<<genNo<<"=============="<<endl;
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
		list_vec* childrenSet = GetChildrenSet(childProbability);
		for(int i = 0; i<childrenSet->size(); i++)
		{
			cout<< (*childrenSet)[i][0] <<" "<<(*childrenSet)[i][1]<<" >>>-"<<(*childrenSet)[i][0]*(*childrenSet)[i][0] + (*childrenSet)[i]	[1]*(*childrenSet)[i][1]<<endl;
		}
		cout<<"------------RLC Set------------"<<endl;
		list_vec* solutionRLC = GetRandomLinearCombination(Y, X);
		for(int i = 0; i<solutionRLC->size(); i++)
		{
			cout<< (*solutionRLC)[i][0] <<" "<<(*solutionRLC)[i][1]<<" >>>-"<<(*solutionRLC)[i][0]*(*solutionRLC)[i][0] + (*solutionRLC)[i][1]*(*solutionRLC)[i][1]<<endl;
		}
		cout<<"------------RRI Set------------"<<endl;
		list_vec* solutionRRI = GetRandomReInitalization(X);
		for(int i = 0; i<solutionRLC->size(); i++)
		{
			cout<< (*solutionRRI)[i][0] <<" "<<(*solutionRRI)[i][1]<<" >>>-"<<(*solutionRRI)[i][0]*(*solutionRRI)[i][0] + (*solutionRRI)[i][1]*(*solutionRRI)[i][1]<<endl;
		}
		cout<<"------------Best Solution Set------------"<<endl;
		X = GetBestSolution(childrenSet, solutionRLC, solutionRRI);
		for(int i = 0; i<X->size(); i++)
		{
			cout<< (*X)[i][0] <<" "<<(*X)[i][1]<<" >>>-"<<(*X)[i][0]*(*X)[i][0] + (*X)[i][1]*(*X)[i][1]<<endl;
		}
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
	double decimal = num - (int)num;

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

list_vec* GetChildrenSet(list_vec* childP)
{
	list_vec* childrenSet = new list_vec[1];
	int size = 1;
	
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
		center->resize(1);
		(*center)[0].resize(2);
		(*center)[0][0] = (*X)[solNumber][0];
		(*center)[0][1] = (*X)[solNumber][1];
		
		list_vec* childVector = new list_vec[1];
		childVector->resize(1);
		(*childVector)[0].resize(2);
		int maxChildren = GetGreatestInteger((*childP)[solNumber][0]);
		//cout<<maxChildren<<" "<<solNumber<<endl;
		for(int child = 0; child<maxChildren; child++)
		{
			alpha = GetRandNumber(1, 0);
			childVector = GetChildPoint(center, radius, alpha);
			childrenSet->resize(size);
			(*childrenSet)[size - 1].resize(2);
			(*childrenSet)[size-1][0] = (*childVector)[0][0];
			(*childrenSet)[size-1][1] = (*childVector)[0][1];
			size++;
		}
	}
	
	return childrenSet;
}

list_vec* GetChildPoint(list_vec* center, double radius, double alpha)
{
	//get random unit vector
	list_vec* unitVector = new list_vec[1];
	unitVector->resize(1);
	(*unitVector)[0].resize(2);
	(*unitVector)[0][0] = GetRandNumber(1, 0);
	(*unitVector)[0][1] = sqrt(1 - (*unitVector)[0][0] * (*unitVector)[0][0]);
	
	
	//get new vector, adding center to alpha*radius*unitVector
	list_vec* childVector = new list_vec[1];
	childVector->resize(1);
	(*childVector)[0].resize(2);
	
	(*childVector)[0][0] = (*center)[0][0] + alpha*radius*(*unitVector)[0][0];
	(*childVector)[0][1] = (*center)[0][1] + alpha*radius*(*unitVector)[0][1];
	return childVector;
}

list_vec* GetRandomLinearCombination(list_vec* y, list_vec* x)
{
	//find max y and min y positions
	double min = 201, max = -201;
	int maxPos, minPos;
	for(int pos = 0; pos<y->size(); pos++)
	{
		if((*y)[pos][0]>max)
		{
			max = (*y)[pos][0];
			maxPos = pos;
		}
		
		if((*y)[pos][0]<min)
		{
			min = (*y)[pos][0];
			minPos = pos;
		}
	}
	
	//get S_rlc
	list_vec* solutionRLC = new list_vec[1];
	solutionRLC->resize(x->size());
	
	double alpha = 0;
	for(int pos = 0; pos<x->size(); pos++)
	{
		alpha = GetRandNumber(1, 0);
		(*solutionRLC)[pos].resize(2);
		(*solutionRLC)[pos][0] = alpha * (*x)[minPos][0] + (1 - alpha) * (*x)[maxPos][0];
		(*solutionRLC)[pos][1] = alpha * (*x)[minPos][1] + (1 - alpha) * (*x)[maxPos][1];
	}
	
	return solutionRLC;
}

list_vec* GetRandomReInitalization(list_vec* x)
{
	list_vec* solutionRRI = new list_vec[1];
	solutionRRI->resize(x->size());
	
	RandomVectorList(solutionRRI, (*x)[0].size());
	return solutionRRI;
}

list_vec* GetBestSolution(list_vec* x, list_vec* rlc, list_vec* rri)
{
	double max = -201, saveY;
	
	int pos = 0, saveI = 0;
	
	list_vec* bestSolution = new list_vec[1];
	bestSolution->resize(elements);
	
	list_vec* combinedSolution = new list_vec[1];
	combinedSolution->resize(x->size() + rlc->size() + rri->size());

	for(int i = 0; i<combinedSolution->size(); i++)
	{
		(*combinedSolution)[i].resize(2);
		if(i < x->size())
		{
			(*combinedSolution)[i][0] = (*x)[i][0];
			(*combinedSolution)[i][1] = (*x)[i][1];
		}
		else if(i < rlc->size() + x->size())
		{
			(*combinedSolution)[i][0] = (*rlc)[i - x->size()][0];
			(*combinedSolution)[i][1] = (*rlc)[i - x->size()][1];
		}
		else
		{
			(*combinedSolution)[i][0] = (*rri)[i - x->size() - rlc->size()][0];
			(*combinedSolution)[i][1] = (*rri)[i - x->size() - rlc->size()][1];
		}
	}
	
	for(int pos = 0; pos<elements; pos++)
	{
		max = -201;
		for(int i = 0; i<combinedSolution->size(); i++)
		{
			saveY = -1*((*combinedSolution)[i][0]*(*combinedSolution)[i][0] + (*combinedSolution)[i][1]*(*combinedSolution)[i][1]);
		
			if(max < saveY)
			{
				saveI = i;
				max = saveY;
			}
		}
	
		(*bestSolution)[pos].resize(2);
		(*bestSolution)[pos][0] = (*combinedSolution)[saveI][0];
		(*bestSolution)[pos][1] = (*combinedSolution)[saveI][1];
		(*combinedSolution)[saveI][0] = -10.1;
		(*combinedSolution)[saveI][1] = -10.1;	
	}
	
	return bestSolution;
}

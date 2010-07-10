/*
 *  Delta programming language
 */

#include "delta/delta.h"

/*
 * Levenshtein Distance Algorithm: C Implementation
 *   by Lorenzo Seidenari (sixmoney@virgilio.it)
 */

/*****************************************************/
/*Function prototypes and libraries needed to compile*/
/*****************************************************/

#include <stdlib.h>
#include <string.h>
int levenshtein_distance(char *s,char*t);
int minimum(int a,int b,int c);

/****************************************/
/*Implementation of Levenshtein distance*/
/****************************************/

int levenshtein_distance(char *s,char*t)
/*Compute levenshtein distance between s and t*/
{
	//Step 1
	int k,i,j,n,m,cost,*d,distance;
	n=strlen(s); 
	m=strlen(t);
	if(n!=0&&m!=0)
	{
		d=malloc((sizeof(int))*(m+1)*(n+1));
		m++;
		n++;
		//Step 2	
		for(k=0;k<n;k++)
			d[k]=k;
		for(k=0;k<m;k++)
			d[k*n]=k;
		//Step 3 and 4	
		for(i=1;i<n;i++)
			for(j=1;j<m;j++)
			{
				//Step 5
				if(s[i-1]==t[j-1])
					cost=0;
				else
					cost=1;
				//Step 6			 
				d[j*n+i]=minimum(d[(j-1)*n+i]+1,d[j*n+i-1]+1,d[(j-1)*n+i-1]+cost);
			}
		distance=d[n*m-1];
		free(d);
		return distance;
	}
	else 
		return -1; //a negative return value means that one or both strings are empty.
}


int minimum(int a,int b,int c)
/*Gets the minimum of three values*/
{
	int min=a;
	if(b<min)
		min=b;
	if(c<min)
		min=c;
	return min;
}


/**
 * @brief Calculate Levenshtein distance between two strings.
 * @syntax int levenshtein ( string str1 , string str2 )
 *
 * The Levenshtein distance is defined as the minimal number of characters you have to replace,
 * insert or delete to transform str1 into str2. The complexity of the algorithm is O(m*n), where n
 * and m are the length of str1 and str2 (rather good when compared to similar_text(), which is
 * O(max(n,m)**3), but still expensive).
 *
 * @param str1 One of the strings being evaluated for Levenshtein distance.
 * @param str2 One of the strings being evaluated for Levenshtein distance.
 * @retrun This function returns the Levenshtein-Distance between the two argument strings or -1,
 *         if one of the argument strings is longer than the limit of 255 characters.
 */
DELTA_FUNCTION(levenshtein)
{
	int release0, release1;
	struct DeltaVariable *arg0 = delta_cast_string(DELTA_ARG0, &release0);
	struct DeltaVariable *arg1 = delta_cast_string(DELTA_ARG1, &release1);
	
	int r = levenshtein_distance(arg0->value.ptr, arg1->value.ptr);
	
	DELTA_RELEASE(release0, arg0);
	DELTA_RELEASE(release1, arg1);
	DELTA_RETURN_NUMBER(r);
}

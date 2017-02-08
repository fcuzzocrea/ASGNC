#include "tmwtypes.h"

void UpdateBuffer(real_T* rptr, int_T* iptr, real_T* u)
{
	rptr[iptr[3]] = u[0] ;			
	iptr[2] = iptr[3] ;

	iptr[3]++ ;
	if(iptr[3] == iptr[0])
		iptr[3] = 0 ;	
}
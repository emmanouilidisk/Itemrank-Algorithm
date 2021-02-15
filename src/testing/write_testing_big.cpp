#include <iostream>
#include <fstream>

#include "ac_fixed.h"

using namespace std;

int limit=1000;

int main()
{
	std::ofstream ofs ("test_cases.txt", std::ofstream::out);
	
	ofs << "m ";
	ofs << limit;
	ofs << "\n";
    	
	/* //two movies
	for(int k=0; k=5; k++)
	{
	for(int i=0; i<limit; i++)
	{
		for(int j=0; j<limit; j++)
		{
			if(i==j)
			{
				table[i][j]=0;
				ofs << table[i][j];
				ofs <<" ";	
			}
			else if((i==80+k && j==90+k) || (i==90+k && j==80+k))
			{
				table[i][j]=100/(100+(limit-2));
				ofs << table[i][j];
				ofs <<" ";
			}
			else
			{
				table[i][j]=1/(100+(limit-2));
				ofs << table[i][j];
				ofs <<" ";
			}
		}
	}
	}
	*/
	
	// four movies
	for(int k=0; k<1; k++)
	{
		for(int i=0; i<limit; i++)
	{
		for(int j=0; j<limit; j++)
		{
			if(i==j)
			{
				ofs << 0.0;
				ofs <<" ";	
			}
			else if((i==20+k && j==30+k) || (i==30+k && j==20+k) || (i==80+k && j==90+k) || (i==90+k && j==80+k))
			{
				ofs << 1000.0/(1000.0+(limit-2));
				ofs <<" ";
			}
			else
			{
				ofs << 1.0/(1000.0+(limit-2));
				ofs <<" ";
			}
		}
	}
	}
	
	//write critics
	ofs << "\n\n";
	
	for(int i=0; i<limit; i++)
	{
		for(int j=0; j<4; j++)
		{
			if(i==21 || i==20)
			{
				ofs << 0.0 ;
				ofs << " ";
				
			}
			else
			{
				ofs << 1.0/((limit-2));
				ofs << " ";
			}
		}
	}		
	
	
	ofs.close();
	return 0;
}

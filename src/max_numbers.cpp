#include <iostream>

using namespace std;


int main()
{
	
	int movies_correlation[4][4] = {{1,2,3,4},{15,12,13,11},{25,1,2,11},{11,14,7,3}};
	int new_movies_correlation[4][4] = {0};
	
	int m=4;
	int n=4;
	int temp = 0;
	
	//max numbers to find
	int max_numbers = 4;
								
	for(int w=0; w<m; w++)
    	{
    		for(int e=0; e<n; e++)
    		{
    			temp = movies_correlation[w][e];
    			
    			for(int b=0; b<max_numbers; b++)
    			{
    				
    				if(temp>new_movies_correlation[w][b])
    				{
    					for(int s=max_numbers-1; s>=b+1; s--)
    					{
    						new_movies_correlation[w][s] = new_movies_correlation[w][s-1];
						}
    					new_movies_correlation[w][b] = temp;
						
						break;	   						
					}
				}
			} 		
		}
	
	
	
	for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            cout << new_movies_correlation[i][j] << " ";
        }
        cout << "\n";
    }
		
	
	return 0;
}

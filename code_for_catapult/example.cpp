#include "itemrank.h"

#include "mc_scverify.h"


CCS_MAIN(int argc, char* argv[]){
//int main(){

/*
	// define arrays used in program
     data_type movies_correlation[m][m]= { // denoted in the paper as C
	        {   0.0,  1.0/9, 2.0/14, 1.0/14, 3.0/19, 4.0/18, 1.0/13.0, 2.0/15.0},
	        {1.0/14,    0.0, 1.0/14, 1.0/14, 1.0/19, 1.0/18, 1.0/13.0, 3.0/15.0},
	        {2.0/14,  1.0/9,    0.0, 2.0/14, 3.0/19, 2.0/18, 2.0/13.0, 2.0/15.0},
	        {1.0/14,  1.0/9, 2.0/14,    0.0, 3.0/19, 4.0/18, 1.0/13.0, 2.0/15.0},
	        {3.0/14,  1.0/9, 3.0/14, 3.0/14,    0.0, 3.0/18, 3.0/13.0, 3.0/15.0},
	        {4.0/14,  1.0/9, 2.0/14, 4.0/14, 3.0/19,    0.0, 3.0/13.0, 1.0/15.0},
	        {1.0/14,  1.0/9, 2.0/14, 1.0/14, 3.0/19, 3.0/18, 0.0/13.0, 2.0/15.0},
	        {2.0/14,  3.0/9, 2.0/14, 2.0/14, 3.0/19, 1.0/18, 2.0/13.0, 0.0/15.0},
	};
	//normalized movies ratings
	data_type initial_critics[n][m] = { //array with columns the vector d of each user
	        {1.0/7, 1.0/7,   4.0/7, 0.0/7, 0.0/7, 1.0/7,  0.0/7, 0.0/7},
	        {5.0/12, 0.0/12, 5.0/12, 0.0/12, 1.0/12, 0.0/12, 1.0/12, 0.0/12},
	        {0/15, 4.0/15, 3.0/15, 1.0/15, 1.0/15, 1.0/15, 2.0/15, 0.0/15},
	        {4.0/11, 1.0/11, 1.0/11, 3.0/11, 0.0/11, 2.0/11, 0.0/11, 0.0/11},
	        {3.0/7, 1.0/7, 0.0/7, 4.0/7, 4.0/7, 0.0/7, 4.0/7, 4.0/7},
	        {5.0/12, 0.0/12, 5.0/12, 0.0/12, 1.0/12, 0.0/12, 1.0/12, 0.0/12},
	        {4/15, 0/15, 3.0/15, 1.0/15, 1.0/15, 4.0/15, 2.0/15, 0.0/15},
	        {4.0/11, 0.0/11, 1.0/11, 3.0/11, 5.0/11, 0.0/11, 5.0/11, 5.0/11},
	};  
	
	*/
	
	//declare matrices
	static data_type movies_correlation[m][m];
	static data_type initial_critics[n][m];
	
	// for movies
	for(int i=0; i<m; i++){
		for(int j=0; j<m; j++){
			if(i==j){
				movies_correlation[i][j] = 0.0;	
			}
			else if(i==20 && j!=20)
			{
				movies_correlation[i][j] = 1000.0/(1000.0+(m-2));
			}			
			else if(j==20)
			{
				movies_correlation[i][j] =  1000.0/((m-1)*1000.0);
			}			
			else
			{
				movies_correlation[i][j] = 1.0/(1000.0+(m-2));
			}
		}
	}
	
	//write critics
	
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<m; j++)
		{
			if(j==20 || j==5)
			{
				initial_critics[i][j] = 0.0 ;				
			}
			else
			{
				initial_critics[i][j] = 1.0/((m-2));
			}
		}
	}	
	
	//declare variables
	compressed_data_type sparce_movies[m][num_elements];
	int sparce_movies_index[m][num_elements], sparce_critics_index[m][n];
    int IR_new[n];
    //initialize 
    for(int i = 0; i < m; i++){
    	for(int j = 0; j < num_elements; j++){
    		sparce_movies[i][j] = 0;
    		sparce_movies_index[i][j] = 0;
    		sparce_critics_index[i][j] = 0;
		}
	}
	
    // create object and run Itemrank algorithm
    itemrank obj;
    obj.run(movies_correlation,initial_critics,IR_new,sparce_movies,sparce_movies_index,sparce_critics_index);
    
    
    // print results
    for(int j = 0; j < n; j++){
        std::cout << IR_new[j] << " ";
    }
 
   CCS_RETURN(0);
   //return(0);
}

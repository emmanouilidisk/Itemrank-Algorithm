#include "header.h"

#include "mc_scverify.h"


CCS_MAIN(int argc, char* argv[]){

/*	data_type **movies_correlation  = new data_type[m];
	for(int i = 0; i < m; ++i) {
    movies_correlation[i] = new data_type[m];
	}
	
	data_type **initial_critics  = new data_type[m];
	for(int i = 0; i < m; ++i) {
    movies_correlation[i] = new data_type[n];
	} */
	//commented this
	/*
	static data_type movies_correlation[m][m];
	static data_type initial_critics[m][n];	
	
	
	for(int l=0; l<m; l++)
	{
		for(int b=0; b<n; b++)
		{
			initial_critics[l][b] = (l+b+1)*0.001;
		}
		
		for(int b=0; b<m; b++)
		{
			movies_correlation[l][b] = (l+b+1)*0.001;
		}
	} 
//	for(int l=0; l<m; l++)
//	{
//		for(int b=0; b<n; b++)
//		{
//			std::cout << initial_critics[l][b] << " ";
//		}
//		std::cout << "\n";
//	} 
	
		
		for(int b=0; b<n; b++)
		{
			initial_critics[0][b] = 0;
			initial_critics[1][b] = 0;
		}
	
	*/
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
//	data_type initial_critics[m][n] = { //array with columns the vector d of each user
//	        {1.0/7, 5.0/12,   0/15, 4.0/11, 3.0/7, 5.0/12,  4/15, 4.0/11},
//	        {1.0/7, 0.0/12, 4.0/15, 1.0/11, 1.0/7, 0.0/12, 0/15, 0.0/11},
//	        {4.0/7, 5.0/12, 3.0/15, 1.0/11, 0.0/7, 5.0/12, 3.0/15, 1.0/11},
//	        {0.0/7, 0.0/12, 1.0/15, 3.0/11, 4.0/7, 0.0/12, 1.0/15, 3.0/11},
//	        {0.0/7, 1.0/12, 1.0/15, 0.0/11, 4.0/7, 1.0/12, 1.0/15, 5.0/11},
//	        {1.0/7, 0.0/12, 1.0/15, 2.0/11, 0.0/7, 0.0/12, 4.0/15, 0.0/11},
//	        {0.0/7, 1.0/12, 2.0/15, 0.0/11, 4.0/7, 1.0/12, 2.0/15, 5.0/11},
//	        {0.0/7, 0.0/12, 0.0/15, 0.0/11, 4.0/7, 0.0/12, 0.0/15, 5.0/11},
//	};  
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
	compressed_data_type sparce_movies[m][num_elements],sparce_critics[m][num_elements/8];
	int sparce_movies_index[m][num_elements], sparce_critics_index[m][n];
    int IR_new[n];
    
    for(int i = 0; i < m; i++){
    	for(int j = 0; j < num_elements; j++){
    		sparce_movies[i][j] = 0;
    		//sparce_critics[i][j] = 0;
    		sparce_movies_index[i][j] = 0;
    		sparce_critics_index[i][j] = 0;
		}
	}
    // create object and run Itemrank algorithm
    itemrank obj;
    obj.run(movies_correlation,initial_critics,IR_new,sparce_movies,sparce_critics,sparce_movies_index,sparce_critics_index);
    // print results
    for(int j = 0; j < n; j++){
        std::cout << IR_new[j] << " ";
    }
    
    
//    //tests for 128 bit
//	std::cout << "\n";
//	compressed_data_type temp_arr[num_elements];
//	 //temp_arr[0] = 0;
//	data_type s[8] = {   1.0,  0.25, 1.0,0.5, 0.5,  0.25, 1.0,0.5, };
//	for(int i=0; i<compressed_size; i++){
//		for(int l=0; l<8; l++){
//			for(int j=total_num_digits-1; j>=0; j--){
//				temp_arr[i][(16*l)+j] = s[l][j];
//				std::cout << temp_arr[i][(16*l)+j] << " ";
//			}
//			for(int v = l*total_num_digits-1; v>=0; v--){
//			
//				std::cout << temp_arr[i][v] << " ";
//			}
//			std::cout << temp_arr[i] << "\n";
//		}
//
//			
//		for(int l=0; l<8; l++){
//			for(int j=total_num_digits-1; j>=0; j--){
//				
//				s[l][j] = temp_arr[i][(16*l)+j] ;
//			}
//			std::cout << s[l];
//			std::cout << "\n";
//		}
//	}

   CCS_RETURN(0);
}

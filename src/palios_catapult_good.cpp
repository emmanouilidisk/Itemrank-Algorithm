#include <iostream>
#include "mc_scverify.h"
#include "ac_int.h"
#include "ac_fixed.h"

const int n=8; // number of users
const int m=100; // number of movies
const int digits = 1; //number of digits of integer part
const int total_num_digits = 16; //total number of digits
typedef ac_fixed<total_num_digits,digits,false> data_type;


#pragma hls_design top
class itemrank{
private:
    int max_iter;
    data_type a;
    data_type summation;
    data_type IR_col[m];
    data_type IR_col_new[m];
public:
    // constructor
    itemrank() {
    	max_iter=20;
    	a=0.85;
    	summation=0;  
    };

	#pragma hls_design interface
    void CCS_BLOCK(run) (data_type movies_correlation[m][m],data_type initial_critics[m][n],data_type IR_new[m][n]){
		
         USER:for(int user = 0; user < n; user++) { //for each user
        	// initialize IR
            IR_COL_INIT:for (int j = 0; j < n; j++) {
                IR_col[j] = 1.0/m;
            }

            TIME:for (int t = 0; t < max_iter; t++) { //for each time
            
                MOVIE:for(int i = 0; i < m; i++) { //for each movie
                    summation = 0;
                    
                    INNER_PRODUCT: for(int j = 0; j < m; j++) {
                        summation = summation + movies_correlation[i][j] * IR_col[j];
                    }
                    IR_col_new[i] = a * summation + (1-a)*initial_critics[i][user]; //rating of i-th movie of user
                }
                
                UPDATE_IR:for(int j = 0; j < m; j++) {
                	IR_col[j] = IR_col_new[j];	
                }
            }
            // write to output
            WRITE_OUTPUT:for(int i = 0; i < m; i++){
                IR_new[i][user] = IR_col[i];
            }
        }
    }
};


CCS_MAIN(int argc, char* argv[]){

	data_type IR_new[m][n];
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
	/*
	// define arrays used in program
    ac_fixed<total_num_digits,digits,false> IR_old[m][n], IR_new[m][n];
    ac_fixed<total_num_digits,digits,false> movies_correlation[m][m]= { // denoted in the paper as C
	        {   0.0,  1.0/9, 2.0/14, 1.0/15, 3.0/25, 4.0/17, 1.0, 2.0},
	        {1.0/14,    0.0, 1.0/14, 1.0/15, 1.0/25, 1.0/17, 1.0, 3.0},
	        {2.0/14,  1.0/9,    0.0, 2.0/15, 3.0/25, 2.0/17, 2.0, 2.0},
	        {1.0/14,  1.0/9, 2.0/14,    0.0, 3.0/25, 4.0/17, 1.0, 2.0},
	        {3.0/14,  1.0/9, 3.0/14, 3.0/15,    0.0, 3.0/17, 7.0, 3.0},
	        {4.0/14,  1.0/9, 2.0/14, 4.0/15, 3.0/25,    0.0, 1.0, 1.0},
	        {1.0/14,  1.0/9, 2.0/14, 1.0/15, 7.0/25, 1.0/17,   0, 2.0},
	        {2.0/14,  3.0/9, 2.0/14, 2.0/15, 3.0/25, 1.0/17, 2.0,   0},
	};
	//normalized movies ratings
    ac_fixed<total_num_digits,digits,false> initial_critics[m][n]= { //array with columns the vector d of each user
	        {1.0/7, 5.0/12,   0/14, 4.0/11, 1.0/7, 5.0/10,   0/9, 4.0/11},
	        {1.0/7, 0.0/12, 4.0/14, 1.0/11, 1.0/7, 0.0/10, 4.0/9, 1.0/11},
	        {4.0/7, 5.0/12, 3.0/14, 1.0/11, 0.0/7, 3.0/10, 1.0/9, 1.0/11},
	        {0.0/7, 0.0/12, 1.0/14, 0.0/11, 1.0/7, 0.0/10, 1.0/9, 1.0/11},
	        {0.0/7, 1.0/12, 1.0/14, 3.0/11, 0.0/7, 1.0/10, 1.0/9, 1.0/11},
	        {1.0/7, 0.0/12, 4.0/14, 2.0/11, 1.0/7, 0.0/10, 0.0/9, 1.0/11},
	        {0.0/7, 1.0/12, 2.0/14, 0.0/11, 2.0/7, 1.0/10, 2.0/9, 1.0/11},
	        {0.0/7, 0.0/12, 0.0/14, 0.0/11, 1.0/7, 0.0/10, 0.0/9, 1.0/11},
	};
  */
	
    // create object and run Itemrank algorithm
    itemrank obj;
    obj.run(movies_correlation,initial_critics,IR_new);

    // print results
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            std::cout << IR_new[i][j] << " ";
        }
        std::cout << "\n";
    }


   CCS_RETURN(0);
}

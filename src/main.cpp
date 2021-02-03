#include <iostream>
//#include "mc_scverify.h"
#include "ac_int.h"
#include "ac_fixed.h"

const int n=8; // number of users
const int m=8; // number of movies
const int digits = 1; //number of digits of integer part
const int total_num_digits = 16; //total number of digits
typedef ac_fixed<total_num_digits,digits,false> data_type;

class inner_product_2users{
	private:
		data_type summation1_u1, summation2_u1, summation1_u2, summation2_u2;
	public:
		inner_product_2users(){
			summation1_u1 = 0;
			summation2_u1 = 0;
			summation1_u2 = 0;
			summation2_u2 = 0;
		}
		void inner_product_4elements(data_type mov1[4], data_type mov2[4], data_type IR_col1[4], data_type IR_col2[4]) {  
			for(int i = 0; i < 4; i++){
				summation1_u1 += mov1[i] * IR_col1[i];
				summation2_u1 += mov2[i] * IR_col1[i];
				summation1_u2 += mov1[i] * IR_col2[i];
				summation2_u2 += mov2[i] * IR_col2[i];
			}
		} 
		void run(data_type mov1[4], data_type mov2[4], data_type mov3[4], data_type mov4[4], data_type IR_col1[4], data_type IR_col2[4], data_type IR_col3[4], data_type IR_col4[4], data_type &output1, data_type &output2, data_type &output3, data_type &output4) { 
			
			// compute partial inner product
			inner_product_4elements(mov1, mov2, IR_col1, IR_col2);
			inner_product_4elements(mov3, mov4, IR_col3, IR_col4);
			
			// add and write to output
			output1 = summation1_u1;
			output2 = summation2_u1;
			output3 = summation1_u2;
			output4 = summation2_u2;
		}		
};


#pragma hls_design top
class itemrank{
private:
    int max_iter;
    data_type a;
    data_type summation1, summation2, summation3, summation4, summation5, summation6, summation7, summation8;
    data_type IR_col_u1[m], IR_col_u2[m], IR_col_u3[m], IR_col_u4[m];
    data_type IR_col_new_u1[m], IR_col_new_u2[m], IR_col_new_u3[m], IR_col_new_u4[m];
    data_type IR_col_u1v1[4], IR_col_u1v2[4], IR_col_u2v1[4], IR_col_u2v2[4], IR_col_u3v1[4], IR_col_u3v2[4] , IR_col_u4v1[4], IR_col_u4v2[4];
    data_type output1, output2, output3, output4, output5, output6, output7, output8;
    data_type mov1[4], mov2[4], mov3[4], mov4[4];
public:
    // constructor
    itemrank() {
    	max_iter = 20;
    	a = 0.85;
		summation1 = 0;
		summation2 = 0;
		summation3 = 0;
		summation4 = 0;
		summation5 = 0;
		summation6 = 0;
		summation7 = 0;
		summation8 = 0;
    };

	#pragma hls_design interface
    void run (data_type movies_correlation[m][n],data_type initial_critics[m][n],data_type IR_output[m][n]){
		
         USER:for(int user = 0; user < n; user = user + 4) { //for each user
        	// initialize IR
            IR_COL_INIT:for (int j = 0; j < n; j++) {
                IR_col_u1[j] = 1.0/m;
                IR_col_u2[j] = 1.0/m;
                IR_col_u3[j] = 1.0/m;
                IR_col_u4[j] = 1.0/m;
            }

            TIME:for (int t = 0; t < max_iter; t++) { //for each time
            
                MOVIE:for(int i = 0; i < m; i = i + 2) { //for each movie
                	
					//create pointers used in object inner_product_2users
					output1 = 0;
					output2 = 0;
					output3 = 0;
					output4 = 0;
					output5 = 0;
					output6 = 0;
					output7 = 0;
					output8 = 0;
						
                    COLS: for(int j = 0; j < m; j = j + 4){
                    	
                		//preprocessing
                		for(int iter=0; iter < 4; iter++){
                			//movies
                			mov1[iter] = movies_correlation[i][iter];
                			mov2[iter] = movies_correlation[i+1][iter];
                			mov3[iter] = movies_correlation[i][iter+4];
                			mov4[iter] = movies_correlation[i+1][iter+4];
                			//IR_col
                			IR_col_u1v1[iter] = IR_col_u1[iter];
							IR_col_u1v2[iter] = IR_col_u1[iter+4];
							IR_col_u2v1[iter] = IR_col_u2[iter];
							IR_col_u2v2[iter] = IR_col_u2[iter+4];
                			IR_col_u3v1[iter] = IR_col_u3[iter];
							IR_col_u3v2[iter] = IR_col_u3[iter+4];
							IR_col_u4v1[iter] = IR_col_u4[iter];
							IR_col_u4v2[iter] = IR_col_u4[iter+4];
						}
						
						//create objects and call run()
                    	inner_product_2users obj1;
                    	inner_product_2users obj2;
                    	obj1.run(mov1, mov2, mov3, mov4, IR_col_u1v1, IR_col_u1v2, IR_col_u2v1, IR_col_u2v2, output1, output2, output3, output4);
                    	obj2.run(mov1, mov2, mov3, mov4, IR_col_u3v1, IR_col_u3v2, IR_col_u4v1, IR_col_u4v2, output5, output6, output7, output8);
                    	
						//create pointers used in object inner_product_2users
						summation1 += output1;
						summation2 += output2;
						summation3 += output3;
						summation4 += output4;
						summation5 += output5;
						summation6 += output6;
						summation7 += output7;
						summation8 += output8;
					}
                    
                    IR_col_new_u1[i]   = a * summation1 + (1-a)*initial_critics[i][user];       //rating of i-th movie of user 1
                    IR_col_new_u1[i+1] = a * summation2 + (1-a)*initial_critics[i+1][user];     //rating of i+1-th movie of user 1
                    IR_col_new_u2[i]   = a * summation3 + (1-a)*initial_critics[i][user+1];     //rating of i-th movie of user 2
                    IR_col_new_u2[i+1] = a * summation4 + (1-a)*initial_critics[i+1][user+1];   //rating of i+1-th movie of user 2
                    IR_col_new_u3[i]   = a * summation5 + (1-a)*initial_critics[i][user+2];     //rating of i-th movie of user 3 
                    IR_col_new_u3[i+1] = a * summation6 + (1-a)*initial_critics[i+1][user+2];   //rating of i+1-th movie of user 3
                    IR_col_new_u4[i]   = a * summation7 + (1-a)*initial_critics[i][user+3];     //rating of i-th movie of user 4
                    IR_col_new_u4[i+1] = a * summation8 + (1-a)*initial_critics[i+1][user+3];   //rating of i+1-th movie of user 4
                }
                
                UPDATE_IR:for(int j = 0; j < m; j++) {
                	IR_col_u1[j] = IR_col_new_u1[j];
					IR_col_u2[j] = IR_col_new_u2[j];	
					IR_col_u3[j] = IR_col_new_u3[j];	
					IR_col_u4[j] = IR_col_new_u4[j];		
                }
            }
            // write to output
            WRITE_OUTPUT:for(int i = 0; i < m; i++){
                IR_output[i][user] = IR_col_u1[i];
                IR_output[i][user+1] = IR_col_u2[i];
                IR_output[i][user+2] = IR_col_u3[i];
                IR_output[i][user+3] = IR_col_u4[i];
            }
        }
    }
};


int main(){


	// define arrays used in program
    data_type IR_old[m][n], IR_new[m][n];
    data_type movies_correlation[m][m]= { // denoted in the paper as C
	        {   0.0,  1.0/9, 2.0/14, 1.0/15, 3.0/25, 4.0/17, 1.0, 2.0},
	        {1.0/14,    0.0, 1.0/14, 1.0/15, 1.0/25, 1.0/17, 1.0, 3.0},
	        {2.0/14,  1.0/9,    0.0, 2.0/15, 3.0/25, 2.0/17, 2.0, 2.0},
	        {1.0/14,  1.0/9, 2.0/14,    0.0, 3.0/25, 4.0/17, 1.0, 2.0},
	        {3.0/14,  1.0/9, 3.0/14, 3.0/15,    0.0, 3.0/17, 7.0, 3.0},
	        {4.0/14,  1.0/9, 2.0/14, 4.0/15, 3.0/25,    0.0, 1.0, 1.0},
	        {1.0/14,  1.0/9, 2.0/14, 1.0/15, 7.0/25, 1.0/17, 0.0, 2.0},
	        {2.0/14,  3.0/9, 2.0/14, 2.0/15, 3.0/25, 1.0/17, 2.0, 0.0},
	};
	//normalized movies ratings
	data_type initial_critics[m][n]= { //array with columns the vector d of each user
	        {1.0/7, 5.0/12,   0/14, 4.0/11, 1.0/7, 5.0/10,   0/9, 4.0/11},
	        {1.0/7, 0.0/12, 4.0/14, 1.0/11, 1.0/7, 0.0/10, 4.0/9, 1.0/11},
	        {4.0/7, 5.0/12, 3.0/14, 1.0/11, 0.0/7, 3.0/10, 1.0/9, 1.0/11},
	        {0.0/7, 0.0/12, 1.0/14, 0.0/11, 1.0/7, 0.0/10, 1.0/9, 1.0/11},
	        {0.0/7, 1.0/12, 1.0/14, 3.0/11, 0.0/7, 1.0/10, 1.0/9, 1.0/11},
	        {1.0/7, 0.0/12, 4.0/14, 2.0/11, 1.0/7, 0.0/10, 0.0/9, 1.0/11},
	        {0.0/7, 1.0/12, 2.0/14, 0.0/11, 2.0/7, 1.0/10, 2.0/9, 1.0/11},
	        {0.0/7, 0.0/12, 0.0/14, 0.0/11, 1.0/7, 0.0/10, 0.0/9, 1.0/11},
	};

	
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


   return(0);
}

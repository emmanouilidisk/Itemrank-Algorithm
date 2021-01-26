#include <iostream>
#include "mc_scverify.h"
#include "ac_int.h"
#include "ac_fixed.h"

const int n=4; // number of users
const int m=4; // number of movies
const int digits = 1; //number of digits of integer part
const int total_num_digits = 15; //total number of digits


template <typename T, int m>
class vector{
	private:
		T *mem;
		int pnt;
	public:
		vector(){
			mem = NULL;
			pnt = 0;
		}
		vector(T *pointer){
			mem = pointer;
			pnt = 0;
		}
		void write(ac_fixed<total_num_digits,digits,false> val) {  
			pnt = (pnt + 1) % m;
			mem[pnt] = val;
		} 
		T read() { 
			pnt = (pnt - 1) % m;
			return mem[pnt];
		}		
};

#pragma hls_design top
class itemrank{
private:
    int max_iter;
    ac_fixed<total_num_digits,digits,false> a;
    ac_fixed<total_num_digits,digits,false> summation, acc1, acc2;
    ac_fixed<total_num_digits,digits,false> IR_col[m];
    ac_fixed<total_num_digits,digits,false> IR_col_new[m];
    ac_fixed<total_num_digits,digits,false> buffer_initial_critics[m];
public:
    // constructor
    itemrank() {
    	max_iter=20;
    	a=0.85;
    	summation=0;  
    };

	#pragma hls_design interface
    void CCS_BLOCK(run) (ac_fixed<total_num_digits,digits,false> movies_correlation[m][n],ac_fixed<total_num_digits,digits,false> initial_critics[m][n],ac_fixed<total_num_digits,digits,false> IR_new[m][n], ac_fixed<total_num_digits,digits,false> IR_col_pointer[m], ac_fixed<total_num_digits,digits,false> IR_col_new_pointer[m]){
		// IR(0) = 1.0/m
		// IR(t+1) = a * C * IR(t) + (1-a) * d
		
        USER:for(int user = 0; user < n; user++) { //for each user
        
        	// declare vector
			vector <ac_fixed<total_num_digits,digits,false>, m> IR_col(IR_col_pointer);
			vector <ac_fixed<total_num_digits,digits,false>, m> IR_col_new(IR_col_new_pointer);
			// initialize buffer IR
			#pragma unroll yes
            #pragma unroll 2
            IR_COL_INIT:for (int j = 0; j < n; j++) {
                IR_col.write(1.0/m);
            }
            TIME:for(int t = 0; t < max_iter; t++) { //for each time
                MOVIE:for(int i = 0; i < m; i++) { //for each movie
                	summation = 0;
                	#pragma unroll yes
                 #pragma unroll 2
                    INNER_PRODUCT: for(int j = 0; j < m; j++) {
                        summation = summation + movies_correlation[i][j] * IR_col.read();
                    }
                    //summation = acc1 + acc2;
                    IR_col_new.write(a * summation + (1-a)*initial_critics[i][user]); //rating of i-th movie of user
                }
                #pragma unroll yes
                #pragma unroll 2
                UPDATE_IR:for(int j = 0; j < m; j++) {
                	IR_col.write(IR_col_new.read());	
                }
            }
            // write to output
            #pragma unroll yes
            #pragma unroll 2
            WRITE_OUTPUT:for(int i = 0; i < m; i++){
                IR_new[i][user] = IR_col.read();
            }
        }
    }
};


CCS_MAIN(int argc, char* argv[]){


	// define arrays used in program
    ac_fixed<total_num_digits,digits,false> IR_new[m][n];
    ac_fixed<total_num_digits,digits,false> IR_col_pointer[m],IR_col_new_pointer[m];
    ac_fixed<total_num_digits,digits,false> movies_correlation[m][m]= { // denoted in the paper as C
	        {0.0,   1.0/6, 1.0/4, 1.0/5},
	        {1.0/3, 0,     2.0/4, 3.0/5},
	        {1.0/3, 2.0/6, 0,     1.0/5},
	        {1.0/3, 3.0/6, 1.0/4, 0}
	};
	//normalized movies ratings
    ac_fixed<total_num_digits,digits,false> initial_critics[m][n]= { //array with columns the vector d of each user
	        {1.0/6, 5.0/10, 0,      4.0/9},
	        {1.0/6, 0.0/10, 4.0/11, 1.0/9},
	        {4.0/6, 5.0/10, 3.0/11, 1.0/9},
	        {0.0,   0.0,   4.0/11, 3.0/9}
	};
	// initialize 
        for (int j = 0; j < m; j++) {
            IR_col_pointer[j] = 1.0/m;
            IR_col_new_pointer[j] = 1.0/m;
        }
	
    // create object and run Itemrank algorithm
    itemrank obj;
    obj.run(movies_correlation,initial_critics,IR_new,IR_col_pointer,IR_col_new_pointer);

    // print results
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            std::cout << IR_new[i][j] << " ";
        }
        std::cout << "\n";
    }


   CCS_RETURN(0);
}


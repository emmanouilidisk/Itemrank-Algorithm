#include <iostream>

#include "ac_int.h"
#include "ac_fixed.h"

const int n=4; // number of users
const int m=4; // number of movies

#pragma hls_design top
class itemrank{
private:
    int max_iter;
    ac_fixed<10,5,false> a;
    ac_fixed<10,5,false> summation;
    ac_fixed<10,5,false> temp[m][n];
public:
    // constructor
    itemrank() {
    	max_iter=20;
    	a=0.85;
    	summation=0;   	
        // initialize IR matrix
       
    };

#pragma hls_design interface
    void run(ac_fixed<10,5,false> IR_old[m][n],ac_fixed<10,5,false> movies_correlation[m][n],ac_fixed<10,5,false> initial_critics[m][n],ac_fixed<10,5,false> IR_new[m][n]){
		
		for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                temp[i][j] = IR_old[i][j];
            }
        }
		
        for(int user = 0; user < n; user++) { //for each user
            for (int t = 0; t < max_iter; t++) { //for each time
                for(int i = 0; i < m; i++) { //for each movie
                    summation = 0;
                    for(int j = 0; j < m; j++) {
                        summation = summation + movies_correlation[i][j] * temp[j][user];
                    }
                    temp[i][user] = a * summation + (1-a)*initial_critics[i][user]; //rating of i-th movie of user
                }
                for(int i = 0; i < m; i++){
                    for(int j = 0; j < n; j++){
                        IR_new[i][j] = temp[i][j];
                    }
                }
            }
        }
    }
};


int main() {

	// define arrays used in program
    ac_fixed<10,5,false> IR_old[m][n], IR_new[m][n];
    ac_fixed<10,5,false>movies_correlation[m][m]= { // denoted in the paper as C
	        {0.0,   1.0/6, 1.0/4, 1.0/5},
	        {1.0/3, 0,     2.0/4, 3.0/5},
	        {1.0/3, 2.0/6, 0,     1.0/5},
	        {1.0/3, 3.0/6, 1.0/4, 0}
	};
	//normalized movies ratings
    ac_fixed<10,5,false> initial_critics[m][n]= { //array with columns the vector d of each user
	        {1.0/6, 5.0/10, 0,      4.0/9},
	        {1.0/6, 0.0/10, 4.0/11, 1.0/9},
	        {4.0/6, 5.0/10, 3.0/11, 1.0/9},
	        {0.0,   0.0,   4.0/11, 3.0/9}
	};
	
	 for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                IR_old[j][i] = 1.0/m;
            }
        }
	
    // create object and run Itemrank algorithm
    itemrank obj;
    obj.run(IR_old,movies_correlation,initial_critics,IR_new);

    // print results
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            std::cout << IR_new[i][j] << " ";
        }
        std::cout << "\n";
    }


    return 0;
}

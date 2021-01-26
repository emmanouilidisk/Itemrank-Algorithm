#include <iostream>
#include <fstream>
#include "ac_int.h"
#include "ac_fixed.h"

const int n = 4; // number of users
const int m = 4; // number of movies
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


class itemrank{
private:
    int max_iter;
    ac_fixed<total_num_digits,digits,false> a;
    ac_fixed<total_num_digits,digits,false> summation;
    ac_fixed<total_num_digits,digits,false> IR_col[m];
    ac_fixed<total_num_digits,digits,false> IR_col_new[m];
public:
    // constructor
    itemrank() {
    	max_iter=20;
    	a=0.85;
    	summation=0;  
    };


    void run (ac_fixed<total_num_digits,digits,false> movies_correlation[m][n],ac_fixed<total_num_digits,digits,false> initial_critics[m][n],ac_fixed<total_num_digits,digits,false> IR_new[m][n], ac_fixed<total_num_digits,digits,false> IR_col_pointer[m], ac_fixed<total_num_digits,digits,false> IR_col_new_pointer[m]){
		
		// IR(0) = 1.0/m
		// IR(t+1) = a * C * IR(t) + (1-a) * d
		
        USER:for(int user = 0; user < n; user++) { //for each user
        
        	// declare vector
			vector <ac_fixed<total_num_digits,digits,false>, m> IR_col(IR_col_pointer);
			vector <ac_fixed<total_num_digits,digits,false>, m> IR_col_new(IR_col_new_pointer);
			// initialize buffer IR
            IR_COL_INIT:for (int j = 0; j < n; j++) {
                IR_col.write(1.0/m);
            }
            TIME:for(int t = 0; t < max_iter; t++) { //for each time
                MOVIE:for(int i = 0; i < m; i++) { //for each movie
                	summation = 0;
                    INNER_PRODUCT: for(int j = 0; j < m; j++) {
                        summation = summation + movies_correlation[i][j] * IR_col.read();
                    }
                    //summation = acc1 + acc2;
                    IR_col_new.write(a * summation + (1-a)*initial_critics[i][user]); //rating of i-th movie of user
                }
                UPDATE_IR:for(int j = 0; j < m; j++) {
                	IR_col.write(IR_col_new.read());	
                }
            }
            // write to output
            WRITE_OUTPUT:for(int i = 0; i < m; i++){
                IR_new[i][user] = IR_col.read();
            }
        }
    }
};


void testing(){
	
	//create output file
	std::ofstream out;
	out.open("testing_results.txt");
	
	//open input file
	std::ifstream file;
    file.open("test_cases.txt");
    if (!file.is_open()) return;
	int array_size; //size of array that is read each time
	
	//variables declaration
    std::string word;
    float num;
    int num_tests = 0; //number of test cases
    
    //main loop
    while (file >> word){	
		if(word == "m"){
			//get matrix size
			file >> array_size;
			num_tests++;
			
			//declare matrices
	    	ac_fixed<total_num_digits,digits,false> movies_correlation[m][n];
			ac_fixed<total_num_digits,digits,false> initial_critics[m][n];
			ac_fixed<total_num_digits,digits,false> IR_new[m][n];
			ac_fixed<total_num_digits,digits,false> IR_col_pointer[m];
			ac_fixed<total_num_digits,digits,false> IR_col_new_pointer[m];
		
	    	//write data to matrices
	    	for(int i=0; i<array_size; i++){
		    	for(int j=0; j<array_size; j++){
		    		file >> num;
		    		movies_correlation[i][j] = num;
				}
			}
			for(int i=0; i<array_size; i++){
		    	for(int j=0; j<array_size; j++){
		    		file >> num;
		    		initial_critics[i][j] = num;
				}
			}
			
			
			//initialize 
		    for (int j = 0; j < array_size; j++) {
		        IR_col_pointer[j] = 1.0/array_size;
		        IR_col_new_pointer[j] = 1.0/array_size;
		    }
		    
		    // Create object and run Itemrank algorithm
		    itemrank obj;
		    obj.run(movies_correlation, initial_critics, IR_new, IR_col_pointer, IR_col_new_pointer);
		    
			//write to output file
		    out << "Test No: "<< num_tests << "\n";
		    for (int i=0; i<m; i++){   
			    for (int j=0; j<m;j++){                      
			        out << IR_new[i][j] << "\t";
			    }
			    out << "\n";
			}
			out << "\n";
			out << " Max elements of every column: \n";
		    //find max of every column and write to output file
		    for (int i = 0; i < m; i++) { 
			// initialize the maximum element 
			// with 0 
			int maxm = IR_new[0][i]; 
			int row = 0;
			// Run the inner loop for rows 
			for (int j = 1; j < m; j++) { 
			    // check if any element is greater  
			    // than the maximum element 
			    // of the column and replace it 
			    if (IR_new[j][i] > maxm) 
				row = j;
				maxm = mat[j][i]; 
			} 

			// print the largest element of the column 
			out << row << "\t";
		    } 
			out << "\n";
		} 
    }
  //close files
  file.close();
  out.close();
}


int main(){
	
	// test code
	testing();

   return(0);
}


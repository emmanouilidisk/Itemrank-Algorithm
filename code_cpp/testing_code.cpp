#include <iostream>
#include <fstream>
#include <stdio.h> 

#include "ac_fixed.h"
#include "itemrank.h"
using namespace std;

//declare matrices
static data_type movies_correlation[m][m];
static data_type initial_critics[n][m];

int array_size = 1000;

void create_testing_matrix(int max_row)
{	
	
	for(int i=0; i<array_size; i++){
		for(int j=0; j<array_size; j++){
			if(i==j){
				movies_correlation[i][j] = 0.0;	
			}
			else if(i==max_row && j!=max_row)
			{
				movies_correlation[i][j] = 1000.0/(1000.0+(array_size-2));
			}			
			else if(j==max_row)
			{
				movies_correlation[i][j] =  1000.0/((array_size-1)*1000.0);
			}			
			else
			{
				movies_correlation[i][j] = 1.0/(1000.0+(array_size-2));
			}
		}
	}
	
	
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<array_size; j++)
		{
			if(j==20 || j==5)
			{
				initial_critics[i][j] = 0.0 ;				
			}
			else
			{
				initial_critics[i][j] = 1.0/((array_size-2));
			}
		}
	}	

}


void testing(int num_tests){
	
	//create output file
	std::ofstream out;
	out.open("testing_results.txt");
	
	compressed_data_type sparce_movies[m][num_elements];
	int sparce_movies_index[m][num_elements], sparce_critics_index[m][n];
	int IR_new[n], max_row, flag;
	
    for(int test=0; test<num_tests; test++)
    {
    	max_row = rand() % 100; 
    	create_testing_matrix(max_row);
    	flag = 1;
    	
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
	    
	    //write to output file
	    out << "Test No: "<< test << "\n";
	    for (int i=0; i<n; i++){                       
		    out << IR_new[i] << "\t";
		    if(IR_new[i] != max_row) flag == 0;
		}
		out << "\n";
		std::cout << "Test No: "<< test;
		if(flag) std::cout << " Correct" << "\n";
		else std::cout << " Error" << "\n";
		
		


	}
       			
  //close file
  out.close();	
}


int main(){
	
	int num_tests = 1001; //number of test cases
	
	// test code
	testing(num_tests);

   return(0);
}



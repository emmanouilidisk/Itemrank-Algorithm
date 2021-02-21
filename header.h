#ifndef ITEMRANK_H
#define ITEMRANK_H



#include <iostream>

#include "ac_int.h"
#include "ac_fixed.h"



const int m = 8; // number of movies
const int n = 8; // number of users
const int digits = 1; //number of digits of integer part
const int total_num_digits = 16; //total number of digits
const int num_elements = 8;
typedef ac_fixed<total_num_digits,digits,false> data_type;
const int compressed_size = num_elements/8;
typedef ac_int<8*total_num_digits,false> compressed_data_type;

#include "mc_scverify.h"

class MinHeap {
	int size;
	data_type  arr[num_elements];
	int *indices;
	//compressed_data_type *sparse_arr;
	int temp_index;
	compressed_data_type buffer;
public:
	
	// Constructor

	MinHeap(){}
	
    
// Constructor 
MinHeap(int inp_size, data_type input[m], int *pointer_indices)
{
	//sparse_arr = pointer_heap;
	indices = pointer_indices;
	// Initializing arr and size
	size = inp_size;
	
	for (int i = 0; i < num_elements; i++) {
		arr[i] = input[i];
		indices[i] = i;
	}
	
	// Building the Min Heap
	buildHeap();
	
}


void findkBiggest(int k, data_type input[m]){
	
	for (int i = k; i < m; i++) {

		// if current element is smaller continue
		if (arr[0] > input[i])
			continue;

		// Otherwise change minimum element to 
		// current element, and restore the heap property
		else {
			arr[0] = input[i];
			indices[0] = i;
			heapify(0);
		}
	}
	
}


void saveAs128bit(compressed_data_type *pointer_heap){
	//compress 8 words of 16bits to one word of 128 bits
	WRITE_128:for(int i=0; i<compressed_size; i++){
		for(int l=0; l<8; l++){
			for(int j=total_num_digits-1; j>=0; j--){
				buffer[(16*l)+j] = arr[l][j];
			}
		}
		pointer_heap[i].set_slc(0,buffer);
	}
		
}


int swap(int x, int y)
{
	data_type temp = arr[x];
	arr[x] = arr[y];
	arr[y] = temp;
	
	return 0;
}


void heapify(int i)
{
	// If Leaf Node, Simply return
	if (i >= size / 2)
		return;

	// variable to store the smallest element
	// index out of i, 2*i+1 and 2*i+2
	int smallest;

	// Index of left node
	int left = 2 * i + 1;

	// Index of right node
	int right = 2 * i + 2;

	// Select minimum from left node and 
	// current node i, and store the minimum
	// index in smallest variable
	if(arr[left] < arr[i]) smallest = left;
	else smallest = i;

	// If right child exist, compare and
	// update the smallest variable
	if (right < size){
		if(arr[right] < arr[smallest]) smallest = right;
	}

	// If Node i violates the min heap 
	// property, swap current node i with
	// smallest to fix the min-heap property 
	// and recursively call heapify for node smallest.
	while (smallest != i) {
		swap(i, smallest);
		int temp = indices[i];
		indices[i] = indices[smallest];
		indices[smallest] = temp;
		i = smallest;
		if (i >= size / 2)
		return;

		// variable to store the smallest element
		// index out of i, 2*i+1 and 2*i+2
		int smallest;
	
		// Index of left node
		int left = 2 * i + 1;
	
		// Index of right node
		int right = 2 * i + 2;
	
		// Select minimum from left node and 
		// current node i, and store the minimum
		// index in smallest variable
		if(arr[left] < arr[i]) smallest = left;
		else smallest = i;
	
		// If right child exist, compare and
		// update the smallest variable
		if (right < size){
			if(arr[right] < arr[smallest]) smallest = right;
		}
		}
}



void buildHeap()
{
	// Calling Heapify for all non leaf nodes
	for (int i = size / 2 - 1; i >= 0; i--) {
		heapify(i);
	}
	
}
	
	data_type* getHeap(){
		return arr;
	}
	data_type getsize(){
		return *(&arr + 1) - arr;
	}
	int* getIndices(){
		return indices;
	}
	
};





#pragma hls_design top
class itemrank{
private:
    int max_iter, index,index2, temp, last_read, l, r, mean, index_binary, buffer_binary;
    int k; //how many elements to keep when making matrices sparce
    int max_elem_indices[2];
    data_type max_elem[2];
    data_type a;
    data_type summation[4];
    data_type IR_col_u1[m], IR_col_u2[m];
    data_type IR_col_new[2][m];
    data_type IR_col_u1v1[4], IR_col_u1v2[4], IR_col_u2v1[4], IR_col_u2v2[4];
    data_type mov0[num_elements],mov01[num_elements];
	compressed_data_type inner_data1, inner_data2;
	MinHeap critics_heap_inside_module[2];
	
public:
    // constructor
    itemrank() {
    	max_iter = 20;
    	a = 0.85;
    	last_read = 0;
    };
    
    void inner_product_8elements(data_type IR_col1[4], data_type IR_col2[4], data_type IR_col3[4], data_type IR_col4[4], compressed_data_type sparce_movies[m][num_elements], int row_to_read) {  
    	
		//read 128-bit word and save it as 8 words of 16 bits
    	inner_data1 = sparce_movies[row_to_read][last_read];
    	inner_data2 = sparce_movies[row_to_read+1][last_read];
		for(int l=0; l<8; l++){
			//ac_int<3,false> a = bvec.slc<3>(0);
			mov0[l] = inner_data1.slc<16>(16*l);
			mov01[l] = inner_data2.slc<16>(16*l);
		
		}
		last_read++;
		
		//inner product
		INNER_SUM:for(int i = 0; i < 4; i++){
		    
			summation[0] += mov0[i] * IR_col1[i];
			summation[1] += mov01[i] * IR_col1[i];
			summation[2] += mov0[i] * IR_col2[i];
			summation[3] += mov01[i] * IR_col2[i];
			
			summation[0] += mov0[i+4] * IR_col3[i];
			summation[1] += mov01[i+4] * IR_col3[i];
			summation[2] += mov0[i+4] * IR_col4[i];
			summation[3] += mov01[i+4] * IR_col4[i];
		}
	} 
			
		
    void preprocessing_movies (data_type movies_correlation[m][m], compressed_data_type sparce_movies[m][num_elements], int sparce_movies_index[m][num_elements]){
    	for(int i = 0; i < m; i++){
			MinHeap mheap(num_elements, movies_correlation[i],  sparce_movies_index[i]);
			//First k elements
			mheap.findkBiggest(num_elements, movies_correlation[i]);
			mheap.saveAs128bit(sparce_movies[i]);
		}
	}

	void preprocessing_critics (data_type initial_critics[n][m], int low_index, compressed_data_type sparce_critics[m][num_elements/8], int sparce_critics_index[m][n]){
    
		for(int user = 0; user < 2; user++){
			//create MinHeap object and find the k biggest elements to make critics array sparce
			MinHeap mheap(num_elements, initial_critics[low_index+user],  sparce_critics_index[low_index+user]);
			mheap.findkBiggest(num_elements, initial_critics[low_index+user]);
			
			critics_heap_inside_module[user] = mheap;
		}
	}
	int binarySearch(int x, int user, int sparce_critics_index[m][n]) 
	{ 
		l = 0;
		r = num_elements;
		index_binary = -1;
		
	    FOR_BIN:for(int i=0; i<3; i++){ 
	        mean = l + (r - l) / 2; 
	        
	        buffer_binary = sparce_critics_index[user%4][mean];
	        if (buffer_binary == x) index_binary = mean; 
	  
	        else if (buffer_binary < x)  l = mean + 1; 
	        else r = mean - 1; 
	    } 
	  
	    return index_binary; 
	} 
	
	
	#pragma hls_design interface
    void CCS_BLOCK(run) (data_type movies_correlation[m][m], data_type initial_critics[n][m], int IR_output[n], compressed_data_type sparce_movies[m][num_elements],compressed_data_type sparce_critics[m][num_elements/8],int sparce_movies_index[m][num_elements],int sparce_critics_index[m][n]){
		
		//preprocessing for sparcity
		std::cout << "Preprocessing";
		preprocessing_movies(movies_correlation,sparce_movies,sparce_movies_index);
		 
		
        USER:for(int user = 0; user < n; user = user + 2) { //for each user
         
         	preprocessing_critics(initial_critics, user, sparce_critics, sparce_critics_index);
         	
        	// initialize IR
            IR_COL_INIT:for (int jj = 0; jj < m; jj++) {
                IR_col_u1[jj] = 1.0/m;
                IR_col_u2[jj] = 1.0/m;
            }
            
            TIME:for (int t = 0; t < max_iter; t++) { //for each time
            	
            	
                MOVIE:for(int i = 0; i < m; i = i + 2) { //for each movie
                	
					//create pointers used in object inner_product_2users
					for(int v=0; v < 4; v++){
						summation[v] = 0;
					}
					
                    COLS: for(int j = 0; j < num_elements/4; j = j + 4){
                		//preprocessing
                		COLS_PREPROC:for(int iter=0; iter < 4; iter++){
                			
                			index = sparce_movies_index[i][2*j+iter];
                			IR_col_u1v1[iter] = IR_col_u1[index];
                			IR_col_u2v1[iter] = IR_col_u2[index];
                			
                			index2 = sparce_movies_index[i][2*(j+2)+iter];
							IR_col_u1v2[iter] = IR_col_u1[index2];
							IR_col_u2v2[iter] = IR_col_u2[index2];
						}
						//compute inner product for two lines and two users
                    	inner_product_8elements(IR_col_u1v1, IR_col_u1v2, IR_col_u2v1, IR_col_u2v2, sparce_movies, i);
					}
					
					temp = 0;
					//data_type sparce_elem;
					BINARY:for(int user_var = 0; user_var <= 1; user_var++){
						for(int i_var = 0; i_var < 2; i_var++){
							index = binarySearch(i+i_var,user+user_var,sparce_critics_index);
							
		                    if(index != -1) {
//		                    	for(int bit=16*index; bit < 17*index; bit++){
//		                    		sparce_elem[bit-(16*index)] = sparce_critics[user_var][0][bit];
//								}
//								std::cout << sparce_critics[user_var][0][(16*index)]] << " " << critics_heap_inside_module[user_var].getHeap()[index] << "\n";
								IR_col_new[user_var][i+i_var] = a * summation[2*user_var+i_var] + (1-a)*critics_heap_inside_module[user_var].getHeap()[index];       
							}
		                    else IR_col_new[user_var][i+i_var] = a * summation[2*user_var+i_var];
		                    
						}
					}
					last_read = 0;
                }
                UPDATE_IR:for(int j = 0; j < m; j++) {
	                	IR_col_u1[j] = IR_col_new[0][j];	
						IR_col_u2[j] = IR_col_new[1][j];	
                }
                
            }
            
            // write to output
            OUTPUT_WRITE: for(int i = 0; i < 2; i++){
            	max_elem[i] = 0;
            	max_elem_indices[i] = 0;
			}
            FIND_Max_INDEX:for(int i = 0; i < m; i++){
            	//pipeline
            	if(IR_col_u1[i] > max_elem[0] && initial_critics[user][i]==0) {
            		max_elem[0] = IR_col_u1[i];
            		max_elem_indices[0] = i; 
				}
            	if(IR_col_u2[i] > max_elem[1] && initial_critics[user+1][i]==0){
            		max_elem[1] = IR_col_u2[i];
            		max_elem_indices[1] = i; 
				} 
            }
            std::cout << "\n";
            std::cout << "User "<< user;
//            for(int j = 0; j < m; j++){
//		        std::cout << initial_critics[j][2] << " ";
//		    }
            if(user < n - 1){
	            IR_output[user] = max_elem_indices[0];
	            IR_output[user+1] = max_elem_indices[1];
        	}
        	else{
        		IR_output[user] = max_elem_indices[0];
			}
        }
    }
};

#endif //ITEMRANK_H


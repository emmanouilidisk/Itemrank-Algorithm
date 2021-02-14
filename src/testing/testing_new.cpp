#include <iostream>
#include <fstream>
#include "ac_int.h"
#include "ac_fixed.h"

const int m = 4; // number of movies
const int n = 4; // number of users
const int digits = 3; //number of digits of integer part
const int total_num_digits = 16; //total number of digits
const int num_elements = 64;
typedef ac_fixed<total_num_digits,digits,false> data_type;
	

class MinHeap {
	int size;
	data_type arr[num_elements];
	int indices[num_elements];
public:
	
	// Constructor
	MinHeap(int inp_size, data_type input[m]);
	MinHeap(){}
	
	void heapify(int i);
	
	void buildHeap();
	
	int swap(int x, int y);
	
	void findkBiggest(int k, data_type input[m]);
	
	data_type* getHeap(){
		return arr;
	}
	data_type getsize(){
		return *(&arr + 1) - arr;;
	}
	int* getIndices(){
		return indices;
	}
	
};


// Constructor 
MinHeap::MinHeap(int inp_size, data_type input[m])
{
	// Initializing arr and size
	size = inp_size;
	
	for (int i = 0; i < num_elements; i++) {
		arr[i] = input[i];
		indices[i] = i;
	}
	
	// Building the Min Heap
	buildHeap();
	
}


void MinHeap::findkBiggest(int k, data_type input[m]){
	// Loop For each element in array
	// after the kth element
		
		for (int i = k; i < m; i++) {
	
			// if current element is smaller 
			// than minimum element, do nothing 
			// and continue to next element
			if (arr[0] > input[i])
				continue;
	
			// Otherwise Change minimum element to 
			// current element, and call heapify to
			// restore the heap property
			else {
				arr[0] = input[i];
				indices[0] = i;
				heapify(0);
			}
		}
//		// Now min heap contains k maximum
//		// elements, Iterate and print
//		std::cout << "\n";
//		std::cout << "Result:";
//		for (int i = 0; i < k; i++) {
//			std::cout << arr[i] << " ";
//		}
//		std::cout << "\n";
//		std::cout << "indices" << "\n";
//		for (int i = 0; i < k; i++) {
//			std::cout << indices[i] << " ";
//		}
}


// Swap function to interchange
// the value of variables x and y
int MinHeap::swap(int x, int y)
{
	data_type temp = arr[x];
	arr[x] = arr[y];
	arr[y] = temp;
}


// Min Heapify function, that assumes
// 2*i+1 and 2*i+2 are min heap and 
// fix min heap property for i
void MinHeap::heapify(int i)
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
	if (smallest != i) {
		swap(i, smallest);
		int temp = indices[i];
		indices[i] = indices[smallest];
		indices[smallest] = temp;
		heapify(smallest);
	}
}



// Build Min Heap
void MinHeap::buildHeap()
{
	// Calling Heapify for all non leaf nodes
	for (int i = size / 2 - 1; i >= 0; i--) {
		heapify(i);
	}
	
}


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
    int max_iter, index,ind1, ind2;
    int k; //how many elements to keep when making matrices sparce
    int max_elem_indices[4];
    data_type max_elem[4];
    data_type a;
    data_type summation[8], output[8];
    data_type IR_col_u1[m], IR_col_u2[m], IR_col_u3[m], IR_col_u4[m];
    data_type IR_col_new_u1[m], IR_col_new_u2[m], IR_col_new_u3[m], IR_col_new_u4[m];
    data_type IR_col_u1v1[4], IR_col_u1v2[4], IR_col_u2v1[4], IR_col_u2v2[4], IR_col_u3v1[4], IR_col_u3v2[4] , IR_col_u4v1[4], IR_col_u4v2[4];
    data_type mov1[4], mov2[4], mov3[4], mov4[4];
    MinHeap sparse_movies[m]; 
	MinHeap sparse_critics[4];
	data_type critics_col[m];
public:
    // constructor
    itemrank() {
    	max_iter = 20;
    	a = 0.85;
    	for(int v=0; v < 8; v++){
			summation[v] = 0;
		}
//		k = 4;
    };
    
    void preprocessing_movies (data_type movies_correlation[m][m]){
    
		//First k elements
		//sparse movies
		for(int i = 0; i < m; i++){
			// Creating Min Heap for given
			MinHeap mheap(num_elements, movies_correlation[i]);
			
			mheap.findkBiggest(num_elements, movies_correlation[i]);
			sparse_movies[i] = mheap;
		}
	}

	void preprocessing_critics (data_type initial_critics[m][n], int low_index){
    
		//sparse critics
		for(int user = 0; user < 4; user++){
			for(int row = 0; row < m; row++){
				critics_col[row] = initial_critics[row][low_index+user];
			}
			// Creating Min Heap for given
			MinHeap mheap(num_elements, critics_col);
			
			mheap.findkBiggest(num_elements, critics_col);
			sparse_critics[user] = mheap;
		}
	}
	int binarySearch(int x, int user) 
	{ 
		int l = 0;
		int r = num_elements;
	    while (l <= r) { 
	        int mean = l + (r - l) / 2; 
	  
	        // Check if x is present at mid 
	        if (sparse_critics[user].getIndices()[mean] == x) 
	            return mean; 
	  
	        // If x greater, ignore left half 
	        if (sparse_critics[user].getIndices()[mean] < x) 
	            l = mean + 1; 
	  
	        // If x is smaller, ignore right half 
	        else
	            r = mean - 1; 
	    } 
	  
	    // if we reach here, then element was 
	    // not present 
	    return -1; 
	} 
	
	
	#pragma hls_design interface
    void run (data_type movies_correlation[m][m], data_type initial_critics[m][n], int* IR_output){
		
		//preprocessing for sparcity
		k = num_elements;
		preprocessing_movies(movies_correlation);
		
		int num_users_to_be_computed = ((n / 4) + 1) * 4;
		int mod_users = n % 4;
        USER:for(int user = 0; user < n; user = user + 4) { //for each user
         
         	preprocessing_critics(initial_critics, user);
         	
        	// initialize IR
            IR_COL_INIT:for (int j = 0; j < m; j++) {
                IR_col_u1[j] = 1.0/m;
                IR_col_u2[j] = 1.0/m;
                IR_col_u3[j] = 1.0/m;
                IR_col_u4[j] = 1.0/m;
            }
			for(int v=0; v < 8; v++){
				output[v] = 0;
				summation[v] = 0;
			}
            TIME:for (int t = 0; t < max_iter; t++) { //for each time
            	
            	
                MOVIE:for(int i = 0; i < m; i = i + 2) { //for each movie
                	
					//create pointers used in object inner_product_2users
					for(int v=0; v < 8; v++){
						summation[v] = 0;
					}
						
                    COLS: for(int j = 0; j < num_elements/8; j = j + 4){
                		//preprocessing
                		for(int iter=0; iter < 4; iter++){
                			//movies
                			mov1[iter] = sparse_movies[i].getHeap()[iter];
                			mov2[iter] = sparse_movies[i+1].getHeap()[iter];
                			mov3[iter] = sparse_movies[i].getHeap()[iter+4];
                			mov4[iter] = sparse_movies[i+1].getHeap()[iter+4];
					        //IR_col
                			ind1 = sparse_movies[i].getIndices()[2*j+iter];
                			ind2 = sparse_movies[i].getIndices()[2*(j+2)+iter];
                			IR_col_u1v1[iter] = IR_col_u1[ind1];
							IR_col_u1v2[iter] = IR_col_u1[ind2];
							IR_col_u2v1[iter] = IR_col_u2[ind1];
							IR_col_u2v2[iter] = IR_col_u2[ind2];
                			IR_col_u3v1[iter] = IR_col_u3[ind1];
							IR_col_u3v2[iter] = IR_col_u3[ind2];
							IR_col_u4v1[iter] = IR_col_u4[ind1];
							IR_col_u4v2[iter] = IR_col_u4[ind2];
						}
						//create objects and call run()
                    	inner_product_2users obj1;
                    	inner_product_2users obj2;
                    	obj1.run(mov1, mov2, mov3, mov4, IR_col_u1v1, IR_col_u1v2, IR_col_u2v1, IR_col_u2v2, output[0], output[1], output[2], output[3]);
                    	obj2.run(mov1, mov2, mov3, mov4, IR_col_u3v1, IR_col_u3v2, IR_col_u4v1, IR_col_u4v2, output[4], output[5], output[6], output[7]);
                    	
						//create pointers used in object inner_product_2users
						for(int v = 0; v < 8; v++){
							summation[v] += output[v];
						}
					}
					
                    index = binarySearch(i,user);
                    if(index != -1) IR_col_new_u1[i]   = a * summation[0] + (1-a)*sparse_critics[0].getHeap()[index];       //rating of i-th movie of user 1
                    else IR_col_new_u1[i] = a * summation[0];
                    
                    
                    index = binarySearch(i+1,user);
                    if(index != -1) IR_col_new_u1[i+1] = a * summation[1] + (1-a)*sparse_critics[0].getHeap()[index];     //rating of i+1-th movie of user 1
                    else IR_col_new_u1[i+1] = a * summation[1];
                    
                    index = binarySearch(i,user+1);
                    if(index != -1) IR_col_new_u2[i]   = a * summation[2] + (1-a)*sparse_critics[1].getHeap()[index];     //rating of i-th movie of user 2
                    else IR_col_new_u2[i] = a * summation[2];
                    
                    index = binarySearch(i+1,user+1);
                    if(index != -1) IR_col_new_u2[i+1] = a * summation[3] + (1-a)*sparse_critics[1].getHeap()[index];   //rating of i+1-th movie of user 2
                    else IR_col_new_u2[i+1] = a * summation[3];
                    
                    index = binarySearch(i,user+2);
                    if(index != -1) IR_col_new_u3[i]   = a * summation[4] + (1-a)*sparse_critics[2].getHeap()[index];     //rating of i-th movie of user 3 
                    else IR_col_new_u3[i] = a * summation[4];
                    
                    index = binarySearch(i+1,user+2);
                    if(index != -1) IR_col_new_u3[i+1] = a * summation[5] + (1-a)*sparse_critics[2].getHeap()[index];   //rating of i+1-th movie of user 3
                    else IR_col_new_u3[i+1] = a * summation[5];
                    
                    index = binarySearch(i,user+3);
                    if(index != -1) IR_col_new_u4[i] = a * summation[6] + (1-a)*sparse_critics[3].getHeap()[index];     //rating of i-th movie of user 4
                    else IR_col_new_u4[i] = a * summation[6];
                    
                    index = binarySearch(i+1,user+3);
                    if(index != -1) IR_col_new_u4[i+1] = a * summation[7] + (1-a)*sparse_critics[3].getHeap()[index];   //rating of i+1-th movie of user 4
                    else IR_col_new_u4[i+1] = a * summation[7];
                    
                }
                
                UPDATE_IR:for(int j = 0; j < m; j++) {
                	IR_col_u1[j] = IR_col_new_u1[j];
					IR_col_u2[j] = IR_col_new_u2[j];	
					IR_col_u3[j] = IR_col_new_u3[j];	
					IR_col_u4[j] = IR_col_new_u4[j];		
                }
            }
            
            // write to output
            for(int i = 0; i < 4; i++){
            	max_elem[i] = 0;
            	max_elem_indices[i] = 0;
			}
            WRITE_OUTPUT:for(int i = 0; i < m; i++){
            	if(IR_col_u1[i] > max_elem[0] && initial_critics[i][user]==0) {
            		max_elem[0] = IR_col_u1[i];
            		max_elem_indices[0] = i; 
				}
            	if(IR_col_u2[i] > max_elem[1] && initial_critics[i][user+1]==0){
            		max_elem[1] = IR_col_u2[i];
            		max_elem_indices[1] = i; 
				} 
            	if(IR_col_u3[i] > max_elem[2] && initial_critics[i][user+2]==0) {
            		max_elem[2] = IR_col_u3[i];
            		max_elem_indices[2] = i;
				}
            	if(IR_col_u4[i] > max_elem[3] && initial_critics[i][user+3]==0){
            		max_elem[3] = IR_col_u4[i];
            		max_elem_indices[3] = i;
				} 
            }
            
            if(user < num_users_to_be_computed-4){
	            IR_output[user] = max_elem_indices[0];
	            IR_output[user+1] = max_elem_indices[1];
	            IR_output[user+2] = max_elem_indices[2];
	            IR_output[user+3] = max_elem_indices[3];
        	}
        	else{
        		for(int o=0; o<mod_users; o++){
        			IR_output[user+o] = max_elem_indices[o];
				}
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
			
		    int IR_new[n];
		    // create object and run Itemrank algorithm
		    itemrank obj;
		    obj.run(movies_correlation,initial_critics,IR_new);
		    
		    //write to output file
		    out << "Test No: "<< num_tests << "\n";
		    for (int i=0; i<m; i++){                       
			    out << IR_new[i] << "\t";
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
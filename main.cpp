#include <iostream>

#define n 4 // number of users
#define m 4 // number of movies

// define arrays used in program
float IR[m][n];
float movies_correlation[m][m]= { // denoted in the paper as C
        {0.0,   1.0/6, 1.0/4, 1.0/2},
        {1.0/3, 0,     2.0/4, 0.0},
        {1.0/3, 2.0/6, 0,     0.0},
        {1.0/3, 3.0/6, 1.0/4, 1/2}
};
//normalized movies ratings
float initial_critics[n][m]= { //array with columns the vector d of each user
        {1.0/6, 1.0/12, 0,     4.0/9},
        {1.0/6, 2.0/12, 4.0/7, 1.0/9},
        {4.0/6, 5.0/12, 3.0/7, 1.0/9},
        {0.0,   0.0,    4.0/12, 3.0/9}
};


class itemrank{
private:
    int max_iter = 10000, a = 0.85;
    float summation = 0;
public:
    // constructor
    itemrank(float C[m][m], float D[n][m]) {
        // initialize IR matrix
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                IR[j][i] = 1.0/m;
            }
        }
    };

    void run(){

        for(int user = 0; user < n; user++) { //for each user
            for (int t = 0; t < max_iter; t++) { //for each time
                for(int i = 0; i < m; i++) {        //for each movie
                    summation = 0;
                    for(int j = 0; j < m; j++) {
                        summation = summation + movies_correlation[i][j] * IR[user][j];
                    }
                    IR[user][i] = a * summation + (1-a)*initial_critics[user][i]; //rating of i-th movie of user
                }
            }
        }
    }
};


int main() {

    // create object and run Itemrank algorithm
    itemrank obj(movies_correlation,initial_critics);
    obj.run();

    // print results
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            std::cout << IR[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}

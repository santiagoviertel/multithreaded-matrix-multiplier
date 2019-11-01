#include <boost/chrono.hpp>
#include <random>
#include <thread>
#include <iostream>
#include <iomanip>

#define SIZE 800

using namespace std;
using namespace boost::chrono;

void generateRandomMatrices(int m1[SIZE][SIZE],int m2[SIZE][SIZE]) {
	random_device r;
	default_random_engine e(r());
	uniform_int_distribution<int> distribution(-20,20);
	for(unsigned int i=0;i<SIZE;++i)
		for(unsigned int j=0;j<SIZE;++j) {
			m1[i][j] = distribution(e);
			m2[i][j] = distribution(e);
		}
}

void showMatrix(int m[SIZE][SIZE]) {
	for(unsigned int i=0;i<SIZE;++i) {
		for(unsigned int j=0;j<SIZE;++j)
			cout << setw(3) << m[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}

void fillRowThread(int m1[SIZE][SIZE],int m2[SIZE][SIZE],int r[SIZE][SIZE],unsigned int row) {
	for(unsigned int i=0;i<SIZE;++i) {
		r[row][i] = 0;
		for(unsigned int j=0;j<SIZE;++j)
			r[row][i] += m1[row][j]*m2[j][i];
	}
}

void multiplyMatricesThreadPerRow(int m1[SIZE][SIZE],int m2[SIZE][SIZE],int r[SIZE][SIZE]) {
	unsigned int i;
	thread t[SIZE];
	for(i=0;i<SIZE;++i)
		t[i] = thread(fillRowThread,m1,m2,r,i);
	for(i=0;i<SIZE;++i)
		t[i].join();
}

void multiplyMatricesWithoutThread(int m1[SIZE][SIZE],int m2[SIZE][SIZE],int r[SIZE][SIZE]) {
	unsigned int i,j,k;
	for(i=0;i<SIZE;++i) {
		for(j=0;j<SIZE;++j) {
			r[i][j] = 0;
			for(k=0;k<SIZE;++k)
				r[i][j] += m1[i][k]*m2[k][j];
		}
	}
}

int main() {
	int m1[SIZE][SIZE],m2[SIZE][SIZE],r[SIZE][SIZE];
	high_resolution_clock::time_point p1,p2;

	cout << "Time to multiply two integer random matrices of size " << SIZE << "x" << SIZE << endl;
	generateRandomMatrices(m1,m2);

	p1 = high_resolution_clock::now();
	multiplyMatricesThreadPerRow(m1,m2,r);
	p2 = high_resolution_clock::now();
	cout << "One thread per row:\t" << duration_cast<milliseconds>(p2-p1) << "\n";

	p1 = high_resolution_clock::now();
	multiplyMatricesWithoutThread(m1,m2,r);
	p2 = high_resolution_clock::now();
	cout << "Without threads:\t" << duration_cast<milliseconds>(p2-p1) << "\n";

	return 0;
}
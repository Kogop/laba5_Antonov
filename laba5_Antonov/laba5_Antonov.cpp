#include <iostream>
#include <mpi.h>
#include <fstream>
//#include <math.h>
#include <cmath>
#include <stdlib.h>
using namespace std;

const int root = 0, tag = 0;
const int n = 100, m = 150;
double A[n][m], B[m][n], v[n], d[n], C[n][n], A1[n][m], B1[m][n], v1[n];

void FillMatrix(double(&AA)[n][m], double(&BB)[m][n]) {
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++) {
			AA[i][j] = rand() % 100 / 2.4;
		}
	}

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++) {
			BB[i][j] = rand() % 100 / 2.4;
		}
	}
}
/*
void FillVector(double v1[n]) {
	for (int j = 0; j < n; j++) {
		v1[j] = 1/*rand() % 100 / 2.4;
	}
}
*/

void Matrix_Peremnoj(double(&AA)[n][m], double(&BB)[m][n]) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = 0;
			for (int k = 0; k < m; k++) {
				C[i][j] += AA[i][k] * BB[k][j];
			}
		}
	}
}
void Matrix_Peremnoj_na_vector(double(&AA)[n][m], double(&vv)[n]) {
	for (int i = 0; i < n; i++) {
		d[i] = 0;
		for (int j = 0; j < m; j++) {
			d[i] += vv[i] * AA[i][j];
		}
	}
}

void Zapis_v_File() {
	ofstream File1("C:\\Users\\neste\\source\\Repos\\SuperCompModel\\laba2\\laba2\\Matrix_1.txt");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++) {
			File1 << A[i][j] << " ";
		}
		File1 << endl;
	}
	File1.close();

	ofstream File2("C:\\Users\\neste\\source\\Repos\\SuperCompModel\\laba2\\laba2\\Matrix_2.txt");
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++) {
			File2 << B[i][j] << " ";
		}
		File2 << endl;
	}
	File2.close();

	ofstream File4("C:\\Users\\neste\\source\\Repos\\SuperCompModel\\laba2\\laba2\\Vector_1.txt");
	for (int i = 0; i < n; i++)
	{
		File4 << v[i] << endl;
	}
	File4.close();
}

void Zapix_otvetov_v_File(double(&CC)[n][n]) {
	ofstream File3("C:\\Users\\neste\\source\\Repos\\SuperCompModel\\laba2\\laba2\\Matrix_Otvet1.txt");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++) {
			File3 << CC[i][j] << "\t";
		}
		File3 << "\n";
	}
	File3.close();
	cout << " c zapisannaya --" << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++) {
			cout << CC[i][j] << " ";
		}
		cout << endl;
	}

	//ofstream File5("C:\\Users\\neste\\source\\Repos\\SuperCompModel\\laba2\\laba2\\Vector_Otvet1.txt");
	//for (int i = 0; i < n; i++)
	//{
	//	File5 << d[i] << endl;
	//}
	//File5.close();
}

void read_Vector() {
	ifstream File5("C:\\Users\\neste\\source\\Repos\\SuperCompModel\\laba2\\laba2\\Vector_1.txt");
	for (int i = 0; i < n; i++) {
		File5 >> v1[i];
		//  cout << DD[i]<<endl;
	}
	File5.close();
}

void read_Matrix() {
	ifstream File1("C:\\Users\\neste\\source\\Repos\\SuperCompModel\\laba2\\laba2\\Matrix_1.txt");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++) {
			File1 >> A1[i][j];
			//cout << AA[i][j] << " ";
		}
		// cout << endl;
	}
	File1.close();

	ifstream File2("C:\\Users\\neste\\source\\Repos\\SuperCompModel\\laba2\\laba2\\Matrix_2.txt");
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++) {
			File2 >> B1[i][j];
			//cout << AA[i][j] << " ";
		}
		// cout << endl;
	}
	File2.close();
}

double k[m];
double l[m];

void vzat_vector_iz_matrix(double(&AA)[n][m], int s1, double(&BB)[m][n], int s2/*, int c*/) {
	// if c == 1 znachit berem stroku if c== 2 znachit berem stolbec
	// s eto nomer stroki ili stolbca kotoriy nuzhno vzat
   // if(c==1){
	for (int j = 0; j < m; j++) {
		k[j] = AA[s1][j];
		//cout << k[j] << endl;
	}
	//  }else if(c==2){
	for (int j = 0; j < m; j++) {
		l[j] = BB[j][s2];
		//cout << l[j] << endl;
	}
	//}
}

double peremnoj_vector_na_vector(double(&kk)[m], double(&ll)[m]) {
	double h = 0;
	for (int i = 0; i < m; i++) {
		h += kk[i] * ll[i];
	}
	return h;
}

double Temp[] = { 0,0,0 };
int Mesto[] = { 0,0 };
int N = 0, M = 0, Nn = 0, Mm = 0, limit_1 = 0;

// make matrix multiplying with MPI_Reduce or MPI_AllReduce 
int main() {
	MPI_Init(NULL, NULL);
	double starttime, endtime;
	starttime = MPI_Wtime();

	int rank, size, limit, end, end_1_otprav = 0, end_1_priem = 0, h = 0, g = 0;
	end = 0;

	MPI_Status status;
	//MPI_Request request;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	endtime = MPI_Wtime();
	printf("vipolnenie zanyalo %f seconds\n", endtime - starttime);
	MPI_Finalize();
	//MPI_Finalize();
	return 1;
};

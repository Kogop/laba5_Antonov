
#include <iostream>
#include <mpi.h>
#include <fstream>
//#include <math.h>
#include <cmath>
#include <stdlib.h>
using namespace std;

const int root = 0, tag = 0;
const int n = 4, m = 4;
double A[n][m], B[m][n], v[n], d[n], C[n][n], A1[n][m], B1[m][n], v1[n];

void FillMatrix(double(&AA)[n][m], double(&BB)[m][n]) {
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++) {
			AA[i][j] = 1/*random() % 100 / 2.4*/;
		}
	}

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++) {
			BB[i][j] = 1/*random() % 100 / 2.4*/;
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
	ofstream File1("Matrix_1.txt");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++) {
			File1 << A[i][j] << " ";
		}
		File1 << endl;
	}
	File1.close();

	ofstream File2("Matrix_2.txt");
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++) {
			File2 << B[i][j] << " ";
		}
		File2 << endl;
	}
	File2.close();

	ofstream File4("Vector_1.txt");
	for (int i = 0; i < n; i++)
	{
		File4 << v[i] << endl;
	}
	File4.close();
}

void Zapix_otvetov_v_File(double(&CC)[n][n]/*,double(&d)[n]*/) {
	ofstream File3("Matrix_Otvet1.txt");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++) {
			File3 << CC[i][j] << " ";
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

	/*ofstream File5("/home/vc/18VF1/laba2-master/laba2_try3/Vector_Otvet1.txt");
	for (int i = 0; i < n; i++)
	{
		File5 << d[i] << endl;
	}
	File5.close();*/
}

void read_Vector() {
	ifstream File5("Vector_1.txt");
	for (int i = 0; i < n; i++) {
		File5 >> v1[i];
		//  cout << DD[i]<<endl;
	}
	File5.close();
}

void read_Matrix() {
	ifstream File1("Matrix_1.txt");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++) {
			File1 >> A1[i][j];
			//cout << AA[i][j] << " ";
		}
		// cout << endl;
	}
	File1.close();

	ifstream File2("Matrix_2.txt");
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

void vzat_vector_iz_matrix(/*double(&AA)[n][m], int s1,*/ double(&BB)[m][n], int s2/*, int c*/) {
	// if c == 1 znachit berem stroku if c== 2 znachit berem stolbec
	// s eto nomer stroki ili stolbca kotoriy nuzhno vzat

	//if(c==1){
	//for (int j = 0; j < m; j++) {
	//	k[j] = AA[s1][j];
		//cout << k[j] << endl;
	//}
	 // }else if(c==2){
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

double Temp[n];
int Mesto[] = { 0,0 };
int N = 0, M = 0, Nn = 0, Mm = 0, limit_1 = 0;
//int kol_strok_v_posled_str_bloke = 0;
//int kol_stolb_v_posled_stolb_bloke = 0;
//bool is_n_menwe_size;

double Temp_dlyua_sloj[n][m];



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


	if (rank == 0) {
		FillMatrix(A, B);
		//FillVector(v);
		Zapis_v_File();
		//read_Vector();
		read_Matrix();
	}

	double  rbufA[m], rbufB[m]; int gsize;
	MPI_Comm_size(MPI_COMM_WORLD, &gsize);

	MPI_Bcast(B1, n*m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(A1, m, MPI_DOUBLE, rbufA, m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	//MPI_Scatter(B1, m, MPI_DOUBLE, rbufB, m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	double buff[1000];
	//MPI_Buffer_attach(&buff, n * m * sizeof(double));
	double buff2[1000];
	//MPI_Buffer_attach(&buff2, n * m * sizeof(double));
	
	// проверка работы русского языка
	/*  крч смотри, на каждом процессе есть нужная строка и вся матрица вторая, оно берет нужные строки и столбцы, 
		надо просто с помощью редюса взять и перемножить строку на столбец, получится строка, её элементы надо
		сложить, получится элемент искомой матрицы, надо только забить его местоположение сразу, чтобы поставить на нужное место
		так что 
		TODO:
		1) написать функцию что будет складывать элементы полученной строки
		2) записывать ответ в правильное место
		
		*/

	for (int i = 0; i < 4; i++) {
		//double kk[m],kkk[m];
		vzat_vector_iz_matrix(B1, i);
		//cout <<rbufB[i]<<endl;
		Temp[i] = peremnoj_vector_na_vector(rbufA, l);

		cout << Temp[i] << " ";

	}

	cout << endl;
	double rbuf[n * n];
	MPI_Gather(&Temp[0], 4, MPI_DOUBLE, rbuf, 4, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	if (rank == 0) {

		//cout <<"joparabotai"<< 228<<endl;
		int g = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {

				cout << rbuf[g] << endl;

				C[i][j] = rbuf[g];

				fflush(stdout);
				g++;
			}

		}
		Zapix_otvetov_v_File(C/*,d*/);

	}








	

	endtime = MPI_Wtime();
	printf("vipolnenie zanyalo %f seconds\n", endtime - starttime);
	MPI_Finalize();
	//MPI_Finalize();
	return 1;
};

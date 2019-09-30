// Proyecto_Tortas.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;
Mat m1, tortaLimpia, tortaLimpia_binaria;
vector<Mat> vector_mat1(3);
Point2f centroTorta=Point2f(0.0,0.0);


void limpiarImagen();
int estaBienCocinada();
int analizarForma();
void estaBuena(bool mostrarPunto);
void estaAgujereada();
Mat ejercicioAmpliacion(Mat mAmpliacion, bool mostrarPunto);

int _tmain(int argc, _TCHAR* argv[])
{
	//carga la imagen indicada y la devuelve en una matriz 
	Mat mbuena, mcruda, mrota, mdeformada, mquemada, magujero, mindeterminada;
	Mat mAmpliacion1, mAmpliacion2, mAmpliacion3, mAmpliacion4, mAmpliacion5;
	Mat menlace;
	clock_t start;
	double duracion;

	mbuena = imread("../Tortas Unitarias/Tortas buenas/B1.BMP"); 
	mcruda = imread("../Tortas Unitarias/Tortas malas/Crudas/C4.BMP"); 
	mrota = imread("../Tortas Unitarias/Tortas malas/Rotas/R4.BMP"); 
	mdeformada = imread("../Tortas Unitarias/Tortas malas/Deformadas/D1.BMP"); 
	mquemada = imread("../Tortas Unitarias/Tortas malas/Quemadas/Q3.BMP"); 
	magujero = imread("../Tortas Unitarias/Tortas malas/Agujereadas/A2.BMP"); 
	mindeterminada = imread("../Tortas Unitarias/Tortas indeterminadas/I3.BMP"); 

	mAmpliacion1 = imread("../Tortas Ampliación/Imagen06-22-2016 - 17.11.25.469.BMP"); 
	mAmpliacion2 = imread("../Tortas Ampliación/Imagen06-22-2016 - 17.11.47.569.BMP"); 
	mAmpliacion3 = imread("../Tortas Ampliación/Imagen06-23-2016 - 14.31.41.956.BMP"); 
	mAmpliacion4 = imread("../Tortas Ampliación/Imagen06-23-2016 - 14.33.12.250.BMP"); 
	mAmpliacion5 = imread("../Tortas Ampliación/Imagen06-23-2016 - 14.36.33.505.BMP"); 

	bool tortasAmpliacion = false;  //false -> ejercicio normal;  true -> ejercicio ampliacion

	if (!tortasAmpliacion){

		for (int contImg=1; contImg < 13; contImg++){

			menlace = imread("../Tortas Unitarias/Tortas indeterminadas/I" + to_string((long long)contImg) + ".BMP");

			m1=menlace; //Seleccionamos el grupo de imagenes a analizar

			//comprueba que ha podido leer la imagen 
			if (m1.empty()){ cout << "Error : Image 1 cannot be loaded..!!" << endl; break; } 
			cout << "Torta " << contImg << endl;

			start = clock();

			limpiarImagen();
			int forma = analizarForma(); //0 Buena, 1 Posible Agujereada, 2 Rota, 3 Deformada
			if (forma == 2) {
				cout << "Esta rota" << endl;
			}else if (forma == 3) {
				cout << "Esta deformada" << endl;
			}else {
				int bienCocinada = estaBienCocinada(); //0 Buena, 1 Cruda, 2 Quemada
				if (bienCocinada == 1){
					cout << "Esta cruda" << endl;
				} else if (bienCocinada == 2){
					cout << "Esta quemada" << endl;
				}else if (forma == 1) {
					estaAgujereada();
					cout << "Esta agujereada" << endl;
				}else {
					estaBuena(true);  //Escribe el centro de la torta;  Con true o false, ademas indicamos si queremos mostrar el centro de la torta como un punto
					cout << "Es buena" << endl;
				}
			}
			duracion = (clock() - start) / (double)CLOCKS_PER_SEC;
			cout << "duracion: " << duracion << '\n'; 
			imshow("Torta " + to_string((long long)contImg),m1);
		} //for
	}else{  // AMPLIACION
		if (mAmpliacion1.empty()){ cout << "Error : Image 1 cannot be loaded..!!" << endl; return -1; } 
		if (mAmpliacion2.empty()){ cout << "Error : Image 2 cannot be loaded..!!" << endl; return -1; } 
		if (mAmpliacion3.empty()){ cout << "Error : Image 3 cannot be loaded..!!" << endl; return -1; } 
		if (mAmpliacion4.empty()){ cout << "Error : Image 4 cannot be loaded..!!" << endl; return -1; } 
		if (mAmpliacion5.empty()){ cout << "Error : Image 5 cannot be loaded..!!" << endl; return -1; } 

		start = clock();
		mAmpliacion1 = ejercicioAmpliacion(mAmpliacion1, true); //Con true o false, indicamos si queremos mostrar el centro de las tortas como un punto
		duracion = (clock() - start) / (double)CLOCKS_PER_SEC;
		cout << "duracion: " << duracion << '\n'; 
		imshow("Torta ampliacion 1",mAmpliacion1);
		start = clock();

		mAmpliacion2 = ejercicioAmpliacion(mAmpliacion2, true);
		duracion = (clock() - start) / (double)CLOCKS_PER_SEC;
		cout << "duracion: " << duracion << '\n'; 
		imshow("Torta ampliacion 2",mAmpliacion2);

		start = clock();
		mAmpliacion3 = ejercicioAmpliacion(mAmpliacion3, true);
		duracion = (clock() - start) / (double)CLOCKS_PER_SEC;
		cout << "duracion: " << duracion << '\n'; 
		imshow("Torta ampliacion 3",mAmpliacion3);

		start = clock();
		mAmpliacion4 = ejercicioAmpliacion(mAmpliacion4, true);
		duracion = (clock() - start) / (double)CLOCKS_PER_SEC;
		cout << "duracion: " << duracion << '\n'; 
		imshow("Torta ampliacion 4",mAmpliacion4);

		start = clock();
		mAmpliacion5 = ejercicioAmpliacion(mAmpliacion5, true);
		duracion = (clock() - start) / (double)CLOCKS_PER_SEC;
		cout << "duracion: " << duracion << '\n'; 
		imshow("Torta ampliacion 5",mAmpliacion5);
		
	}


	//Esperamos a presionar una tecla
	waitKey(0);

	return 0;
}

void limpiarImagen(){
	vector<Mat> vector_mat2(3);
	split(m1,vector_mat2);
	Mat dst;
	/// Call the function to initialize
	threshold(vector_mat2[2], dst, 40, 255, CV_THRESH_BINARY_INV);

	tortaLimpia_binaria = dst;
	vector<Mat> vector_mat_merge = vector_mat2;

	vector_mat_merge[0]=vector_mat2[0]-dst;
	vector_mat_merge[1]=vector_mat2[1]-dst;
	vector_mat_merge[2]=vector_mat2[2]-dst;

	cv::merge(vector_mat_merge,tortaLimpia);
}

int estaBienCocinada (){

	//valor máximo de píxel 
	int histSize = 256;
	//rango de píxels 
	float range[] = { 0, 256 }; 
	const float* histRange = { range };

	int hist_w = 512; int hist_h = 400;

	Mat b_hist1, g_hist1, r_hist1;


	int minr1=99999;
	int midg1=0;
	int midr1=0;

	split(tortaLimpia,vector_mat1);

	//calculo del histograma 
	//calcHist(&vector_mat1[0], 1, 0, Mat(), b_hist1, 1, &histSize, &histRange, uniform, accumulate); //azul
	calcHist(&vector_mat1[1], 1, 0, Mat(), g_hist1, 1, &histSize, &histRange); //verde
	calcHist(&vector_mat1[2], 1, 0, Mat(), r_hist1, 1, &histSize, &histRange); //rojo

	Mat histImage1(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	for (int i = 10; i < histSize-10; i++) {   //evito coger los bordes del histograma para evitar incorrecciones

		if (cvRound(g_hist1.at<float>(i - 1)) > 0) midg1=midg1+cvRound(g_hist1.at<float>(i - 1));
		if (cvRound(r_hist1.at<float>(i - 1)) > 0) midr1=midr1+cvRound(r_hist1.at<float>(i - 1));
		if (cvRound(r_hist1.at<float>(i - 1)) > 0 && minr1 > cvRound(r_hist1.at<float>(i - 1))) minr1=cvRound(r_hist1.at<float>(i - 1));

	}

	midg1 = midg1/(histSize-21);
	midr1 = midr1/(histSize-21);

	/*
	cout << "midg1 " << midg1 << endl;
	cout << "midr1 " << midr1 << endl;
	cout << "minr1 " << minr1 << endl;
	*/
	//cruda:
	//*max g: 140-185        **media:  25-45
	//*max r: 50-70          *media:  12-16    min: 3-5

	//quemada:
	//min g:          media: 100-125
	//min r: 9-14     media: 40-65

	//normal
	//g:			   media: 65-115
	//*min r: 5-8      media: 20-45


	if (midg1 < 55) {
		putText(m1,"Cruda",Point(20,30),FONT_HERSHEY_DUPLEX,1,Scalar(0,0,255));
		//cout << "Cruda" << endl;
		return 1;
	}
	if (minr1 >= 9 && minr1 <= 20 && midg1 < 126 && midg1 > 99 && ((midg1 > 120 && midr1 >50) || midg1 <= 120)) {
		putText(m1,"Quemada",Point(20,30),FONT_HERSHEY_DUPLEX,1,Scalar(0,0,255));
		//cout << "Quemada" << endl;
		return 2;
	}
	return 0;
}

int analizarForma(){
	int thresh = 10;
	Mat canny_output;
	Mat src_gray;

	GaussianBlur( tortaLimpia_binaria, src_gray, Size(9, 9), 1.5, 1.5 );
	//imshow( "circles2", src_gray );
	Canny( src_gray, canny_output, thresh, thresh*2, 3, true );
	//imshow( "circles22222", canny_output );
	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;
	findContours(canny_output, contours, hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	//cout << "contornos: " << contours.size() << endl;
	//cout << hierarchy.size() << endl;
	vector<Point2f> centro_agujero(contours.size());
	vector<float> radio_agujero(contours.size());

	for (unsigned int i = 0;  i < contours.size();  i++)
	{
		if (contours[i].size()>0 && contours[i].size()<5000){

			//cout << "jerarquia: " << hierarchy[i] << endl;

			double area = contourArea(contours[i]);
			//cout << " Area: " << area << endl;

			if (area > 15000 && area < 38000){
				putText(m1,"Deformada",Point(20,30),FONT_HERSHEY_DUPLEX,1,Scalar(0,0,255));
				//cout << "Deformada " << area << endl;
				return 3;
			}

			if ( i>0 && (hierarchy[i][3]==(hierarchy[i-1][3]+1) && hierarchy[i][2]==(hierarchy[i-1][2]+1) && contourArea(contours[i-1]) > 20000)){
				minEnclosingCircle(contours[i],centro_agujero[i],radio_agujero[i]);
				//circle( m1, centro_agujero[i], (int)radio_agujero[i], Scalar(0,0,255), 3);
				//cout << "Agujero: " << radio_agujero[i] << endl;
				//imshow( "circle", m1 );
				//putText(m1,"Agujereada",Point(20,30),FONT_HERSHEY_DUPLEX,1,Scalar(0,0,255));
				return 1;
			}
			/*
			else{
			minEnclosingCircle(contours[i],centro_agujero[i],radio_agujero[i]);
			circle( m1, centro_agujero[i], (int)radio_agujero[i], Scalar(0,255,0), 3);
			//cout << "Agujero: " << radio_agujero[i] << endl;
			}*/


			if (area>38000){
				//cout << " Area: " << area << endl;
				vector<float> dist(contours[i].size());
				bool estaRota=false;
				bool escalonEncontrado=false;
				//cout << contours[i].size() << endl;
				unsigned int intervalo = int(contours[i].size()*0.01);
				minEnclosingCircle(contours[i],centro_agujero[i],radio_agujero[i]);

				for (unsigned int j=0; j<contours[i].size();j++){
					dist[j] = sqrt(float((contours[i][j].x-centro_agujero[i].x)*(contours[i][j].x - 
						centro_agujero[i].x) + (contours[i][j].y-centro_agujero[i].y)*(contours[i][j].y -
						centro_agujero[i].y)));
					//cout << dist[j] <<endl;
					if (j>intervalo && abs(dist[j]-dist[j-intervalo])>dist[j]*0.05) {
						escalonEncontrado = true;
						//cout << "ROTA " << abs(dist[j]-dist[j-intervalo]) << " >" << dist[j]*0.05 <<endl;
						if (escalonEncontrado){
							putText(m1,"Rota",Point(20,30),FONT_HERSHEY_DUPLEX,1,Scalar(0,0,255));
							return 2;
						}
					}
				}

				if (!estaRota){
					centroTorta = centro_agujero[i];
				}
				//imshow( "circle", m1 );	
			}
		}

	} //for


	return 0;
}

void estaBuena(bool mostrarPunto){
	putText(m1,format("(%d,%d)", (int)centroTorta.x, (int)centroTorta.y),Point(20,30),FONT_HERSHEY_DUPLEX,1,Scalar(0,0,255));
	if (mostrarPunto) circle( m1, centroTorta, 1, Scalar(0,0,255), 3);  //mostrar centro de la torta
}

void estaAgujereada(){
	putText(m1,"Agujereada",Point(20,30),FONT_HERSHEY_DUPLEX,1,Scalar(0,0,255));
}

Mat ejercicioAmpliacion(Mat mAmpliacion, bool mostrarPunto){

	int numTortas=0;
	Mat src_gray, dst;
	vector<Mat> vector_mat(3);
	double area = 0.0;
	int thresh = 10;
	Mat canny_output;
	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;

	split(mAmpliacion, vector_mat);
	/// Call the function to initialize
	threshold(vector_mat[2], src_gray, 123, 10, CV_THRESH_BINARY_INV);
	GaussianBlur( src_gray, dst, Size(9, 9), 0.8, 0.8 );
	Canny( dst, canny_output, thresh, thresh*2, 3, true );

	findContours(canny_output, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	vector<Point2f> centro_agujero(contours.size());
	vector<float> radio_agujero(contours.size());

	for (unsigned int i = 0;  i < contours.size();  i++){
		area = contourArea(contours[i],true);
		//cout << "area: " << area << endl;
		if (contours[i].size()>50 && contours[i].size()<5000 && area > 20000 && area < 45000){
			//cout << "area: " << area << endl;
			numTortas++;
			minEnclosingCircle(contours[i],centro_agujero[i],radio_agujero[i]);
			//circle( mAmpliacion, centro_agujero[i], (int)radio_agujero[i], Scalar(0,0,255), 3);
			putText(mAmpliacion,format("(%d,%d)", (int)centro_agujero[i].x, (int)centro_agujero[i].y),Point(20,numTortas*40),FONT_HERSHEY_DUPLEX,1,Scalar(0,0,255));
			if (mostrarPunto) circle( mAmpliacion, centro_agujero[i], 1, Scalar(255,255,0), 3);  //mostrar centro de la torta
		}
	}
	return mAmpliacion;
	
}


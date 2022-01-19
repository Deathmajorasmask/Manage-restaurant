#include <windows.h>
#include <string>
#include <CommDlg.h>
#include "resource.h"
#include <mmsystem.h>
#include <fstream>
#include <string.h>
#include <ctime>
#include <windowsx.h>
#include <cstddef>
#pragma comment(lib, "winmm.lib")

using namespace std;

struct almacen
{
	char numero[50];
	char nombre[50];
	char fecha[20];
	int cantidad;
	bool tipo;
	almacen *sig;
	almacen *ant;
};

struct platillos
{
	char numero[50];
	char nombre[50];
	int precio;
	bool disponible;
	char imagen[260];
	char ingredientes[260];
	char descripcion[260];
	platillos *sig;
	platillos *ant;
};

struct membresia 
{
	char numero[50];
	char nombre[50];
	char emision[20];
	char expedicion[20];
	char imagen[260];
	membresia *sig;
	membresia *ant;

};

struct pedidos
{
	char nummem[50];
	char nombre[50];
	char mesa[20];
	char emi[20];
	int pagar;
	char selecc[260];
	char anotaciones[260];
	pedidos * sig;
	pedidos * ant;
};

//Variable Globales
pedidos *listaped = NULL;
membresia *listam = NULL;
almacen * listaa = NULL;
platillos * listap = NULL;
bool BanderaModPla = false;

//funciones para el tiempo
time_t rawtime;
struct tm *timeinfo;

// Dialog handle.
HWND ghDlg = 0;
HWND ghMainWnd = 0;
HINSTANCE ghAppInst = 0;

//Ordenamiento de membresia
void quickSort(struct membresia *head);
void _quickSort(struct membresia* l, struct membresia *h);
membresia* partition(membresia *l, membresia *h);
void swap(char* num1, char* nom1, char* emi1, char* exp1, char* img1, char* num2, char* nom2, char* emi2, char* exp2, char* img2);
struct membresia *lastNode(membresia *root);

//Ordenamiento de Almacen
struct almacen *lastNodeA(almacen *root);
void swapA(char* num1, char* nom1, char* fecha1, int* cant1, bool* tipo1, char* num2, char* nom2, char* fecha2, int* cant2, bool* tipo2);
almacen* partitionA(almacen *l, almacen *h);
void _quickSortA(struct almacen* l, struct almacen *h);
void quickSortA(struct almacen *head);

//Ordenamiento de Pedidos
struct pedidos *lastNodeP(pedidos *root);
void swapP(char* num1, char* nom1, char* mesa1, char* emi1, int* pagar1, char* selecc1, char* anota1, char* num2, char* nom2, char* mesa2, char* emi2, int* pagar2, char* selecc2, char* anota2);
pedidos* partitionP(pedidos *l, pedidos *h);
void _quickSortP(struct pedidos* l, struct pedidos *h);
void quickSortP(struct pedidos *head);

//Ordenamiento de Platillo
struct platillos *lastNodePl(platillos *root);
void swapPl(char* num1, char* nom1, int* prec1, bool* disp1, char* img1, char* ing1, char* descp1, char* num2, char* nom2, int* prec2, bool* disp2, char* img2, char* ing2, char* descp2);
platillos* partitionPl(platillos *l, platillos *h);
void _quickSortPl(struct platillos* l, struct platillos *h);
void quickSortPl(struct platillos *head);

//ordenamiento Heapsorts
void heapsort(int a[], int n);
void makeheap(int a[], int n);

BOOL CALLBACK AboutDlg(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgIndex(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgADM_ALM_AGR(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgADM_ALM_MOD(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgADM_MEM_AGR(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgADM_MEM_MOD(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgADM_PLA_AGR(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgADM_PLA_MOD(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgCON_ALM(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgCON_MEM(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgCON_PLA(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgCON_PED(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgREG_PED(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
char *AbrirArchivos(HWND Dialogo);
string obtenerDireccion(string filename);

//------HACE QUE FUNCIONE TODO (VOID MAIN)----
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	
	ghDlg = CreateDialog(
		hInstance, // Application instance.
		MAKEINTRESOURCE(IDD_LOGIN), // Dialog resource ID.
		0, // Parent window--null for no parent.
		AboutDlg); // Dialog window procedure.
					// Show the dialog.
	ShowWindow(ghDlg, showCmd);
	UpdateWindow(ghDlg);
	// Enter the message loop.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, 0, 0, 0))
	{
		
		if (ghDlg == 0 || !IsDialogMessage(ghDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
//-----------------------------------------------

//IDD_LOGIN
BOOL CALLBACK AboutDlg(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hUsuario = 0, hContraseña = 0;
	static char usu[11], pas[11];
	static membresia * auxarchmem, *auxpunmem = NULL ;
	static platillos * auxarchpla, *auxpunpla = NULL;
	static pedidos * auxarchped, *auxpunped = NULL;
	static almacen * auxarchalm, *auxpunalm = NULL;
	ifstream Archmementra;
	static string path_filemembers, path_fileplatillos, path_filepedidos, path_filealmacen;
	switch (msg)
	{

	case WM_INITDIALOG:
		hUsuario = GetDlgItem(hDlg, IDC_USU);
		hContraseña = GetDlgItem(hDlg, IDC_PASS);
		path_filemembers = obtenerDireccion("membr.bin");
		path_fileplatillos = obtenerDireccion("plat.bin");
		path_filepedidos = obtenerDireccion("ped.bin");
		path_filealmacen = obtenerDireccion("alm.bin");

		//Recuperar lista de membresia
		Archmementra.open(path_filemembers,ios::binary);
		if (Archmementra.is_open()) {
			membresia * auxnuevomem = new membresia;
			Archmementra.read((char *)auxnuevomem, sizeof(membresia));
			while (!Archmementra.eof()) {
				auxnuevomem->sig = NULL;
				auxnuevomem->ant = NULL;
				if (listam == NULL) {
					listam = auxnuevomem;
				}
				else {
					auxpunmem = listam;
					while (auxpunmem->sig != NULL){
						auxpunmem =auxpunmem->sig;
					}
					auxpunmem->sig = auxnuevomem;
					auxnuevomem->ant = auxpunmem;
				}
				auxnuevomem = new membresia;
				Archmementra.read((char *)auxnuevomem, sizeof(membresia));
			}
			Archmementra.close();
		}
		else{
			MessageBox(hDlg, "No se han registrado membresias previamente", "AVISO", MB_OK + MB_ICONINFORMATION);
		}

		//Recuperar lista de Platillos
		Archmementra.open(path_fileplatillos, ios::binary);
		if (Archmementra.is_open()) {
			platillos * auxnuevopla = new platillos;
			Archmementra.read((char *)auxnuevopla, sizeof(platillos));
			while (!Archmementra.eof()) {
				auxnuevopla->sig = NULL;
				auxnuevopla->ant = NULL;
				if (listap == NULL) {
					listap = auxnuevopla;
				}
				else {
					auxpunpla = listap;
					while (auxpunpla->sig != NULL) {
						auxpunpla = auxpunpla->sig;
					}
					auxpunpla->sig = auxnuevopla;
					auxnuevopla->ant = auxpunpla;
				}
				auxnuevopla = new platillos;
				Archmementra.read((char *)auxnuevopla, sizeof(platillos));
			}
			Archmementra.close();
		}
		else {
			MessageBox(hDlg, "No se han registrado platillos previamente", "AVISO", MB_OK + MB_ICONINFORMATION);
		}

		//Recuperar lista de pedidos
		Archmementra.open(path_filepedidos, ios::binary);
		if (Archmementra.is_open()) {
			pedidos * auxnuevoped = new pedidos;
			Archmementra.read((char *)auxnuevoped, sizeof(pedidos));
			while (!Archmementra.eof()) {
				auxnuevoped->sig = NULL;
				auxnuevoped->ant = NULL;
				if (listaped == NULL) {
					listaped = auxnuevoped;
				}
				else {
					auxpunped = listaped;
					while (auxpunped->sig != NULL) {
						auxpunped = auxpunped->sig;
					}
					auxpunped->sig = auxnuevoped;
					auxnuevoped->ant = auxpunped;
				}
				auxnuevoped = new pedidos;
				Archmementra.read((char *)auxnuevoped, sizeof(pedidos));
			}
			Archmementra.close();
		}
		else {
			MessageBox(hDlg, "No se han registrado pedidos previamente", "AVISO", MB_OK + MB_ICONINFORMATION);
		}

		//Recuperar lista de almacen
		Archmementra.open(path_filealmacen, ios::binary);
		if (Archmementra.is_open()) {
			almacen * auxnuevoalm = new almacen;
			Archmementra.read((char *)auxnuevoalm, sizeof(almacen));
			while (!Archmementra.eof()) {
				auxnuevoalm->sig = NULL;
				auxnuevoalm->ant = NULL;
				if (listaa == NULL) {
					listaa = auxnuevoalm;
				}
				else {
					auxpunalm = listaa;
					while (auxpunalm->sig != NULL) {
						auxpunalm = auxpunalm->sig;
					}
					auxpunalm->sig = auxnuevoalm;
					auxnuevoalm->ant = auxpunalm;
				}
				auxnuevoalm = new almacen;
				Archmementra.read((char *)auxnuevoalm, sizeof(almacen));
			}
			Archmementra.close();
		}
		else {
			MessageBox(hDlg, "No se han registrado productos en almacén previamente", "AVISO", MB_OK + MB_ICONINFORMATION);
		}
		return true;
		
	case WM_COMMAND:
		
		switch (LOWORD(wParam))
		{

		case IDC_ACCEDER:
			SendMessage(hUsuario, WM_GETTEXT, 10, (LPARAM)usu);
			SendMessage(hContraseña, WM_GETTEXT, 10, (LPARAM)pas);
			if (strcmp(usu, "") == 0 && strcmp(pas, "") == 0) {
				MessageBox(hDlg, "Favor de capturar", "Aviso", MB_OK);
			}else if ((strcmp(usu, "user") == 0) && (strcmp(pas, "Password")== 0) ) {
				DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_INDEX), hDlg, AboutDlgIndex);
			}
			else {
				MessageBox(hDlg, "Usuario y/o Contraseña incorrectos", "Aviso", MB_OK + MB_ICONINFORMATION);
			}
			return true;

		case IDC_SALIR:
			DestroyWindow(hDlg);
			return true;

		}
		return true;

	case WM_CLOSE:
		DestroyWindow(hDlg);
		
		return true;

	case WM_DESTROY:
		//Guardar membresias
		ofstream FileMembres;
		FileMembres.open(path_filemembers, ios::out | ios::binary | ios::trunc);
		if (FileMembres.is_open()) {
			auxarchmem = listam;
			while (auxarchmem !=NULL ) {
				FileMembres.write((char*)auxarchmem, sizeof(membresia));
				auxarchmem = auxarchmem->sig;
			}
			FileMembres.close();
		} // Finalizar guardar membresias

		//Guardar platillos
		FileMembres.open(path_fileplatillos, ios::out | ios::binary | ios::trunc);
		if (FileMembres.is_open()) {
			auxarchpla = listap;
			while (auxarchpla != NULL) {
				FileMembres.write((char*)auxarchpla, sizeof(platillos));
				auxarchpla = auxarchpla->sig;
			}
			FileMembres.close();
		} //Finalizar paltillos

		  //Guardar pedidos
		FileMembres.open(path_filepedidos, ios::out | ios::binary | ios::trunc);
		if (FileMembres.is_open()) {
			auxarchped = listaped;
			while (auxarchped != NULL) {
				FileMembres.write((char*)auxarchped, sizeof(pedidos));
				auxarchped = auxarchped->sig;
			}
			FileMembres.close();
		} //Finalizar pedidos

		  //Guardar almacén
		FileMembres.open(path_filealmacen, ios::out | ios::binary | ios::trunc);
		if (FileMembres.is_open()) {
			auxarchalm = listaa;
			while (auxarchalm != NULL) {
				FileMembres.write((char*)auxarchalm, sizeof(almacen));
				auxarchalm = auxarchalm->sig;
			}
			FileMembres.close();
		} //Finalizar pedidos



		PostQuitMessage(0);
		return true;
	}
	return false;
}

//IDD_INDEX
BOOL CALLBACK AboutDlgIndex(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char fecha[80], indauxprec[50], filenameindex[MAX_PATH];
	static HWND hfechap, hnomp, hprep, hdesp, hdisponibilidad;
	static HBITMAP imgbitind;
	static platillos * varauxpla = NULL;
	
	switch (msg)
	{
	case WM_INITDIALOG:

		hfechap = GetDlgItem(hDlg, IDC_PLA_FECHA);
		hnomp = GetDlgItem(hDlg, IDC_PLATILLO);
		hprep = GetDlgItem(hDlg, IDC_PRECIO);
		hdesp = GetDlgItem(hDlg, IDC_DESC);
		hdisponibilidad = GetDlgItem(hDlg, IDC_DISP_PLA_ED);
		
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(fecha, 80, "%d-%m-%Y", timeinfo);
		SendMessage(hfechap, WM_SETTEXT, 80, (LPARAM)fecha);
		varauxpla = listap;
		if (listap != NULL) {
			
			SendMessage(hnomp, WM_SETTEXT, 50, (LPARAM)varauxpla->nombre);
			_itoa(varauxpla->precio, indauxprec, 10);
			SendMessage(hprep, WM_SETTEXT, 50, (LPARAM)indauxprec);
			SendMessage(hdesp, WM_SETTEXT, 260, (LPARAM)varauxpla->descripcion);
			strcpy(filenameindex, varauxpla->imagen);
			imgbitind = (HBITMAP)LoadImage(NULL, filenameindex, IMAGE_BITMAP, 650, 250, LR_LOADFROMFILE);
			//Filename tiene la dirreción de la imagen
			SendDlgItemMessage(hDlg, IDC_PLA_IMGPL, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imgbitind);
			if (varauxpla->disponible == true) {
				SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"Platillo disponible");
			}
			else {
				SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"Platillo No disponible");
			}
		}

		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
	
			//-----------------
		case ID_MEN_PED_REG:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_REG_PED), hDlg, AboutDlgREG_PED);
				return true;

		case ID_MEN_PED_CON:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_CON_PED), hDlg, AboutDlgCON_PED);
			return true;

		case ID_ADM_PLA_AGR:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_ADM_PLA_AGR), hDlg, AboutDlgADM_PLA_AGR);
			return true;

		case ID_ADM_PLA_MOD:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_ADM_PLA_MOD), hDlg, AboutDlgADM_PLA_MOD);
			return true;

		case ID_ADM_MEM_AGR:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_ADM_MEM_AGR), hDlg, AboutDlgADM_MEM_AGR);
			return true;

		case ID_ADM_MEM_MOD:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_ADM_MEM_MOD), hDlg, AboutDlgADM_MEM_MOD);
			return true;

		case ID_ADM_ALM_AGR:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_ADM_ALM_AGR), hDlg, AboutDlgADM_ALM_AGR);
			return true;
			
		case ID_ADM_ALM_MOD:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_ADM_ALM_MOD), hDlg, AboutDlgADM_ALM_MOD);
			return true;

		case ID_CON_ALM:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_CON_ALM), hDlg, AboutDlgCON_ALM);
			return true;

		case ID_CON_MEM:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_CON_MEM), hDlg, AboutDlgCON_MEM);
			return true;

		case ID_CON_PLA:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_CON_PLA), hDlg, AboutDlgCON_PLA);
			return true;

		case ID_CONSULTAS_PEDIDOS:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_CON_PED), hDlg, AboutDlgCON_PED);
			return true;
			//-----------------

		case IDC_NEXT:
			if (BanderaModPla == true) {
				//Algún platillo fue eliminado
				BanderaModPla = false;
				varauxpla = listap;
				SendMessage(hnomp, WM_SETTEXT, 50, (LPARAM)"");
				SendMessage(hprep, WM_SETTEXT, 50, (LPARAM)"");
				SendMessage(hdesp, WM_SETTEXT, 260, (LPARAM)"");
				SendDlgItemMessage(hDlg, IDC_PLA_IMGPL, STM_SETIMAGE, IMAGE_BITMAP, NULL);
				SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"");
			}


			if(listap != NULL){
					/*if (varauxpla == NULL) {
						varauxpla = listap;
					}*/
					if (varauxpla->sig != NULL) {
						varauxpla = varauxpla->sig;
						SendMessage(hnomp, WM_SETTEXT, 50, (LPARAM)varauxpla->nombre);
						_itoa(varauxpla->precio, indauxprec, 10);
						SendMessage(hprep, WM_SETTEXT, 50, (LPARAM)indauxprec);
						SendMessage(hdesp, WM_SETTEXT, 260, (LPARAM)varauxpla->descripcion);
						strcpy(filenameindex, varauxpla->imagen);
						imgbitind = (HBITMAP)LoadImage(NULL, filenameindex, IMAGE_BITMAP, 650, 250, LR_LOADFROMFILE);
						//Filename tiene la dirreción de la imagen
						SendDlgItemMessage(hDlg, IDC_PLA_IMGPL, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imgbitind);
						if (varauxpla->disponible == true) {
							SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"Platillo disponible");
						}
						else {
							SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"Platillo No disponible");
						}

					}
					else {
							varauxpla = listap;
							SendMessage(hnomp, WM_SETTEXT, 50, (LPARAM)varauxpla->nombre);
							_itoa(varauxpla->precio, indauxprec, 10);
							SendMessage(hprep, WM_SETTEXT, 50, (LPARAM)indauxprec);
							SendMessage(hdesp, WM_SETTEXT, 260, (LPARAM)varauxpla->descripcion);
							strcpy(filenameindex, varauxpla->imagen);
							imgbitind = (HBITMAP)LoadImage(NULL, filenameindex, IMAGE_BITMAP, 650, 250, LR_LOADFROMFILE);
							//Filename tiene la dirreción de la imagen
							SendDlgItemMessage(hDlg, IDC_PLA_IMGPL, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imgbitind);
							if (varauxpla->disponible == true) {
								SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"Platillo disponible");
							}
							else {
								SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"Platillo No disponible");
							}
						}
			}

			return true;

		case IDC_PREV:

			if (BanderaModPla == true) {
				//Algún platillo fue eliminado
				BanderaModPla = false;
				varauxpla = listap;
				SendMessage(hnomp, WM_SETTEXT, 50, (LPARAM)"");
				SendMessage(hprep, WM_SETTEXT, 50, (LPARAM)"");
				SendMessage(hdesp, WM_SETTEXT, 260, (LPARAM)"");
				SendDlgItemMessage(hDlg, IDC_PLA_IMGPL, STM_SETIMAGE, IMAGE_BITMAP, NULL);
				SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"");
			}

			if (listap != NULL) {
				/*if (varauxpla == NULL) {
					varauxpla = listap;
				}*/
				if (varauxpla->ant != NULL) {
					varauxpla = varauxpla->ant;
					SendMessage(hnomp, WM_SETTEXT, 50, (LPARAM)varauxpla->nombre);
					_itoa(varauxpla->precio, indauxprec, 10);
					SendMessage(hprep, WM_SETTEXT, 50, (LPARAM)indauxprec);
					SendMessage(hdesp, WM_SETTEXT, 260, (LPARAM)varauxpla->descripcion);
					strcpy(filenameindex, varauxpla->imagen);
					imgbitind = (HBITMAP)LoadImage(NULL, filenameindex, IMAGE_BITMAP, 650, 250, LR_LOADFROMFILE);
					//Filename tiene la dirreción de la imagen
					SendDlgItemMessage(hDlg, IDC_PLA_IMGPL, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imgbitind);
					if (varauxpla->disponible == true) {
						SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"Platillo disponible");
					}
					else {
						SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"Platillo No disponible");
					}
				}
				else {
					while (varauxpla->sig != NULL) {
						varauxpla = varauxpla->sig;
					}
					SendMessage(hnomp, WM_SETTEXT, 50, (LPARAM)varauxpla->nombre);
					_itoa(varauxpla->precio, indauxprec, 10);
					SendMessage(hprep, WM_SETTEXT, 50, (LPARAM)indauxprec);
					SendMessage(hdesp, WM_SETTEXT, 260, (LPARAM)varauxpla->descripcion);
					strcpy(filenameindex, varauxpla->imagen);
					imgbitind = (HBITMAP)LoadImage(NULL, filenameindex, IMAGE_BITMAP, 650, 250, LR_LOADFROMFILE);
					//Filename tiene la dirreción de la imagen
					SendDlgItemMessage(hDlg, IDC_PLA_IMGPL, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imgbitind);
					if (varauxpla->disponible == true) {
						SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"Platillo disponible");
					}
					else {
						SendMessage(hdisponibilidad, WM_SETTEXT, 30, (LPARAM)"Platillo No disponible");
					}
				}
			}
			return true;

		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

//IDD_ADM_ALM_AGR
BOOL CALLBACK AboutDlgADM_ALM_AGR(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char fechaa[80], cantaux[50];
	static HWND hnuma, hnoma, hfechaa, hcanta;
	
	almacen * aux = NULL;

	switch (msg)
	{
	case WM_INITDIALOG:
		hnuma = GetDlgItem(hDlg, IDC_ALM_NUM);
		hnoma = GetDlgItem(hDlg, IDC_ALM_NOM);
		hfechaa = GetDlgItem(hDlg, IDC_ALM_EMI);
		hcanta = GetDlgItem(hDlg, IDC_ALM_CART);
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(fechaa, 80, "%d/%m/%Y", timeinfo);
		SendMessage(hfechaa, WM_SETTEXT, 80, (LPARAM)fechaa);
		//htipa = GetDlgItem(hDlg, IDC_MEM_EXP);

		return true;
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
			case IDC_RAD_ALM_COC:
			case IDC_RAD_ALM_AAL:
				CheckRadioButton(hDlg, IDC_RAD_ALM_COC, IDC_RAD_ALM_AAL, LOWORD(wParam));
			return true;

			case IDC_BOT_ALM_AGR:
				almacen * nuevo = new almacen;
				SendMessage(hnuma, WM_GETTEXT, 50, (LPARAM)nuevo->numero);
				SendMessage(hnoma, WM_GETTEXT, 50, (LPARAM)nuevo->nombre);
				SendMessage(hfechaa, WM_GETTEXT, 20, (LPARAM)nuevo->fecha);
				SendMessage(hcanta, WM_GETTEXT, 50, (LPARAM)cantaux);
				nuevo->cantidad = atoi(cantaux);
				if (IsDlgButtonChecked(hDlg, IDC_RAD_ALM_COC) == BST_CHECKED) {
					nuevo->tipo = true;
				} else {
					nuevo->tipo = false;
				}

				aux = listaa;
				if (aux != NULL) {
					while (aux->sig != NULL) {
						if (strcmp(nuevo->numero, aux->numero) == 0) {
							SendMessage(hnuma, WM_SETTEXT, 10, (LPARAM)"");
							MessageBox(hDlg, "Clave de almacén duplicada", "AVISO", MB_OK + MB_ICONERROR);
							delete nuevo;
							return true;
						}
						aux = aux->sig;
					}
				}

				if ((*(nuevo->numero) == NULL) || (*(nuevo->nombre) == NULL) || (*(nuevo->fecha) == NULL) || ((nuevo->cantidad) == NULL) ) {
					MessageBox(hDlg, "Faltan datos por capturar", "AVISO", MB_OK + MB_ICONERROR);
					delete nuevo;
				}
				else {
					
					nuevo->ant = NULL;
					nuevo->sig = NULL;
					if (listaa == NULL) {
						listaa = nuevo;
					}
					else {
						aux = listaa;
						while (aux->sig != NULL) {
							aux = aux->sig;
						}
						aux->sig = nuevo;
						nuevo->ant = aux;
					}
					MessageBox(hDlg, "Articulo de almacen agregado correctamente", "AVISO", MB_OK + MB_ICONINFORMATION);
					SendMessage(hnuma, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(hnoma, WM_SETTEXT, 50, (LPARAM)"");
					SendMessage(hcanta, WM_SETTEXT, 50, (LPARAM)"");
					//SendMessage(hemim, WM_SETTEXT, 10, (LPARAM)" ");
					//SendMessage(hexpm, WM_SETTEXT, 20, (LPARAM)" ");
				}
			return true;
		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:
		return true;
	}
	return false;
}

//IDD_ADM_ALM_MOD
BOOL CALLBACK AboutDlgADM_ALM_MOD(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char cantauxamod[50];
	static HWND hnumacmod, hnomacmod, hfechaacmod, hcantacmod;
	static almacen * auxalmmod;
	switch (msg)
	{
	case WM_INITDIALOG:
		if(listaa == NULL){
			EndDialog(hDlg, 0);
			return true;
		}
		hnumacmod = GetDlgItem(hDlg, IDC_ALM_NUMM);
		hnomacmod = GetDlgItem(hDlg, IDC_ALM_NOMM);
		hfechaacmod = GetDlgItem(hDlg, IDC_ALM_EMIM);
		hcantacmod = GetDlgItem(hDlg, IDC_ALM_CARTM);
		auxalmmod = listaa;
		SendMessage(hnumacmod, WM_SETTEXT, 50, (LPARAM)auxalmmod->numero);
		SendMessage(hnomacmod, WM_SETTEXT, 50, (LPARAM)auxalmmod->nombre);
		SendMessage(hfechaacmod, WM_SETTEXT, 20, (LPARAM)auxalmmod->fecha);
		_itoa(auxalmmod->cantidad, cantauxamod, 10);
		SendMessage(hcantacmod, WM_SETTEXT, 50, (LPARAM)cantauxamod);

		if (auxalmmod->tipo == true) {
			CheckRadioButton(hDlg, IDC_RAD_ALM_COCM, IDC_RAD_ALM_AALM, IDC_RAD_ALM_COCM);
		}
		else {
			CheckRadioButton(hDlg, IDC_RAD_ALM_COCM, IDC_RAD_ALM_AALM, IDC_RAD_ALM_AALM);
		}

		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RAD_ALM_COCM:
		case IDC_RAD_ALM_AALM:
			CheckRadioButton(hDlg, IDC_RAD_ALM_COCM, IDC_RAD_ALM_AALM, LOWORD(wParam));
			return true;

		case IDC_BOT_ALM_SIGM:
			if (auxalmmod->sig != NULL) {
				auxalmmod = auxalmmod->sig;
				SendMessage(hnumacmod, WM_SETTEXT, 50, (LPARAM)auxalmmod->numero);
				SendMessage(hnomacmod, WM_SETTEXT, 50, (LPARAM)auxalmmod->nombre);
				SendMessage(hfechaacmod, WM_SETTEXT, 20, (LPARAM)auxalmmod->fecha);
				_itoa(auxalmmod->cantidad, cantauxamod, 10);
				SendMessage(hcantacmod, WM_SETTEXT, 50, (LPARAM)cantauxamod);

				if (auxalmmod->tipo == true) {
					CheckRadioButton(hDlg, IDC_RAD_ALM_COCM, IDC_RAD_ALM_AALM, IDC_RAD_ALM_COCM);
				}
				else {
					CheckRadioButton(hDlg, IDC_RAD_ALM_COCM, IDC_RAD_ALM_AALM, IDC_RAD_ALM_AALM);
				}
			}
			else {
				MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
			}
			return true;

		case IDC_BOT_ALM_PREM:
			if (auxalmmod->ant != NULL) {
				auxalmmod = auxalmmod->ant;
				SendMessage(hnumacmod, WM_SETTEXT, 50, (LPARAM)auxalmmod->numero);
				SendMessage(hnomacmod, WM_SETTEXT, 50, (LPARAM)auxalmmod->nombre);
				SendMessage(hfechaacmod, WM_SETTEXT, 20, (LPARAM)auxalmmod->fecha);
				_itoa(auxalmmod->cantidad, cantauxamod, 10);
				SendMessage(hcantacmod, WM_SETTEXT, 50, (LPARAM)cantauxamod);

				if (auxalmmod->tipo == true) {
					CheckRadioButton(hDlg, IDC_RAD_ALM_COCM, IDC_RAD_ALM_AALM, IDC_RAD_ALM_COCM);
				}
				else {
					CheckRadioButton(hDlg, IDC_RAD_ALM_COCM, IDC_RAD_ALM_AALM, IDC_RAD_ALM_AALM);
				}
			}
			else {
				MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
			}
			return true;

		case IDC_BOT_ALM_GRDCM:
			SendMessage(hnumacmod, WM_GETTEXT, 50, (LPARAM)auxalmmod->numero);
			SendMessage(hnomacmod, WM_GETTEXT, 50, (LPARAM)auxalmmod->nombre);
			SendMessage(hfechaacmod, WM_GETTEXT, 20, (LPARAM)auxalmmod->fecha);
			SendMessage(hcantacmod, WM_GETTEXT, 50, (LPARAM)cantauxamod);
			auxalmmod->cantidad = atoi(cantauxamod);
			if (IsDlgButtonChecked(hDlg, IDC_RAD_ALM_COCM) == BST_CHECKED) {
				auxalmmod->tipo = true;
			}
			else {
				auxalmmod->tipo = false;
			}
			MessageBox(hDlg, "Cambios guardados con exito", "Aviso", MB_OK + MB_ICONINFORMATION);
			return true;

		case IDC_BOT_ALM_EXIM:
			EndDialog(hDlg, 0);
			return true;

		case IDC_BOT_ALM_DELCMBM:
			almacen *auxpunalmsig = NULL, *auxpunalmant = NULL;
			auxpunalmant = auxalmmod->ant;
			auxpunalmsig = auxalmmod->sig;
				if((auxpunalmant == NULL) && (auxpunalmsig == NULL)){
					listaa = NULL;
				}
				else if (auxpunalmant == NULL) {
					listaa = listaa->sig;
					listaa->ant = NULL;
					
				} else if (auxpunalmsig == NULL) {
					auxpunalmant->sig = NULL;
				}
				else {
					auxpunalmant->sig = auxpunalmsig;
					auxpunalmsig->ant = auxpunalmant;
				}
				delete auxalmmod;
				auxalmmod = listaa;
				if (listaa != NULL) {
					SendMessage(hnumacmod, WM_SETTEXT, 50, (LPARAM)auxalmmod->numero);
					SendMessage(hnomacmod, WM_SETTEXT, 50, (LPARAM)auxalmmod->nombre);
					SendMessage(hfechaacmod, WM_SETTEXT, 20, (LPARAM)auxalmmod->fecha);
					_itoa(auxalmmod->cantidad, cantauxamod, 10);
					SendMessage(hcantacmod, WM_SETTEXT, 50, (LPARAM)cantauxamod);
					if (auxalmmod->tipo == true) {
						CheckRadioButton(hDlg, IDC_RAD_ALM_COCM, IDC_RAD_ALM_AALM, IDC_RAD_ALM_COCM);
					}
					else {
						CheckRadioButton(hDlg, IDC_RAD_ALM_COCM, IDC_RAD_ALM_AALM, IDC_RAD_ALM_AALM);
					}
				} else {
					EndDialog(hDlg, 0);
				}
				MessageBox(hDlg, "Elemento eliminado satisfactoriamente", "Aviso", MB_OK + MB_ICONINFORMATION);
				
			return true;

		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

//IDD_ADM_MEM_AGR
BOOL CALLBACK AboutDlgADM_MEM_AGR(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnumm, hnomm, hemim, hexpm, hrimgm;
	static HBITMAP bmp02;
	static char fecha[80];
	static char filename[MAX_PATH];
	char *file1;	
	membresia * aux = NULL;
	switch (msg)
	{
	case WM_INITDIALOG:
		hnumm = GetDlgItem(hDlg, IDC_MEM_NUM);
		hnomm = GetDlgItem(hDlg, IDC_MEM_NOM);
		hemim = GetDlgItem(hDlg, IDC_MEM_EMI);
		hexpm = GetDlgItem(hDlg, IDC_MEM_EXP);
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(fecha, 80, "%d/%m/%Y", timeinfo);
		SendMessage(hemim, WM_SETTEXT, 80, (LPARAM)fecha);

		return true;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_MEM_EXA:

			file1 = AbrirArchivos(hDlg);
			strcpy(filename, file1);
			//La dirección se encuentra en filename
			bmp02 = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
			//Filename tiene la dirreción de la imagen
			SendDlgItemMessage(hDlg, IDC_MEM_MEPI, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp02);
			return true;

		case IDC_MEM_AGR:
			//{
			//Va un if para saber si no puso nulos
			membresia * nuevo = new membresia;
			SendMessage(hnumm, WM_GETTEXT, 10, (LPARAM)nuevo->numero);
			SendMessage(hnomm, WM_GETTEXT, 50, (LPARAM)nuevo->nombre);
			SendMessage(hemim, WM_GETTEXT, 20, (LPARAM)nuevo->emision);

			GetWindowText(hexpm, (LPTSTR)nuevo->expedicion, 20);
			//SendMessage(hexpm, WM_GETTEXT, 10, (LPARAM)nuevo->expedicion);
			strcpy(nuevo->imagen, filename);

			aux = listam;
			if (aux != NULL) {
				while (aux != NULL) {
					if (strcmp(nuevo->numero, aux->numero) == 0) {
						SendMessage(hnumm, WM_SETTEXT, 10, (LPARAM)"");
						MessageBox(hDlg, "Clave de almacén duplicada", "AVISO", MB_OK + MB_ICONERROR);
						delete nuevo;
						return true;
					}
					aux = aux->sig;
				}
			}

			if ( (*(nuevo->numero) == NULL) || (*(nuevo->nombre) == NULL) || (*(nuevo->emision) == NULL) || (*(nuevo->expedicion) == NULL) ||(*(nuevo->imagen) == NULL)){
				MessageBox(hDlg, "Faltan datos por capturar", "AVISO", MB_OK + MB_ICONERROR);
				delete nuevo;
			}
			else {
				nuevo->ant = NULL;
				nuevo->sig = NULL;
				if (listam == NULL) {
					listam = nuevo;
				}
				else {
					aux = listam;
					while (aux->sig != NULL) {
						aux = aux->sig;
					}
					aux->sig = nuevo;
					nuevo->ant = aux;
				}
				MessageBox(hDlg, "Membresia agregada correctamente", "AVISO", MB_OK + MB_ICONINFORMATION);
				SendMessage(hnumm, WM_SETTEXT, 10, (LPARAM)"");
				SendMessage(hnomm, WM_SETTEXT, 50, (LPARAM)"");
				//SendMessage(hemim, WM_SETTEXT, 10, (LPARAM)" ");
				//SendMessage(hexpm, WM_SETTEXT, 20, (LPARAM)" ");
				SendDlgItemMessage(hDlg, IDC_MEM_MEPI, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				
			}
			return true;
		//}
		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

//IDD_ADM_MEM_MOD
BOOL CALLBACK AboutDlgADM_MEM_MOD(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnummm, hnommm, hemimm, hexpmm;
	static HBITMAP bmp;
	static char fecha[80];
	static char filename[MAX_PATH];
	char *file1;
	static membresia * auxmodmem;

	switch (msg)
	{
	case WM_INITDIALOG:
		if (listam == NULL) {
			EndDialog(hDlg, 0);
			return true;
		}
		hnummm = GetDlgItem(hDlg, IDC_MEM_NUMM);
		hnommm = GetDlgItem(hDlg, IDC_MEM_NOMM);
		hemimm = GetDlgItem(hDlg, IDC_MEM_EMIM);
		hexpmm = GetDlgItem(hDlg, IDC_MEM_EXPM);
		auxmodmem = listam;
		SendMessage(hnummm, WM_SETTEXT, 50, (LPARAM)auxmodmem->numero);
		SendMessage(hnommm, WM_SETTEXT, 50, (LPARAM)auxmodmem->nombre);
		SendMessage(hemimm, WM_SETTEXT, 20, (LPARAM)auxmodmem->emision);
		SendMessage(hexpmm, WM_SETTEXT, 20, (LPARAM)auxmodmem->expedicion);
		strcpy(filename, auxmodmem->imagen);
		//La dirección se encuentra en filename
		bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
		//Filename tiene la dirreción de la imagen
		SendDlgItemMessage(hDlg, IDC_MEM_MEPIM, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDC_MEM_EXAM:
				file1 = AbrirArchivos(hDlg);
				strcpy(filename, file1);
				//La dirección se encuentra en filename
				bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
				//Filename tiene la dirreción de la imagen
				SendDlgItemMessage(hDlg, IDC_MEM_MEPIM, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			return true;

			case IDC_MEM_NEXTM:
				if (auxmodmem->sig != NULL) {
					auxmodmem = auxmodmem->sig;
					SendMessage(hnummm, WM_SETTEXT, 50, (LPARAM)auxmodmem->numero);
					SendMessage(hnommm, WM_SETTEXT, 50, (LPARAM)auxmodmem->nombre);
					SendMessage(hemimm, WM_SETTEXT, 20, (LPARAM)auxmodmem->emision);
					SendMessage(hexpmm, WM_SETTEXT, 20, (LPARAM)auxmodmem->expedicion);
					strcpy(filename, auxmodmem->imagen);
					//La dirección se encuentra en filename
					bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
					//Filename tiene la dirreción de la imagen
					SendDlgItemMessage(hDlg, IDC_MEM_MEPIM, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				}
				else {
					MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
				}
				return true;

			case IDC_MEM_ANTPM:
				if (auxmodmem->ant != NULL) {
					auxmodmem = auxmodmem->ant;
					SendMessage(hnummm, WM_SETTEXT, 50, (LPARAM)auxmodmem->numero);
					SendMessage(hnommm, WM_SETTEXT, 50, (LPARAM)auxmodmem->nombre);
					SendMessage(hemimm, WM_SETTEXT, 20, (LPARAM)auxmodmem->emision);
					SendMessage(hexpmm, WM_SETTEXT, 20, (LPARAM)auxmodmem->expedicion);
					strcpy(filename, auxmodmem->imagen);
					//La dirección se encuentra en filename
					bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
					//Filename tiene la dirreción de la imagen
					SendDlgItemMessage(hDlg, IDC_MEM_MEPIM, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				}
				else {
					MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
				}
				return true;

			case IDC_MEM_AGRM:
				SendMessage(hnummm, WM_GETTEXT, 10, (LPARAM)auxmodmem->numero);
				SendMessage(hnommm, WM_GETTEXT, 50, (LPARAM)auxmodmem->nombre);
				GetWindowText(hexpmm, (LPTSTR)auxmodmem->expedicion, 20);
				//SendMessage(hexpm, WM_GETTEXT, 10, (LPARAM)nuevo->expedicion);
				strcpy(auxmodmem->imagen, filename);
				MessageBox(hDlg, "Membresia modificada correctamente", "AVISO", MB_OK + MB_ICONINFORMATION);
				return true;

			case IDC_MEM_DELM:
				membresia *auxpunmemsig = NULL, *auxpunmemant = NULL;
				auxpunmemant = auxmodmem->ant;
				auxpunmemsig = auxmodmem->sig;
				if ((auxpunmemant == NULL) && (auxpunmemsig == NULL)) {
					listam = NULL;
				}
				else if (auxpunmemant == NULL) {
					listam = listam->sig;
					listam->ant = NULL;
				}
				else if (auxpunmemsig == NULL) {
					auxpunmemant->sig = NULL;
				}
				else {
					auxpunmemant->sig = auxpunmemsig;
					auxpunmemsig->ant = auxpunmemant;
				}
				delete auxmodmem;
				auxmodmem = listam;
				if (listam != NULL) {
					SendMessage(hnummm, WM_SETTEXT, 50, (LPARAM)auxmodmem->numero);
					SendMessage(hnommm, WM_SETTEXT, 50, (LPARAM)auxmodmem->nombre);
					SendMessage(hemimm, WM_SETTEXT, 20, (LPARAM)auxmodmem->emision);
					SendMessage(hexpmm, WM_SETTEXT, 20, (LPARAM)auxmodmem->expedicion);
					strcpy(filename, auxmodmem->imagen);
					//La dirección se encuentra en filename
					bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
					//Filename tiene la dirreción de la imagen
					SendDlgItemMessage(hDlg, IDC_MEM_MEPIM, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				}
				else {
					EndDialog(hDlg, 0);
				}
				MessageBox(hDlg, "Elemento eliminado satisfactoriamente", "Aviso", MB_OK + MB_ICONINFORMATION);

				return true;

		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

//IDD_ADM_PLA_AGR
BOOL CALLBACK AboutDlgADM_PLA_AGR(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnump, hnomp, hprecp, hdescp, hlistagr, hlisting;
	static HBITMAP bmpp;
	UINT indice;
	static char fileagr[MAX_PATH];
	char *file2;
	static int toindex, i = 0;
	static char texto[256], auxprec[50], auxselp[256];
	platillos * auxpla = NULL;
	almacen * auxdaux = NULL;
	switch (msg)
	{
	case WM_INITDIALOG:

		hnump = GetDlgItem(hDlg, IDC_PLA_NUMA);
		hnomp = GetDlgItem(hDlg, IDC_PLA_NOMA);
		hprecp = GetDlgItem(hDlg, IDC_PLA_PREA);
		hdescp = GetDlgItem(hDlg, IDC_LIST_PLADESC);
		hlistagr = GetDlgItem(hDlg, IDC_LIST_PLADIS);
		hlisting = GetDlgItem(hDlg, IDC_LIST_PLAING);
		auxdaux = listaa;
		if (auxdaux == NULL) {
			MessageBox(hDlg, "No se encuentran registrados productos en almacén", "AVISO", MB_OK + MB_ICONINFORMATION);
			EndDialog(hDlg, 0);
		}
		else {
			while (auxdaux != NULL) {
				SendMessage(hlistagr, LB_ADDSTRING, 0, (LPARAM)auxdaux->nombre);
				auxdaux = auxdaux->sig;
			}
		}
		//Para que me devulva el valor de lso items int index = SendMessage(hList, LB_GETCOUNT, 0, 0);
		//para resetear el list SendMessage(hListBox2,LB_RESETCONTENT, 0, 0);

		return true;
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case IDC_BOT_PLAEX:
			file2 = AbrirArchivos(hDlg);
			strcpy(fileagr, file2);
			//La dirección se encuentra en filename
			bmpp = (HBITMAP)LoadImage(NULL, fileagr, IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);
			//Filename tiene la dirreción de la imagen
			SendDlgItemMessage(hDlg, IDC_STACPLAA, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpp);
			return true;

		case IDC_BOT_PLASAL:
			EndDialog(hDlg, 0);
			return true;

		case IDC_BOT_PLAAGR:
			indice = SendMessage(hlistagr, LB_GETCURSEL, 0, 0);
			SendMessage(hlistagr, LB_GETTEXT, indice, (LPARAM)texto);
			SendMessage(hlisting, LB_ADDSTRING, 0, (LPARAM)texto);
			return true;

		case IDC_BOT_PLAEL:
			indice = SendMessage(hlisting, LB_GETCURSEL, 0, 0);
			SendMessage(hlisting, LB_DELETESTRING, indice, 0);
			//SendMessage(hlistagr, LB_ADDSTRING, 0, (LPARAM)texto);
			return true;

		case IDC_BOT_PLASAVE:
			platillos * newpla = new platillos;
			SendMessage(hnump, WM_GETTEXT, 50, (LPARAM)newpla->numero);
			SendMessage(hnomp, WM_GETTEXT, 50, (LPARAM)newpla->nombre);
			SendMessage(hprecp, WM_GETTEXT, 50, (LPARAM)auxprec);
			newpla->precio = atoi(auxprec);
			newpla->disponible = true;
			strcpy(newpla->imagen, fileagr);
			SendMessage(hdescp, WM_GETTEXT, 260, (LPARAM)newpla->descripcion);
			toindex = SendMessage(hlisting, LB_GETCOUNT, 0, 0);
			while (i < toindex )
			{
				if (i<1) {
					SendMessage(hlisting, LB_GETTEXT, i, (LPARAM)newpla->ingredientes);
				}
				else {
					SendMessage(hlisting, LB_GETTEXT, i, (LPARAM)auxselp);
					strcat(newpla->ingredientes, "/");
					strcat(newpla->ingredientes, auxselp);
				}
				i++;
			}
			i = 0;


			auxpla = listap;
			if (auxpla != NULL) {
				while (auxpla->sig != NULL) {
					if (strcmp(newpla->numero, auxpla->numero) == 0) {
						SendMessage(hnump, WM_SETTEXT, 10, (LPARAM)"");
						MessageBox(hDlg, "Clave de platillo duplicada", "AVISO", MB_OK + MB_ICONERROR);
						delete newpla;
						return true;
					}
					auxpla = auxpla->sig;
				}
			}

			if ((*(newpla->numero) == NULL) || (*(newpla->nombre) == NULL) || ((newpla->precio) == NULL) || (*(newpla->imagen) == NULL) || (*(newpla->ingredientes) == NULL) || (*(newpla->descripcion) == NULL)) {
				MessageBox(hDlg, "Faltan datos por capturar", "AVISO", MB_OK + MB_ICONERROR);
				delete newpla;
			}
			else {
				newpla->ant = NULL;
				newpla->sig = NULL;
				if (listap == NULL) {
					listap = newpla;
				}
				else {
					auxpla = listap;
					while (auxpla->sig != NULL) {
						auxpla = auxpla->sig;
					}
					auxpla->sig = newpla;
					newpla->ant = auxpla;
				}
				MessageBox(hDlg, "Platillo agregado correctamente", "AVISO", MB_OK + MB_ICONINFORMATION);
				//SendMessage(hemim, WM_SETTEXT, 10, (LPARAM)" ");
				SendMessage(hnump, WM_SETTEXT, 50, (LPARAM)"");
				SendMessage(hnomp, WM_SETTEXT, 50, (LPARAM)"");
				SendMessage(hprecp, WM_SETTEXT, 50, (LPARAM)"");
				SendMessage(hdescp, WM_SETTEXT, 50, (LPARAM)"");
				SendMessage(hlisting, LB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hDlg, IDC_STACPLAA, STM_SETIMAGE, IMAGE_BITMAP, NULL);
				//SendMessage(hexpm, WM_SETTEXT, 20, (LPARAM)" ");
				}
				return true;
			}
			return true;
			// If the dialog was closed (user pressed 'X' button)
			// then terminate the dialog.
		case WM_CLOSE:
			EndDialog(hDlg, 0);
			return true;
		case WM_DESTROY:

			return true;
		}
		return false;
	}

//IDD_ADM_PLA_MOD
BOOL CALLBACK AboutDlgADM_PLA_MOD(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnumpm, hnompm, hprecpm, hdescpm, hlistagrm, hlistingm;
	static HBITMAP bmpp;
	UINT indicepm;
	static char filenammodpla[MAX_PATH];
	static char *file3, *pchm;
	static int toindexpm, ipm = 0;
	static char textopm[256], auxprecpm[50], auxselppm[256], auxprecm[50], auxpchmplam[256];
	static platillos * auxplamod;
	static almacen * auxdaux;
	switch (msg)
	{
	case WM_INITDIALOG:
		if (listap == NULL) {
			EndDialog(hDlg, 0);
			return true;
		}
		hnumpm = GetDlgItem(hDlg, IDC_PLA_NUMAM);
		hnompm = GetDlgItem(hDlg, IDC_PLA_NOMAM);
		hprecpm = GetDlgItem(hDlg, IDC_STACPLAAM);
		hdescpm = GetDlgItem(hDlg, IDC_LIST_PLADESCM);
		hlistagrm = GetDlgItem(hDlg, IDC_LIST_PLADISM);
		hlistingm = GetDlgItem(hDlg, IDC_LIST_PLAINGM);
		auxplamod = listap;
		auxdaux = listaa;
		if (auxdaux == NULL) {
			MessageBox(hDlg, "No se encuentran registrados productos en almacén", "AVISO", MB_OK + MB_ICONINFORMATION);
			EndDialog(hDlg, 0);
		}
		else {
			while (auxdaux != NULL) {
				SendMessage(hlistagrm, LB_ADDSTRING, 0, (LPARAM)auxdaux->nombre);
				auxdaux = auxdaux->sig;
			}
		}

		SendMessage(hnumpm, WM_SETTEXT, 50, (LPARAM)auxplamod->numero);
		SendMessage(hnompm, WM_SETTEXT, 50, (LPARAM)auxplamod->nombre);
		//10 por que lo queremos en base decimal
		_itoa(auxplamod->precio, auxprecm, 10);
		SendMessage(hprecpm, WM_SETTEXT, 50, (LPARAM)auxprecm);
		SendMessage(hdescpm, WM_SETTEXT, 260, (LPARAM)auxplamod->descripcion);
		strcpy(filenammodpla, auxplamod->imagen);
		//La dirección se encuentra en filename
		bmpp = (HBITMAP)LoadImage(NULL, filenammodpla, IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);
		//Filename tiene la dirreción de la imagen
		SendDlgItemMessage(hDlg, IDC_STACPLAM, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpp);
		if (auxplamod->disponible == true) {
			CheckRadioButton(hDlg, IDC_RAD_CATSIM, IDC_RAD_CATNOM, IDC_RAD_CATSIM);
		}
		else {
			CheckRadioButton(hDlg, IDC_RAD_CATSIM, IDC_RAD_CATNOM, IDC_RAD_CATNOM);
		}
		strcpy(auxpchmplam, auxplamod->ingredientes);
		pchm = strtok(auxpchmplam, "/");
		while (pchm != NULL)
		{
			SendMessage(hlistingm, LB_ADDSTRING, 0, (LPARAM)pchm);
			pchm = strtok(NULL, "/");
		}
		return true;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_RAD_CATSIM:
		case IDC_RAD_CATNOM:
			CheckRadioButton(hDlg, IDC_RAD_CATSIM, IDC_RAD_CATNOM, LOWORD(wParam));
			return true;

			case IDC_BOT_PLAAGRM:
				indicepm = SendMessage(hlistagrm, LB_GETCURSEL, 0, 0);
				SendMessage(hlistagrm, LB_GETTEXT, indicepm, (LPARAM)textopm);
				SendMessage(hlistingm, LB_ADDSTRING, 0, (LPARAM)textopm);
				return true;

			case IDC_BOT_PLAELMM:
				indicepm = SendMessage(hlistingm, LB_GETCURSEL, 0, 0);
				SendMessage(hlistingm, LB_DELETESTRING, indicepm, 0);
				return true;

			case IDC_BOT_PLAEXM:
				file3 = AbrirArchivos(hDlg);
				strcpy(filenammodpla, file3);
				//La dirección se encuentra en filename
				bmpp = (HBITMAP)LoadImage(NULL, filenammodpla, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
				//Filename tiene la dirreción de la imagen
				SendDlgItemMessage(hDlg, IDC_STACPLAM, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpp);
				return true;

			case IDC_BOT_PLASIGM:
				if (listap != NULL) {
					if (auxplamod->sig != NULL) {
						SendMessage(hlistingm, LB_RESETCONTENT, 0, 0);
						auxplamod = auxplamod->sig;
						//+++
						SendMessage(hnumpm, WM_SETTEXT, 50, (LPARAM)auxplamod->numero);
						SendMessage(hnompm, WM_SETTEXT, 50, (LPARAM)auxplamod->nombre);
						//10 por que lo queremos en base decimal
						_itoa(auxplamod->precio, auxprecm, 10);
						SendMessage(hprecpm, WM_SETTEXT, 50, (LPARAM)auxprecm);
						SendMessage(hdescpm, WM_SETTEXT, 260, (LPARAM)auxplamod->descripcion);
						strcpy(filenammodpla, auxplamod->imagen);
						//La dirección se encuentra en filename
						bmpp = (HBITMAP)LoadImage(NULL, filenammodpla, IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);
						//Filename tiene la dirreción de la imagen
						SendDlgItemMessage(hDlg, IDC_STACPLAM, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpp);
						if (auxplamod->disponible == true) {
							CheckRadioButton(hDlg, IDC_RAD_CATSIM, IDC_RAD_CATNOM, IDC_RAD_CATSIM);
						}
						else {
							CheckRadioButton(hDlg, IDC_RAD_CATSIM, IDC_RAD_CATNOM, IDC_RAD_CATNOM);
						}
						strcpy(auxpchmplam, auxplamod->ingredientes);
						pchm = strtok(auxpchmplam, "/");
						while (pchm != NULL)
						{
							SendMessage(hlistingm, LB_ADDSTRING, 0, (LPARAM)pchm);
							pchm = strtok(NULL, "/");
						}
					}
					else {
						MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
					}
				}

			return true;

			case IDC_BOT_PLAANTM:
				if (listap != NULL) {
					if (auxplamod->ant != NULL) {
						SendMessage(hlistingm, LB_RESETCONTENT, 0, 0);
						auxplamod = auxplamod->ant;
						//+++
						SendMessage(hnumpm, WM_SETTEXT, 50, (LPARAM)auxplamod->numero);
						SendMessage(hnompm, WM_SETTEXT, 50, (LPARAM)auxplamod->nombre);
						//10 por que lo queremos en base decimal
						_itoa(auxplamod->precio, auxprecm, 10);
						SendMessage(hprecpm, WM_SETTEXT, 50, (LPARAM)auxprecm);
						SendMessage(hdescpm, WM_SETTEXT, 260, (LPARAM)auxplamod->descripcion);
						strcpy(filenammodpla, auxplamod->imagen);
						//La dirección se encuentra en filename
						bmpp = (HBITMAP)LoadImage(NULL, filenammodpla, IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);
						//Filename tiene la dirreción de la imagen
						SendDlgItemMessage(hDlg, IDC_STACPLAM, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpp);
						if (auxplamod->disponible == true) {
							CheckRadioButton(hDlg, IDC_RAD_CATSIM, IDC_RAD_CATNOM, IDC_RAD_CATSIM);
						}
						else {
							CheckRadioButton(hDlg, IDC_RAD_CATSIM, IDC_RAD_CATNOM, IDC_RAD_CATNOM);
						}
						strcpy(auxpchmplam, auxplamod->ingredientes);
						pchm = strtok(auxpchmplam, "/");
						while (pchm != NULL)
						{
							SendMessage(hlistingm, LB_ADDSTRING, 0, (LPARAM)pchm);
							pchm = strtok(NULL, "/");
						}
					}
					else {
						MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
					}
				}
				return true;

			case IDC_BOT_PLASAVEM:
				SendMessage(hnumpm, WM_GETTEXT, 50, (LPARAM)auxplamod->numero);
				SendMessage(hnompm, WM_GETTEXT, 50, (LPARAM)auxplamod->nombre);
				SendMessage(hprecpm, WM_GETTEXT, 50, (LPARAM)auxprecpm);
				auxplamod->precio = atoi(auxprecpm);
				strcpy(auxplamod->imagen, filenammodpla);
				SendMessage(hdescpm, WM_GETTEXT, 260, (LPARAM)auxplamod->descripcion);
				
				if (IsDlgButtonChecked(hDlg, IDC_RAD_CATSIM) == BST_CHECKED) {
					auxplamod->disponible = true;
				} else {
					auxplamod->disponible = false;
				}

				toindexpm = SendMessage(hlistingm, LB_GETCOUNT, 0, 0);
				while (ipm < toindexpm)
				{
					if (ipm<1) {
						SendMessage(hlistingm, LB_GETTEXT, ipm, (LPARAM)auxplamod->ingredientes);
					}
					else {
						SendMessage(hlistingm, LB_GETTEXT, ipm, (LPARAM)auxselppm);
						strcat(auxplamod->ingredientes, "/");
						strcat(auxplamod->ingredientes, auxselppm);
					}
					ipm++;
				}
				ipm = 0;
				MessageBox(hDlg, "Cambios guardados con exito", "Aviso", MB_OK + MB_ICONINFORMATION);

			return true;
			case IDC_BOT_PLASALM:
					EndDialog(hDlg, 0);
				return true;

			case IDC_BOT_PLADELEM:
					platillos *auxpunplasig = NULL, *auxpunplaant = NULL;
					auxpunplaant = auxplamod->ant;
					auxpunplasig = auxplamod->sig;
					if ((auxpunplaant == NULL) && (auxpunplasig == NULL)) {
						listap = NULL;
					}
					else if (auxpunplaant == NULL) {
						listap = listap->sig;
						listap->ant = NULL;
					}
					else if (auxpunplasig == NULL) {
						auxpunplaant->sig = NULL;
					}
					else {
						auxpunplaant->sig = auxpunplasig;
						auxpunplasig->ant = auxpunplaant;
					}
					delete auxplamod;
					auxplamod = listap;
					if (listap != NULL) {
						SendMessage(hlistingm, LB_RESETCONTENT, 0, 0);
						SendMessage(hnumpm, WM_SETTEXT, 50, (LPARAM)auxplamod->numero);
						SendMessage(hnompm, WM_SETTEXT, 50, (LPARAM)auxplamod->nombre);
						//10 por que lo queremos en base decimal
						_itoa(auxplamod->precio, auxprecm, 10);
						SendMessage(hprecpm, WM_SETTEXT, 50, (LPARAM)auxprecm);
						SendMessage(hdescpm, WM_SETTEXT, 260, (LPARAM)auxplamod->descripcion);
						strcpy(filenammodpla, auxplamod->imagen);
						//La dirección se encuentra en filename
						bmpp = (HBITMAP)LoadImage(NULL, filenammodpla, IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);
						//Filename tiene la dirreción de la imagen
						SendDlgItemMessage(hDlg, IDC_STACPLAM, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpp);
						if (auxplamod->disponible == true) {
							CheckRadioButton(hDlg, IDC_RAD_CATSIM, IDC_RAD_CATNOM, IDC_RAD_CATSIM);
						}
						else {
							CheckRadioButton(hDlg, IDC_RAD_CATSIM, IDC_RAD_CATNOM, IDC_RAD_CATNOM);
						}
						strcpy(auxpchmplam, auxplamod->ingredientes);
						pchm = strtok(auxpchmplam, "/");
						while (pchm != NULL)
						{
							SendMessage(hlistingm, LB_ADDSTRING, 0, (LPARAM)pchm);
							pchm = strtok(NULL, "/");
						}
					}
					else {
						SendMessage(hlistingm, LB_RESETCONTENT, 0, 0);
						SendMessage(hnumpm, WM_SETTEXT, 50, (LPARAM)"");
						SendMessage(hnompm, WM_SETTEXT, 50, (LPARAM)"");
						SendMessage(hprecpm, WM_SETTEXT, 50, (LPARAM)"");
						SendMessage(hdescpm, WM_SETTEXT, 260, (LPARAM)"");
						SendDlgItemMessage(hDlg, IDC_STACPLAM, STM_SETIMAGE, IMAGE_BITMAP, NULL);
						EndDialog(hDlg, 0);
					}

					MessageBox(hDlg, "Elemento eliminado satisfactoriamente", "Aviso", MB_OK + MB_ICONINFORMATION);
					BanderaModPla = true;
				
				return true;

		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

//IDD_CON_ALM
BOOL CALLBACK AboutDlgCON_ALM(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static almacen * auxca = NULL, *auxheap, *busca;
	static char cantauxa[50], auxi[50], auxbuscanum[50];
	char dia[20], mes[20], año[20], *pchm, auxpchfecha[20];
	static int cadena[50], com;
	static int contados = 0, busqconte=0;
	static bool heap = false, encontrado = false;
	static HWND hnumac, hnomac, hfechaac, hcantac, hbusca;
	switch (msg)
	{
	case WM_INITDIALOG:

		if (listaa == NULL) {
			EndDialog(hDlg, 0);
			return true;
		}

		hnumac = GetDlgItem(hDlg, IDC_ALM_NUMC);
		hnomac = GetDlgItem(hDlg, IDC_ALM_NOMC);
		hfechaac = GetDlgItem(hDlg, IDC_ALM_EMIC);
		hcantac = GetDlgItem(hDlg, IDC_ALM_CARTC);
		hbusca = GetDlgItem(hDlg, IDC_ALM_CORD);
		auxca = listaa;
		SendMessage(hnumac, WM_SETTEXT, 50, (LPARAM)auxca->numero);
		SendMessage(hnomac, WM_SETTEXT, 50, (LPARAM)auxca->nombre);
		SendMessage(hfechaac, WM_SETTEXT, 20, (LPARAM)auxca->fecha);
		_itoa(auxca->cantidad,cantauxa,10);
		SendMessage(hcantac, WM_SETTEXT, 50, (LPARAM)cantauxa);

		if (auxca->tipo == true) {
			CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_COCC);
		}
		else {
			CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_AALC);
		}

		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDC_RAD_ALM_COCC:
			case IDC_RAD_ALM_AALC:
				CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, LOWORD(wParam));
				return true;
			//Busqueda binaria
			case IDC_BOT_ALM_CORDNUM:
				SendMessage(hbusca, WM_GETTEXT, 50, (LPARAM)auxbuscanum);
				busca = listaa;
				while (busca != NULL) {
					busqconte++;
					busca = busca->sig;
				}

				busca = listaa;
				busqconte = busqconte / 2;
				for (int u = 1; u <= busqconte; u++) {
					busca = busca->sig;
				}
				if (strcmp(auxbuscanum, busca->numero) == 0) {
					SendMessage(hnumac, WM_SETTEXT, 50, (LPARAM)busca->numero);
					SendMessage(hnomac, WM_SETTEXT, 50, (LPARAM)busca->nombre);
					SendMessage(hfechaac, WM_SETTEXT, 20, (LPARAM)busca->fecha);
					_itoa(busca->cantidad, cantauxa, 10);
					SendMessage(hcantac, WM_SETTEXT, 50, (LPARAM)cantauxa);

					if (busca->tipo == true) {
						CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_COCC);
					}
					else {
						CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_AALC);
					}
					encontrado = true;
				}
				if (strcmp(auxbuscanum, busca->numero) == -1) {
					while (busca !=NULL){
						if (strcmp(auxbuscanum, busca->numero) == 0) {
							SendMessage(hnumac, WM_SETTEXT, 50, (LPARAM)busca->numero);
							SendMessage(hnomac, WM_SETTEXT, 50, (LPARAM)busca->nombre);
							SendMessage(hfechaac, WM_SETTEXT, 20, (LPARAM)busca->fecha);
							_itoa(busca->cantidad, cantauxa, 10);
							SendMessage(hcantac, WM_SETTEXT, 50, (LPARAM)cantauxa);

							if (busca->tipo == true) {
								CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_COCC);
							}
							else {
								CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_AALC);
							}
							encontrado = true;
						}
						busca = busca->ant;
					}
				}

				if (strcmp(auxbuscanum, busca->numero) == +1) {
					while (busca != NULL) {
						if (strcmp(auxbuscanum, busca->numero) == 0) {
							SendMessage(hnumac, WM_SETTEXT, 50, (LPARAM)busca->numero);
							SendMessage(hnomac, WM_SETTEXT, 50, (LPARAM)busca->nombre);
							SendMessage(hfechaac, WM_SETTEXT, 20, (LPARAM)busca->fecha);
							_itoa(busca->cantidad, cantauxa, 10);
							SendMessage(hcantac, WM_SETTEXT, 50, (LPARAM)cantauxa);

							if (busca->tipo == true) {
								CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_COCC);
							}
							else {
								CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_AALC);
							}
							encontrado = true;
						}
						busca = busca->sig;
					}
				}
				
				//Reseteo todas las variable
				if (encontrado == false) {
					MessageBox(hDlg, "No se encuentra el elemento buscado", "AVISO", MB_OK + MB_ICONERROR);
				}
				busqconte = 0;
				encontrado = false;
			return true;

			case IDC_BOT_ALM_CORDNOM:
				busca = listaa;
				SendMessage(hbusca, WM_GETTEXT, 50, (LPARAM)auxbuscanum);
				while (busca != NULL) {
					if (strcmp(auxbuscanum, busca->nombre) == 0) {

						SendMessage(hnumac, WM_SETTEXT, 50, (LPARAM)busca->numero);
						SendMessage(hnomac, WM_SETTEXT, 50, (LPARAM)busca->nombre);
						SendMessage(hfechaac, WM_SETTEXT, 20, (LPARAM)busca->fecha);
						_itoa(busca->cantidad, cantauxa, 10);
						SendMessage(hcantac, WM_SETTEXT, 50, (LPARAM)cantauxa);

						if (busca->tipo == true) {
							CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_COCC);
						}
						else {
							CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_AALC);
						}

						encontrado = true;
						break;
					}
					busca = busca->sig;
				}

				if (encontrado == false) {
					MessageBox(hDlg, "No se encuentra el elemento buscado", "AVISO", MB_OK + MB_ICONERROR);
				}

				encontrado = false;
				return true;


			case IDC_BOT_ALM_SIG:
				if (heap == true) {
					if (auxheap->sig != NULL) {
						while (auxheap != NULL) {
							//com = atoi(auxheap->numero);
							_itoa(cadena[contados], auxi, 10);
							if (strcmp(auxi,auxheap->numero)==0) {

								SendMessage(hnumac, WM_SETTEXT, 50, (LPARAM)auxheap->numero);
								SendMessage(hnomac, WM_SETTEXT, 50, (LPARAM)auxheap->nombre);
								SendMessage(hfechaac, WM_SETTEXT, 20, (LPARAM)auxheap->fecha);
								_itoa(auxheap->cantidad, cantauxa, 10);
								SendMessage(hcantac, WM_SETTEXT, 50, (LPARAM)cantauxa);

								if (auxheap->tipo == true) {
									CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_COCC);
								}
								else {
									CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_AALC);
								}
								break;
							}
							auxheap = auxheap->sig;
						}
						contados++;
						auxheap = listaa;
					}
					else {
						MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
					}
				}
				else {

					if (auxca->sig != NULL) {
						auxca = auxca->sig;
						SendMessage(hnumac, WM_SETTEXT, 50, (LPARAM)auxca->numero);
						SendMessage(hnomac, WM_SETTEXT, 50, (LPARAM)auxca->nombre);
						SendMessage(hfechaac, WM_SETTEXT, 20, (LPARAM)auxca->fecha);
						_itoa(auxca->cantidad, cantauxa, 10);
						SendMessage(hcantac, WM_SETTEXT, 50, (LPARAM)cantauxa);

						if (auxca->tipo == true) {
							CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_COCC);
						}
						else {
							CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_AALC);
						}
					}
					else {
						MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
					}

				}
			return true;

			case IDC_BOT_ALM_PRE:
				if (heap == true) {
					if (auxheap->sig != NULL) {
						while (auxheap != NULL) {
							//com = atoi(auxheap->numero);
							_itoa(cadena[contados], auxi, 10);
							if (strcmp(auxi, auxheap->numero) == 0) {

								SendMessage(hnumac, WM_SETTEXT, 50, (LPARAM)auxheap->numero);
								SendMessage(hnomac, WM_SETTEXT, 50, (LPARAM)auxheap->nombre);
								SendMessage(hfechaac, WM_SETTEXT, 20, (LPARAM)auxheap->fecha);
								_itoa(auxheap->cantidad, cantauxa, 10);
								SendMessage(hcantac, WM_SETTEXT, 50, (LPARAM)cantauxa);

								if (auxheap->tipo == true) {
									CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_COCC);
								}
								else {
									CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_AALC);
								}
								break;
							}
							auxheap = auxheap->sig;
						}
						contados--;
						auxheap = listaa;
					}
					else {
						MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
					}
				}

				else {
					if (auxca->ant != NULL) {
						auxca = auxca->ant;
						SendMessage(hnumac, WM_SETTEXT, 50, (LPARAM)auxca->numero);
						SendMessage(hnomac, WM_SETTEXT, 50, (LPARAM)auxca->nombre);
						SendMessage(hfechaac, WM_SETTEXT, 20, (LPARAM)auxca->fecha);
						_itoa(auxca->cantidad, cantauxa, 10);
						SendMessage(hcantac, WM_SETTEXT, 50, (LPARAM)cantauxa);

						if (auxca->tipo == true) {
							CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_COCC);
						}
						else {
							CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_AALC);
						}
					}
					else {
						MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
					}

				}
			return true;

			case IDC_BOT_ALM_ORN:
				heap = false;
				quickSortA(listaa);
				auxca = listaa;
				SendMessage(hnumac, WM_SETTEXT, 50, (LPARAM)auxca->numero);
				SendMessage(hnomac, WM_SETTEXT, 50, (LPARAM)auxca->nombre);
				SendMessage(hfechaac, WM_SETTEXT, 20, (LPARAM)auxca->fecha);
				_itoa(auxca->cantidad, cantauxa, 10);
				SendMessage(hcantac, WM_SETTEXT, 50, (LPARAM)cantauxa);
				if (auxca->tipo == true) {
					CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_COCC);
				}
				else {
					CheckRadioButton(hDlg, IDC_RAD_ALM_COCC, IDC_RAD_ALM_AALC, IDC_RAD_ALM_AALC);
				}
				MessageBox(hDlg, "Metodo de ordenación completado", "Aviso", MB_OK + MB_ICONINFORMATION);
				return true;

			case IDC_BOT_ALM_ORN2:
				auxheap = listaa;
				contados = 0;
				while (auxheap != NULL) {
					auxheap = auxheap->sig;
					contados++;
				}
				auxheap = listaa;
				for (int i = 0; i < 50; i++)
				{
					com = atoi(auxheap->numero);
					cadena[i] = com;
					if (auxheap->sig == NULL) {
							i++;
							cadena[i] = 0;
							break;
						}
					auxheap = auxheap->sig;
				}
				heapsort(cadena, contados);

				auxheap = listaa;
				heap = true;
				contados = 0;
				MessageBox(hDlg, "Metodo de ordenación completado", "Aviso", MB_OK + MB_ICONINFORMATION);
				return true;

		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:
		heap = false;

		return true;
	}
	return false;
}

//IDD_CON_MEM 
BOOL CALLBACK AboutDlgCON_MEM(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnummc, hnommc, hemimc, hexpmc, hrimgmc, hbusca1;
	static HBITMAP bmp;
	static membresia * auxcm = NULL;
	static char filename[MAX_PATH];

	static bool encontrado=false;
	static membresia * busca;
	static char auxbuscanum[50];

	switch (msg)
	{
	case WM_INITDIALOG:
		if (listam == NULL) {
			EndDialog(hDlg, 0);
			return true;
		}
		hnummc = GetDlgItem(hDlg, IDC_MEMC);
		hnommc = GetDlgItem(hDlg, IDC_NOMC);
		hemimc = GetDlgItem(hDlg, IDC_EMIC);
		hexpmc = GetDlgItem(hDlg, IDC_EXPC);
		hbusca1 = GetDlgItem(hDlg, IDC_MEM_CORD);
		auxcm = listam;
		SendMessage(hnummc, WM_SETTEXT, 10, (LPARAM)auxcm->numero);
		SendMessage(hnommc, WM_SETTEXT, 50, (LPARAM)auxcm->nombre);
		SendMessage(hemimc, WM_SETTEXT, 10, (LPARAM)auxcm->emision);
		SendMessage(hexpmc, WM_SETTEXT, 10, (LPARAM)auxcm->expedicion);
		strcpy(filename, auxcm->imagen);
		//La dirección se encuentra en filename
		bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
		//Filename tiene la dirreción de la imagen
		SendDlgItemMessage(hDlg, IDC_CON_IMG, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		return true;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_BOT_MEM_CORDNUM:
			busca = listam;
			SendMessage(hbusca1, WM_GETTEXT, 50, (LPARAM)auxbuscanum);
			while (busca != NULL) {
				if (strcmp(auxbuscanum, busca->numero) == 0) {

					SendMessage(hnummc, WM_SETTEXT, 10, (LPARAM)busca->numero);
					SendMessage(hnommc, WM_SETTEXT, 50, (LPARAM)busca->nombre);
					SendMessage(hemimc, WM_SETTEXT, 10, (LPARAM)busca->emision);
					SendMessage(hexpmc, WM_SETTEXT, 10, (LPARAM)busca->expedicion);
					strcpy(filename, busca->imagen);
					//La dirección se encuentra en filename
					bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
					//Filename tiene la dirreción de la imagen
					SendDlgItemMessage(hDlg, IDC_CON_IMG, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);


					encontrado = true;
					break;
				}
				busca = busca->sig;
			}

			if (encontrado == false) {
				MessageBox(hDlg, "No se encuentra el elemento buscado", "AVISO", MB_OK + MB_ICONERROR);
			}

			encontrado = false;
			return true;

		case IDC_BOT_MEM_CORDNOM:
			busca = listam;
			SendMessage(hbusca1, WM_GETTEXT, 50, (LPARAM)auxbuscanum);
			while (busca != NULL) {
				if (strcmp(auxbuscanum, busca->nombre) == 0) {

					SendMessage(hnummc, WM_SETTEXT, 10, (LPARAM)busca->numero);
					SendMessage(hnommc, WM_SETTEXT, 50, (LPARAM)busca->nombre);
					SendMessage(hemimc, WM_SETTEXT, 10, (LPARAM)busca->emision);
					SendMessage(hexpmc, WM_SETTEXT, 10, (LPARAM)busca->expedicion);
					strcpy(filename, busca->imagen);
					//La dirección se encuentra en filename
					bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
					//Filename tiene la dirreción de la imagen
					SendDlgItemMessage(hDlg, IDC_CON_IMG, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

					encontrado = true;
					break;
				}
				busca = busca->sig;
			}

			if (encontrado == false) {
				MessageBox(hDlg, "No se encuentra el elemento buscado", "AVISO", MB_OK + MB_ICONERROR);
			}

			encontrado = false;
			return true;




			case IDC_MEMSIG:
				if (auxcm->sig != NULL) {
				auxcm = auxcm->sig;
				SendMessage(hnummc, WM_SETTEXT, 10, (LPARAM)auxcm->numero);
				SendMessage(hnommc, WM_SETTEXT, 50, (LPARAM)auxcm->nombre);
				SendMessage(hemimc, WM_SETTEXT, 10, (LPARAM)auxcm->emision);
				SendMessage(hexpmc, WM_SETTEXT, 10, (LPARAM)auxcm->expedicion);
				strcpy(filename, auxcm->imagen);
				//La dirección se encuentra en filename
				bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
				//Filename tiene la dirreción de la imagen
				SendDlgItemMessage(hDlg, IDC_CON_IMG, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				}
				else {
					MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
				}
			return true;

			case IDC_MEMANT:
				if (auxcm->ant != NULL) {
					auxcm = auxcm->ant;
					SendMessage(hnummc, WM_SETTEXT, 10, (LPARAM)auxcm->numero);
					SendMessage(hnommc, WM_SETTEXT, 50, (LPARAM)auxcm->nombre);
					SendMessage(hemimc, WM_SETTEXT, 10, (LPARAM)auxcm->emision);
					SendMessage(hexpmc, WM_SETTEXT, 10, (LPARAM)auxcm->expedicion);
					strcpy(filename, auxcm->imagen);
					//La dirección se encuentra en filename
					bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
					//Filename tiene la dirreción de la imagen
					SendDlgItemMessage(hDlg, IDC_CON_IMG, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				}
				else {
					MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
				}
			return true;


			case IDC_ORDMC:
				quickSort(listam);
				auxcm = listam;
				SendMessage(hnummc, WM_SETTEXT, 10, (LPARAM)auxcm->numero);
				SendMessage(hnommc, WM_SETTEXT, 50, (LPARAM)auxcm->nombre);
				SendMessage(hemimc, WM_SETTEXT, 10, (LPARAM)auxcm->emision);
				SendMessage(hexpmc, WM_SETTEXT, 10, (LPARAM)auxcm->expedicion);
				strcpy(filename, auxcm->imagen);
				//La dirección se encuentra en filename
				bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
				//Filename tiene la dirreción de la imagen
				SendDlgItemMessage(hDlg, IDC_CON_IMG, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				MessageBox(hDlg, "Metodo de ordenación completado", "Aviso", MB_OK + MB_ICONINFORMATION);
			return true;

		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

//IDD_CON_PED
BOOL CALLBACK AboutDlgCON_PED(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnummec, hnombrec, hmesac, hemic, hpagac, hanotc, hseleccc, hbusca2;
	static pedidos * toauxconped = NULL, *auxheap;
	static char auxprecons[50], auxpchconped[260], auxi[50];
	char *pch;
	static int contados = 0 , com, cadena[50];
	static bool heap = false;

	static bool encontrado = false;
	static pedidos * busca;
	static char auxbuscanum[50];

	switch (msg)
	{
	case WM_INITDIALOG:
		if (listaped == NULL) {
			EndDialog(hDlg, 0);
			return true;
		}
		hnummec = GetDlgItem(hDlg, IDC_ED_PEDNUMMC);
		hnombrec = GetDlgItem(hDlg, IDC_ED_PEDNOMCLC);
		hmesac = GetDlgItem(hDlg, IDC_ED_PEDMESAC);
		hemic = GetDlgItem(hDlg, IDC_ED_PEDEMISC);
		hpagac = GetDlgItem(hDlg, IDC_ED_PEDTOTALPC);
		hanotc = GetDlgItem(hDlg, IDC_ED_PEDANOTC);
		hseleccc = GetDlgItem(hDlg, IDC_LIST_PEDSELCC);
		hbusca2 = GetDlgItem(hDlg, IDC_PED_CORD);
		toauxconped = listaped;
		SendMessage(hnummec, WM_SETTEXT, 50, (LPARAM)toauxconped->nummem);
		SendMessage(hnombrec, WM_SETTEXT, 50, (LPARAM)toauxconped->nombre);
		SendMessage(hmesac, WM_SETTEXT, 20, (LPARAM)toauxconped->mesa);
		SendMessage(hemic, WM_SETTEXT, 20, (LPARAM)toauxconped->emi);
		SendMessage(hanotc, WM_SETTEXT, 260, (LPARAM)toauxconped->anotaciones);
		_itoa(toauxconped->pagar, auxprecons, 10);
		SendMessage(hpagac, WM_SETTEXT, 50, (LPARAM)auxprecons);

		strcpy(auxpchconped, toauxconped->selecc);
		pch = strtok(auxpchconped, "/");
		while (pch != NULL)
		{
			SendMessage(hseleccc, LB_ADDSTRING, 0, (LPARAM)pch);
			pch = strtok(NULL, "/");
		}

		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDC_BOT_PEDNEXT:

				if (heap == true) {
					if (auxheap->sig != NULL) {
						while (auxheap != NULL) {
							//com = atoi(auxheap->numero);
							_itoa(cadena[contados], auxi, 10);
							if (strcmp(auxi, auxheap->nummem) == 0) {

								SendMessage(hseleccc, LB_RESETCONTENT, 0, 0);
								toauxconped = toauxconped->sig;
								SendMessage(hnummec, WM_SETTEXT, 50, (LPARAM)auxheap->nummem);
								SendMessage(hnombrec, WM_SETTEXT, 50, (LPARAM)auxheap->nombre);
								SendMessage(hmesac, WM_SETTEXT, 20, (LPARAM)auxheap->mesa);
								SendMessage(hemic, WM_SETTEXT, 20, (LPARAM)auxheap->emi);
								SendMessage(hanotc, WM_SETTEXT, 260, (LPARAM)auxheap->anotaciones);
								_itoa(auxheap->pagar, auxprecons, 10);
								SendMessage(hpagac, WM_SETTEXT, 50, (LPARAM)auxprecons);

								strcpy(auxpchconped, auxheap->selecc);
								pch = strtok(auxpchconped, "/");
								while (pch != NULL)
								{
									SendMessage(hseleccc, LB_ADDSTRING, 0, (LPARAM)pch);
									pch = strtok(NULL, "/");
								}
								break;
							}
							auxheap = auxheap->sig;
						}
						contados++;
						auxheap = listaped;
					}
					else {
						MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
					}
				}
				else {

					if (toauxconped->sig != NULL) {
						SendMessage(hseleccc, LB_RESETCONTENT, 0, 0);
						toauxconped = toauxconped->sig;
						SendMessage(hnummec, WM_SETTEXT, 50, (LPARAM)toauxconped->nummem);
						SendMessage(hnombrec, WM_SETTEXT, 50, (LPARAM)toauxconped->nombre);
						SendMessage(hmesac, WM_SETTEXT, 20, (LPARAM)toauxconped->mesa);
						SendMessage(hemic, WM_SETTEXT, 20, (LPARAM)toauxconped->emi);
						SendMessage(hanotc, WM_SETTEXT, 260, (LPARAM)toauxconped->anotaciones);
						_itoa(toauxconped->pagar, auxprecons, 10);
						SendMessage(hpagac, WM_SETTEXT, 50, (LPARAM)auxprecons);

						strcpy(auxpchconped, toauxconped->selecc);
						pch = strtok(auxpchconped, "/");
						while (pch != NULL)
						{
							SendMessage(hseleccc, LB_ADDSTRING, 0, (LPARAM)pch);
							pch = strtok(NULL, "/");
						}
					}
					else {
						MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
					}
				}
			return true;

			case IDC_BOT_PEDPREV:

				if (heap == true) {
					if (auxheap->sig != NULL) {
						while (auxheap != NULL) {
							//com = atoi(auxheap->numero);
							_itoa(cadena[contados], auxi, 10);
							if (strcmp(auxi, auxheap->nummem) == 0) {

								SendMessage(hseleccc, LB_RESETCONTENT, 0, 0);
								SendMessage(hnummec, WM_SETTEXT, 50, (LPARAM)auxheap->nummem);
								SendMessage(hnombrec, WM_SETTEXT, 50, (LPARAM)auxheap->nombre);
								SendMessage(hmesac, WM_SETTEXT, 20, (LPARAM)auxheap->mesa);
								SendMessage(hemic, WM_SETTEXT, 20, (LPARAM)auxheap->emi);
								SendMessage(hanotc, WM_SETTEXT, 260, (LPARAM)auxheap->anotaciones);
								_itoa(auxheap->pagar, auxprecons, 10);
								SendMessage(hpagac, WM_SETTEXT, 50, (LPARAM)auxprecons);

								strcpy(auxpchconped, auxheap->selecc);
								pch = strtok(auxpchconped, "/");
								while (pch != NULL)
								{
									SendMessage(hseleccc, LB_ADDSTRING, 0, (LPARAM)pch);
									pch = strtok(NULL, "/");
								}
								break;
							}
							auxheap = auxheap->sig;
						}
						contados--;
						auxheap = listaped;
					}
					else {
						MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					if (toauxconped->ant != NULL) {
						toauxconped = toauxconped->ant;
						SendMessage(hseleccc, LB_RESETCONTENT, 0, 0);
						SendMessage(hnummec, WM_SETTEXT, 50, (LPARAM)toauxconped->nummem);
						SendMessage(hnombrec, WM_SETTEXT, 50, (LPARAM)toauxconped->nombre);
						SendMessage(hmesac, WM_SETTEXT, 20, (LPARAM)toauxconped->mesa);
						SendMessage(hemic, WM_SETTEXT, 20, (LPARAM)toauxconped->emi);
						SendMessage(hanotc, WM_SETTEXT, 260, (LPARAM)toauxconped->anotaciones);
						_itoa(toauxconped->pagar, auxprecons, 10);
						SendMessage(hpagac, WM_SETTEXT, 50, (LPARAM)auxprecons);
						strcpy(auxpchconped, toauxconped->selecc);
						pch = strtok(auxpchconped, "/");
						while (pch != NULL)
						{
							SendMessage(hseleccc, LB_ADDSTRING, 0, (LPARAM)pch);
							pch = strtok(NULL, "/");
						}
					}
					else {
						MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
					}
				}
				return true;

			case IDC_BOT_CONNMEM:
				heap = false;
				quickSortP(listaped);
				toauxconped = listaped;
				SendMessage(hseleccc, LB_RESETCONTENT, 0, 0);
				SendMessage(hnummec, WM_SETTEXT, 50, (LPARAM)toauxconped->nummem);
				SendMessage(hnombrec, WM_SETTEXT, 50, (LPARAM)toauxconped->nombre);
				SendMessage(hmesac, WM_SETTEXT, 20, (LPARAM)toauxconped->mesa);
				SendMessage(hemic, WM_SETTEXT, 20, (LPARAM)toauxconped->emi);
				SendMessage(hanotc, WM_SETTEXT, 260, (LPARAM)toauxconped->anotaciones);
				_itoa(toauxconped->pagar, auxprecons, 10);
				SendMessage(hpagac, WM_SETTEXT, 50, (LPARAM)auxprecons);
				strcpy(auxpchconped, toauxconped->selecc);
				pch = strtok(auxpchconped, "/");
				while (pch != NULL)
				{
					SendMessage(hseleccc, LB_ADDSTRING, 0, (LPARAM)pch);
					pch = strtok(NULL, "/");
				}
				MessageBox(hDlg, "Metodo de ordenación completado", "Aviso", MB_OK + MB_ICONINFORMATION);
			return true;

			case IDC_BOT_CONNMEM2:
				auxheap = listaped;
				contados = 0;
				while (auxheap != NULL) {
					auxheap = auxheap->sig;
					contados++;
				}
				auxheap = listaped;
				for (int i = 0; i < 50; i++)
				{
					com = atoi(auxheap->nummem);
					cadena[i] = com;
					if (auxheap->sig == NULL) {
						i++;
						cadena[i] = 0;
						break;
					}
					auxheap = auxheap->sig;
				}
				heapsort(cadena, contados);

				auxheap = listaped;
				heap = true;
				contados = 0;
				MessageBox(hDlg, "Metodo de ordenación completado", "Aviso", MB_OK + MB_ICONINFORMATION);
			return true;

			case IDC_BOT_PED_CORDNUM:
				busca = listaped;
				SendMessage(hbusca2, WM_GETTEXT, 50, (LPARAM)auxbuscanum);
				while (busca != NULL) {
					if (strcmp(auxbuscanum, busca->nummem) == 0) {

						SendMessage(hseleccc, LB_RESETCONTENT, 0, 0);
						SendMessage(hnummec, WM_SETTEXT, 50, (LPARAM)busca->nummem);
						SendMessage(hnombrec, WM_SETTEXT, 50, (LPARAM)busca->nombre);
						SendMessage(hmesac, WM_SETTEXT, 20, (LPARAM)busca->mesa);
						SendMessage(hemic, WM_SETTEXT, 20, (LPARAM)busca->emi);
						SendMessage(hanotc, WM_SETTEXT, 260, (LPARAM)busca->anotaciones);
						_itoa(busca->pagar, auxprecons, 10);
						SendMessage(hpagac, WM_SETTEXT, 50, (LPARAM)auxprecons);
						strcpy(auxpchconped, busca->selecc);
						pch = strtok(auxpchconped, "/");
						while (pch != NULL)
						{
							SendMessage(hseleccc, LB_ADDSTRING, 0, (LPARAM)pch);
							pch = strtok(NULL, "/");
						}


						encontrado = true;
						break;
					}
					busca = busca->sig;
				}

				if (encontrado == false) {
					MessageBox(hDlg, "No se encuentra el elemento buscado", "AVISO", MB_OK + MB_ICONERROR);
				}

				encontrado = false;
				return true;

			case IDC_BOT_PED_CORDNOM:
				busca = listaped;
				SendMessage(hbusca2, WM_GETTEXT, 50, (LPARAM)auxbuscanum);
				while (busca != NULL) {
					if (strcmp(auxbuscanum, busca->nombre) == 0) {

						SendMessage(hseleccc, LB_RESETCONTENT, 0, 0);
						SendMessage(hnummec, WM_SETTEXT, 50, (LPARAM)busca->nummem);
						SendMessage(hnombrec, WM_SETTEXT, 50, (LPARAM)busca->nombre);
						SendMessage(hmesac, WM_SETTEXT, 20, (LPARAM)busca->mesa);
						SendMessage(hemic, WM_SETTEXT, 20, (LPARAM)busca->emi);
						SendMessage(hanotc, WM_SETTEXT, 260, (LPARAM)busca->anotaciones);
						_itoa(busca->pagar, auxprecons, 10);
						SendMessage(hpagac, WM_SETTEXT, 50, (LPARAM)auxprecons);
						strcpy(auxpchconped, busca->selecc);
						pch = strtok(auxpchconped, "/");
						while (pch != NULL)
						{
							SendMessage(hseleccc, LB_ADDSTRING, 0, (LPARAM)pch);
							pch = strtok(NULL, "/");
						}

						encontrado = true;
						break;
					}
					busca = busca->sig;
				}

				if (encontrado == false) {
					MessageBox(hDlg, "No se encuentra el elemento buscado", "AVISO", MB_OK + MB_ICONERROR);
				}

				encontrado = false;
				return true;


		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:
		heap = false;
		return true;
	}
	return false;
}

//IDD_CON_PLA
BOOL CALLBACK AboutDlgCON_PLA(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnumpc, hnompc, hprecpc, hdescpc, hlistingc, hbusca3;
	static HBITMAP bmppc;
	UINT indicec;
	static char fileagrc[MAX_PATH];
	char *filec2, *pch;
	static int toindexc, ic = 0;
	static char textoc[256], auxprecc[50], auxselpc[256], auxpchconpla[260];
	static platillos * auxplac = NULL;

	static bool encontrado = false;
	static platillos * busca;
	static char auxbuscanum[50];

	switch (msg)
	{
	case WM_INITDIALOG:
		if (listap == NULL) {
			EndDialog(hDlg, 0);
			return true;
		}
		hnumpc = GetDlgItem(hDlg, IDC_PLA_NUMAC);
		hnompc = GetDlgItem(hDlg, IDC_PLA_NOMAC);
		hprecpc = GetDlgItem(hDlg, IDC_PLA_PREAC);
		hdescpc = GetDlgItem(hDlg, IDC_LIST_PLADESCC);
		hlistingc = GetDlgItem(hDlg, IDC_LIST_PLAINGC);
		hbusca3 = GetDlgItem(hDlg, IDC_PLA_CORD);
		auxplac = listap;

		SendMessage(hnumpc, WM_SETTEXT, 50, (LPARAM)auxplac->numero);
		SendMessage(hnompc, WM_SETTEXT, 50, (LPARAM)auxplac->nombre);
		//10 por que lo queremos en base decimal
		_itoa(auxplac->precio, auxprecc, 10);
		SendMessage(hprecpc, WM_SETTEXT, 50, (LPARAM)auxprecc);
		SendMessage(hdescpc, WM_SETTEXT, 260, (LPARAM)auxplac->descripcion);
		strcpy(fileagrc, auxplac->imagen);
		//La dirección se encuentra en filename
		bmppc = (HBITMAP)LoadImage(NULL, fileagrc, IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);
		//Filename tiene la dirreción de la imagen
		SendDlgItemMessage(hDlg, IDC_STACPLAAC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmppc);
		if (auxplac->disponible == true) {
			CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPYES);
		}
		else {
			CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPNO);
		}

		strcpy(auxpchconpla,auxplac->ingredientes);
		pch = strtok(auxpchconpla, "/");
		while (pch != NULL)
		{
			SendMessage(hlistingc, LB_ADDSTRING, 0, (LPARAM)pch);
			pch = strtok(NULL, "/");
		}
		
		return true;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_RAD_PLADISPYES:
		case IDC_RAD_PLADISPNO:
			CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, LOWORD(wParam));
			return true;

		case IDC_BOT_PLA_CORDNUM:
			busca = listap;
			SendMessage(hbusca3, WM_GETTEXT, 50, (LPARAM)auxbuscanum);
			while (busca != NULL) {
				if (strcmp(auxbuscanum, busca->numero) == 0) {

					SendMessage(hlistingc, LB_RESETCONTENT, 0, 0);
					SendMessage(hnumpc, WM_SETTEXT, 50, (LPARAM)busca->numero);
					SendMessage(hnompc, WM_SETTEXT, 50, (LPARAM)busca->nombre);
					//10 por que lo queremos en base decimal
					_itoa(busca->precio, auxprecc, 10);
					SendMessage(hprecpc, WM_SETTEXT, 50, (LPARAM)auxprecc);
					SendMessage(hdescpc, WM_SETTEXT, 260, (LPARAM)busca->descripcion);
					strcpy(fileagrc, busca->imagen);
					//La dirección se encuentra en filename
					bmppc = (HBITMAP)LoadImage(NULL, fileagrc, IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);
					//Filename tiene la dirreción de la imagen
					SendDlgItemMessage(hDlg, IDC_STACPLAAC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmppc);
					if (busca->disponible == true) {
						CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPYES);
					}
					else {
						CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPNO);
					}

					strcpy(auxpchconpla, busca->ingredientes);
					pch = strtok(auxpchconpla, "/");
					while (pch != NULL)
					{
						SendMessage(hlistingc, LB_ADDSTRING, 0, (LPARAM)pch);
						pch = strtok(NULL, "/");
					}

					encontrado = true;
					break;
				}
				busca = busca->sig;
			}

			if (encontrado == false) {
				MessageBox(hDlg, "No se encuentra el elemento buscado", "AVISO", MB_OK + MB_ICONERROR);
			}

			encontrado = false;
		return true;

		case IDC_BOT_PLA_CORDNOM:
			busca = listap;
			SendMessage(hbusca3, WM_GETTEXT, 50, (LPARAM)auxbuscanum);
			while (busca != NULL) {
				if (strcmp(auxbuscanum, busca->nombre) == 0) {

					SendMessage(hlistingc, LB_RESETCONTENT, 0, 0);
					SendMessage(hnumpc, WM_SETTEXT, 50, (LPARAM)busca->numero);
					SendMessage(hnompc, WM_SETTEXT, 50, (LPARAM)busca->nombre);
					//10 por que lo queremos en base decimal
					_itoa(busca->precio, auxprecc, 10);
					SendMessage(hprecpc, WM_SETTEXT, 50, (LPARAM)auxprecc);
					SendMessage(hdescpc, WM_SETTEXT, 260, (LPARAM)busca->descripcion);
					strcpy(fileagrc, busca->imagen);
					//La dirección se encuentra en filename
					bmppc = (HBITMAP)LoadImage(NULL, fileagrc, IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);
					//Filename tiene la dirreción de la imagen
					SendDlgItemMessage(hDlg, IDC_STACPLAAC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmppc);
					if (busca->disponible == true) {
						CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPYES);
					}
					else {
						CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPNO);
					}

					strcpy(auxpchconpla, busca->ingredientes);
					pch = strtok(auxpchconpla, "/");
					while (pch != NULL)
					{
						SendMessage(hlistingc, LB_ADDSTRING, 0, (LPARAM)pch);
						pch = strtok(NULL, "/");
					}

					encontrado = true;
					break;
				}
				busca = busca->sig;
			}

			if (encontrado == false) {
				MessageBox(hDlg, "No se encuentra el elemento buscado", "AVISO", MB_OK + MB_ICONERROR);
			}

			encontrado = false;
		return true;



		case IDC_BOT_PLANEXC:
			
				if (IsDlgButtonChecked(hDlg, IDC_RAD_PLADISPYES) == BST_CHECKED) {
					auxplac->disponible = true;
				}
				else {
					auxplac->disponible = false;
				}

			if (auxplac->sig != NULL) {
				SendMessage(hlistingc, LB_RESETCONTENT, 0, 0);
				auxplac = auxplac->sig;
				SendMessage(hnumpc, WM_SETTEXT, 50, (LPARAM)auxplac->numero);
				SendMessage(hnompc, WM_SETTEXT, 50, (LPARAM)auxplac->nombre);
				//10 por que lo queremos en base decimal
				_itoa(auxplac->precio, auxprecc, 10);
				SendMessage(hprecpc, WM_SETTEXT, 50, (LPARAM)auxprecc);
				SendMessage(hdescpc, WM_SETTEXT, 260, (LPARAM)auxplac->descripcion);
				strcpy(fileagrc, auxplac->imagen);
				//La dirección se encuentra en filename
				bmppc = (HBITMAP)LoadImage(NULL, fileagrc, IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);
				//Filename tiene la dirreción de la imagen
				SendDlgItemMessage(hDlg, IDC_STACPLAAC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmppc);
				if (auxplac->disponible == true) {
					CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPYES);
				}
				else {
					CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPNO);
				}
				strcpy(auxpchconpla, auxplac->ingredientes);
				pch = strtok(auxpchconpla, "/");
				while (pch != NULL)
				{
					SendMessage(hlistingc, LB_ADDSTRING, 0, (LPARAM)pch);
					pch = strtok(NULL, "/");
				}
			}
			else {
				MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
			}
			return true;

		case IDC_BOT_PLAPREC:
			if (IsDlgButtonChecked(hDlg, IDC_RAD_PLADISPYES) == BST_CHECKED) {
				auxplac->disponible = true;
			}
			else {
				auxplac->disponible = false;
			}

			if (auxplac->ant != NULL) {
				SendMessage(hlistingc, LB_RESETCONTENT, 0, 0);
				auxplac = auxplac->ant;
				SendMessage(hnumpc, WM_SETTEXT, 50, (LPARAM)auxplac->numero);
				SendMessage(hnompc, WM_SETTEXT, 50, (LPARAM)auxplac->nombre);
				//10 por que lo queremos en base decimal
				_itoa(auxplac->precio, auxprecc, 10);
				SendMessage(hprecpc, WM_SETTEXT, 50, (LPARAM)auxprecc);
				SendMessage(hdescpc, WM_SETTEXT, 260, (LPARAM)auxplac->descripcion);
				strcpy(fileagrc, auxplac->imagen);
				//La dirección se encuentra en filename
				bmppc = (HBITMAP)LoadImage(NULL, fileagrc, IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);
				//Filename tiene la dirreción de la imagen
				SendDlgItemMessage(hDlg, IDC_STACPLAAC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmppc);
				if (auxplac->disponible == true) {
					CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPYES);
				}
				else {
					CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPNO);
				}
				strcpy(auxpchconpla, auxplac->ingredientes);
				pch = strtok(auxpchconpla, "/");
				while (pch != NULL)
				{
					SendMessage(hlistingc, LB_ADDSTRING, 0, (LPARAM)pch);
					pch = strtok(NULL, "/");
				}
			}
			else {
				MessageBox(hDlg, "Final de la Lista", "Aviso", MB_OK + MB_ICONINFORMATION);
			}

			return true;

			case IDC_BOT_PLAORDNUMC:
				quickSortPl(listap);
				auxplac = listap;
				SendMessage(hlistingc, LB_RESETCONTENT, 0, 0);
				SendMessage(hnumpc, WM_SETTEXT, 50, (LPARAM)auxplac->numero);
				SendMessage(hnompc, WM_SETTEXT, 50, (LPARAM)auxplac->nombre);
				//10 por que lo queremos en base decimal
				_itoa(auxplac->precio, auxprecc, 10);
				SendMessage(hprecpc, WM_SETTEXT, 50, (LPARAM)auxprecc);
				SendMessage(hdescpc, WM_SETTEXT, 260, (LPARAM)auxplac->descripcion);
				strcpy(fileagrc, auxplac->imagen);
				//La dirección se encuentra en filename
				bmppc = (HBITMAP)LoadImage(NULL, fileagrc, IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);
				//Filename tiene la dirreción de la imagen
				SendDlgItemMessage(hDlg, IDC_STACPLAAC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmppc);
				if (auxplac->disponible == true) {
					CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPYES);
				}
				else {
					CheckRadioButton(hDlg, IDC_RAD_PLADISPYES, IDC_RAD_PLADISPNO, IDC_RAD_PLADISPNO);
				}
				strcpy(auxpchconpla, auxplac->ingredientes);
				pch = strtok(auxpchconpla, "/");
				while (pch != NULL)
				{
					SendMessage(hlistingc, LB_ADDSTRING, 0, (LPARAM)pch);
					pch = strtok(NULL, "/");
				}
				MessageBox(hDlg, "Metodo de ordenación completado", "Aviso", MB_OK + MB_ICONINFORMATION);
			return true;
		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

//IDD_REG_PED
BOOL CALLBACK AboutDlgREG_PED(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnumme, hnombre, hmesa, hemi, hpaga, hanot, hselecc, hdispo;
	UINT indicee;
	static int toindexped, iped = 0, TotalPagar;
	membresia * toauxmemped = NULL;
	platillos * toauxpla = NULL;
	pedidos * auxpedid = NULL;
	static char textoped[256], auxprecped[50], auxselped[256], fecha[80],toauxprecp[50];
	char textoped1[50];
	static bool encontradopedi = false;
	switch (msg)
	{
	case WM_INITDIALOG:
		hnumme = GetDlgItem(hDlg, IDC_ED_PEDNUMM);
		hnombre = GetDlgItem(hDlg, IDC_ED_PEDNOMCL);
		hmesa = GetDlgItem(hDlg, IDC_ED_PEDMESA);
		hemi = GetDlgItem(hDlg, IDC_ED_PEDEMIS);
		hpaga = GetDlgItem(hDlg, IDC_ED_PEDTOTALP);
		hanot = GetDlgItem(hDlg, IDC_ED_PEDANOT);
		hselecc = GetDlgItem(hDlg, IDC_LIST_PEDSELC);
		hdispo = GetDlgItem(hDlg, IDC_LIST_PEDDISP);
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(fecha, 80, "%d/%m/%Y", timeinfo);
		SendMessage(hemi, WM_SETTEXT, 80, (LPARAM)fecha);

		toauxpla = listap;
		if (toauxpla == NULL) {
			MessageBox(hDlg, "No se encuentran platillos registrados", "AVISO", MB_OK + MB_ICONINFORMATION);
			EndDialog(hDlg, 0);
		}
		else {
			while (toauxpla != NULL) {
				SendMessage(hdispo, LB_ADDSTRING, 0, (LPARAM)toauxpla->nombre);
				toauxpla = toauxpla->sig;
			}
		}

		return true;
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{			
			case IDC_RAD_PEDYES:
				SendMessage(hnumme, WM_SETTEXT, 10, (LPARAM)"");
				Edit_Enable(hnumme, TRUE );
				//SendMessage(hnumme, ES_READONLY, false, 0);
				return true;

			case IDC_RAD_PEDNO:
				SendMessage(hnumme, WM_SETTEXT, 10, (LPARAM)"0");
				Edit_Enable(hnumme, FALSE);
				//SendMessage(hnumme, ES_READONLY, true, 0);
				return true;

			case IDC_BOT_VERIFICAR:
				toauxmemped = listam;
				char numeromem[50];
				SendMessage(hnumme, WM_GETTEXT, 50, (LPARAM)numeromem);
				while (toauxmemped != NULL) {
					if (strcmp(toauxmemped->numero, numeromem) == 0) {
						SendMessage(hnombre, WM_SETTEXT, 50, (LPARAM)toauxmemped->nombre);
						encontradopedi = true;
					}
					toauxmemped = toauxmemped->sig;
				}
				if (encontradopedi == false) {
					MessageBox(hDlg, "Membresia Inexistente", "AVISO", MB_OK + MB_ICONERROR);
					SendMessage(hnumme, WM_SETTEXT, 10, (LPARAM)"0");
					Edit_Enable(hnumme, FALSE);
				} 
				encontradopedi = false;
				return true;

			case IDC_BOT_PEDAGL:
				indicee = SendMessage(hdispo, LB_GETCURSEL, 0, 0);
				SendMessage(hdispo, LB_GETTEXT, indicee, (LPARAM)textoped);
				SendMessage(hselecc, LB_ADDSTRING, 0, (LPARAM)textoped);

				//10 por que lo queremos en base decimal
				toauxpla = listap;
				while (toauxpla != NULL) {
					if (strcmp(toauxpla->nombre, textoped) == 0) {
						TotalPagar += toauxpla->precio;
					}
					toauxpla = toauxpla->sig;
				}
				_itoa(TotalPagar, toauxprecp, 10);
				SendMessage(hpaga, WM_SETTEXT, 50, (LPARAM)toauxprecp);
				return true;

			case IDC_BOT_PEDELL:
				indicee = SendMessage(hselecc, LB_GETCURSEL, 0, 0);
				SendMessage(hselecc, LB_GETTEXT, indicee, (LPARAM)textoped1);
				SendMessage(hselecc, LB_DELETESTRING, indicee, 0);
				//SendMessage(hlistagr, LB_ADDSTRING, 0, (LPARAM)texto);
				toauxpla = listap;
				while (toauxpla != NULL) {
					if (strcmp(toauxpla->nombre, textoped1) == 0) {

						TotalPagar -= toauxpla->precio;
					}
					toauxpla = toauxpla->sig;
				}
				_itoa(TotalPagar, toauxprecp, 10);
				SendMessage(hpaga, WM_SETTEXT, 50, (LPARAM)toauxprecp);
				return true;

			case IDC_BOT_PEDREG:

				pedidos * newped = new pedidos;
				SendMessage(hnumme, WM_GETTEXT, 50, (LPARAM)newped->nummem);
				SendMessage(hnombre, WM_GETTEXT, 50, (LPARAM)newped->nombre);
				SendMessage(hmesa, WM_GETTEXT, 20, (LPARAM)newped->mesa);
				SendMessage(hemi, WM_GETTEXT, 20, (LPARAM)newped->emi);
				SendMessage(hpaga, WM_GETTEXT, 50, (LPARAM)auxprecped);
				newped->pagar = atoi(auxprecped);
				SendMessage(hanot, WM_GETTEXT, 260, (LPARAM)newped->anotaciones);


				toindexped = SendMessage(hselecc, LB_GETCOUNT, 0, 0);
				while (iped < toindexped)
				{
					if (iped<1) {
						SendMessage(hselecc, LB_GETTEXT, iped, (LPARAM)newped->selecc);
					}
					else {
						SendMessage(hselecc, LB_GETTEXT, iped, (LPARAM)auxselped);
						strcat(newped->selecc, "/");
						strcat(newped->selecc, auxselped);
					}
					iped++;
				}
				iped = 0;

				if ((*(newped->nummem) == NULL) || (*(newped->nombre) == NULL) || (*(newped->mesa) == NULL) || (*(newped->emi) == NULL) || ((newped->pagar) == NULL) || (*(newped->anotaciones) == NULL)) {
					MessageBox(hDlg, "Faltan datos por capturar", "AVISO", MB_OK + MB_ICONERROR);
					delete newped;
				}
				else {
					newped->ant = NULL;
					newped->sig = NULL;
					if (listaped == NULL) {
						listaped = newped;
					}
					else {
						auxpedid = listaped;
						while (auxpedid->sig != NULL) {
							auxpedid = auxpedid->sig;
						}
						auxpedid->sig = newped;
						newped->ant = auxpedid;
					}

					MessageBox(hDlg, "Platillo agregado correctamente", "AVISO", MB_OK + MB_ICONINFORMATION);
					//SendMessage(hemim, WM_SETTEXT, 10, (LPARAM)" ");
					SendMessage(hnumme, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(hnombre, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(hmesa, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(hpaga, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(hanot, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(hselecc, LB_RESETCONTENT, 0, 0);
					TotalPagar = 0;
				}
				return true;
		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

string obtenerDireccion(string filename) {
		WCHAR path[MAX_PATH];
		GetModuleFileNameW(NULL, path, MAX_PATH);
		char ch[260];
		char DefChar = ' ';
		WideCharToMultiByte(CP_ACP, 0, path, -1, ch, 260, &DefChar, NULL);
		string str(ch);
		size_t found = str.find_last_of("/\\");
		string pathoriginal = str.substr(0, found);
		string fileexecutable = str.substr(found + 1);
		string pathfile = pathoriginal + "\\" + filename;
	return pathfile;
}

char *AbrirArchivos(HWND Dialogo)
{
	char *nombre = "";
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	HANDLE hf;              // file handle
							// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Dialogo;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Todos\0*.*\0Imagenes BMP\0*.BMP\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn) == TRUE)
	{
		nombre = ofn.lpstrFile;
	}
	else
	{
		nombre = "";
		MessageBox(Dialogo, "No eligió archivo", "Aviso", MB_OK | MB_ICONINFORMATION);
	}
	return nombre;
}

//Quicksort para memebresia
struct membresia *lastNode(membresia *root)
{
	while (root && root->sig)
		root = root->sig;
	return root;
}

// Cambio de variables usando auxiliares
void swap(char* num1, char* nom1, char* emi1, char* exp1, char* img1, char* num2, char* nom2, char* emi2, char* exp2, char* img2)
{
	char auxnum[50], auxnom[50], auxemi[20], auxexp[20], auximg[256];
	//Copio los datos a las variables auxiliares
	strcpy(auxnum, num1);
	strcpy(auxnom, nom1);
	strcpy(auxemi, emi1);
	strcpy(auxexp, exp1);
	strcpy(auximg, img1);
	//paso la variables a intercambiar
	strcpy(num1, num2);
	strcpy(nom1, nom2);
	strcpy(emi1, emi2);
	strcpy(exp1, exp2);
	strcpy(img1, img2);
	//Regresar el cambio 
	strcpy(num2, auxnum);
	strcpy(nom2, auxnom);
	strcpy(emi2, auxemi);
	strcpy(exp2, auxexp);
	strcpy(img2, auximg);
}

/* Considers last element as pivot, places the pivot element at its
correct position in sorted array, and places all smaller (smaller than
pivot) to left of pivot and all greater elements to right of pivot */
membresia* partition(membresia *l, membresia *h)
{
	// set pivot as h element
	int auxj;
	int x;
	x = atoi(h->numero);

	// similar to i = l-1 for array implementation
	membresia *i = l->ant;

	// Similar to "for (int j = l; j <= h- 1; j++)"
	for (membresia *j = l; j != h; j = j->sig)
	{
		auxj = atoi(j->numero);
		if (auxj <= x)
		{
			// Similar to i++ for array
			i = (i == NULL) ? l : i->sig;
			swap( (i->numero), (i->nombre), (i->emision), (i->expedicion), (i->imagen), (j->numero), (j->nombre), (j->emision), (j->expedicion), (j->imagen));
		}
	}
	i = (i == NULL) ? l : i->sig; // Similar to i++
	swap((i->numero), (i->nombre), (i->emision), (i->expedicion), (i->imagen), (h->numero), (h->nombre), (h->emision), (h->expedicion), (h->imagen));
	return i;
}

/* Implementación recursiva de Quicksort controlada */
void _quickSort(struct membresia* l, struct membresia *h)
{
	if (h != NULL && l != h && l != h->sig)
	{
		struct membresia *p = partition(l, h);
		_quickSort(l, p->ant);
		_quickSort(p->sig, h);
	}
}

// Llamo a la función _quickSort()
void quickSort(struct membresia *head)
{
	// Find last node
	struct membresia *h = lastNode(head);

	// Call the recursive QuickSort
	_quickSort(head, h);
}


//Quicksort para Almacén
struct almacen *lastNodeA(almacen *root)
{
	while (root && root->sig)
		root = root->sig;
	return root;
}

void swapA(char* num1, char* nom1, char* fecha1, int* cant1, bool* tipo1, char* num2, char* nom2, char* fecha2, int* cant2, bool* tipo2)
{
	char auxnum[50], auxnom[50], auxfecha[20];
	int auxcant;
	bool auxtipo;

	//Copio los datos a las variables auxiliares
	strcpy(auxnum, num1);
	strcpy(auxnom, nom1);
	strcpy(auxfecha, fecha1);
	auxcant = *cant1;
	auxtipo = *tipo1;

	//paso la variables a intercambiar
	strcpy(num1, num2);
	strcpy(nom1, nom2);
	strcpy(fecha1, fecha2);
	*cant1 = *cant2;
	*tipo1 = *tipo2;

	//Regresar el cambio 
	strcpy(num2, auxnum);
	strcpy(nom2, auxnom);
	strcpy(fecha2, auxfecha);
	*cant2 = auxcant;
	*tipo2 = auxtipo;
}

almacen* partitionA(almacen *l, almacen *h)
{
	int auxj;
	int x;
	x = atoi(h->numero);

	almacen *i = l->ant;

	for (almacen *j = l; j != h; j = j->sig)
	{
		auxj = atoi(j->numero);
		if (auxj <= x)
		{
			// Similar to i++ for array
			i = (i == NULL) ? l : i->sig;
			swapA((i->numero), (i->nombre), (i->fecha), &(i->cantidad), &(i->tipo), (j->numero), (j->nombre), (j->fecha), &(j->cantidad), &(j->tipo));
		}
	}
	i = (i == NULL) ? l : i->sig; // Similar to i++
	swapA((i->numero), (i->nombre), (i->fecha), &(i->cantidad), &(i->tipo), (h->numero), (h->nombre), (h->fecha), &(h->cantidad), &(h->tipo));
	return i;
}

void _quickSortA(struct almacen* l, struct almacen *h)
{
	if (h != NULL && l != h && l != h->sig)
	{
		struct almacen *p = partitionA(l, h);
		_quickSortA(l, p->ant);
		_quickSortA(p->sig, h);
	}
}

void quickSortA(struct almacen *head)
{
	// Find last node
	struct almacen *h = lastNodeA(head);

	// Call the recursive QuickSort
	_quickSortA(head, h);
}


//Quicksort para Pedidos
struct pedidos *lastNodeP(pedidos *root)
{
	while (root && root->sig)
		root = root->sig;
	return root;
}

void swapP(char* num1, char* nom1, char* mesa1, char* emi1, int* pagar1, char* selecc1, char* anota1, char* num2, char* nom2, char* mesa2, char* emi2, int* pagar2, char* selecc2, char* anota2)
{
	char auxnum[50], auxnom[50], auxmesa[20], auxemi[20], auxselecc[260], auxanota[260];
	int auxpagar;

	//Copio los datos a las variables auxiliares
	strcpy(auxnum, num1);
	strcpy(auxnom, nom1);
	strcpy(auxmesa, mesa1);
	strcpy(auxemi, emi1);
	auxpagar = *pagar1;
	strcpy(auxselecc, selecc1);
	strcpy(auxanota, anota1);
	
	//paso la variables a intercambiar
	strcpy(num1, num2);
	strcpy(nom1, nom2);
	strcpy(mesa1, mesa2);
	strcpy(emi1, emi2);
	*pagar1 = *pagar2;
	strcpy(selecc1, selecc2);
	strcpy(anota1, anota2);
	
	//Regresar el cambio 
	strcpy(num2, auxnum);
	strcpy(nom2, auxnom);
	strcpy(mesa2, auxmesa);
	strcpy(emi2, auxemi);
	*pagar2 = auxpagar;
	strcpy(selecc2, auxselecc);
	strcpy(anota2, auxanota);

}

pedidos* partitionP(pedidos *l, pedidos *h)
{
	int auxj;
	int x;
	x = atoi(h->nummem);

	pedidos *i = l->ant;

	for (pedidos *j = l; j != h; j = j->sig)
	{
		auxj = atoi(j->nummem);
		if (auxj <= x)
		{
			// Similar to i++ for array
			i = (i == NULL) ? l : i->sig;
			swapP((i->nummem), (i->nombre), (i->mesa), (i->emi), &(i->pagar), (i->selecc), (i->anotaciones), (j->nummem), (j->nombre), (j->mesa), (j->emi), &(j->pagar), (j->selecc), (j->anotaciones));
		}
	}
	i = (i == NULL) ? l : i->sig; // Similar to i++
	swapP((i->nummem), (i->nombre), (i->mesa), (i->emi), &(i->pagar), (i->selecc), (i->anotaciones), (h->nummem), (h->nombre), (h->mesa), (h->emi), &(h->pagar), (h->selecc), (h->anotaciones));
	return i;
}

void _quickSortP(struct pedidos* l, struct pedidos *h)
{
	if (h != NULL && l != h && l != h->sig)
	{
		struct pedidos *p = partitionP(l, h);
		_quickSortP(l, p->ant);
		_quickSortP(p->sig, h);
	}
}

void quickSortP(struct pedidos *head)
{
	// Find last node
	struct pedidos *h = lastNodeP(head);

	// Call the recursive QuickSort
	_quickSortP(head, h);
}


//Quicksort para Platillos
struct platillos *lastNodePl(platillos *root)
{
	while (root && root->sig)
		root = root->sig;
	return root;
}

void swapPl(char* num1, char* nom1, int* prec1, bool* disp1, char* img1, char* ing1, char* descp1, char* num2, char* nom2, int* prec2, bool* disp2, char* img2, char* ing2, char* descp2)
{
	char auxnum[50], auxnom[50], auximg[260], auxing[260], auxdesc[260];
	int auxpre;
	bool disp;

	//Copio los datos a las variables auxiliares
	strcpy(auxnum, num1);
	strcpy(auxnom, nom1);
	auxpre = *prec1;
	disp = *disp1;
	strcpy(auximg, img1);
	strcpy(auxing, ing1);
	strcpy(auxdesc, descp1);

	//paso la variables a intercambiar
	strcpy(num1, num2);
	strcpy(nom1, nom2);
	*prec1 = *prec2;
	*disp1 = *disp2;
	strcpy(img1, img2);
	strcpy(ing1, ing2);
	strcpy(descp1, descp2);
	

	//Regresar el cambio 
	strcpy(num2, auxnum);
	strcpy(nom2, auxnom);
	*prec2 = auxpre;
	*disp2 = disp;
	strcpy(img2, auximg);
	strcpy(ing2, auxing);
	strcpy(descp2, auxdesc);

}

platillos* partitionPl(platillos *l, platillos *h)
{
	int auxj;
	int x;
	x = atoi(h->numero);

	platillos *i = l->ant;

	for (platillos *j = l; j != h; j = j->sig)
	{
		auxj = atoi(j->numero);
		if (auxj <= x)
		{
			// Similar to i++ for array
			i = (i == NULL) ? l : i->sig;
			swapPl((i->numero), (i->nombre), &(i->precio), &(i->disponible), (i->imagen), (i->ingredientes), (i->descripcion), (j->numero), (j->nombre), &(j->precio), &(j->disponible), (j->imagen), (j->ingredientes), (j->descripcion));
		}
	}
	i = (i == NULL) ? l : i->sig; // Similar to i++
	swapPl((i->numero), (i->nombre), &(i->precio), &(i->disponible), (i->imagen), (i->ingredientes), (i->descripcion), (h->numero), (h->nombre), &(h->precio), &(h->disponible), (h->imagen), (h->ingredientes), (h->descripcion));
	return i;
}

void _quickSortPl(struct platillos* l, struct platillos *h)
{
	if (h != NULL && l != h && l != h->sig)
	{
		struct platillos *p = partitionPl(l, h);
		_quickSortPl(l, p->ant);
		_quickSortPl(p->sig, h);
	}
}

void quickSortPl(struct platillos *head)
{
	// Find last node
	struct platillos *h = lastNodePl(head);

	// Call the recursive QuickSort
	_quickSortPl(head, h);
}

void makeheap(int a[], int n)
{
	int i, temp, k, j;
	for (i = 1; i<n; i++)

	{
		temp = a[i];
		k = (i - 1) / 2;
		j = i;
		while (j>0 && a[k] > temp)
		{
			a[j] = a[k];
			j = k;
			k = (j - 1) / 2;
		}
		a[j] = temp;
	}
}
void heapsort(int a[], int n)
{
	int temp, i, j;
	for (i = n - 1; i >= 1; i--)
	{
		temp = a[i];
		a[i] = a[0];
		a[0] = temp;
		makeheap(a, i);

	}
}


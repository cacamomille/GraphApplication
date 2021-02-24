
// GraphApplication.cpp: define los comportamientos de las clases para la aplicación.
//

#include "pch.h"
#include "framework.h"
#include "GraphApplication.h"
#include "GraphApplicationDlg.h"
#include <io.h>
#include <fcntl.h>
#include <iomanip>  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// =============================================================================
// IDENTIFICACION DE LOS ALUMNOS ===============================================
// =============================================================================

CString NombreAlumno1 = "Camille";
CString ApellidosAlumno1 = "Caudrillier";
CString NIUAlumno1 = "1591320"; // NIU alumno1

// No rellenar en caso de grupo de un alumno
CString NombreAlumno2 = "nombre del alumno 2";
CString ApellidosAlumno2 = "apellidos del alumno 2";
CString NIUAlumno2 = ""; // NIU alumno2

bool CheckNIU(const char* niu)
{
	char* filename= "Alumnes.csv";
	ifstream nius(filename);

	if (!nius.good()) {
		cout << "Error opening file Alumnes.csv" << endl;
		filename = "..\\Alumnes.csv";
		nius.open(filename);
		if (!nius.good()) {
			cout << "Error opening file ..\\Alumnes.csv" << endl;
			filename = "..\\..\\Alumnes.csv";
			nius.open(filename);
			if (!nius.good()) {
				cout << "Error opening file ..\\..\\Alumnes.csv" << endl;
				filename = "..\\..\\..\\Alumnes.csv";
				nius.open(filename);
				if (!nius.good()) {
					cout << "Error opening file ..\\..\\..\\Alumnes.csv" << endl;
					return false;
				}
			}
		}	
	}
	cout << filename << " readed" << endl;
	char buf[256];
	while (!nius.eof()) {
		nius.getline(buf, 256);
		if (buf[0] == '\0') break;
		if (strncmp(niu, buf, 7) == 0) return true;
	}
	return false;
}

// ReadCommandArguments ========================================================

void ReadCommandArguments(int &nArgs, CString args[])
{
	nArgs = 0;
	char* pCommand = GetCommandLineA();
	//cout << pCommand << endl;
	char *pArg = pCommand;
	// Saltar el nombre del ejecutable
	while (*pArg <= ' ') if (*pArg) ++pArg; else return;
	if (*pArg == '\"') {
		// Leer hasta "
		++pArg;
		while (*pArg != '\"') {
			if (*pArg) ++pArg;
			else return;
		}
		++pArg;
	}
	else {
		while (*pArg > ' ') {
			if (*pArg) ++pArg;
			else return;
		}
	}
	// Saltar espacios
	while (*pArg == ' ') ++pArg;
	if (*pArg == '\0') return;
	//cout << "Linea de comandos " << (int) *pArg << endl;
	//AttachConsole(ATTACH_PARENT_PROCESS);
	//ofstream *pout = new ofstream("CONOUT$", ofstream::out | ofstream::app);
	//cout.rdbuf(pout->rdbuf());

	while (*pArg != '\0') {
		// Saltar espacios
		while (*pArg == ' ') ++pArg;
		if (*pArg == '\0') break;
		else if (*pArg == '\"') {
			++pArg;
			for (;;) {
				if (*pArg == '\0') break;
				else if (*pArg == '\"') break;
				args[nArgs].AppendChar(*pArg);
				++pArg;
			}
		}
		else {
			while (*pArg > ' ') {
				args[nArgs].AppendChar(*pArg);
				++pArg;
			}
		}
		++nArgs;
		if (nArgs >= 10) {
			return;
		}
	}
}

// EjecutarLineaDeComandos =====================================================

bool EjecutarLineaDeComandos(int nArgs, CString args[])
{
	cout << "NARGS " << nArgs << endl;
	if (nArgs < 3) {
		cout << "Número de argumentos erroneo" << endl;
		return true;
	}
	cout << "PARAMETROS:" << endl;

	if (args[1].CompareNoCase("Prim") == 0) {
		if (nArgs != 4) {
			cout << "Número de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo....: " << args[1] << endl;
		cout << "Grafo........: " << args[2] << endl;
		cout << "SpanningTree.: " << args[3] << endl;
		try {
			CGraph graph(false);
			graph.Load(args[2]);
			graph.SetDistancesToEdgeLength();
			double t0 = Clock();
			CSpanningTree tree=SpanningTreePrim(graph);
			double t1 = Clock();
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
			tree.Save(args[3]);
		}
		catch (exception& ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("Kruscal") == 0) {
		if (nArgs != 4) {
			cout << "Número de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo....: " << args[1] << endl;
		cout << "Grafo........: " << args[2] << endl;
		cout << "SpanningTree.: " << args[3] << endl;
		try {
			CGraph graph(false);
			graph.Load(args[2]);
			graph.SetDistancesToEdgeLength();
			double t0 = Clock();
			CSpanningTree tree = SpanningTreeKruskal(graph);
			double t1 = Clock();
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
			tree.Save(args[3]);
		}
		catch (exception& ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("dijkstra") == 0) {
		// dijkstra grafo distancias
		if (nArgs != 4) {
			cout << "Número de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Distancias.: " << args[3] << endl;
		try {
			CGraph graph(false);
			graph.Load(args[2]);
			graph.SetDistancesToEdgeLength();
			CVertex* pStart = graph.FindVertex("Start");
			if (!pStart) {
				if (graph.m_Vertices.empty()) throw MyException("No es poden calcular distancies en un graf sense vertex");
				pStart = &graph.m_Vertices.front();
			}
			double t0 = Clock();
			Dijkstra(graph, pStart);
			double t1 = Clock();
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
			graph.SaveDistances(args[3]);
		}
		catch (exception& ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("dijkstraqueue") == 0) {
		// dijkstraqueue grafo distancias
		if (nArgs != 4) {
			cout << "Número de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Distancias.: " << args[3] << endl;
		try {
			CGraph graph(false);
			graph.Load(args[2]);
			graph.SetDistancesToEdgeLength();
			CVertex* pStart = graph.FindVertex("Start");
			if (!pStart) {
				if (graph.m_Vertices.empty()) throw MyException("No es poden calcular distancies en un graf sense vertex");
				pStart = &graph.m_Vertices.front();
			}
			double t0 = Clock();
			DijkstraQueue(graph, pStart);
			double t1 = Clock();
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
			graph.SaveDistances(args[3]);
		}
		catch (exception& ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("greedy") == 0) {
		if (nArgs != 5) {
			cout << "Número de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph(false);
			graph.Load(args[2]);
			graph.SetDistancesToEdgeLength();
			CVisits visits(&graph);
			visits.Load(args[3]);
			double t0 = Clock();
			CTrack track = SalesmanTrackGreedy(graph, visits);
			double t1 = Clock();
			track.Save(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
		}
		catch (exception& ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("backtracking") == 0) {
		if (nArgs != 5) {
			cout << "Número de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph(false);
			graph.Load(args[2]);
			graph.SetDistancesToEdgeLength();
			CVisits visits(&graph);
			visits.Load(args[3]);
			double t0 = Clock();
			CTrack track = SalesmanTrackBacktracking(graph, visits);
			double t1 = Clock();
			track.Save(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
		}
		catch (exception& ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("backtrackinggreedy") == 0) {
		if (nArgs != 5) {
			cout << "Número de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph(false);
			graph.Load(args[2]);
			graph.SetDistancesToEdgeLength();
			CVisits visits(&graph);
			visits.Load(args[3]);
			double t0 = Clock();
			CTrack track = SalesmanTrackBacktrackingGreedy(graph, visits);
			double t1 = Clock();
			track.Save(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
		}
		catch (exception& ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("branchandbound1") == 0) {
		if (nArgs != 5) {
			cout << "Número de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph(false);
			graph.Load(args[2]);
			graph.SetDistancesToEdgeLength();
			CVisits visits(&graph);
			visits.Load(args[3]);
			double t0 = Clock();
			CTrack track = SalesmanTrackBranchAndBound1(graph, visits);
			double t1 = Clock();
			track.Save(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
		}
		catch (exception& ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("branchandbound2") == 0) {
		if (nArgs != 5) {
			cout << "Número de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph(false);
			graph.Load(args[2]);
			graph.SetDistancesToEdgeLength();
			CVisits visits(&graph);
			visits.Load(args[3]);
			double t0 = Clock();
			CTrack track = SalesmanTrackBranchAndBound2(graph, visits);
			double t1 = Clock();
			track.Save(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
		}
		catch (exception& ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("branchandbound3") == 0) {
		if (nArgs != 5) {
			cout << "Número de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo..: " << args[1] << endl;
		cout << "Grafo......: " << args[2] << endl;
		cout << "Visitas....: " << args[3] << endl;
		cout << "Track......: " << args[4] << endl;
		try {
			CGraph graph(false);
			graph.Load(args[2]);
			graph.SetDistancesToEdgeLength();
			CVisits visits(&graph);
			visits.Load(args[3]);
			double t0 = Clock();
			CTrack track = SalesmanTrackBranchAndBound3(graph, visits);
			double t1 = Clock();
			track.Save(args[4]);
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << setprecision(9) << t1 - t0 << endl;
		}
		catch (exception& ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else if (args[1].CompareNoCase("QuickHull") == 0) {
		if (nArgs != 4) {
			cout << "Número de argumentos erroneo" << endl;
			return true;
		}
		cout << "Algoritmo....: " << args[1] << endl;
		cout << "Grafo........: " << args[2] << endl;
		cout << "Convex Hull..: " << args[3] << endl;
		try {
			CGraph graph(false);
			graph.Load(args[2]);
			double t0 = Clock();
			CConvexHull convexHull = QuickHull(graph);
			double t1 = Clock();
			cout << endl << "TIEMPO DE EJECUCION: " << fixed << t1 - t0 << endl;
			convexHull.Save(args[3]);
		}
		catch (exception& ex) {
			cout << "EXCEPTION: " << ex.what() << endl;
		}
	}
	else {
		cout << "Error en el nombre del algoritmo" << endl;
	}
	cout << "FIN EJECUCION COMANDOS" << endl;
	return true;
}

// CGraphApplicationApp

BEGIN_MESSAGE_MAP(CGraphApplicationApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Construcción de CGraphApplicationApp

CGraphApplicationApp::CGraphApplicationApp()
{
	// TODO: agregar aquí el código de construcción,
	// Colocar toda la inicialización importante en InitInstance
}


// Único objeto CGraphApplicationApp

CGraphApplicationApp theApp;


// Inicialización de CGraphApplicationApp

BOOL CGraphApplicationApp::InitInstance()
{
	ofstream sortida;
	int nArgs;
	CString args[10];
	ReadCommandArguments(nArgs, args);
	CString outputName = "Output.txt";
	if (nArgs > 1) outputName = args[0];
	bool lineaDeComandos = false;
	if (AttachConsole(ATTACH_PARENT_PROCESS)) {
		ofstream *pout = new ofstream(outputName);
		cout.rdbuf(pout->rdbuf());
		lineaDeComandos = true;
	}
	else {
		AllocConsole();
		sortida.open("CONOUT$");
		cout.rdbuf(sortida.rdbuf());
		//ofstream *pout = new ofstream("CONOUT$");
		//cout.rdbuf(pout->rdbuf());
	}
	bool error = false;
	cout << "Nom Alumne 1.....: " << NombreAlumno1 << endl;
	cout << "Cognoms Alumne 1.: " << ApellidosAlumno1 << endl;
	cout << "NIU Alumne 1.....:" << NIUAlumno1 << endl;
	if (!CheckNIU(NIUAlumno1)) {
		cout << " NIU ERRONEO!!!";
		error = true;
	}
	cout << endl;
	if (NIUAlumno2 != "") {
		cout << "Nom Alumne 2.....: " << NombreAlumno2 << endl;
		cout << "Cognoms Alumne 2.: " << ApellidosAlumno2 << endl;
		cout << "NIU Alumne 2.....:" << NIUAlumno2 << endl;
		if (!CheckNIU(NIUAlumno1)) {
			cout << " NIU ERRONEO!!!";
			error = true;
		}
		cout << endl;
	}
	if (error) {
		if (!lineaDeComandos) AfxMessageBox("ERROR EN LA IDENTIFICACION DE LOS ALUMNOS", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// Funcionamiento de la aplicación sólo en modo consola si tiene parámetros
	if (nArgs > 0 && EjecutarLineaDeComandos(nArgs, args)) return FALSE;
	// Windows XP requiere InitCommonControlsEx() si un manifiesto de
	// aplicación especifica el uso de ComCtl32.dll versión 6 o posterior para habilitar
	// estilos visuales.  De lo contrario, se generará un error al crear ventanas.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Establecer para incluir todas las clases de control comunes que desee utilizar
	// en la aplicación.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Crear el administrador de shell, en caso de que el cuadro de diálogo contenga
	// alguna vista de árbol de shell o controles de vista de lista de shell.
	CShellManager *pShellManager = new CShellManager;

	// Activar el administrador visual "nativo de Windows" para habilitar temas en controles MFC
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Inicialización estándar
	// Si no utiliza estas funcionalidades y desea reducir el tamaño
	// del archivo ejecutable final, debe quitar
	// las rutinas de inicialización específicas que no necesite
	// Cambie la clave del Registro en la que se almacena la configuración
	// TODO: debe modificar esta cadena para que contenga información correcta
	// como el nombre de su compañía u organización
	SetRegistryKey(_T("Aplicaciones generadas con el Asistente para aplicaciones local"));

	CGraphApplicationDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: insertar aquí el código para controlar
		//  cuándo se descarta el cuadro de diálogo con Aceptar
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: insertar aquí el código para controlar
		//  cuándo se descarta el cuadro de diálogo con Cancelar
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Advertencia: la aplicación está finalizando porque hubo un error al crear el cuadro de diálogo.\n");
		TRACE(traceAppMsg, 0, "Advertencia: si usa controles MFC en el cuadro de diálogo, no puede usar #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Eliminar el administrador de shell creado anteriormente.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Dado que el cuadro de diálogo se ha cerrado, devolver FALSE para salir
	//  de la aplicación en vez de iniciar el suministro de mensajes de dicha aplicación.
	return FALSE;
}


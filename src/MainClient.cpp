/*
	UDP Client routine
*/

#include "UDPSocket.h"
#include <iostream>
#include <stdio.h>

#include <Windows.h> //for Sleep and other time funcs, and thread funcs
typedef HANDLE THREADVAR;
typedef DWORD WINAPI THREADFUNCVAR;
typedef LPVOID THREADFUNCARGS;
typedef THREADFUNCVAR(*THREADFUNC)(THREADFUNCARGS);
typedef CRITICAL_SECTION THREAD_LOCK;


//cross platform funcs
THREADVAR SpawnThread(THREADFUNC f, THREADFUNCARGS arg);
void StopThread(THREADVAR t);
void InitThreadLock(THREAD_LOCK& t);
void LockThread(THREAD_LOCK& t);
void UnlockThread(THREAD_LOCK& t);
void sleep(int ms);



// the thread function declaration
THREADFUNCVAR MyAsyncThread(THREADFUNCARGS lpParam);

// globals
//struct sockaddr_in si_other;
//int slen = sizeof(si_other);

char buf[BUFLEN];
char message[BUFLEN];

unsigned short srv_port = 0;
char srv_ip_addr[40];

//create socket
UDPSocket client_sock;



///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                      //
/// entry point                                                                                          //
///                                                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//int main() {
//	THREAD_LOCK recv_lock;
//	InitThreadLock(recv_lock);
//
//	// prepare parameter(s) for the async thread
//	DWORD_PTR* svRecvThrArgs = new DWORD_PTR[1];
//	// pass the thread lock variable as parameter
//	svRecvThrArgs[0] = (DWORD_PTR)&recv_lock;
//
//	// start the thread with parameters
//	THREADVAR recvThrHandle = SpawnThread(MyAsyncThread, (THREADFUNCARGS)svRecvThrArgs);
//
//	// loop increment and check for exit
//	while (!quitnow) {
//		///*
//		LockThread(recv_lock); // lock with the same var
//
//		globlint++;
//		if (globlint > 5000) {
//			quitnow = true;
//		}
//		cout << "main " << globlint << endl;
//		//cout << "\nmain loop, in LockThread block\n";
//		UnlockThread(recv_lock); // unlock with the same var
////*/
//		sleep(10);
//		//		cout << "\nmain loop, after sleep\n";
//	}
//
//	
//
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                      //
/// thread                                                                                               //
///                                                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//THREADFUNCVAR MyAsyncThread(THREADFUNCARGS lpParam) {
//	// get parameter array
//	DWORD_PTR* arg = (DWORD_PTR*)lpParam;
//	// get 0th param, thats the thread lock variable used in main func
//	THREAD_LOCK& ref_recv_lock = *((THREAD_LOCK*)arg[0]);
//
//	// loop increment, check for exit and print
//	while (!quitnow) {
//		LockThread(ref_recv_lock);  // lock with the same var
//		globlint++;
//		if (globlint > 500) {
//			quitnow = true;
//		}
//		cout << "function " << globlint << endl;
//		UnlockThread(ref_recv_lock);  // unlock with the same var
//		sleep(1);
//	}
//	return NULL;
//}






///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                      //
/// cross platform definitions                                                                           //
///                                                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

THREADVAR SpawnThread(THREADFUNC f, THREADFUNCARGS arg) 
{
	DWORD thrId;
	THREADVAR out = CreateThread(
		NULL,          // default security attributes
		0,             // use default stack size  
		(LPTHREAD_START_ROUTINE)f,    			// thread function name
		arg,          // argument to thread function 
		0,             // use default creation flags 
		&thrId			// returns the thread identifier 
	);
	return out;
}

void StopThread(THREADVAR t) {
#ifdef _COMPILE_LINUX
	pthread_exit((void*)t);
#endif
#ifdef _COMPILE_WINDOWS
	TerminateThread(t, 0);
	CloseHandle(t);
#endif
}


void InitThreadLock(THREAD_LOCK& t) {
#ifdef _COMPILE_LINUX
	t = PTHREAD_MUTEX_INITIALIZER;
	//UnlockThread(t);
	//pthread_mutex_init(&t, NULL);
#endif
#ifdef _COMPILE_WINDOWS
	InitializeCriticalSection(&t);
#endif
}

void LockThread(THREAD_LOCK& t) {
#ifdef _COMPILE_LINUX
	pthread_mutex_lock(&t);
#endif
#ifdef _COMPILE_WINDOWS
	EnterCriticalSection(&t);
#endif
}

void UnlockThread(THREAD_LOCK& t) {
#ifdef _COMPILE_LINUX
	pthread_mutex_unlock(&t);
#endif
#ifdef _COMPILE_WINDOWS
	LeaveCriticalSection(&t);
#endif
}


void sleep(int ms) {
#ifdef _COMPILE_LINUX
	usleep(ms * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#endif
#ifdef _COMPILE_WINDOWS
	Sleep(ms);
#endif
}


int main(int argc, char* argv[])
{
//	SOCKET s;
	struct sockaddr_in si_other;
	int slen = sizeof(si_other);

	//char buf[BUFLEN];
	//char message[BUFLEN];

	//unsigned short srv_port = 0;
	//char srv_ip_addr[40];
	//memset(srv_ip_addr, 0, 40);

	//create socket
	//UDPSocket client_sock;

	//setup address structure
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;

	//si_other.sin_port = htons(PORT);
	//si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	if (1 == argc)
	{
		si_other.sin_port = htons(PORT);
		si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
		printf("1: Server - addr=%s , port=%d\n", SERVER, PORT);
	}
	else if (2 == argc)
	{
//		si_other.sin_port = htons(PORT);
//		si_other.sin_addr.S_un.S_addr = inet_addr(argv[1]);
		si_other.sin_port = htons(atoi(argv[1]));
		si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
		printf("2: argv[0]: Server - addr=%s , port=%d\n", SERVER, atoi(argv[1]));
	}
	else
	{
		si_other.sin_port = htons( atoi(argv[2]) );
		si_other.sin_addr.S_un.S_addr = inet_addr(argv[1]);
		printf("3: Server - addr=%s , port=%d\n", argv[1], atoi(argv[2]));
	}

	//start communication  lifecircle

	THREAD_LOCK recv_lock;
	InitThreadLock(recv_lock);

	// prepare parameter(s) for the async thread
	DWORD_PTR* svRecvThrArgs = new DWORD_PTR[1];
	// pass the thread lock variable as parameter
	svRecvThrArgs[0] = (DWORD_PTR)&recv_lock;

	// start the thread with parameters
	THREADVAR recvThrHandle = SpawnThread(MyAsyncThread, (THREADFUNCARGS)svRecvThrArgs);
	while (1)
	{
		memset(buf, '\0', BUFLEN);
		LockThread(recv_lock); // lock with the same var
		client_sock.RecvDatagram(buf, BUFLEN, (struct sockaddr*)&si_other, &slen);
		puts(buf);
		UnlockThread(recv_lock); // unlock with the same var
	}
	StopThread(recvThrHandle);
	return 0;
}

THREADFUNCVAR MyAsyncThread(THREADFUNCARGS lpParam) {
	struct sockaddr_in si_other;
	int slen = sizeof(si_other);
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;

	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	// get parameter array
	DWORD_PTR* arg = (DWORD_PTR*)lpParam;
	// get 0th param, thats the thread lock variable used in main func
	THREAD_LOCK& ref_recv_lock = *((THREAD_LOCK*)arg[0]);

	// loop increment, check for exit and print
	while (1) {
		LockThread(ref_recv_lock);  // lock with the same var
		printf("\nEnter message : ");
		gets_s(message, BUFLEN);
		client_sock.SendDatagram(message, (int)strlen(message), (struct sockaddr*)&si_other, slen);
		UnlockThread(ref_recv_lock);  // unlock with the same var
	}
	return NULL;
}

#include "Windows.h"
#include "Direct.h"
#include "iostream"

using namespace std;



bool IsOnlyInstance(LPCTSTR gameTitle) {
	HANDLE handle = CreateMutex(NULL, TRUE, gameTitle);
	if (GetLastError() != ERROR_SUCCESS) {
		HWND hWnd = FindWindow(gameTitle, NULL);
		if (hWnd) {
			// An instance of your game is already running.
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			cout << "There is no game instance currently running." << endl;
			return false;
		}
	}
	cout << "There is currently a game instance running." << endl;
	return true;
}

bool CheckStorage(const DWORDLONG diskSpaceNeeded) {
	// Check for enough free disk space on the current disk.
	int const drive = _getdrive();
	struct _diskfree_t diskfree;
	_getdiskfree(drive, &diskfree);
	unsigned __int64 const neededClusters = diskSpaceNeeded /
		(diskfree.sectors_per_cluster*diskfree.bytes_per_sector);
	if (diskfree.avail_clusters < neededClusters) {
		// if you get here you don’t have enough disk space!
		cout<<"CheckStorage Failure : Not enough physical storage."<<endl;
		return false;
	}
	return true;
}

bool CheckMemory() {
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	cout<<"Available Physical Memory:" << status.ullAvailPhys<<" Bytes"<<endl;
		
	
	
	
	cout<<"Available Virtual Memory:"<< status.ullAvailVirtual<<" Bytes"<<endl;
	return false;
	
	
}

DWORD ReadCPUSpeed() {
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;
	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, reinterpret_cast<LPCTSTR>("\\DESCRIPTION\\System\\CentralProcessor\\0"), 0, KEY_READ, &hKey);
	if (lError == ERROR_SUCCESS) {
		// query the key:
		RegQueryValueEx(hKey, reinterpret_cast<LPCTSTR>("MHz"), NULL, &type, (LPBYTE)
			&dwMHz, &BufSize);
	}
	return dwMHz;
}

int main()
{
	IsOnlyInstance("game");
	CheckStorage(300);
	CheckMemory();
	cout << "CPU Speed: " << ReadCPUSpeed() << endl;
	system("pause");
	return 0;
}
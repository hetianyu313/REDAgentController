#include <bits/stdc++.h>
#include <winsock2.h>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "psapi.lib")
//-lpsapi -lws2_32
namespace nwb{//NetworkBlocker
	//���¿�����Ψ������ 
	DWORD g_targetPid = 0;
	BYTE g_originalBytes[5];
	int (WINAPI *g_pOriginalConnect)(SOCKET, const sockaddr*, int) = nullptr;
	int WINAPI HookedConnect(SOCKET s, const sockaddr* name, int namelen) {
	    if (GetCurrentProcessId() == g_targetPid) {
	        WSASetLastError(WSAEACCES);
	        return SOCKET_ERROR;
	    }
	    // �ָ�ԭʼ����
	    DWORD oldProtect;
	    VirtualProtect((LPVOID)g_pOriginalConnect, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	    memcpy((LPVOID)g_pOriginalConnect, g_originalBytes, 5);
	    VirtualProtect((LPVOID)g_pOriginalConnect, 5, oldProtect, &oldProtect);
	    // ����ԭʼ����
	    int result = g_pOriginalConnect(s, name, namelen);
	    // �������ù���
	    VirtualProtect((LPVOID)g_pOriginalConnect, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	    BYTE jmp[5] = { 0xE9 };
	    *(DWORD*)(jmp + 1) = (DWORD)HookedConnect - (DWORD)g_pOriginalConnect - 5;
	    memcpy((LPVOID)g_pOriginalConnect, jmp, 5);
	    VirtualProtect((LPVOID)g_pOriginalConnect, 5, oldProtect, &oldProtect);
	    return result;
	}
	BOOL SetHook(DWORD pid) {
	    g_targetPid = pid;
	    HMODULE hWs2 = LoadLibrary("ws2_32.dll");
	    if (!hWs2) return FALSE;
	    g_pOriginalConnect = (int(WINAPI*)(SOCKET, const sockaddr*, int))GetProcAddress(hWs2, "connect");
	    if (!g_pOriginalConnect) return FALSE;
	    // ����ԭʼ�ֽ�
	    memcpy(g_originalBytes, (LPVOID)g_pOriginalConnect, 5);
	    // �޸��ڴ汣������
	    DWORD oldProtect;
	    if (!VirtualProtect((LPVOID)g_pOriginalConnect, 5, PAGE_EXECUTE_READWRITE, &oldProtect))
	        return FALSE;
	    // д����תָ��
	    BYTE jmp[5] = { 0xE9 };
	    *(DWORD*)(jmp + 1) = (DWORD)HookedConnect - (DWORD)g_pOriginalConnect - 5;
	    memcpy((LPVOID)g_pOriginalConnect, jmp, 5);
	    VirtualProtect((LPVOID)g_pOriginalConnect, 5, oldProtect, &oldProtect);
	    return TRUE;
	}
	BOOL RemoveHook() {
	    if (!g_pOriginalConnect) return TRUE;
	    DWORD oldProtect;
	    VirtualProtect((LPVOID)g_pOriginalConnect, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	    memcpy((LPVOID)g_pOriginalConnect, g_originalBytes, 5);
	    VirtualProtect((LPVOID)g_pOriginalConnect, 5, oldProtect, &oldProtect);
	    g_pOriginalConnect = nullptr;
	    return TRUE;
	}
	int main_nwb() {
	    DWORD pid;
	    printf("Enter PID to block: ");
	    scanf("%d", &pid);
	    if (SetHook(pid)) {
	        printf("Network access blocked for PID %d\n", pid);
	        printf("Press any key to unblock...\n");
	        getchar();
	        RemoveHook();
	    } else {
	        printf("Failed to block PID %d\n", pid);
	    }
	    return 0;
	}
}



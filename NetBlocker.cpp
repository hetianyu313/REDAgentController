#include <bits/stdc++.h>
#include <winsock2.h>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "psapi.lib")
//-lpsapi -lws2_32
//天下控屏，唯钩不破 
//NetworkBlocker
using namespace std;
namespace nwb_old{
	DWORD g_targetPid = 0;
	BYTE g_originalBytes[5];
	int (WINAPI *g_pOriginalConnect)(SOCKET, const sockaddr*, int) = nullptr;
	int WINAPI HookedConnect(SOCKET s, const sockaddr* name, int namelen) {
	    if (GetCurrentProcessId() == g_targetPid) {
	        WSASetLastError(WSAEACCES);
	        return SOCKET_ERROR;
	    }
	    memset(g_originalBytes,0,sizeof g_originalBytes);
	    // 恢复原始函数
	    DWORD oldProtect;
	    VirtualProtect((LPVOID)g_pOriginalConnect, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	    memcpy((LPVOID)g_pOriginalConnect, g_originalBytes, 5);
	    VirtualProtect((LPVOID)g_pOriginalConnect, 5, oldProtect, &oldProtect);
	    // 调用原始函数
	    int result = g_pOriginalConnect(s, name, namelen);
	    // 重新设置钩子
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
	    // 保存原始字节
	    memcpy(g_originalBytes, (LPVOID)g_pOriginalConnect, 5);
	    // 修改内存保护属性
	    DWORD oldProtect;
	    if (!VirtualProtect((LPVOID)g_pOriginalConnect, 5, PAGE_EXECUTE_READWRITE, &oldProtect))
	        return FALSE;
	    // 写入跳转指令
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

#include <windows.h>
#include <winsock2.h>
#include <iostream>

namespace nwb {
    DWORD target_pid = 0;
    BYTE original_bytes[5];
    int (WINAPI *original_connect)(SOCKET, const sockaddr*, int) = nullptr;

    int WINAPI hooked_connect(SOCKET s, const sockaddr* name, int namelen) {
        if (GetCurrentProcessId() == target_pid) {
            WSASetLastError(WSAEACCES);
            return SOCKET_ERROR;
        }
        
        DWORD old_protect;
        VirtualProtect((LPVOID)original_connect, 5, PAGE_EXECUTE_READWRITE, &old_protect);
        memcpy((LPVOID)original_connect, original_bytes, 5);
        VirtualProtect((LPVOID)original_connect, 5, old_protect, &old_protect);
        
        int result = original_connect(s, name, namelen);
        
        VirtualProtect((LPVOID)original_connect, 5, PAGE_EXECUTE_READWRITE, &old_protect);
        BYTE jmp[5] = {0xE9};
        *(DWORD*)(jmp+1) = (DWORD)hooked_connect - (DWORD)original_connect - 5;
        memcpy((LPVOID)original_connect, jmp, 5);
        VirtualProtect((LPVOID)original_connect, 5, old_protect, &old_protect);
        
        return result;
    }

    bool SetHook(DWORD pid) {
        target_pid = pid;
        HMODULE ws2 = LoadLibrary("ws2_32.dll");
        if (!ws2) return false;
        
        original_connect = (int(WINAPI*)(SOCKET, const sockaddr*, int))GetProcAddress(ws2, "connect");
        if (!original_connect) return false;
        
        memcpy((LPVOID)original_bytes, (LPVOID)original_connect, 5);
        
        DWORD old_protect;
        if (!VirtualProtect((LPVOID)original_connect, 5, PAGE_EXECUTE_READWRITE, &old_protect))
            return false;
            
        BYTE jmp[5] = {0xE9};
        *(DWORD*)(jmp+1) = (DWORD)hooked_connect - (DWORD)original_connect - 5;
        memcpy((LPVOID)original_connect, jmp, 5);
        VirtualProtect((LPVOID)original_connect, 5, old_protect, &old_protect);
        
        return true;
    }

    bool RemoveHook() {
        if (!original_connect) return true;
        
        DWORD old_protect;
        VirtualProtect((LPVOID)original_connect, 5, PAGE_EXECUTE_READWRITE, &old_protect);
        memcpy((LPVOID)original_connect, original_bytes, 5);
        VirtualProtect((LPVOID)original_connect, 5, old_protect, &old_protect);
        
        original_connect = nullptr;
        return true;
    }
    int main_nwb() {
        cout << "Enter PID to block: ";
        DWORD pid;
        cin >> pid;
        if (SetHook(pid)) {
            cout << "Network blocked for PID " << pid << endl;
            cout << "Press enter to unblock...";
            cin.ignore();
            cin.get();
            RemoveHook();
        } else {
            cerr << "Failed to block PID " << pid << endl;
        }
        return 0;
    }
}


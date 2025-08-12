#include <windows.h>
#include <psapi.h>
#include <winuser.h>
#include <atomic>
#include <thread>
#include <string>
#include <bits/stdc++.h>
#include <shellapi.h>
#include <ntdef.h>
#include <mutex>
#include <tlhelp32.h>
#include <shlobj.h>
#include <psapi.h>
#include <ntstatus.h>
#include <Lmcons.h>
#include <winternl.h>
#include <tchar.h>
using namespace std; 
//-lpsapi -lntdll -lgdi32
atomic<bool> g_threadA_active(false);
atomic<bool> g_threadB_active(false);
atomic<bool> g_threadC_active(false);
atomic<bool> g_threadE_active(false);
atomic<bool> g_threadF_active(false);
atomic<bool> g_threadG_active(false);
atomic<bool> g_running(true);
static const string kill_exe = "REDAgent.exe";

vector<HWND> FindWindowsByProcessName(const char* processName) {
    vector<HWND> result;
    DWORD pid = 0;
    PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
    // ��ȡ����ID
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (strcmp(pe32.szExeFile, processName) == 0) {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
    if (pid == 0) return result;
    // ö�ٴ���
    struct EnumData {
        DWORD pid;
        vector<HWND>* handles;
    };
    EnumData data = { pid, &result };
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        EnumData* data = reinterpret_cast<EnumData*>(lParam);
        DWORD processId = 0;
        GetWindowThreadProcessId(hwnd, &processId);
        if (processId == data->pid && IsWindowVisible(hwnd)) {
            data->handles->push_back(hwnd);
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(&data));
    return result;
}
DWORD FindProcessId(const string& processName){
    DWORD processes[1024],cbNeeded;
    if(!EnumProcesses(processes,sizeof(processes),&cbNeeded))return 0;
    DWORD count = cbNeeded / sizeof(DWORD);
    for(DWORD i = 0;i<count;i++){
        if(processes[i]==0) continue;
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE,processes[i]);
        if(hProcess){
            CHAR szProcessName[MAX_PATH] = "";
            if(GetModuleFileNameEx(hProcess,NULL,szProcessName,MAX_PATH)){
                string name(szProcessName);
                if(name.find(processName) != wstring::npos){
                    CloseHandle(hProcess);
                    return processes[i];
                }
            }
            CloseHandle(hProcess);
        }
    }
    return 0;
}
void ClearWindowContent(HWND hwnd, COLORREF bgColor = RGB(255, 255, 255)) {
    //lock_guard<mutex> lock(mtx);
    if (!IsWindow(hwnd)) return;
    HDC hdc = GetDC(hwnd);
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 
        clientRect.right, clientRect.bottom);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    HBRUSH hBrush = CreateSolidBrush(bgColor);
    FillRect(hMemDC, &clientRect, hBrush);
    BitBlt(hdc, 0, 0, 
        clientRect.right, clientRect.bottom,
        hMemDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    DeleteObject(hBrush);
    ReleaseDC(hwnd, hdc);
    RedrawWindow(hwnd, nullptr, nullptr, 
        RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}
// ǿ��ˢ��ָ������
static void ForceRefreshWindow(HWND hwnd) {
    if (!IsWindow(hwnd)) return;
    // ����WM_NCCALCSIZE��Ϣ�ػ�ǿͻ���
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
        SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    // ǿ���ػ�ͻ���
    RedrawWindow(hwnd, nullptr, nullptr, 
        RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}
// ����ˢ�´���
static void RefreshWindows(const vector<HWND>& windows) {
    for (HWND hwnd : windows) {
        //ForceRefreshWindow(hwnd);
        ClearWindowContent(hwnd);
    }
}
/*void ToggleWindowMode(HWND hwnd, bool windowed) {
    static WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
    DWORD style = GetWindowLong(hwnd, GWL_STYLE);
    
    if (windowed) {
        // ���ڻ�ģʽ����ӱ��������߿�ʹ�С������
        SetWindowLong(hwnd, GWL_STYLE, 
            style | WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_THICKFRAME);
        SetWindowPlacement(hwnd, &wpPrev);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    } else {
        // ȫ��ģʽ���Ƴ��������ͱ߿�
        if (GetWindowPlacement(hwnd, &wpPrev)) {
            SetWindowLong(hwnd, GWL_STYLE, 
                style & ~(WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_THICKFRAME));
            SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
}*/
void ToggleWindowMode(HWND hwnd, bool windowed) {
    static std::unordered_map<HWND, std::pair<bool, bool>> windowStates; // ��Ϊ�洢״̬���Ƿ��״μ��
    //static std::mutex mtx;
    
    //std::lock_guard<std::mutex> lock(mtx);
    
    if (!IsWindow(hwnd)) return;
    
    // ���״̬�Ƿ��Ѹı�
    if (windowStates.find(hwnd) != windowStates.end() && 
        windowStates[hwnd].first == windowed) {
        return;
    }
    
    DWORD style = GetWindowLong(hwnd, GWL_STYLE);
    
    if (windowed) {
        SetWindowLong(hwnd, GWL_STYLE, 
            style | WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_THICKFRAME);
        
        // �״μ��ʱ�������ڴ�С
        if (windowStates.find(hwnd) == windowStates.end() || !windowStates[hwnd].second) {
            RECT rect;
            GetWindowRect(hwnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            SetWindowPos(hwnd, NULL, 0, 0, width + 12, height + 30,
                SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
            windowStates[hwnd].second = true; // ���Ϊ�ѵ���
        }
    } else {
        SetWindowLong(hwnd, GWL_STYLE,
            style & ~(WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_THICKFRAME));
    }
    
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
        SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    
    windowStates[hwnd].first = windowed;
}
void ThreadA_Function() {
    while (g_running) {
        vector<HWND> v = FindWindowsByProcessName(kill_exe.c_str());
        for (HWND hwnd : v) {
            if (hwnd) {
                ToggleWindowMode(hwnd, g_threadA_active);
            }
        }
        RefreshWindows(v);
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

void ThreadB_Function(){
    while(g_running){
        if(g_threadB_active){
            DWORD pid = FindProcessId(kill_exe.c_str());
            if(pid != 0){
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,pid);
                if(hProcess){
                    TerminateProcess(hProcess,0);
                    CloseHandle(hProcess);
                }
            }
        }
        this_thread::sleep_for(chrono::milliseconds(700));
    }
}
int closewindowpro(HWND hwnd,int processId){
    int r = SendMessage(hwnd, WM_CLOSE, 0, 0); // ����WM_CLOSE��Ϣ�رմ���
    //cout<<"closewindowpro:mode 1 return "<<r<<"\n";
	r = DestroyWindow(hwnd);
    //cout<<"closewindowpro:mode 2 return "<<r<<"\n";
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId); // ��ȡ���̾��
    if (hProcess != NULL) {
        NTSTATUS status = NtClose(hProcess); // �رվ��
        r = !NT_SUCCESS(status);
    }
    //cout<<"closewindowpro:mode 3 return "<<r<<"\n";
    //cout<<"closewindowpro:ended\n";
}
int getpidbyhwnd(HWND hwnd){
	DWORD pid;
    DWORD threadId = GetWindowThreadProcessId(hwnd, &pid);
    if (threadId == 0) {
        //cout << "getpidbyhwnd:error at getting PID" << endl;
        return 65535;
    }
    return pid;
}
void ThreadC_Function(){
    while(g_running){
        if(g_threadC_active){
        	vector<HWND> v = FindWindowsByProcessName(kill_exe.c_str());
            for(HWND hwnd:v){
	            if(hwnd){
	                PostMessage(hwnd,WM_CLOSE,0,0);
	                closewindowpro(hwnd,getpidbyhwnd(hwnd));
	            }
			}
        }
        this_thread::sleep_for(chrono::milliseconds(1300));
    }
}
void ThreadD_Function(){
	while(g_running){
		if((GetAsyncKeyState(VK_ESCAPE) & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000)){
            g_threadA_active = !g_threadA_active;
            cout<<"Ctrl+Escape pressed ������ϼ�\nWindow mode ���ڻ�ģʽ: " <<(g_threadA_active ? "ON" : "OFF")<<endl;
            while((GetAsyncKeyState(VK_ESCAPE) & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) this_thread::sleep_for(chrono::milliseconds(250));
		}
	}
}
void SetConsoleTopMost(bool topmost) {
    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
        SetWindowPos(hwnd, 
                   topmost ? HWND_TOPMOST : HWND_NOTOPMOST,
                   0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE);
    }
}
void ThreadE_Function() {
    while (g_running) {
        if (g_threadE_active) {
            SetConsoleTopMost(true);
        } else {
            SetConsoleTopMost(false);
        }
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_CLOSE_EVENT || 
        signal == CTRL_LOGOFF_EVENT || 
        signal == CTRL_SHUTDOWN_EVENT) {
        return TRUE; // ��ֹ�ر�
    }
    return FALSE;
}
void ThreadF_Function() {
	while (g_running) {
	    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	    while (g_threadF_active) {
	        // ������ֹϵͳ�ر�
	        Sleep(4000);
	    }
	    SetConsoleCtrlHandler(ConsoleHandler, FALSE);
	    Sleep(1000);
	}
}
/*LRESULT CALLBACK LowLevelKeyboardProc(int nCode,WPARAM wParam,LPARAM lParam){
    if(nCode==HC_ACTION){
        KBDLLHOOKSTRUCT* p =(KBDLLHOOKSTRUCT*)lParam;
        if(wParam==WM_KEYDOWN && p->vkCode==VK_ESCAPE &&
            GetAsyncKeyState(VK_CONTROL) & 0x8000){
            g_threadA_active = !g_threadA_active;
            cout<<"CTRL+ESCAPE pressed. Toggle fullscreen mode: "
                      <<(g_threadA_active ? "ON" : "OFF")<<endl;
        }
    }
    return CallNextHookEx(NULL,nCode,wParam,lParam);
}*/
string GetCurrentUsername() {
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    if (GetUserNameA(username, &username_len)) {
        return string(username);
    } else {
        DWORD error = GetLastError();
        cerr << "��ȡ�û���ʧ�ܣ��������: " << error << endl;
        return "";
    }
}
typedef NTSTATUS(NTAPI* NtQuerySystemInformation_t)(
    SYSTEM_INFORMATION_CLASS,
    PVOID,
    ULONG,
    PULONG);
void ThreadG_Function() {
	while (g_running) {
        if (g_threadE_active) {
        	HMODULE ntdll = GetModuleHandle("ntdll.dll");
		    NtQuerySystemInformation_t NtQuerySystemInformation = 
		        (NtQuerySystemInformation_t)GetProcAddress(ntdll, "NtQuerySystemInformation");
		    while (g_threadG_active) {
		        DWORD processes[1024], cbNeeded;
		        if (!EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
		            Sleep(1000);
		            continue;
		        }
		        DWORD cProcesses = cbNeeded / sizeof(DWORD);
		        for (unsigned int i = 0; i < cProcesses; i++) {
		            DWORD pid = processes[i];
		            if (pid == 0 || pid == GetCurrentProcessId()) continue;
		            HANDLE hProcess = OpenProcess(
		                PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		                FALSE, pid);
		            if (!hProcess) continue;
		            HMODULE hMod;
		            DWORD cbNeededMod;
		            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededMod)) {
		                CHAR szModName[MAX_PATH];
		                if (GetModuleFileNameEx(hProcess, hMod, szModName, MAX_PATH)) {
		                    if (((string)szModName).find(".exe")!=string::npos) {
		                        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		                        if (hSnapshot != INVALID_HANDLE_VALUE) {
		                            THREADENTRY32 te32;
		                            te32.dwSize = sizeof(THREADENTRY32);
		                            if (Thread32First(hSnapshot, &te32)) {
		                                do {
		                                    if (te32.th32OwnerProcessID == pid) {
		                                        HANDLE hThread = OpenThread(
		                                            THREAD_QUERY_INFORMATION,
		                                            FALSE,
		                                            te32.th32ThreadID);
		                                        if (hThread) {
		                                            if (GetThreadDesktop(te32.th32ThreadID) == GetThreadDesktop(GetCurrentThreadId())) {
		                                                DWORD_PTR hook = 0;
		                                                if (GetGUIThreadInfo(te32.th32ThreadID, nullptr)) {
		                                                    UnhookWindowsHookEx((HHOOK)WH_KEYBOARD);
		                                                    UnhookWindowsHookEx((HHOOK)WH_KEYBOARD_LL);
		                                                }
		                                            }
		                                            CloseHandle(hThread);
		                                        }
		                                    }
		                                } while (Thread32Next(hSnapshot, &te32));
		                            }
		                            CloseHandle(hSnapshot);
		                        }
		                    }
		                }
		            }
		            CloseHandle(hProcess);
		        }
		        //Sleep(500);
		    }
        }
        this_thread::sleep_for(chrono::milliseconds(2000));
    }
    
}
string getopen(){
	stringstream ss("");
	if(g_threadA_active) ss<<"���ڻ� ";
	if(g_threadB_active) ss<<"�Զ���ֹ ";
	if(g_threadC_active) ss<<"�رմ��� ";
	if(g_threadE_active) ss<<"����̨�ö� ";
	if(g_threadF_active) ss<<"���ػ� ";
	if(g_threadG_active) ss<<"������� ";
	if(ss.str()=="") return "(����)";
	else return ss.str();
}
void SetConsoleWindowSize(int width, int height) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {0, 0, static_cast<SHORT>(width-1), static_cast<SHORT>(height-1)};
    SetConsoleWindowInfo(hStdout, TRUE, &windowSize);
    
    COORD bufferSize = {static_cast<SHORT>(width), static_cast<SHORT>(height)};
    SetConsoleScreenBufferSize(hStdout, bufferSize);
}
void hideclose(){
	HWND hwnd = GetConsoleWindow();
    if (hwnd != NULL) {
        HMENU hMenu = GetSystemMenu(hwnd, FALSE);
        if (hMenu != NULL) {
            DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
            DrawMenuBar(hwnd);
        }
    	LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
        style &= ~WS_THICKFRAME;  // �Ƴ��ɵ����߿���ʽ
        style &= ~WS_MAXIMIZEBOX; // �Ƴ���󻯰�ť
        SetWindowLongPtr(hwnd, GWL_STYLE, style);
        // ǿ���ػ洰��
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0, 
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
}
string exedir;
void getexedir(){
	char path[MAX_PATH];
    DWORD len = GetModuleFileName(NULL, path, MAX_PATH);
    string t = string(path, len);
    exedir = t.substr(0,t.find_last_of("\\"))+"\\";
    cout<<"getexedir:exedir is "<<exedir<<endl;
}
void seticon(string icon,HWND hwnd = GetConsoleWindow()) {
	HICON hIcon = (HICON)LoadImage(NULL,icon.c_str(),IMAGE_ICON,0,0,LR_LOADFROMFILE | LR_DEFAULTSIZE);
    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL2, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }
    else{
    	cout<<"seticon:hicon error\n";
    	cout<<"seticon:hicon path="<<icon<<"\n";
	}
}
int main(){
	SetConsoleTitle("�ƽ��ַ����缯ѵӪ��Ļ����");
	SetConsoleCP(CP_ACP);
	SetConsoleWindowSize(60,20);
	cout<<"�ƽ��ַ����缯ѵӪ��Ļ����-REDAgentController\n��ǰ���й��ܾ�Ϊ�ر�״̬\n";
	getexedir();
	seticon(exedir+"icon.ico");
	cout<<"�û���:"<<GetCurrentUsername()<<endl;
    //HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyboardProc,NULL,0);
	cout<<"Open all threads\n";
    thread threadA(ThreadA_Function);
    thread threadB(ThreadB_Function);
    thread threadC(ThreadC_Function);
    thread threadD(ThreadD_Function);
    thread threadE(ThreadE_Function);
    thread threadF(ThreadF_Function);
    thread threadG(ThreadG_Function);
    cout<<"����:hty\n";
    hideclose(); 
    int m_brk = 0;
    while(!m_brk){
        cout<<"Menu �˵�:\n"
                 <<"1. Toggle window mode(CTRL+ESCAPE) �л����ڻ�ģʽ\n"
                 <<"2. Toggle force terminate process �л��Զ���ֹ����\n"
                 <<"3. Toggle close windows �л��Զ��رմ���\n"
                 <<"4. Open webpage ����ҳ\n"
                 <<"5. Exit �˳�\n"
                 <<"6. Console top �л�����̨�ö�\n"
                 <<"7. No shutdown �л����ػ�\n"
                 <<"8. No key hook �л��������\n"
                 <<"Opened �ѿ���: "<<getopen()<<"\n"
                 <<"Enter choice ����ѡ��:";
        int choice;
        cin>>choice;
        switch(choice){
        	case 0:
        		break;
            case 1:
                g_threadA_active = !g_threadA_active;
                cout<<"Window mode ���ڻ�ģʽ:" <<(g_threadA_active ? "ON" : "OFF")<<endl;
                break;
            case 2:
                g_threadB_active = !g_threadB_active;
                cout<<"Force terminate �Զ���ֹ:" <<(g_threadB_active ? "ON" : "OFF")<<endl;
                break;
            case 3:
                g_threadC_active = !g_threadC_active;
                cout<<"Close windows �رմ���:" <<(g_threadC_active ? "ON" : "OFF")<<endl;
                break;
            case 4:
                system("start https://vjrcuidy.us-west-1.clawcloudrun.com/");
                cout<<"����:hty\nvjrcuidy.us-west-1.clawcloudrun.com\nhttp://123.60.188.246/group/1025\n";
                break;
            case 5:
                g_running = false;
                m_brk = 1;
                cout<<"����:hty\nvjrcuidy.us-west-1.clawcloudrun.com\nhttp://123.60.188.246/group/1025\n";
                break;
            case 6:
                g_threadE_active = !g_threadE_active;
                cout<<"Console top ����̨�ö�:" <<(g_threadE_active ? "ON" : "OFF")<<endl;
                break;
            case 7:
                g_threadF_active = !g_threadF_active;
                cout<<"No shutdown ���ػ�:" <<(g_threadF_active ? "ON" : "OFF")<<endl;
                break;
            case 8:
                g_threadG_active = !g_threadG_active;
                cout<<"No key hook �л��������:" <<(g_threadG_active ? "ON" : "OFF")<<endl;
                break;
            default:
                cout<<"Invalid choice ������ѡ��\n";
        }
    }
	cout<<"Waiting for all threads join\n";
    threadA.join();
    threadB.join();
    threadC.join();
    threadD.join();
    threadE.join();
    threadF.join();
    threadG.join();
	cout<<"Close main thread\n"; 
    //UnhookWindowsHookEx(hHook);
    return 0;
}

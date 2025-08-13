#include <winsock2.h>
#include <windows.h>
#include <psapi.h>
#include <winuser.h>
#include <atomic>
#include <thread>
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <bits/stdc++.h>
#include <shellapi.h>
#include <ntdef.h>
#include <mutex>
#include <tlhelp32.h>
#include <shlobj.h>
#include <psapi.h>
#include <ntstatus.h>
#include <Lmcons.h>
#include "NetBlocker.cpp"
#include <winternl.h>
#include <tchar.h>
using namespace std; 
//-lpsapi -lntdll -lgdi32 -std=c++14 -O2 -s -lws2_32
//32bit mode
atomic<bool> g_threadA_active(false);
atomic<bool> g_threadB_active(false);
atomic<bool> g_threadC_active(false);
atomic<bool> g_threadE_active(false);
atomic<bool> g_threadF_active(false);
atomic<bool> g_threadG_active(false);
atomic<bool> g_threadH_active(false);
atomic<bool> g_kzjc(false);//测试：控制进程运行 
atomic<bool> g_kzjcwl(false);//测试：控制进程网络 
atomic<bool> g_running(true);
static const string kill_exe = "REDAgent.exe";
struct EnumData {
    DWORD pid;
    vector<HWND>* handles;
};
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
vector<HWND> FindWindowsByProcessName(const char* processName) {
    vector<HWND> result;
    DWORD pid = 0;
    PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
    // 获取进程ID
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
    // 枚举窗口
    EnumData data = { pid, &result };
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&data));
    return result;
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    EnumData* data = reinterpret_cast<EnumData*>(lParam);
    DWORD processId = 0;
    GetWindowThreadProcessId(hwnd, &processId);
    if (processId == data->pid && IsWindowVisible(hwnd)) {
        data->handles->push_back(hwnd);
    }
    return TRUE;
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
// 强制刷新指定窗口
static void ForceRefreshWindow(HWND hwnd) {
    if (!IsWindow(hwnd)) return;
    // 触发WM_NCCALCSIZE消息重绘非客户区
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
        SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    // 强制重绘客户区
    RedrawWindow(hwnd, nullptr, nullptr, 
        RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}
// 批量刷新窗口
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
        // 窗口化模式：添加标题栏、边框和大小调整栏
        SetWindowLong(hwnd, GWL_STYLE, 
            style | WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_THICKFRAME);
        SetWindowPlacement(hwnd, &wpPrev);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    } else {
        // 全屏模式：移除标题栏和边框
        if (GetWindowPlacement(hwnd, &wpPrev)) {
            SetWindowLong(hwnd, GWL_STYLE, 
                style & ~(WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_THICKFRAME));
            SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
}*/
void ToggleWindowMode(HWND hwnd, bool windowed) {
    static unordered_map<HWND, pair<bool, bool>> windowStates; // 改为存储状态和是否首次检测
    //static mutex mtx;
    
    //lock_guard<mutex> lock(mtx);
    
    if (!IsWindow(hwnd)) return;
    
    // 检查状态是否已改变
    if (windowStates.find(hwnd) != windowStates.end() && 
        windowStates[hwnd].first == windowed) {
        return;
    }
    
    DWORD style = GetWindowLong(hwnd, GWL_STYLE);
    
    if (windowed) {
        SetWindowLong(hwnd, GWL_STYLE, 
            style | WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_THICKFRAME);
        
        // 首次检测时调整窗口大小
        if (windowStates.find(hwnd) == windowStates.end() || !windowStates[hwnd].second) {
            RECT rect;
            GetWindowRect(hwnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            SetWindowPos(hwnd, NULL, 0, 0, width + 12, height + 30,
                SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
            windowStates[hwnd].second = true; // 标记为已调整
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
    int r = SendMessage(hwnd, WM_CLOSE, 0, 0); // 发送WM_CLOSE消息关闭窗口
    //cout<<"closewindowpro:mode 1 return "<<r<<"\n";
	r = DestroyWindow(hwnd);
    //cout<<"closewindowpro:mode 2 return "<<r<<"\n";
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId); // 获取进程句柄
    if (hProcess != NULL) {
        NTSTATUS status = NtClose(hProcess); // 关闭句柄
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
            cout<<"Ctrl+Escape pressed 按下组合键\nWindow mode 窗口化模式: " <<(g_threadA_active ? "ON" : "OFF")<<endl;
            while((GetAsyncKeyState(VK_ESCAPE) & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) this_thread::sleep_for(chrono::milliseconds(250));
		}
	}
}
void SetConsoleTopMost(bool topmost) {
    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
    	if(topmost){
    		SetWindowPos(hwnd, 
	                   HWND_TOPMOST,
	                   0, 0, 0, 0,
	                   SWP_NOMOVE | SWP_NOSIZE);
	    	BringWindowToTop(hwnd);
	    	SetForegroundWindow(hwnd);
		}
        else{
	        SetWindowPos(hwnd, 
	                   HWND_NOTOPMOST,
	                   0, 0, 0, 0,
	                   SWP_NOMOVE | SWP_NOSIZE);	
		}
    }//顶层 
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
        return TRUE; // 阻止关闭
    }
    return FALSE;
}
void ThreadF_Function() {
	while (g_running) {
	    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	    while (g_threadF_active&&g_running) {
	        // 持续阻止系统关闭
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
        cerr << "获取用户名失败，错误代码: " << error << endl;
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
        if (g_threadG_active) {
        	HMODULE ntdll = GetModuleHandle("ntdll.dll");
		    NtQuerySystemInformation_t NtQuerySystemInformation = 
		        (NtQuerySystemInformation_t)GetProcAddress(ntdll, "NtQuerySystemInformation");
		    while (g_threadG_active&&g_running) {
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
		                                        HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION,FALSE,te32.th32ThreadID);
		                                        if (hThread) {
		                                            if (GetThreadDesktop(te32.th32ThreadID) == GetThreadDesktop(GetCurrentThreadId())) {
		                                                DWORD_PTR hook = 0;
		                                                if (GetGUIThreadInfo(te32.th32ThreadID, nullptr)) {
		                                                    UnhookWindowsHookEx((HHOOK)WH_KEYBOARD);
		                                                    UnhookWindowsHookEx((HHOOK)WH_KEYBOARD_LL);
		                                                    //UnhookWindowsHookEx((HHOOK)WH_MOUSE);
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
		    }
        }
		else Sleep(2500);
        this_thread::sleep_for(chrono::milliseconds(2000));
    }
}
int newmsg(string text,int sx=0,int sy=0,HDC uhdc = NULL){
	HDC hdc = (uhdc==NULL ? GetDC(NULL) : uhdc);
	TextOutA(hdc, sx, sy, TEXT((LPCSTR)text.c_str()), text.length());
	if(uhdc==NULL)ReleaseDC(NULL, hdc);
}
void setfont(HDC hdc, const string& fontName){
    // 创建新字体
    LOGFONTA lf = {0};
    strncpy(lf.lfFaceName, fontName.c_str(), LF_FACESIZE);
    lf.lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT hFont = CreateFontIndirectA(&lf);
    // 应用新字体
    SelectObject(hdc, hFont);
}

string getopen(){
	stringstream ss("");
	if(g_threadA_active) ss<<"窗口化 ";
	if(g_threadB_active) ss<<"自动终止 ";
	if(g_threadC_active) ss<<"关闭窗口 ";
	if(g_threadE_active) ss<<"控制台置顶 ";
	if(g_threadF_active) ss<<"反关机 ";
	if(g_threadG_active) ss<<"鼠标键盘 ";
	if(g_threadH_active) ss<<"置顶信息 ";
	if(g_kzjc) ss<<"映像劫持 ";
	if(g_kzjcwl) ss<<"控制进程网络 ";
	if(ss.str()=="") return "(暂无)";
	else return ss.str();
}
string time_to_string(time_t timestamp) {
    struct tm tm_info;
    localtime_r(&timestamp, &tm_info);
    
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_info);
    return string(buffer);
}
string chrono_to_string(chrono::system_clock::time_point tp) {
    auto timestamp = chrono::system_clock::to_time_t(tp);
    return time_to_string(timestamp);
}
void ThreadH_Function(){
	int sx = GetSystemMetrics(SM_CXSCREEN);
    int sy = GetSystemMetrics(SM_CYSCREEN);
	HDC hdc = GetDC(NULL);
	while(g_running){
		if (g_threadH_active) {
			setfont(hdc,"Consolas");
			newmsg("REDAgentController Working",0,0,hdc);
			newmsg("Opened:"+getopen(),0,20,hdc);
			newmsg("Time:"+time_to_string(time(0)),0,40,hdc);
		}
		else{
			Sleep(1000);
		}
		Sleep(1000);
	}
	ReleaseDC(NULL, hdc);
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
        	EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
            DrawMenuBar(hwnd);
        }
    	LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
        style &= ~WS_THICKFRAME;  // 移除可调整边框样式
        style &= ~WS_MAXIMIZEBOX; // 移除最大化按钮
        SetWindowLongPtr(hwnd, GWL_STYLE, style);
        // 强制重绘窗口
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
int safetoi(string s){
	int w = 1,r = 0;
	for(char c : s){
		if(c=='-') w = -1;
		else if(c>='0'&&c<='9'){
			r*=10;
			r+=c-'0'; 
		}
		else{
			return -1145;
		}
	}
	return r;
}
void ThreadKZ_Function(int op){
	if(op==1){
		string ops = "reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\"+kill_exe+"\" /f";
		int r = system(ops.c_str());
	}
	else if(op==2){
		string ops = "reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\"+kill_exe+
			"\" /v Debugger /t REG_SZ /d \"%systemroot%\\System32\\alg.exe\" /f";
		int r = system(ops.c_str());
		system(("taskkill /f /im "+kill_exe).c_str());
	}
	else if(op==3){
		int pid = FindProcessId(kill_exe);
		cout<<"ThreadKZ_Function:pid="<<pid<<endl;
		int r = nwb::SetHook(pid);
		cout<<"NetBlocker:state="<<r<<endl;
	}
	else if(op==4){
		int pid = FindProcessId(kill_exe);
		cout<<"ThreadKZ_Function:pid="<<pid<<endl;
		int r = nwb::RemoveHook();
		cout<<"NetBlocker:state="<<r<<endl;
	}
}
int main(){
	SetConsoleTitle("破解字符世界集训营屏幕锁定");
	SetConsoleCP(CP_ACP);
	//SetConsoleWindowSize(60,20);
	cout<<"破解字符世界集训营屏幕锁定-REDAgentController\n当前所有功能均为关闭状态\n";
	getexedir();
	seticon(exedir+"icon.ico");
	cout<<"用户名:"<<GetCurrentUsername()<<endl;
    //HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyboardProc,NULL,0);
	cout<<"Open all threads\n";
    thread threadA(ThreadA_Function);
    thread threadB(ThreadB_Function);
    thread threadC(ThreadC_Function);
    thread threadD(ThreadD_Function);
    thread threadE(ThreadE_Function);
    thread threadF(ThreadF_Function);
    thread threadG(ThreadG_Function);
    thread threadH(ThreadH_Function);
    cout<<"作者:hty\n项目:github.com/hetianyu313/REDAgentController\n";
    hideclose(); 
    int m_brk = 0;
    while(!m_brk){
        cout<<"Menu 菜单:\n"
                 <<"1. Toggle window mode(CTRL+ESCAPE) 切换窗口化模式\n"
                 <<"2. Toggle force terminate process 切换自动终止进程\n"
                 <<"3. Toggle close windows 切换自动关闭窗口\n"
                 <<"4. Open webpage 打开网页\n"
                 <<"5. Exit 退出\n"
                 <<"6. Console top 切换控制台置顶\n"
                 <<"7. No shutdown 切换反关机\n"
                 <<"8. No any hook 切换允许鼠标键盘\n"
                 <<"9. Top Message 切换置顶信息\n"
                 <<"10. Toggle control work 切换映像劫持\n"
                 <<"11. Toggle control network 切换控制进程网络\n"
                 <<"Opened 已开启: "<<getopen()<<"\n"
                 <<"Enter choice 输入选择:";
        string s;
        getline(cin,s);
        int choice = safetoi(s);
        thread kz;
        switch(choice){
        	case 0:
        		break;
            case 1:
                g_threadA_active = !g_threadA_active;
                cout<<"Window mode 窗口化模式:" <<(g_threadA_active ? "ON" : "OFF")<<endl;
                break;
            case 2:
                g_threadB_active = !g_threadB_active;
                cout<<"Force terminate 自动终止:" <<(g_threadB_active ? "ON" : "OFF")<<endl;
                break;
            case 3:
                g_threadC_active = !g_threadC_active;
                cout<<"Close windows 关闭窗口:" <<(g_threadC_active ? "ON" : "OFF")<<endl;
                break;
            case 4:
                system("start https://vjrcuidy.us-west-1.clawcloudrun.com/");
                cout<<"作者:hty\nvjrcuidy.us-west-1.clawcloudrun.com\nhttp://123.60.188.246/group/1025\n";
                break;
            case 5:
                g_running = false;
                m_brk = 1;
                cout<<"作者:hty\nvjrcuidy.us-west-1.clawcloudrun.com\nhttp://123.60.188.246/group/1025\n";
                break;
            case 6:
                g_threadE_active = !g_threadE_active;
                cout<<"Console top 控制台置顶:" <<(g_threadE_active ? "ON" : "OFF")<<endl;
                break;
            case 7:
                g_threadF_active = !g_threadF_active;
                cout<<"No shutdown 反关机:" <<(g_threadF_active ? "ON" : "OFF")<<endl;
                break;
            case 8:
                g_threadG_active = !g_threadG_active;
                cout<<"No any hook 允许鼠标键盘:" <<(g_threadG_active ? "ON" : "OFF")<<endl;
                break;
            case 9:
                g_threadH_active = !g_threadH_active;
                cout<<"Top Message 置顶信息:" <<(g_threadH_active ? "ON" : "OFF")<<endl;
                break;
            case 10:
                g_kzjc = !g_kzjc;
                cout<<"Control 映像劫持:" <<(g_kzjc ? "ON" : "OFF")<<"\n";
                Sleep(50);
                kz = thread(ThreadKZ_Function,g_kzjc?2:1);
				kz.join();
                break;
            case 11:
                g_kzjcwl = !g_kzjcwl;
                cout<<"Network 控制进程网络:" <<(g_kzjcwl ? "ON" : "OFF")<<"\n";
                Sleep(50);
                kz = thread(ThreadKZ_Function,g_kzjcwl?4:3);
				kz.join();
                break;
            default:
                cout<<"Invalid choice 非正常选择\n";
        }
    }
	cout<<"Waiting for all threads join\n";
    threadA.join();
	cout<<"Waiting for thread 1 join\n";
    threadB.join();
	cout<<"Waiting for thread 2 join\n";
    threadC.join();
	cout<<"Waiting for thread 3 join\n";
    threadD.join();
	cout<<"Waiting for thread 4 join\n";
    threadE.join();
	cout<<"Waiting for thread 5 join\n";
    threadF.join();
	cout<<"Waiting for thread 6 join\n";
    threadG.join();
	cout<<"Waiting for thread 7 join\n";
    threadH.join();
	cout<<"Waiting for thread 8 join\n";
	cout<<"Close main thread\n"; 
    //UnhookWindowsHookEx(hHook);
    return 0;
}

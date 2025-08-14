#define amyzjvx1rg BOOL
#define whw8fvasp4 BYTE
#define w539bauaeu DWORD
#define trsd2xo21w FALSE
#define qcto86r2c5 GetCurrentProcessId
#define kkjf46lx71 GetProcAddress
#define ig2eljocgb HMODULE
#define nksceenzfw HookedConnect
#define z8g5sga4w6 LPVOID
#define f29rz37sb8 LoadLibrary
#define jupczpb5f6 PAGE_EXECUTE_READWRITE
#define esokfycy4u RemoveHook
#define ltdm62qezz SOCKET
#define ax07vebnus SOCKET_ERROR
#define zmh2qbl05j SetHook
#define fd3yop0zyv TRUE
#define vwmcciwjgt VirtualProtect
#define x1espdvped WINAPI
#define kmni2j2qo1 WSAEACCES
#define cc57o847qa WSASetLastError
#define qwq9v9xkhm bool
#define x7w5p9wt2i cerr
#define gysc6xyfv2 cin
#define e5i3twlwkh const
#define mp4gt1yhwk cout
#define zvwnssomx8 else
#define b5bou6u2mx endl
#define j3axw6uwr1 false
#define t0nb6tc3ss g_originalBytes
#define h1hqiyugmt g_pOriginalConnect
#define n5qf1jm40w g_targetPid
#define mfpyd2824g get
#define pyrn3y8sjw getchar
#define lr7667gdmc hWs2
#define yplxcpbs3f hooked_connect
#define y2fd2rulmw if
#define i6d4tkre8o ignore
#define rrc49xghzi int
#define q2gvyjys37 jmp
#define ifhyks2hjr main_nwb
#define xhger04dmd memcpy
#define ofzv15nweo memset
#define cjondz2x6u name
#define raat6i3pjz namelen
#define knzuq5esra namespace
#define h29z8iitq9 nullptr
#define zlwlhopgnk nwb
#define v8mqhtqhyq nwb_old
#define icp647z0xo oldProtect
#define dtd5s2mcof old_protect
#define me9a95fsmv original_bytes
#define lpdr9iwcs2 original_connect
#define ax6dp218ub pid
#define t7vws7twwt printf
#define u3pl2o11qz result
#define t3s0mbwtav return
#define jdy3aqstlk s
#define hqg3xffbuh scanf
#define is8p552i94 sizeof
#define kz9faqa76q sockaddr
#define w4pdxap7a7 std
#define eom2xanqit target_pid
#define v89uab2nw4 true
#define q72nw2io55 using
#define jo0lexe37h ws2
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
q72nw2io55 knzuq5esra w4pdxap7a7;
knzuq5esra v8mqhtqhyq{
w539bauaeu n5qf1jm40w = 0;
whw8fvasp4 t0nb6tc3ss[5];
rrc49xghzi (x1espdvped *h1hqiyugmt)(ltdm62qezz, e5i3twlwkh kz9faqa76q*, rrc49xghzi) = h29z8iitq9;
rrc49xghzi x1espdvped nksceenzfw(ltdm62qezz jdy3aqstlk, e5i3twlwkh kz9faqa76q* cjondz2x6u, rrc49xghzi raat6i3pjz) {
 y2fd2rulmw (qcto86r2c5() == n5qf1jm40w) {
 cc57o847qa(kmni2j2qo1);
 t3s0mbwtav ax07vebnus;
 }
 ofzv15nweo(t0nb6tc3ss,0,is8p552i94 t0nb6tc3ss);
 // 恢复原始函数
 w539bauaeu icp647z0xo;
 vwmcciwjgt((z8g5sga4w6)h1hqiyugmt, 5, jupczpb5f6, &icp647z0xo);
 xhger04dmd((z8g5sga4w6)h1hqiyugmt, t0nb6tc3ss, 5);
 vwmcciwjgt((z8g5sga4w6)h1hqiyugmt, 5, icp647z0xo, &icp647z0xo);
 // 调用原始函数
 rrc49xghzi u3pl2o11qz = h1hqiyugmt(jdy3aqstlk, cjondz2x6u, raat6i3pjz);
 // 重新设置钩子
 vwmcciwjgt((z8g5sga4w6)h1hqiyugmt, 5, jupczpb5f6, &icp647z0xo);
 whw8fvasp4 q2gvyjys37[5] = { 0xE9 };
 *(w539bauaeu*)(q2gvyjys37 + 1) = (w539bauaeu)nksceenzfw - (w539bauaeu)h1hqiyugmt - 5;
 xhger04dmd((z8g5sga4w6)h1hqiyugmt, q2gvyjys37, 5);
 vwmcciwjgt((z8g5sga4w6)h1hqiyugmt, 5, icp647z0xo, &icp647z0xo);
 t3s0mbwtav u3pl2o11qz;
}
amyzjvx1rg zmh2qbl05j(w539bauaeu ax6dp218ub) {
 n5qf1jm40w = ax6dp218ub;
 ig2eljocgb lr7667gdmc = f29rz37sb8("ws2_32.dll");
 y2fd2rulmw (!lr7667gdmc) t3s0mbwtav trsd2xo21w;
 h1hqiyugmt = (rrc49xghzi(x1espdvped*)(ltdm62qezz, e5i3twlwkh kz9faqa76q*, rrc49xghzi))kkjf46lx71(lr7667gdmc, "connect");
 y2fd2rulmw (!h1hqiyugmt) t3s0mbwtav trsd2xo21w;
 // 保存原始字节
 xhger04dmd(t0nb6tc3ss, (z8g5sga4w6)h1hqiyugmt, 5);
 // 修改内存保护属性
 w539bauaeu icp647z0xo;
 y2fd2rulmw (!vwmcciwjgt((z8g5sga4w6)h1hqiyugmt, 5, jupczpb5f6, &icp647z0xo))
 t3s0mbwtav trsd2xo21w;
 // 写入跳转指令
 whw8fvasp4 q2gvyjys37[5] = { 0xE9 };
 *(w539bauaeu*)(q2gvyjys37 + 1) = (w539bauaeu)nksceenzfw - (w539bauaeu)h1hqiyugmt - 5;
 xhger04dmd((z8g5sga4w6)h1hqiyugmt, q2gvyjys37, 5);
 vwmcciwjgt((z8g5sga4w6)h1hqiyugmt, 5, icp647z0xo, &icp647z0xo);
 t3s0mbwtav fd3yop0zyv;
}
amyzjvx1rg esokfycy4u() {
 y2fd2rulmw (!h1hqiyugmt) t3s0mbwtav fd3yop0zyv;
 w539bauaeu icp647z0xo;
 vwmcciwjgt((z8g5sga4w6)h1hqiyugmt, 5, jupczpb5f6, &icp647z0xo);
 xhger04dmd((z8g5sga4w6)h1hqiyugmt, t0nb6tc3ss, 5);
 vwmcciwjgt((z8g5sga4w6)h1hqiyugmt, 5, icp647z0xo, &icp647z0xo);
 h1hqiyugmt = h29z8iitq9;
 t3s0mbwtav fd3yop0zyv;
}
rrc49xghzi ifhyks2hjr() {
 w539bauaeu ax6dp218ub;
 t7vws7twwt("Enter PID to block: ");
 hqg3xffbuh("%d", &ax6dp218ub);
 y2fd2rulmw (zmh2qbl05j(ax6dp218ub)) {
 t7vws7twwt("Network access blocked for PID %d\n", ax6dp218ub);
 t7vws7twwt("Press any key to unblock...\n");
 pyrn3y8sjw();
 esokfycy4u();
 } zvwnssomx8 {
 t7vws7twwt("Failed to block PID %d\n", ax6dp218ub);
 }
 t3s0mbwtav 0;
}
}

#include <windows.h>
#include <winsock2.h>
#include <iostream>

knzuq5esra zlwlhopgnk {
w539bauaeu eom2xanqit = 0;
whw8fvasp4 me9a95fsmv[5];
rrc49xghzi (x1espdvped *lpdr9iwcs2)(ltdm62qezz, e5i3twlwkh kz9faqa76q*, rrc49xghzi) = h29z8iitq9;

rrc49xghzi x1espdvped yplxcpbs3f(ltdm62qezz jdy3aqstlk, e5i3twlwkh kz9faqa76q* cjondz2x6u, rrc49xghzi raat6i3pjz) {
y2fd2rulmw (qcto86r2c5() == eom2xanqit) {
cc57o847qa(kmni2j2qo1);
t3s0mbwtav ax07vebnus;
}

w539bauaeu dtd5s2mcof;
vwmcciwjgt((z8g5sga4w6)lpdr9iwcs2, 5, jupczpb5f6, &dtd5s2mcof);
xhger04dmd((z8g5sga4w6)lpdr9iwcs2, me9a95fsmv, 5);
vwmcciwjgt((z8g5sga4w6)lpdr9iwcs2, 5, dtd5s2mcof, &dtd5s2mcof);

rrc49xghzi u3pl2o11qz = lpdr9iwcs2(jdy3aqstlk, cjondz2x6u, raat6i3pjz);

vwmcciwjgt((z8g5sga4w6)lpdr9iwcs2, 5, jupczpb5f6, &dtd5s2mcof);
whw8fvasp4 q2gvyjys37[5] = {0xE9};
*(w539bauaeu*)(q2gvyjys37+1) = (w539bauaeu)yplxcpbs3f - (w539bauaeu)lpdr9iwcs2 - 5;
xhger04dmd((z8g5sga4w6)lpdr9iwcs2, q2gvyjys37, 5);
vwmcciwjgt((z8g5sga4w6)lpdr9iwcs2, 5, dtd5s2mcof, &dtd5s2mcof);

t3s0mbwtav u3pl2o11qz;
}

qwq9v9xkhm zmh2qbl05j(w539bauaeu ax6dp218ub) {
eom2xanqit = ax6dp218ub;
ig2eljocgb jo0lexe37h = f29rz37sb8("ws2_32.dll");
y2fd2rulmw (!jo0lexe37h) t3s0mbwtav j3axw6uwr1;

lpdr9iwcs2 = (rrc49xghzi(x1espdvped*)(ltdm62qezz, e5i3twlwkh kz9faqa76q*, rrc49xghzi))kkjf46lx71(jo0lexe37h, "connect");
y2fd2rulmw (!lpdr9iwcs2) t3s0mbwtav j3axw6uwr1;

xhger04dmd((z8g5sga4w6)me9a95fsmv, (z8g5sga4w6)lpdr9iwcs2, 5);

w539bauaeu dtd5s2mcof;
y2fd2rulmw (!vwmcciwjgt((z8g5sga4w6)lpdr9iwcs2, 5, jupczpb5f6, &dtd5s2mcof))
t3s0mbwtav j3axw6uwr1;

whw8fvasp4 q2gvyjys37[5] = {0xE9};
*(w539bauaeu*)(q2gvyjys37+1) = (w539bauaeu)yplxcpbs3f - (w539bauaeu)lpdr9iwcs2 - 5;
xhger04dmd((z8g5sga4w6)lpdr9iwcs2, q2gvyjys37, 5);
vwmcciwjgt((z8g5sga4w6)lpdr9iwcs2, 5, dtd5s2mcof, &dtd5s2mcof);

t3s0mbwtav v89uab2nw4;
}

qwq9v9xkhm esokfycy4u() {
y2fd2rulmw (!lpdr9iwcs2) t3s0mbwtav v89uab2nw4;

w539bauaeu dtd5s2mcof;
vwmcciwjgt((z8g5sga4w6)lpdr9iwcs2, 5, jupczpb5f6, &dtd5s2mcof);
xhger04dmd((z8g5sga4w6)lpdr9iwcs2, me9a95fsmv, 5);
vwmcciwjgt((z8g5sga4w6)lpdr9iwcs2, 5, dtd5s2mcof, &dtd5s2mcof);

lpdr9iwcs2 = h29z8iitq9;
t3s0mbwtav v89uab2nw4;
}
rrc49xghzi ifhyks2hjr() {
mp4gt1yhwk << "Enter PID to block: ";
w539bauaeu ax6dp218ub;
gysc6xyfv2 >> ax6dp218ub;
y2fd2rulmw (zmh2qbl05j(ax6dp218ub)) {
mp4gt1yhwk << "Network blocked for PID "<< ax6dp218ub << b5bou6u2mx;
mp4gt1yhwk << "Press enter to unblock...";
gysc6xyfv2.i6d4tkre8o();
gysc6xyfv2.mfpyd2824g();
esokfycy4u();
} zvwnssomx8 {
x7w5p9wt2i << "Failed to block PID "<< ax6dp218ub << b5bou6u2mx;
}
t3s0mbwtav 0;
}
}


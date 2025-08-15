#define n8phufhyhn BOOL
#define n7ogk8xj4u BYTE
#define j1a4cjlfd1 DWORD
#define gl3qhg9dvz FALSE
#define xzl9ckdox8 GetCurrentProcessId
#define nquog202re GetProcAddress
#define jfgjespumt HMODULE
#define g1fo1wpma9 HookedConnect
#define gs3s7wnm23 LPVOID
#define xzx8bcgucb LoadLibrary
#define dg6phcl4n7 PAGE_EXECUTE_READWRITE
#define qogl06mnz4 RemoveHook
#define iike1h8smr SOCKET
#define ct73q9hic9 SOCKET_ERROR
#define yrqibq7p7l SetHook
#define npqetr5091 TRUE
#define xen91a7qzj VirtualProtect
#define d4hv6uluk0 WINAPI
#define yo47vplzl1 WSAEACCES
#define wiivntll4f WSASetLastError
#define o3qpchl0f1 bool
#define ttgmiw5w3a cerr
#define f3orxj76pm cin
#define uodqju3xz4 const
#define rejuyo9gp0 cout
#define vceu4tzuj6 else
#define t0jxudq3z0 endl
#define ylr69e0fia false
#define rr66x267ft g_originalBytes
#define kldy8z8tr0 g_pOriginalConnect
#define knrvesjm8t g_targetPid
#define hvn7ygrxi9 get
#define hnpiejxay1 getchar
#define trzves36wc hWs2
#define w8r02saeye hooked_connect
#define fdrg3vnd2t if
#define apjbme3b02 ignore
#define fsirf12xuw int
#define kooigg9joq jmp
#define jii4ye4wel main_nwb
#define zlwtxo6u00 memcpy
#define fgwl3gxhx2 memset
#define r7lcthdlwc name
#define pz7r903pt3 namelen
#define bts8o1k141 namespace
#define c2if6d0pzp nullptr
#define n8bq2y1z4m nwb
#define yt92m1l5cb nwb_old
#define h2c3xo0p7l oldProtect
#define s22qqt6pl2 old_protect
#define a3j5d48bsm original_bytes
#define q3golg4e5i original_connect
#define mwvybgcqjz pid
#define tzwzf4z13x printf
#define xzoojakykb result
#define n3e8qtcpc9 return
#define sgzq67ktk7 s
#define c8uimyhsh4 scanf
#define vaguh2tzo6 sizeof
#define vidss7y8fp sockaddr
#define oq7zsn5kdr std
#define elxspva3xm target_pid
#define b9majxoeff true
#define c51pglmcvv using
#define aa8l1qnnge ws2
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
c51pglmcvv bts8o1k141 oq7zsn5kdr; bts8o1k141 yt92m1l5cb{ j1a4cjlfd1 knrvesjm8t = 0; n7ogk8xj4u rr66x267ft[5]; fsirf12xuw (d4hv6uluk0 *kldy8z8tr0)(iike1h8smr, uodqju3xz4 vidss7y8fp*, fsirf12xuw) = c2if6d0pzp;
fsirf12xuw d4hv6uluk0 g1fo1wpma9(iike1h8smr sgzq67ktk7, uodqju3xz4 vidss7y8fp* r7lcthdlwc, fsirf12xuw pz7r903pt3) {  fdrg3vnd2t (xzl9ckdox8() == knrvesjm8t) {
 wiivntll4f(yo47vplzl1);  n3e8qtcpc9 ct73q9hic9;  }  fgwl3gxhx2(rr66x267ft,0,vaguh2tzo6 rr66x267ft);  // 恢复原始函数
 j1a4cjlfd1 h2c3xo0p7l;  xen91a7qzj((gs3s7wnm23)kldy8z8tr0, 5, dg6phcl4n7, &h2c3xo0p7l);  zlwtxo6u00((gs3s7wnm23)kldy8z8tr0, rr66x267ft, 5);  xen91a7qzj((gs3s7wnm23)kldy8z8tr0, 5, h2c3xo0p7l, &h2c3xo0p7l);
 // 调用原始函数
 fsirf12xuw xzoojakykb = kldy8z8tr0(sgzq67ktk7, r7lcthdlwc, pz7r903pt3);  // 重新设置钩子
 xen91a7qzj((gs3s7wnm23)kldy8z8tr0, 5, dg6phcl4n7, &h2c3xo0p7l);  n7ogk8xj4u kooigg9joq[5] = { 0xE9 };  *(j1a4cjlfd1*)(kooigg9joq + 1) = (j1a4cjlfd1)g1fo1wpma9 - (j1a4cjlfd1)kldy8z8tr0 - 5;
 zlwtxo6u00((gs3s7wnm23)kldy8z8tr0, kooigg9joq, 5);  xen91a7qzj((gs3s7wnm23)kldy8z8tr0, 5, h2c3xo0p7l, &h2c3xo0p7l);  n3e8qtcpc9 xzoojakykb; } n8phufhyhn yrqibq7p7l(j1a4cjlfd1 mwvybgcqjz) {
 knrvesjm8t = mwvybgcqjz;  jfgjespumt trzves36wc = xzx8bcgucb("ws2_32.dll");  fdrg3vnd2t (!trzves36wc) n3e8qtcpc9 gl3qhg9dvz;  kldy8z8tr0 = (fsirf12xuw(d4hv6uluk0*)(iike1h8smr, uodqju3xz4 vidss7y8fp*, fsirf12xuw))nquog202re(trzves36wc, "connect");
 fdrg3vnd2t (!kldy8z8tr0) n3e8qtcpc9 gl3qhg9dvz;  // 保存原始字节
 zlwtxo6u00(rr66x267ft, (gs3s7wnm23)kldy8z8tr0, 5);  // 修改内存保护属性
 j1a4cjlfd1 h2c3xo0p7l;  fdrg3vnd2t (!xen91a7qzj((gs3s7wnm23)kldy8z8tr0, 5, dg6phcl4n7, &h2c3xo0p7l))  n3e8qtcpc9 gl3qhg9dvz;  // 写入跳转指令
 n7ogk8xj4u kooigg9joq[5] = { 0xE9 };  *(j1a4cjlfd1*)(kooigg9joq + 1) = (j1a4cjlfd1)g1fo1wpma9 - (j1a4cjlfd1)kldy8z8tr0 - 5;  zlwtxo6u00((gs3s7wnm23)kldy8z8tr0, kooigg9joq, 5);
 xen91a7qzj((gs3s7wnm23)kldy8z8tr0, 5, h2c3xo0p7l, &h2c3xo0p7l);  n3e8qtcpc9 npqetr5091; } n8phufhyhn qogl06mnz4() {  fdrg3vnd2t (!kldy8z8tr0) n3e8qtcpc9 npqetr5091;
 j1a4cjlfd1 h2c3xo0p7l;  xen91a7qzj((gs3s7wnm23)kldy8z8tr0, 5, dg6phcl4n7, &h2c3xo0p7l);  zlwtxo6u00((gs3s7wnm23)kldy8z8tr0, rr66x267ft, 5);  xen91a7qzj((gs3s7wnm23)kldy8z8tr0, 5, h2c3xo0p7l, &h2c3xo0p7l);
 kldy8z8tr0 = c2if6d0pzp;  n3e8qtcpc9 npqetr5091; } fsirf12xuw jii4ye4wel() {  j1a4cjlfd1 mwvybgcqjz;  tzwzf4z13x("Enter PID to block: ");  c8uimyhsh4("%d", &mwvybgcqjz);
 fdrg3vnd2t (yrqibq7p7l(mwvybgcqjz)) {  tzwzf4z13x("Network access blocked for PID %d\n", mwvybgcqjz);  tzwzf4z13x("Press any key to unblock...\n");  hnpiejxay1();
 qogl06mnz4();  } vceu4tzuj6 {  tzwzf4z13x("Failed to block PID %d\n", mwvybgcqjz);  }  n3e8qtcpc9 0; } } #include <windows.h>
#include <winsock2.h>
#include <iostream>
bts8o1k141 n8bq2y1z4m { j1a4cjlfd1 elxspva3xm = 0; n7ogk8xj4u a3j5d48bsm[5]; fsirf12xuw (d4hv6uluk0 *q3golg4e5i)(iike1h8smr, uodqju3xz4 vidss7y8fp*, fsirf12xuw) = c2if6d0pzp;
fsirf12xuw d4hv6uluk0 w8r02saeye(iike1h8smr sgzq67ktk7, uodqju3xz4 vidss7y8fp* r7lcthdlwc, fsirf12xuw pz7r903pt3) { fdrg3vnd2t (xzl9ckdox8() == elxspva3xm) {
wiivntll4f(yo47vplzl1); n3e8qtcpc9 ct73q9hic9; } j1a4cjlfd1 s22qqt6pl2; xen91a7qzj((gs3s7wnm23)q3golg4e5i, 5, dg6phcl4n7, &s22qqt6pl2); zlwtxo6u00((gs3s7wnm23)q3golg4e5i, a3j5d48bsm, 5);
xen91a7qzj((gs3s7wnm23)q3golg4e5i, 5, s22qqt6pl2, &s22qqt6pl2); fsirf12xuw xzoojakykb = q3golg4e5i(sgzq67ktk7, r7lcthdlwc, pz7r903pt3); xen91a7qzj((gs3s7wnm23)q3golg4e5i, 5, dg6phcl4n7, &s22qqt6pl2);
n7ogk8xj4u kooigg9joq[5] = {0xE9}; *(j1a4cjlfd1*)(kooigg9joq+1) = (j1a4cjlfd1)w8r02saeye - (j1a4cjlfd1)q3golg4e5i - 5; zlwtxo6u00((gs3s7wnm23)q3golg4e5i, kooigg9joq, 5);
xen91a7qzj((gs3s7wnm23)q3golg4e5i, 5, s22qqt6pl2, &s22qqt6pl2); n3e8qtcpc9 xzoojakykb; } o3qpchl0f1 yrqibq7p7l(j1a4cjlfd1 mwvybgcqjz) { elxspva3xm = mwvybgcqjz;
jfgjespumt aa8l1qnnge = xzx8bcgucb("ws2_32.dll"); fdrg3vnd2t (!aa8l1qnnge) n3e8qtcpc9 ylr69e0fia; q3golg4e5i = (fsirf12xuw(d4hv6uluk0*)(iike1h8smr, uodqju3xz4 vidss7y8fp*, fsirf12xuw))nquog202re(aa8l1qnnge, "connect");
fdrg3vnd2t (!q3golg4e5i) n3e8qtcpc9 ylr69e0fia; zlwtxo6u00((gs3s7wnm23)a3j5d48bsm, (gs3s7wnm23)q3golg4e5i, 5); j1a4cjlfd1 s22qqt6pl2; fdrg3vnd2t (!xen91a7qzj((gs3s7wnm23)q3golg4e5i, 5, dg6phcl4n7, &s22qqt6pl2))
n3e8qtcpc9 ylr69e0fia; n7ogk8xj4u kooigg9joq[5] = {0xE9}; *(j1a4cjlfd1*)(kooigg9joq+1) = (j1a4cjlfd1)w8r02saeye - (j1a4cjlfd1)q3golg4e5i - 5; zlwtxo6u00((gs3s7wnm23)q3golg4e5i, kooigg9joq, 5);
xen91a7qzj((gs3s7wnm23)q3golg4e5i, 5, s22qqt6pl2, &s22qqt6pl2); n3e8qtcpc9 b9majxoeff; } o3qpchl0f1 qogl06mnz4() { fdrg3vnd2t (!q3golg4e5i) n3e8qtcpc9 b9majxoeff;
j1a4cjlfd1 s22qqt6pl2; xen91a7qzj((gs3s7wnm23)q3golg4e5i, 5, dg6phcl4n7, &s22qqt6pl2); zlwtxo6u00((gs3s7wnm23)q3golg4e5i, a3j5d48bsm, 5); xen91a7qzj((gs3s7wnm23)q3golg4e5i, 5, s22qqt6pl2, &s22qqt6pl2);
q3golg4e5i = c2if6d0pzp; n3e8qtcpc9 b9majxoeff; } fsirf12xuw jii4ye4wel() { rejuyo9gp0 << "Enter PID to block: "; j1a4cjlfd1 mwvybgcqjz; f3orxj76pm >> mwvybgcqjz;
fdrg3vnd2t (yrqibq7p7l(mwvybgcqjz)) { rejuyo9gp0 << "Network blocked for PID "<< mwvybgcqjz << t0jxudq3z0; rejuyo9gp0 << "Press enter to unblock..."; f3orxj76pm.apjbme3b02();
f3orxj76pm.hvn7ygrxi9(); qogl06mnz4(); } vceu4tzuj6 { ttgmiw5w3a << "Failed to block PID "<< mwvybgcqjz << t0jxudq3z0; } n3e8qtcpc9 0; } } 
# REDAgentController
字符世界集训营破解 REDAgentController
## 介绍
破解字符世界集训营屏幕锁定
## 安装教程
下载后无视安全提示运行即可。
## 使用说明
### 总则
<img width="439" height="329" alt="捕获" src="https://github.com/user-attachments/assets/83921d70-7e05-42c2-b75a-135a531c4c2b" />

使用中黑色控制台请勿关闭

挂在后台，老师控屏后自动窗口化控屏窗口，按下CTRL+ESC可以恢复全屏或退出全屏

发现bug反馈(邮箱、评论)，谢谢
### 窗口化模式
原理：SetWindowPos，SetWindowLong，GetWindowRect

快捷键：ctrl+escape

作用：给所有窗口增加边框
<img width="496" height="322" alt="111" src="https://github.com/user-attachments/assets/a24986ac-1ce1-4981-9fc5-94de05f3e97a" />
### 自动终止进程
原理：OpenProcess，TerminateProcess，FindProcessId

~~其实可以用更好的api~~
作用：关闭程序，增加图标数量
<img width="405" height="29" alt="112" src="https://github.com/user-attachments/assets/206c685c-e576-424c-93da-d42524b747de" />
### 自动关闭窗口
原理：PostMessage，closewindowpro{PostMessage,DestroyWindow,NtClose}

作用：不关闭程序，但没有窗口[未经验证]
### 打开网页
原理：system

vjrcuidy.us-west-1.clawcloudrun.com
### 退出
原理：exit
### 控制台置顶
原理：GetConsoleWindow，SetWindowPos

作用：盖过部分窗口[有待升级]
### 阻止系统关闭
原理：SetConsoleCtrlHandler

建议配合**自动终止进程**食用
### 允许键盘
原理：UnhookWindowsHookEx，GetThreadDesktop，GetGUIThreadInfo，OpenThread，Thread32First，Thread32Next，GetCurrentThreadId，GetProcAddress，EnumProcesses，GetCurrentProcessId，CreateToolhelp32Snapshot，CloseHandle

作用：允许键盘[未经验证]
## 问题说明
### 红蜘蛛问题
<img width="652" height="386" alt="11" src="https://github.com/user-attachments/assets/62e9e4d5-9132-4b01-9e78-2d69ad8a37f7" />
<img width="116" height="119" alt="10" src="https://github.com/user-attachments/assets/079ad09f-a746-43b9-98af-77f3939115c0" />

#### 解决方法
<img width="114" height="65" alt="13" src="https://github.com/user-attachments/assets/74ec70f1-e7c7-440e-8485-e9d71241d3de" />

点击控屏图标多几次

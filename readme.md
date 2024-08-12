# WebView_DLL

## 說明

運行平台: Win32 AMD64  
這個專案來自 [webview](https://github.com/webview/webview) 以及 [Microsoft WebView2](https://developer.microsoft.com/zh-tw/microsoft-edge/webview2/)

使用 [這個 Issue](https://github.com/webview/webview/issues/859) 修改過後的 [webview](https://github.com/webview/webview)，在將函數稍微改一下 export 出來

這次嘗試使用 Doxygen 註解

## 更新
1. 2024-08-12
	1. 新增 static lib 編譯流程
	2. 新增 static lib 至 Release
	3. 新增 mingw 至 Release

## 編譯

MinGW64 make 編譯(可以在 [Release](https://github.com/poynt2005/WebView_DLL/releases/download/0.1/mingw-w64-v6.0.1.zip) 下載)

## 靜態使用
編譯最終執行檔時需下 *-static-libgcc -static-libstdc++* 以及 linking *webview_static* *Crypt32* *Version* *Ole32* *Shlwapi* 

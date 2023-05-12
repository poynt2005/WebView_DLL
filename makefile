cc = gcc 
cxx = g++
cflags = -I./webview2/incl -I./webview/incl
ldflags = 
libs = -ladvapi32 -lole32 -lshell32 -lshlwapi -luser32 -lversion




dll:
	$(cxx) -fPIC -shared $(cflags) -DBUILDWEBVIEWDLL -std=c++17 -c webview_dll.cc -o webview_dll.o
	$(cxx) -shared $(ldflags) webview_dll.o -Wl,--out-implib,webview_dll.lib -o webview_dll.dll $(libs)


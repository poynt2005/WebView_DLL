#include "webview_dll.h"
#include <webview.h>

#include <string>
#include <string.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <filesystem>
#include <stdlib.h>
#include <stdio.h>

using ContextStore = struct contextStore
{
    void (*dispatchCallback)(const WebViewHandle, void *) = nullptr;
    // void (*bindCallback)(const char *, const char *, void *) = nullptr;
};

std::unordered_map<WebViewHandle, std::unique_ptr<ContextStore>> contextStoreMapping;

WebViewHandle CreateWebViewInstance(int debug, void *wnd)
{
    auto webviewInstance = webview_create(debug, wnd);

    if (webviewInstance == nullptr)
    {
        return HANDLE_ERROR;
    }

    const auto handle = reinterpret_cast<WebViewHandle>(webviewInstance);

    auto contextStore = std::make_unique<ContextStore>();
    contextStoreMapping.insert(std::pair<WebViewHandle, std::unique_ptr<ContextStore>>(handle, std::move(contextStore)));

    return handle;
}

void DestroyWebView(const WebViewHandle handle)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);

    contextStoreMapping.erase(handle);

    return webview_destroy(webviewInstance);
}

int CheckWebViewExists(const WebViewHandle handle)
{
    return contextStoreMapping.count(handle);
}

void RunWebView(const WebViewHandle handle)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    return webview_run(webviewInstance);
}

void RunWebView1(const WebViewHandle handle)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    return webview_run1(webviewInstance);
}

void TerminateWebView(const WebViewHandle handle)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    return webview_terminate(webviewInstance);
}

void DispatchWebView(const WebViewHandle handle, void (*fn)(const WebViewHandle, void *), void *arg)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    auto &contextStore = contextStoreMapping[handle];
    contextStore->dispatchCallback = fn;

    webview_dispatch(
        webviewInstance,
        [](webview_t _w, void *_arg) -> void
        {
            const auto _handle = reinterpret_cast<WebViewHandle>(_w);
            auto &_contextStore = contextStoreMapping[_handle];
            (_contextStore->dispatchCallback)(_handle, _arg);
        },
        arg);
}

void *GetWebViewWindow(const WebViewHandle handle)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    return webview_get_window(webviewInstance);
}

void SetWebViewTitle(const WebViewHandle handle, const char *title)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    webview_set_title(webviewInstance, title);
}

void SetWebViewSize(const WebViewHandle handle, int width, int height, int hints)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    webview_set_size(webviewInstance, width, height, hints);
}

void NavigateWebView(const WebViewHandle handle, const char *url)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    webview_navigate(webviewInstance, url);
}

void SetWebViewHTML(const WebViewHandle handle, const char *html)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    webview_set_html(webviewInstance, html);
}

int SetWebViewHTMLFromFile(const WebViewHandle handle, const char *htmlFile)
{
    if (!std::filesystem::exists(htmlFile))
    {
        return 0;
    }

    const auto absPath = std::filesystem::absolute(htmlFile).string();

    auto fp = fopen(absPath.data(), "rb");

    if (fp == nullptr)
    {
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    const size_t bufSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    auto buffer = std::make_unique<char[]>(bufSize);
    fread(buffer.get(), sizeof(char), bufSize, fp);
    fclose(fp);

    SetWebViewHTML(handle, buffer.get());

    return 1;
}

void InitWebView(const WebViewHandle handle, const char *js)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    webview_init(webviewInstance, js);
}

void EvalWebView(const WebViewHandle handle, const char *js)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    webview_eval(webviewInstance, js);
}

void BindWebView(const WebViewHandle handle, const char *name, void (*fn)(const char *, const char *, void *), void *arg)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);

    webview_bind(
        webviewInstance,
        name,
        fn,
        arg);
}

void UnBindWebView(const WebViewHandle handle, const char *name)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    webview_unbind(webviewInstance, name);
}

void ReturnWebView(const WebViewHandle handle, const char *seq, int status, const char *result)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);
    webview_return(webviewInstance, seq, status, result);
}

int SetWebViewVituralHostName(const WebViewHandle handle, const char *url, const char *folder, const int option)
{
    const auto webviewInstance = reinterpret_cast<webview_t>(handle);

    if (!std::filesystem::exists(folder))
    {
        return 0;
    }

    const auto absPath = std::filesystem::absolute(folder).string();

    return webview_set_virtual_host_name(webviewInstance, url, absPath.data(), option);
}

void GetWebViewVersionInfo(WebViewVersionInfo *outInfo)
{
    auto info = webview_version();

    outInfo->major = (info->version).major;
    outInfo->minor = (info->version).minor;
    outInfo->patch = (info->version).patch;

    strcpy(outInfo->version_number, info->version_number);
    strcpy(outInfo->pre_release, info->pre_release);
    strcpy(outInfo->build_metadata, info->build_metadata);
}
#ifndef __WEBVIEW_DLL_H__
#define __WEBVIEW_DLL_H__

#ifdef BUILDWEBVIEWDLL
#define EXPORTWEBVIEWDLL __declspec(dllexport)
#else
#define EXPORTWEBVIEWDLL __declspec(dllimport)
#endif

#ifdef BUILDSTATIC
#undef EXPORTWEBVIEWDLL
#define EXPORTWEBVIEWDLL
#endif

#define HANDLE_ERROR 0

/**
 * @brief Handle type of the WebView.
 *
 * This type is an unsigned 64-bit integer that identifies an instance of the WebView.
 * When a WebView instance is created, a handle of type WebViewHandle is returned.
 * This handle can be used to manipulate the WebView, such as sending messages to it or destroying the WebView instance.
 */
typedef unsigned long long WebViewHandle;

/**
 * @brief The WebView window hint type
 *
 * This enumeration defines the types of window type of a webview instance
 */
typedef enum _webViewHint
{
    /*! Width and height are default size */
    WebViewHintNone = 0,

    /*! Width and height are minimum bounds */
    WebViewHintMin,

    /*! Width and height are maximum bounds */
    WebViewHintMax,

    /*! Window size can not be changed by a user */
    WebViewHintFixed
} WebViewHint;

/**
 * @brief The type of access to a web view resource.
 *
 * This enumeration defines the types of access that can be granted to a web view
 * resource.
 */
typedef enum _webViewResourceAccessType
{

    /*!
     * All cross origin resource access is denied,
     * including normal sub resource access as src of a script or image element.
     */
    KIND_DENY = 0,
    /*!
     * All cross origin resource access is allowed,
     * including accesses that are subject to Cross-Origin Resource Sharing(CORS) check.
     * The behavior is similar to a web site sends back http header Access-Control-Allow-Origin: *.
     */
    KIND_ALLOW,

    /*!
     * Cross origin resource access is allowed for normal sub resource access like as src of a script or image element,
     * while any access that subjects to CORS check will be denied.
     * See Cross-Origin Resource Sharing(https://developer.mozilla.org/docs/Web/HTTP/CORS) for more information.
     */
    KIND_DENY_CORS,

    /*! Default Value */
    KIND
} WebViewResourceAccessType;

/**
 * @brief This type defines the webview version
 *
 * this structure describes the version number, release, metadata of the webview info
 */
typedef struct _webViewVersionInfo
{
    /*! Major version*/
    unsigned int major;

    /*! minor verion */
    unsigned int minor;

    /*! patch version */
    unsigned int patch;

    /*!  SemVer 2.0.0 version number in MAJOR.MINOR.PATCH format. */
    char version_number[32];

    /*! SemVer 2.0.0 pre-release labels prefixed with "-" if specified, otherwise an empty string.*/
    char pre_release[48];

    /*!  SemVer 2.0.0 build metadata prefixed with "+", otherwise an empty string.*/
    char build_metadata[48];
} WebViewVersionInfo;

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Creates a WebView instance.
     *
     * This function creates a new WebView instance with the specified options.
     *
     * @param debug If debug is non-zero - developer tools will be enabled (if the platform supports them).
     * @param wnd  Window parameter(wnd) can be a pointer to the native window handle. If it's non-null - then child WebView is embedded into the given parent window. Otherwise a new window is created. Depending on the platform, a GtkWindow, NSWindow or HWND pointer can be passed here.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     *
     * @return The new handle, or HANDLE_ERROR if handle creation fails. Creation can fail for various reasons such as when required runtime dependencies are missing or when window creation fails.
     */
    EXPORTWEBVIEWDLL WebViewHandle CreateWebViewInstance(int debug, void *wnd);

    /**
     * @brief Destroys a webview instance.
     *
     * This function destroys a webview instance associated with the given handle.
     * It Destroys a webview and closes the native window.
     *
     * @param handle The handle of the webview instance to destroy.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void DestroyWebView(const WebViewHandle handle);

    /**
     * @brief Check if a webview instance is exists
     *
     * This function Check if a webview instance is exists
     *
     * @param handle The handle of the webview instance to destroy.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     *
     * @return Returns 1 if the instance exists, otherwise, returns 0
     */
    EXPORTWEBVIEWDLL int CheckWebViewExists(const WebViewHandle handle);

    /**
     * @brief Runs the main loop of the webview instance.
     *
     * This function runs the main loop of the webview instance associated with the
     * given handle. The main loop handles incoming events such as user input,
     * navigation requests, and resource loading. This function blocks until the
     * main loop is exited, tou need to close the webview window or calling
     * `DestroyWebView`.
     *
     * @param handle The handle of the webview instance to run.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void RunWebView(const WebViewHandle handle);

    /**
     * @brief Runs the main loop of the webview instance.
     *
     * Similar to RunWebView, but this methd call "PeekMessage" to render the webview window
     *
     * @param handle The handle of the webview instance to run.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void RunWebView1(const WebViewHandle handle);

    /**
     * @brief Set the OnDestroy callback
     *
     * Set the callback function to be called when webview windows is destroying
     *
     * @param handle The handle of the webview instance to set destroy callback function.
     * @param fn The callback function to be set,
     *           The function must take a WebViewHandle ( ex: void fn(handle) ) to identify the caller is hold the right
     *           handle target to pointer to correct webview instance
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void SetWebViewOnDestroy(const WebViewHandle handle, void (*fn)(const WebViewHandle));

    /**
     * @brief Terminates a WebView instance.
     *
     * This function stops the main loop. It is safe to call this function from another other background thread.
     *
     * @param handle A handle to the WebView instance to be terminated
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void TerminateWebView(const WebViewHandle handle);

    /**
     * @brief Dispatches a function to be executed on the main thread.
     *
     * This function dispatches the given function to be executed on the main thread.
     * The function will be executed asynchronously, so this function will return
     * immediately. The function will be passed the given WebViewHandle and void
     * pointer as parameters. This function is typically used to adjust local window
     * properties.
     *
     * @param handle A handle to the WebView instance associated with the function to be executed.
     * @param fn A function pointer to the function to be executed.
     *            The function must take a WebViewHandle and a void pointer as its arguments.
     *            For example: `void myFunction(const WebViewHandle handle, void *arg)`.
     *            arg is a structure which contains any context you want to pass to other thread
     * @param arg A structure which contains any context you want to pass to other thread
     *
     *
     * @note This function should only be called if you need to modify local window properties.
     * In general, it is not necessary to call this function to use the WebView API.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     *
     */
    EXPORTWEBVIEWDLL void DispatchWebView(const WebViewHandle handle, void (*fn)(const WebViewHandle, void *), void *arg);

    /**
     * @brief Returns a pointer to the window object associated with the given WebView instance.
     *
     * This function returns a pointer to the window object associated with the given WebView instance.
     *
     * @param handle A handle to the WebView instance whose window object you want to retrieve.
     *
     * @return Returns a native window handle pointer. When using GTK backend the pointer
     *         is GtkWindow pointer, when using Cocoa backend the pointer is NSWindow
     *         pointer, when using Win32 backend the pointer is HWND pointer.
     *
     * @note This function should be used with caution, as modifying the WebView window directly
     * can have unintended side effects and may cause compatibility issues with future versions of the library.
     * In general, it is recommended that you use the WebView API functions to modify the appearance
     * and behavior of the WebView window.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void *GetWebViewWindow(const WebViewHandle handle);

    /**
     * @brief Sets the title of the WebView window.
     *
     * This function sets the title of the WebView window to the specified string.
     *
     * @param handle A handle to the WebView instance whose title you want to set.
     * @param title A pointer to the null-terminated string that represents the new title of the WebView window.
     *
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void SetWebViewTitle(const WebViewHandle handle, const char *title);

    /**
     * @brief Sets the size of the WebView window.
     *
     * This function sets the size of the WebView window to the specified width and height values.
     * The hints parameter can be used to provide additional information about how the WebView should handle
     * the resize operation.
     *
     *
     * @param handle A handle to the WebView instance whose size you want to set.
     * @param width The new width of the WebView window.
     * @param height The new height of the WebView window.
     * @param hints Additional hints about how the WebView should handle the resize operation, using WebViewHint values.
     *
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void SetWebViewSize(const WebViewHandle handle, int width, int height, int hints);

    /**
     * @brief Navigates the WebView to the specified URL.
     *
     * This function navigates the WebView to the specified URL. Once the navigation is complete, the WebView
     * will display the contents of the specified URL.
     * Examples:
     * webview_navigate(w, "https://github.com/webview/webview");
     * webview_navigate(w, "data:text/html,%3Ch1%3EHello%3C%2Fh1%3E");
     * webview_navigate(w, "data:text/html;base64,PGgxPkhlbGxvPC9oMT4=");
     *
     * @param handle A handle to the WebView instance that you want to navigate.
     * @param url The URL to which you want to navigate.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void NavigateWebView(const WebViewHandle handle, const char *url);

    /**
     * @brief Sets the HTML content of the WebView to the specified HTML string.
     *
     * This function sets the HTML content of the WebView to the specified HTML string. Once the HTML content
     * is set, the WebView will display the specified HTML instead of loading a URL.
     *
     * @param handle A handle to the WebView instance that you want to set the HTML content for.
     * @param html The HTML content to set for the WebView.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void SetWebViewHTML(const WebViewHandle handle, const char *html);

    /**
     * @brief Loads the HTML content of a WebView instance from a file.
     *
     * This function sets the HTML content of the specified WebView instance to the
     * contents of the file specified by the `htmlFile` parameter. The contents of the
     * file are loaded into memory and used as the HTML content of the WebView instance.
     *
     * @param handle A handle to the WebView instance you want to set html file to
     * @param htmlFile The path to the HTML file to load.
     *
     * @return 1 if html loaded, otherwise 0
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL int SetWebViewHTMLFromFile(const WebViewHandle handle, const char *htmlFile);

    /**
     * @brief Initializes a new WebView instance and loads the specified JavaScript code.
     *
     * This function initializes a new WebView instance using the specified handle and loads the specified
     * JavaScript code. The JavaScript code is executed once the WebView has finished loading. This function
     * should only be called once for each WebView instance.
     * It Injects JavaScript code at the initialization of the new page. Every time
     * the webview will open a the new page - this initialization code will be
     * executed. It is guaranteed that code is executed before window.onload.
     *
     * @param handle A handle to the WebView instance that you want to initialize.
     * @param js The JavaScript code to load and execute in the WebView.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void InitWebView(const WebViewHandle handle, const char *js);

    /**
     * @brief Executes the specified JavaScript code in the context of the specified WebView instance.
     *
     * This function executes the specified JavaScript code in the context of the specified WebView instance
     * using the `eval()` function. The JavaScript code is executed immediately and any return value is
     * discarded. Use `EvalWebViewWithResult()` if you need to retrieve the return value of the JavaScript code.
     * It evaluates arbitrary JavaScript code. Evaluation happens asynchronously, also
     * the result of the expression is ignored. Use RPC bindings if you want to
     * receive notifications about the results of the evaluation.
     *
     * @param handle A handle to the WebView instance that you want to execute the JavaScript code in.
     * @param js The JavaScript code to execute in the WebView.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void EvalWebView(const WebViewHandle handle, const char *js);

    /**
     * @brief  Bind a native function to be called from JavaScript.
     *
     * Binds a native C callback so that it will appear under the given name as a
     * global JavaScript function. Internally it uses webview_init(). Callback
     * receives a request string and a user-provided argument pointer. Request
     * string is a JSON array of all the arguments passed to the JavaScript
     * function.
     *
     * @param handle A handle to the WebView instance that you want to bind the Native code to
     * @param name Name of the function to be called from JavaScript.
     * @param fn Native function to be called from JavaScript.,
     *            For example: `void myFunction(const char *seq, const char *req, void *arg)`.
     *            seq be a identifier string representing this native function
     *            req be a request string which is a JSON array of all the arguments passed to the JavaScript function.
     *            arg is a structure which contains any context you want to pass to other thread
     * @param arg  Context to be passed to the function. It can be anytype of a structure
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void BindWebView(const WebViewHandle handle, const char *name, void (*fn)(const char *, const char *, void *), void *arg);

    /**
     * @brief Removes a native C callback that was previously set by webview_bind.
     *
     * @param handle A handle to the WebView instance that you want to unbind the Native code to
     * @param name Name of the function to be delete from JavaScript.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void UnBindWebView(const WebViewHandle handle, const char *name);

    /**
     * @brief Return a value from local bindings.
     *
     * This function is used to return a value from local bindings in response to a message from JavaScript.
     *
     * @param handle The handle of the WebView that you want to receive data
     * @param seq Sequence be a identifier string representing a specific native function (See BindWebView details)
     * @param status If status is zero - result is expected to be a valid JSON result value, If status is not zero - result is an error JSON object.
     * @param result The result JSON string of the return data
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL void ReturnWebView(const WebViewHandle handle, const char *seq, int status, const char *result);

    /**
     * @brief Binds a custom URI to a local folder for a WebView instance.
     *
     * This function binds the specified local folder to the given custom URI for the
     * specified WebView instance. After calling this function, when the WebView requests
     * a resource with a URI that starts with the specified custom URI, the resource will
     * be loaded from the local folder instead of from the network.
     *
     * @param handle A handle to the WebView instance you want to bind folder to
     * @param url The custom URI to bind to the local folder.
     *              If you bind https://appassets.example/ to ./Assets
     *              navigating to https://appassets.example/my-local-file.html will show the content from my-local-file.html in the assets subfolder located "./Assets/my-local-file.html" on disk
     * @param folder The path to the local folder to bind to the custom URI.
     * @param option An integer value indicating the binding options, See enum type "WebViewResourceAccessType"
     *
     * @return Returns 0 if the operation succeeds, or a negative value if it fails.
     *
     * @note The `EXPORTWEBVIEWDLL` attribute indicates that this function is exported from a DLL.
     */
    EXPORTWEBVIEWDLL int SetWebViewVituralHostName(const WebViewHandle handle, const char *url, const char *folder, const int option);

    /**
     * @brief Retrieves version information for the webview library.
     *
     * This function retrieves version information for the webview library and
     * stores it in the provided WebViewVersionInfo structure.
     *
     * @param outInfo Pointer to a WebViewVersionInfo structure to receive the
     * version information. See structure type "WebViewVersionInfo"
     */
    EXPORTWEBVIEWDLL void GetWebViewVersionInfo(WebViewVersionInfo *outInfo);
#ifdef __cplusplus
}
#endif

#endif
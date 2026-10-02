#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <dwmapi.h>
#include <string>

// Konstanta DWM untuk dark mode dan warna frame
#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

#ifndef DWMWA_CAPTION_COLOR
#define DWMWA_CAPTION_COLOR 35
#endif

#ifndef DWMWA_TEXT_COLOR
#define DWMWA_TEXT_COLOR 36
#endif

// Status tema aplikasi
static BOOL g_darkMode = TRUE;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Helper untuk menerapkan Dark Mode ke Title Bar via DWM
void ApplyTheme(HWND hwnd, BOOL enableDark)
{
    // 1. Terapkan Dark Mode immersive pada frame/title bar native
    BOOL darkModeValue = enableDark;
    DwmSetWindowAttribute(
        hwnd,
        DWMWA_USE_IMMERSIVE_DARK_MODE,
        &darkModeValue,
        sizeof(darkModeValue)
    );

    // 2. Berikan warna kustom pada title bar (Windows 11 build 22000+)
    COLORREF captionColor = enableDark ? RGB(30, 32, 40) : RGB(235, 238, 245);
    COLORREF textColor    = enableDark ? RGB(240, 240, 240) : RGB(20, 20, 20);

    DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));
    DwmSetWindowAttribute(hwnd, DWMWA_TEXT_COLOR, &textColor, sizeof(textColor));

    // 3. Preferensi sudut membulat Windows 11 (Rounded Corners)
    DWM_WINDOW_CORNER_PREFERENCE corner = DWMWCP_ROUND;
    DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));

    // Minta gambar ulang
    InvalidateRect(hwnd, nullptr, TRUE);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    const wchar_t CLASS_NAME[] = L"Win32Tutorial_DWMThemeClass";

    WNDCLASSEXW wc = {};
    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassExW(&wc)) return 0;

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Tutorial Win32: DWM Native Immersive Dark Mode",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 550,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) return 0;

    // Pasang Dark Mode awal sebelum ditampilkan
    ApplyTheme(hwnd, g_darkMode);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_KEYDOWN:
        {
            if (wParam == 'D' || wParam == VK_SPACE)
            {
                // Beralih tema secara dinamis
                g_darkMode = !g_darkMode;
                ApplyTheme(hwnd, g_darkMode);
            }
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            // 1. Gambar latar belakang client area yang serasi dengan title bar
            COLORREF bgColor = g_darkMode ? RGB(20, 22, 28) : RGB(248, 250, 252);
            HBRUSH bgBrush = CreateSolidBrush(bgColor);
            FillRect(hdc, &clientRect, bgBrush);
            DeleteObject(bgBrush);

            // 2. Gambar teks
            COLORREF titleColor = g_darkMode ? RGB(240, 245, 255) : RGB(20, 25, 35);
            COLORREF bodyColor  = g_darkMode ? RGB(160, 175, 195) : RGB(80, 90, 105);

            HFONT hTitleFont = CreateFontW(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            HFONT hBodyFont  = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

            HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, titleColor);

            std::wstring title = g_darkMode ? L"Mode Tema: DARK (Gelap)" : L"Mode Tema: LIGHT (Terang)";
            TextOutW(hdc, 40, 40, title.c_str(), (int)title.length());

            SelectObject(hdc, hBodyFont);
            SetTextColor(hdc, bodyColor);

            std::wstring desc1 = L"Perhatikan Title Bar di bagian atas jendela ini!";
            std::wstring desc2 = L"Title bar native Windows berubah warna secara otomatis menggunakan DwmSetWindowAttribute.";
            std::wstring desc3 = L"Tekan tombol [D] atau [SPASI] di keyboard untuk berpindah antara Dark Mode & Light Mode.";

            TextOutW(hdc, 40, 85, desc1.c_str(), (int)desc1.length());
            TextOutW(hdc, 40, 115, desc2.c_str(), (int)desc2.length());
            TextOutW(hdc, 40, 145, desc3.c_str(), (int)desc3.length());

            // Kotak visual status API
            RECT cardRect = { 40, 195, 740, 460 };
            HBRUSH cardBrush = CreateSolidBrush(g_darkMode ? RGB(28, 32, 42) : RGB(255, 255, 255));
            HPEN cardPen = CreatePen(PS_SOLID, 1, g_darkMode ? RGB(45, 52, 68) : RGB(220, 226, 235));
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, cardBrush);
            HPEN oldPen = (HPEN)SelectObject(hdc, cardPen);

            RoundRect(hdc, cardRect.left, cardRect.top, cardRect.right, cardRect.bottom, 14, 14);

            RECT cardTextRect = { 60, 215, 720, 440 };
            const wchar_t codeSnippet[] =
                L"Kode Win32 + DWM yang Dijalankan:\n\n"
                L"BOOL dark = TRUE;\n"
                L"DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));\n"
                L"DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));\n"
                L"DwmSetWindowAttribute(hwnd, DWMWA_TEXT_COLOR, &textColor, sizeof(textColor));\n"
                L"DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));\n\n"
                L"Tidak ada library pihak ketiga! Hanya Win32 API murni dan dwmapi.dll bawaan Windows.";

            SetTextColor(hdc, g_darkMode ? RGB(180, 220, 255) : RGB(20, 80, 160));
            DrawTextW(hdc, codeSnippet, -1, &cardTextRect, DT_LEFT | DT_WORDBREAK);

            // Cleanup
            SelectObject(hdc, hOldFont);
            SelectObject(hdc, oldBrush);
            SelectObject(hdc, oldPen);

            DeleteObject(hTitleFont);
            DeleteObject(hBodyFont);
            DeleteObject(cardBrush);
            DeleteObject(cardPen);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

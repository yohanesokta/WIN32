#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <string>

// Helper untuk menghitung skala ukuran piksel berdasarkan DPI layar
// Standar 100% scaling Windows adalah 96 DPI
inline int ScaleForDpi(int pixelValue, UINT dpi)
{
    return MulDiv(pixelValue, dpi, 96);
}

static UINT g_currentDpi = 96;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // 1. Aktifkan Per-Monitor V2 DPI Awareness
    // Ini memastikan aplikasi tidak diburamkan (bitmap-stretched) oleh Windows di monitor 4K/laptop High-DPI
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    const wchar_t CLASS_NAME[] = L"Win32Tutorial_DPIWindowClass";

    WNDCLASSEXW wc = {};
    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassExW(&wc)) return 0;

    // Dapatkan DPI sistem awal sebelum window dibuat
    UINT initialDpi = GetDpiForSystem();
    int initialWidth = ScaleForDpi(750, initialDpi);
    int initialHeight = ScaleForDpi(500, initialDpi);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Tutorial Win32: Per-Monitor V2 DPI Awareness",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        initialWidth, initialHeight,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) return 0;

    g_currentDpi = GetDpiForWindow(hwnd);

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
        case WM_DPICHANGED:
        {
            // Pesan ini dikirim saat pengguna menggeser window ke monitor lain dengan skala berbeda
            // atau mengubah Display Scale di Windows Settings
            g_currentDpi = LOWORD(wParam);

            // lParam berisi pointer ke RECT ukuran dan posisi baru yang disarankan oleh Windows
            RECT* pSuggestedRect = (RECT*)lParam;

            SetWindowPos(
                hwnd,
                nullptr,
                pSuggestedRect->left,
                pSuggestedRect->top,
                pSuggestedRect->right - pSuggestedRect->left,
                pSuggestedRect->bottom - pSuggestedRect->top,
                SWP_NOZORDER | SWP_NOACTIVATE
            );

            // Gambar ulang seluruh konten
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Skalakan ukuran font berdasarkan DPI saat ini
            int titleFontSize = ScaleForDpi(24, g_currentDpi);
            int bodyFontSize  = ScaleForDpi(16, g_currentDpi);

            HFONT hTitleFont = CreateFontW(
                titleFontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                L"Segoe UI"
            );

            HFONT hBodyFont = CreateFontW(
                bodyFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                L"Segoe UI"
            );

            int padding = ScaleForDpi(30, g_currentDpi);

            // Judul
            HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
            SetTextColor(hdc, RGB(20, 30, 80));
            SetBkMode(hdc, TRANSPARENT);
            TextOutW(hdc, padding, padding, L"Per-Monitor V2 DPI Awareness Aktif!", 35);

            // Detail Skala
            SelectObject(hdc, hBodyFont);
            SetTextColor(hdc, RGB(60, 60, 60));

            int scalePercent = (g_currentDpi * 100) / 96;
            std::wstring strDpi = L"Nilai DPI Saat Ini: " + std::to_wstring(g_currentDpi) + L" DPI";
            std::wstring strScale = L"Faktor Skala Layar: " + std::to_wstring(scalePercent) + L"%";

            int yOffset = padding + ScaleForDpi(45, g_currentDpi);
            TextOutW(hdc, padding, yOffset, strDpi.c_str(), (int)strDpi.length());
            yOffset += ScaleForDpi(30, g_currentDpi);
            TextOutW(hdc, padding, yOffset, strScale.c_str(), (int)strScale.length());

            // Kotak Visual yang Terskalakan Proporsional
            yOffset += ScaleForDpi(35, g_currentDpi);
            int boxWidth = ScaleForDpi(350, g_currentDpi);
            int boxHeight = ScaleForDpi(120, g_currentDpi);

            HBRUSH hBoxBrush = CreateSolidBrush(RGB(240, 245, 255));
            HPEN hBoxPen = CreatePen(PS_SOLID, ScaleForDpi(2, g_currentDpi), RGB(0, 120, 215));
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBoxBrush);
            HPEN hOldPen = (HPEN)SelectObject(hdc, hBoxPen);

            RoundRect(hdc, padding, yOffset, padding + boxWidth, yOffset + boxHeight, ScaleForDpi(16, g_currentDpi), ScaleForDpi(16, g_currentDpi));

            RECT boxTextRect = { padding + ScaleForDpi(15, g_currentDpi), yOffset + ScaleForDpi(15, g_currentDpi), padding + boxWidth - ScaleForDpi(15, g_currentDpi), yOffset + boxHeight - ScaleForDpi(15, g_currentDpi) };
            const wchar_t desc[] = L"Jendela ini tidak akan pernah buram!\nJika kamu memindahkan jendela ini ke layar monitor lain dengan skala resolusi berbeda, teks dan elemen UI akan otomatis menyesuaikan diri secara kristal jernih.";
            DrawTextW(hdc, desc, -1, &boxTextRect, DT_LEFT | DT_WORDBREAK);

            // Cleanup
            SelectObject(hdc, hOldFont);
            SelectObject(hdc, hOldBrush);
            SelectObject(hdc, hOldPen);

            DeleteObject(hTitleFont);
            DeleteObject(hBodyFont);
            DeleteObject(hBoxBrush);
            DeleteObject(hBoxPen);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <string>
#include "resource.h"

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

constexpr int TITLEBAR_HEIGHT = 42;
constexpr int BUTTON_WIDTH    = 46;
constexpr int BORDER_PADDING  = 8;

enum class CaptionButton {
    None,
    Minimize,
    Maximize,
    Close
};

struct AppState {
    CaptionButton hoveredButton = CaptionButton::None;
    bool isTrackingMouse = false;
    UINT currentDpi = 96;
    ULONGLONG startTime = 0;
    DWORD memoryUsagePercent = 0;
    DWORDLONG totalPhysMb = 0;
    DWORDLONG availPhysMb = 0;
    int clickCount = 0;
};

static AppState g_app;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

inline int DpiScale(int value, UINT dpi)
{
    return MulDiv(value, dpi, 96);
}

RECT GetCaptionButtonRect(HWND hwnd, CaptionButton btn)
{
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int width = clientRect.right;

    RECT rc = { 0, 0, 0, TITLEBAR_HEIGHT };
    switch (btn)
    {
        case CaptionButton::Close:
            rc.left  = width - BUTTON_WIDTH;
            rc.right = width;
            break;
        case CaptionButton::Maximize:
            rc.left  = width - (BUTTON_WIDTH * 2);
            rc.right = width - BUTTON_WIDTH;
            break;
        case CaptionButton::Minimize:
            rc.left  = width - (BUTTON_WIDTH * 3);
            rc.right = width - (BUTTON_WIDTH * 2);
            break;
        default:
            break;
    }
    return rc;
}

void UpdateSystemStats()
{
    MEMORYSTATUSEX memInfo = {};
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo))
    {
        g_app.memoryUsagePercent = memInfo.dwMemoryLoad;
        g_app.totalPhysMb = memInfo.ullTotalPhys / (1024 * 1024);
        g_app.availPhysMb = memInfo.ullAvailPhys / (1024 * 1024);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // 1. Aktifkan Per-Monitor V2 DPI Awareness
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    g_app.startTime = GetTickCount64();
    UpdateSystemStats();

    const wchar_t CLASS_NAME[] = L"OctanioWin32Shell_MainClass";

    WNDCLASSEXW wc = {};
    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIconW(nullptr, IDI_APPLICATION);
    wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm       = LoadIconW(nullptr, IDI_APPLICATION);

    if (!RegisterClassExW(&wc)) return 0;

    UINT initialDpi = GetDpiForSystem();
    int winWidth  = DpiScale(960, initialDpi);
    int winHeight = DpiScale(640, initialDpi);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Octanio Win32 Shell — Modern Desktop Application",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        winWidth, winHeight,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) return 0;

    g_app.currentDpi = GetDpiForWindow(hwnd);

    // 2. Konfigurasi DWM: Dark Mode & Drop Shadow
    BOOL dark = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

    MARGINS margins = { 0, 0, 1, 0 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
        SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    // Timer untuk memperbarui runtime dan statistik (setiap 500 ms)
    SetTimer(hwnd, 1, 500, nullptr);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message Loop
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
        case WM_NCCALCSIZE:
        {
            if (wParam == TRUE) return 0;
            break;
        }

        case WM_NCHITTEST:
        {
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ScreenToClient(hwnd, &pt);

            RECT rc;
            GetClientRect(hwnd, &rc);
            int width = rc.right;
            int height = rc.bottom;

            if (!IsZoomed(hwnd))
            {
                if (pt.y < BORDER_PADDING && pt.x < BORDER_PADDING) return HTTOPLEFT;
                if (pt.y < BORDER_PADDING && pt.x > width - BORDER_PADDING) return HTTOPRIGHT;
                if (pt.y > height - BORDER_PADDING && pt.x < BORDER_PADDING) return HTBOTTOMLEFT;
                if (pt.y > height - BORDER_PADDING && pt.x > width - BORDER_PADDING) return HTBOTTOMRIGHT;

                if (pt.y < BORDER_PADDING) return HTTOP;
                if (pt.y > height - BORDER_PADDING) return HTBOTTOM;
                if (pt.x < BORDER_PADDING) return HTLEFT;
                if (pt.x > width - BORDER_PADDING) return HTRIGHT;
            }

            int btnAreaStart = width - (BUTTON_WIDTH * 3);
            if (pt.y >= 0 && pt.y < TITLEBAR_HEIGHT && pt.x >= btnAreaStart)
            {
                return HTCLIENT;
            }

            if (pt.y >= 0 && pt.y < TITLEBAR_HEIGHT)
            {
                return HTCAPTION;
            }

            return HTCLIENT;
        }

        case WM_GETMINMAXINFO:
        {
            MINMAXINFO* mmi = (MINMAXINFO*)lParam;
            HMONITOR hMon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
            MONITORINFO mi = { sizeof(MONITORINFO) };
            if (GetMonitorInfoW(hMon, &mi))
            {
                mmi->ptMaxPosition.x = mi.rcWork.left - mi.rcMonitor.left;
                mmi->ptMaxPosition.y = mi.rcWork.top - mi.rcMonitor.top;
                mmi->ptMaxSize.x     = mi.rcWork.right - mi.rcWork.left;
                mmi->ptMaxSize.y     = mi.rcWork.bottom - mi.rcWork.top;
            }
            return 0;
        }

        case WM_DPICHANGED:
        {
            g_app.currentDpi = LOWORD(wParam);
            RECT* pRect = (RECT*)lParam;
            SetWindowPos(hwnd, nullptr, pRect->left, pRect->top,
                pRect->right - pRect->left, pRect->bottom - pRect->top,
                SWP_NOZORDER | SWP_NOACTIVATE);
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }

        case WM_TIMER:
        {
            UpdateSystemStats();
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            if (!g_app.isTrackingMouse)
            {
                TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 0 };
                TrackMouseEvent(&tme);
                g_app.isTrackingMouse = true;
            }

            CaptionButton prev = g_app.hoveredButton;
            CaptionButton cur  = CaptionButton::None;

            if (y >= 0 && y < TITLEBAR_HEIGHT)
            {
                RECT rcClose = GetCaptionButtonRect(hwnd, CaptionButton::Close);
                RECT rcMax   = GetCaptionButtonRect(hwnd, CaptionButton::Maximize);
                RECT rcMin   = GetCaptionButtonRect(hwnd, CaptionButton::Minimize);

                POINT pt = { x, y };
                if (PtInRect(&rcClose, pt)) cur = CaptionButton::Close;
                else if (PtInRect(&rcMax, pt)) cur = CaptionButton::Maximize;
                else if (PtInRect(&rcMin, pt)) cur = CaptionButton::Minimize;
            }

            if (prev != cur)
            {
                g_app.hoveredButton = cur;
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }

        case WM_MOUSELEAVE:
        {
            g_app.isTrackingMouse = false;
            if (g_app.hoveredButton != CaptionButton::None)
            {
                g_app.hoveredButton = CaptionButton::None;
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }

        case WM_LBUTTONUP:
        {
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            RECT rcClose = GetCaptionButtonRect(hwnd, CaptionButton::Close);
            RECT rcMax   = GetCaptionButtonRect(hwnd, CaptionButton::Maximize);
            RECT rcMin   = GetCaptionButtonRect(hwnd, CaptionButton::Minimize);

            if (PtInRect(&rcClose, pt))
            {
                PostMessageW(hwnd, WM_CLOSE, 0, 0);
            }
            else if (PtInRect(&rcMax, pt))
            {
                ShowWindow(hwnd, IsZoomed(hwnd) ? SW_RESTORE : SW_MAXIMIZE);
            }
            else if (PtInRect(&rcMin, pt))
            {
                ShowWindow(hwnd, SW_MINIMIZE);
            }
            else if (pt.y > TITLEBAR_HEIGHT)
            {
                g_app.clickCount++;
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }

        case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE)
            {
                PostMessageW(hwnd, WM_CLOSE, 0, 0);
                return 0;
            }
            return 0;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int width = clientRect.right;
            int height = clientRect.bottom;

            // Double Buffering
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            // 1. Background
            HBRUSH bgBrush = CreateSolidBrush(RGB(15, 17, 23));
            FillRect(memDC, &clientRect, bgBrush);
            DeleteObject(bgBrush);

            // 2. Title Bar
            RECT tbRect = { 0, 0, width, TITLEBAR_HEIGHT };
            HBRUSH tbBrush = CreateSolidBrush(RGB(22, 25, 35));
            FillRect(memDC, &tbRect, tbBrush);
            DeleteObject(tbBrush);

            HPEN tbBorderPen = CreatePen(PS_SOLID, 1, RGB(38, 43, 58));
            HPEN oldPen = (HPEN)SelectObject(memDC, tbBorderPen);
            MoveToEx(memDC, 0, TITLEBAR_HEIGHT - 1, nullptr);
            LineTo(memDC, width, TITLEBAR_HEIGHT - 1);

            // Judul Titlebar
            HFONT hTitleFont = CreateFontW(14, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            HFONT oldFont = (HFONT)SelectObject(memDC, hTitleFont);
            SetBkMode(memDC, TRANSPARENT);
            SetTextColor(memDC, RGB(220, 225, 235));

            // Ikon aksen bulat
            HBRUSH brandBrush = CreateSolidBrush(RGB(0, 166, 255));
            SelectObject(memDC, brandBrush);
            RoundRect(memDC, 14, 12, 32, 30, 8, 8);
            DeleteObject(brandBrush);

            TextOutW(memDC, 42, 12, L"Octanio Win32 Shell — Final Project", 35);

            // Tombol Kontrol
            auto DrawBtn = [&](CaptionButton btn, const wchar_t* sym, bool isClose) {
                RECT rc = GetCaptionButtonRect(hwnd, btn);
                if (g_app.hoveredButton == btn)
                {
                    HBRUSH hHover = CreateSolidBrush(isClose ? RGB(232, 17, 35) : RGB(45, 52, 68));
                    FillRect(memDC, &rc, hHover);
                    DeleteObject(hHover);
                }
                SetTextColor(memDC, RGB(240, 240, 240));
                DrawTextW(memDC, sym, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            };

            DrawBtn(CaptionButton::Minimize, L"—", false);
            DrawBtn(CaptionButton::Maximize, IsZoomed(hwnd) ? L"❐" : L"□", false);
            DrawBtn(CaptionButton::Close, L"✕", true);

            // 3. Hero Dashboard Cards
            HFONT hHeroFont = CreateFontW(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            HFONT hCardTitleFont = CreateFontW(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            HFONT hBodyFont = CreateFontW(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

            SelectObject(memDC, hHeroFont);
            SetTextColor(memDC, RGB(255, 255, 255));
            TextOutW(memDC, 40, 65, L"Selamat Datang di Win32 Native Dashboard!", 42);

            SelectObject(memDC, hBodyFont);
            SetTextColor(memDC, RGB(145, 160, 180));
            TextOutW(memDC, 40, 102, L"Dibangun dengan C++20 murni, Windows Win32 API, DWM Compositor, dan CMake.", 74);

            // Helper Gambar Kartu (Card)
            auto DrawCard = [&](int x, int y, int w, int h, const wchar_t* title, const std::wstring& line1, const std::wstring& line2) {
                RECT cRc = { x, y, x + w, y + h };
                HBRUSH cBrush = CreateSolidBrush(RGB(24, 28, 38));
                HPEN cPen = CreatePen(PS_SOLID, 1, RGB(42, 48, 65));
                SelectObject(memDC, cBrush);
                SelectObject(memDC, cPen);
                RoundRect(memDC, cRc.left, cRc.top, cRc.right, cRc.bottom, 12, 12);

                SelectObject(memDC, hCardTitleFont);
                SetTextColor(memDC, RGB(0, 180, 255));
                TextOutW(memDC, x + 16, y + 16, title, (int)wcslen(title));

                SelectObject(memDC, hBodyFont);
                SetTextColor(memDC, RGB(220, 225, 235));
                TextOutW(memDC, x + 16, y + 46, line1.c_str(), (int)line1.length());
                SetTextColor(memDC, RGB(150, 165, 185));
                TextOutW(memDC, x + 16, y + 70, line2.c_str(), (int)line2.length());

                DeleteObject(cBrush);
                DeleteObject(cPen);
            };

            ULONGLONG uptimeSec = (GetTickCount64() - g_app.startTime) / 1000;
            std::wstring strUptime = L"Uptime: " + std::to_wstring(uptimeSec) + L" detik";
            std::wstring strClicks = L"Interaksi Klik: " + std::to_wstring(g_app.clickCount) + L" kali";

            std::wstring strRam = L"Beban RAM OS: " + std::to_wstring(g_app.memoryUsagePercent) + L"%";
            std::wstring strRamFree = L"Sisa Bebas: " + std::to_wstring(g_app.availPhysMb) + L" MB / " + std::to_wstring(g_app.totalPhysMb) + L" MB";

            std::wstring strDpi = L"DPI: " + std::to_wstring(g_app.currentDpi) + L" (Skala " + std::to_wstring((g_app.currentDpi * 100) / 96) + L"%)";
            std::wstring strRes = L"Ukuran Jendela: " + std::to_wstring(width) + L" x " + std::to_wstring(height);

            int cardW = (width - 120) / 3;
            if (cardW < 200) cardW = 200;
            int cardH = 105;

            DrawCard(40, 140, cardW, cardH, L"⏱️ Statistik Aplikasi", strUptime, strClicks);
            DrawCard(40 + cardW + 20, 140, cardW, cardH, L"💻 Memori Komputer", strRam, strRamFree);
            DrawCard(40 + (cardW * 2) + 40, 140, cardW, cardH, L"🖥️ Monitor & Tampilan", strDpi, strRes);

            // Kotak Informasi Edukasi di Bawah
            RECT bottomCard = { 40, 275, width - 40, height - 40 };
            HBRUSH bBrush = CreateSolidBrush(RGB(20, 24, 32));
            HPEN bPen = CreatePen(PS_SOLID, 1, RGB(38, 44, 60));
            SelectObject(memDC, bBrush);
            SelectObject(memDC, bPen);
            RoundRect(memDC, bottomCard.left, bottomCard.top, bottomCard.right, bottomCard.bottom, 14, 14);

            SelectObject(memDC, hCardTitleFont);
            SetTextColor(memDC, RGB(46, 204, 113));
            TextOutW(memDC, 60, 295, L"✨ Arsitektur Teknis yang Aktif pada Project Ini:", 49);

            SelectObject(memDC, hBodyFont);
            SetTextColor(memDC, RGB(180, 195, 215));
            RECT textBlock = { 60, 330, width - 60, height - 55 };
            const wchar_t techDetails[] =
                L"1. Native Frameless: Frame default Windows dihilangkan melalui WM_NCCALCSIZE.\n"
                L"2. Drop Shadow Asli: DwmExtendFrameIntoClientArea menyalakan bayangan DWM Windows 11.\n"
                L"3. Hit Testing: WM_NCHITTEST mendeteksi area judul sebagai HTCAPTION sehingga dragging & snapping berjalan native.\n"
                L"4. Kontrol Kustom: Tombol Minimize, Maximize/Restore, dan Close dengan efek animasi hover & active.\n"
                L"5. Per-Monitor V2 DPI Awareness: Responsif terhadap perubahan skala monitor via WM_DPICHANGED.\n"
                L"6. Zero Flicker: Render double buffering menggunakan Memory DC dan BitBlt secepat kilat (60 FPS).\n"
                L"7. Windows Resource: Metadata program tersemat resmi ke dalam file exe melalui app.rc.\n\n"
                L"💡 Coba klik di mana saja pada area konten di bawah untuk menambah penghitung interaksi!";
            DrawTextW(memDC, techDetails, -1, &textBlock, DT_LEFT | DT_WORDBREAK);

            // Selesai & Blit ke layar
            BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

            SelectObject(memDC, oldFont);
            SelectObject(memDC, oldPen);
            SelectObject(memDC, oldBitmap);

            DeleteObject(hTitleFont);
            DeleteObject(hHeroFont);
            DeleteObject(hCardTitleFont);
            DeleteObject(hBodyFont);
            DeleteObject(tbBorderPen);
            DeleteObject(bBrush);
            DeleteObject(bPen);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

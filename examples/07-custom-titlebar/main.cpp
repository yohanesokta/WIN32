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

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

// Tinggi title bar kustom dalam piksel
constexpr int TITLEBAR_HEIGHT = 40;
constexpr int BUTTON_WIDTH    = 46;
constexpr int BORDER_PADDING  = 8; // Ketebalan area sensor resize di tepi jendela

enum class HoveredButton {
    None,
    Minimize,
    Maximize,
    Close
};

struct WindowState {
    HoveredButton hoveredBtn = HoveredButton::None;
    bool isTrackingMouse = false;
};

static WindowState g_state;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Dapatkan koordinat tombol-tombol title bar
RECT GetButtonRect(HWND hwnd, HoveredButton btn)
{
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int windowWidth = clientRect.right;

    RECT rc = { 0, 0, 0, TITLEBAR_HEIGHT };
    switch (btn)
    {
        case HoveredButton::Close:
            rc.left  = windowWidth - BUTTON_WIDTH;
            rc.right = windowWidth;
            break;
        case HoveredButton::Maximize:
            rc.left  = windowWidth - (BUTTON_WIDTH * 2);
            rc.right = windowWidth - BUTTON_WIDTH;
            break;
        case HoveredButton::Minimize:
            rc.left  = windowWidth - (BUTTON_WIDTH * 3);
            rc.right = windowWidth - (BUTTON_WIDTH * 2);
            break;
        default:
            break;
    }
    return rc;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    const wchar_t CLASS_NAME[] = L"Win32Tutorial_CustomTitlebarClass";

    WNDCLASSEXW wc = {};
    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr; // Kita menggambar seluruh jendela sendiri
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassExW(&wc)) return 0;

    // Tetap gunakan WS_THICKFRAME agar animasi maximize, snap layout, dan resize tetap berfungsi
    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Octanio Win32 Shell — Custom Title Bar Modern",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        900, 600,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) return 0;

    // 1. Terapkan dark mode DWM
    BOOL dark = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

    // 2. Perluas frame DWM sebanyak 1 piksel ke client area agar Windows tetap merender drop shadow asli
    MARGINS margins = { 0, 0, 1, 0 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    // Paksa kalkulasi ulang ukuran frame non-client
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
        SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

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
        case WM_NCCALCSIZE:
        {
            // Jika wParam TRUE, Windows bertanya berapa ukuran client area baru.
            // Dengan mengembalikan 0 tanpa memanggil DefWindowProcW, kita menghilangkan
            // seluruh frame dan title bar bawaan Windows secara mulus!
            if (wParam == TRUE)
            {
                return 0;
            }
            break;
        }

        case WM_NCHITTEST:
        {
            // Deteksi di mana kursor berada di layar (Hit Testing)
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ScreenToClient(hwnd, &pt);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int width = clientRect.right;
            int height = clientRect.bottom;

            // Jika window tidak sedang di-maximize, tangani resize di tepi-tepi border
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

            // Jika kursor berada di area tombol kontrol (Min, Max, Close), serahkan ke client
            // agar tombol kita bisa menangkap efek hover dan klik mouse
            int buttonZoneStart = width - (BUTTON_WIDTH * 3);
            if (pt.y >= 0 && pt.y < TITLEBAR_HEIGHT && pt.x >= buttonZoneStart)
            {
                return HTCLIENT;
            }

            // Jika kursor berada di sisa area title bar, kembalikan HTCAPTION!
            // Windows akan secara otomatis menangani dragging, snapping, dan double-click to maximize!
            if (pt.y >= 0 && pt.y < TITLEBAR_HEIGHT)
            {
                return HTCAPTION;
            }

            return HTCLIENT;
        }

        case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            // Pasang pelacakan mouse leave jika belum aktif
            if (!g_state.isTrackingMouse)
            {
                TRACKMOUSEEVENT tme = {};
                tme.cbSize = sizeof(TRACKMOUSEEVENT);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = hwnd;
                TrackMouseEvent(&tme);
                g_state.isTrackingMouse = true;
            }

            HoveredButton prev = g_state.hoveredBtn;
            HoveredButton current = HoveredButton::None;

            if (y >= 0 && y < TITLEBAR_HEIGHT)
            {
                RECT rcClose = GetButtonRect(hwnd, HoveredButton::Close);
                RECT rcMax   = GetButtonRect(hwnd, HoveredButton::Maximize);
                RECT rcMin   = GetButtonRect(hwnd, HoveredButton::Minimize);

                POINT pt = { x, y };
                if (PtInRect(&rcClose, pt)) current = HoveredButton::Close;
                else if (PtInRect(&rcMax, pt)) current = HoveredButton::Maximize;
                else if (PtInRect(&rcMin, pt)) current = HoveredButton::Minimize;
            }

            if (prev != current)
            {
                g_state.hoveredBtn = current;
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }

        case WM_MOUSELEAVE:
        {
            g_state.isTrackingMouse = false;
            if (g_state.hoveredBtn != HoveredButton::None)
            {
                g_state.hoveredBtn = HoveredButton::None;
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }

        case WM_LBUTTONUP:
        {
            // Eksekusi aksi tombol saat klik kiri dilepas
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            POINT pt = { x, y };

            RECT rcClose = GetButtonRect(hwnd, HoveredButton::Close);
            RECT rcMax   = GetButtonRect(hwnd, HoveredButton::Maximize);
            RECT rcMin   = GetButtonRect(hwnd, HoveredButton::Minimize);

            if (PtInRect(&rcClose, pt))
            {
                PostMessageW(hwnd, WM_CLOSE, 0, 0);
            }
            else if (PtInRect(&rcMax, pt))
            {
                if (IsZoomed(hwnd))
                    ShowWindow(hwnd, SW_RESTORE);
                else
                    ShowWindow(hwnd, SW_MAXIMIZE);
            }
            else if (PtInRect(&rcMin, pt))
            {
                ShowWindow(hwnd, SW_MINIMIZE);
            }
            return 0;
        }

        case WM_ERASEBKGND:
            return 1; // Double buffering aktif

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int width = clientRect.right;
            int height = clientRect.bottom;

            // === Double Buffering ===
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            // 1. Gambar Latar Belakang Konten Utama (Dark Navy Slate)
            HBRUSH bgBrush = CreateSolidBrush(RGB(18, 20, 26));
            FillRect(memDC, &clientRect, bgBrush);
            DeleteObject(bgBrush);

            // 2. Gambar Custom Title Bar
            RECT titlebarRect = { 0, 0, width, TITLEBAR_HEIGHT };
            HBRUSH tbBrush = CreateSolidBrush(RGB(26, 29, 38));
            FillRect(memDC, &titlebarRect, tbBrush);
            DeleteObject(tbBrush);

            // Garis batas halus di bawah title bar
            HPEN borderPen = CreatePen(PS_SOLID, 1, RGB(42, 46, 58));
            HPEN oldPen = (HPEN)SelectObject(memDC, borderPen);
            MoveToEx(memDC, 0, TITLEBAR_HEIGHT - 1, nullptr);
            LineTo(memDC, width, TITLEBAR_HEIGHT - 1);

            // 3. Judul Aplikasi di Title Bar
            HFONT hTitleFont = CreateFontW(15, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            HFONT oldFont = (HFONT)SelectObject(memDC, hTitleFont);
            SetBkMode(memDC, TRANSPARENT);
            SetTextColor(memDC, RGB(225, 230, 240));

            // Ikon mini kustom di sebelah kiri judul
            HBRUSH iconBrush = CreateSolidBrush(RGB(0, 160, 255));
            SelectObject(memDC, iconBrush);
            RoundRect(memDC, 14, 11, 32, 29, 6, 6);
            DeleteObject(iconBrush);

            TextOutW(memDC, 42, 11, L"Octanio Win32 Shell — Custom Modern Titlebar", 44);

            // 4. Gambar Tombol Kontrol (Minimize, Maximize, Close)
            auto DrawButton = [&](HoveredButton btn, const wchar_t* symbol, bool isClose) {
                RECT rc = GetButtonRect(hwnd, btn);
                if (g_state.hoveredBtn == btn)
                {
                    HBRUSH hHover = CreateSolidBrush(isClose ? RGB(232, 17, 35) : RGB(48, 54, 70));
                    FillRect(memDC, &rc, hHover);
                    DeleteObject(hHover);
                }

                SetTextColor(memDC, RGB(240, 240, 240));
                DrawTextW(memDC, symbol, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            };

            DrawButton(HoveredButton::Minimize, L"—", false);
            DrawButton(HoveredButton::Maximize, IsZoomed(hwnd) ? L"❐" : L"□", false);
            DrawButton(HoveredButton::Close, L"✕", true);

            // 5. Gambar Isi Konten di Tengah
            HFONT hHeroFont = CreateFontW(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            HFONT hBodyFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

            SelectObject(memDC, hHeroFont);
            SetTextColor(memDC, RGB(255, 255, 255));
            TextOutW(memDC, 50, 80, L"Aplikasi Desktop Modern Tanpa Title Bar Kuno", 45);

            SelectObject(memDC, hBodyFont);
            SetTextColor(memDC, RGB(160, 175, 195));

            RECT descRect = { 50, 130, width - 50, height - 50 };
            const wchar_t contentText[] =
                L"Selamat! Kamu telah berhasil membuat antarmuka jendela frameless modern menggunakan C++ murni.\n\n"
                L"Fitur-fitur yang berjalan secara native di jendela ini:\n"
                L"✓ Custom Title Bar terintegrasi sempurna dengan warna tema gelap.\n"
                L"✓ Drop Shadow asli Windows 11 tetap aktif berkat DwmExtendFrameIntoClientArea.\n"
                L"✓ Geser jendela (Dragging) berfungsi mulus dengan mengembalikan HTCAPTION pada WM_NCHITTEST.\n"
                L"✓ Double-click title bar otomatis melakukan maximize dan restore.\n"
                L"✓ Ubah ukuran (Resizing) tetap bisa dilakukan di seluruh tepi dan sudut jendela.\n"
                L"✓ Tombol Minimize, Maximize, dan Close memiliki efek hover yang interaktif.\n"
                L"✓ Konsumsi RAM di bawah 10 MB dan biner hanya puluhan kilobyte tanpa runtime Electron!";

            DrawTextW(memDC, contentText, -1, &descRect, DT_LEFT | DT_WORDBREAK);

            // Cleanup & BitBlt ke layar
            BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

            SelectObject(memDC, oldFont);
            SelectObject(memDC, oldPen);
            SelectObject(memDC, oldBitmap);

            DeleteObject(hTitleFont);
            DeleteObject(hHeroFont);
            DeleteObject(hBodyFont);
            DeleteObject(borderPen);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

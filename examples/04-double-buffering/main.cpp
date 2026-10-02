#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <string>

// State animasi bola memantul
struct BallState {
    float x = 200.0f;
    float y = 150.0f;
    float vx = 6.0f;
    float vy = 5.0f;
    int radius = 30;
    bool useDoubleBuffering = true;
};

static BallState g_ball;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Win32Tutorial_DoubleBufferClass";

    WNDCLASSEXW wc = {};
    wc.cbSize        = sizeof(WNDCLASSEXW);
    // CS_HREDRAW & CS_VREDRAW sengaja tidak digunakan untuk mencegah redraw berlebih
    wc.style         = 0;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassExW(&wc)) return 0;

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Tutorial Win32: Animasi Halus Tanpa Flicker (Double Buffering)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) return 0;

    // Pasang timer dengan interval ~16ms (~60 Frame Per Detik)
    SetTimer(hwnd, 1, 16, nullptr);

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

// Fungsi bantu untuk menggambar isi frame animasi ke suatu Device Context
void RenderScene(HDC hdc, int width, int height)
{
    // 1. Gambar latar belakang gelap modern
    HBRUSH bgBrush = CreateSolidBrush(RGB(24, 28, 36));
    RECT bgRect = { 0, 0, width, height };
    FillRect(hdc, &bgRect, bgBrush);
    DeleteObject(bgBrush);

    // 2. Gambar teks informasi
    SetTextColor(hdc, RGB(220, 230, 242));
    SetBkMode(hdc, TRANSPARENT);

    std::wstring modeStr = g_ball.useDoubleBuffering ? 
        L"MODE: Double Buffering [AKTIF] (Halus, 60 FPS, Tanpa Flicker)" :
        L"MODE: Double Buffering [NONAKTIF] (Layar Berkedip / Flickering)";

    TextOutW(hdc, 25, 20, modeStr.c_str(), (int)modeStr.length());
    
    std::wstring hint = L"Tekan tombol [SPASI] di keyboard untuk membandingkan perbedaannya!";
    SetTextColor(hdc, RGB(140, 160, 180));
    TextOutW(hdc, 25, 50, hint.c_str(), (int)hint.length());

    // 3. Gambar bola yang bergerak memantul
    HBRUSH ballBrush = CreateSolidBrush(RGB(0, 180, 255));
    HPEN ballPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, ballBrush);
    HPEN oldPen = (HPEN)SelectObject(hdc, ballPen);

    Ellipse(
        hdc,
        (int)g_ball.x - g_ball.radius,
        (int)g_ball.y - g_ball.radius,
        (int)g_ball.x + g_ball.radius,
        (int)g_ball.y + g_ball.radius
    );

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(ballBrush);
    DeleteObject(ballPen);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_TIMER:
        {
            // Perbarui fisika posisi bola
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int width = clientRect.right;
            int height = clientRect.bottom;

            g_ball.x += g_ball.vx;
            g_ball.y += g_ball.vy;

            // Pantulan dinding kiri/kanan
            if (g_ball.x - g_ball.radius < 0) {
                g_ball.x = (float)g_ball.radius;
                g_ball.vx = -g_ball.vx;
            } else if (g_ball.x + g_ball.radius > width) {
                g_ball.x = (float)(width - g_ball.radius);
                g_ball.vx = -g_ball.vx;
            }

            // Pantulan dinding atas/bawah
            if (g_ball.y - g_ball.radius < 80) { // Batasi di bawah area teks
                g_ball.y = (float)(80 + g_ball.radius);
                g_ball.vy = -g_ball.vy;
            } else if (g_ball.y + g_ball.radius > height) {
                g_ball.y = (float)(height - g_ball.radius);
                g_ball.vy = -g_ball.vy;
            }

            // Minta gambar ulang
            InvalidateRect(hwnd, nullptr, !g_ball.useDoubleBuffering);
            return 0;
        }

        case WM_KEYDOWN:
        {
            if (wParam == VK_SPACE)
            {
                // Toggle mode Double Buffering
                g_ball.useDoubleBuffering = !g_ball.useDoubleBuffering;
            }
            return 0;
        }

        case WM_ERASEBKGND:
        {
            // Jika Double Buffering aktif, beri tahu Windows bahwa kita menangani penghapusan background sendiri
            // Ini mencegah Windows menggambar kotak putih di setiap frame yang menjadi penyebab flicker!
            if (g_ball.useDoubleBuffering)
            {
                return 1; // 1 = Berhasil ditangani
            }
            break; // Jika tidak, gunakan default behavior Windows
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rc;
            GetClientRect(hwnd, &rc);
            int width = rc.right;
            int height = rc.bottom;

            if (g_ball.useDoubleBuffering)
            {
                // === TEKNIK DOUBLE BUFFERING ===
                // 1. Buat Memory DC (kanvas memori bayangan off-screen)
                HDC memDC = CreateCompatibleDC(hdc);

                // 2. Buat bitmap kompatibel dengan ukuran client area
                HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);

                // 3. Pasangkan bitmap ke Memory DC
                HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

                // 4. Gambar seluruh elemen ke kanvas memori (di latar belakang tanpa terlihat layar)
                RenderScene(memDC, width, height);

                // 5. Transfer gambar utuh dari memori ke layar monitor dalam 1 operasi super cepat (BitBlt)
                BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

                // 6. Bersihkan resource memori
                SelectObject(memDC, oldBitmap);
                DeleteObject(memBitmap);
                DeleteDC(memDC);
            }
            else
            {
                // Menggambar langsung ke layar monitor (Menyebabkan flicker hebat)
                RenderScene(hdc, width, height);
            }

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

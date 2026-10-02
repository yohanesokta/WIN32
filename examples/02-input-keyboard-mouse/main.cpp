#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <windowsx.h> // Header untuk macro GET_X_LPARAM dan GET_Y_LPARAM
#include <string>

// Struktur penyimpan status interaksi input pengguna
struct InputState {
    int mouseX = 100;
    int mouseY = 100;
    bool isMouseDown = false;
    std::wstring lastKeyName = L"Belum ada tombol ditekan";
    std::wstring lastChar = L"-";
    int pressCount = 0;
};

static InputState g_inputState;

// Forward declaration Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Win32Tutorial_InputWindowClass";

    WNDCLASSEXW wc = {};
    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassExW(&wc))
    {
        MessageBoxW(nullptr, L"Gagal mendaftarkan Window Class!", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Tutorial Win32: Demo Input Keyboard & Mouse",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr)
    {
        MessageBoxW(nullptr, L"Gagal membuat Window!", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message Loop
    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg); // Menerjemahkan WM_KEYDOWN ke WM_CHAR
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_MOUSEMOVE:
        {
            // Ambil koordinat kursor mouse dari lParam menggunakan macro windowsx.h
            g_inputState.mouseX = GET_X_LPARAM(lParam);
            g_inputState.mouseY = GET_Y_LPARAM(lParam);

            // Minta Windows untuk menggambar ulang jendela (memicu WM_PAINT)
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            g_inputState.isMouseDown = true;
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }

        case WM_LBUTTONUP:
        {
            g_inputState.isMouseDown = false;
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }

        case WM_KEYDOWN:
        {
            // wParam berisi Virtual Key Code (VK_*)
            g_inputState.pressCount++;

            std::wstring keyName;
            switch (wParam)
            {
                case VK_SPACE:   keyName = L"Space"; break;
                case VK_RETURN:  keyName = L"Enter"; break;
                case VK_ESCAPE:  keyName = L"Escape"; break;
                case VK_BACK:    keyName = L"Backspace"; break;
                case VK_TAB:     keyName = L"Tab"; break;
                case VK_LEFT:    keyName = L"Panah Kiri (Left)"; break;
                case VK_RIGHT:   keyName = L"Panah Kanan (Right)"; break;
                case VK_UP:      keyName = L"Panah Atas (Up)"; break;
                case VK_DOWN:    keyName = L"Panah Bawah (Down)"; break;
                default:
                    // Jika huruf atau angka biasa (A-Z, 0-9)
                    if (wParam >= 'A' && wParam <= 'Z')
                    {
                        keyName = L"Huruf ";
                        keyName += (wchar_t)wParam;
                    }
                    else if (wParam >= '0' && wParam <= '9')
                    {
                        keyName = L"Angka ";
                        keyName += (wchar_t)wParam;
                    }
                    else
                    {
                        keyName = L"Kode VK: " + std::to_wstring(wParam);
                    }
                    break;
            }

            g_inputState.lastKeyName = keyName;
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }

        case WM_CHAR:
        {
            // wParam berisi karakter Unicode hasil terjemahan TranslateMessage
            wchar_t ch = (wchar_t)wParam;
            if (ch >= 32) // Abaikan karakter kontrol tak terlihat (Enter/Tab/Esc)
            {
                g_inputState.lastChar = std::wstring(1, ch);
            }
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // 1. Tampilkan teks informasi status di pojok kiri atas
            std::wstring info1 = L"=== STATUS INPUT KEYBOARD & MOUSE ===";
            std::wstring info2 = L"Koordinat Mouse: X = " + std::to_wstring(g_inputState.mouseX) +
                                 L", Y = " + std::to_wstring(g_inputState.mouseY);
            std::wstring info3 = L"Tombol Kiri Mouse: " +
                                 std::wstring(g_inputState.isMouseDown ? L"SEDANG DITEKAN (HOLD)" : L"Dilepas");
            std::wstring info4 = L"Tombol Terakhir: " + g_inputState.lastKeyName;
            std::wstring info5 = L"Karakter WM_CHAR: '" + g_inputState.lastChar + L"'";
            std::wstring info6 = L"Total Ketukan Keyboard: " + std::to_wstring(g_inputState.pressCount);
            std::wstring info7 = L"(Coba gerakkan mouse, klik kiri, atau ketik tombol apa saja di keyboard)";

            TextOutW(hdc, 20, 20,  info1.c_str(), (int)info1.length());
            TextOutW(hdc, 20, 50,  info2.c_str(), (int)info2.length());
            TextOutW(hdc, 20, 75,  info3.c_str(), (int)info3.length());
            TextOutW(hdc, 20, 100, info4.c_str(), (int)info4.length());
            TextOutW(hdc, 20, 125, info5.c_str(), (int)info5.length());
            TextOutW(hdc, 20, 150, info6.c_str(), (int)info6.length());
            TextOutW(hdc, 20, 190, info7.c_str(), (int)info7.length());

            // 2. Gambar lingkaran indikator kursor mouse
            // Warna merah jika mouse ditekan, biru muda jika hanya digerakkan
            COLORREF circleColor = g_inputState.isMouseDown ? RGB(230, 50, 50) : RGB(40, 140, 240);
            HBRUSH hBrush = CreateSolidBrush(circleColor);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            int radius = g_inputState.isMouseDown ? 25 : 15;
            Ellipse(
                hdc,
                g_inputState.mouseX - radius,
                g_inputState.mouseY - radius,
                g_inputState.mouseX + radius,
                g_inputState.mouseY + radius
            );

            // Kembalikan brush lama dan hapus brush baru untuk mencegah GDI resource leak
            SelectObject(hdc, hOldBrush);
            DeleteObject(hBrush);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Win32Tutorial_GDIDrawingClass";

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
        L"Tutorial Win32: Menggambar Bentuk & Warna dengan GDI",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        850, 650,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) return 0;

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
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // 1. Gambar Judul Menggunakan Font Kustom
            HFONT hTitleFont = CreateFontW(
                26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                L"Segoe UI"
            );
            HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);

            SetTextColor(hdc, RGB(20, 30, 80));
            SetBkMode(hdc, TRANSPARENT); // Background teks transparan
            TextOutW(hdc, 30, 25, L"Galeri Menggambar GDI (Graphics Device Interface)", 49);

            // 2. Gambar Garis Pembatas (Pena / HPEN)
            HPEN hBluePen = CreatePen(PS_SOLID, 3, RGB(0, 120, 215));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hBluePen);

            MoveToEx(hdc, 30, 65, nullptr); // Titik awal garis
            LineTo(hdc, 800, 65);            // Tarik garis ke titik akhir

            // 3. Menggambar Persegi Berwarna (Solid Brush)
            HBRUSH hOrangeBrush = CreateSolidBrush(RGB(255, 140, 0));
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hOrangeBrush);

            Rectangle(hdc, 30, 90, 230, 240);

            // 4. Menggambar Persegi Sudut Tumpul (Rounded Rectangle)
            HBRUSH hGreenBrush = CreateSolidBrush(RGB(46, 204, 113));
            SelectObject(hdc, hGreenBrush);
            RoundRect(hdc, 260, 90, 460, 240, 30, 30); // 30, 30 = radius kelengkungan sudut

            // 5. Menggambar Lingkaran / Elips
            HBRUSH hPurpleBrush = CreateSolidBrush(RGB(155, 89, 182));
            SelectObject(hdc, hPurpleBrush);
            Ellipse(hdc, 490, 90, 640, 240);

            // 6. Label Keterangan di Bawah Bentuk
            SelectObject(hdc, hOldFont); // Kembalikan ke font sistem biasa
            SetTextColor(hdc, RGB(80, 80, 80));
            TextOutW(hdc, 75, 255, L"Rectangle", 9);
            TextOutW(hdc, 305, 255, L"Rounded Rect", 12);
            TextOutW(hdc, 535, 255, L"Ellipse", 7);

            // 7. Kotak Info Edukasi (DrawTextW dengan Word Wrap)
            RECT infoRect = { 30, 310, 800, 560 };
            HBRUSH hBoxBrush = CreateSolidBrush(RGB(245, 247, 250));
            HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(210, 215, 225));
            SelectObject(hdc, hBoxBrush);
            SelectObject(hdc, hBorderPen);
            RoundRect(hdc, infoRect.left, infoRect.top, infoRect.right, infoRect.bottom, 12, 12);

            RECT textRect = { 50, 330, 780, 540 };
            const wchar_t notes[] =
                L"Catatan Penting Pengelolaan Memori GDI:\n\n"
                L"1. Setiap kali membuat objek GDI seperti CreateSolidBrush, CreatePen, atau CreateFontW, "
                L"objek tersebut dialokasikan di dalam memori GDI kernel Windows.\n"
                L"2. Setelah selesai digunakan, kamu WAJIB mengembalikan objek sebelumnya dengan SelectObject "
                L"dan menghapus objek buatanmu dengan DeleteObject.\n"
                L"3. Jika kamu lupa menghapusnya, akan terjadi GDI Resource Leak yang dapat membuat tampilan Windows "
                L"rusak setelah beberapa waktu!";

            SetTextColor(hdc, RGB(40, 40, 40));
            DrawTextW(hdc, notes, -1, &textRect, DT_LEFT | DT_WORDBREAK);

            // 8. Bersihkan Seluruh Objek GDI (Cleanup)
            SelectObject(hdc, hOldPen);
            SelectObject(hdc, hOldBrush);

            DeleteObject(hTitleFont);
            DeleteObject(hBluePen);
            DeleteObject(hOrangeBrush);
            DeleteObject(hGreenBrush);
            DeleteObject(hPurpleBrush);
            DeleteObject(hBoxBrush);
            DeleteObject(hBorderPen);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

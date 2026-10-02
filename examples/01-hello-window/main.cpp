#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>

// Forward declaration untuk Window Procedure (fungsi pemroses event)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Entry point untuk aplikasi GUI Windows (Unicode)
int WINAPI wWinMain(
    HINSTANCE hInstance,     // Handle ke instance aplikasi saat ini
    HINSTANCE hPrevInstance, // Tidak lagi digunakan (selalu NULL di Win32 modern)
    PWSTR     pCmdLine,      // Argumen command line dalam bentuk wide string
    int       nCmdShow       // Instruksi cara menampilkan window (maximize, normal, dsb.)
)
{
    // Nama class window yang akan didaftarkan ke sistem operasi Windows
    const wchar_t CLASS_NAME[] = L"Win32Tutorial_HelloWindowClass";

    // 1. Daftarkan Window Class menggunakan struktur WNDCLASSEXW
    WNDCLASSEXW wc = {};
    wc.cbSize        = sizeof(WNDCLASSEXW);                   // Ukuran struktur dalam bytes
    wc.style         = CS_HREDRAW | CS_VREDRAW;               // Gambar ulang jika ukuran window berubah
    wc.lpfnWndProc   = WindowProc;                            // Pointer ke fungsi Window Procedure kita
    wc.hInstance     = hInstance;                             // Handle instance aplikasi
    wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);       // Cursor mouse panah standar
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);            // Warna background putih standar sistem
    wc.lpszClassName = CLASS_NAME;                            // Nama unik untuk mengenali class ini

    if (!RegisterClassExW(&wc))
    {
        MessageBoxW(
            nullptr,
            L"Gagal mendaftarkan Window Class!",
            L"Error Win32",
            MB_ICONERROR | MB_OK
        );
        return 0;
    }

    // 2. Buat Window menggunakan CreateWindowExW
    HWND hwnd = CreateWindowExW(
        0,                               // Style ekstra (extended style)
        CLASS_NAME,                      // Nama class yang sudah didaftarkan sebelumnya
        L"Halo Win32 Indonesia!",        // Judul pada title bar window
        WS_OVERLAPPEDWINDOW,             // Style window standar (title bar, border, minimize, maximize, close)
        CW_USEDEFAULT, CW_USEDEFAULT,    // Posisi X dan Y default yang ditentukan oleh Windows
        800, 600,                        // Lebar dan tinggi window (800x600 piksel)
        nullptr,                         // Handle parent window (nullptr = tidak memiliki parent / top-level)
        nullptr,                         // Handle menu (nullptr = tidak menggunakan menu bar)
        hInstance,                       // Instance aplikasi pembuat window
        nullptr                          // Data pointer tambahan (lParam pada WM_CREATE)
    );

    if (hwnd == nullptr)
    {
        MessageBoxW(
            nullptr,
            L"Gagal membuat Window!",
            L"Error Win32",
            MB_ICONERROR | MB_OK
        );
        return 0;
    }

    // 3. Tampilkan dan lakukan render awal window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 4. Message Loop: Jantung dari setiap aplikasi desktop Win32
    // Mengambil antrean pesan dari sistem operasi dan meneruskannya ke WindowProc
    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);  // Menerjemahkan input keyboard virtual key ke character messages (WM_CHAR)
        DispatchMessageW(&msg);  // Mengirim pesan ke WindowProc milik window terkait
    }

    return (int)msg.wParam;
}

// Window Procedure: Fungsi callback tempat seluruh pesan/event Windows ditangani
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            // Tombol close ditekan dan window dihancurkan.
            // Kirim WM_QUIT ke message queue agar loop GetMessageW berhenti.
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Teks sederhana di tengah window
            const wchar_t text[] = L"Selamat Datang di Tutorial Win32 & DWM Indonesia!";
            RECT rect;
            GetClientRect(hwnd, &rect);
            DrawTextW(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            EndPaint(hwnd, &ps);
            return 0;
        }
    }

    // Serahkan penanganan event lainnya ke fungsi default Windows
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

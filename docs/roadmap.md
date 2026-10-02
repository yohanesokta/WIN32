# Roadmap Pembelajaran Win32 + DWM Indonesia

Roadmap ini dirancang secara sistematis agar siapa pun yang baru belajar dapat mengikuti alur materi dengan jelas, bertahap, dan tidak kewalahan. Kita mulai dari konsep paling dasar di sistem operasi Windows sebelum melangkah ke manipulasi grafis dan Desktop Window Manager (DWM).

```text
┌────────────────────────────────────────────────────────┐
│ 00. Persiapan Toolchain, Compiler MSVC & CMake         │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│ 01. C++ Dasar, Tipe Data Windows & Unicode UTF-16      │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│ 02. Pondasi Win32, wWinMain & Window Pertama           │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│ 03. Message Loop & Window Procedure (WndProc)          │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│ 04. Input Keyboard & Mouse Tracking                    │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│ 05. Rendering Grafis GDI & Double Buffering 60 FPS     │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│ 06. Per-Monitor V2 DPI Awareness                       │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│ 07. Desktop Window Manager (DWM) & Immersive Dark Mode │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│ 08. Custom Title Bar & Frameless Window Modern         │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│ 09. Project Akhir: Octanio Win32 Shell                 │
└────────────────────────────────────────────────────────┘
```

---

## Daftar Modul Tutorial

### 00. Persiapan & Lingkungan Pengembangan
* [Kenapa Belajar Win32 & DWM?](/00-persiapan/01-kenapa-win32) — Mengenal arsitektur OS, keunggulan performa, RAM < 10 MB.
* [Setup Toolchain (MSVC, Windows SDK, Git)](/00-persiapan/02-setup-toolchain) — Memasang compiler tanpa beban IDE berat.
* [Mengenal CMake & Build System Terminal](/00-persiapan/03-cmake-dan-ninja) — Perbedaan Subsystem Windows vs Console, flag `WIN32`.

### 01. C++ Dasar yang Diperlukan Win32
* [Tipe Data Primitif Windows](/01-cpp-dasar/01-tipe-data-windows) — Bedah `DWORD`, `BOOL` vs `bool`, `WPARAM`, `LPARAM`, `LRESULT`.
* [Unicode & String di Windows](/01-cpp-dasar/02-unicode-dan-string) — Mengapa wajib memakai akhiran `W`, `wchar_t`, literal `L""`, dan `LPCWSTR`.
* [Handle, Pointer & Konvensi Panggilan](/01-cpp-dasar/03-handle-dan-pointer) — Analogi tiket penitipan barang, `HWND`, `HINSTANCE`, `HDC`, dan `GetLastError`.

### 02. Pondasi Win32 & Window Pertama
* [Arsitektur Aplikasi Desktop Windows](/02-win32-dasar/01-arsitektur-win32) — 4 fase utama siklus hidup aplikasi GUI.
* [Entry Point Aplikasi: wWinMain](/02-win32-dasar/02-entry-point-wWinMain) — Membedah 4 parameter fungsi `wWinMain`.
* [Mendaftarkan Window Class (WNDCLASSEXW)](/02-win32-dasar/03-window-class) — Cetak biru jendela dan fungsi callback `WindowProc`.
* [Membuat & Menampilkan Window](/02-win32-dasar/04-membuat-window) — Parameter `CreateWindowExW`, style `WS_OVERLAPPEDWINDOW`, dan `ShowWindow`.

### 03. Message Loop & Event Handling
* [Konsep Event-Driven di Windows](/03-message-loop/01-konsep-event-driven) — Alur hardware interrupt hingga menjadi pesan di aplikasi.
* [Bedah Tuntas Message Loop](/03-message-loop/02-message-loop-detail) — Cara kerja `GetMessageW > 0`, `TranslateMessage`, dan `DispatchMessageW`.
* [Anatomi Window Procedure (WndProc)](/03-message-loop/03-wndproc-anatomi) — Blok `switch (uMsg)`, peran krusial `DefWindowProcW`, dan mencegah zombie process.

### 04. Input Keyboard & Mouse
* [Keyboard Input](/04-input/01-keyboard) — Menangkap `WM_KEYDOWN`, `WM_KEYUP`, `WM_CHAR`, dan Virtual Key Codes.
* [Mouse Input & Tracking](/04-input/02-mouse) — Membaca koordinat mouse (`GET_X_LPARAM`, `GET_Y_LPARAM`), klik kiri/kanan, dan `SetCapture`.

### 05. Rendering & Grafis (GDI)
* [WM_PAINT & Device Context (HDC)](/05-rendering/01-hdc-dan-paint) — Siklus gambar ulang, pasangan wajib `BeginPaint` / `EndPaint`, dan `InvalidateRect`.
* [Menggambar Bentuk, Teks & Warna](/05-rendering/02-gdi-dasar) — Pena (`HPEN`), Kuas (`HBRUSH`), Font Segoe UI, dan pencegahan GDI memory leak.
* [Timer & Animasi di Win32](/05-rendering/03-timer-dan-animasi) — Membuat pergerakan dinamis via `SetTimer`, `KillTimer`, dan pesan `WM_TIMER`.
* [Double Buffering (Mencegah Flicker)](/05-rendering/04-double-buffering) — Menggambar off-screen menggunakan Memory DC dan `BitBlt` 60 FPS.

### 06. DPI Awareness & Multi-Monitor
* [Per-Monitor V2 DPI Awareness](/06-dpi/01-per-monitor-dpi) — Solusi agar tampilan teks dan UI tidak buram di layar 4K/High-DPI dan penanganan `WM_DPICHANGED`.

### 07. Desktop Window Manager (DWM)
* [Pengenalan Komposisi DWM](/07-dwm/01-pengenalan-dwm) — Cara kerja GPU compositor di Windows modern.
* [Client Area vs Non-Client Area](/07-dwm/02-client-vs-nonclient) — Memahami batas pembagian wilayah jendela.
* [DwmExtendFrameIntoClientArea](/07-dwm/03-dwm-extend-frame) — Menghidupkan kembali native drop shadow Windows 11.
* [Dark Mode & Atribut Modern Windows 11](/07-dwm/04-dark-mode-dan-backdrop) — `DWMWA_USE_IMMERSIVE_DARK_MODE`, warna caption kustom, dan sudut membulat (*rounded corners*).

### 08. Custom Title Bar & Frameless Window
* [Menghilangkan Frame Default Windows](/08-custom-titlebar/01-frameless-window) — Menggunakan `WM_NCCALCSIZE` tanpa merusak fitur native.
* [Hit Testing (WM_NCHITTEST)](/08-custom-titlebar/02-hit-testing) — Mengembalikan `HTCAPTION` untuk dragging native dan border resize di seluruh tepi.
* [Tombol Kontrol Kustom (Min, Max, Close)](/08-custom-titlebar/03-caption-buttons) — Tata letak tombol, efek hover mouse, dan logika maximize/restore.
* [Snap Layouts & Window Resizing](/08-custom-titlebar/04-snap-layouts-resizing) — Integrasi `HTMAXBUTTON` untuk flyout Windows 11 Snap Layouts dan batas work area taskbar (`WM_GETMINMAXINFO`).

### 09. Project Akhir
* [Modern Win32 App (Octanio Win32 Shell)](/09-project/01-modern-app) — Aplikasi dashboard modern utuh yang menggabungkan seluruh konsep: CMake, resource exe, DPI aware, dark mode, custom title bar, dan render 60 FPS.

# Win32 + DWM Indonesia

Tutorial berbahasa Indonesia untuk mempelajari pengembangan aplikasi desktop Windows native menggunakan C++, Win32 API, dan Desktop Window Manager (DWM), mulai dari dasar hingga pembuatan custom title bar dan window modern.

[![Build](https://img.shields.io/github/actions/workflow/status/yohanesokta/win32-id/pages.yml?label=build)](../../actions)
[![GitHub Pages](https://img.shields.io/badge/docs-GitHub%20Pages-blue)](../../)
[![License](https://img.shields.io/github/license/yohanesokta/win32-id)](LICENSE)
[![C%2B%2B](https://img.shields.io/badge/C%2B%2B-20-blue)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-supported-064F8C)](https://cmake.org/)
[![Windows](https://img.shields.io/badge/platform-Windows-0078D4)](https://www.microsoft.com/windows)

> Dokumentasi Win32 yang dibuat untuk belajar, bukan sekadar membaca referensi API.

---

## Tentang

Win32 API merupakan salah satu fondasi utama pengembangan aplikasi desktop Windows. Namun, dokumentasi resmi Microsoft umumnya berfungsi sebagai referensi API dan sering kali tidak memberikan jalur pembelajaran yang jelas bagi pemula.

Project ini mencoba menyediakan jalur tersebut.

Materi disusun secara bertahap mulai dari konsep dasar C++, CMake, dan arsitektur aplikasi Win32 hingga message loop, window lifecycle, rendering, DPI awareness, DWM, dan custom title bar.

Fokus utama project:

* Bahasa Indonesia sebagai bahasa utama dokumentasi.
* Pembelajaran bertahap dari konsep dasar hingga project nyata.
* Contoh kode yang dapat langsung di-build.
* CMake sebagai build system.
* Tidak bergantung pada IDE tertentu.
* Menggunakan Win32 API secara langsung sebelum memperkenalkan abstraction.
* Membahas API Windows modern ketika relevan.
* Menjelaskan alasan di balik setiap konsep, bukan hanya cara menggunakannya.

---

## Tujuan

Setelah menyelesaikan tutorial, pembaca diharapkan memahami:

* bagaimana aplikasi Win32 dimulai;
* bagaimana Windows membuat dan mengelola window;
* cara kerja `HWND`, `HINSTANCE`, `HDC`, dan handle lainnya;
* cara kerja `WndProc`;
* bagaimana Windows mengirim message kepada aplikasi;
* cara kerja message loop;
* keyboard dan mouse event;
* proses painting dan rendering;
* window style dan extended style;
* client area dan non-client area;
* DPI awareness;
* resource aplikasi Windows;
* dasar GDI dan Direct2D;
* konsep Desktop Window Manager;
* DWM attributes;
* frameless window;
* `WM_NCHITTEST`;
* custom title bar;
* window resizing, dragging, minimize, maximize, dan close.

Target akhirnya adalah mampu membuat aplikasi Windows native menggunakan Win32 API tanpa harus bergantung pada framework GUI seperti Electron atau Qt.

---

## Status

Project ini dikembangkan secara bertahap.

| Komponen                | Status             |
| ----------------------- | ------------------ |
| Struktur repository     | Selesai            |
| CMake                   | Selesai            |
| VitePress documentation | Selesai            |
| GitHub Pages deployment | Selesai            |
| C++ prerequisite        | Selesai            |
| Win32 fundamentals      | Selesai            |
| Message loop            | Selesai            |
| Keyboard & mouse        | Selesai            |
| GDI rendering           | Selesai            |
| DPI awareness           | Selesai            |
| DWM                     | Selesai            |
| Frameless window        | Selesai            |
| Custom title bar        | Selesai            |
| Final project           | Selesai            |

---

## Roadmap

### 00. Persiapan & Toolchain

* [x] Mengapa Win32 dan DWM
* [x] Setup MSVC
* [x] Windows SDK
* [x] CMake
* [x] Ninja
* [x] Struktur project
* [x] Windows subsystem vs console subsystem

### 01. C++ Dasar untuk Win32

* [x] Tipe data Windows
* [x] `DWORD`, `BOOL`, `WPARAM`, `LPARAM`, `LRESULT`
* [x] Unicode
* [x] `wchar_t`
* [x] String Windows
* [x] Handle
* [x] Pointer
* [x] `GetLastError`

### 02. Pondasi Win32

* [x] Arsitektur aplikasi Win32
* [x] `wWinMain`
* [x] `WNDCLASSEXW`
* [x] `RegisterClassExW`
* [x] `CreateWindowExW`
* [x] Window styles
* [x] Window lifecycle
* [x] `ShowWindow`
* [x] `UpdateWindow`

### 03. Message Loop & Event Handling

* [x] Event-driven programming
* [x] Windows message queue
* [x] `MSG`
* [x] `GetMessageW`
* [x] `TranslateMessage`
* [x] `DispatchMessageW`
* [x] `WndProc`
* [x] `DefWindowProcW`
* [x] `WM_DESTROY`
* [x] `PostQuitMessage`

### 04. Input

* [x] `WM_KEYDOWN`
* [x] `WM_KEYUP`
* [x] `WM_CHAR`
* [x] Virtual-key codes
* [x] `WM_MOUSEMOVE`
* [x] Mouse button events
* [x] Mouse capture (`SetCapture`, `ReleaseCapture`)
* [x] `WPARAM`
* [x] `LPARAM`
* [x] `GET_X_LPARAM`
* [x] `GET_Y_LPARAM`

### 05. Window Painting & GDI

* [x] `WM_PAINT`
* [x] `BeginPaint`
* [x] `EndPaint`
* [x] `PAINTSTRUCT`
* [x] `HDC`
* [x] `TextOutW` & `DrawTextW`
* [x] `CreateSolidBrush` & `CreatePen`
* [x] Bentuk dasar (Rectangle, RoundRect, Ellipse, LineTo)
* [x] Invalidation (`InvalidateRect`)
* [x] Timer & Animasi 60 FPS (`SetTimer`, `WM_TIMER`)
* [x] Double buffering (`CreateCompatibleDC`, `BitBlt`)

### 06. DPI Awareness & Multi-Monitor

* [x] Masalah blur pada High-DPI
* [x] Per-Monitor V2 DPI Awareness
* [x] `GetDpiForWindow` & `GetDpiForSystem`
* [x] `WM_DPICHANGED`
* [x] Penskalaan dinamis (`MulDiv`)

### 07. Desktop Window Manager (DWM)

* [x] Apa itu DWM & GPU Compositor
* [x] Client vs non-client area
* [x] `DwmExtendFrameIntoClientArea` (menghidupkan drop shadow)
* [x] `DwmSetWindowAttribute`
* [x] Immersive Dark Mode (`DWMWA_USE_IMMERSIVE_DARK_MODE`)
* [x] Kustomisasi warna frame & teks (Windows 11)
* [x] Window corner preference (Rounded Corners)

### 08. Custom Title Bar & Frameless Window

* [x] Menghilangkan frame bawaan via `WM_NCCALCSIZE` (tanpa `WS_POPUP`)
* [x] `WM_NCHITTEST`
* [x] Dragging jendela & Aero snapping via `HTCAPTION`
* [x] Resizing di semua tepi dan sudut border
* [x] Tombol Minimize, Maximize, dan Close kustom dengan hover effect
* [x] Integrasi Windows 11 Snap Layouts (`HTMAXBUTTON`)
* [x] Menjaga batas area kerja Taskbar (`WM_GETMINMAXINFO`)

### 09. Final Project: Octanio Win32 Shell

Membangun aplikasi Windows native utuh yang menggabungkan seluruh materi:

* [x] Modern CMake & resource script (`app.rc`)
* [x] Win32 API murni & C++20
* [x] Per-Monitor V2 DPI Awareness
* [x] Custom frameless title bar dengan DWM shadow & dark mode
* [x] Double buffering 60 FPS
* [x] Live system stats (CPU, RAM, Uptime, Screen info)
* [x] Ukuran file exe hanya ~100 KB dan konsumsi RAM < 10 MB!

---

## Dokumentasi

Dokumentasi tersedia sebagai website yang dibangun menggunakan VitePress.

Untuk menjalankannya secara lokal:

```powershell
npm install
npm run dev
```

Kemudian buka:

```text
http://localhost:5173
```

Build production:

```powershell
npm run build
```

Preview hasil build:

```powershell
npm run preview
```

---

## Struktur Repository

```text
.
├── docs/
│   ├── index.md
│   ├── 00-persiapan/
│   ├── 01-cpp-dasar/
│   ├── 02-win32/
│   ├── 03-message-loop/
│   ├── ...
│   └── 13-final-project/
│
├── examples/
│   ├── 01-hello-window/
│   ├── 02-window-class/
│   ├── 03-message-loop/
│   ├── ...
│   └── final-project/
│
├── .github/
│   └── workflows/
│       └── pages.yml
│
├── CMakeLists.txt
├── package.json
├── README.md
└── LICENSE
```

Dokumentasi dan source code contoh dipisahkan agar pembaca dapat mempelajari teori sekaligus melihat implementasi yang dapat dijalankan.

---

## Requirements

Untuk mengikuti tutorial dan membuild contoh kode, diperlukan:

* Windows 10 atau Windows 11
* C++ compiler dengan Windows SDK
* MSVC
* CMake
* Ninja, opsional
* Git
* Node.js dan npm untuk dokumentasi

Visual Studio IDE tidak wajib digunakan. Build system menggunakan CMake sehingga project dapat digunakan bersama berbagai editor dan IDE yang mendukung CMake.

---

## Build Examples

Build seluruh contoh dari root repository:

```powershell
cmake -S . -B build
cmake --build build
```

Jika menggunakan Ninja:

```powershell
cmake -S . -B build -G Ninja
cmake --build build
```

Untuk membuild contoh tertentu:

```powershell
cd examples/01-hello-window

cmake -S . -B build
cmake --build build
```

Setiap example dirancang sebagai project yang mandiri sehingga dapat dipelajari dan dimodifikasi secara terpisah.

---

## Prinsip Pembelajaran

Tutorial ini mengikuti beberapa prinsip:

### Mulai dari Win32 API langsung

Abstraksi tidak diperkenalkan sebelum konsep Win32 yang mendasarinya dipahami.

Pembaca akan terlebih dahulu melihat API seperti:

```cpp
RegisterClassExW(...);
CreateWindowExW(...);
ShowWindow(...);
UpdateWindow(...);
```

sebelum diperkenalkan helper atau abstraction layer.

### Kode harus dapat dijalankan

Contoh tutorial harus dapat di-build dan dijalankan.

Dokumentasi tidak hanya memberikan potongan kode tanpa konteks.

### Jelaskan alasan, bukan hanya penggunaan

Setiap API penting dibahas dari sisi:

* apa fungsinya;
* parameter yang digunakan;
* kapan digunakan;
* bagaimana hubungannya dengan API lain;
* dan mengapa API tersebut diperlukan.

### Belajar melalui eksperimen

Setiap bagian sebisa mungkin memiliki latihan atau eksperimen kecil sehingga pembaca dapat mengubah kode dan melihat efeknya secara langsung.

---

## Referensi

Dokumentasi resmi Microsoft digunakan sebagai referensi teknis untuk memverifikasi API, parameter, behavior, dan kompatibilitas Windows.

Tutorial ini bukan pengganti dokumentasi resmi Microsoft. Tujuannya adalah menyediakan jalur pembelajaran yang lebih mudah sebelum pembaca menggunakan dokumentasi API sebagai referensi lanjutan.

Referensi utama:

* Microsoft Learn — Windows App Development
* Win32 API Reference
* Desktop Window Manager Documentation
* CMake Documentation
* VitePress Documentation

---

## Contributing

Kontribusi terbuka untuk siapa saja.

Beberapa bentuk kontribusi yang dapat membantu:

* memperbaiki kesalahan teknis;
* memperbaiki typo atau tata bahasa;
* memperjelas penjelasan;
* menambahkan contoh kode;
* menambahkan latihan;
* memperbaiki build example;
* melaporkan API yang sudah deprecated;
* melaporkan perbedaan behavior antar versi Windows.

Sebelum membuat perubahan besar, disarankan untuk membuka Issue terlebih dahulu agar perubahan dapat didiskusikan.

Pull Request yang memperbaiki dokumentasi atau contoh kode sangat dipersilakan.

---

## License

Source code dan materi dalam repository ini didistribusikan di bawah lisensi MIT.

Lihat [LICENSE](LICENSE) untuk informasi lengkap.

---

## Project Goals

Project ini tidak bertujuan menggantikan dokumentasi resmi Windows.

Tujuannya lebih sederhana:

> Membuat Win32 API lebih mudah dipelajari oleh developer Indonesia.

Dari window pertama:

```cpp
CreateWindowExW(...)
```

hingga:

```cpp
DwmSetWindowAttribute(...)
```

dan akhirnya memahami bagaimana sebuah aplikasi Windows native bekerja dari bawah.

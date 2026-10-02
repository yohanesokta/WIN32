# Project Akhir: Modern Win32 Application (Octanio Win32 Shell)

## Apa yang kita buat?

Pada bab penutup ini, kita merangkum dan mengintegrasikan seluruh materi yang telah kita pelajari dari awal hingga akhir ke dalam satu project nyata utuh: **Octanio Win32 Shell** (Modern Win32 App).

Sebuah aplikasi desktop dashboard modern berukuran biner hanya **~100 KB** dengan konsumsi RAM di bawah 10 MB, waktu startup instan, custom frameless title bar, dark mode DWM, hit-testing cerdas, dan rendering halus 60 FPS tanpa framework GUI eksternal!

---

## Fitur-Fitur Utama yang Terintegrasi

```text
┌────────────────────────────────────────────────────────┐
│             Per-Monitor V2 DPI Awareness               │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│               Window Class (WNDCLASSEXW)               │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│        Custom Frameless Title Bar (WM_NCCALCSIZE)      │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│        DWM Native Drop Shadow & Immersive Dark Mode    │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│           Hit Testing Cerdas (WM_NCHITTEST)            │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│       Double Buffering 60 FPS (CreateCompatibleDC)     │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│      Resource Compiler: app.rc Metadata Executable     │
└────────────────────────────────────────────────────────┘
```

1. **Struktur Project & Build Terminal**: Dikompilasi menggunakan CMake modern dan compiler MSVC murni.
2. **Arsitektur Tanpa Title Bar Kuno**: Menggunakan intersep pesan `WM_NCCALCSIZE` untuk memperluas kanvas ke seluruh bidang layar.
3. **Drop Shadow Windows 11**: Dihidupkan kembali menggunakan `DwmExtendFrameIntoClientArea`.
4. **Hit-Testing Presisi**:
   * Dragging jendela, aero snapping ke tepi layar, dan double-click to maximize via `HTCAPTION`.
   * Resizing border halus di seluruh 4 tepi sisi dan 4 sudut.
   * Tombol kontrol kustom (Minimize, Maximize/Restore, Close) dengan efek hover merah terang dan abu-abu.
5. **Per-Monitor V2 DPI Awareness**: UI otomatis terskalakan secara jernih saat berpindah monitor resolusi tinggi tanpa pernah buram.
6. **Double Buffering 60 FPS**: Mencegah kedipan layar (*flicker*) saat timer memperbarui data secara langsung.
7. **Statistik Sistem Real-time**: Mengambil status memori RAM (`GlobalMemoryStatusEx`) dan runtime aplikasi secara live.
8. **Resource Metadata Exe**: File executable memiliki metadata resmi (nama pembuat, versi, deskripsi) melalui script sumber daya `app.rc`.

---

## Struktur File Project

```text
examples/final-app/
├── CMakeLists.txt
├── resource.h
├── app.rc
└── main.cpp
```

---

## Cara Build & Menjalankan

Dari root repository:
```powershell
cmake -S . -B build
cmake --build build
.\build\examples\final-app\Debug\modern_app.exe
```

Atau masuk langsung ke subfoldernya:
```powershell
cd examples/final-app
cmake -S . -B build
cmake --build build
.\build\Debug\modern_app.exe
```

---

## Apa yang Bisa Kamu Eksplorasi Lebih Lanjut?

Dengan menguasai seluruh konsep di tutorial ini, kamu sekarang memiliki pondasi terkuat sebagai Windows Native Developer. Beberapa ide pengembangan mandiri yang bisa kamu coba:
1. **Direct2D / DirectWrite**: Mengganti GDI dengan Direct2D untuk rendering grafis dengan akselerasi GPU perangkat keras, font subpixel rendering, dan gradien warna modern.
2. **Systray Icon (`Shell_NotifyIcon`)**: Menambahkan ikon aplikasi di sudut kanan taskbar (dekat jam) saat tombol minimize ditekan.
3. **Dialog File (`IFileDialog`)**: Menambahkan dialog modern Windows untuk membuka atau menyimpan file.
4. **Child Windows / Custom Controls**: Membuat tombol, tab, atau panel kustom sendiri menggunakan Win32 murni.

---

## Kesimpulan Akhir

Kamu telah membuktikan bahwa membuat aplikasi desktop modern di Windows tidak harus bergantung pada framework raksasa yang rakus memori. Dengan C++, Win32, dan DWM, kamu memegang kendali penuh atas sistem operasi Windows dengan kecepatan dan keanggunan maksimal.

Selamat berkreasi! 🚀

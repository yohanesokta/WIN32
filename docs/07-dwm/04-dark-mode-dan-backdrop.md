# Dark Mode & Atribut Modern Windows 11 (DwmSetWindowAttribute)

## Apa yang akan kita pelajari?

Pada era Windows 10 dan 11, Microsoft memperkenalkan banyak atribut visual baru yang bisa diatur menggunakan satu fungsi sakti: **`DwmSetWindowAttribute`**.

Di bab ini, kita akan mempelajari cara mengaktifkan **Dark Mode** native pada title bar, mengatur warna frame kustom, sudut membulat (*Rounded Corners*), dan efek material modern seperti *Mica* dan *Acrylic*.

---

## Membedah Fungsi `DwmSetWindowAttribute`

Fungsi ini memiliki signature yang sangat bersih:

```cpp
HRESULT DwmSetWindowAttribute(
    HWND    hwnd,         // Handle window target
    DWORD   dwAttribute,  // Atribut apa yang ingin diubah (DWMWA_*)
    LPCVOID pvAttribute,  // Pointer ke nilai konfigurasi baru
    DWORD   cbAttribute   // Ukuran nilai dalam byte (sizeof(...))
);
```

---

## 1. Mengaktifkan Dark Mode (`DWMWA_USE_IMMERSIVE_DARK_MODE`)

Secara default, title bar Windows berwarna putih terang. Jika aplikasi kita memiliki UI bernuansa gelap (*dark theme*), title bar putih tersebut akan terlihat sangat silau dan tidak serasi.

Untuk mengubah title bar dan tombol kontrol default menjadi tema gelap:

```cpp
#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

BOOL useDarkMode = TRUE;
DwmSetWindowAttribute(
    hwnd,
    DWMWA_USE_IMMERSIVE_DARK_MODE,
    &useDarkMode,
    sizeof(useDarkMode)
);
```

> [!NOTE] Kompatibilitas Versi Windows
> * **Windows 11 & Windows 10 (20H1 / build 19041 ke atas)**: Menggunakan konstanta resmi **`20`**.
> * **Windows 10 lawas (1809 - 1909)**: Menggunakan atribut tidak terdokumentasi nomor **`19`**.
> Jika fungsi ini dipanggil di Windows yang belum mendukungnya, fungsi hanya akan mengembalikan kode error tanpa membuat aplikasi crash.

---

## 2. Mengatur Warna Title Bar Kustom (Windows 11)

Mulai Windows 11 (Build 22000), kamu bisa mewarnai title bar dan teksnya secara bebas tanpa harus membuat custom title bar sendiri:

```cpp
// 1. Ubah warna latar belakang title bar menjadi Dark Navy
COLORREF captionColor = RGB(30, 32, 40);
DwmSetWindowAttribute(hwnd, 35 /* DWMWA_CAPTION_COLOR */, &captionColor, sizeof(captionColor));

// 2. Ubah warna teks judul menjadi putih bersih
COLORREF textColor = RGB(255, 255, 255);
DwmSetWindowAttribute(hwnd, 36 /* DWMWA_TEXT_COLOR */, &textColor, sizeof(textColor));

// 3. Ubah warna garis border luar
COLORREF borderColor = RGB(0, 120, 215);
DwmSetWindowAttribute(hwnd, 34 /* DWMWA_BORDER_COLOR */, &borderColor, sizeof(borderColor));
```

---

## 3. Mengatur Sudut Membulat (*Rounded Corners*)

Windows 11 secara default membulatkan sudut-sudut jendela. Kamu bisa mengontrol perilaku ini:

```cpp
#include <dwmapi.h>

// Pilihan: DWMWCP_DEFAULT (0), DWMWCP_DONOTROUND (1), DWMWCP_ROUND (2), DWMWCP_ROUNDSMALL (3)
DWM_WINDOW_CORNER_PREFERENCE corner = DWMWCP_ROUND;
DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
```

---

## Contoh Nyata di Proyek `06-dwm-dark-mode`

Kamu bisa mencoba beralih tema Dark Mode dan Light Mode secara live di:
```text
examples/06-dwm-dark-mode/
├── CMakeLists.txt
└── main.cpp
```

Jalankan:
```powershell
cmake -S . -B build
cmake --build build
.\build\examples\06-dwm-dark-mode\Debug\dwm_app.exe
```

Tekan tombol **[D]** di keyboard untuk melihat title bar native Windows berubah seketika antara mode gelap dan terang!

---

## Materi Selanjutnya

Sekarang kita sudah menguasai DWM. Di modul pamungkas berikutnya, kita akan menggabungkan semuanya untuk membuat **Custom Title Bar Modern** dari nol: menghilangkan frame default, menangani hit-testing, dragging, hingga tombol kontrol interaktif!

👉 [Lanjut ke: 01. Menghilangkan Frame Default (Frameless Window)](/08-custom-titlebar/01-frameless-window)

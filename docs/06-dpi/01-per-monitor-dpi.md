# Per-Monitor V2 DPI Awareness

## Apa yang akan kita pelajari?

Pernahkah kamu membuka aplikasi lawas di laptop beresolusi Full HD atau monitor 4K, lalu melihat teks dan antarmuka aplikasinya terlihat sangat buram (*blurry*) seperti foto JPEG pecah?

Penyebabnya adalah **DPI Scaling**. Di bab ini, kita akan mempelajari cara mengaktifkan **Per-Monitor V2 DPI Awareness** agar aplikasi Win32 kita selalu tampil tajam setajam silet (*crystal clear*) di semua resolusi dan konfigurasi multi-monitor.

---

## Masalah: Mengapa Aplikasi Windows Bisa Buram?

Layar monitor standar zaman dahulu memiliki kerapatan titik piksel **96 DPI (Dots Per Inch)** atau skala 100%.

Pada layar laptop modern atau monitor 4K, kerapatan piksel menjadi sangat padat. Jika Windows menampilkan teks dalam ukuran 12 piksel biasa, teks tersebut akan tampak sekecil semut dan tidak terbaca. Oleh karena itu, Windows menerapkan fitur **Display Scaling** (misalnya 125%, 150%, atau 200%).

```text
Aplikasi Zaman Dulu (DPI Unaware):
Resolusi Asli: 800x600 ➔ Ditarik paksa oleh Windows (Bitmap Stretch) ke 1600x1200 ➔ BURAM!

Aplikasi Modern (Per-Monitor V2 Aware):
Windows: "Layar ini 200% (192 DPI). Tolong gambar dirimu 2x lebih besar!"
Aplikasi: Menggambar font dan bentuk 2x lebih tajam dengan vektor piksel asli ➔ SANGAT JERNIH!
```

---

## 3 Tingkatan DPI Awareness di Windows

| Tingkat DPI | Perilaku Windows | Kualitas Visual |
| :--- | :--- | :--- |
| **DPI Unaware** (Default lama) | Windows menganggap layar selalu 96 DPI, lalu memperbesar gambar biner (*bitmap stretch*). | **Buram / Pecah** |
| **System DPI Aware** | Menyesuaikan diri hanya dengan monitor utama saat pengguna login. Jika jendela digeser ke monitor kedua dengan skala berbeda, akan menjadi buram. | **Buram di Multi-Monitor** |
| **Per-Monitor V2** (Rekomendasi Modern) | Secara dinamis mendeteksi DPI setiap monitor secara individual dan mengirimkan pesan `WM_DPICHANGED` saat jendela berpindah layar. | **100% Selalu Tajam** |

---

## Mengaktifkan Per-Monitor V2 di C++

Cukup panggil fungsi ini di baris pertama fungsi `wWinMain` sebelum jendela dibuat:

```cpp
int WINAPI wWinMain(...)
{
    // Aktifkan mode ketajaman resolusi tingkat tertinggi Windows
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    // Lanjutkan registrasi class dan pembuatan window...
}
```

---

## Rumus Skala Piksel Dinamis (`MulDiv`)

Jangan pernah menggunakan angka piksel tetap (*hardcoded*) seperti `width = 200` atau `fontSize = 16` tanpa penskalaan.

Gunakan rumus ini:
$$\text{Piksel Terskalakan} = \frac{\text{Nilai Dasar} \times \text{DPI Saat Ini}}{96}$$

Windows menyediakan fungsi helper bawaan `MulDiv` yang menghitung perkalian lalu pembagian dengan pembulatan integer yang presisi:

```cpp
inline int ScaleForDpi(int pixelValue, UINT dpi)
{
    return MulDiv(pixelValue, dpi, 96);
}
```

Contoh penggunaannya:
* Pada skala 100% (96 DPI): `ScaleForDpi(16, 96) = 16 piksel`.
* Pada skala 150% (144 DPI): `ScaleForDpi(16, 144) = 24 piksel`.
* Pada skala 200% (192 DPI): `ScaleForDpi(16, 192) = 32 piksel`.

---

## Menangani Pesan `WM_DPICHANGED`

Ketika pengguna menggeser jendela dari layar laptop (skala 125%) ke monitor eksternal 4K (skala 200%), Windows akan mengirimkan pesan `WM_DPICHANGED`:

```cpp
case WM_DPICHANGED:
{
    // 1. Ambil nilai DPI baru dari wParam
    g_currentDpi = LOWORD(wParam);

    // 2. lParam membawa saran ukuran dan koordinat RECT baru dari Windows
    RECT* pSuggestedRect = (RECT*)lParam;

    // 3. Ubah ukuran jendela ke ukuran baru yang proporsional
    SetWindowPos(
        hwnd,
        nullptr,
        pSuggestedRect->left,
        pSuggestedRect->top,
        pSuggestedRect->right - pSuggestedRect->left,
        pSuggestedRect->bottom - pSuggestedRect->top,
        SWP_NOZORDER | SWP_NOACTIVATE
    );

    // 4. Minta gambar ulang dengan ukuran font baru
    InvalidateRect(hwnd, nullptr, TRUE);
    return 0;
}
```

---

## Contoh Proyek Lengkap: `05-dpi-aware`

Kamu bisa menguji langsung ketajaman visual di direktori:
```text
examples/05-dpi-aware/
├── CMakeLists.txt
└── main.cpp
```

### Cara Build & Menjalankan:
```powershell
cmake -S . -B build
cmake --build build
.\build\examples\05-dpi-aware\Debug\dpi_app.exe
```

Coba ubah Display Scaling di **Settings ➔ System ➔ Display** atau geser jendela ke monitor kedua. Kamu akan melihat ukuran teks dan elemen kotak menyesuaikan diri secara otomatis tanpa kehilangan ketajaman sedikit pun!

---

## Materi Selanjutnya

Sekarang pondasi Win32, rendering GDI, input, dan penskalaan DPI kita sudah sangat kokoh. Saatnya kita melangkah ke topik inti yang paling dinanti: **Desktop Window Manager (DWM)**!

👉 [Lanjut ke: 07. Pengenalan Komposisi DWM](/07-dwm/01-pengenalan-dwm)

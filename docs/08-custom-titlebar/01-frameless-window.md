# Menghilangkan Frame Default (Frameless Window)

## Apa yang akan kita buat?

Aplikasi modern seperti VS Code, Discord, Spotify, dan Google Chrome tidak lagi menggunakan title bar standar Windows yang kaku. Mereka memiliki antarmuka yang menyatu dari ujung atas hingga ke bawah (*frameless window*) dengan title bar kustom mereka sendiri.

Di bab ini, kita akan mempelajari cara menghilangkan frame default Windows dengan cara yang benar tanpa merusak animasi, snapping, dan drop shadow bawaan sistem operasi.

---

## Kesalahan Umum Pemula: Menggunakan `WS_POPUP`

Banyak tutorial pemula menyarankan untuk membuat frameless window dengan mengubah style saat `CreateWindowExW` menjadi:

```cpp
// CARA SALAH PEMULA:
CreateWindowExW(0, CLASS_NAME, L"App", WS_POPUP | WS_VISIBLE, ...);
```

Mengapa cara ini **sangat buruk**?
1. Jendela kehilangan bayangan asli (*drop shadow*).
2. Animasi minimasi dan maksimasi menjadi hilang (jendela berkedip kasar saat dibuka/tutup).
3. Fitur **Aero Snap** (menyeret jendela ke pinggir layar untuk membelah tampilan 50:50) menjadi **rusak/mati total**.
4. Jendela tidak bisa di-resize oleh pengguna.

---

## Cara Profesional: Tetap Gunakan `WS_THICKFRAME` & Intersep `WM_NCCALCSIZE`

Cara yang digunakan oleh software profesional kelas dunia adalah:

1. **Tetap daftarkan style `WS_OVERLAPPEDWINDOW`** saat memanggil `CreateWindowExW`.
2. **Cegat pesan `WM_NCCALCSIZE`** di dalam `WindowProc`!

```text
Windows mengirim WM_NCCALCSIZE
              │
              ▼
   Apakah wParam == TRUE?
   ├── [DefWindowProc Default] ──> Sisakan ruang untuk title bar & border standar
   └── [Kita Return 0 Langsung] ──> Client Area diperluas 100% menutupi seluruh jendela!
```

### Implementasi Kode

```cpp
case WM_NCCALCSIZE:
{
    // Jika wParam bernilai TRUE, Windows sedang menanyakan berapa ukuran area client kita.
    // Dengan mengembalikan angka 0 langsung (tanpa memanggil DefWindowProcW),
    // kita memberitahu Windows bahwa client area kita mengisi 100% seluruh jendela!
    if (wParam == TRUE)
    {
        return 0;
    }
    break;
}
```

Hanya dengan 4 baris kode di atas, seluruh frame dan title bar lama Windows akan lenyap seketika, memberikan kita kanvas kosong utuh dari ujung atas hingga bawah!

---

## Mengembalikan Drop Shadow: `DwmExtendFrameIntoClientArea`

Setelah frame dihilangkan dengan `WM_NCCALCSIZE`, panggil fungsi DWM ini tepat setelah `CreateWindowExW`:

```cpp
// Perluas frame DWM sebanyak 1 piksel di bagian atas
MARGINS margins = { 0, 0, 1, 0 };
DwmExtendFrameIntoClientArea(hwnd, &margins);

// Beritahu Windows bahwa struktur frame non-client telah berubah
SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
    SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
```

Sekarang jendela aplikasimu bersih tanpa title bar kuno, namun tetap memiliki bayangan halus Windows yang sangat elegan!

---

## Materi Selanjutnya

Sekarang jendela kita sudah frameless. Tetapi muncul masalah baru: bagaimana cara agar jendela tetap bisa digeser (*dragged*) dan diubah ukurannya (*resized*)? 

Jawabannya ada pada pesan paling penting: **`WM_NCHITTEST`**.

👉 [Lanjut ke: 02. Hit Testing (WM_NCHITTEST)](/08-custom-titlebar/02-hit-testing)

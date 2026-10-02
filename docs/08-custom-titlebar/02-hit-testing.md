# Hit Testing (WM_NCHITTEST)

## Apa yang akan kita pelajari?

Ketika kita menghapus title bar bawaan, pengguna tidak bisa lagi mengklik dan menyeret (*drag*) jendela kita. Selain itu, kursor mouse tidak lagi berubah menjadi panah resize dua arah di tepi jendela.

Di bab ini, kita akan mempelajari pesan paling ajaib dalam arsitektur desktop Windows: **`WM_NCHITTEST`**.

---

## Apa itu Hit Testing?

Setiap kali kursor mouse bergerak di atas layar, Windows terus-menerus bertanya kepada aplikasimu:
> *"Hei jendela, mouse sekarang sedang berada di koordinat (X, Y). Koordinat ini mewakili bagian apa dari tubuhmu?"*

Pertanyaan ini dikirimkan melalui pesan **`WM_NCHITTEST`** (Non-Client Hit Test). 

Jawaban yang kita kembalikan (*return value*) menentukan bagaimana sistem operasi Windows merespons:

| Nilai Kembalian LRESULT | Arti Bagi Windows | Perilaku Otomatis Windows |
| :--- | :--- | :--- |
| **`HTCAPTION`** | Bagian Title Bar | Windows otomatis menangani aksi geser (*drag*), snapping ke tepi layar, dan double-click untuk maximize! |
| **`HTCLIENT`** | Area Konten / Tombol Kustom | Windows mengirimkan event mouse biasa (`WM_MOUSEMOVE`, `WM_LBUTTONDOWN`) ke aplikasimu. |
| **`HTLEFT` / `HTRIGHT`** | Tepi Kiri / Kanan | Kursor otomatis berubah menjadi panah horizontal `↔` dan menangani resize. |
| **`HTTOP` / `HTBOTTOM`** | Tepi Atas / Bawah | Kursor otomatis berubah menjadi panah vertikal `↕` dan menangani resize. |
| **`HTTOPLEFT`, dll.** | Sudut-Sudut Jendela | Kursor otomatis berubah menjadi panah diagonal `⤢` dan menangani resize diagonal. |

---

## Implementasi Kode `WM_NCHITTEST`

Berikut logika pembagian zona koordinat yang presisi:

```cpp
case WM_NCHITTEST:
{
    // 1. Ambil koordinat mouse dalam koordinat layar monitor absolut
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    // 2. Ubah menjadi koordinat relatif jendela kita (0,0 ada di pojok kiri atas jendela)
    ScreenToClient(hwnd, &pt);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int width = clientRect.right;
    int height = clientRect.bottom;

    constexpr int BORDER = 8;        // Lebar area sensor resize di tepi jendela (8 piksel)
    constexpr int TITLEBAR_H = 40;   // Tinggi title bar kustom kita (40 piksel)

    // A. Deteksi 4 Sudut Resize (jika tidak sedang di-maximize)
    if (!IsZoomed(hwnd))
    {
        if (pt.y < BORDER && pt.x < BORDER) return HTTOPLEFT;
        if (pt.y < BORDER && pt.x > width - BORDER) return HTTOPRIGHT;
        if (pt.y > height - BORDER && pt.x < BORDER) return HTBOTTOMLEFT;
        if (pt.y > height - BORDER && pt.x > width - BORDER) return HTBOTTOMRIGHT;

        // B. Deteksi 4 Tepi Sisi Resize
        if (pt.y < BORDER) return HTTOP;
        if (pt.y > height - BORDER) return HTBOTTOM;
        if (pt.x < BORDER) return HTLEFT;
        if (pt.x > width - BORDER) return HTRIGHT;
    }

    // C. Pengecualian Tombol Kontrol (Min, Max, Close di pojok kanan atas)
    // Kita WAJIB mengembalikan HTCLIENT agar tombol kustom kita bisa diklik!
    int areaTombolMulai = width - (46 * 3);
    if (pt.y >= 0 && pt.y < TITLEBAR_H && pt.x >= areaTombolMulai)
    {
        return HTCLIENT;
    }

    // D. Sisa Area Title Bar: Kembalikan HTCAPTION!
    if (pt.y >= 0 && pt.y < TITLEBAR_H)
    {
        return HTCAPTION;
    }

    // E. Seluruh Area Lainnya adalah Client Konten Biasa
    return HTCLIENT;
}
```

> [!TIP] Keajaiban `HTCAPTION`
> Perhatikan bahwa kita sama sekali tidak perlu menulis rumus matematika rumit untuk menggeser jendela atau menghitung posisi monitor saat drag. Cukup kembalikan `HTCAPTION`, dan Windows OS yang akan melakukan semua pekerjaan berat tersebut secara native!

---

## Materi Selanjutnya

Kini jendela kita sudah bisa di-drag dan di-resize layaknya aplikasi profesional. Selanjutnya, mari kita buat tombol kontrol kustom: **Minimize, Maximize, dan Close** yang responsif dengan efek hover!

👉 [Lanjut ke: 03. Tombol Kontrol (Min, Max, Close)](/08-custom-titlebar/03-caption-buttons)

# Tombol Kontrol Kustom (Min, Max, Close)

## Apa yang akan kita buat?

Setelah title bar default dihilangkan, kita perlu membuat sendiri tombol navigasi jendela di pojok kanan atas: **Minimize**, **Maximize / Restore**, dan **Close**.

Di bab ini, kita akan mempelajari cara menghitung tata letak tombol, mendeteksi efek hover menggunakan `TrackMouseEvent`, dan mengeksekusi aksi jendela saat tombol diklik.

---

## Tata Letak Tombol di Pojok Kanan Atas

Standar lebar tombol Windows modern adalah **46 piksel** dengan tinggi menyesuaikan title bar (misalnya 40 piksel):

```text
Lebar Jendela = W
┌──────────────────────────────────────────────────────────┐
│ Title Bar                               [ _ ][ □ ][ ✕ ]  │
└──────────────────────────────────────────────────────────┘
                                          ▲    ▲    ▲
                                          │    │    └── Close: [W - 46, W]
                                          │    └─────── Maximize: [W - 92, W - 46]
                                          └──────────── Minimize: [W - 138, W - 92]
```

---

## Melacak Kursor Mouse: `TrackMouseEvent` & `WM_MOUSELEAVE`

Untuk menciptakan efek hover saat kursor mouse melintas di atas tombol, kita perlu tahu:
1. Kapan mouse **memasuki** area tombol (`WM_MOUSEMOVE`).
2. Kapan mouse **pergi meninggalkan** jendela (`WM_MOUSELEAVE`).

Windows tidak mengirimkan `WM_MOUSELEAVE` secara otomatis kecuali kita memintanya secara eksplisit menggunakan fungsi `TrackMouseEvent`:

```cpp
case WM_MOUSEMOVE:
{
    // Pasang pelacak agar Windows mengirimkan WM_MOUSELEAVE saat kursor keluar
    TRACKMOUSEEVENT tme = {};
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = hwnd;
    TrackMouseEvent(&tme);

    // Periksa tombol mana yang sedang disentuh kursor...
    break;
}

case WM_MOUSELEAVE:
{
    // Kursor mouse keluar dari jendela, hilangkan semua efek hover
    g_hoveredButton = HoveredButton::None;
    InvalidateRect(hwnd, nullptr, FALSE);
    return 0;
}
```

---

## Logika Klik Tombol di `WM_LBUTTONUP`

Saat pengguna mengklik dan melepas tombol mouse di atas area tombol:

```cpp
case WM_LBUTTONUP:
{
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    if (PtInRect(&rcClose, pt))
    {
        // Tombol Close: Kirim pesan tutup jendela
        PostMessageW(hwnd, WM_CLOSE, 0, 0);
    }
    else if (PtInRect(&rcMax, pt))
    {
        // Tombol Maximize: Cek apakah jendela sedang layar penuh
        if (IsZoomed(hwnd))
            ShowWindow(hwnd, SW_RESTORE);  // Kembalikan ke ukuran normal
        else
            ShowWindow(hwnd, SW_MAXIMIZE); // Perbesar ke layar penuh
    }
    else if (PtInRect(&rcMin, pt))
    {
        // Tombol Minimize
        ShowWindow(hwnd, SW_MINIMIZE);
    }
    return 0;
}
```

---

## Simbol Ikon Maximize yang Cerdas

Ketika jendela sedang dalam ukuran biasa, tombol menampilkan kotak tunggal: **`□`**.
Namun ketika jendela sedang di-maximize, tombol harus berubah menjadi simbol restore dua kotak bertumpuk: **`❐`**!

```cpp
const wchar_t* maxSymbol = IsZoomed(hwnd) ? L"❐" : L"□";
```

---

## Warna Hover Standar Windows

* **Tombol Minimize & Maximize**: Berikan warna abu-abu gelap transparan yang halus: `RGB(48, 54, 70)`.
* **Tombol Close**: Berikan warna merah khas Windows: `RGB(232, 17, 35)`. Teks silang '✕' berubah menjadi putih terang.

Hasilnya adalah tombol kustom yang terasa 100% identik dengan aplikasi bawaan Windows modern!

---

## Materi Selanjutnya

Di bab terakhir dari modul custom title bar, kita akan membahas integrasi dengan fitur mutakhir Windows 11: **Snap Layouts** dan penanganan area kerja multi-monitor!

👉 [Lanjut ke: 04. Snap Layouts & Window Resizing](/08-custom-titlebar/04-snap-layouts-resizing)

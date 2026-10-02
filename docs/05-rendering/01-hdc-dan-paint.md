# WM_PAINT & Device Context (HDC)

## Apa yang akan kita pelajari?

Dalam sistem operasi Windows, aplikasi tidak bisa begitu saja mencoret-coret layar monitor secara bebas. Seluruh proses penggambaran visual dikontrol oleh subsistem grafis melalui konsep **Device Context (`HDC`)** dan pesan **`WM_PAINT`**.

Di bab ini, kita akan membongkar siklus hidup penggambaran grafis, fungsi pasangan `BeginPaint` / `EndPaint`, serta cara memicu gambar ulang menggunakan `InvalidateRect`.

---

## Konsep Device Context (`HDC`)

Secara harfiah, **Device Context (HDC)** adalah *handle* ke struktur data yang mewakili kanvas gambar abstrak.

Mengapa disebut abstrak?
Karena dengan API yang sama persis (`TextOut`, `Rectangle`, `LineTo`), Windows dapat mengarahkan hasil gambarmu ke berbagai perangkat keluaran yang berbeda:
* Layar monitor fisik.
* Buffer memori di RAM (*Memory DC*).
* Mesin printer kertas.

Sebagai programmer, kita hanya perlu menggambar ke `HDC` tersebut, dan driver grafis Windows yang akan menerjemahkannya ke perangkat keras yang sesuai.

---

## Siklus Hidup `WM_PAINT`

Pesan `WM_PAINT` dikirimkan oleh Windows setiap kali ada bagian dari jendela yang perlu diperbarui, misalnya:
1. Jendela baru pertama kali dibuka di layar.
2. Jendela diperbesar (*resizing*) atau dipulihkan dari kondisi minimized.
3. Jendela lain yang sebelumnya menutupi jendela kita digeser atau ditutup.
4. Aplikasi kita sendiri secara sengaja meminta penggambaran ulang melalui fungsi `InvalidateRect`.

```text
Windows OS                         WindowProc                     Device Context (HDC)
    │                                  │                                   │
    │── Kirim pesan WM_PAINT ─────────>│                                   │
    │                                  │── Panggil BeginPaint(hwnd, &ps) ─>│
    │                                  │<─ Kembalikan HDC yang valid ──────│
    │                                  │                                   │
    │                                  │   (Eksekusi TextOut, Bentuk dll)  │
    │                                  │                                   │
    │                                  │── Panggil EndPaint(hwnd, &ps) ───>│
    │<─ Wilayah dinyatakan bersih ─────│                                   │
```

---

## Pasangan Wajib: `BeginPaint` dan `EndPaint`

Di dalam blok penanganan `case WM_PAINT:`, kamu **wajib** menggunakan pasangan fungsi ini:

```cpp
case WM_PAINT:
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // Lakukan proses menggambar di sini menggunakan hdc
    TextOutW(hdc, 50, 50, L"Halo GDI!", 9);

    EndPaint(hwnd, &ps);
    return 0;
}
```

### Apa yang Terjadi di Balik Layar?
* **`BeginPaint`**: Menyiapkan jendela untuk digambar, mengisi struktur `PAINTSTRUCT`, dan memotong (*clipping*) area gambar hanya pada bagian yang kotor (*invalid region*).
* **`EndPaint`**: Menandai ke sistem operasi bahwa area jendela tersebut sekarang sudah bersih (*validated*).

> [!CAUTION] Jangan Pernah Menggunakan `GetDC` di dalam `WM_PAINT`!
> Fungsi `GetDC(hwnd)` digunakan jika kamu ingin menggambar di luar event `WM_PAINT`. Jika kamu memanggil `GetDC` di dalam `WM_PAINT` alih-alih `BeginPaint`, Windows tidak akan pernah tahu bahwa jendela sudah selesai digambar. Akibatnya, Windows akan terus-menerus membombardir aplikasimu dengan jutaan pesan `WM_PAINT`, membuat penggunaan CPU melonjak hingga 100%!

---

## Memicu Gambar Ulang: `InvalidateRect`

Ketika data aplikasimu berubah (misalnya posisi mouse bergeser atau skor bertambah), bagaimana cara memberitahu Windows agar menggambar ulang?

Jawabannya adalah **`InvalidateRect`**:

```cpp
InvalidateRect(
    hwnd,     // Handle window
    nullptr,  // Rect pointer (nullptr = tandai SELURUH area jendela kotor)
    TRUE      // bErase (TRUE = hapus background terlebih dahulu sebelum menggambar)
);
```

Fungsi ini menandai area tertentu sebagai area kotor (*invalid*). Windows kemudian akan secara otomatis menjadwalkan pesan `WM_PAINT` pada siklus berikutnya.

---

## Materi Selanjutnya

Sekarang kita sudah paham cara kerja kanvas `HDC` dan siklus `WM_PAINT`. Di bab berikutnya, kita akan belajar menggambar berbagai bentuk geometri (persegi, lingkaran, garis) dan mewarnainya menggunakan **Brush** dan **Pen**.

👉 [Lanjut ke: 02. Menggambar Bentuk, Teks & Warna (GDI Dasar)](/05-rendering/02-gdi-dasar)

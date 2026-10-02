# Double Buffering (Mencegah Flicker Layar)

## Apa yang akan kita buat?

Ketika membuat animasi atau UI interaktif di Win32, salah satu keluhan paling umum dari pemula adalah layar yang **berkedip-kedip kencang (*flicker*)**. 

Di bab ini, kita akan membongkar penyebab teknis di balik kedipan layar tersebut dan menerapkan teknik standar industri grafis: **Double Buffering** menggunakan Memory DC dan fungsi transfer kilat `BitBlt`.

---

## Mengapa Layar Berkedip (*Flicker*)?

Secara default di Windows, setiap kali kamu memanggil `InvalidateRect(hwnd, nullptr, TRUE)`, Windows melakukan dua langkah berurutan:

1. **Langkah 1 (`WM_ERASEBKGND`)**: Windows menyapu seluruh layar dengan kuas latar belakang (misalnya warna putih).
2. **Langkah 2 (`WM_PAINT`)**: Windows memanggil kodemu untuk menggambar objek (misalnya bola berwarna biru).

```text
Tanpa Double Buffering:
Frame N:    [Layar Dihapus Putih] ➔ [Bola Digambar]  (Mata melihat kedipan putih!)
Frame N+1:  [Layar Dihapus Putih] ➔ [Bola Digambar]  (Berkedip lagi!)
Frame N+2:  [Layar Dihapus Putih] ➔ [Bola Digambar]  (Terjadi 60 kali per detik!)
```

Meskipun jeda antara langkah 1 dan 2 hanya sepersekian milidetik, mata manusia sangat peka terhadap perubahan cahaya yang mendadak. Akibatnya, animasi terlihat bergetar dan sangat tidak nyaman dilihat.

---

## Solusi: Teknik Double Buffering

Solusinya sangat elegan: **Jangan pernah menggambar langsung ke layar monitor yang sedang dilihat pengguna!**

Kita membuat kanvas bayangan tak terlihat di dalam memori RAM (*Off-Screen Memory Buffer*):
1. Seluruh proses penghapusan latar dan penggambaran bentuk dilakukan di kanvas bayangan tersebut di balik layar.
2. Setelah gambar frame jadi 100%, seluruh gambar dipindahkan ke layar monitor dalam satu kali operasi penyalinan blok memori yang sangat cepat (*Blit* / `BitBlt`).

```mermaid
flowchart TD
    subgraph Memori RAM Tersembunyi (Off-screen)
    A[CreateCompatibleDC] --> B[CreateCompatibleBitmap]
    B --> C[Hapus Latar Belakang di Memory DC]
    C --> D[Gambar Objek & Teks di Memory DC]
    end
    D -->|BitBlt dalam 0.0001 detik| E[Layar Monitor Fisik (HDC Asli)]
```

---

## 2 Langkah Wajib Penerapan di Kode

### Langkah 1: Tangani `WM_ERASEBKGND`
Beri tahu Windows agar **tidak menghapus** latar belakang secara otomatis:

```cpp
case WM_ERASEBKGND:
    // Kembalikan angka 1 (non-zero) untuk memberitahu Windows
    // bahwa kita menangani penghapusan latar belakang sendiri
    return 1;
```

### Langkah 2: Buat Memory DC dan Pindahkan dengan `BitBlt` di `WM_PAINT`

```cpp
case WM_PAINT:
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT rc;
    GetClientRect(hwnd, &rc);
    int width = rc.right;
    int height = rc.bottom;

    // 1. Buat Memory DC (kanvas bayangan) yang kompatibel dengan monitor
    HDC memDC = CreateCompatibleDC(hdc);

    // 2. Buat bitmap di RAM seukuran jendela
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);

    // 3. Pasangkan bitmap ke Memory DC
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

    // 4. Lakukan SEMUA proses menggambar ke memDC (bukan hdc!)
    HBRUSH bgBrush = CreateSolidBrush(RGB(24, 28, 36));
    FillRect(memDC, &rc, bgBrush);
    DeleteObject(bgBrush);

    // Contoh: gambar bola ke memDC
    Ellipse(memDC, 100, 100, 160, 160);

    // 5. Salin gambar utuh dari RAM ke layar monitor dalam 1 operasi instan
    BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

    // 6. Bersihkan memori GDI
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);

    EndPaint(hwnd, &ps);
    return 0;
}
```

---

## Bukti Nyata: Proyek `04-double-buffering`

Kami telah menyiapkan aplikasi demonstrasi interaktif untuk membuktikan perbedaannya di:
```text
examples/04-double-buffering/
├── CMakeLists.txt
└── main.cpp
```

### Cara Build & Menjalankan:
```powershell
cmake -S . -B build
cmake --build build
.\build\examples\04-double-buffering\Debug\double_buffer_app.exe
```

Ketika aplikasi terbuka:
* Kamu akan melihat bola animasi bergerak sangat mulus pada kecepatan 60 FPS tanpa ada sedikit pun kedipan.
* Tekan tombol **[SPASI]** pada keyboard untuk mematikan Double Buffering: kamu akan langsung melihat layar berkedip kencang.
* Tekan **[SPASI]** sekali lagi untuk mengaktifkannya kembali dan melihat keajaiban Double Buffering!

---

## Materi Selanjutnya

Sekarang kita sudah menguasai rendering grafis dan animasi halus. Di modul berikutnya, kita akan mempelajari bagaimana membuat aplikasi kita tampak tajam dan tidak buram di layar resolusi tinggi (4K/High-DPI) menggunakan **Per-Monitor V2 DPI Awareness**.

👉 [Lanjut ke: 06. DPI Awareness & Multi-Monitor](/06-dpi/01-per-monitor-dpi)

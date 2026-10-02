# Membuat Window (CreateWindowExW & ShowWindow)

## Apa yang akan kita pelajari?

Setelah mencetak birukan class jendela kita di sistem, saatnya mewujudkan jendela tersebut ke layar komputer.

Di bab ini, kita akan mempelajari fungsi **`CreateWindowExW`**, sistem koordinat Windows, serta bagaimana cara menampilkan dan merender jendela menggunakan **`ShowWindow`** dan **`UpdateWindow`**.

---

## Membedah Fungsi `CreateWindowExW`

Fungsi `CreateWindowExW` adalah salah satu fungsi paling terkenal di dunia Windows API. Fungsi ini menerima 12 parameter:

```cpp
HWND hwnd = CreateWindowExW(
    0,                              // 1. dwExStyle: Extended window style
    CLASS_NAME,                     // 2. lpClassName: Nama class terdaftar
    L"Halo Win32 Indonesia!",       // 3. lpWindowName: Teks di title bar
    WS_OVERLAPPEDWINDOW,            // 4. dwStyle: Style jendela utama
    CW_USEDEFAULT, CW_USEDEFAULT,   // 5 & 6. X, Y: Posisi jendela di layar
    800, 600,                       // 7 & 8. nWidth, nHeight: Lebar & tinggi
    nullptr,                        // 9. hWndParent: Handle jendela induk
    nullptr,                        // 10. hMenu: Handle menu bar
    hInstance,                      // 11. hInstance: Handle instance program
    nullptr                         // 12. lpParam: Pointer data kustom
);
```

Mari kita teliti parameter-parameter pentingnya:

### 1. `dwStyle`: Apa itu `WS_OVERLAPPEDWINDOW`?
Di Windows, karakteristik visual jendela diatur menggunakan kombinasi bit (*bitmask*). `WS_OVERLAPPEDWINDOW` adalah gabungan dari beberapa style standar:

```cpp
WS_OVERLAPPEDWINDOW = (WS_OVERLAPPED     | // Jendela memiliki frame overlap
                       WS_CAPTION        | // Jendela memiliki title bar (caption)
                       WS_SYSMENU        | // Memiliki menu sistem (ikon pojok kiri atas)
                       WS_THICKFRAME     | // Memiliki border tebal yang bisa di-resize mouse
                       WS_MINIMIZEBOX    | // Memiliki tombol minimize
                       WS_MAXIMIZEBOX)     // Memiliki tombol maximize
```

### 2. Posisi `X`, `Y` dan `CW_USEDEFAULT`
Sistem koordinat monitor di Windows dimulai dari pojok kiri-atas layar:
* Koordinat `(0, 0)` adalah sudut kiri-atas monitor utama.
* Sumbu **X** semakin membesar ke arah **kanan**.
* Sumbu **Y** semakin membesar ke arah **bawah**.

Konstanta **`CW_USEDEFAULT`** ("Create Window Use Default") memerintahkan Windows untuk secara cerdas memilih posisi penempatan jendela agar tidak menumpuk tepat di atas jendela lain yang sedang aktif (*cascading*).

### 3. Lebar & Tinggi (`nWidth`, `nHeight`)
Perlu diingat bahwa ukuran `800x600` ini adalah ukuran **luar jendela** (*window rectangle*), yang sudah mencakup ketebalan border dan tinggi title bar default Windows. 

Nanti di bab DPI dan rendering grafis, kita akan belajar fungsi `AdjustWindowRectEx` untuk memastikan area gambar di dalam (*client area*) berukuran persis `800x600`.

---

## Memeriksa Validitas HWND

Jika pembuatan jendela gagal (misalnya karena sistem kehabisan sumber daya memori), fungsi akan mengembalikan nilai `nullptr` (atau `NULL`). Kita harus selalu memvalidasinya:

```cpp
if (hwnd == nullptr)
{
    MessageBoxW(
        nullptr,
        L"Gagal membuat jendela di layar!",
        L"Error Win32",
        MB_ICONERROR | MB_OK
    );
    return 0;
}
```

---

## Memunculkan Jendela: `ShowWindow` & `UpdateWindow`

Setelah `CreateWindowExW` berhasil, jendela kita sebenarnya sudah ada di memori internal Windows, tetapi belum digambar ke layar.

Untuk memunculkannya:

```cpp
// 1. Atur status visibilitas jendela
ShowWindow(hwnd, nCmdShow);

// 2. Paksa pengiriman pesan WM_PAINT pertama kali secara langsung
UpdateWindow(hwnd);
```

* **`ShowWindow(hwnd, nCmdShow)`**: Mengubah status jendela menjadi terlihat (*visible*). Parameter `nCmdShow` diteruskan langsung dari argumen `wWinMain`.
* **`UpdateWindow(hwnd)`**: Mengirimkan pesan gambar ulang (`WM_PAINT`) secara langsung ke `WindowProc` tanpa menunggu giliran antrean pesan lain, sehingga jendela langsung tampak terisi dan tidak putih sesaat saat pertama kali muncul.

---

## Materi Selanjutnya

Sekarang jendela kita sudah muncul di layar monitor! Namun, jika kita tidak memiliki sistem untuk menangani event pengguna, jendela tersebut akan langsung macet (*hang* / *Not Responding*). 

Mari kita pelajari bagian paling krusial dari Win32: **Message Loop & Window Procedure**.

👉 [Lanjut ke: 01. Konsep Event-Driven di Windows](/03-message-loop/01-konsep-event-driven)

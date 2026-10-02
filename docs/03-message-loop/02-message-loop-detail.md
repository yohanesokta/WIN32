# Bedah Tuntas Message Loop (GetMessage, Translate, Dispatch)

## Apa yang akan kita pelajari?

Message loop adalah jantung dari setiap aplikasi GUI Win32. Hanya terdiri dari 6 baris kode C++, tetapi baris-baris ini yang menentukan hidup dan matinya aplikasi kita.

Di bab ini, kita akan membongkar setiap kata dalam loop tersebut hingga ke tingkat cara kerja sistem operasi.

---

## Kode Lengkap Message Loop Standar

Berikut adalah bentuk idiomatis standar dari message loop Win32:

```cpp
MSG msg = {};
while (GetMessageW(&msg, nullptr, 0, 0) > 0)
{
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
}
return (int)msg.wParam;
```

---

## 1. Membedah Struktur `MSG`

Struktur `MSG` membawa seluruh informasi tentang satu peristiwa yang terjadi:

```cpp
typedef struct tagMSG {
    HWND   hwnd;     // Window mana yang dituju oleh pesan ini
    UINT   message;  // ID pesan (misalnya WM_PAINT, WM_KEYDOWN)
    WPARAM wParam;   // Data parameter 1 (misal: kode tombol virtual)
    LPARAM lParam;   // Data parameter 2 (misal: koordinat mouse X dan Y)
    DWORD  time;     // Waktu stempel (timestamp) kapan pesan dikirimkan
    POINT  pt;       // Posisi koordinat kursor mouse saat pesan terjadi
} MSG;
```

---

## 2. Cara Kerja `GetMessageW`

Fungsi `GetMessageW` bertugas mengambil satu pesan paling depan dari antrean pesan thread:

```cpp
BOOL bRet = GetMessageW(
    &msg,      // Alamat memori struktur MSG tempat data akan diisi
    nullptr,   // Filter HWND (nullptr = ambil pesan untuk SEMUA window milik thread ini)
    0, 0       // Filter rentang ID pesan (0, 0 = ambil semua jenis pesan tanpa filter)
);
```

### Apakah `GetMessageW` Menghabiskan CPU?
**Tidak!** `GetMessageW` adalah fungsi pemblokir cerdas (*blocking function*). Jika tidak ada interaksi pengguna (antrean pesan kosong), sistem operasi Windows akan menidurkan thread aplikasimu (*sleep state*). Penggunaan CPU aplikasi adalah **0.00%**. 

Begitu mouse digerakkan atau tombol keyboard ditekan, kernel Windows akan membangunkan thread dan `GetMessageW` segera mengembalikan pesan tersebut.

### Mengapa Kondisi Loop Ditulis `> 0`?
Perhatikan:
```cpp
while (GetMessageW(&msg, nullptr, 0, 0) > 0)
```
Bukan `while (GetMessageW(&msg, ...))` biasa!

Nilai kembalian `GetMessageW`:
* **Angka Positif (`> 0`)**: Ada pesan normal yang siap diproses. Loop lanjut berjalan.
* **Angka Nol (`0`)**: Pesan yang diterima adalah **`WM_QUIT`**. Ini adalah sinyal bahwa aplikasi harus ditutup. Loop berhenti secara alami.
* **Angka Negatif (`-1`)**: Terjadi kesalahan kritis di sistem (misalnya pointer buffer tidak valid). Menulis `> 0` memastikan program tidak terjebak dalam *infinite loop* jika terjadi error sistem.

---

## 3. Apa Fungsi `TranslateMessage`?

Keyboard komputer tidak langsung menghasilkan huruf 'A' atau 'a'. Keyboard mengirimkan kode fisik tombol (*scan code* dan *virtual key code*).

Fungsi `TranslateMessage(&msg)` bertugas:
1. Memeriksa pesan `WM_KEYDOWN` dan `WM_KEYUP`.
2. Mengecek status tombol pendukung (apakah tombol `Shift` sedang ditekan? Apakah `Caps Lock` aktif? Apa tata letak bahasa keyboard pengguna?).
3. Menghasilkan pesan karakter baru bernama **`WM_CHAR`** yang sudah diterjemahkan menjadi karakter Unicode yang benar, lalu meletakkannya kembali ke antrean pesan.

Jika kamu menghapus baris `TranslateMessage(&msg)`, jendela aplikasimu tidak akan pernah menerima input teks karakter `WM_CHAR` dari pengguna!

---

## 4. Apa Fungsi `DispatchMessageW`?

Setelah pesan diterjemahkan, siapa yang memprosesnya?

Fungsi `DispatchMessageW(&msg)` membaca field `msg.hwnd`, melihat fungsi Window Procedure mana yang terdaftar pada window tersebut saat registrasi class, lalu **langsung memanggil fungsi callback `WindowProc`** jendela terkait dan menyerahkan pesan tersebut.

---

## Ringkasan Alur Perjalanan Pesan

```text
1. Pengguna menekan tombol keyboard
              ↓
2. Windows membuat paket MSG dan menaruhnya di Message Queue
              ↓
3. GetMessageW(&msg, ...) terbangun dan mengambil pesan
              ↓
4. TranslateMessage(&msg) menerjemahkan tombol menjadi WM_CHAR
              ↓
5. DispatchMessageW(&msg) memanggil WindowProc(hwnd, msg, wParam, lParam)
              ↓
6. WindowProc kita mengeksekusi aksi yang sesuai
```

---

## Materi Selanjutnya

Sekarang kita sudah paham bagaimana pesan diambil dan dikirimkan. Di bab berikutnya, kita akan membedah anatomi dari fungsi penerima akhir: **`WindowProc`**.

👉 [Lanjut ke: 03. Anatomi Window Procedure (WndProc)](/03-message-loop/03-wndproc-anatomi)

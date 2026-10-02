# Tipe Data Windows (HWND, DWORD, WPARAM, LPARAM)

## Apa yang akan kita bahas?

Ketika pertama kali membuka kode C++ Win32 atau membaca dokumentasi Microsoft, kamu akan melihat banyak tipe data bertuliskan huruf kapital seperti `DWORD`, `BOOL`, `UINT`, `LPARAM`, `WPARAM`, dan `LRESULT`. 

Di bab ini, kita akan membongkar arti tipe-tipe data tersebut, kenapa Microsoft membuatnya, dan padanannya dalam tipe data standar C++.

---

## Mengapa Windows Memiliki Tipe Data Sendiri?

Pada awal era 1980-an hingga 1990-an, standar C belum memiliki header `<cstdint>` (yang menyediakan tipe seperti `uint32_t` atau `int64_t`). Ukuran tipe data dasar C (`int`, `long`) bisa berbeda-beda tergantung arsitektur (16-bit, 32-bit, hingga 64-bit).

Untuk menjaga kompatibilitas kode agar tetap dapat dikompilasi selama puluhan tahun, Microsoft membuat *typedef* khusus di dalam `<windows.h>` yang ukurannya dijamin tetap.

---

## Tabel Tipe Data Umum Win32

| Tipe Data Win32 | Definisi Asli di C++ | Ukuran | Penjelasan & Contoh Penggunaan |
| :--- | :--- | :--- | :--- |
| **`BYTE`** | `unsigned char` | 8-bit (1 byte) | Data biner mentah, warna saluran RGB individual |
| **`WORD`** | `unsigned short` | 16-bit (2 byte) | Nomor port, modifier key (Shift, Ctrl, Alt) |
| **`DWORD`** | `unsigned long` | 32-bit (4 byte) | "Double Word", style window, kode error `GetLastError()` |
| **`UINT`** | `unsigned int` | 32-bit (4 byte) | ID pesan Windows (seperti `WM_PAINT`, `WM_DESTROY`) |
| **`LONG`** | `long` | 32-bit (4 byte) | Nilai koordinat piksel (X, Y) dalam struktur `RECT` / `POINT` |
| **`BOOL`** | `int` | 32-bit (4 byte) | Nilai boolean Win32 (`TRUE` atau `FALSE`) |

> [!WARNING] Perbedaan Kritis: `BOOL` vs C++ `bool`
> Di C++, tipe bawaan `bool` berukuran 1 byte dengan nilai `true` atau `false` (huruf kecil).
> Di Win32 API, `BOOL` adalah *typedef* untuk `int` (4 byte) dengan nilai konstanta `TRUE` (1) atau `FALSE` (0).
> Jangan sampai tertukar saat berinteraksi dengan API yang mengembalikan pointer ke `BOOL*`.

---

## Memahami `WPARAM`, `LPARAM`, dan `LRESULT`

Tiga tipe ini akan kamu temui di setiap fungsi `WndProc`:

```cpp
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
```

### 1. `WPARAM` ("Word Parameter")
* **Ukuran**: 32-bit di sistem 32-bit, dan **64-bit (unsigned)** di sistem 64-bit (`UINT_PTR`).
* **Fungsi**: Membawa data tambahan untuk pesan `uMsg`. Contohnya pada pesan keyboard `WM_KEYDOWN`, `wParam` berisi kode tombol virtual (*virtual key code*) seperti `VK_ESCAPE` atau `VK_SPACE`.

### 2. `LPARAM` ("Long Parameter")
* **Ukuran**: 32-bit di sistem 32-bit, dan **64-bit (signed)** di sistem 64-bit (`LONG_PTR`).
* **Fungsi**: Membawa data pointer atau data gabungan. Contohnya pada pesan mouse `WM_MOUSEMOVE`, `lParam` membawa dua angka 16-bit sekaligus: koordinat X mouse di setengah bagian bawah (*low-word*) dan koordinat Y di setengah bagian atas (*high-word*).

Untuk mengambil koordinat mouse dari `lParam`, Windows menyediakan macro:
```cpp
int xPos = GET_X_LPARAM(lParam); // Koordinat X mouse
int yPos = GET_Y_LPARAM(lParam); // Koordinat Y mouse
```

### 3. `LRESULT`
* **Ukuran**: Nilai integer bertanda seukuran pointer (64-bit di x64).
* **Fungsi**: Nilai balik (*return value*) dari fungsi pemroses pesan (`WndProc`) kepada sistem operasi Windows untuk memberitahu apakah pesan sudah selesai ditangani atau bernilai kode tertentu.

---

## Analogi Sederhana

Bayangkan `uMsg` adalah **surat perintah**:
* `uMsg` = *"Ada ketukan di pintu!"*
* `wParam` = Informasi ringkas: *"Siapa yang mengetuk?"*
* `lParam` = Informasi detail: *"Membawa barang apa dan berapa dimensinya?"*
* `LRESULT` = Jawaban dari penerima: *"Diterima dengan baik"* (biasanya mengembalikan `0`).

---

## Materi Selanjutnya

Setelah memahami tipe data dasar, mari kita pelajari bagaimana Windows menangani teks dan string melalui standar **Unicode** dan `wchar_t`.

👉 [Lanjut ke: 02. Unicode, wchar_t & String Windows](/01-cpp-dasar/02-unicode-dan-string)

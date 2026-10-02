# Unicode, wchar_t & String di Windows

## Apa yang akan kita pelajari?

Dalam pemrograman Win32, salah satu sumber kebingungan terbesar bagi programmer pemula adalah perbedaan antara string `char*`, `wchar_t*`, tipe `LPCWSTR`, serta mengapa fungsi Win32 sering berakhiran huruf **`A`** atau **`W`** (misalnya `MessageBoxA` vs `MessageBoxW`).

Di bab ini, kita akan memahami standar Unicode di Windows dan aturan terbaik menulis string modern di C++.

---

## Misteri Akhiran Fungsi: `A` vs `W`

Jika kamu mencari dokumentasi suatu fungsi Windows (misalnya `CreateWindowEx`), kamu sebenarnya tidak akan menemukan fungsi bernama persis seperti itu di dalam DLL Windows.

Windows menyediakan dua variasi untuk hampir setiap API yang menerima teks:

```text
               ┌──> CreateWindowExA  (Versi ANSI / ASCII / Multi-byte)
CreateWindowEx │
               └──> CreateWindowExW  (Versi WIDE / Unicode UTF-16)
```

1. **Versi `A` (ANSI)**: Menggunakan tipe `char*` (1 byte per karakter). Versi ini adalah peninggalan masa lalu Windows 95/98 dengan codepage lokal.
2. **Versi `W` (Wide / Unicode)**: Menggunakan tipe `wchar_t*` (2 byte per karakter dengan enkoding UTF-16 LE).

### Mengapa Kita Harus Selalu Menggunakan Versi `W`?
Sistem operasi Windows NT (termasuk Windows 10 dan Windows 11 modern) **secara internal bekerja 100% menggunakan Unicode UTF-16**.

Ketika kamu memanggil fungsi berakhiran `A` (seperti `MessageBoxA`):
1. Windows mengalokasikan memori sementara di heap.
2. Mengonversi string ANSI kamu ke Unicode UTF-16.
3. Memanggil fungsi versi `W` (`MessageBoxW`).
4. Mengonversi kembali hasilnya jika ada.
5. Membebaskan alokasi memori.

> [!IMPORTANT] Rekomendasi Modern
> Selalu gunakan fungsi eksplisit berakhiran **`W`** (misalnya `CreateWindowExW`, `RegisterClassExW`, `DefWindowProcW`) dan gunakan string wide `const wchar_t*`. Jangan membuang siklus CPU untuk konversi string kuno!

---

## Wide Character (`wchar_t`) dan Literal `L""`

Di C++, karakter standar `char` berukuran 1 byte. Untuk menyimpan karakter Unicode dua byte di Windows, C++ menyediakan tipe bawaan `wchar_t`.

Untuk membuat string bernilai `wchar_t`, tambahkan prefiks huruf **`L`** di depan tanda kutip:

```cpp
// String ANSI biasa (1 byte per karakter) - JANGAN GUNAKAN UNTUK WIN32 MODERN
const char* strAnsi = "Halo Dunia"; 

// String Unicode Wide (2 byte per karakter) - STANDAR RESMI WIN32
const wchar_t* strUnicode = L"Halo Dunia Win32!";
```

Dengan string wide `L""`, aplikasimu dapat menampilkan karakter dari berbagai bahasa di dunia (Jepang, Arab, Mandarin, aksara daerah) serta simbol emoji secara sempurna tanpa teks menjadi rusak (*mojibake*).

---

## Membaca Singkatan Tipe Pointer String Microsoft

Dokumentasi Microsoft penuh dengan singkatan tipe pointer string. Berikut cara membacanya dengan sangat mudah:

| Singkatan | Kepanjangan Asli | Definisi Nyata di C++ | Penjelasan |
| :--- | :--- | :--- | :--- |
| **`LPSTR`** | **L**ong **P**ointer to **Str**ing | `char*` | Pointer ke string ANSI (dapat diedit) |
| **`LPCSTR`** | **L**ong **P**ointer to **C**onst **Str**ing | `const char*` | Pointer ke string ANSI *read-only* |
| **`LPWSTR`** | **L**ong **P**ointer to **W**ide **Str**ing | `wchar_t*` | Pointer ke string Unicode (dapat diedit) |
| **`LPCWSTR`** | **L**ong **P**ointer to **C**onst **W**ide **Str**ing | `const wchar_t*` | Pointer ke string Unicode *read-only* |
| **`PWSTR`** | **P**ointer to **W**ide **Str**ing | `wchar_t*` | Sama dengan `LPWSTR` |
| **`PCWSTR`** | **P**ointer to **C**onst **W**ide **Str**ing | `const wchar_t*` | Sama dengan `LPCWSTR` |

> [!TIP] Trik Menghafal
> * Huruf **`LP`** / **`P`** artinya **Pointer**.
> * Huruf **`C`** artinya **Const** (konstan / tidak boleh diubah).
> * Huruf **`W`** artinya **Wide** (`wchar_t` / Unicode).
> * Huruf **`STR`** artinya **String**.

Jadi jika suatu fungsi Win32 meminta parameter bertipe `LPCWSTR`, itu artinya fungsi tersebut hanya meminta:
```cpp
const wchar_t*
// Contoh: L"Judul Jendela Saya"
```

---

## Bagaimana dengan `std::wstring`?

Jika kamu ingin memanipulasi string secara dinamis di C++ (seperti menggabungkan teks, memotong, atau mengubah huruf), gunakan `std::wstring` dari header `<string>`:

```cpp
#include <string>

std::wstring judul = L"Aplikasi Saya";
judul += L" - v1.0";

// Ambil raw pointer C-style untuk diteruskan ke fungsi Win32:
const wchar_t* rawPointer = judul.c_str();
```

---

## Materi Selanjutnya

Sekarang kamu sudah fasih dengan tipe data dan string Unicode di Windows. Selanjutnya, kita akan membahas salah satu konsep paling sentral di Win32: **Handle (`HWND`, `HINSTANCE`, `HDC`)**.

👉 [Lanjut ke: 03. Handle, Pointer & Konvensi Win32](/01-cpp-dasar/03-handle-dan-pointer)

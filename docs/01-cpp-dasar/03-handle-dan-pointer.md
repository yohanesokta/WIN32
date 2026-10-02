# Handle, Pointer & Konvensi Panggilan di Win32

## Apa yang akan kita pelajari?

Di Win32 API, hampir setiap operasi berputar di sekitar konsep **Handle** (seperti `HWND`, `HINSTANCE`, `HDC`, `HBRUSH`). Selain itu, kamu akan sering melihat keyword seperti `CALLBACK` dan `WINAPI`.

Di bab ini, kita akan memahami apa itu Handle secara konseptual dan mengapa Windows tidak mengizinkan kita memegang pointer memori internal secara langsung.

---

## Apa Sebenarnya Sebuah "Handle"?

Secara teknis di C++, sebuah `HANDLE` biasanya didefinisikan sebagai pointer buram (*opaque pointer*):
```cpp
typedef void* HANDLE;
```

Tetapi **jangan** memperlakukannya seperti pointer memori biasa! Kamu tidak bisa melakukan dereferensi `*handle` atau mengakses properti di dalamnya (`handle->width`).

### Analogi Tiket Penitipan Barang

Bayangkan kamu menitipkan koper di tempat penitipan bandara:
1. Kamu menyerahkan koper fisikmu ke petugas.
2. Petugas **tidak** menyuruhmu masuk ke dalam gudang rahasia bandara.
3. Sebagai gantinya, petugas memberimu selembar **karcis bernomor #42**.
4. Nomor karcis #42 ini adalah **HANDLE**.
5. Kapan pun kamu ingin mengambil, memeriksa, atau memindahkan kopermu, kamu cukup menunjukkan nomor #42 tersebut ke petugas.

```text
┌────────────────┐      Beri perintah dengan HWND #42      ┌────────────────┐
│  Aplikasi Kita │ ──────────────────────────────────────> │ Kernel Windows │
└────────────────┘                                         └───────┬────────┘
                                                                   │
                                                                   │ Mencari objek di tabel internal
                                                                   ▼
                                                   ┌────────────────────────────────┐
                                                   │ Struktur Window Fisik di RAM   │
                                                   └────────────────────────────────┘
```

### Mengapa Windows Menggunakan Handle?
1. **Keamanan & Stabilitas**: Struktur internal window berisi data sensitif kernel. Jika aplikasi diberi pointer memori langsung, bug atau aplikasi berbahaya dapat merusak stabilitas seluruh sistem operasi.
2. **Fleksibilitas Internal**: Windows bebas memindahkan atau memodifikasi tata letak memori internal window tanpa membuat kode aplikasimu rusak (*breaking change*).

---

## Jenis-Jenis Handle yang Paling Sering Digunakan

| Tipe Handle | Kepanjangan | Mewakili Objek Apa? |
| :--- | :--- | :--- |
| **`HWND`** | Handle to a Window | Satu jendela aktif di layar |
| **`HINSTANCE`** | Handle to an Instance | Modul/executable program kita yang dimuat di memori RAM |
| **`HDC`** | Handle to a Device Context | Kanvas gambar untuk melukis teks/grafis ke layar |
| **`HICON`** | Handle to an Icon | Ikon aplikasi |
| **`HCURSOR`** | Handle to a Cursor | Bentuk kursor mouse (panah, tanda silang, loading) |
| **`HBRUSH`** | Handle to a Brush | Warna atau pola untuk mengisi bidang latar belakang |
| **`HPEN`** | Handle to a Pen | Alat untuk menggambar garis tepi dan kontur |

---

## Apa Maksud `WINAPI` dan `CALLBACK`?

Dalam deklarasi fungsi Win32, kamu akan melihat:

```cpp
int WINAPI wWinMain(HINSTANCE hInstance, ...);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, ...);
```

Kedua keyword ini sebenarnya adalah alias untuk **konvensi panggilan** (*calling convention*):
```cpp
#define WINAPI   __stdcall
#define CALLBACK __stdcall
```

### Apa itu `__stdcall`?
Konvensi panggilan menentukan aturan bagaimana compiler memasukkan argumen fungsi ke dalam register/stack prosesor dan siapa yang bertugas membersihkan memori stack setelah fungsi selesai:
* Pada `__stdcall`, fungsi yang dipanggil (*callee*) bertugas membersihkan stack.
* Pada arsitektur **x64 (64-bit)** modern, seluruh fungsi menggunakan konvensi panggilan tunggal terpadu berbasis register, sehingga `__stdcall` diabaikan secara otomatis oleh compiler. Namun, kita **tetap wajib menuliskannya** agar kode kompatibel dan sesuai dengan signature resmi Windows SDK.

---

## Memeriksa Kesalahan: `GetLastError()`

Hampir semua fungsi Win32 mengembalikan nilai khusus jika terjadi kegagalan (misalnya mengembalikan `NULL`, `0`, atau `FALSE`).

Ketika suatu fungsi gagal, Windows menyimpan kode error penyebabnya. Kamu bisa mengambil nomor kode error tersebut menggunakan:

```cpp
DWORD errorCode = GetLastError();
```

> [!WARNING] Aturan Emas `GetLastError()`
> Panggil `GetLastError()` **segera** tepat setelah fungsi yang gagal dieksekusi. Jangan memanggil fungsi Win32 lain di antaranya, karena fungsi lain tersebut dapat menimpa kode error terakhir menjadi sukses (`0`).

Untuk mengubah kode angka tersebut menjadi kalimat pesan error yang jelas dibaca manusia:

```cpp
#include <windows.h>
#include <string>

std::wstring GetLastErrorMessage()
{
    DWORD err = GetLastError();
    if (err == 0) return L"Tidak ada error.";

    LPWSTR buffer = nullptr;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&buffer,
        0,
        nullptr
    );

    std::wstring message = buffer ? buffer : L"Unknown error.";
    if (buffer) LocalFree(buffer);
    return message;
}
```

---

## Materi Selanjutnya

Pondasi teori C++ dan konsep sistem Windows sudah tuntas! Sekarang kita siap masuk ke modul inti: **Membuat Jendela Win32 Pertama Kita dari Nol**.

👉 [Lanjut ke: 01. Arsitektur Aplikasi Win32](/02-win32-dasar/01-arsitektur-win32)

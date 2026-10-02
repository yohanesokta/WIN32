# Mendaftarkan Window Class (WNDCLASSEXW)

## Apa yang akan kita pelajari?

Sebelum kita bisa membuat jendela di layar, kita harus mendaftarkan karakteristiknya ke Windows. Konsep ini disebut **Window Class**.

Di bab ini, kita akan membedah struktur **`WNDCLASSEXW`** dan fungsi pendaftarannya **`RegisterClassExW`**.

---

## Apa itu Window Class?

Window Class dapat diibaratkan sebagai **cetak biru (blueprint)** atau cetakan kue:
* Satu cetak biru dapat digunakan untuk mencetak banyak jendela dengan karakteristik serupa.
* Cetak biru ini mengaitkan nama unik (misalnya `L"MyWindowClass"`) dengan fungsi C++ pemroses event (`WindowProc`).

---

## Mengapa Menggunakan `WNDCLASSEXW` (Bukan `WNDCLASSW`)?

Di dokumentasi lama, kamu mungkin melihat struktur `WNDCLASS` (tanpa akhiran `EX`). 

Struktur `WNDCLASSEXW` ("EX" = Extended) adalah versi modern yang menyempurnakan struktur lama dengan dua fitur penting:
1. **`cbSize`**: Menyimpan ukuran struktur dalam byte, memungkinkan Windows memeriksa versi struktur secara aman di masa depan.
2. **`hIconSm`**: Mendukung ikon kecil khusus (16x16 piksel) untuk title bar dan taskbar.

---

## Bedah Setiap Anggota Struktur `WNDCLASSEXW`

Berikut adalah cara mengisi struktur `WNDCLASSEXW` secara lengkap:

```cpp
WNDCLASSEXW wc = {};
wc.cbSize        = sizeof(WNDCLASSEXW);
wc.style         = CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc   = WindowProc;
wc.cbClsExtra    = 0;
wc.cbWndExtra    = 0;
wc.hInstance     = hInstance;
wc.hIcon         = LoadIconW(nullptr, IDI_APPLICATION);
wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
wc.lpszMenuName  = nullptr;
wc.lpszClassName = L"Win32Tutorial_WindowClass";
wc.hIconSm       = LoadIconW(nullptr, IDI_APPLICATION);
```

> [!TIP] Inisialisasi Nol (`= {}`)
> Selalu inisialisasi struktur dengan `= {}`. Ini memastikan semua field yang tidak kita atur secara eksplisit bernilai `0` / `NULL`, mencegah bug aneh akibat nilai sampah (*garbage value*) di memori.

### 1. `cbSize`
Wajib diisi dengan `sizeof(WNDCLASSEXW)`. Jika lupa diisi, fungsi `RegisterClassExW` akan langsung gagal karena Windows menganggap struktur tidak valid.

### 2. `style`
Flag karakteristik jendela:
* `CS_HREDRAW`: Gambar ulang seluruh jendela jika lebar jendela diubah (*horizontal redraw*).
* `CS_VREDRAW`: Gambar ulang seluruh jendela jika tinggi jendela diubah (*vertical redraw*).

### 3. `lpfnWndProc`
**Bagian paling penting!** Singkatan dari *Long Pointer to Function Window Procedure*. Ini adalah alamat memori dari fungsi C++ yang akan menerima setiap pesan dari Windows untuk jendela ini.

### 4. `cbClsExtra` & `cbWndExtra`
Jumlah byte memori ekstra yang dialokasikan oleh sistem untuk class atau window. Biasanya selalu diisi `0`.

### 5. `hInstance`
Handle instance program kita yang didapat dari parameter `wWinMain`.

### 6. `hIcon` & `hIconSm`
Handle ke ikon besar (tampil saat `Alt + Tab`) dan ikon kecil (tampil di pojok kiri atas title bar). Kita bisa menggunakan ikon standar bawaan Windows seperti `IDI_APPLICATION`.

### 7. `hCursor`
Bentuk kursor mouse saat berada di atas jendela. `IDC_ARROW` adalah kursor panah standar.

### 8. `hbrBackground`
Kuas latar belakang (*brush*). Angka `(HBRUSH)(COLOR_WINDOW + 1)` adalah warna putih jendela standar Windows.

### 9. `lpszClassName`
String nama unik untuk class ini. Nama inilah yang nanti akan kita sebutkan saat memanggil `CreateWindowExW`.

---

## Mendaftarkan Class ke Windows

Setelah struktur selesai diisi, kita daftarkan ke sistem operasi:

```cpp
if (!RegisterClassExW(&wc))
{
    MessageBoxW(
        nullptr,
        L"Gagal mendaftarkan Window Class!",
        L"Error Win32",
        MB_ICONERROR | MB_OK
    );
    return 0; // Hentikan program
}
```

Jika fungsi mengembalikan `0`, pendaftaran gagal (misalnya karena nama class sudah pernah didaftarkan sebelumnya atau parameter ada yang salah).

---

## Materi Selanjutnya

Cetak biru jendela sudah berhasil didaftarkan ke Windows! Sekarang saatnya membuat jendela fisik di layar menggunakan **`CreateWindowExW`**.

👉 [Lanjut ke: 04. Membuat Window (CreateWindowExW)](/02-win32-dasar/04-membuat-window)

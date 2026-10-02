# Entry Point: wWinMain

## Apa yang akan kita bahas?

Di program C++ konsol biasa, eksekusi kode selalu dimulai dari fungsi `main()`. Namun pada aplikasi GUI Windows modern, titik masuk (*entry point*) resmi yang dicari oleh sistem operasi dan linker adalah **`wWinMain`**.

Di bab ini, kita akan membedah signature fungsi `wWinMain` dan arti dari setiap parameternya.

---

## Mengapa Bukan `main()`?

Ketika kamu mengatur CMake dengan `add_executable(App WIN32 main.cpp)`, linker MSVC mengonfigurasi biner dengan flag `/SUBSYSTEM:WINDOWS`. 

Pada subsystem ini, runtime Windows C tidak mencari `main()`, melainkan mencari:
* `wWinMain`: Titik masuk untuk aplikasi Unicode (standar modern).
* `WinMain`: Titik masuk lama untuk string ANSI.

---

## Signature Lengkap `wWinMain`

Berikut adalah bentuk resmi fungsi `wWinMain`:

```cpp
#include <windows.h>

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR     pCmdLine,
    int       nCmdShow
)
{
    // Logika aplikasi dimulai di sini
    return 0;
}
```

Mari kita bedah satu per satu setiap komponennya:

### 1. `int WINAPI`
* `int`: Nilai pengembalian program ke sistem operasi saat aplikasi ditutup (biasanya `0` jika sukses, atau nilai error tertentu).
* `WINAPI`: Menentukan konvensi panggilan fungsi (`__stdcall`).

### 2. `HINSTANCE hInstance`
* **Definisi**: "Handle to an Instance".
* **Fungsi**: Merupakan alamat basis memori tempat file executable (`.exe`) aplikasimu dimuat oleh Windows ke dalam memori RAM virtual.
* **Penggunaan**: Handle ini akan sering kamu butuhkan saat memuat aset resource dari file exe (seperti ikon, kursor kustom, menu, atau string terjemahan).

### 3. `HINSTANCE hPrevInstance`
* **Definisi**: "Handle to Previous Instance".
* **Fungsi**: Peninggalan zaman 16-bit (Windows 3.1) ketika Windows belum memiliki isolasi memori virtual antar-proses, digunakan untuk mendeteksi apakah instance program yang sama sudah berjalan sebelumnya.
* **Status Modern**: Pada Windows 32-bit dan 64-bit modern (Win32), setiap proses memiliki ruang memori independen. Parameter ini **selalu bernilai `NULL`** dan tidak pernah digunakan lagi.

### 4. `PWSTR pCmdLine`
* **Definisi**: "Pointer to Wide String".
* **Fungsi**: Berisi seluruh argumen baris perintah (*command line arguments*) yang dilewatkan pengguna saat menjalankan aplikasi, dalam bentuk string Unicode (`wchar_t*`).
* **Catatan**: Berbeda dengan `argv[]` di `main()`, `pCmdLine` adalah satu string mentah utuh tanpa nama file executable di awalnya.

### 5. `int nCmdShow`
* **Definisi**: "Number Command Show".
* **Fungsi**: Flag instruksi dari sistem operasi atau aplikasi pemanggil mengenai bagaimana jendela utama seharusnya pertama kali ditampilkan di layar.
* **Contoh Nilai**:
  * `SW_SHOWNORMAL` (1): Tampilkan jendela dalam ukuran normal.
  * `SW_SHOWMAXIMIZED` (3): Buka jendela langsung dalam keadaan layar penuh (*maximized*).
  * `SW_SHOWMINIMIZED` (2): Buka jendela langsung di-minimize ke taskbar.

Kita akan meneruskan nilai `nCmdShow` ini langsung ke fungsi `ShowWindow(hwnd, nCmdShow)`.

---

## Materi Selanjutnya

Sekarang kita sudah memahami titik masuk aplikasi. Langkah selanjutnya adalah menyiapkan dan mendaftarkan cetak biru jendela kita menggunakan **`WNDCLASSEXW`**.

👉 [Lanjut ke: 03. Mendaftarkan Window Class](/02-win32-dasar/03-window-class)

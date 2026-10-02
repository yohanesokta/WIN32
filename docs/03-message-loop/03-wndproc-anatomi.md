# Anatomi Window Procedure (WndProc)

## Apa yang akan kita pelajari?

Jika `wWinMain` adalah pintu gerbang dan Message Loop adalah jantungnya, maka **Window Procedure (`WndProc`)** adalah otak dari aplikasi Win32. Di sinilah seluruh logika respons terhadap aksi pengguna dipusatkan.

Di bab ini, kita akan membedah anatomi `WndProc`, peran vital `DefWindowProcW`, dan mengapa proses di background bisa menjadi *zombie process* jika kita salah menangani `WM_DESTROY`.

---

## Signature Lengkap Window Procedure

Berikut adalah bentuk resmi fungsi Window Procedure:

```cpp
LRESULT CALLBACK WindowProc(
    HWND   hwnd,
    UINT   uMsg,
    WPARAM wParam,
    LPARAM lParam
);
```

Jangan hanya menghafal signature ini! Mari kita pahami alasan di balik setiap parameternya:

### 1. `HWND hwnd`
Handle ke jendela yang menerima pesan ini. 

> [!NOTE] Kenapa Parameter Ini Penting?
> Satu fungsi `WindowProc` bisa digunakan bersama-sama oleh **banyak jendela sekaligus**. Parameter `hwnd` memberitahu kita jendela spesifik mana yang sedang diklik atau digambar ulang saat ini.

### 2. `UINT uMsg`
Nomor pengenal pesan (*Message ID*). Ini adalah bilangan unsigned 32-bit (seperti `WM_PAINT = 0x000F`, `WM_DESTROY = 0x0002`). Kita menggunakan blok `switch (uMsg)` untuk memilih kode mana yang ingin dieksekusi.

### 3. `WPARAM wParam` & `LPARAM lParam`
Data pembawa parameter tambahan yang relevan dengan jenis pesan `uMsg`.

### 4. `LRESULT`
Nilai kembalian berupa integer 64-bit ke Windows. Makna dari angka yang dikembalikan bergantung pada jenis pesan:
* Sebagian besar pesan mengharapkan return `0` jika kita sudah selesai menanganinya.
* Beberapa pesan khusus (seperti `WM_NCHITTEST`) mengharapkan return kode posisi hit testing.

---

## Pola Dasar Penulisan `WindowProc`

```cpp
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            // Tangani proses menggambar di sini
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // ... gambar sesuatu ...
            EndPaint(hwnd, &ps);
            return 0; // Selesai ditangani
        }

        case WM_DESTROY:
            // Jendela dihancurkan, kirim sinyal berhenti ke Message Loop
            PostQuitMessage(0);
            return 0;
    }

    // WAJIB: Pesan yang tidak kita proses diserahkan ke penanganan default Windows
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}
```

---

## Apa itu `DefWindowProcW` dan Mengapa Wajib Dipanggil?

Windows mengirimkan **ratusan pesan** ke setiap jendela setiap detik:
* Pesan saat kursor mouse melintas di atas border (`WM_SETCURSOR`).
* Pesan saat pengguna mengklik dan menggeser title bar (`WM_NCLBUTTONDOWN`).
* Pesan aktivasi jendela, shadow, animasi minimize, dan focus.

Kita tentu tidak ingin menulis kode sendiri dari nol hanya untuk membuat window bisa digeser atau di-minimize!

Fungsi **`DefWindowProcW`** ("Default Window Procedure") adalah implementasi bawaan dari Microsoft Windows yang menangani seluruh perilaku standar tersebut. 

> [!CAUTION] Kesalahan Fatal Pemula
> Jika kamu lupa memanggil `DefWindowProcW` di bagian akhir fungsi `WindowProc` dan hanya menulis `return 0;`, jendela aplikasimu akan rusak: tidak bisa dipindahkan, border tidak bisa di-resize, tombol minimize/maximize mati, dan kursor mouse tidak berubah bentuk.

---

## Siklus Penutupan Jendela: `WM_CLOSE` vs `WM_DESTROY` vs `PostQuitMessage`

Banyak pemula bingung membedakan antara `WM_CLOSE`, `WM_DESTROY`, dan `PostQuitMessage`. Berikut urutan peristiwa saat pengguna mengklik tombol silang [X]:

```text
Pengguna                 Sistem Windows               WindowProc             Message Loop
   │                           │                           │                      │
   │── Klik Silang [X] ───────>│                           │                      │
   │                           │── Kirim WM_CLOSE ────────>│                      │
   │                           │                           │ (Cek Simpan Data)    │
   │                           │<─ Panggil DestroyWindow ──│                      │
   │                           │                           │                      │
   │                           │── Kirim WM_DESTROY ──────>│                      │
   │                           │                           │── PostQuitMessage ──>│
   │                           │                           │                      │
   │                           │                           │  GetMessage terima   │
   │                           │                           │  WM_QUIT (return 0)  │
   │                           │                           │                      ▼
   │                           │                           │              Loop Berhenti!
```

### Apa yang Terjadi Jika Kita Lupa `PostQuitMessage(0)`?
Jika kamu menghapus baris `PostQuitMessage(0)` pada `WM_DESTROY`:
1. Jendela memang akan hilang dari layar monitor.
2. **Tetapi program tidak pernah keluar!**
3. Fungsi `GetMessageW` di `wWinMain` masih terus menunggu pesan berikutnya.
4. Programmu menjadi **"Zombie Process"** yang terus berjalan di latar belakang dan menghabiskan memori RAM. Kamu harus membunuhnya secara manual melalui *Task Manager*.

---

## Contoh Proyek Nyata: `01-hello-window`

Seluruh konsep yang kita pelajari dari Bab 00 hingga Bab 03 terangkum dalam contoh kode lengkap di direktori:
```text
examples/01-hello-window/
├── CMakeLists.txt
└── main.cpp
```

### Cara Build & Menjalankan:
```powershell
cd examples/01-hello-window
cmake -S . -B build
cmake --build build
.\build\Debug\hello_window.exe
```

Jendela berukuran 800x600 dengan latar belakang putih dan teks *"Selamat Datang di Tutorial Win32 & DWM Indonesia!"* akan muncul di layarmu. Selamat, kamu baru saja membuat aplikasi Windows native murni pertamamu!

---

## Materi Selanjutnya

Di modul berikutnya, kita akan mempelajari bagaimana menangani interaksi pengguna: menangkap ketukan tombol keyboard dan melacak posisi kursor mouse secara presisi.

👉 [Lanjut ke: 04. Input Keyboard](/04-input/01-keyboard)

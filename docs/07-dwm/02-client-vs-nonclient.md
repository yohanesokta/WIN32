# Client Area vs Non-Client Area

## Apa yang akan kita pelajari?

Dalam arsitektur Win32, setiap jendela dibagi menjadi dua zona terpisah: **Client Area** dan **Non-Client Area**. Memahami batas pemisah antara kedua zona ini adalah kunci mutlak sebelum kita bisa membangun *Custom Title Bar* modern.

---

## Anatomi Pembagian Wilayah Jendela

```text
┌──────────────────────────────────────────────────────────┐  ▲
│ [Icon] Judul Aplikasi                           — □  ✕  │  │ Non-Client Area
├──────────────────────────────────────────────────────────┤  ▼ (Dikelola oleh Windows)
│                                                          │  ▲
│                                                          │  │
│                                                          │  │ Client Area
│                      Area Konten Kita                    │  │ (Dikelola oleh Kodemu)
│                                                          │  │
│                                                          │  │
└──────────────────────────────────────────────────────────┘  ▼
```

### 1. Non-Client Area
* **Wilayah**: Title bar, border luar, tombol kontrol (Minimize, Maximize, Close), ikon menu sistem, dan bayangan (*drop shadow*).
* **Siapa yang Mengelola?**: Secara default ditangani oleh **Windows dan DWM**.
* **Pesan Terkait**: Seluruh pesan yang diawali dengan **`WM_NC...`** (singkatan dari *Non-Client*), seperti:
  * `WM_NCCALCSIZE`: Windows menanyakan berapa ukuran area client.
  * `WM_NCPAINT`: Windows menggambar frame dan title bar.
  * `WM_NCHITTEST`: Windows mendeteksi apakah kursor mouse sedang berada di atas tombol title bar atau border resize.
  * `WM_NCLBUTTONDOWN`: Pengguna mengklik tombol di title bar.

### 2. Client Area
* **Wilayah**: Kanvas putih/kosong di bagian dalam jendela tempat kamu menggambar teks, tombol kustom, dan grafik.
* **Siapa yang Mengelola?**: **Kode aplikasimu sendiri** melalui `WM_PAINT` dan `WindowProc`.

---

## Fungsi Mengukur Kedua Wilayah

### 1. `GetClientRect`
Mengambil dimensi kanvas gambar di dalam. Nilai `left` dan `top` selalu bernilai `0`.
```cpp
RECT rc;
GetClientRect(hwnd, &rc);
int lebarDalam  = rc.right;
int tinggiDalam = rc.bottom;
```

### 2. `GetWindowRect`
Mengambil dimensi luar jendela secara keseluruhan dalam koordinat layar monitor absolut (termasuk border dan title bar).
```cpp
RECT rc;
GetWindowRect(hwnd, &rc);
int lebarLuar  = rc.right - rc.left;
int tinggiLuar = rc.bottom - rc.top;
```

---

## Mengapa Memahami Ini Sangat Penting?

Jika kita ingin membuat title bar modern yang menyatu dengan konten (seperti tampilan VS Code, Discord, atau Spotify), kita harus memerintahkan Windows untuk **menghapus Non-Client Area default** dan memperluas Client Area kita ke seluruh bidang jendela hingga ke ujung paling atas.

Teknik inilah yang akan kita pelajari di bab-bab berikutnya!

---

## Materi Selanjutnya

Mari kita pelajari API DWM pertama kita: memperluas frame jendela ke dalam client area.

👉 [Lanjut ke: 03. DwmExtendFrameIntoClientArea](/07-dwm/03-dwm-extend-frame)

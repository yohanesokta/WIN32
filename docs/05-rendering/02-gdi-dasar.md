# Menggambar Bentuk, Teks & Warna (GDI Dasar)

## Apa yang akan kita pelajari?

**GDI (Graphics Device Interface)** adalah pustaka grafis 2D bawaan Windows. Menggunakan GDI, kita bisa menggambar garis, persegi panjang, lingkaran, teks indah berformat Unicode, dan mewarnainya sesuka hati.

Di bab ini, kita akan mempelajari instrumen gambar dasar: **Pena (`HPEN`)**, **Kuas (`HBRUSH`)**, **Font (`HFONT`)**, serta aturan emas pengelolaan memori GDI agar terhindar dari *GDI resource leak*.

---

## Warna di Windows: Makro `RGB` dan Tipe `COLORREF`

Warna di GDI disimpan dalam tipe data `COLORREF` (bilangan 32-bit). Kita menggunakan macro `RGB(red, green, blue)` di mana setiap saluran bernilai antara `0` sampai `255`:

```cpp
COLORREF merah = RGB(255, 0, 0);
COLORREF hijau = RGB(0, 255, 0);
COLORREF biru  = RGB(0, 120, 215);
COLORREF gelap = RGB(30, 30, 30);
```

---

## Dua Alat Utama: Kuas (`HBRUSH`) vs Pena (`HPEN`)

Bayangkan kamu sedang melukis di atas kertas:
* **Pena (`HPEN`)**: Menggambar garis luar (*outline* atau kontur).
* **Kuas (`HBRUSH`)**: Mengisi warna bagian dalam suatu bangun datar (*fill*).

```text
┌────────────────────────────────┐
│   HPEN (Garis & Border Luar)   │ ───┐
└────────────────────────────────┘    │
                                      ├───> [ Hasil Bentuk Grafis Jadi ]
┌────────────────────────────────┐    │
│    HBRUSH (Warna Isi Bidang)   │ ───┘
└────────────────────────────────┘
```

### Cara Membuat Pena (`CreatePen`)
```cpp
// Buat pena garis solid, ketebalan 3 piksel, warna biru
HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 120, 215));
```

### Cara Membuat Kuas (`CreateSolidBrush`)
```cpp
// Buat kuas warna oranye solid
HBRUSH hBrush = CreateSolidBrush(RGB(255, 140, 0));
```

---

## Pola "Pilih, Gambar, Kembalikan, Hapus" (*Select & Restore*)

Ini adalah pola paling fundamental saat memprogram GDI:

```cpp
// 1. Buat objek kustom baru
HBRUSH hBrush = CreateSolidBrush(RGB(255, 140, 0));

// 2. Pasang ke HDC dan SIMPAN objek lama bawaan Windows
HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

// 3. Lakukan proses menggambar
Rectangle(hdc, 50, 50, 250, 200);

// 4. KEMBALIKAN objek lama ke HDC
SelectObject(hdc, hOldBrush);

// 5. HAPUS objek kustom buatan kita untuk membebaskan memori
DeleteObject(hBrush);
```

> [!CAUTION] Batas Maksimal GDI Handle Windows
> Sistem operasi Windows membatasi setiap aplikasi hanya boleh memiliki maksimal **10.000 handle GDI**. Jika kamu membuat kuas di dalam `WM_PAINT` tanpa pernah memanggil `DeleteObject`, aplikasimu akan mengalami kebocoran memori (*resource leak*). Dalam beberapa menit, aplikasi akan macet dan seluruh elemen grafis Windows bisa menjadi hitam!

---

## Fungsi Menggambar Bentuk Dasar

### 1. Persegi Panjang (`Rectangle`)
```cpp
// Parameter: HDC, Kiri, Atas, Kanan, Bawah
Rectangle(hdc, 50, 50, 250, 200);
```

### 2. Persegi Sudut Tumpul (`RoundRect`)
```cpp
// Dua angka terakhir (20, 20) adalah diameter kelengkungan sudut X dan Y
RoundRect(hdc, 50, 50, 250, 200, 20, 20);
```

### 3. Lingkaran atau Elips (`Ellipse`)
```cpp
// Menggambar lingkaran di dalam kotak pembatas
Ellipse(hdc, 300, 50, 450, 200);
```

### 4. Menarik Garis Bebas (`MoveToEx` & `LineTo`)
```cpp
// Pindahkan pena ke titik awal (50, 300) tanpa menggambar
MoveToEx(hdc, 50, 300, nullptr);

// Tarik garis ke titik akhir (500, 300)
LineTo(hdc, 500, 300);
```

---

## Menggambar Teks dengan Font Indah

Untuk menampilkan teks dengan latar belakang transparan (tidak menimpa warna di bawahnya dengan kotak putih):

```cpp
// 1. Buat font Segoe UI ukuran 20pt
HFONT hFont = CreateFontW(
    20, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
    L"Segoe UI"
);
HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

// 2. Atur warna teks dan background transparan
SetTextColor(hdc, RGB(255, 255, 255));
SetBkMode(hdc, TRANSPARENT);

// 3. Gambar teks ke area RECT dengan perataan tengah
RECT rc = { 50, 50, 400, 100 };
DrawTextW(hdc, L"Teks Modern Win32", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

// 4. Cleanup
SelectObject(hdc, hOldFont);
DeleteObject(hFont);
```

---

## Contoh Proyek Lengkap: `03-gdi-drawing`

Kamu bisa melihat dan menjalankan contoh galeri bentuk GDI interaktif lengkap di:
```text
examples/03-gdi-drawing/
├── CMakeLists.txt
└── main.cpp
```

Build dan jalankan:
```powershell
cmake -S . -B build
cmake --build build
.\build\examples\03-gdi-drawing\Debug\gdi_app.exe
```

---

## Materi Selanjutnya

Setelah menguasai penggambaran statis, bagaimana cara membuat objek bergerak dan beranimasi? Mari kita pelajari penggunaan **Timer Windows (`SetTimer`)**.

👉 [Lanjut ke: 03. Timer & Animasi](/05-rendering/03-timer-dan-animasi)

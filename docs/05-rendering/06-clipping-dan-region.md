# Clipping & Regions (Mencegah Bleeding & Masking Grafis)

## Apa yang akan kita pelajari?

Pernahkah kamu menggambar gambar bitmap atau gradasi warna di dalam kartu berujung bulat (*rounded card*), tetapi warna atau gambarnya **bocor dan meluber keluar** melewati sudut-sudut lengkung kartu?

Untuk mengatasi masalah ini, GDI menyediakan mekanisme **Clipping Region (`HRGN`)**. Di bab ini, kita akan mempelajari cara membuat topeng batas (*masking*), menggabungkan bentuk region, dan mengunci area penggambaran agar elemen UI kita selalu rapi.

---

## Apa Perbedaan `RECT` vs `HRGN`?

* **`RECT`**: Hanya struktur data C++ biasa yang berisi 4 angka integer (`left`, `top`, `right`, `bottom`).
* **`HRGN` (Handle to a Region)**: Objek GDI sejati di dalam kernel Windows yang dapat mewakili bentuk geometri kompleks (persegi sudut tumpul, lingkaran, bintang, poligon tak beraturan) yang digunakan untuk mendeteksi tabrakan atau membatasi wilayah lukis.

```text
Tanpa Clipping Region:
┌───────────────────────────┐
│ Kotak Gradasi Meluber     │ <--- Sudut tajam bocor keluar dari kartu!
│    ╭─────────────────╮    │
│    │  Kartu Melengkung│    │
│    ╰─────────────────╯    │
└───────────────────────────┘

Dengan Clipping Region:
     ╭─────────────────╮
     │  Gradasi Terpotong│ <--- Gradasi terpotong rapi mengikuti lekukan!
     ╰─────────────────╯
```

---

## Fungsi-Fungsi Pembuat Region

```cpp
// 1. Region Persegi Biasa
HRGN rgn1 = CreateRectRgn(0, 0, 300, 200);

// 2. Region Persegi Sudut Tumpul (Paling sering untuk Card UI Modern)
HRGN rgn2 = CreateRoundRectRgn(0, 0, 300, 200, 16, 16);

// 3. Region Lingkaran / Elips
HRGN rgn3 = CreateEllipticRgn(0, 0, 200, 200);

// 4. Region Poligon Segitiga / Bintang
POINT points[3] = { {100, 0}, {200, 200}, {0, 200} };
HRGN rgn4 = CreatePolygonRgn(points, 3, WINDING);
```

---

## Mengunci Batas Lukis: `SelectClipRgn`

Setelah membuat region, pasangkan ke `HDC` menggunakan fungsi **`SelectClipRgn`**:

```cpp
RECT cardRect = { 50, 50, 400, 250 };

// 1. Buat Region berujung tumpul seukuran kartu
HRGN hCardRgn = CreateRoundRectRgn(cardRect.left, cardRect.top, cardRect.right + 1, cardRect.bottom + 1, 14, 14);

// 2. Kunci kanvas ke region tersebut!
SelectClipRgn(hdc, hCardRgn);

// 3. Sekarang kamu bisa menggambar APA PUN (Gradasi, Foto, Pola)
// Seluruh goresan yang melewati batas lengkungan akan OTOMATIS TERPOTONG rapi!
DrawLinearGradient(hdc, cardRect, RGB(0, 80, 180), RGB(20, 20, 60));

// 4. Buka kembali kuncian clipping (reset ke normal)
SelectClipRgn(hdc, nullptr);

// 5. Hapus objek region dari memori
DeleteObject(hCardRgn);
```

> [!CAUTION] Urutan Membersihkan Region
> Selalu panggil `SelectClipRgn(hdc, nullptr)` **terlebih dahulu** untuk melepaskan region dari kanvas sebelum kamu memanggil `DeleteObject(hRgn)`.

---

## Operasi Boolean Region: `CombineRgn`

Kamu bisa menggabungkan dua bentuk region untuk membuat bentuk baru menggunakan fungsi **`CombineRgn`**:

```cpp
HRGN hResultRgn = CreateRectRgn(0, 0, 0, 0); // Wadah kosong

CombineRgn(
    hResultRgn,     // Handle region tujuan
    hRgnA,          // Region pertama
    hRgnB,          // Region kedua
    fnCombineMode   // Mode penggabungan
);
```

| Mode Penggabungan | Efek Operasi Bentuk |
| :--- | :--- |
| **`RGN_AND`** | Irisan (Hanya area yang saling bertumpuk antara A dan B) |
| **`RGN_OR`** | Gabungan (Area A ditambah Area B) |
| **`RGN_DIFF`** | Selisih (Area A dikurangi area yang dipotong oleh B) |
| **`RGN_XOR`** | Wilayah eksklusif (Semua area A dan B kecuali bagian yang bertumpuk) |

Contoh penggunaan: Membuat jendela aplikasi berbentuk lingkaran dengan lubang donat di tengahnya!

---

## Materi Selanjutnya

Sekarang kita sudah menguasai seluruh teknik menggambar dan masking batas. Mari kita bahas topik paling penting untuk membuat aplikasi desktop sesungguhnya: **Arsitektur Layouting & Membuat Custom Widgets dari Nol**.

👉 [Lanjut ke: 07. Arsitektur Layouting & Custom Widgets](/05-rendering/07-arsitektur-layouting-dan-widgets)

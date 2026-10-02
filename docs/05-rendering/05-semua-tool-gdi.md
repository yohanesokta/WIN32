# Semua Tool GDI, Gradasi & Pengukuran Teks

## Apa yang akan kita pelajari?

Pada bab sebelumnya, kita sudah mengenal kuas dan pena dasar. Namun, GDI memiliki gudang peralatan grafis yang jauh lebih lengkap: **Stock Objects**, **Hatch Brushes** (arsir), **ExtCreatePen** (ujung pena kustom), **GradientFill** (gradasi modern), fungsi pengukuran teks **`GetTextExtentPoint32W`**, hingga teknik isolasi kanvas menggunakan **`SaveDC`** dan **`RestoreDC`**.

Di bab ini, kita akan membongkar seluruh instrumen tersebut agar kamu memiliki kendali 100% atas kanvas Windows!

---

## 1. Stock Objects: Objek Bawaan Sistem Tanpa Perlu Dihapus

Windows memiliki objek-objek GDI standar yang sudah dialokasikan secara permanen di memori sistem. Kamu bisa mengambilnya menggunakan fungsi **`GetStockObject`**:

```cpp
// Kuas bawaan
HBRUSH hWhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
HBRUSH hBlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
HBRUSH hGrayBrush  = (HBRUSH)GetStockObject(GRAY_BRUSH);
HBRUSH hNullBrush  = (HBRUSH)GetStockObject(NULL_BRUSH); // Kuas transparan! (HOLLOW_BRUSH)

// Pena bawaan
HPEN hWhitePen = (HPEN)GetStockObject(WHITE_PEN);
HPEN hBlackPen = (HPEN)GetStockObject(BLACK_PEN);
HPEN hNullPen  = (HPEN)GetStockObject(NULL_PEN); // Pena tak terlihat (hanya mengisi warna tanpa garis tepi)
```

> [!TIP] Keuntungan Stock Object
> Objek yang didapat dari `GetStockObject` **TIDAK PERLU** dan **JANGAN** dihapus dengan `DeleteObject()`. Sistem operasi yang memelihara siklus hidupnya.

### Kapan Menggunakan `NULL_BRUSH`?
Jika kamu ingin menggambar bentuk lingkaran atau persegi **hanya garis tepinya saja** tanpa menimpa warna latar belakang di dalamnya, pilih `NULL_BRUSH`:
```cpp
SelectObject(hdc, GetStockObject(NULL_BRUSH));
SelectObject(hdc, hMyPen);
Rectangle(hdc, 50, 50, 200, 200); // Hanya garis tepinya yang tergambar!
```

---

## 2. Kuas Arsir (*Hatch Brushes*)

Selain kuas satu warna polos (`CreateSolidBrush`), kamu bisa membuat kuas bermotif arsir geometris menggunakan **`CreateHatchBrush`**:

| Gaya Hatch | Efek Visual Pola Arsir |
| :--- | :--- |
| **`HS_HORIZONTAL`** | Garis-garis horizontal sejajar `-----` |
| **`HS_VERTICAL`** | Garis-garis vertikal tegak lurus `|||||` |
| **`HS_FDIAGONAL`** | Garis miring ke kanan atas (45 derajat) `/////` |
| **`HS_BDIAGONAL`** | Garis miring ke kanan bawah `\\\\\` |
| **`HS_CROSS`** | Kisi-kisi kotak tegak `+++++` |
| **`HS_DIAGCROSS`** | Kisi-kisi diagonal intan / wajik `xxxxx` |

```cpp
// Buat kuas arsir silang berwarna biru muda
HBRUSH hHatch = CreateHatchBrush(HS_DIAGCROSS, RGB(0, 150, 255));
SelectObject(hdc, hHatch);
Rectangle(hdc, 20, 20, 200, 200);
DeleteObject(hHatch);
```

---

## 3. Gaya Garis Pena (*Pen Styles*)

Saat memanggil `CreatePen(fnStyle, nWidth, crColor)`:

* **`PS_SOLID`**: Garis lurus biasa tanpa putus.
* **`PS_DASH`**: Garis putus-putus strip `----`. *(Hanya berfungsi jika ketebalan = 1 piksel)*.
* **`PS_DOT`**: Garis titik-titik `....`. *(Hanya berfungsi jika ketebalan = 1 piksel)*.
* **`PS_DASHDOT`**: Kombinasi strip-titik `-.-.-.`.
* **`PS_INSIDEFRAME`**: Menggambar garis border **tepat di dalam** batas RECT (mencegah border meluber keluar 1 piksel).

---

## 4. Gradasi Halus Modern: `GradientFill` (`msimg32`)

Banyak pemula mengira Win32 GDI hanya bisa menggambar warna datar kuno. Padahal Windows menyediakan fungsi **`GradientFill`** di dalam library `msimg32.lib` untuk menghasilkan gradasi warna yang sangat halus menggunakan akselerasi grafis!

```cpp
#include <windows.h>

void DrawLinearGradient(HDC hdc, RECT rect, COLORREF topColor, COLORREF bottomColor)
{
    // Siapkan dua titik simpul (vertex): Atas dan Bawah
    TRIVERTEX vertex[2] = {};
    
    // Titik 1: Pojok Kiri Atas
    vertex[0].x     = rect.left;
    vertex[0].y     = rect.top;
    vertex[0].Red   = (COLOR16)(GetRValue(topColor) << 8);
    vertex[0].Green = (COLOR16)(GetGValue(topColor) << 8);
    vertex[0].Blue  = (COLOR16)(GetBValue(topColor) << 8);

    // Titik 2: Pojok Kanan Bawah
    vertex[1].x     = rect.right;
    vertex[1].y     = rect.bottom;
    vertex[1].Red   = (COLOR16)(GetRValue(bottomColor) << 8);
    vertex[1].Green = (COLOR16)(GetGValue(bottomColor) << 8);
    vertex[1].Blue  = (COLOR16)(GetBValue(bottomColor) << 8);

    GRADIENT_RECT gRect = { 0, 1 };
    
    // GRADIENT_FILL_RECT_V = Gradasi Vertikal (Atas ke Bawah)
    // GRADIENT_FILL_RECT_H = Gradasi Horizontal (Kiri ke Kanan)
    GradientFill(hdc, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
}
```

> [!NOTE] Mengapa Bitwise Shift `<< 8`?
> Komponen warna pada `COLORREF` bernilai 8-bit (0-255), sedangkan pada `TRIVERTEX` menggunakan `COLOR16` (16-bit, 0-65535). Operasi `<< 8` memetakan warna 8-bit ke skala 16-bit secara akurat.

---

## 5. Pengukuran Teks Sebelum Digambar (`GetTextExtentPoint32W`)

Saat membuat tombol atau label kustom, kamu sering perlu tahu: *"Berapa piksel sebenarnya lebar teks ini di layar?"* agar tombol bisa melebar secara otomatis menyesuaikan panjang kata (*auto-width*).

Gunakan **`GetTextExtentPoint32W`**:

```cpp
HFONT hFont = CreateFontW(...);
SelectObject(hdc, hFont);

std::wstring label = L"Klik Saya Sekarang!";
SIZE textSize;
GetTextExtentPoint32W(hdc, label.c_str(), (int)label.length(), &textSize);

// Sekarang kamu tahu dimensi presisinya!
int lebarTeksPiksel  = textSize.cx; // Contoh: 142 piksel
int tinggiTeksPiksel = textSize.cy; // Contoh: 18 piksel

// Buat tombol dengan padding 20 piksel di kiri dan kanan teks
int buttonWidth = lebarTeksPiksel + 40;
```

---

## 6. Isolasi State Kanvas: `SaveDC` & `RestoreDC`

Ketika kamu memiliki banyak widget yang saling bertumpuk (misalnya tombol di dalam kartu, kartu di dalam sidebar), setiap widget mungkin mengubah font, warna teks, pen, dan kuas.

Mengembalikan objek satu per satu secara manual sangat rawan bug dan membingungkan.

Solusinya: gunakan **`SaveDC`** dan **`RestoreDC`**!

```cpp
void RenderMyWidget(HDC hdc, RECT bounds)
{
    // 1. Simpan seluruh status konfigurasi kanvas saat ini ke stack
    int savedState = SaveDC(hdc);

    // 2. Ubah apa pun sesukamu (Font, Pen, Mode Transparan, Clipping Region)
    SelectObject(hdc, myCustomFont);
    SetTextColor(hdc, RGB(255, 100, 0));
    SetBkMode(hdc, TRANSPARENT);
    // ... gambar widget ...

    // 3. Kembalikan kanvas PERSIS seperti keadaan semula hanya dalam 1 panggilan!
    RestoreDC(hdc, savedState);
}
```

---

## Materi Selanjutnya

Sekarang persenjataan tool grafis kita sudah lengkap! Selanjutnya, mari kita pelajari cara mengunci batas gambar agar tidak meluber keluar menggunakan **Clipping Regions (`HRGN`)**.

👉 [Lanjut ke: 06. Clipping & Regions](/05-rendering/06-clipping-dan-region)

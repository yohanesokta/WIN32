# DwmExtendFrameIntoClientArea & Drop Shadow

## Apa yang akan kita pelajari?

Fungsi **`DwmExtendFrameIntoClientArea`** adalah salah satu fungsi paling terkenal di DWM API. Di era Windows Vista dan Windows 7, fungsi ini digunakan untuk membuat efek kaca transparan (*Aero Glass*).

Namun di era Windows 10 dan Windows 11, fungsi ini memiliki peran rahasia yang jauh lebih krusial: **mengaktifkan bayangan jendela (*native drop shadow*) pada jendela frameless kustom**.

---

## Memahami Struktur `MARGINS`

Fungsi ini menerima handle jendela dan sebuah struktur bernama `MARGINS`:

```cpp
#include <dwmapi.h>

MARGINS margins = {
    0, // cxLeftWidth: Perluasan dari sisi kiri
    0, // cxRightWidth: Perluasan dari sisi kanan
    1, // cyTopHeight: Perluasan dari sisi atas (1 piksel)
    0  // cyBottomHeight: Perluasan dari sisi bawah
};

DwmExtendFrameIntoClientArea(hwnd, &margins);
```

### Nilai Khusus: `-1` (Full Sheet of Glass)
Jika kamu mengisi semua margin dengan `-1`:
```cpp
MARGINS margins = { -1 };
DwmExtendFrameIntoClientArea(hwnd, &margins);
```
DWM akan memperluas frame ke **seluruh area jendela**. Di Windows 11, teknik ini sering digunakan saat kita ingin menggabungkan material *Mica* atau *Acrylic* ke seluruh latar belakang aplikasi.

---

## Trik Drop Shadow pada Jendela Kustom

Ketika kita menghapus title bar dan border default Windows (frameless window), Windows secara otomatis **mematikan bayangan jendela (*drop shadow*)**. Akibatnya, jendela terlihat pipih dan menyatu canggung dengan wallpaper desktop di belakangnya.

Dengan memanggil:
```cpp
MARGINS margins = { 0, 0, 1, 0 };
DwmExtendFrameIntoClientArea(hwnd, &margins);
```
Kita memberitahu DWM: *"Perluas frame DWM sebanyak 1 piksel saja di bagian atas."*

Meskipun perluasannya hanya 1 piksel yang nyaris tak terlihat, DWM Compositor akan mengenali jendela ini sebagai jendela ber-frame resmi, dan **menghidupkan kembali drop shadow asli Windows yang halus di sekeliling jendela!**

---

## Materi Selanjutnya

Sekarang kita tahu cara mengontrol komposisi DWM. Selanjutnya, mari kita pelajari bagaimana mengaktifkan **Dark Mode Immersive** dan efek material modern Windows 11 menggunakan `DwmSetWindowAttribute`.

👉 [Lanjut ke: 04. Dark Mode & Atribut Modern Windows 11](/07-dwm/04-dark-mode-dan-backdrop)

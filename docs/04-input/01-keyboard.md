# Input Keyboard (WM_KEYDOWN, WM_KEYUP & WM_CHAR)

## Apa yang akan kita buat?

Pada bab ini, kita akan mempelajari bagaimana menangkap ketukan tombol keyboard di Windows Win32. Kita akan memahami perbedaan mendasar antara tombol fisik (tombol panah, F1–F12, Space, Enter) dan karakter teks (huruf 'A', 'a', '@', '#'), serta cara mengidentifikasi tombol apa yang sedang ditekan oleh pengguna.

---

## Konsep yang Perlu Dipahami

Windows membagi input keyboard menjadi dua lapisan pesan yang berbeda:

```text
┌──────────────────────────────────────────────┐
│  Pengguna Menekan Tombol Fisik di Keyboard   │
└──────────────────────┬───────────────────────┘
                       │
                       ▼
┌──────────────────────────────────────────────┐
│         Pesan Keystroke: WM_KEYDOWN          │
└──────────────┬───────────────────────────────┘
               │                               
               ├─ wParam = Virtual Key (VK_*) ──> Logika Game & Shortcut
               │                               
               ▼
┌──────────────────────────────────────────────┐
│      TranslateMessage di Message Loop        │
└──────────────┬───────────────────────────────┘
               │  Mempertimbangkan Shift & CapsLock
               ▼
┌──────────────────────────────────────────────┐
│          Pesan Karakter: WM_CHAR             │
└──────────────┬───────────────────────────────┘
               │
               ▼
   Input Teks & Textbox (wchar_t)
```

1. **Pesan Keystroke (`WM_KEYDOWN` dan `WM_KEYUP`)**: Memberitahu status fisik tombol keyboard. `wParam` berisi kode tombol virtual (**Virtual-Key Codes** atau `VK_*`).
2. **Pesan Karakter (`WM_CHAR`)**: Dihasilkan oleh `TranslateMessage`. Pesan ini sudah mempertimbangkan tombol pendukung (Shift, Caps Lock, tata letak bahasa keyboard) dan langsung menghasilkan karakter teks Unicode yang valid (`wchar_t`).

---

## Teori Singkat: Mengapa Ada Dua Pesan Berbeda?

Bayangkan pengguna menekan tombol `A` di keyboard:
* Jika tombol `Shift` **tidak** ditekan:
  * `WM_KEYDOWN`: `wParam` bernilai `'A'` (kode virtual key untuk tombol fisik A).
  * `WM_CHAR`: `wParam` bernilai karakter `'a'` (huruf kecil).
* Jika tombol `Shift` **sedang ditekan**:
  * `WM_KEYDOWN`: `wParam` tetap bernilai `'A'`.
  * `WM_CHAR`: `wParam` bernilai karakter `'A'` (huruf kapital).

> [!TIP] Aturan Pemilihan Pesan
> * Gunakan **`WM_KEYDOWN`** untuk kontrol permainan (panah atas/bawah/kiri/kanan, WASD, Spasi), tombol pintasan (*shortcut* seperti Ctrl+S, Esc, Tab, F1).
> * Gunakan **`WM_CHAR`** jika kamu sedang membuat form input teks atau editor kode.

---

## Tabel Virtual-Key Codes Umum (`VK_*`)

| Konstanta Win32 | Tombol Fisik |
| :--- | :--- |
| `VK_SPACE` | Spasi |
| `VK_RETURN` | Enter |
| `VK_ESCAPE` | Esc |
| `VK_BACK` | Backspace |
| `VK_TAB` | Tab |
| `VK_LEFT`, `VK_RIGHT`, `VK_UP`, `VK_DOWN` | Tombol Panah Arah |
| `VK_SHIFT`, `VK_CONTROL`, `VK_MENU` | Tombol Shift, Ctrl, Alt |
| `'A'` sampai `'Z'` | Tombol Alfabet (gunakan huruf kapital langsung) |
| `'0'` sampai `'9'` | Tombol Angka |

Untuk memeriksa apakah tombol modifier seperti Shift atau Ctrl sedang ditekan saat event berlangsung:
```cpp
bool isShiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
bool isCtrlPressed  = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
```

---

## Contoh Kode Penanganan Keyboard di WndProc

```cpp
case WM_KEYDOWN:
{
    // wParam berisi Virtual Key Code
    switch (wParam)
    {
        case VK_ESCAPE:
            // Tutup aplikasi jika tombol Esc ditekan
            DestroyWindow(hwnd);
            return 0;

        case VK_SPACE:
            // Tangani tombol spasi (misalnya karakter melompat)
            break;

        case VK_LEFT:
            // Geser ke kiri
            break;
    }
    return 0;
}

case WM_CHAR:
{
    wchar_t ch = (wchar_t)wParam;
    // Tambahkan karakter ch ke buffer string teks
    return 0;
}
```

---

## Apa yang Terjadi?

Ketika tombol ditekan dan ditahan (*hold*), sistem keyboard repeat Windows akan mengirimkan serangkaian pesan `WM_KEYDOWN` berulang-ulang sampai pengguna melepas tombol, di mana satu pesan `WM_KEYUP` terakhir akan dikirimkan.

---

## Kesalahan Umum

1. **Memeriksa huruf kecil di `WM_KEYDOWN`**:
   ```cpp
   // SALAH: wParam pada WM_KEYDOWN selalu huruf kapital!
   if (wParam == 'a') { ... } 
   
   // BENAR:
   if (wParam == 'A') { ... }
   ```
2. **Lupa `TranslateMessage(&msg)`**: Jika baris ini dihapus dari Message Loop di `wWinMain`, pesan `WM_CHAR` tidak akan pernah dikirimkan ke `WindowProc`.

---

## Materi Selanjutnya

Sekarang kita sudah menguasai input tombol keyboard. Selanjutnya, mari kita pelajari bagaimana melacak pergerakan dan klik kursor mouse secara akurat.

👉 [Lanjut ke: 02. Mouse Input & Tracking](/04-input/02-mouse)

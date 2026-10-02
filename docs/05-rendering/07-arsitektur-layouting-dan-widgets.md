# Arsitektur Layouting & Custom Widgets dari Nol

## Apa yang akan kita buat?

Ketika membangun antarmuka desktop modern menggunakan C++ dan Win32 murni, kita tidak harus puas dengan tombol abu-abu kuno bawaan Windows 95. Kita bisa membangun **komponen UI (widgets) dan sistem layouting responsif kita sendiri dari nol**: tombol interaktif dengan efek hover/press, slider/trackbar yang bisa digeser, toggle switch, kartu informasi (cards), dan tata letak yang otomatis menyesuaikan ukuran jendela saat di-resize!

Di bab ini, kita akan membongkar arsitektur *windowless widget system* yang digunakan oleh aplikasi kelas dunia seperti Telegram Desktop, Blender, dan Spotify.

---

## 2 Pendekatan Membuat UI di Windows

Sebelum menulis kode, kamu harus tahu ada 2 cara membangun UI di Win32:

### 1. Pendekatan HWND Beranak (*Child Windows*)
Setiap tombol dan kotak input dibuat sebagai jendela anak (`HWND`) terpisah menggunakan `CreateWindowExW(0, L"BUTTON", ...)`.
* **Kelemahan**: Sangat sulit dikustomisasi tema gelapnya (*Dark Mode*), rentan berkedip (*flicker*), dan performanya lambat jika ada ribuan elemen.

### 2. Pendekatan Windowless Custom-Drawn (Rekomendasi Modern)
Aplikasi hanya memiliki **satu HWND utama**. Seluruh tombol, slider, dan kartu hanyalah struktur data C++ ringan di memori yang digambar langsung ke kanvas Memory DC (Double Buffering) dan menerima event melalui satu fungsi `WindowProc` utama.
* **Keunggulan**: 
  * 🚀 **Performa Instan**: Konsumsi RAM sangat rendah (< 10 MB).
  * 🎨 **Bebas 100%**: Kamu bisa membuat bentuk, warna gradasi, animasi, dan sudut melengkung apa pun tanpa batas!
  * ✨ **Zero Flicker**: Tampilan sangat halus pada 60 FPS.

---

## Konsep Box Model & Layout Responsif

Untuk membuat tampilan yang tidak rusak saat pengguna mengubah ukuran jendela (*resizing*), kita membagi kanvas menjadi sistem tata letak terstruktur:

```text
┌─────────────────────────────────────────────────────────────┐
│ Header Bar (Tinggi Tetap: 50px)                             │
├─────────────────┬───────────────────────────────────────────┤
│ Sidebar Kiri    │ Area Konten Utama (Fleksibel / Responsive)│
│ (Lebar Tetap)   │                                           │
│                 │  ┌─────────────────┐ ┌─────────────────┐  │
│  [Tombol 1]     │  │ Card 1          │ │ Card 2          │  │
│  [Tombol 2]     │  │ (Gradasi)       │ │ (Grid)          │  │
│  [Slider]       │  └─────────────────┘ └─────────────────┘  │
│  [Toggle]       │                                           │
│                 │                                           │
└─────────────────┴───────────────────────────────────────────┘
```

### Rumus Perhitungan Responsif di `WM_SIZE`
Kapan pun jendela diubah ukurannya, Windows mengirim pesan `WM_SIZE`. Di sini kita menghitung ulang seluruh koordinat `RECT`:

```cpp
void RecalculateLayout(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);
    int windowWidth  = rc.right;
    int windowHeight = rc.bottom;

    int padding = ScaleDpi(20, g_currentDpi);
    int sidebarWidth = ScaleDpi(260, g_currentDpi);

    // 1. Koordinat Sidebar Tetap di Sisi Kiri
    RECT sidebarRect = { padding, padding, padding + sidebarWidth, windowHeight - padding };

    // 2. Koordinat Konten Utama Menyesuaikan Sisa Ruang Kanan
    int contentLeft  = padding + sidebarWidth + ScaleDpi(16, g_currentDpi);
    int contentWidth = windowWidth - contentLeft - padding;
    RECT contentRect = { contentLeft, padding, contentLeft + contentWidth, windowHeight - padding };
}
```

---

## Anatomi State Machine Widget

Sebuah widget interaktif (seperti tombol) memiliki 3 status visual (*visual states*):

```text
               Kursor Mendekat                  Klik Mouse Ditekan
  [ Normal ] ──────────────────> [ Hover ] ─────────────────────────> [ Pressed ]
             <──────────────────           <─────────────────────────
                Kursor Pergi                    Klik Mouse Dilepas
```

### Struktur Data C++ Widget
```cpp
enum class WidgetState { Normal, Hover, Pressed };

struct CustomButton {
    RECT rect;           // Koordinat tombol di layar
    std::wstring text;   // Teks tombol
    WidgetState state;   // Status visual saat ini
};
```

---

## 3 Event Kunci Penanganan Input Widget

Di dalam `WindowProc`, kita menghubungkan pergerakan mouse ke state widget:

### 1. Deteksi Hover (`WM_MOUSEMOVE` & `TrackMouseEvent`)
```cpp
case WM_MOUSEMOVE:
{
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    // Aktifkan sinyal saat mouse keluar jendela
    TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 0 };
    TrackMouseEvent(&tme);

    // Periksa apakah kursor berada di dalam kotak tombol
    if (PtInRect(&myButton.rect, pt))
    {
        myButton.state = WidgetState::Hover;
    }
    else
    {
        myButton.state = WidgetState::Normal;
    }

    InvalidateRect(hwnd, nullptr, FALSE);
    return 0;
}
```

### 2. Deteksi Klik & Mouse Capture (`WM_LBUTTONDOWN`)
```cpp
case WM_LBUTTONDOWN:
{
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    if (PtInRect(&myButton.rect, pt))
    {
        myButton.state = WidgetState::Pressed;
        SetCapture(hwnd); // Kunci mouse agar drag tidak lepas
        InvalidateRect(hwnd, nullptr, FALSE);
    }
    return 0;
}
```

### 3. Eksekusi Aksi & Release Capture (`WM_LBUTTONUP`)
```cpp
case WM_LBUTTONUP:
{
    ReleaseCapture(); // Lepaskan kuncian mouse
    myButton.state = WidgetState::Normal;

    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    if (PtInRect(&myButton.rect, pt))
    {
        // KLIK BERHASIL! Jalankan fungsi aplikasimu di sini:
        OnButtonClicked();
    }

    InvalidateRect(hwnd, nullptr, FALSE);
    return 0;
}
```

---

## Contoh Nyata di Proyek `08-custom-widgets-layout`

Seluruh arsitektur layouting responsif, custom button, slider interaktif, toggle switch, gradient card, dan clipping regions terangkum di:
```text
examples/08-custom-widgets-layout/
├── CMakeLists.txt
└── main.cpp
```

### Cara Build & Menjalankan:
```powershell
cmake -S . -B build
cmake --build build
.\build\examples\08-custom-widgets-layout\Debug\widgets_layout_app.exe
```

Coba jalankan dan lakukan eksperimen:
1. Geser tombol slider untuk melihat pengisian warna biru dan persentase yang berubah secara halus.
2. Klik tombol utama untuk menambah counter.
3. Ubah ukuran jendela (*resize*): perhatikan bagaimana kartu di sebelah kanan melebar dan menyempit secara mulus mengikuti ukuran layar!

---

## Materi Selanjutnya

Sekarang kamu sudah memiliki fondasi desain grafis dan UI tingkat lanjut! Di modul berikutnya, kita akan membahas integrasi dengan layar resolusi tinggi: **Per-Monitor V2 DPI Awareness**.

👉 [Lanjut ke: 06. DPI Awareness & Multi-Monitor](/06-dpi/01-per-monitor-dpi)

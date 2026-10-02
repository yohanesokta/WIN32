# Snap Layouts & Window Resizing Modern

## Apa yang akan kita pelajari?

Pada Windows 11, fitur unggulan yang sangat disukai pengguna adalah **Snap Layouts**: menu *flyout* pilihan pembagian layar yang muncul secara otomatis saat kursor mouse diarahkan (*hover*) di atas tombol Maximize.

Di bab penutup modul ini, kita akan mempelajari trik Win32 untuk mengaktifkan Snap Layouts pada tombol maximize kustom kita, serta cara mencegah jendela menutupi Taskbar saat di-maximize menggunakan `WM_GETMINMAXINFO`.

---

## Rahasia Mengaktifkan Windows 11 Snap Layouts: `HTMAXBUTTON`

Biasanya kita mengembalikan `HTCLIENT` pada tombol kontrol agar aplikasi kita bisa menangani klik mouse secara manual.

Namun, Windows 11 memiliki fitur pintar: jika pada pesan `WM_NCHITTEST` kamu mengembalikan nilai **`HTMAXBUTTON`** tepat di koordinat tombol Maximize kustommu:
1. Windows 11 DWM akan mendeteksi tombol tersebut sebagai tombol Maximize resmi!
2. DWM akan menampilkan menu pop-up **Snap Layouts** native tepat di bawah tombol kustommu.
3. Efek animasi snap layar bawaan Windows 11 akan berjalan 100% mulus!

```cpp
case WM_NCHITTEST:
{
    // ... kalkulasi koordinat ...

    // Jika kursor berada tepat di atas tombol Maximize kustom:
    if (PtInRect(&rcMaximizeButton, pt))
    {
        return HTMAXBUTTON; // Aktifkan Windows 11 Snap Layouts flyout!
    }

    if (PtInRect(&rcCloseButton, pt)) return HTCLOSE;
    if (PtInRect(&rcMinimizeButton, pt)) return HTMINBUTTON;

    // ...
}
```

---

## Mencegah Jendela Menutupi Taskbar: `WM_GETMINMAXINFO`

Saat membuat frameless window, jika pengguna menekan Maximize, jendela terkadang melebar hingga **menutupi Taskbar** di bagian bawah layar.

Hal ini terjadi karena tanpa border default, Windows berasumsi ukuran maksimal jendela adalah ukuran fisik monitor utuh.

Untuk membatasi agar jendela hanya memenuhi **Area Kerja (*Work Area*)** di atas Taskbar, tangani pesan `WM_GETMINMAXINFO`:

```cpp
case WM_GETMINMAXINFO:
{
    MINMAXINFO* mmi = (MINMAXINFO*)lParam;

    // Ambil informasi monitor tempat jendela berada saat ini
    HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = {};
    mi.cbSize = sizeof(MONITORINFO);

    if (GetMonitorInfo(hMonitor, &mi))
    {
        // Sesuaikan ukuran maksimal jendela dengan rcWork (area monitor dikurangi tinggi Taskbar)
        mmi->ptMaxPosition.x = mi.rcWork.left - mi.rcMonitor.left;
        mmi->ptMaxPosition.y = mi.rcWork.top - mi.rcMonitor.top;
        mmi->ptMaxSize.x     = mi.rcWork.right - mi.rcWork.left;
        mmi->ptMaxSize.y     = mi.rcWork.bottom - mi.rcWork.top;
    }
    return 0;
}
```

---

## Contoh Proyek Lengkap: `07-custom-titlebar`

Seluruh arsitektur frameless window, hit testing, dragging native, tombol kontrol, dan dark theme DWM terangkum dalam:
```text
examples/07-custom-titlebar/
├── CMakeLists.txt
└── main.cpp
```

### Cara Build & Menjalankan:
```powershell
cmake -S . -B build
cmake --build build
.\build\examples\07-custom-titlebar\Debug\custom_titlebar_app.exe
```

Jalankan program dan rasakan sendiri kemulusannya:
* Klik dan geser title bar untuk memindahkan jendela.
* Double-click title bar untuk beralih maximize/restore.
* Sentuh tombol minimize, maximize, dan close untuk melihat efek hover modern.
* Tarik tepi atau sudut jendela mana saja untuk mengubah ukuran secara fleksibel.

---

## Kesimpulan Modul

Selamat! Kamu telah berhasil menaklukkan tantangan terbesar pengembangan desktop Windows native: **membangun custom title bar modern tanpa framework GUI raksasa**.

Di modul berikutnya, kita akan merangkum seluruh perjalanan belajar ini ke dalam **Project Akhir: Modern Win32 Application**.

👉 [Lanjut ke: 09. Project Akhir Modern Win32 App](/09-project/01-modern-app)

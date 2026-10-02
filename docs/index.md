---
layout: home

hero:
  name: "Win32 + DWM Indonesia"
  text: "Dokumentasi Pemrograman Desktop Native Windows"
  tagline: "Panduan komprehensif pengembangan aplikasi desktop menggunakan C++, Win32 API, dan Desktop Window Manager (DWM) dari tingkat dasar hingga implementasi antarmuka modern."
  actions:
    - theme: brand
      text: Memulai Tutorial
      link: /00-persiapan/01-kenapa-win32
    - theme: alt
      text: Kurikulum & Roadmap
      link: /roadmap
    - theme: alt
      text: Repositori Kode Sumber
      link: https://github.com/yohanesokta/WIN32

features:
  - title: Arsitektur Native Murni
    details: Pengembangan aplikasi desktop berkinerja tinggi tanpa dependensi runtime pihak ketiga. Menghasilkan biner mandiri dengan footprint memori minimal dan eksekusi instan.
  - title: Pemahaman Sistem Tingkat Rendah
    details: Mempelajari arsitektur internal sistem operasi Windows, mencakup siklus pesan, manajemen memori, handle kernel, subsistem GDI, hingga window procedure.
  - title: Komposisi Grafis Modern
    details: Integrasi Desktop Window Manager (DWM) untuk implementasi custom non-client area, frameless window, akselerasi GPU, dan tema visual Windows 11.
  - title: Toolchain Portabel Berbasis CMake
    details: Alur kompilasi mandiri menggunakan compiler MSVC dan sistem build CMake melalui terminal, tanpa ketergantungan pada IDE tertentu.
---

## Gambaran Umum

Dokumentasi ini disusun sebagai referensi teknis dan panduan rekayasa perangkat lunak untuk pengembangan aplikasi desktop Windows native. Seluruh materi dirancang secara berurutan, dimulai dari prinsip dasar sistem operasi hingga teknik pembuatan antarmuka pengguna modern berkinerja tinggi.

---

## Spesifikasi Teknis

* **Bahasa Pemrograman**: C++ (Standar C++20)
* **Antarmuka Pemrograman Aplikasi (API)**: Windows API (Win32), Desktop Window Manager (DWM), Graphics Device Interface (GDI)
* **Enkoding Karakter**: Unicode UTF-16 murni (`wchar_t`, API varian `W`)
* **Sistem Build**: CMake versi 3.20 ke atas
* **Compiler**: Microsoft Visual C++ (MSVC) dengan Windows SDK
* **Target Platform**: Microsoft Windows 10 dan Windows 11 (arsitektur x64)

---

## Struktur Kurikulum

1. **Persiapan dan Toolchain**: Konfigurasi lingkungan kompilasi, generator CMake, dan perbandingan subsystem Windows vs Console.
2. **Pondasi C++ dan Win32**: Tipe data primitif Windows, pengelolaan string Unicode, siklus handle kernel, dan entry point `wWinMain`.
3. **Mekanisme Message Loop**: Arsitektur event-driven, antrean pesan sistem, proses translasi masukan, dan fungsi callback `WindowProc`.
4. **Manajemen Masukan (Input)**: Penanganan virtual key codes keyboard, penerjemahan karakter teks, penelusuran posisi mouse, dan mekanisme mouse capture.
5. **Rendering Grafis dan Tata Letak (Layouting)**: Device Context (`HDC`), objek GDI (Pena, Kuas, Font), eliminasi flicker melalui double buffering, masking region (`HRGN`), gradasi warna (`msimg32`), dan perancangan widget mandiri.
6. **Skalabilitas Tampilan (DPI Awareness)**: Penerapan Per-Monitor V2 DPI Awareness untuk memastikan ketajaman visual pada monitor resolusi tinggi (High-DPI).
7. **Desktop Window Manager (DWM)**: Pemahaman arsitektur komposisi GPU, pemisahan client vs non-client area, ekstensi frame DWM, dan mode gelap terintegrasi.
8. **Custom Title Bar Modern**: Intersep `WM_NCCALCSIZE`, hit testing `WM_NCHITTEST`, pemeliharaan fitur native (dragging, resizing, snapping), serta implementasi tombol kontrol kustom.
9. **Aplikasi Terintegrasi (Proyek Akhir)**: Penggabungan seluruh konsep ke dalam aplikasi dashboard utuh dengan metadata resource biner terstandarisasi.

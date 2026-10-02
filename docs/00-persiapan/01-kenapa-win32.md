# Kenapa Belajar Win32 & DWM?

## Apa yang akan kita bahas?

Pada bab pembuka ini, kita akan membahas apa sebenarnya **Win32 API** dan **Desktop Window Manager (DWM)**, mengapa mempelajari teknologi ini sangat berharga di era modern, serta mengapa aplikasi Win32 native terasa sangat cepat dan efisien dibandingkan alternatif berbasis web atau framework besar.

---

## Konsep yang Perlu Dipahami

Sebelum melangkah ke baris kode C++, penting untuk memahami posisi Win32 di dalam ekosistem sistem operasi Windows.

```text
┌──────────────────────────────────────────────────────────┐
│      Aplikasi Pengguna (C++, C#, Rust, Python, dll)      │
├──────────────────────────────────────────────────────────┤
│           Win32 API (User32, Gdi32, Kernel32, DWM)       │  <-- Posisi Kita di Sini!
├──────────────────────────────────────────────────────────┤
│             Windows Subsystem & Windows Kernel           │
├──────────────────────────────────────────────────────────┤
│                        Perangkat Keras                   │
└──────────────────────────────────────────────────────────┘
```

* **Win32 API (Windows API)**: Antarmuka pemrograman aplikasi tingkat rendah inti dari Microsoft Windows. Hampir semua framework GUI yang berjalan di Windows (termasuk .NET WPF, Qt, Flutter, bahkan engine browser Chromium pada Electron) pada akhirnya memanggil Win32 API di lapisan terbawahnya.
* **DWM (Desktop Window Manager)**: Mesin pengomposisi grafis (*window compositor*) yang diperkenalkan sejak Windows Vista dan terus dimutakhirkan hingga Windows 11. DWM bertanggung jawab atas rendering jendela, efek transparansi (Aero, Acrylic, Mica), shadow, animasi transisi, dan frame jendela.

---

## Kenapa Masih Relevan Belajar Win32?

Mungkin kamu bertanya: *"Hari ini sudah ada Electron, Flutter, Qt, dan MAUI. Mengapa kita harus repot-repot belajar Win32 dan C++?"*

Berikut beberapa alasan kuatnya:

### 1. Performa dan Efisiensi Ekstrem
Aplikasi berbasis Electron seringkali membutuhkan RAM 150 MB hingga 400 MB hanya untuk menampilkan satu jendela kosong, karena mereka membawa seluruh instance browser Chromium dan runtime Node.js. 

Sebaliknya, aplikasi Win32 murni:
* Ukuran file executable (`.exe`): **kurang dari 1 MB**
* Penggunaan memori RAM: **di bawah 5–10 MB**
* Waktu startup: **instan (0.01 detik)**

### 2. Kontrol Penuh Tanpa Batasan (*Zero Abstraction Penalty*)
Ketika kamu menggunakan framework tingkat tinggi, kamu terkunci pada apa yang didukung oleh pembuat framework tersebut. Jika kamu ingin mengimplementasikan custom title bar dengan integrasi Windows 11 Snap Layouts, efek Mica transparan, atau manipulasi pesan jendela tingkat rendah, framework sering kali tidak memiliki fiturnya atau membutuhkan *native plugin* yang rumit. Dengan Win32, kamu berbicara langsung dengan sistem operasi.

### 3. Memahami Cara Kerja OS Sebenarnya
Memahami Win32 akan membuka rahasia bagaimana sistem operasi Windows bekerja: bagaimana mouse dan keyboard mengirimkan sinyal melalui *message queue*, bagaimana jendela digambar ulang melalui *device context*, dan bagaimana *window hierarchy* dikelola.

---

## Apa Itu Desktop Window Manager (DWM)?

Dahulu pada era Windows XP ke bawah, setiap aplikasi menggambar langsung ke buffer layar (*video memory*). Jika sebuah aplikasi macet (*hang*), menggeser jendela di atasnya akan meninggalkan jejak coretan putih yang berulang (*ghosting artifact*).

Sejak Windows Vista, Microsoft memperkenalkan **DWM**. Dengan DWM:
1. Setiap jendela aplikasi menggambar ke buffer memori *off-screen* miliknya sendiri.
2. DWM bertindak sebagai komposer: DWM mengambil buffer dari setiap jendela yang aktif, menerapkan dekorasi (border, shadow, rounded corners), dan menggabungkannya menjadi tampilan layar akhir menggunakan akselerasi GPU (DirectX).

Dengan memahami DWM, kita bisa memerintahkan Windows untuk:
* Mengaktifkan **Dark Mode** native pada title bar jendela.
* Memperluas area kerja hingga ke tepi layar (*extend frame into client area*).
* Menerapkan efek modern Windows 11 seperti **Mica** dan **Acrylic**.
* Membuat **Custom Title Bar** yang mulus tanpa merusak fungsionalitas native Windows (seperti snapping, maximize animation, dan aero peek).

---

## Kesimpulan

Belajar Win32 bukan berarti kembali ke masa lalu; ini adalah langkah untuk menguasai pondasi terdalam pengembangan software di Windows. Dengan menguasai Win32 + DWM, kamu memiliki kekuatan untuk membangun aplikasi desktop yang tidak hanya indah dan modern, tetapi juga luar biasa ringan dan stabil.

---

## Materi Selanjutnya

Di bab berikutnya, kita akan mempersiapkan alat tempur kita: compiler MSVC, Windows SDK, CMake, dan terminal.

👉 [Lanjut ke: 02. Setup Toolchain (MSVC & Windows SDK)](/00-persiapan/02-setup-toolchain)

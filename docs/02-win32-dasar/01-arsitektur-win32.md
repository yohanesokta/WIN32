# Arsitektur Aplikasi Desktop Win32

## Apa yang akan kita pelajari?

Sebelum menulis baris kode program, kita harus melihat "gambaran besar" (*big picture*) bagaimana sebuah aplikasi desktop Windows bekerja. Berbeda dengan program CLI (Command Line Interface) yang berjalan linier dari baris atas ke baris bawah lalu selesai, aplikasi desktop adalah sistem yang **berbasis event (event-driven)** dan harus tetap hidup menunggu interaksi pengguna.

---

## 4 Tahapan Utama Aplikasi Win32

Setiap aplikasi GUI Windows murni, dari aplikasi Notepad sederhana hingga game kompleks, selalu memiliki 4 tahapan siklus hidup berikut:

```text
┌─────────────────────────────────────────┐
│ 1. Registrasi Window Class              │  (RegisterClassExW)
└────────────────────┬────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────┐
│ 2. Instansiasi Jendela                  │  (CreateWindowExW)
└────────────────────┬────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────┐
│ 3. Tampilkan Jendela di Layar           │  (ShowWindow & UpdateWindow)
└────────────────────┬────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────┐
│ 4. Message Loop                         │  (GetMessage ➔ Translate ➔ Dispatch)
└────────────────────┬────────────────────┘
                     │  Event Mouse, Keyboard, Paint, Close
                     ▼
┌─────────────────────────────────────────┐
│ Window Procedure (WndProc)              │  (Fungsi Pemroses Event)
└────────────────────┬────────────────────┘
                     │  WM_DESTROY ➔ PostQuitMessage(0)
                     ▼
┌─────────────────────────────────────────┐
│ Aplikasi Selesai & Keluar Bersih        │
└─────────────────────────────────────────┘
```

### Tahap 1: Mendaftarkan Cetak Biru Jendela (*Window Class*)
Sebelum membuat jendela fisik, kita harus mendaftarkan "tipe" jendela kita ke sistem operasi. Kita memberitahu Windows karakteristik jendela kita:
* Apa warna latarnya?
* Kursor apa yang muncul saat mouse berada di atasnya?
* Fungsi C++ mana yang bertugas menangani event jendela ini (*Window Procedure*)?

### Tahap 2: Membuat Jendela Fisik (*CreateWindowExW*)
Setelah cetak biru terdaftar di sistem, kita meminta Windows untuk membuat instance jendela berdasarkan cetak biru tersebut. Di sini kita menentukan:
* Judul window pada title bar.
* Posisi koordinat (X, Y) dan dimensi (lebar, tinggi).
* Style jendela (apakah memiliki tombol minimize, maximize, border yang bisa di-resize, dsb.).

### Tahap 3: Menampilkan Jendela (*ShowWindow*)
Saat jendela pertama kali dibuat di memori kernel, jendela tersebut belum tentu langsung terlihat oleh pengguna. Kita secara eksplisit memanggil `ShowWindow` untuk memunculkannya di layar monitor.

### Tahap 4: Masuk ke *Message Loop*
Ini adalah inti yang membuat aplikasi tetap hidup. Windows akan menaruh setiap interaksi pengguna (klik mouse, ketukan keyboard, permintaan gambar ulang) ke dalam antrean pesan (*message queue*). Loop aplikasi kita akan terus mengambil pesan-pesan tersebut dan mengirimkannya ke fungsi pemroses kita (*WndProc*).

Ketika jendela ditutup dan pesan keluar dikirimkan, loop berhenti dan program berakhir secara bersih.

---

## Materi Selanjutnya

Mari kita bedah fungsi pembuka tempat seluruh proses ini dimulai: **`wWinMain`**.

👉 [Lanjut ke: 02. Entry Point Aplikasi: wWinMain](/02-win32-dasar/02-entry-point-wWinMain)

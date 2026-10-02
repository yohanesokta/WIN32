# Konsep Event-Driven di Windows

## Apa yang akan kita pelajari?

Dalam pemrograman CLI (seperti `cin >> angka` atau script otomatisasi), alur jalannya program ditentukan oleh kode program itu sendiri: baris demi baris secara berurutan.

Namun dalam aplikasi desktop dengan antarmuka grafis (GUI), **pengguna dan sistem operasi yang memegang kendali**. Pengguna bisa mengklik tombol kapan saja, mengetik di keyboard, mengubah ukuran jendela, atau menutup aplikasi sewaktu-waktu.

Pola ini disebut **Event-Driven Programming (Pemrograman Berbasis Peristiwa)**. Di bab ini, kita akan memahami konsep dasar pesan (*messages*) di Windows.

---

## Bagaimana Windows Mengirimkan Informasi ke Aplikasi Kita?

Setiap kali pengguna melakukan aksi fisik pada komputer, alur komunikasi berikut terjadi di tingkat sistem operasi:

```text
┌──────────────────────────────────────┐
│  Hardware: Keyboard / Mouse Monitor  │
└──────────────────┬───────────────────┘
                   │  Sinyal Driver Interrupt
                   ▼
┌──────────────────────────────────────┐
│          Windows OS Kernel           │
└──────────────────┬───────────────────┘
                   │  Bungkus ke dalam struktur MSG
                   ▼
┌──────────────────────────────────────┐
│     Message Queue Aplikasi Kita      │
└──────────────────┬───────────────────┘
                   │  Diambil oleh GetMessageW
                   ▼
┌──────────────────────────────────────┐
│        Message Loop Aplikasi         │
└──────────────────┬───────────────────┘
                   │  Translate & DispatchMessageW
                   ▼
┌──────────────────────────────────────┐
│      Window Procedure (WndProc)      │
└──────────────────┬───────────────────┘
                   │  Eksekusi switch-case kode kita
                   ▼
┌──────────────────────────────────────┐
│       Tampilan Layar Diperbarui      │
└──────────────────────────────────────┘
```

1. **Hardware Interrupt**: Kamu menekan tombol keyboard atau menggeser mouse. Driver perangkat keras memberi sinyal ke kernel Windows.
2. **Paket Pesan (Message Packet)**: Kernel Windows mendeteksi jendela mana yang sedang memiliki fokus atau berada di bawah posisi kursor mouse. Windows membungkus data tersebut ke dalam struktur pesan bernama `MSG`.
3. **Antrean Pesan (*Message Queue*)**: Windows meletakkan pesan tersebut ke dalam antrean milik thread aplikasi kita.
4. **Pengambilan (*Polling/Retrieval*)**: Aplikasi kita mengambil pesan tersebut satu per satu dari antrean menggunakan fungsi `GetMessageW`.
5. **Eksekusi (*Dispatch*)**: Aplikasi kita meneruskan pesan tersebut ke fungsi callback jendela kita (`WndProc`) untuk dieksekusi.

---

## Anatomi Pesan Windows yang Paling Sering Muncul

Berikut adalah daftar konstanta pesan Windows standar yang akan sering kamu temui:

| Nama Pesan (`uMsg`) | Kapan Dihasilkan? | Informasi Tambahan |
| :--- | :--- | :--- |
| **`WM_CREATE`** | Saat jendela baru saja selesai dibuat oleh `CreateWindowExW` | Tempat inisialisasi awal objek sebelum jendela tampil |
| **`WM_PAINT`** | Saat sebagian atau seluruh area jendela perlu digambar ulang | Dipicu saat jendela dibuka, di-resize, atau dipindahkan |
| **`WM_SIZE`** | Saat ukuran jendela berubah | `lParam` membawa lebar dan tinggi jendela yang baru |
| **`WM_MOUSEMOVE`** | Saat kursor mouse digerakkan di atas area jendela | `lParam` membawa koordinat (X, Y) kursor mouse |
| **`WM_LBUTTONDOWN`**| Saat tombol kiri mouse ditekan | Digunakan untuk mendeteksi klik atau drag |
| **`WM_KEYDOWN`** | Saat sebuah tombol keyboard ditekan | `wParam` membawa kode tombol virtual (*virtual key code*) |
| **`WM_CLOSE`** | Pengguna menekan tombol silang merah [X] atau `Alt + F4` | Kesempatan untuk menampilkan dialog konfirmasi *"Simpan perubahan?"* |
| **`WM_DESTROY`** | Jendela sedang dihancurkan dan dihapus dari layar | Tempat memanggil `PostQuitMessage(0)` untuk menghentikan aplikasi |

---

## Mengapa Aplikasi Menjadi "Not Responding"?

Pernahkah kamu melihat jendela aplikasi di Windows berubah warna menjadi putih pucat dan muncul tulisan **"(Not Responding)"** di title bar?

Penyebabnya hampir selalu sama: **Message Loop aplikasi macet atau diblokir**.

Jika kamu menjalankan proses kalkulasi yang sangat lama (misalnya loop berat 10 detik) langsung di dalam `WindowProc` tanpa thread terpisah:
* Fungsi `GetMessageW` tidak sempat dipanggil untuk mengambil pesan baru.
* Pesan dari Windows menumpuk di antrean (*queue*).
* Windows menganggap aplikasi telah mati (*hang*), lalu menampilkan overlay "Not Responding" kepada pengguna.

Oleh karena itu, aturan terpenting dalam Win32: **Window Procedure harus memproses pesan secepat kilat dan segera mengembalikan kontrol!**

---

## Materi Selanjutnya

Sekarang setelah paham konsepnya, mari kita bedah baris demi baris bagaimana loop pengambilan pesan dijalankan di kode C++.

👉 [Lanjut ke: 02. Bedah Tuntas Message Loop](/03-message-loop/02-message-loop-detail)

# Setup Toolchain (MSVC & Windows SDK)

## Apa yang akan kita siapkan?

Untuk membangun aplikasi Win32 native di Windows secara profesional dan efisien, kita tidak wajib membuka Visual Studio IDE yang berukuran puluhan gigabyte setiap kali ingin menulis kode. Kita dapat menggunakan kombinasi compiler **MSVC (Microsoft Visual C++)**, **Windows SDK**, dan **CMake** langsung dari terminal (seperti PowerShell atau Windows Terminal).

---

## Komponen yang Dibutuhkan

Berikut tiga komponen utama yang wajib terpasang di komputermu:

```text
┌────────────────────────────────────────────────────────┐
│ 1. Compiler: MSVC (cl.exe & link.exe)                  │
├────────────────────────────────────────────────────────┤
│ 2. Windows SDK (Header Windows.h & Library User32.lib) │
├────────────────────────────────────────────────────────┤
│ 3. Build System: CMake (dan Ninja jika ingin cepat)    │
└────────────────────────────────────────────────────────┘
```

### 1. Visual Studio Build Tools atau Visual Studio Community
Untuk mendapatkan compiler resmi Microsoft C++ (MSVC) dan Windows SDK:
1. Unduh **Visual Studio Installer** atau **Build Tools for Visual Studio** dari situs resmi Microsoft.
2. Saat instalasi, centang beban kerja (*workload*): **Desktop development with C++** (*Pengembangan desktop dengan C++*).
3. Pastikan komponen berikut tercentang di sisi kanan:
   * **MSVC v143 / v144** (C++ x64/x86 build tools)
   * **Windows 10 SDK** atau **Windows 11 SDK** (versi 10.0.19041 atau lebih baru)
   * **C++ CMake tools for Windows**

### 2. CMake
CMake adalah sistem otomatisasi build lintas-platform standar industri untuk C++. CMake yang akan mengatur cara mengompilasi file C++ kita, menambahkan library Windows yang sesuai, dan menghasilkan file biner.

Unduh CMake dari [cmake.org/download](https://cmake.org/download/) atau install via package manager Windows:
```powershell
winget install Kitware.CMake
```

### 3. Git
Untuk version control dan cloning repository:
```powershell
winget install Git.Git
```

---

## Verifikasi Instalasi dari Terminal

Buka **PowerShell** dan periksa apakah alat-alat tersebut telah terdaftar di sistem:

```powershell
# Periksa versi CMake
cmake --version

# Periksa versi Git
git --version
```

### Menjalankan Developer Command Prompt / PowerShell
Compiler MSVC (`cl.exe`) dan environment Windows SDK biasanya membutuhkan variabel lingkungan khusus (seperti path include dan lib). 

Jika kamu ingin menjalankan perintah compiler secara manual atau menggunakan generator *Ninja*, kamu bisa membuka **Developer PowerShell for VS** dari menu Start Windows, atau menginisialisasi environment MSVC di PowerShell biasa menggunakan script `vcvarsall.bat`:

```powershell
# Contoh mengaktifkan environment MSVC 64-bit:
& "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
```

> [!TIP] Keuntungan Menggunakan CMake
> Kabar baiknya: Jika kamu menggunakan **CMake** dengan generator default Visual Studio (`Visual Studio 17 2022` / `Visual Studio 18`), CMake akan **secara otomatis mendeteksi lokasi MSVC dan Windows SDK** tanpa kamu perlu mengatur environment variable atau membuka Developer Prompt secara manual!

---

## Struktur Folder Kerja

Agar rapi, buat satu direktori kerja utama untuk seluruh materi belajar kita:

```powershell
mkdir -p C:\BelajarWin32
cd C:\BelajarWin32
```

---

## Materi Selanjutnya

Sekarang lingkungan dasar kita sudah siap. Mari kita pelajari bagaimana cara kerja CMake dan bagaimana CMake mempermudah proses kompilasi kode C++ Win32 kita.

👉 [Lanjut ke: 03. Mengenal CMake & Ninja](/00-persiapan/03-cmake-dan-ninja)

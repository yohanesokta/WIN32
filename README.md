# Win32 + DWM Indonesia

Tutorial berbahasa Indonesia untuk mempelajari pengembangan aplikasi desktop Windows native menggunakan C++, Win32 API, dan Desktop Window Manager (DWM), mulai dari dasar hingga pembuatan custom title bar dan window modern.

[![Build](https://img.shields.io/github/actions/workflow/status/yohanesokta/WIN32/pages.yml?label=build)](../../actions)
[![GitHub Pages](https://img.shields.io/badge/docs-GitHub%20Pages-blue)](../../)
[![License](https://img.shields.io/github/license/yohanesokta/WIN32)](LICENSE)
[![C%2B%2B](https://img.shields.io/badge/C%2B%2B-20-blue)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-supported-064F8C)](https://cmake.org/)
[![Windows](https://img.shields.io/badge/platform-Windows-0078D4)](https://www.microsoft.com/windows)

> Dokumentasi Win32 yang dibuat untuk belajar, bukan sekadar membaca referensi API.

---

## Tentang

Win32 API merupakan salah satu fondasi utama pengembangan aplikasi desktop Windows. Namun, dokumentasi resmi Microsoft umumnya berfungsi sebagai referensi API dan sering kali tidak memberikan jalur pembelajaran yang jelas bagi pemula.

Project ini mencoba menyediakan jalur tersebut.

Materi disusun secara bertahap mulai dari konsep dasar C++, CMake, dan arsitektur aplikasi Win32 hingga message loop, window lifecycle, rendering, DPI awareness, DWM, dan custom title bar.

Fokus utama project:

* Bahasa Indonesia sebagai bahasa utama dokumentasi.
* Pembelajaran bertahap dari konsep dasar hingga project nyata.
* Contoh kode yang dapat langsung di-build.
* CMake sebagai build system.
* Tidak bergantung pada IDE tertentu.
* Menggunakan Win32 API secara langsung sebelum memperkenalkan abstraction.
* Membahas API Windows modern ketika relevan.
* Menjelaskan alasan di balik setiap konsep, bukan hanya cara menggunakannya.

---


## Requirements

Untuk mengikuti tutorial dan membuild contoh kode, diperlukan:

* Windows 10 atau Windows 11
* C++ compiler dengan Windows SDK
* MSVC
* CMake
* Ninja, opsional
* Git
* Node.js dan npm untuk dokumentasi

Visual Studio IDE tidak wajib digunakan. Build system menggunakan CMake sehingga project dapat digunakan bersama berbagai editor dan IDE yang mendukung CMake.

---

## Build Examples

Build seluruh contoh dari root repository:

```powershell
cmake -S . -B build
cmake --build build
```

Jika menggunakan Ninja:

```powershell
cmake -S . -B build -G Ninja
cmake --build build
```

Untuk membuild contoh tertentu:

```powershell
cd examples/01-hello-window

cmake -S . -B build
cmake --build build
```

Setiap example dirancang sebagai project yang mandiri sehingga dapat dipelajari dan dimodifikasi secara terpisah.

---

## Prinsip Pembelajaran

Tutorial ini mengikuti beberapa prinsip:

### Mulai dari Win32 API langsung

Abstraksi tidak diperkenalkan sebelum konsep Win32 yang mendasarinya dipahami.

Pembaca akan terlebih dahulu melihat API seperti:

```cpp
RegisterClassExW(...);
CreateWindowExW(...);
ShowWindow(...);
UpdateWindow(...);
```


## Referensi

Dokumentasi resmi Microsoft digunakan sebagai referensi teknis untuk memverifikasi API, parameter, behavior, dan kompatibilitas Windows.

Tutorial ini bukan pengganti dokumentasi resmi Microsoft. Tujuannya adalah menyediakan jalur pembelajaran yang lebih mudah sebelum pembaca menggunakan dokumentasi API sebagai referensi lanjutan.

Referensi utama:

* Microsoft Learn — Windows App Development
* Win32 API Reference
* Desktop Window Manager Documentation
* CMake Documentation
* VitePress Documentation

---

## Contributing

Kontribusi terbuka untuk siapa saja.

Beberapa bentuk kontribusi yang dapat membantu:

* memperbaiki kesalahan teknis;
* memperbaiki typo atau tata bahasa;
* memperjelas penjelasan;
* menambahkan contoh kode;
* menambahkan latihan;
* memperbaiki build example;
* melaporkan API yang sudah deprecated;
* melaporkan perbedaan behavior antar versi Windows.

Sebelum membuat perubahan besar, disarankan untuk membuka Issue terlebih dahulu agar perubahan dapat didiskusikan.

Pull Request yang memperbaiki dokumentasi atau contoh kode sangat dipersilakan.

---

## License

Source code dan materi dalam repository ini didistribusikan di bawah lisensi MIT.

Lihat [LICENSE](LICENSE) untuk informasi lengkap.

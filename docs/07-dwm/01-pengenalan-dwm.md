# Pengenalan Desktop Window Manager (DWM)

## Apa yang akan kita pelajari?

Sebelum Windows Vista, setiap aplikasi Windows menggambar langsung ke video memory layar utama. Jika aplikasi macet (*hang*), menggeser jendela di atasnya akan meninggalkan jejak coretan berulang (*ghosting artifact*).

Untuk menyelesaikan masalah ini selamanya, Microsoft memperkenalkan **Desktop Window Manager (DWM)**. Di bab ini, kita akan memahami apa itu DWM, bagaimana Windows me-render jendela di era modern menggunakan akselerasi GPU, dan mengapa DWM adalah kunci dari UI desktop modern.

---

## Konsep: Window Compositor Modern

DWM adalah sebuah **Compositing Window Manager** berbasis DirectX:

```mermaid
flowchart TD
    subgraph Proses Aplikasi Pengguna
    A[Aplikasi Notepad] -->|Render ke Buffer RAM/VRAM Sendiri| B[Off-screen Surface A]
    C[Aplikasi Kita] -->|Render ke Buffer RAM/VRAM Sendiri| D[Off-screen Surface B]
    end

    subgraph Desktop Window Manager (DWM Engine)
    B --> E[DWM Compositor GPU]
    D --> E
    E -->|Terapkan Drop Shadow, Animasi Transisi, Glass/Mica| F[Framebuffer Layar Monitor Utama]
    end
```

1. **Isolasi Buffer**: Setiap jendela tidak lagi menggambar langsung ke layar fisik. Mereka menggambar ke buffer permukaan *off-screen* masing-masing.
2. **GPU Compositing**: DWM mengambil buffer dari setiap jendela yang aktif, menggabungkannya, menambahkan efek bayangan (*drop shadow*), efek blur/material (Mica/Acrylic), dan animasi minimasi/maksimasi secara perangkat keras menggunakan GPU.
3. **Anti-Freeze Artifact**: Karena DWM selalu menyimpan salinan buffer terakhir dari setiap jendela, jika aplikasimu sedang sibuk melakukan proses berat, jendela lain yang digeser di atasnya tidak akan merusak tampilan aplikasimu!

---

## Library & Header DWM

Untuk mengakses fungsi-fungsi DWM dari C++, sertakan:
* **Header**: `#include <dwmapi.h>`
* **Library Linker**: `dwmapi.lib` (di CMake: `target_link_libraries(target PRIVATE dwmapi)`)

---

## Mengetahui Apakah Komposisi DWM Aktif

Pada Windows 7 ke bawah, pengguna bisa mematikan komposisi DWM (beralih ke tema Windows Classic). Namun sejak Windows 8, Windows 10, dan Windows 11, **DWM selalu aktif secara permanen**.

Kamu bisa memeriksa status komposisi menggunakan:

```cpp
#include <dwmapi.h>

BOOL isCompositionEnabled = FALSE;
HRESULT hr = DwmIsCompositionEnabled(&isCompositionEnabled);
if (SUCCEEDED(hr) && isCompositionEnabled)
{
    // DWM aktif dan siap digunakan
}
```

---

## Materi Selanjutnya

Sebelum kita mengubah warna dan efek DWM, kita harus memahami pembagian anatomi jendela Windows: **Client Area vs Non-Client Area**.

👉 [Lanjut ke: 02. Client Area vs Non-Client Area](/07-dwm/02-client-vs-nonclient)

import { defineConfig } from 'vitepress'

export default defineConfig({
  title: "Win32 + DWM Indonesia",
  description: "Tutorial Windows Win32 API dan DWM dari Nol Berbahasa Indonesia",
  base: process.env.GITHUB_ACTIONS ? '/WIN32/' : '/',
  
  head: [
    ['link', { rel: 'icon', href: '/favicon.ico' }]
  ],

  themeConfig: {
    nav: [
      { text: 'Beranda', link: '/' },
      { text: 'Roadmap Belajar', link: '/roadmap' },
      { text: 'Panduan Tutorial', link: '/00-persiapan/01-kenapa-win32' },
      { text: 'Project Akhir', link: '/09-project/01-modern-app' },
      { text: 'Contoh Kode', link: 'https://github.com/yohanesokta/WIN32/tree/main/examples' }
    ],

    sidebar: [
      {
        text: '00. Persiapan & Lingkungan',
        collapsed: false,
        items: [
          { text: 'Kenapa Win32 & DWM?', link: '/00-persiapan/01-kenapa-win32' },
          { text: 'Setup Toolchain (MSVC & SDK)', link: '/00-persiapan/02-setup-toolchain' },
          { text: 'Mengenal CMake & Ninja', link: '/00-persiapan/03-cmake-dan-ninja' }
        ]
      },
      {
        text: '01. C++ Dasar untuk Win32',
        collapsed: false,
        items: [
          { text: 'Tipe Data Windows (HWND, DWORD, dll)', link: '/01-cpp-dasar/01-tipe-data-windows' },
          { text: 'Unicode, wchar_t & String Windows', link: '/01-cpp-dasar/02-unicode-dan-string' },
          { text: 'Handle, Pointer & Konvensi Win32', link: '/01-cpp-dasar/03-handle-dan-pointer' }
        ]
      },
      {
        text: '02. Pondasi Win32 & Window Pertama',
        collapsed: false,
        items: [
          { text: 'Arsitektur Aplikasi Win32', link: '/02-win32-dasar/01-arsitektur-win32' },
          { text: 'Entry Point: wWinMain', link: '/02-win32-dasar/02-entry-point-wWinMain' },
          { text: 'Mendaftarkan Window Class', link: '/02-win32-dasar/03-window-class' },
          { text: 'Membuat Window (CreateWindowExW)', link: '/02-win32-dasar/04-membuat-window' }
        ]
      },
      {
        text: '03. Message Loop & Event Handling',
        collapsed: false,
        items: [
          { text: 'Konsep Event-Driven di Windows', link: '/03-message-loop/01-konsep-event-driven' },
          { text: 'Bedah Tuntas Message Loop', link: '/03-message-loop/02-message-loop-detail' },
          { text: 'Anatomi Window Procedure (WndProc)', link: '/03-message-loop/03-wndproc-anatomi' }
        ]
      },
      {
        text: '04. Input Keyboard & Mouse',
        collapsed: false,
        items: [
          { text: 'Keyboard Input (WM_KEYDOWN, WM_CHAR)', link: '/04-input/01-keyboard' },
          { text: 'Mouse Input & Tracking', link: '/04-input/02-mouse' }
        ]
      },
      {
        text: '05. Rendering & Grafis (GDI)',
        collapsed: false,
        items: [
          { text: 'WM_PAINT & Device Context (HDC)', link: '/05-rendering/01-hdc-dan-paint' },
          { text: 'Menggambar Bentuk, Teks & Warna', link: '/05-rendering/02-gdi-dasar' },
          { text: 'Timer & Animasi di Win32', link: '/05-rendering/03-timer-dan-animasi' },
          { text: 'Double Buffering (Mencegah Flicker)', link: '/05-rendering/04-double-buffering' }
        ]
      },
      {
        text: '06. DPI Awareness & Multi-Monitor',
        collapsed: false,
        items: [
          { text: 'Per-Monitor V2 DPI Awareness', link: '/06-dpi/01-per-monitor-dpi' }
        ]
      },
      {
        text: '07. Desktop Window Manager (DWM)',
        collapsed: false,
        items: [
          { text: 'Pengenalan Komposisi DWM', link: '/07-dwm/01-pengenalan-dwm' },
          { text: 'Client Area vs Non-Client Area', link: '/07-dwm/02-client-vs-nonclient' },
          { text: 'DwmExtendFrameIntoClientArea', link: '/07-dwm/03-dwm-extend-frame' },
          { text: 'Dark Mode & Atribut Modern Windows 11', link: '/07-dwm/04-dark-mode-dan-backdrop' }
        ]
      },
      {
        text: '08. Custom Title Bar Modern',
        collapsed: false,
        items: [
          { text: 'Menghilangkan Frame Default', link: '/08-custom-titlebar/01-frameless-window' },
          { text: 'Hit Testing (WM_NCHITTEST)', link: '/08-custom-titlebar/02-hit-testing' },
          { text: 'Tombol Kontrol (Min, Max, Close)', link: '/08-custom-titlebar/03-caption-buttons' },
          { text: 'Snap Layouts & Window Resizing', link: '/08-custom-titlebar/04-snap-layouts-resizing' }
        ]
      },
      {
        text: '09. Project Akhir',
        collapsed: false,
        items: [
          { text: 'Modern Win32 App (Octanio Shell)', link: '/09-project/01-modern-app' }
        ]
      }
    ],

    search: {
      provider: 'local'
    },

    socialLinks: [
      { icon: 'github', link: 'https://github.com/yohanesokta/WIN32' }
    ],

    footer: {
      message: 'Dirilis di bawah Lisensi MIT. Dibuat untuk komunitas developer Indonesia.',
      copyright: 'Copyright © 2026 Yohanes Oktanio'
    },

    outline: {
      level: [2, 3],
      label: 'Daftar Isi Halaman'
    },

    docFooter: {
      prev: 'Halaman Sebelumnya',
      next: 'Halaman Selanjutnya'
    }
  },

  markdown: {
    lineNumbers: true
  }
})

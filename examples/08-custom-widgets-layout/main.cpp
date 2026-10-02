#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <string>
#include <vector>
#include <algorithm>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

// ============================================================================
// DATA STRUKTUR WIDGET & STATE
// ============================================================================

enum class WidgetState {
    Normal,
    Hover,
    Pressed
};

struct CustomButton {
    RECT rect = {};
    std::wstring text;
    WidgetState state = WidgetState::Normal;
    bool isPrimary = false;
};

struct CustomSlider {
    RECT rect = {};
    float value = 0.65f; // 0.0f sampai 1.0f (65%)
    bool isDragging = false;
};

struct CustomToggle {
    RECT rect = {};
    bool isOn = true;
};

struct UIState {
    CustomButton btn1;
    CustomButton btn2;
    CustomSlider slider;
    CustomToggle toggle;
    float progressBarValue = 0.40f;
    int clickCounter = 0;
    bool isTrackingMouse = false;
    UINT currentDpi = 96;
};

static UIState g_ui;

// Forward declaration
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

inline int ScaleDpi(int value, UINT dpi)
{
    return MulDiv(value, dpi, 96);
}

// ============================================================================
// SISTEM LAYOUTING RESPONSIF
// ============================================================================

void RecalculateLayout(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);
    int windowWidth  = rc.right;
    int windowHeight = rc.bottom;

    UINT dpi = g_ui.currentDpi;
    int pad = ScaleDpi(20, dpi);
    int sidebarWidth = ScaleDpi(260, dpi);

    // 1. Sidebar di sebelah kiri
    int left = pad;
    int top  = pad + ScaleDpi(50, dpi); // Sisakan ruang header di atas

    // Tombol 1: Primary Button
    g_ui.btn1.rect = { left, top, left + sidebarWidth - pad, top + ScaleDpi(42, dpi) };
    g_ui.btn1.text = L"Tombol Utama (Primary)";
    g_ui.btn1.isPrimary = true;

    // Tombol 2: Secondary Button
    top += ScaleDpi(54, dpi);
    g_ui.btn2.rect = { left, top, left + sidebarWidth - pad, top + ScaleDpi(42, dpi) };
    g_ui.btn2.text = L"Reset Nilai (Secondary)";
    g_ui.btn2.isPrimary = false;

    // Slider
    top += ScaleDpi(70, dpi);
    g_ui.slider.rect = { left, top, left + sidebarWidth - pad, top + ScaleDpi(24, dpi) };

    // Toggle Switch
    top += ScaleDpi(60, dpi);
    g_ui.toggle.rect = { left, top, left + ScaleDpi(52, dpi), top + ScaleDpi(28, dpi) };
}

// ============================================================================
// FUNGSI MENGGAMBAR GRADASI (GradientFill via msimg32)
// ============================================================================

void DrawLinearGradient(HDC hdc, RECT rect, COLORREF topColor, COLORREF bottomColor)
{
    TRIVERTEX vertex[2] = {};
    vertex[0].x     = rect.left;
    vertex[0].y     = rect.top;
    vertex[0].Red   = (COLOR16)(GetRValue(topColor) << 8);
    vertex[0].Green = (COLOR16)(GetGValue(topColor) << 8);
    vertex[0].Blue  = (COLOR16)(GetBValue(topColor) << 8);
    vertex[0].Alpha = 0x0000;

    vertex[1].x     = rect.right;
    vertex[1].y     = rect.bottom;
    vertex[1].Red   = (COLOR16)(GetRValue(bottomColor) << 8);
    vertex[1].Green = (COLOR16)(GetGValue(bottomColor) << 8);
    vertex[1].Blue  = (COLOR16)(GetBValue(bottomColor) << 8);
    vertex[1].Alpha = 0x0000;

    GRADIENT_RECT gRect = { 0, 1 };
    GradientFill(hdc, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
}

// ============================================================================
// FUNGSI MENGGAMBAR WIDGET KUSTOM
// ============================================================================

void RenderCustomButton(HDC hdc, const CustomButton& btn)
{
    COLORREF bgCol, borderCol, textCol;

    if (btn.isPrimary)
    {
        if (btn.state == WidgetState::Pressed)      bgCol = RGB(0, 100, 200);
        else if (btn.state == WidgetState::Hover)   bgCol = RGB(30, 150, 255);
        else                                        bgCol = RGB(0, 122, 255);
        borderCol = RGB(50, 170, 255);
        textCol   = RGB(255, 255, 255);
    }
    else
    {
        if (btn.state == WidgetState::Pressed)      bgCol = RGB(35, 40, 52);
        else if (btn.state == WidgetState::Hover)   bgCol = RGB(45, 52, 68);
        else                                        bgCol = RGB(32, 36, 48);
        borderCol = RGB(60, 70, 90);
        textCol   = RGB(220, 230, 245);
    }

    HBRUSH hBrush = CreateSolidBrush(bgCol);
    HPEN hPen     = CreatePen(PS_SOLID, 1, borderCol);
    HBRUSH oBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN oPen     = (HPEN)SelectObject(hdc, hPen);

    RoundRect(hdc, btn.rect.left, btn.rect.top, btn.rect.right, btn.rect.bottom, 10, 10);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, textCol);
    RECT textRc = btn.rect;
    DrawTextW(hdc, btn.text.c_str(), -1, &textRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oBrush);
    SelectObject(hdc, oPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void RenderCustomSlider(HDC hdc, const CustomSlider& slider, UINT dpi)
{
    int trackHeight = ScaleDpi(6, dpi);
    int trackY = (slider.rect.top + slider.rect.bottom - trackHeight) / 2;
    int trackWidth = slider.rect.right - slider.rect.left;

    // Track Background (Abu-abu gelap)
    RECT bgTrack = { slider.rect.left, trackY, slider.rect.right, trackY + trackHeight };
    HBRUSH hBgBrush = CreateSolidBrush(RGB(40, 46, 60));
    FillRect(hdc, &bgTrack, hBgBrush);
    DeleteObject(hBgBrush);

    // Track Aktif (Biru menyala terisi sesuai value)
    int fillWidth = (int)(trackWidth * slider.value);
    RECT fillTrack = { slider.rect.left, trackY, slider.rect.left + fillWidth, trackY + trackHeight };
    HBRUSH hFillBrush = CreateSolidBrush(RGB(0, 160, 255));
    FillRect(hdc, &fillTrack, hFillBrush);
    DeleteObject(hFillBrush);

    // Handle / Knob Bulat yang bisa ditarik
    int knobRadius = ScaleDpi(9, dpi);
    int knobX = slider.rect.left + fillWidth;
    int knobY = (slider.rect.top + slider.rect.bottom) / 2;

    HBRUSH hKnobBrush = CreateSolidBrush(slider.isDragging ? RGB(255, 255, 255) : RGB(220, 235, 255));
    HPEN hKnobPen     = CreatePen(PS_SOLID, 2, RGB(0, 120, 215));
    HBRUSH oBrush = (HBRUSH)SelectObject(hdc, hKnobBrush);
    HPEN oPen     = (HPEN)SelectObject(hdc, hKnobPen);

    Ellipse(hdc, knobX - knobRadius, knobY - knobRadius, knobX + knobRadius, knobY + knobRadius);

    SelectObject(hdc, oBrush);
    SelectObject(hdc, oPen);
    DeleteObject(hKnobBrush);
    DeleteObject(hKnobPen);
}

void RenderCustomToggle(HDC hdc, const CustomToggle& toggle, UINT dpi)
{
    // Bentuk pil kapsul toggle
    HBRUSH hPillBrush = CreateSolidBrush(toggle.isOn ? RGB(46, 204, 113) : RGB(60, 65, 80));
    HPEN hPillPen     = CreatePen(PS_SOLID, 1, toggle.isOn ? RGB(39, 174, 96) : RGB(80, 85, 100));
    HBRUSH oBrush = (HBRUSH)SelectObject(hdc, hPillBrush);
    HPEN oPen     = (HPEN)SelectObject(hdc, hPillPen);

    int radius = toggle.rect.bottom - toggle.rect.top;
    RoundRect(hdc, toggle.rect.left, toggle.rect.top, toggle.rect.right, toggle.rect.bottom, radius, radius);

    // Lingkaran putih di dalam
    int knobPadding = ScaleDpi(3, dpi);
    int knobDiameter = radius - (knobPadding * 2);
    int knobLeft = toggle.isOn ? (toggle.rect.right - knobPadding - knobDiameter) : (toggle.rect.left + knobPadding);

    HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hWhiteBrush);
    Ellipse(hdc, knobLeft, toggle.rect.top + knobPadding, knobLeft + knobDiameter, toggle.rect.top + knobPadding + knobDiameter);

    SelectObject(hdc, oBrush);
    SelectObject(hdc, oPen);
    DeleteObject(hPillBrush);
    DeleteObject(hPillPen);
    DeleteObject(hWhiteBrush);
}

// ============================================================================
// ENTRY POINT & WNDPROC
// ============================================================================

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    const wchar_t CLASS_NAME[] = L"Win32Tutorial_WidgetsLayoutClass";

    WNDCLASSEXW wc = {};
    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassExW(&wc)) return 0;

    g_ui.currentDpi = GetDpiForSystem();
    int initW = ScaleDpi(960, g_ui.currentDpi);
    int initH = ScaleDpi(620, g_ui.currentDpi);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Tutorial Win32: Sistem Layouting & Custom Widgets Lengkap",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        initW, initH,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) return 0;

    // Dark Mode DWM
    BOOL dark = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

    RecalculateLayout(hwnd);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_SIZE:
        {
            RecalculateLayout(hwnd);
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }

        case WM_DPICHANGED:
        {
            g_ui.currentDpi = LOWORD(wParam);
            RECT* pRect = (RECT*)lParam;
            SetWindowPos(hwnd, nullptr, pRect->left, pRect->top,
                pRect->right - pRect->left, pRect->bottom - pRect->top,
                SWP_NOZORDER | SWP_NOACTIVATE);
            RecalculateLayout(hwnd);
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            POINT pt = { x, y };

            if (!g_ui.isTrackingMouse)
            {
                TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 0 };
                TrackMouseEvent(&tme);
                g_ui.isTrackingMouse = true;
            }

            // Tangani drag pada slider
            if (g_ui.slider.isDragging)
            {
                int trackWidth = g_ui.slider.rect.right - g_ui.slider.rect.left;
                int relativeX = x - g_ui.slider.rect.left;
                g_ui.slider.value = std::clamp((float)relativeX / (float)trackWidth, 0.0f, 1.0f);
                InvalidateRect(hwnd, nullptr, FALSE);
                return 0;
            }

            // Periksa hover pada Button 1
            WidgetState b1State = PtInRect(&g_ui.btn1.rect, pt) ? WidgetState::Hover : WidgetState::Normal;
            WidgetState b2State = PtInRect(&g_ui.btn2.rect, pt) ? WidgetState::Hover : WidgetState::Normal;

            if (g_ui.btn1.state != b1State || g_ui.btn2.state != b2State)
            {
                g_ui.btn1.state = b1State;
                g_ui.btn2.state = b2State;
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }

        case WM_MOUSELEAVE:
        {
            g_ui.isTrackingMouse = false;
            g_ui.btn1.state = WidgetState::Normal;
            g_ui.btn2.state = WidgetState::Normal;
            g_ui.slider.isDragging = false;
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            POINT pt = { x, y };

            if (PtInRect(&g_ui.btn1.rect, pt))
            {
                g_ui.btn1.state = WidgetState::Pressed;
                g_ui.clickCounter++;
                SetCapture(hwnd);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            else if (PtInRect(&g_ui.btn2.rect, pt))
            {
                g_ui.btn2.state = WidgetState::Pressed;
                g_ui.clickCounter = 0;
                g_ui.slider.value = 0.50f;
                SetCapture(hwnd);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            else if (PtInRect(&g_ui.slider.rect, pt))
            {
                g_ui.slider.isDragging = true;
                int trackWidth = g_ui.slider.rect.right - g_ui.slider.rect.left;
                int relativeX = x - g_ui.slider.rect.left;
                g_ui.slider.value = std::clamp((float)relativeX / (float)trackWidth, 0.0f, 1.0f);
                SetCapture(hwnd);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            else if (PtInRect(&g_ui.toggle.rect, pt))
            {
                g_ui.toggle.isOn = !g_ui.toggle.isOn;
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }

        case WM_LBUTTONUP:
        {
            ReleaseCapture();
            g_ui.slider.isDragging = false;
            g_ui.btn1.state = WidgetState::Normal;
            g_ui.btn2.state = WidgetState::Normal;
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int width  = clientRect.right;
            int height = clientRect.bottom;
            UINT dpi   = g_ui.currentDpi;

            // === Double Buffering ===
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            // 1. Background Dasar Gelap
            HBRUSH bgBrush = CreateSolidBrush(RGB(18, 20, 28));
            FillRect(memDC, &clientRect, bgBrush);
            DeleteObject(bgBrush);

            // Font
            HFONT hTitleFont = CreateFontW(ScaleDpi(20, dpi), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            HFONT hCardFont  = CreateFontW(ScaleDpi(15, dpi), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            HFONT hBodyFont  = CreateFontW(ScaleDpi(13, dpi), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            HFONT oldFont    = (HFONT)SelectObject(memDC, hTitleFont);

            SetBkMode(memDC, TRANSPARENT);

            // 2. Header Banner
            SetTextColor(memDC, RGB(255, 255, 255));
            TextOutW(memDC, ScaleDpi(20, dpi), ScaleDpi(16, dpi), L"Win32 Custom Painting & Layouting Engine", 40);

            // 3. Panel Kiri (Sidebar Controls)
            int pad = ScaleDpi(20, dpi);
            int sidebarW = ScaleDpi(260, dpi);
            RECT sidebarRect = { pad, ScaleDpi(50, dpi), pad + sidebarW, height - pad };

            HBRUSH sideBrush = CreateSolidBrush(RGB(25, 28, 38));
            HPEN sidePen     = CreatePen(PS_SOLID, 1, RGB(42, 48, 65));
            HBRUSH oBrush    = (HBRUSH)SelectObject(memDC, sideBrush);
            HPEN oPen        = (HPEN)SelectObject(memDC, sidePen);
            RoundRect(memDC, sidebarRect.left, sidebarRect.top, sidebarRect.right, sidebarRect.bottom, 14, 14);

            // Label Kontrol di Sidebar
            SelectObject(memDC, hCardFont);
            SetTextColor(memDC, RGB(0, 180, 255));
            TextOutW(memDC, pad + ScaleDpi(15, dpi), ScaleDpi(65, dpi), L"Panel Kontrol Widget", 20);

            // Render Button 1 & 2
            SelectObject(memDC, hBodyFont);
            RenderCustomButton(memDC, g_ui.btn1);
            RenderCustomButton(memDC, g_ui.btn2);

            // Render Slider
            SetTextColor(memDC, RGB(200, 210, 225));
            int sliderPercent = (int)(g_ui.slider.value * 100.0f);
            std::wstring strSlider = L"Slider Nilai: " + std::to_wstring(sliderPercent) + L"%";
            TextOutW(memDC, g_ui.slider.rect.left, g_ui.slider.rect.top - ScaleDpi(22, dpi), strSlider.c_str(), (int)strSlider.length());
            RenderCustomSlider(memDC, g_ui.slider, dpi);

            // Render Toggle Switch
            std::wstring strToggle = L"Toggle Fitur: " + std::wstring(g_ui.toggle.isOn ? L"AKTIF" : L"MATI");
            TextOutW(memDC, g_ui.toggle.rect.left, g_ui.toggle.rect.top - ScaleDpi(22, dpi), strToggle.c_str(), (int)strToggle.length());
            RenderCustomToggle(memDC, g_ui.toggle, dpi);

            // Counter Klik
            std::wstring strClick = L"Jumlah Klik: " + std::to_wstring(g_ui.clickCounter);
            TextOutW(memDC, g_ui.toggle.rect.left, g_ui.toggle.rect.bottom + ScaleDpi(25, dpi), strClick.c_str(), (int)strClick.length());

            // 4. Area Konten Kanan (Responsive Grid Layout)
            int contentLeft = pad + sidebarW + ScaleDpi(16, dpi);
            int contentWidth = width - contentLeft - pad;

            if (contentWidth > ScaleDpi(250, dpi))
            {
                // Kartu 1: Gradient Card (GradientFill)
                int card1H = ScaleDpi(150, dpi);
                RECT card1Rect = { contentLeft, ScaleDpi(50, dpi), contentLeft + contentWidth, ScaleDpi(50, dpi) + card1H };

                // Gunakan Clipping Region agar gradasi memiliki sudut membulat sempurna!
                HRGN hCard1Rgn = CreateRoundRectRgn(card1Rect.left, card1Rect.top, card1Rect.right + 1, card1Rect.bottom + 1, 14, 14);
                SelectClipRgn(memDC, hCard1Rgn);

                DrawLinearGradient(memDC, card1Rect, RGB(18, 50, 110), RGB(30, 20, 60));

                // Gambar teks di dalam gradasi yang terpotong clipping
                SelectObject(memDC, hCardFont);
                SetTextColor(memDC, RGB(255, 255, 255));
                TextOutW(memDC, card1Rect.left + ScaleDpi(20, dpi), card1Rect.top + ScaleDpi(18, dpi), L"1. Gradient Card (msimg32 / GradientFill)", 40);

                SelectObject(memDC, hBodyFont);
                SetTextColor(memDC, RGB(200, 220, 255));
                RECT card1Desc = { card1Rect.left + ScaleDpi(20, dpi), card1Rect.top + ScaleDpi(50, dpi), card1Rect.right - ScaleDpi(20, dpi), card1Rect.bottom - ScaleDpi(15, dpi) };
                const wchar_t desc1[] =
                    L"Kartu ini dirender menggunakan GradientFill murni tanpa loop piksel lambat. "
                    L"Dikombinasikan dengan SelectClipRgn(CreateRoundRectRgn), sudut-sudut gradasi membulat secara presisi!";
                DrawTextW(memDC, desc1, -1, &card1Desc, DT_LEFT | DT_WORDBREAK);

                // Matikan clipping region
                SelectClipRgn(memDC, nullptr);
                DeleteObject(hCard1Rgn);

                // Kartu 2: Penjelasan Sistem Box Model & Layout
                int card2Top = card1Rect.bottom + ScaleDpi(16, dpi);
                RECT card2Rect = { contentLeft, card2Top, contentLeft + contentWidth, height - pad };

                HBRUSH c2Brush = CreateSolidBrush(RGB(24, 28, 38));
                HPEN c2Pen     = CreatePen(PS_SOLID, 1, RGB(42, 48, 65));
                SelectObject(memDC, c2Brush);
                SelectObject(memDC, c2Pen);
                RoundRect(memDC, card2Rect.left, card2Rect.top, card2Rect.right, card2Rect.bottom, 14, 14);

                SelectObject(memDC, hCardFont);
                SetTextColor(memDC, RGB(46, 204, 113));
                TextOutW(memDC, card2Rect.left + ScaleDpi(20, dpi), card2Rect.top + ScaleDpi(18, dpi), L"2. Aturan Emas Layouting Desktop Modern", 39);

                SelectObject(memDC, hBodyFont);
                SetTextColor(memDC, RGB(180, 195, 215));
                RECT card2Desc = { card2Rect.left + ScaleDpi(20, dpi), card2Rect.top + ScaleDpi(52, dpi), card2Rect.right - ScaleDpi(20, dpi), card2Rect.bottom - ScaleDpi(15, dpi) };
                const wchar_t desc2[] =
                    L"Prinsip Layouting Win32 yang Dipakai:\n\n"
                    L"• Tangani WM_SIZE: Hitung ulang seluruh koordinat RECT saat jendela diperbesar/perkecil.\n"
                    L"• Skala DPI (MulDiv): Kalikan seluruh padding dan dimensi dengan DPI / 96.\n"
                    L"• Relative Positioning: Sidebar berukuran tetap (fixed width), sedangkan Area Konten Utama dinamis (width - sidebarWidth - padding).\n"
                    L"• State Management: Pisahkan logika state widget (Normal, Hover, Pressed) dari fungsi gambar.\n"
                    L"• Clipping Regions (HRGN): Lindungi batas elemen agar tidak meluber keluar kotak pembatas!";
                DrawTextW(memDC, desc2, -1, &card2Desc, DT_LEFT | DT_WORDBREAK);

                DeleteObject(c2Brush);
                DeleteObject(c2Pen);
            }

            // Blit ke layar monitor
            BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

            // Cleanup GDI
            SelectObject(memDC, oldFont);
            SelectObject(memDC, oBrush);
            SelectObject(memDC, oPen);
            SelectObject(memDC, oldBitmap);

            DeleteObject(hTitleFont);
            DeleteObject(hCardFont);
            DeleteObject(hBodyFont);
            DeleteObject(sideBrush);
            DeleteObject(sidePen);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

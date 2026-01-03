// #include "utiSystem.h"
//
// #include <iostream>
//
// #if _WIN32
// #include <Windows.h>
// #endif
//
// namespace uti::system {
//   Image loadImageFromClipboard() {
// #if _WIN32
//     if (!OpenClipboard(nullptr)) {
//       std::cerr << "Nie można otworzyć schowka\n";
//       return Image {};
//     }
//
//     HANDLE hData = GetClipboardData(CF_DIB);
//     if (!hData) {
//       std::cerr << "Brak obrazu w schowku\n";
//       CloseClipboard();
//       return Image {};
//     }
//
//     BITMAPINFO* bmi = static_cast<BITMAPINFO*>(GlobalLock(hData));
//     if (!bmi) {
//       CloseClipboard();
//       return Image {};
//     }
//
//     int width = bmi->bmiHeader.biWidth;
//     int height = bmi->bmiHeader.biHeight;
//     int bpp = bmi->bmiHeader.biBitCount;
//
//     // Raylib używa Image z RGBA
//     int size = width * height * 4;
//     unsigned char* pixels = new unsigned char[size];
//
//     // Skopiowanie danych z DIB do RGBA
//     unsigned char* src = reinterpret_cast<unsigned char*>(bmi) + bmi->bmiHeader.biSize + bmi->bmiHeader.biClrUsed * sizeof(RGBQUAD);
//     for (int y = 0; y < height; y++) {
//       for (int x = 0; x < width; x++) {
//         int srcIdx = (y * width + x) * (bpp / 8);
//         int dstIdx = ((height - 1 - y) * width + x) * 4; // odwrócenie Y
//         if (bpp == 24) {
//           pixels[dstIdx + 0] = src[srcIdx + 2];
//           pixels[dstIdx + 1] = src[srcIdx + 1];
//           pixels[dstIdx + 2] = src[srcIdx + 0];
//           pixels[dstIdx + 3] = 255;
//         } else if (bpp == 32) {
//           pixels[dstIdx + 0] = src[srcIdx + 2];
//           pixels[dstIdx + 1] = src[srcIdx + 1];
//           pixels[dstIdx + 2] = src[srcIdx + 0];
//           pixels[dstIdx + 3] = src[srcIdx + 3];
//         }
//       }
//     }
//
//     Image img = { pixels, width, height, 1, 4 }; // Raylib Image
//     GlobalUnlock(hData);
//     CloseClipboard();
//
//     return img;
// #else
//     // Zapisz schowek do tymczasowego pliku PNG
//     system("xclip -selection clipboard -t image/png -o > /tmp/clipboard.png 2>/dev/null");
//
//     // Załaduj obraz przez Raylib
//     Image img = LoadImage("/tmp/clipboard.png");
//     if (img.data == nullptr) {
//       std::cerr << "Brak obrazu w schowku lub nieobsługiwany format\n";
//       return Image {};
//     }
//
//     return img;
// #endif
//
//   }
// }
#pragma once

enum class KeyCode
{
  Lbutton = 0x01, // Left mouse button
  Rbutton = 0x02, // Right mouse button
  Cancel = 0x03, // Control-break processing
  Mbutton = 0x04, // Middle mouse button (three-button mouse)
  Xbutton1 = 0x05, // X1 mouse button
  Xbutton2 = 0x07, // X2 mouse button
  Back = 0x08, // BACKSPACE key
  Clear = 0x0C, // CLEAR key
  Return = 0x0D, // ENTER key
  Shift = 0x10, // SHIFT key
  Control = 0x11, // CTRL key
  Menu = 0x12, // ALT key
  Pause = 0x13, // PAUSE key
  Capital = 0x14, // CAPS LOCK key
  Escape = 0x1B, // ESC key
  Space = 0x20, // SPACEBAR
  Prior = 0x21, // PAGE UP key
  Next = 0x22, // PAGE DOWN key
  End = 0x23, // END key
  Home = 0x24, // HOME key
  Left = 0x25, // LEFT ARROW key
  Up = 0x26, // UP ARROW key
  Right = 0x27, // RIGHT ARROW key
  Down = 0x28, // DOWN ARROW key
  Select = 0x29, // SELECT key
  Print = 0x2A, // PRINT key
  Execute = 0x2B, // EXECUTE key
  Snapshot = 0x2C, // PRINT SCREEN key
  Insert = 0x2D, // INS key
  Delete = 0x2E, // DEL key
  Help = 0x2F, // HELP key
  Zero = 0x30, // 0 key
  One = 0x31, // 1 key
  Two = 0x32, // 2 key
  Three = 0x33, // 3 key
  Four = 0x34, // 4 key
  Five = 0x35, // 5 key
  Six = 0x36, // 6 key
  Seven = 0x37, // 7 key
  Eight = 0x38, // 8 key
  Nine = 0x39, // 9 key
  A = 0x41, // A key
  B = 0x42, // B key
  C = 0x43, // C key
  D = 0x44, // D key
  E = 0x45, // E key
  F = 0x46, // F key
  G = 0x47, // G key
  H = 0x48, // H key
  I = 0x49, // I key
  J = 0x4A, // J key
  K = 0x4B, // K key
  L = 0x4C, // L key
  M = 0x4D, // M key
  N = 0x4E, // N key
  O = 0x4F, // O key
  P = 0x50, // P key
  Q = 0x51, // Q key
  R = 0x52, // R key
  S = 0x53, // S key
  T = 0x54, // T key
  U = 0x55, // U key
  V = 0x56, // V key
  W = 0x57, // W key
  X = 0x58, // X key
  Y = 0x59, // Y key
  Z = 0x5A, // Z key
  Lwin = 0x5B, // Left Windows key (Natural keyboard)
  Rwin = 0x5C, // Right Windows key (Natural keyboard)
  Apps = 0x5E, // Reserved
  Sleep = 0x5F, // Computer Sleep key
  Numpad0 = 0x60, // Numeric keypad 0 key
  Numpad1 = 0x61, // Numeric keypad 1 key
  Numpad2 = 0x62, // Numeric keypad 2 key
  Numpad3 = 0x63, // Numeric keypad 3 key
  Numpad4 = 0x64, // Numeric keypad 4 key
  Numpad5 = 0x65, // Numeric keypad 5 key
  Numpad6 = 0x66, // Numeric keypad 6 key
  Numpad7 = 0x67, // Numeric keypad 7 key
  Numpad8 = 0x68, // Numeric keypad 8 key
  Numpad9 = 0x69, // Numeric keypad 9 key
  Multiply = 0x6A, // Multiply key
  Add = 0x6B, // Add key
  Separator = 0x6C, // Separator key
  Subtract = 0x6D, // Subtract key
  Decimal = 0x6E, // Decimal key
  Divide = 0x6F, // Divide key
  F1 = 0x70, // F1 key
  F2 = 0x71, // F2 key
  F3 = 0x72, // F3 key
  F4 = 0x73, // F4 key
  F5 = 0x74, // F5 key
  F6 = 0x75, // F6 key
  F7 = 0x76, // F7 key
  F8 = 0x77, // F8 key
  F9 = 0x78, // F9 key
  F10 = 0x79, // F10 key
  F11 = 0x7A, // F11 key
  F12 = 0x7B, // F12 key
  F13 = 0x7C, // F13 key
  F14 = 0x7D, // F14 key
  F15 = 0x7E, // F15 key
  F16 = 0x7F, // F16 key
  F17 = 0x80, // F17 key
  F18 = 0x81, // F18 key
  F19 = 0x82, // F19 key
  F20 = 0x83, // F20 key
  F21 = 0x84, // F21 key
  F22 = 0x85, // F22 key
  F23 = 0x86, // F23 key
  Numlock = 0x90, // NUM LOCK key
  Lshift = 0xA0, // Left SHIFT key
  Rshift = 0xA1, // Right SHIFT key
  Lcontrol = 0xA2, // Left CONTROL key
  Rcontrol = 0xA3, // Right CONTROL key
  Lmenu = 0xA4, // Left MENU key
  Rmenu = 0xA5, // Right MENU key
  Browser_back = 0xA6, // Browser Back key
  Browser_forward = 0xA7, // Browser Forward key
  Browser_refresh = 0xA8, // Browser Refresh key
  Browser_stop = 0xA9, // Browser Stop key
  Browser_search = 0xAA, // Browser Search key
  Browser_favorites = 0xAB, // Browser Favorites key
  Browser_home = 0xAC, // Browser Start and Home key
  Volume_mute = 0xAD, // Volume Mute key
  Volume_down = 0xAE, // Volume Down key
  Volume_up = 0xAF, // Volume Up key
  Media_next_track = 0xB0, // Next Track key
  Media_prev_track = 0xB1, // Previous Track key
  Media_stop = 0xB2, // Stop Media key
  Media_play_pause = 0xB3, // Play/Pause Media key
  Launch_mail = 0xB4, // Start Mail key
  Launch_media_select = 0xB5, // Select Media key
  Launch_app1 = 0xB6, // Start Application 1 key
  Oem_1 = 0xBA, // For the US standard keyboard, the ';:' key
  Oem_plus = 0xBB, // For any country/region, the '+' key
  Oem_comma = 0xBC, // For any country/region, the ',' key
  Oem_minus = 0xBD, // For any country/region, the '-' key
  Oem_period = 0xBE, // For any country/region, the '.' key
  Oem_2 = 0xBF, // For the US standard keyboard, the '/?' key
  Oem_3 = 0xBF, // For the US standard keyboard, the '~' key
  Oem_4 = 0xDB, // For the US standard keyboard, the '[{' key
  Oem_5 = 0xDC, // For the US standard keyboard, the '\|' key
  Oem_6 = 0xDD, // For the US standard keyboard, the ']}' key
  Oem_7 = 0xDE, // For the US standard keyboard, the 'single-quote/double-quote' key
  Oem_8 = 0xE1, // Used for miscellaneous characters; it can vary by keyboard.
  Oem_102 = 0xE2, // Either the angle bracket key or the backslash key on the RT 102-key keyboard
};

#include "VirtKBDefs.h"

static const short s_refs_id_VKB_KEY_NUMPAD_ENTER[] = { 90, 96 };
static const short s_refs_id_VKB_KEY_RETURN[] = { 44, 65 };

const VirtKeyDef g_vkbKeyDefs[] =
{
	{ VKB_KEY_F1, "f1", "F1", 131, 1, 1, 0, 0x3b, { 44, 33, 16, 62, 65, 62, 93, 33 }, FLAG_POLY|FLAG_STFNKEY, { 1, 290, 33, 321 }, 1, 0 },
	{ VKB_KEY_F2, "f2", "F2", 132, 1, 1, 0, 0x3c, { 94, 33, 66, 62, 115, 62, 143, 33 }, FLAG_POLY|FLAG_STFNKEY, { 34, 290, 66, 321 }, 1, 0 },
	{ VKB_KEY_F3, "f3", "F3", 133, 1, 1, 0, 0x3d, { 144, 33, 116, 62, 165, 62, 193, 33 }, FLAG_POLY|FLAG_STFNKEY, { 67, 290, 99, 321 }, 1, 0 },
	{ VKB_KEY_F4, "f4", "F4", 134, 1, 1, 0, 0x3e, { 194, 33, 166, 62, 215, 62, 243, 33 }, FLAG_POLY|FLAG_STFNKEY, { 100, 290, 132, 321 }, 1, 0 },
	{ VKB_KEY_F5, "f5", "F5", 135, 1, 1, 0, 0x3f, { 244, 33, 216, 62, 265, 62, 293, 33 }, FLAG_POLY|FLAG_STFNKEY, { 133, 290, 165, 321 }, 1, 0 },
	{ VKB_KEY_F6, "f6", "F6", 136, 1, 1, 0, 0x40, { 294, 33, 266, 62, 315, 62, 343, 33 }, FLAG_POLY|FLAG_STFNKEY, { 166, 290, 198, 321 }, 1, 0 },
	{ VKB_KEY_F7, "f7", "F7", 137, 1, 1, 0, 0x41, { 344, 33, 316, 62, 365, 62, 393, 33 }, FLAG_POLY|FLAG_STFNKEY, { 199, 290, 231, 321 }, 1, 0 },
	{ VKB_KEY_F8, "f8", "F8", 138, 1, 1, 0, 0x42, { 394, 33, 366, 62, 415, 62, 443, 33 }, FLAG_POLY|FLAG_STFNKEY, { 232, 290, 264, 321 }, 1, 0 },
	{ VKB_KEY_F9, "f9", "F9", 139, 1, 1, 0, 0x43, { 444, 33, 416, 62, 465, 62, 493, 33 }, FLAG_POLY|FLAG_STFNKEY, { 265, 290, 297, 321 }, 1, 0 },
	{ VKB_KEY_F10, "f10", "F10", 140, 1, 1, 1, 0x44, { 494, 33, 466, 62, 515, 62, 543, 33 }, FLAG_POLY|FLAG_STFNKEY, { 298, 290, 330, 321 }, 1, 0 },
	{ VKB_KEY_ESC, "Esc", "Esc", 111, 1, 1, 0, 0x1, { 16, 80, 47, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 1, 323, 33, 354 }, 1, 0 },
	{ VKB_KEY_1, "1!", "1", 8, 1, 1, 0, 0x2, { 48, 80, 79, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 34, 323, 66, 354 }, 1, 0 },
	{ VKB_KEY_2, "2\"", "2", 9, 1, 1, 0, 0x3, { 80, 80, 111, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 67, 323, 99, 354 }, 1, 0 },
	{ VKB_KEY_3, "3?", "3", 10, 1, 1, 0, 0x4, { 112, 80, 143, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 100, 323, 132, 354 }, 1, 0 },
	{ VKB_KEY_4, "4$", "4", 11, 1, 1, 0, 0x5, { 144, 80, 175, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 133, 323, 165, 354 }, 1, 0 },
	{ VKB_KEY_5, "5%", "5", 12, 1, 1, 0, 0x6, { 176, 80, 207, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 166, 323, 198, 354 }, 1, 0 },
	{ VKB_KEY_6, "6^", "6", 13, 1, 1, 0, 0x7, { 208, 80, 239, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 199, 323, 231, 354 }, 1, 0 },
	{ VKB_KEY_7, "7&", "7", 14, 1, 1, 0, 0x8, { 240, 80, 271, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 232, 323, 264, 354 }, 1, 0 },
	{ VKB_KEY_8, "8*", "8", 15, 1, 1, 0, 0x9, { 272, 80, 303, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 265, 323, 297, 354 }, 1, 0 },
	{ VKB_KEY_9, "9(", "9", 16, 1, 1, 0, 0xa, { 304, 80, 335, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 298, 323, 330, 354 }, 1, 0 },
	{ VKB_KEY_0, "0)", "0", 7, 1, 1, 0, 0xb, { 336, 80, 367, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 331, 323, 363, 354 }, 1, 0 },
	{ VKB_KEY_SUBTRACT, "-_", "- (Minus)", 69, 1, 1, 0, 0xc, { 368, 80, 399, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 364, 323, 396, 354 }, 1, 0 },
	{ VKB_KEY_EQUAL, "=+", "= (Equal)", 70, 1, 1, 0, 0xd, { 400, 80, 431, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 397, 323, 429, 354 }, 1, 0 },
	{ VKB_KEY_BACKQUOTE, "`?", "` (Backquote)", 68, 1, 1, 0, 0x29, { 432, 80, 463, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 430, 323, 462, 354 }, 1, 0 },
	{ VKB_KEY_BACKSPACE, "Bkspc", "Backspace", 67, 1, 1, 0, 0xe, { 464, 80, 511, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 463, 323, 495, 354 }, 1, 0 },
	{ VKB_KEY_HELP, "Help", "Help", -1, 1, 1, 0, 0x62, { 520, 80, 567, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 496, 323, 528, 354 }, 1, 0 },
	{ VKB_KEY_UNDO, "Undo", "Undo", -1, 1, 1, 0, 0x61, { 568, 80, 615, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 529, 323, 561, 354 }, 1, 0 },
	{ VKB_KEY_NUMPAD_LEFTPAREN, "Num(", "Numpad (", 162, 1, 1, 0, 0x63, { 624, 80, 655, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 562, 323, 594, 354 }, 1, 0 },
	{ VKB_KEY_NUMPAD_RIGHTPAREN, "Num)", "Numpad )", 163, 1, 1, 0, 0x64, { 656, 80, 687, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 595, 323, 627, 354 }, 1, 0 },
	{ VKB_KEY_NUMPAD_DIVIDE, "Num/", "Numpad /", 154, 1, 1, 0, 0x65, { 688, 80, 719, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 628, 323, 660, 354 }, 1, 0 },
	{ VKB_KEY_NUMPAD_MULTIPLY, "Num*", "Numpad *", 155, 1, 1, 0, 0x66, { 720, 80, 751, 111, 0, 0, 0, 0 }, FLAG_STKEY, { 661, 323, 693, 354 }, 1, 0 },
	{ VKB_KEY_TAB, "Tab", "Tab", 61, 1, 1, 0, 0xf, { 16, 112, 63, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 1, 356, 33, 387 }, 1, 0 },
	{ VKB_KEY_Q, "q", "Q", 45, 1, 1, 0, 0x10, { 64, 112, 95, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 35, 356, 66, 387 }, 1, 0 },
	{ VKB_KEY_W, "w", "W", 51, 1, 1, 0, 0x11, { 96, 112, 127, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 68, 356, 99, 387 }, 1, 0 },
	{ VKB_KEY_E, "e", "E", 33, 1, 1, 0, 0x12, { 128, 112, 159, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 101, 356, 132, 387 }, 1, 0 },
	{ VKB_KEY_R, "r", "R", 46, 1, 1, 0, 0x13, { 160, 112, 191, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 134, 356, 165, 387 }, 1, 0 },
	{ VKB_KEY_T, "t", "T", 48, 1, 1, 0, 0x14, { 192, 112, 223, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 167, 356, 198, 387 }, 1, 0 },
	{ VKB_KEY_Y, "y", "Y", 53, 1, 1, 0, 0x15, { 224, 112, 255, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 200, 356, 231, 387 }, 1, 0 },
	{ VKB_KEY_U, "u", "U", 49, 1, 1, 0, 0x16, { 256, 112, 287, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 233, 356, 264, 387 }, 1, 0 },
	{ VKB_KEY_I, "i", "I", 37, 1, 1, 0, 0x17, { 288, 112, 319, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 266, 356, 297, 387 }, 1, 0 },
	{ VKB_KEY_O, "o", "O", 43, 1, 1, 0, 0x18, { 320, 112, 351, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 299, 356, 330, 387 }, 1, 0 },
	{ VKB_KEY_P, "p", "P", 44, 1, 1, 0, 0x19, { 352, 112, 383, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 332, 356, 363, 387 }, 1, 0 },
	{ VKB_KEY_LEFTBRACKET, "[{", "[ (Left Bracket)", 71, 1, 1, 0, 0x1a, { 384, 112, 415, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 365, 356, 396, 387 }, 1, 0 },
	{ VKB_KEY_RIGHTBRACKET, "]}", "] (Right Bracket)", 72, 1, 1, 0, 0x1b, { 416, 112, 447, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 398, 356, 429, 387 }, 1, 0 },
	{ VKB_KEY_RETURN, "Ret", "Return", 66, 1, 1, 0, 0x1c, { 448, 112, 479, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 431, 356, 462, 387 }, 2, s_refs_id_VKB_KEY_RETURN },
	{ VKB_KEY_DELETE, "Del", "Delete", 112, 1, 1, 0, 0x53, { 480, 112, 511, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 464, 356, 495, 387 }, 1, 0 },
	{ VKB_KEY_INSERT, "Ins", "Insert", 124, 1, 1, 0, 0x52, { 520, 112, 551, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 497, 356, 528, 387 }, 1, 0 },
	{ VKB_KEY_UPARROW, "Up", "Arrow Up", 19, 1, 1, 0, 0x48, { 552, 112, 583, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 530, 356, 561, 387 }, 1, 0 },
	{ VKB_KEY_HOME, "Hm Clr", "Home", -1, 1, 1, 0, 0x47, { 584, 112, 615, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 563, 356, 594, 387 }, 1, 0 },
	{ VKB_KEY_NUMPAD_7, "Num7", "Numpad 7", 151, 1, 1, 0, 0x67, { 624, 112, 655, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 596, 356, 627, 387 }, 1, 0 },
	{ VKB_KEY_NUMPAD_8, "Num8", "Numpad 8", 152, 1, 1, 0, 0x68, { 656, 112, 687, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 629, 356, 660, 387 }, 1, 0 },
	{ VKB_KEY_NUMPAD_9, "Num9", "Numpad 9", 153, 1, 1, 0, 0x69, { 688, 112, 719, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 662, 356, 693, 387 }, 1, 0 },
	{ VKB_KEY_NUMPAD_SUBTRACT, "Num-", "Numpad -", 156, 1, 1, 0, 0x4a, { 720, 112, 751, 143, 0, 0, 0, 0 }, FLAG_STKEY, { 695, 356, 726, 387 }, 1, 0 },
	{ VKB_KEY_CONTROL, "Ctrl", "Control", 113, 1, 1, 0, 0x1d, { 16, 144, 71, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 1, 389, 33, 420 }, 1, 0 },
	{ VKB_KEY_A, "a", "A", 29, 1, 1, 0, 0x1e, { 72, 144, 103, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 34, 389, 66, 420 }, 1, 0 },
	{ VKB_KEY_S, "s", "S", 47, 1, 1, 0, 0x1f, { 104, 144, 135, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 67, 389, 99, 420 }, 1, 0 },
	{ VKB_KEY_D, "d", "D", 32, 1, 1, 0, 0x20, { 136, 144, 167, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 100, 389, 132, 420 }, 1, 0 },
	{ VKB_KEY_F, "f", "F", 34, 1, 1, 0, 0x21, { 168, 144, 199, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 133, 389, 165, 420 }, 1, 0 },
	{ VKB_KEY_G, "g", "G", 35, 1, 1, 0, 0x22, { 200, 144, 231, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 166, 389, 198, 420 }, 1, 0 },
	{ VKB_KEY_H, "h", "H", 36, 1, 1, 0, 0x23, { 232, 144, 263, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 199, 389, 231, 420 }, 1, 0 },
	{ VKB_KEY_J, "j", "J", 38, 1, 1, 0, 0x24, { 264, 144, 295, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 232, 389, 264, 420 }, 1, 0 },
	{ VKB_KEY_K, "k", "K", 39, 1, 1, 0, 0x25, { 296, 144, 327, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 265, 389, 297, 420 }, 1, 0 },
	{ VKB_KEY_L, "l", "L", 40, 1, 1, 0, 0x26, { 328, 144, 359, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 298, 389, 330, 420 }, 1, 0 },
	{ VKB_KEY_SEMICOLON, ";:", "; (Semicolon)", 74, 1, 1, 0, 0x27, { 360, 144, 391, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 331, 389, 363, 420 }, 1, 0 },
	{ VKB_KEY_APOSTROPHE, "'@", "' (Apostrophe)", 75, 1, 1, 0, 0x28, { 392, 144, 423, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 364, 389, 396, 420 }, 1, 0 },
	{ VKB_KEY_RETURN, "Ret", "Return", 66, 0, 0, 0, 0x1c, { 424, 144, 479, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 397, 389, 429, 420 }, 2, s_refs_id_VKB_KEY_RETURN },
	{ VKB_KEY_TILDA, "~#", "~ (Tilda)", -1, 1, 1, 0, 0x2b, { 480, 144, 511, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 430, 389, 462, 420 }, 1, 0 },
	{ VKB_KEY_LEFTARROW, "Left", "Arrow Left", 21, 1, 1, 0, 0x4b, { 520, 144, 551, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 463, 389, 495, 420 }, 1, 0 },
	{ VKB_KEY_DOWNARROW, "Down", "Arrow Down", 20, 1, 1, 0, 0x50, { 552, 144, 583, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 496, 389, 528, 420 }, 1, 0 },
	{ VKB_KEY_RIGHTARROW, "Right", "Arrow Right", 22, 1, 1, 0, 0x4d, { 584, 144, 615, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 529, 389, 561, 420 }, 1, 0 },
	{ VKB_KEY_NUMPAD_4, "Num4", "Numpad 4", 148, 1, 1, 0, 0x6a, { 624, 144, 655, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 562, 389, 594, 420 }, 1, 0 },
	{ VKB_KEY_NUMPAD_5, "Num5", "Numpad 5", 149, 1, 1, 0, 0x6b, { 656, 144, 687, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 595, 389, 627, 420 }, 1, 0 },
	{ VKB_KEY_NUMPAD_6, "Num6", "Numpad 6", 150, 1, 1, 0, 0x6c, { 688, 144, 719, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 628, 389, 660, 420 }, 1, 0 },
	{ VKB_KEY_NUMPAD_ADD, "Num+", "Numpad +", 157, 1, 1, 0, 0x4e, { 720, 144, 751, 175, 0, 0, 0, 0 }, FLAG_STKEY, { 661, 389, 693, 420 }, 1, 0 },
	{ VKB_KEY_LEFTSHIFT, "Lshift", "Left Shift", 59, 1, 1, 0, 0x2a, { 16, 176, 55, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 1, 422, 33, 453 }, 1, 0 },
	{ VKB_KEY_BACKSLASH, "\\|", "\\ (Backslash)", 73, 1, 1, 0, 0x60, { 56, 176, 87, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 35, 422, 66, 453 }, 1, 0 },
	{ VKB_KEY_Z, "z", "Z", 54, 1, 1, 0, 0x2c, { 88, 176, 119, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 68, 422, 99, 453 }, 1, 0 },
	{ VKB_KEY_X, "x", "X", 52, 1, 1, 0, 0x2d, { 120, 176, 151, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 101, 422, 132, 453 }, 1, 0 },
	{ VKB_KEY_C, "c", "C", 31, 1, 1, 0, 0x2e, { 152, 176, 183, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 134, 422, 165, 453 }, 1, 0 },
	{ VKB_KEY_V, "v", "V", 50, 1, 1, 0, 0x2f, { 184, 176, 215, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 167, 422, 198, 453 }, 1, 0 },
	{ VKB_KEY_B, "b", "B", 30, 1, 1, 0, 0x30, { 216, 176, 247, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 200, 422, 231, 453 }, 1, 0 },
	{ VKB_KEY_N, "n", "N", 42, 1, 1, 0, 0x31, { 248, 176, 279, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 233, 422, 264, 453 }, 1, 0 },
	{ VKB_KEY_M, "m", "M", 41, 1, 1, 0, 0x32, { 280, 176, 311, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 266, 422, 297, 453 }, 1, 0 },
	{ VKB_KEY_COMMA, ",<", ", (Comma)", 55, 1, 1, 0, 0x33, { 312, 176, 343, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 299, 422, 330, 453 }, 1, 0 },
	{ VKB_KEY_PERIOD, ".>", ". (Period)", 56, 1, 1, 0, 0x34, { 344, 176, 375, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 332, 422, 363, 453 }, 1, 0 },
	{ VKB_KEY_FORWARDSLASH, "/?", "/ (Forward Slash)", 76, 1, 1, 0, 0x35, { 376, 176, 407, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 365, 422, 396, 453 }, 1, 0 },
	{ VKB_KEY_RIGHTSHIFT, "Rshift", "Right Shift", 60, 1, 1, 0, 0x36, { 408, 176, 455, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 398, 422, 429, 453 }, 1, 0 },
	{ VKB_KEY_NUMPAD_1, "Num1", "Numpad 1", 145, 1, 1, 0, 0x6d, { 624, 176, 655, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 431, 422, 462, 453 }, 1, 0 },
	{ VKB_KEY_NUMPAD_2, "Num2", "Numpad 2", 146, 1, 1, 0, 0x6e, { 656, 176, 687, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 464, 422, 495, 453 }, 1, 0 },
	{ VKB_KEY_NUMPAD_3, "Num3", "Numpad 3", 147, 1, 1, 0, 0x6f, { 688, 176, 719, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 497, 422, 528, 453 }, 1, 0 },
	{ VKB_KEY_NUMPAD_ENTER, "NumEnt", "Numpad Enter", 160, 1, 1, 0, 0x72, { 720, 176, 751, 207, 0, 0, 0, 0 }, FLAG_STKEY, { 530, 422, 561, 453 }, 2, s_refs_id_VKB_KEY_NUMPAD_ENTER },
	{ VKB_KEY_ALTERNATE, "Alt", "Alternate", 57, 1, 1, 0, 0x38, { 56, 208, 103, 239, 0, 0, 0, 0 }, FLAG_STKEY, { 1, 455, 33, 486 }, 1, 0 },
	{ VKB_KEY_SPACE, "Space", "Space", 62, 1, 1, 0, 0x39, { 104, 208, 391, 239, 0, 0, 0, 0 }, FLAG_STKEY, { 34, 455, 66, 486 }, 1, 0 },
	{ VKB_KEY_CAPS, "Caps", "Capslock", 115, 1, 1, 0, 0x3a, { 392, 208, 439, 239, 0, 0, 0, 0 }, FLAG_STKEY, { 67, 455, 99, 486 }, 1, 0 },
	{ VKB_KEY_NUMPAD_0, "Num0", "Numpad 0", 144, 1, 1, 0, 0x70, { 624, 208, 687, 239, 0, 0, 0, 0 }, FLAG_STKEY, { 100, 455, 132, 486 }, 1, 0 },
	{ VKB_KEY_NUMPAD_PERIOD, "Num.", "Numpad . (Period)", 158, 1, 1, 0, 0x71, { 688, 208, 719, 239, 0, 0, 0, 0 }, FLAG_STKEY, { 133, 455, 165, 486 }, 1, 0 },
	{ VKB_KEY_NUMPAD_ENTER, "NumEnt", "Numpad Enter", 160, 0, 0, 0, 0x72, { 720, 208, 751, 239, 0, 0, 0, 0 }, FLAG_STKEY, { 166, 455, 198, 486 }, 2, s_refs_id_VKB_KEY_NUMPAD_ENTER },
	{ VKB_KEY_KEYBOARDTOGGLE, "KbdToggle", "Toggle Keyboard", -1, 1, 0, 0, 0xff, { 1, 257, 32, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_PERSIST, { 1, 257, 33, 288 }, 1, 0 },
	{ VKB_KEY_KEYBOARDTOGGLE_SEL, "KbdToggleSel", "Toggle Keyboard", -1, 0, 0, 0, 0xff, { 34, 257, 65, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_PERSIST, { 34, 257, 66, 288 }, 1, 0 },
	{ VKB_KEY_JOYLEFT, "Jleft", "Joystick Left", -1, 1, 0, 1, 0xff, { 67, 257, 98, 288, 0, 0, 0, 0 }, FLAG_JOY, { 67, 257, 99, 288 }, 1, 0 },
	{ VKB_KEY_JOYRIGHT, "Jright", "Joystick Right", -1, 1, 0, 2, 0xff, { 100, 257, 131, 288, 0, 0, 0, 0 }, FLAG_JOY, { 100, 257, 132, 288 }, 1, 0 },
	{ VKB_KEY_JOYDOWN, "Jdown", "Joystick Down", -1, 1, 0, 4, 0xff, { 133, 257, 164, 288, 0, 0, 0, 0 }, FLAG_JOY, { 133, 257, 165, 288 }, 1, 0 },
	{ VKB_KEY_JOYUP, "Jup", "Joystick Up", -1, 1, 0, 3, 0xff, { 166, 257, 197, 288, 0, 0, 0, 0 }, FLAG_JOY, { 166, 257, 198, 288 }, 1, 0 },
	{ VKB_KEY_JOYFIRE_OLD, "JfireOld", "Joystick Fire Old", -1, 0, 0, 0, 0xff, { 199, 257, 230, 288, 0, 0, 0, 0 }, FLAG_JOY, { 199, 257, 231, 288 }, 1, 0 },
	{ VKB_KEY_KEYBOARDZOOM, "KbdZoom", "Keyboard Zoom", -1, 0, 0, 0, 0xff, { 232, 257, 263, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_VKB, { 232, 257, 264, 288 }, 1, 0 },
	{ VKB_KEY_KEYBOARDZOOM_SEL, "KbdZoomSel", "Keyboard Zoom", -1, 0, 0, 0, 0xff, { 265, 257, 296, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_VKB, { 265, 257, 297, 288 }, 1, 0 },
	{ VKB_KEY_SCREENZOOM, "ScrnZoom", "Screen Zoom", -1, 0, 0, 0, 0xff, { 298, 257, 329, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_SCREEN|FLAG_SCREEN2, { 298, 257, 330, 288 }, 1, 0 },
	{ VKB_KEY_SCREENZOOM_SEL, "ScrnZoomSel", "Screen Zoom", -1, 0, 0, 0, 0xff, { 331, 257, 362, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_SCREEN|FLAG_SCREEN2, { 331, 257, 363, 288 }, 1, 0 },
	{ VKB_KEY_MOUSETOGGLE, "MJToggle", "Toggle Mouse/Joystick", -1, 1, 0, 5, 0xff, { 364, 257, 395, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_JOY, { 364, 257, 396, 288 }, 1, 0 },
	{ VKB_KEY_JOYTOGGLE, "MJToggle", "Toggle Mouse/Joystick", -1, 0, 0, 0, 0xff, { 397, 257, 428, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_MOUSE, { 397, 257, 429, 288 }, 1, 0 },
	{ VKB_KEY_MOUSELB, "MouseLB", "Left Mouse Button", -1, 1, 0, 0, 0xff, { 430, 257, 461, 288, 0, 0, 0, 0 }, FLAG_MOUSEBUTTON, { 430, 257, 462, 288 }, 1, 0 },
	{ VKB_KEY_MOUSERB, "MouseRB", "Right Mouse Button", -1, 1, 0, 0, 0xff, { 463, 257, 494, 288, 0, 0, 0, 0 }, FLAG_MOUSEBUTTON, { 463, 257, 495, 288 }, 1, 0 },
	{ VKB_KEY_NORMALSPEED, "NormalSpd", "Toggle Turbo Speed", -1, 0, 0, 0, 0xff, { 496, 257, 527, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_MAIN, { 496, 257, 528, 288 }, 1, 0 },
	{ VKB_KEY_TURBOSPEED, "TurboSpd", "Toggle Turbo Speed", -1, 1, 1, 0, 0xff, { 529, 257, 560, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_MAIN, { 529, 257, 561, 288 }, 1, 0 },
	{ VKB_KEY_SCREENPRESETS, "ScrnPreset", "Cycle Screen Zoom Presets", -1, 1, 0, 0, 0xff, { 562, 257, 593, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_SCREEN2, { 562, 257, 594, 288 }, 1, 0 },
	{ VKB_KEY_KEYBOARDPRESETS, "VKBPreset", "Cycle Keyboard Zoom Presets", -1, 1, 0, 0, 0xff, { 562, 257, 593, 288, 0, 0, 0, 0 }, FLAG_CUSTOMKEY|FLAG_VKB, { 562, 257, 594, 288 }, 1, 0 },
	{ VKB_KEY_JOYFIRE, "Jfire", "Joystick Fire", -1, 1, 0, 0, 0xff, { 701, 421, 766, 486, 0, 0, 0, 0 }, FLAG_JOY, { 701, 421, 766, 486 }, 1, 0 },
	{ VKB_KEY_LEFTSHIFT_BUTTON, "Lshift_Btn", "Left Shift Button", -1, 0, 0, 0, 0x2a, { 701, 421, 766, 486, 0, 0, 0, 0 }, FLAG_STKEY, { 701, 421, 766, 486 }, 1, 0 },
	{ VKB_KEY_RIGHTSHIFT_BUTTON, "Rshift_btn", "Right Shift Button", -1, 0, 0, 0, 0x36, { 701, 421, 766, 486, 0, 0, 0, 0 }, FLAG_STKEY, { 701, 421, 766, 486 }, 1, 0 },
	{ VKB_KEY_TOGGLEUI, "UIToggle", "Toggle UI", -1, 1, 0, 0, 0xff, { 104, 208, 391, 239, 0, 0, 0, 0 }, FLAG_CUSTOMKEY, { 34, 455, 66, 486 }, 1, 0 },
	{ VKB_KEY_AUTOFIRE, "AutoFire", "Toggle Auto Fire", -1, 1, 0, 0, 0xff, { 104, 208, 391, 239, 0, 0, 0, 0 }, FLAG_CUSTOMKEY, { 34, 455, 66, 486 }, 1, 0 },
	{ VKB_KEY_QUICKSAVESTATE, "SaveState", "Quick Save State", -1, 1, 0, 0, 0xff, { 104, 208, 391, 239, 0, 0, 0, 0 }, FLAG_CUSTOMKEY, { 34, 455, 66, 486 }, 1, 0 },
	{ VKB_KEY_QUICKLOADSTATE, "LoadState", "Quick Load State", -1, 1, 0, 0, 0xff, { 104, 208, 391, 239, 0, 0, 0, 0 }, FLAG_CUSTOMKEY, { 34, 455, 66, 486 }, 1, 0 }
};

const int g_vkbKeyDefsSize = 123;

const int g_vkbTexFullW		= 768;
const int g_vkbTexFullH		= 488;
const int g_vkbTexKbW		= 768;
const int g_vkbTexKbH		= 256;

const int g_vkbRowSearchSize		 = 6;

const RowSearch g_vkbRowSearch[] =
{
	{ 33, 62, 0, 9 },
	{ 80, 111, 10, 30 },
	{ 112, 143, 31, 52 },
	{ 144, 175, 53, 73 },
	{ 176, 207, 74, 90 },
	{ 208, 239, 91, 96 }
};

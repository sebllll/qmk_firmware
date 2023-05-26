/* Copyright 2021 Kyle McCreery
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "murphpad.h"
#include <string.h>
// char keylog_str[24] = {};

#ifdef ENCODER_ENABLE
bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) {
        return false;
    }

    switch (index) {
        case 0:
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        break;
        case 1:
            if (clockwise) {
                tap_code(KC_BRIU);
            } else {
                tap_code(KC_BRID);
            }
        break;
    }
    return true;
}
#endif

#ifdef OLED_ENABLE

#define  KEYLOG_LEN 6
char     keylog_str[KEYLOG_LEN] = {};
uint8_t  keylogs_str_idx        = 0;
uint16_t log_timer              = 0;

// clang-format off
static const char PROGMEM code_to_name[256] = {
//   0     1     2     3     4     5     6     7     8     9    A     B     c     D     E     F
    ' ',  ' ',  ' ',  ' ',  'a',  'b',  'c',  'd',  'e',  'f', 'g',  'h',  'i',  'j',  'k',  'l',  // 0x
    'm',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v', 'w',  'x',  'y',  'z',  '1',  '2',  // 1x
    '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  20,   19,  27,   26,   22,   '-',  '=',  '[',  // 2x
    ']',  '\\', '#',  ';',  '\'', '`',  ',',  '.',  '/',  128, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, // 3x
    0xDB, 0xDC, 0xDD, 0xDE, 0XDF, 0xFB, 'P',  'S',  19,   ' ', 17,   30,   16,   16,   31,   26,   // 4x
    27,   25,   24,   'N',  '/',  '*',  '-',  '+',  23,   '1', '2',  '3',  '4',  '5',  '6',  '7',  // 5x
    '8',  '9',  '0',  '.',  '\\', 'A',  0,    '=',  ' ',  ' ', ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  // 6x
    ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  // 7x
    ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  // 8x
    ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  // 9x
    ' ',  ' ',  ' ',  ' ',  ' ',  0,    ' ',  ' ',  ' ',  ' ', ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  // Ax
    ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  // Bx
    ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  // Cx
    ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  // Dx
    'C',  'S',  'A',  'C',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', ' ',  ' ',  ' ',  24,   26,   24,   // Ex
    25,   0x9D, 0x9D, 0x9D, 0x9D, 0x9D, 0x9D, 0x9D, 0x9D, 24,  25,   27,   26,   ' ',  ' ',  ' '   // Fx
};
// clang-format on

// static const char *code_to_string[256] = {
// //   0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F
//     "0x0", "0x1", "0x2", "0x3", "a",   "b",   "c",   "d",   "e",   "f",   "g",   "h",   "i",   "j",   "k",   "l",   // 0x
//     "m",   "n",   "o",   "p",   "q",   "r",   "s",   "t",   "u",   "v",   "w",   "x",   "y",   "z",   "1",   "2",   // 1x
//     "3",   "4",   "5",   "6",   "7",   "8",   "9",   "0",   "2x8", "2x9", "2xA", "2xB", "2xC", "-",   "=",   "[",   // 2x
//     "]",   "\\",  "#",   ";",   "\\",  "`",   ",",   ".",   "/",   "3x9", "3xA", "3xB", "3xC", "3xD", "3xE", "3xF", // 3x
//     "4x0", "4x1", "4x2", "4x3", "4x4", "4x5", "P",   "S",   "4x8", "4x9", "4xA", "4xB", "4xC", "4xD", "4xE", "4xF", // 4x
//     "5x0", "5x1", "5x2", "N",   "/",   "*",   "-",   "+",   "5x8", "1",   "2",   "3",   "4",   "5",   "6",   "7",   // 5x
//     "8",   "9",   "0",   ".",   "\\",  "A",   "6x6", "=",   "6x8", "6x9" "6xA", "6xB", "6xC", "6xD", "6xE", "6xF",  // 6x
//     "7x0", "7x1", "7x2", "7x3", "7x4", "7x5", "7x6", "7x7", "7x8", "7x9", "7xA", "7xB", "7xC", "7xD", "7xE", "7xF", // 7x
//     "8x0", "8x1", "8x2", "8x3", "8x4", "8x5", "8x6", "8x8", "8x8", "8x9", "8xA", "8xB", "8xC", "8xD", "8xE", "8xF", // 8x
//     "9x0", "9x1", "9x2", "9x3", "9x4", "9x5", "9x6", "9x9", "9x9", "9x9", "9xA", "9xB", "9xC", "9xD", "9xE", "9xF", // 9x
//     "Ax0", "Ax1", "Ax2", "Ax3", "Ax4", "Ax5", "Ax6", "AxA", "AxA", "Ax9", "AxA", "AxB", "AxC", "AxD", "AxE", "AxF", // Ax
//     "Bx0", "Bx1", "Bx2", "Bx3", "Bx4", "Bx5", "Bx6", "BxB", "BxB", "Bx9", "BxA", "BxB", "BxC", "BxD", "BxE", "BxF", // Bx
//     "Cx0", "Cx1", "Cx2", "Cx3", "Cx4", "Cx5", "Cx6", "CxC", "CxC", "Cx9", "CxA", "CxB", "CxC", "CxD", "CxE", "CxF", // Cx
//     "Dx0", "Dx1", "Dx2", "Dx3", "Dx4", "Dx5", "Dx6", "DxD", "DxD", "Dx9", "DxA", "DxB", "DxC", "DxD", "DxE", "DxF", // Dx
//     "Ex0", "Ex1", "Ex2", "Ex3", "Ex4", "Ex5", "Ex6", "ExE", "ExE", "Ex9", "ExA", "ExB", "ExC", "ExD", "ExE", "ExF", // Ex
//     "Fx0", "Fx1", "Fx2", "Fx3", "Fx4", "Fx5", "Fx6", "FxF", "FxF", "Fx9", "FxA", "FxB", "FxC", "FxD", "FxE", "FxF", // Fx
// };

// void add_keylog(uint16_t keycode) {
//     if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
//         (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
//         keycode = keycode & 0xFF;
//     }
//
//     for (uint8_t i = KEYLOG_LEN - 1; i > 0; i--) {
//         keylog_str[i] = keylog_str[i - 1];
//     }
//     if (keycode < 256) {
//         keylog_str[0] = code_to_name[keycode];
//     }
//     keylog_str[KEYLOG_LEN - 1] = 0;
//
//     log_timer = timer_read();
// }

void add_keylog(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) {
        keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
    } else if (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) {
        keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
    } else if (keycode >= QK_MODS && keycode <= QK_MODS_MAX) {
        keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
    }

    if ((keycode == KC_BSPC) && mod_config(get_mods() | get_oneshot_mods()) & MOD_MASK_CTRL) {
        memset(keylog_str, ' ', KEYLOG_LEN);
        keylog_str[KEYLOG_LEN - 1] = 0x00;
        return;
    }

    if (record->tap.count) {
        keycode &= 0xFF;
    } else if (keycode > 0xFF) {
        return;
    }

    // memmove(keylog_str, keylog_str + 1, KEYLOG_LEN - 2);



    if (keycode < ARRAY_SIZE(code_to_name)) {
        for (uint8_t i = KEYLOG_LEN - 1; i > 0; i--) {
            keylog_str[i] = keylog_str[i - 1];
        }

        // keylog_str[(KEYLOG_LEN - 2)] = pgm_read_byte(&code_to_name[keycode]);
        keylog_str[0] = pgm_read_byte(&code_to_name[keycode]);
    }

    log_timer = timer_read();
}

// void update_log(void) {
//     if (timer_elapsed(log_timer) > 750) {
//         add_keylog(0);
//     }
// }

void render_keylogger(void) {
    oled_write_ln_P(PSTR("Klog:"), false);
    oled_write_ln(keylog_str, false);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        add_keylog(keycode, record);
    }

    // if (keycode < ARRAY_SIZE(code_to_name)) {
    //     oled_set_cursor(0,7);
    //     char keyname;
    //     keyname = pgm_read_byte(&code_to_name[keycode]);
    //     if (record->event.pressed) {
    //             oled_write(&keyname, true);
    //             // oled_write(color[1], true)
    //             // oled_write_raw(get_u16_str(keycode, ' '), true);
    //     } else {
    //             oled_write(&keyname, false);
    //             // oled_write_ln_P(PSTR("DOWN"), false);
    //     }
    // }
    //
    // // switch (keycode) {
    // // case KC_MUTE:
    // //         if (record->event.pressed) { // key down
    // //             oled_write_ln_P(PSTR("MUTE"), false);
    // //             // x = position, y = row
    // //             // for (uint8_t x = 0; x < CUBE_NUMBER * TAB_UNIT; x++) {
    // //             //     for (uint8_t y = R1; y < CUBE_NUMBER; y++) {
    // //             //         oled_write_pixel(x, y, true);
    // //             //     }
    // //             // }
    // //         } else { // key up
    // //             oled_write_ln_P(PSTR("mute"), false);7744887jnjf
    // //             // for (uint8_t x = 0; x < CUBE_NUMBER * TAB_UNIT; x++) {
    // //             //     for (uint8_t y = R1; y < CUBE_NUMBER; y++) {
    // //             //         oled_write_pixel(x, y, false);
    // //             //     }
    // //             // }
    // //         }
    // //         return true;
    // // case KC_P7:
    // //         if (record->event.pressed) { // key down
    // //             oled_write_ln_P(PSTR("7 ON"), false);
    // //         } else { // key up
    // //             oled_write_ln_P(PSTR("7 OFF"), false);
    // //         }
    // //         return true;
    // // default:
    // //         return true;
    // // }

    return true;
}

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_270;       // flips the display 270 degrees
}

bool oled_task_user(void) {

    // LOGO
    oled_write_P(PSTR("MURPH"), false);
    oled_set_cursor(0,1);
    oled_write_ln_P(PSTR("  pad"), true);

    // LAYER
    oled_write_P(PSTR("FN:"), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
        oled_write_P(PSTR(" 0"), false);
        break;
        case 1:
        oled_write_P(PSTR(" 1"), false);
        break;
        case 2:
        oled_write_P(PSTR(" 2"), false);
        break;
        case 3:
        oled_write_P(PSTR(" 3"), false);
        break;
        default:
        oled_write_P(PSTR(" ?"), false);
    }

    oled_write_ln_P(PSTR(""), false);

    // NUMlock
    led_t led_state = host_keyboard_led_state();
    oled_write_ln_P(led_state.num_lock ? PSTR("NUMl") : PSTR("    "), false);
    // oled_write_ln_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    // oled_write_ln_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);

    oled_write_ln_P(PSTR(""), false);
    render_keylogger();

    // oled_write_ln_P(PSTR("-"), false);
    // oled_write_ln_P(PSTR(""), false);

    // oled_write(get_u16_str(timer_elapsed(log_timer), '0'), false);

    // WPM
    // oled_write_ln_P(PSTR("WPM: "), false);
    // oled_write(get_u8_str(get_current_wpm(), '0'), false);

    return true;
}
#endif

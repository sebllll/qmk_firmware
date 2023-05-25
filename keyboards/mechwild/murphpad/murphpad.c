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

bool display_keyboard = false;

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

// #define OLED_ENABLE = yes // HACK
#ifdef OLED_ENABLE
// static const char PROGMEM mw_logo[] = {
//     0x8A, 0x8B, 0x8C, 0x8D, '\r',
//     0xAA, 0xAB, 0xAC, 0xAD, 0xAE,
//     0xCA, 0xCB, 0xCC, 0xCD, '\r',
//     0x20, 0x8E, 0x8F, 0x90, 0x00};

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

static const char* PROGMEM code_to_string[256] = {
//   0      1       2       3       4       5       6       7       8       9       A       B       C       D       E       F
    '0x0',  '0x1',  '0x2',  '0x3',  'a'  ,  'b'  ,  'c'  ,  'd'  ,  'e'  ,  'f'  , 'g'  ,  'h'  ,  'i'  ,  'j'  ,  'k'  ,  'l'  ,  // 0x
    'm'  ,  'n'  ,  'o'  ,  'p'  ,  'q'  ,  'r'  ,  's'  ,  't'  ,  'u'  ,  'v'  , 'w'  ,  'x'  ,  'y'  ,  'z'  ,  '1'  ,  '2'  ,  // 1x
    '3'  ,  '4'  ,  '5'  ,  '6'  ,  '7'  ,  '8'  ,  '9'  ,  '0'  ,  '2x8',  '2x9', '2xA',  '2xB',  '2xC',  '-'  ,  '='  ,  '['  ,  // 2x
    ']'  ,  '\\' ,  '#'  ,  ';'  ,  '\'' ,  '`'  ,  ','  ,  '.'  ,  '/'  ,  '3x9', '3xA',  '3xB',  '3xC',  '3xD',  '3xE',  '3xF',  // 3x
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

// const char *keyname {
//     return code_to_name
// }

// const char *read_keylog(void) {
//     return keylog_str;
// }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (display_keyboard == true){
        display_keyboard = false;

        // char keyname = code_to_name[keycode];
        // char *keyname = (*char)code_to_name[keycode];

        if (record->event.pressed) {
            // oled_write_ln_P((char *)&code_to_name[keycode], true);
            // oled_write_ln_P(PSTR("UP"), false);
        oled_write_ln_P(&code_to_string[keycode], true);
        } else {
            oled_write_ln_P(PSTR("DOWN"), false);
        // oled_write_ln_P(get_u16_str(keycode, ' '), false);
        }

        // switch (keycode) {
        // case KC_MUTE:
        //         if (record->event.pressed) { // key down
        //             oled_write_ln_P(PSTR("MUTE"), false);
        //             // x = position, y = row
        //             // for (uint8_t x = 0; x < CUBE_NUMBER * TAB_UNIT; x++) {
        //             //     for (uint8_t y = R1; y < CUBE_NUMBER; y++) {
        //             //         oled_write_pixel(x, y, true);
        //             //     }
        //             // }
        //         } else { // key up
        //             oled_write_ln_P(PSTR("mute"), false);
        //             // for (uint8_t x = 0; x < CUBE_NUMBER * TAB_UNIT; x++) {
        //             //     for (uint8_t y = R1; y < CUBE_NUMBER; y++) {
        //             //         oled_write_pixel(x, y, false);
        //             //     }
        //             // }
        //         }
        //         return true;
        // case KC_P7:
        //         if (record->event.pressed) { // key down
        //             oled_write_ln_P(PSTR("7 ON"), false);
        //         } else { // key up
        //             oled_write_ln_P(PSTR("7 OFF"), false);
        //         }
        //         return true;
        // default:
        //         return true;
        // }
        }
    return true;
}

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_270;       // flips the display 270 degrees
}

bool oled_task_user(void) {
    // oled_set_cursor(0, 0);
    oled_write_P(PSTR("MURPH"), false);
    oled_set_cursor(0,1);
    oled_write_ln_P(PSTR("  pad"), true);

    oled_write_P(PSTR("FN:"), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
        oled_write_P(PSTR(" 0"), false);
        display_keyboard = true;
        break;
        case 1:
        oled_write_P(PSTR(" 1"), false);
        display_keyboard = false;
        break;
        case 2:
        oled_write_P(PSTR(" 2"), false);
        display_keyboard = false;
        break;
        case 3:
        oled_write_P(PSTR(" 3"), false);
        display_keyboard = false;
        break;
        default:
        oled_write_P(PSTR(" ?"), false);
    }

    oled_write_ln_P(PSTR(""), false);

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_ln_P(led_state.num_lock ? PSTR("NUMl") : PSTR("    "), false);
    oled_write_ln_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_ln_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);

    // oled_write_ln_P(PSTR("WPM: "), false);
    // oled_write(get_u8_str(get_current_wpm(), '0'), false);









    // oled_set_cursor(0,6);

    // oled_write_ln_P(PSTR("Layer"), false);

    // switch (get_highest_layer(layer_state)) {
    //     case 0:
    //         oled_write_ln_P(PSTR("Base"), false);
    //         break;
    //     case 1:
    //         oled_write_ln_P(PSTR("FN 1"), false);
    //         break;
    //     case 2:
    //         oled_write_ln_P(PSTR("FN 2"), false);
    //         break;
    //     case 3:
    //         oled_write_ln_P(PSTR("FN 3"), false);
    //         break;
    //     default:
    //         oled_write_ln_P(PSTR("Undef"), false);
    // }

    // oled_write_ln_P(PSTR(""), false);

    // // Host Keyboard LED Status
    // led_t led_state = host_keyboard_led_state();
    // oled_write_ln_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    // oled_write_ln_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    // oled_write_ln_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);



    return true;
}
#endif

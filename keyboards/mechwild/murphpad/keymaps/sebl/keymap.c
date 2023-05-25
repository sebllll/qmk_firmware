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
 *
 *
 * https://github.com/vuon0029/qmk/tree/master/keyboards/mechwild/mercutio/keymaps/dracutio
 *
 *
 *
 */
#include QMK_KEYBOARD_H
// #include "quantum.h"
#include "raw_hid.h"
#include <stdio.h>
#include <string.h>

// unit
// #define GAP 2
// #define CUBE_NUMBER 3.75
// #define SPACE_UNIT 25
// #define TAB_UNIT 1.25
// #define CAPS_UNIT 1.5
// #define ENTER_UNIT 2
// #define RSHIFT_UNIT 1.75

// bool display_keyboard = false;

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN1,
    _FN2,
    _FN3
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
                  KC_F1,   KC_F2,   KC_F3,   KC_F4,
                 KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
                  KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_MUTE,  KC_P4,   KC_P5,   KC_P6,   _______,
        MO(_FN1), KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        KC_BSPC,  KC_P0,   _______, KC_PDOT, _______,

                  KC_F5,   KC_F6,   KC_F7

    ),
    [_FN1] = LAYOUT(
                 _______,  _______, _______, _______,
                 _______,  _______, _______, _______,
                 RGB_HUD,  RGB_SPI, RGB_HUI, _______,
        _______, RGB_RMOD, RGB_TOG, RGB_MOD, _______,
        _______, RGB_VAD,  RGB_SPD, RGB_VAI, _______,
        _______, RGB_SAD,  _______, RGB_SAI, _______,

                 _______,  _______, _______

    ),
    [_FN2] = LAYOUT(
                  _______, _______, _______, _______,
                  _______, _______, _______, _______,
                  _______, _______, _______, _______,
        _______,  _______, _______, _______, _______,
        _______,  _______, _______, _______, _______,
        _______,  _______, _______, _______, _______,

                  _______, _______, _______

    ),
    [_FN3] = LAYOUT(
                  _______, _______, _______, _______,
                  _______, _______, _______, _______,
                  _______, _______, _______, _______,
        _______,  _______, _______, _______, _______,
        _______,  _______, _______, _______, _______,
        _______,  _______, _______, _______, _______,

                  _______, _______, _______

    )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_BRID, KC_BRIU) },
    [_FN1]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_FN2]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_FN3]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};
#endif

// #define OLED_ENABLE = yes //HACK
#ifdef OLED_ENABLE

// static const char PROGMEM mw_logo[] = {0x8A, 0x8B, 0x8C, 0x8D, '\r', 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xCA, 0xCB, 0xCC, 0xCD, '\r', 0x20, 0x8E, 0x8F, 0x90, 0x00};


// oled_rotation_t oled_init_user(oled_rotation_t rotation) {
//     return OLED_ROTATION_270; // do not flip the display
// }

// static void render_hid(void) {
//     oled_set_cursor(16, 3); // TODO: fix cursor position
//     oled_write_P(is_hid_connected == true ? PSTR("HID") : PSTR(""), false);
// }

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//     if (display_keyboard == true){
//             display_keyboard = false;
//             switch (keycode) {
//                 case KC_MUTE:
//                     if (record->event.pressed) { // key down
//                         oled_write_ln_P(PSTR("MUTE"), false);

//                         // x = position, y = row
//                         // for (uint8_t x = 0; x < CUBE_NUMBER * TAB_UNIT; x++) {
//                         //     for (uint8_t y = R1; y < CUBE_NUMBER; y++) {
//                         //         oled_write_pixel(x, y, true);
//                         //     }
//                         // }
//                     } else { // key up
//                         oled_write_ln_P(PSTR("mute"), false);
//                         // for (uint8_t x = 0; x < CUBE_NUMBER * TAB_UNIT; x++) {
//                         //     for (uint8_t y = R1; y < CUBE_NUMBER; y++) {
//                         //         oled_write_pixel(x, y, false);

//                         //     }
//                         // }
//                     }
//                     return true;
//                 case KC_P7:
//                     if (record->event.pressed) { // key down
//                         oled_write_ln_P(PSTR("7 ON"), false);
//                     } else { // key up
//                         oled_write_ln_P(PSTR("7 OFF"), false);
//                     }
//                     return true;
//                 default:
//                     return true;
//             }
//     }
//     return true;
// }



// bool oled_task_user(void) {
//     // if (!oled_task_user()) {
//     //     return false;
//     // }
//     // oled_set_cursor(0, 0);
//     oled_write_ln_P(PSTR("murph"), false);
//     // oled_set_cursor(0,1);
//     oled_write_ln_P(PSTR("  pad"), true);

//     // oled_write_P(mw_logo, false); // Render MechWild "MW" Logo
//     oled_set_cursor(0, 6);

//     oled_write_P(PSTR("FN:"), false);

//     switch (get_highest_layer(layer_state)) {
//         case 0:
//             oled_write_P(PSTR(" 0"), false);
//             display_keyboard = true;
//             break;
//         case 1:
//             oled_write_P(PSTR(" 1"), false);
//             display_keyboard = false;
//             break;
//         case 2:
//             oled_write_P(PSTR(" 2"), false);
//             display_keyboard = false;
//             break;
//         case 3:
//             oled_write_P(PSTR(" 3"), false);
//             display_keyboard = false;
//             break;
//         default:
//             oled_write_P(PSTR(" ?"), false);
//     }

//     oled_write_ln_P(PSTR(""), false);

//     // Host Keyboard LED Status
//     led_t led_state = host_keyboard_led_state();
//     oled_write_ln_P(led_state.num_lock ? PSTR("NUMl") : PSTR("    "), false);
//     oled_write_ln_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
//     oled_write_ln_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);

//     // oled_write_ln_P(PSTR("WPM: "), false);
//     // oled_write(get_u8_str(get_current_wpm(), '0'), false);

//     return true;
// }
#endif

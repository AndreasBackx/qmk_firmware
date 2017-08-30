#include "kbd75.h"

#define _BL 0
#define _FL 1

#define _______ KC_TRNS

#define MODS_CTRL_MASK  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Keymap _BL: (Base Layer) Default Layer
   * ,----------------------------------------------------------------.
   * |Esc |F1|F 2|F 3|F 4|F 5|F 6|F 7|F 8|F 9|F10|F11|F12|Prt|Fn |Del |
   * |----------------------------------------------------------------|
   * |~ ` | 1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp |Home|
   * |----------------------------------------------------------------|
   * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  |PgUp|
   * |----------------------------------------------------------------|
   * |CAPS   |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return |PgDn|
   * |----------------------------------------------------------------|
   * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift | Up|End |
   * |----------------------------------------------------------------|
   * |Ctrl|Alt |Sup |        Space          | FN|Alt|Ctrl|Lef|Dow|Rig |
   * `----------------------------------------------------------------'
   */
  // KC_BSPC
[_BL] = KEYMAP(
  KC_ESC,    KC_F1,   KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,   KC_F8,  KC_F9,  KC_F10,  KC_F11,  KC_F12, KC_PSCR,   MO(1),  KC_DEL,
  KC_GRV,     KC_1,    KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,    KC_8,   KC_9,    KC_0, KC_MINS,  KC_EQL, KC_DEL,  KC_BSPC, KC_HOME, \
  KC_TAB,     KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,    KC_I,   KC_O,    KC_P, KC_LBRC, KC_RBRC,          KC_BSLS, KC_PGUP, \
  KC_CAPS,    KC_A,    KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,    KC_K,   KC_L, KC_SCLN, KC_QUOT,                    KC_ENT, KC_PGDN, \
  KC_LSFT, _______,    KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M, KC_COMM,  KC_DOT, KC_SLSH,           KC_RSFT,   KC_UP, KC_END, \
  KC_LCTL, KC_LALT, KC_LGUI,         KC_SPC, KC_SPC, KC_SPC,                          MO(_FL), KC_RALT, KC_RCTRL, KC_LEFT, KC_DOWN,KC_RGHT),

  /* Keymap _FL: Function Layer
   * ,----------------------------------------------------------------.
   * |Pow|   |   |   |   |   |   |   |   |   |   |   |   |       |    |
   * |----------------------------------------------------------------|
   * |   |   |   |   |   |   |   |   |   |   |   |   |   |Del    |    |
   * |----------------------------------------------------------------|
   * |Reset|   |Up |   |   |   |   |   |   |   |   |   |   |     |BL  |
   * |----------------------------------------------------------------|
   * |      |<- |Dn | ->|   |   |   |   |   |   |   |   |Play/Pau|BL+ |
   * |----------------------------------------------------------------|
   * |        |   |   |   |   |   |   |   |   |   |   |      |VU+|BL- |
   * |----------------------------------------------------------------|
   * |    |    |    |                       |   |   |    |PRV|VU-|NXT |
   * `----------------------------------------------------------------'
   */
[_FL] = KEYMAP(
  KC_SYSTEM_SLEEP, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  KC_DEL, _______, \
            RESET, RGB_HUD, RGB_VAI, RGB_HUI, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, BL_TOGG, \
          RGB_TOG, RGB_SAD, RGB_VAD, RGB_SAI, RGB_MOD, _______, _______, _______, _______, _______, _______, _______,                   KC_MPLY, _______, \
          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, KC_VOLU, _______, \
          _______, _______, _______,          _______, _______, _______,                            _______, _______, _______, KC_MPRV, KC_VOLD, KC_MNXT),
};

/**
 * Startup commands.
 */
void matrix_init_user(void) {
  rgblight_enable();
  rgblight_mode(1);  // Solid colour
  rgblight_sethsv(43, 255, 255);  // Yellow
};

static bool control_disabled = false;
static bool delete_pressed = false;

/**
 * Change ctrl+backspace into delete and do not register the ctrl modifier.
 */
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  if(keycode == KC_BSPC) {
    if (record->event.pressed) {
      if(keyboard_report->mods & MOD_BIT(KC_LCTL)) {
        delete_pressed = true;
        control_disabled = true;
        unregister_code(KC_LCTL);
        register_code(KC_DEL);
        return false;
      }
    } else if(delete_pressed) {
      delete_pressed = false;
      unregister_code(KC_DEL);

      if(control_disabled) {
        control_disabled = false;
        register_code(KC_LCTL);
      }
      return false;
    }
  } else if(keycode == KC_LCTL && !record->event.pressed && delete_pressed) {
    delete_pressed = false;
    control_disabled = false;
    unregister_code(KC_DEL);
    register_code(KC_BSPC);
    return false;
  }

  if(keycode == RESET) {
    rgblight_enable();
    rgblight_mode(1);
    rgblight_sethsv(0, 255, 255);  // Red
  }

  return true;
}

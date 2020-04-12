#include QMK_KEYBOARD_H


#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 3
#define _ALTER 4
#define _MOUSE 5

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  ALTER,
  Y_HACK,
  U_HACK,
  I_HACK,
  O_HACK,
  P_HACK,
  H_HACK,
  J_HACK,
  K_HACK,
  L_HACK,
  N_HACK,
  M_HACK,
  Com_HACK,
  Per_HACK,
  Ent_HACK,
  LSPC_HACK,
};

#define JA_CLON KC_QUOT  // : and +
#define JA_AT   KC_LBRC  // @ and `
#define JA_HAT  KC_EQL   // ^ and ~
#define JA_ENUN KC_RO    // \ and _ (EN mark and UNder score)
#define JA_ENVL KC_JYEN  // \ and | (EN mark and Vertical Line)
#define JA_LBRC KC_RBRC  // [ and {
#define JA_RBRC KC_BSLS  // ] and }
#define JA_SLBRC KC_RCBR  // {
#define JA_SRBRC KC_PIPE  // }


// enum combos{
//    LOEWER_ENTER
// };
// const uint16_t PROGMEM enter_combo[] = {LOWER, LSFT_T(KC_SPC), COMBO_END};
// combo_t key_combos[COMBO_COUNT]={
//    [LOEWER_ENTER]=COMBO_ACTION(enter_combo)
// };

static bool shift_pressed = false; 
static bool alt_pressed = false; 
static bool lower_pressed=false;

enum {
   TD_BRACKET_L=0,
   TD_BRACKET_R,
   TD_LOWER,
   TD_LCLK,
   TD_RCLK
};
void dance_bracket_l_finished(qk_tap_dance_state_t *state, void *user_data){
   if(state->count == 1){
      if(lower_pressed == false){
         register_code(KC_F);
      }else{
         register_code(KC_LSFT);
         register_code(KC_8);
      }
   }else{
      register_code(JA_LBRC);
   }
}
void dance_bracket_l_reset(qk_tap_dance_state_t *state, void *user_data){
   if(state->count == 1){
      unregister_code(KC_LSFT);
      unregister_code(KC_8);
   }else{
      unregister_code(JA_LBRC);
   }
   unregister_code(KC_F);
}
void dance_bracket_r_finished(qk_tap_dance_state_t *state, void *user_data){
   if(state->count == 1){
      if(lower_pressed == false){
         register_code(KC_G);
      }else{
         register_code(KC_LSFT);
         register_code(KC_9);
      }
   }else{
      register_code(JA_RBRC);
   }
}
void dance_bracket_r_reset(qk_tap_dance_state_t *state, void *user_data){
   if(state->count == 1){
      unregister_code(KC_LSFT);
      unregister_code(KC_9);
   }else{
      unregister_code(JA_RBRC);
   }
   unregister_code(KC_G);
}
enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5, //send two single taps
  TRIPLE_TAP = 6,
  TRIPLE_HOLD = 7
};

int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
   //  if (!state->interrupted && !state->pressed)  return SINGLE_TAP;
    if(!state->pressed)return SINGLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     * keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  //Assumes no one is trying to type the same letter three times (at least not quickly).
  //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
  //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8; //magic number. At some point this method will expand to work for more presses
};

typedef struct {
  bool is_press_action;
  int state;
} tap;

//instanalize an instance of 'tap' for the 'lower' tap dance.
static tap lower_tap_state = {
  .is_press_action = true,
  .state = 0
};

void dance_lower_finished(qk_tap_dance_state_t *state, void *user_data){
   lower_tap_state.state = cur_dance(state);
  switch (lower_tap_state.state) {
    case SINGLE_TAP: 
      lower_pressed=false;
      tap_code(KC_F14);
      break;
    case SINGLE_HOLD: 
      /* レイヤ切り替えはeachの方で行っているためここでは不要 */
      // layer_on(_LOWER); 
      // update_tri_layer(_LOWER, _RAISE, _ADJUST);
      break;
    case DOUBLE_TAP:
      break;
    case DOUBLE_HOLD:
      break;
    case DOUBLE_SINGLE_TAP:
      break;
  }
}

void dance_lower_reset(qk_tap_dance_state_t *state, void *user_data){
   switch (lower_tap_state.state) {
      case SINGLE_TAP:
         break;
      case SINGLE_HOLD: 
         break;
      case DOUBLE_TAP: 
         break;
      case DOUBLE_HOLD:
         break;
      case DOUBLE_SINGLE_TAP:
         break;
   }
   layer_off(_MOUSE); 
   layer_off(_LOWER); 
   update_tri_layer(_LOWER, _RAISE, _ADJUST);
   lower_tap_state.state = 0;
   if(alt_pressed){
      unregister_code(KC_LALT);//Alt+Shift+Tabが押されているケースがあるため，解除する
      alt_pressed = false;
   }
   if(shift_pressed){//Alt+Shift+Tabが押されているケースがあるため，解除する
      unregister_code(KC_LSFT);
      shift_pressed = false;
   }
}

//このメソッドは押下した瞬間に呼ばれる
void dance_lower_each(qk_tap_dance_state_t *state, void *user_data){
   if(state->count == 1){
      lower_pressed=true;
      layer_on(_LOWER); 
      update_tri_layer(_LOWER, _RAISE, _ADJUST);
   }else if(state->count == 2){
      lower_pressed=false;
      layer_on(_MOUSE); 
   }
}

//instanalize an instance of 'tap' for the 'lower' tap dance.
static tap lclk_tap_state = {
  .is_press_action = true,
  .state = 0
};

void dance_lclk_finished(qk_tap_dance_state_t *state, void *user_data){
   lclk_tap_state.state = cur_dance(state);
  switch (lclk_tap_state.state) {
    case SINGLE_TAP: 
      tap_code(KC_BTN1);
       break;
    case SINGLE_HOLD: 
      register_code(KC_MS_DOWN);
      break;
    case DOUBLE_TAP:
      tap_code(KC_BTN1);
      tap_code(KC_BTN1);
      break;
    case DOUBLE_HOLD:
      register_code(KC_MS_DOWN);
      break;
    case DOUBLE_SINGLE_TAP:
      tap_code(KC_BTN1);
      tap_code(KC_BTN1);
      break;
  }
}

void dance_lclk_reset(qk_tap_dance_state_t *state, void *user_data){
   switch (lclk_tap_state.state) {
      case SINGLE_TAP:
         break;
      case SINGLE_HOLD: 
         unregister_code(KC_MS_DOWN);
         break;
      case DOUBLE_TAP: 
         break;
      case DOUBLE_HOLD:
         unregister_code(KC_MS_DOWN);
         break;
      case DOUBLE_SINGLE_TAP:
         break;
   }
   lclk_tap_state.state = 0;
}


//instanalize an instance of 'tap' for the 'Mouse up' tap dance.
static tap rclk_tap_state = {
  .is_press_action = true,
  .state = 0
};

void dance_rclk_finished(qk_tap_dance_state_t *state, void *user_data){
   rclk_tap_state.state = cur_dance(state);
  switch (rclk_tap_state.state) {
    case SINGLE_TAP: 
      tap_code(KC_BTN2);
       break;
    case SINGLE_HOLD: 
      register_code(KC_MS_UP);
      break;
    case DOUBLE_TAP:
      tap_code(KC_BTN3);
      break;
    case DOUBLE_HOLD:
      register_code(KC_MS_UP);
      break;
    case DOUBLE_SINGLE_TAP:
      tap_code(KC_BTN2);
      tap_code(KC_BTN2);
      break;
  }
}

void dance_rclk_reset(qk_tap_dance_state_t *state, void *user_data){
   switch (rclk_tap_state.state) {
      case SINGLE_TAP:
         break;
      case SINGLE_HOLD: 
         unregister_code(KC_MS_UP);
         break;
      case DOUBLE_TAP: 
         break;
      case DOUBLE_HOLD:
         unregister_code(KC_MS_UP);
         break;
      case DOUBLE_SINGLE_TAP:
         break;
   }
   rclk_tap_state.state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] ={
   [TD_BRACKET_L] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,dance_bracket_l_finished,dance_bracket_l_reset),
   [TD_BRACKET_R] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,dance_bracket_r_finished,dance_bracket_r_reset),
   [TD_LOWER] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(dance_lower_each,dance_lower_finished,dance_lower_reset,200),
   [TD_LCLK] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL,dance_lclk_finished,dance_lclk_reset,100),
   [TD_RCLK] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL,dance_rclk_finished,dance_rclk_reset,100)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    JA_AT,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, JA_CLON,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    LSFT_T(KC_SPC),     _______,  KC_N,    KC_M,   KC_COMM, KC_DOT, KC_SLSH,  KC_RO,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    KC_LALT,  RAISE,  _______,             LCTL_T(KC_SPC),TD(TD_LOWER),ALTER
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_LOWER] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────────────┬────────────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_F12,  KC_F1,    KC_F2,   KC_F3,   KC_F4,           KC_F5,                                      KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10, KC_F11,
  //├────────┼────────┼────────┼────────┼────────────────┼────────────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,  _______, _______,_______,    JA_SLBRC,           JA_SRBRC,                               Y_HACK,  U_HACK,   I_HACK,  O_HACK,  P_HACK, _______,
  //├────────┼────────┼────────┼────────┼────────────────┼────────────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,  _______, _______,_______, TD(TD_BRACKET_L), TD(TD_BRACKET_R),                           H_HACK, J_HACK,   K_HACK,   L_HACK, KC_BSPC, KC_DEL,
  //├────────┼────────┼────────┼────────┼────────────────┼────────────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______,         _______,         Ent_HACK,          _______, N_HACK,  M_HACK, Com_HACK,Per_HACK, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────────────┴───────────┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______,     _______,             _______,              LCTL_T(KC_ENT),  _______, _______
                                // └────────┴────────────────────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_RAISE] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_F12,  KC_F1,    KC_F2,   KC_F3,   KC_F4,  KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10, KC_F11,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     RESET,   KC_F12,    KC_2,    KC_3,    KC_4,    KC_5,                               Y_HACK,  U_HACK,   I_HACK,  O_HACK,  P_HACK, KC_LBRC,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,  _______, KC_LEFT, KC_RGHT, KC_UP,   KC_LBRC,                            H_HACK, J_HACK,   K_HACK,   L_HACK, KC_BSPC, KC_DEL,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, KC_DOWN, KC_LCBR,LSFT_T(KC_ENT),   _______, N_HACK, M_HACK, Com_HACK,Per_HACK,  KC_MINS, KC_RO,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,              LCTL_T(KC_ENT),_______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_ADJUST] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, KC_JYEN,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            KC_F15,   KC_F15,   KC_F16, KC_F17, _______, KC_LBRC,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            KC_F18, KC_F19,  KC_F20,   KC_F21, _______, KC_RBRC,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     RESET, _______, _______, _______, _______, _______,   _______,          _______,  _______, KC_F22,  KC_F23, KC_F24, _______, KC_RO,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),
  
  [_ALTER] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                            KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_PSLS, JA_HAT,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     RESET,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_PAST,    JA_LBRC,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,  _______, KC_LEFT, KC_RGHT, KC_UP,   KC_LBRC,                            KC_LEFT, KC_4,   KC_5,     KC_6,   KC_PPLS,  JA_RBRC,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, KC_DOWN, KC_LCBR, RALT(KC_ENT),    _______,   KC_0,     KC_P1,   KC_P2,   KC_P3,  KC_PMNS, KC_JYEN,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______,  _______,                  KC_ENT,  _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_MOUSE] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                ┌──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┐
     _______, _______, _______,   _______, _______,  _______,                                   _______,      _______,       _______,       _______,       _______,       _______,
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                ├──────────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
     _______, _______, _______,   _______, _______,  _______,                                   _______,      _______,       _______,       _______,       _______,       _______,
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                ├──────────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
     _______,  _______, _______, _______, _______,  _______,                                  KC_MS_LEFT,    TD(TD_LCLK),   TD(TD_RCLK),   KC_MS_RIGHT,  KC_ACL0,       _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌──────────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤
     _______, _______, _______, _______, _______,  _______, KC_ACL0,           _______,      _______,      _______,       _______,       _______,       _______,       _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └──────┬───────┴──────┬───────┴──────┬───────┴──────┬───────┴──────────────┴──────────────┴──────────────┘
                                    _______, _______,  _______,                       _______,        _______,      _______
                                // └────────┴────────┴────────┘                    └──────────────┴──────────────┴──────────────┘
  )
};

// void process_combo_event(uint8_t combo_index, bool pressed){
//    switch (combo_index)
//    {
//    case LOEWER_ENTER:
//       if(pressed){
//         tap_code(KC_ENT);
//       }
//       break;
   
//    default:
//       break;
//    }
// }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
   static uint16_t mem_keycode;
   uint16_t prev_keycode = mem_keycode;
   bool is_tapped = ((!record->event.pressed) && (keycode == prev_keycode));
   mem_keycode = keycode;
   static bool enter_interrupted = false; //LOWER と Enter以外が入力された際にtrueになるフラグ
   if(!(keycode == TD(TD_LOWER)) &&  !(keycode == Ent_HACK) ){
      enter_interrupted = true;
   }
   static bool rl_interrupted = false;//LOWER/RAISEキー以外が押された際にtrueになるフラグ
   if(!(keycode == TD(TD_LOWER)) &&  !(keycode == RAISE) ){
      rl_interrupted = true;
   }
   static bool control_pressed = false; 
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
   //  case LOWER:
   //    if (record->event.pressed) {
   //       layer_on(_LOWER);
   //       update_tri_layer(_LOWER, _RAISE, _ADJUST);
   //    } else {
   //       if(alt_pressed){
   //          unregister_code(KC_LALT);
   //          alt_pressed = false;
   //       }
   //       if(shift_pressed){
   //          unregister_code(KC_LSFT);
   //          shift_pressed = false;
   //       }

   //       layer_off(_LOWER);
   //       update_tri_layer(_LOWER, _RAISE, _ADJUST);
   //      if (is_tapped) {
   //         if(layer_state_is(_RAISE)){
   //          // tap_code(KC_F13);
   //          tap_code(KC_ESC);
   //         }else{
   //          tap_code(KC_F14);
   //         }
   //       } else if(prev_keycode == RAISE){
   //          // tap_code(KC_F13);
   //          tap_code(KC_ESC);
   //       }
   //    }
   //    return false;
   //    break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
        rl_interrupted = false;
      } else {
         if(alt_pressed){
            unregister_code(KC_LALT);
            alt_pressed = false;
         }
         if(shift_pressed){
            unregister_code(KC_LSFT);
            shift_pressed = false;
         }

        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
        if (is_tapped && layer_state_is(_QWERTY)) {
            tap_code(KC_F13);
         }else if(rl_interrupted == false){
            // tap_code(KC_F13);
            tap_code(KC_ESC);
         }
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;

   case ALTER:
      if (record->event.pressed) {
        layer_on(_ALTER);
      } else {
        layer_off(_ALTER);
      }
      return false;
      break;

   case KC_LCTL:
      if (record->event.pressed) {
         control_pressed = true;
      } else {
         control_pressed = false;
      }
      return true;
      break;

   case Ent_HACK:
      if(record->event.pressed){
         register_code(KC_RSFT);
         enter_interrupted = false;
      }else{
         unregister_code(KC_RSFT);
         if(enter_interrupted == false){
            if(is_tapped || prev_keycode == TD(TD_LOWER)){
               tap_code(KC_ENT);
            }
         }
      }
      return false;
      break;
      
   case Y_HACK:
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_Y);
         }else{
            if(control_pressed == true){
               tap_code(KC_F15);
            }else{
               tap_code(KC_HOME);
            }
         }
      }else{
         unregister_code(KC_Y);
      }
      
      return false;
      break;

   case U_HACK:

      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_U);
            return false;
         }
         if(control_pressed == true){
            tap_code(KC_F15);
         }else{
         }
      }else{
         unregister_code(KC_U);
      }
      return false;
      break;

   case I_HACK:
      if (record->event.pressed) {
         if(control_pressed == true){
            if(lower_pressed==false){
               register_code(KC_I);
               return false;
            }

            tap_code(KC_F16);
         }else{
         }
      }else{
         unregister_code(KC_I);
      }
      return false;
      break;

   case O_HACK:
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_O);
            return false;
         }
         if(control_pressed == true){
            tap_code(KC_F17);
         }else{
            tap_code(KC_END);
         }
      }else{
         unregister_code(KC_O);
      }
      return false;
      break;

   case P_HACK:
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_P);
            return false;
         }
         if(control_pressed == true){
         }else{
            if(!alt_pressed){
               register_code(KC_LALT);
               alt_pressed = true;
            }
            if(!shift_pressed){
               register_code(KC_LSFT);
               shift_pressed = true;
            }
            tap_code(KC_TAB);
         }
      }else{
         unregister_code(KC_P);
      }
      return false;
      break;

   case H_HACK:
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_H);
            return false;
         }
         if(control_pressed == true){
            tap_code(KC_F18);
         }else{
            register_code(KC_LEFT);
         }
      }else{
         unregister_code(KC_H);
         if(control_pressed == true){
         }else{
            unregister_code(KC_LEFT);
         }
         
      }
      return false;
      break;

   case J_HACK:
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_J);
            return false;
         }
         if(control_pressed == true){
            tap_code(KC_F19);
         }else{
            register_code(KC_DOWN);
         }
      }else{
         unregister_code(KC_J);
         if(control_pressed == true){
         }else{
            unregister_code(KC_DOWN);
         }
         
      }
   
      return false;
      break;

   case K_HACK:
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_K);
            return false;
         }
         if(control_pressed == true){
            tap_code(KC_F20);
         }else{
            register_code(KC_UP);
         }
      }else{
         unregister_code(KC_K);
         if(control_pressed == true){
         }else{
            unregister_code(KC_UP);
         }
      }
      return false;
      break;

   case L_HACK:
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_L);
            return false;
         }
         if(control_pressed == true){
            tap_code(KC_F21);
         }else{
            register_code(KC_RIGHT);
         }
      }else{
         unregister_code(KC_L);
         if(control_pressed == true){
         }else{
            unregister_code(KC_RIGHT);
         }
      }
      return false;
      break;

   case N_HACK:
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_H);
            return false;
         }
         if(!alt_pressed){
            register_code(KC_LALT);
            alt_pressed = true;
         }
         if(shift_pressed){
            unregister_code(KC_LSFT);
            shift_pressed = false;
         }
         tap_code(KC_TAB);

      }else{
         unregister_code(KC_H);

      }
      return false;
      break;

   case M_HACK:
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_H);
            return false;
         }
         if(control_pressed == true){
            tap_code(KC_F22);
         }else{
         }
      }else{
         unregister_code(KC_H);
      }
      return false;
      break;

   case Com_HACK:
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_COMM);
            return false;
         }
         if(control_pressed == true){
            tap_code(KC_F23);
         }else{
         }
      }else{
         unregister_code(KC_COMM);
      }
      return false;
      break;
      
   case Per_HACK:
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_DOT);
            return false;
         }
         if(control_pressed == true){
            tap_code(KC_F24);
         }else{
         }
      }else{
         unregister_code(KC_DOT);
      }
      return false;
      break;

   case JA_SLBRC:
      //returnしないルートは入力がそのまま返る
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_R);
            return false;
         }
      }else{
         unregister_code(KC_R);
         return false;
      }
      break;

   case JA_SRBRC:
      //returnしないルートは入力がそのまま返る
      if (record->event.pressed) {
         if(lower_pressed==false){
            register_code(KC_T);
            return false;
         }
      }else{
         unregister_code(KC_T);
         return false;
      }
      break;
  }

  return true;
}

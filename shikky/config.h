/*
Copyright 2017 Danny Nguyen <danny@keeb.io>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// #define USE_I2C
#define EE_HANDS

#undef RGBLED_NUM
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 12
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8

//user defined
#define TAPPING_TERM  150
#define COMBO_TERM  200
#define COMBO_COUNT 1

#define MOUSEKEY_DELAY 0
#define MK_3_SPEED //Mouse速度を3段階のコンスタントに．
#define MK_MOMENTARY_ACCEL//ACELLボタンを押している間だけ速度変更

#define MK_C_OFFSET_UMMOD 4 //おそらく一回押した際に進む量．大きいほど速い
#define MK_C_OFFSET_INTERVAL_UMMOD 4//おそらく次のキーが押されるまでの時間間隔．大きいほど遅い．

#define MK_C_OFFSET_0 4 //低速モード
#define MK_C_INTERVAL_0 8//低速モード
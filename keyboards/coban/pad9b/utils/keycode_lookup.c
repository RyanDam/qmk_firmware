/* Copyright 2024 RyanDam (https://github.com/RyanDam)
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

#include "keycode_lookup.h"
#include "print.h"
#include "via.h"

#define num_keycodes ARRAY_SIZE(lookup_table)
static char UNKNOWN_KEYCODE[] = "?";

int cmp(const void *v1, const void *v2) {
    const lookup_table_t *c1 = v1;
    const lookup_table_t *c2 = v2;

    return (c1->keycode - c2->keycode);
}

/*
    Returns a pointer to a string containing the string describing the keycode, such as those found here:
    https://beta.docs.qmk.fm/using-qmk/simple-keycodes/keycodes

    Will return a string that says "UNKNOWN" if the keycode cannot be found.
*/
// char* translate_keycode_to_string(uint16_t code) {
//     lookup_table_t * result = NULL;
//     lookup_table_t target = {.key_string = "", .keycode = code};
//     char * return_p;

//     result = bsearch(&target, lookup_table, num_keycodes, sizeof(lookup_table_t), cmp);

//     if(result != NULL) {
//         return_p = result->key_string;
//     } else {
//         return_p = UNKNOWN_KEYCODE;
//     }

//     return (return_p);
// }

char* translate_keycode_to_string(uint16_t code) {
    char * return_p;

    for (int i=0; i<361; i++) {
        if (lookup_table[i].keycode == code) {
            return_p = lookup_table[i].key_string;
            return return_p;
        }
    }

    return_p = UNKNOWN_KEYCODE;
    return return_p;
}

lookup_table_t lookup_table[361] = {
    {"M0", 0x7700}, //_QK_MACRO
    {"M1", 0x7701},
    {"M2", 0x7702},
    {"M3", 0x7703},
    {"M4", 0x7704},
    {"M5", 0x7705},
    {"M6", 0x7706},
    {"M7", 0x7707},
    {"M8", 0x7708},
    {"M9", 0x7709},
    {"MO(0)", 0x5220}, //_QK_MOMENTARY
    {"MO(1)", 0x5221},
    {"MO(2)", 0x5222},
    {"MO(3)", 0x5223},
    {"MO(4)", 0x5224},
    {"MO(5)", 0x5225},
    {"MO(6)", 0x5226},
    {"MO(7)", 0x5227},
    {"MO(8)", 0x5228},
    {"MO(9)", 0x5229},
    {"TG(0)", 0x5260}, //_QK_TOGGLE_LAYER
    {"TG(1)", 0x5261},
    {"TG(2)", 0x5262},
    {"TG(3)", 0x5263},
    {"TG(4)", 0x5264},
    {"TG(5)", 0x5265},
    {"TG(6)", 0x5266},
    {"TG(7)", 0x5267},
    {"TG(8)", 0x5268},
    {"TG(9)", 0x5269},
    {"TT(0)", 0x52c0}, //_QK_LAYER_TAP_TOGGLE
    {"TT(1)", 0x52c1},
    {"TT(2)", 0x52c2},
    {"TT(3)", 0x52c3},
    {"TT(4)", 0x52c4},
    {"TT(5)", 0x52c5},
    {"TT(6)", 0x52c6},
    {"TT(7)", 0x52c7},
    {"TT(8)", 0x52c8},
    {"TT(9)", 0x52c9},
    {"OSL(0)", 0x5280}, //_QK_ONE_SHOT_LAYER
    {"OSL(1)", 0x5281},
    {"OSL(2)", 0x5282},
    {"OSL(3)", 0x5283},
    {"OSL(4)", 0x5284},
    {"OSL(5)", 0x5285},
    {"OSL(6)", 0x5286},
    {"OSL(7)", 0x5287},
    {"OSL(8)", 0x5288},
    {"OSL(9)", 0x5289},
    {"TO(0)", 0x5200}, //_QK_TO
    {"TO(1)", 0x5201},
    {"TO(2)", 0x5202},
    {"TO(3)", 0x5203},
    {"TO(4)", 0x5204},
    {"TO(5)", 0x5205},
    {"TO(6)", 0x5206},
    {"TO(7)", 0x5207},
    {"TO(8)", 0x5208},
    {"TO(9)", 0x5209},
    {"DF(0)", 0x5240}, //_QK_DEF_LAYER
    {"DF(1)", 0x5241},
    {"DF(2)", 0x5242},
    {"DF(3)", 0x5243},
    {"DF(4)", 0x5244},
    {"DF(5)", 0x5245},
    {"DF(6)", 0x5246},
    {"DF(7)", 0x5247},
    {"DF(8)", 0x5248},
    {"DF(9)", 0x5249},
    {" ", KC_NO},
    {" ", KC_TRNS},
    {"A", KC_A},
    {"B", KC_B},
    {"C", KC_C},
    {"D", KC_D},
    {"E", KC_E},
    {"F", KC_F},
    {"G", KC_G},
    {"H", KC_H},
    {"I", KC_I},
    {"J", KC_J},
    {"K", KC_K},
    {"L", KC_L},
    {"M", KC_M},
    {"N", KC_N},
    {"O", KC_O},
    {"P", KC_P},
    {"Q", KC_Q},
    {"R", KC_R},
    {"S", KC_S},
    {"T", KC_T},
    {"U", KC_U},
    {"V", KC_V},
    {"W", KC_W},
    {"X", KC_X},
    {"Y", KC_Y},
    {"Z", KC_Z},
    {"1", KC_1},
    {"2", KC_2},
    {"3", KC_3},
    {"4", KC_4},
    {"5", KC_5},
    {"6", KC_6},
    {"7", KC_7},
    {"8", KC_8},
    {"9", KC_9},
    {"0", KC_0},
    {"Enter", KC_ENT},
    {"Esc", KC_ESC},
    {"<-", KC_BSPC},
    {"Tab", KC_TAB},
    {"Space", KC_SPC},
    {"--", KC_MINS},
    {"+=", KC_EQL},
    {"{[", KC_LBRC},
    {"}]", KC_RBRC},
    {"\\|", KC_BSLS},
    {"~", KC_NUHS},
    {";:", KC_SCLN},
    {"'\"", KC_QUOT},
    {"`~", KC_GRV},
    {",<", KC_COMM},
    {".>", KC_DOT},
    {"/?", KC_SLSH},
    {"CAPS", KC_CAPS},
    {"F1", KC_F1},
    {"F2", KC_F2},
    {"F3", KC_F3},
    {"F4", KC_F4},
    {"F5", KC_F5},
    {"F6", KC_F6},
    {"F7", KC_F7},
    {"F8", KC_F8},
    {"F9", KC_F9},
    {"F10", KC_F10},
    {"F11", KC_F11},
    {"F12", KC_F12},
    {"Print", KC_PSCR},
    {"Scrl", KC_SCRL},
    {"Pause", KC_PAUS},
    {"Insrt", KC_INS},
    {"Home", KC_HOME},
    {"PGUp", KC_PGUP},
    {"Del", KC_DEL},
    {"End", KC_END},
    {"PGDw", KC_PGDN},
    {"Right", KC_RGHT},
    {"Left", KC_LEFT},
    {"Down", KC_DOWN},
    {"Up", KC_UP},
    {"NLock", KC_NUM},
    {"/", KC_PSLS},
    {"*", KC_PAST},
    {"-", KC_PMNS},
    {"+", KC_PPLS},
    {"PEnter", KC_PENT},
    {"1", KC_P1},
    {"2", KC_P2},
    {"3", KC_P3},
    {"4", KC_P4},
    {"5", KC_P5},
    {"6", KC_P6},
    {"7", KC_P7},
    {"8", KC_P8},
    {"9", KC_P9},
    {"0", KC_P0},
    {".", KC_PDOT},
    {"\\|", KC_NUBS},
    {"App", KC_APP},
    {"Power", KC_KB_POWER},
    {"=", KC_PEQL},
    {"F13", KC_F13},
    {"F14", KC_F14},
    {"F15", KC_F15},
    {"F16", KC_F16},
    {"F17", KC_F17},
    {"F18", KC_F18},
    {"F19", KC_F19},
    {"F20", KC_F20},
    {"F21", KC_F21},
    {"F22", KC_F22},
    {"F23", KC_F23},
    {"F24", KC_F24},
    {"Exec", KC_EXECUTE},
    {"Help", KC_HELP},
    {"Menu", KC_MENU},
    {"Sel", KC_SELECT},
    {"Stop", KC_STOP},
    {"Again", KC_AGAIN},
    {"Undo", KC_UNDO},
    {"Cut", KC_CUT},
    {"Copy", KC_COPY},
    {"Paste", KC_PASTE},
    {"Find", KC_FIND},
    {"LCap", KC_LCAP},
    {"LNum", KC_LNUM},
    {"LSCR", KC_LSCR},
    {",", KC_PCMM},
    {"=", KC_KP_EQUAL_AS400},
    {"Int1", KC_INT1},
    {"Int2", KC_INT2},
    {"Int3", KC_INT3},
    {"Int4", KC_INT4},
    {"Int5", KC_INT5},
    {"Int6", KC_INT6},
    {"Int7", KC_INT7},
    {"Int8", KC_INT8},
    {"Int9", KC_INT9},
    {"Lng1", KC_LNG1},
    {"Lng2", KC_LNG2},
    {"Lng3", KC_LNG3},
    {"Lng4", KC_LNG4},
    {"Lng5", KC_LNG5},
    {"Lng6", KC_LNG6},
    {"Lng7", KC_LNG7},
    {"Lng8", KC_LNG8},
    {"Lng9", KC_LNG9},
    {"Erase", KC_ERAS},
    {"Syrq", KC_SYRQ},
    {"Cancel", KC_CANCEL},
    {"Clear", KC_CLR},
    {"Clear", KC_CLEAR},
    {"Prior", KC_PRIOR},
    {"Out", KC_OUT},
    {"Oper", KC_OPER},
    {"Clear", KC_CLEAR_AGAIN},
    {"Crsl", KC_CRSEL},
    {"Exsel", KC_EXSEL},
    {"Pwr", KC_PWR},
    {"Sleep", KC_SLEP},
    {"Wake", KC_WAKE},
    {"Mute", KC_MUTE},
    {"Vol+", KC_VOLU},
    {"Vol-", KC_VOLD},
    {"Next", KC_MNXT},
    {"Prev", KC_MPRV},
    {"Stop", KC_MSTP},
    {"Play", KC_MPLY},
    {"MSel", KC_MSEL},
    {"Ejct", KC_EJCT},
    {"Mail", KC_MAIL},
    {"Calc", KC_CALC},
    {"Mycm", KC_MYCM},
    {"Next", KC_MFFD},
    {"Prev", KC_MRWD},
    {"Bright+", KC_BRIU},
    {"Bright-", KC_BRID},
    {"LCtrl", KC_LCTL},
    {"LShift", KC_LSFT},
    {"LAlt", KC_LALT},
    {"LMeta", KC_LGUI},
    {"RCtrl", KC_RCTL},
    {"RShift", KC_RSFT},
    {"RAlt", KC_RALT},
    {"RMeta", KC_RGUI},
    {"MsUp", KC_MS_UP},
    {"MsDow", KC_MS_DOWN},
    {"MsLeft", KC_MS_LEFT},
    {"MsRight", KC_MS_RIGHT},
    {"MsBtn1", KC_MS_BTN1},
    {"MsBtn2", KC_MS_BTN2},
    {"MsBtn3", KC_MS_BTN3},
    {"MsBtn4", KC_MS_BTN4},
    {"MsBtn5", KC_MS_BTN5},
    {"WheelUp", KC_MS_WH_UP},
    {"WheelDow", KC_MS_WH_DOWN},
    {"WheelLeft", KC_MS_WH_LEFT},
    {"WheelRight", KC_MS_WH_RIGHT},
    {"Accel0", KC_MS_ACCEL0},
    {"Accel1", KC_MS_ACCEL1},
    {"Accel2", KC_MS_ACCEL2},
    {"!", KC_EXLM},
    {"@", KC_AT},
    {"#", KC_HASH},
    {"K$$", KC_DLR},
    {"%", KC_PERC},
    {"^", KC_CIRC},
    {"&", KC_AMPR},
    {"*", KC_ASTR},
    {"(", KC_LPRN},
    {")", KC_RPRN},
    {"_", KC_UNDS},
    {"+", KC_PLUS},
    {"{", KC_LCBR},
    {"}", KC_RCBR},
    {"|", KC_PIPE},
    {":", KC_COLN},
    {"\"", KC_DQUO},
    {"~", KC_TILD},
    {"<", KC_LT},
    {">", KC_GT},
    {"?", KC_QUES},
    {"BlOn", BL_ON},
    {"BlOff", BL_OFF},
    {"BlDow", BL_DOWN},
    {"BlUp", BL_UP},
    {"BlTog", BL_TOGG},
    {"BlStep", BL_STEP},
    {"BlBRTG", BL_BRTG},
    {"RGB_TOG", RGB_TOG},
    {"RGB_MOD", RGB_MOD},
    {"RGB_RMOD", RGB_RMOD},
    {"RGB_HUI", RGB_HUI},
    {"RGB_HUD", RGB_HUD},
    {"RGB_SAI", RGB_SAI},
    {"RGB_SAD", RGB_SAD},
    {"RGB_VAI", RGB_VAI},
    {"RGB_VAD", RGB_VAD},
    {"RGB_SPI", RGB_SPI},
    {"RGB_SPD", RGB_SPD},
    {"RGB_M_P", RGB_M_P},
    {"RGB_M_B", RGB_M_B},
    {"RGB_M_R", RGB_M_R},
    {"RGB_M_SW", RGB_M_SW},
    {"RGB_M_SN", RGB_M_SN},
    {"RGB_M_K", RGB_M_K},
    {"RGB_M_X", RGB_M_X},
    {"RGB_M_G", RGB_M_G},
    {"M0", QK_MACRO_0},
    {"M1", QK_MACRO_1},
    {"M2", QK_MACRO_2},
    {"M3", QK_MACRO_3},
    {"M4", QK_MACRO_4},
    {"M5", QK_MACRO_5},
    {"M6", QK_MACRO_6},
    {"M7", QK_MACRO_7},
    {"M8", QK_MACRO_8},
    {"M9", QK_MACRO_9},
    {"M10", QK_MACRO_10},
    {"M11", QK_MACRO_11},
    {"M12", QK_MACRO_12},
    {"M13", QK_MACRO_13},
    {"M14", QK_MACRO_14},
    {"M15", QK_MACRO_15},
    {"KB0", QK_KB_0},
    {"KB1", QK_KB_1},
    {"KB2", QK_KB_2},
    {"KB3", QK_KB_3},
    {"KB4", QK_KB_4},
    {"KB5", QK_KB_5},
    {"KB6", QK_KB_6},
    {"KB7", QK_KB_7},
    {"KB8", QK_KB_8},
    {"KB9", QK_KB_9},
    {"KB10", QK_KB_10},
    {"KB11", QK_KB_11},
    {"KB12", QK_KB_12},
    {"KB13", QK_KB_13},
    {"KB14", QK_KB_14},
    {"KB15", QK_KB_15},
    {"M0", QK_MACRO_0},
    {"M1", QK_MACRO_1},
    {"M2", QK_MACRO_2},
    {"M3", QK_MACRO_3},
    {"M4", QK_MACRO_4},
    {"M5", QK_MACRO_5},
    {"M6", QK_MACRO_6},
    {"M7", QK_MACRO_7},
    {"M8", QK_MACRO_8},
    {"M9", QK_MACRO_9},
};

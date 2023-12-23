#pragma once

#include <cstdint>

enum class KeyCode : std::uint16_t
{
    Unknown      = 0,
    A            = 4,
    B            = 5,
    C            = 6,
    D            = 7,
    E            = 8,
    F            = 9,
    G            = 10,
    H            = 11,
    I            = 12,
    J            = 13,
    K            = 14,
    L            = 15,
    M            = 16,
    N            = 17,
    O            = 18,
    P            = 19,
    Q            = 20,
    R            = 21,
    S            = 22,
    T            = 23,
    U            = 24,
    V            = 25,
    W            = 26,
    X            = 27,
    Y            = 28,
    Z            = 29,
    One          = 30,
    Two          = 31,
    Three        = 32,
    Four         = 33,
    Five         = 34,
    Six          = 35,
    Seven        = 36,
    Eight        = 37,
    Nine         = 38,
    Zero         = 39,
    Return       = 40,
    Escape       = 41,
    Backspace    = 42,
    Tab          = 43,
    Space        = 44,
    Minus        = 45,
    Equals       = 46,
    LeftBracket  = 47,
    RightBracket = 48,
    Backslash    = 49,
    NoNusHash    = 50,
    Semicolon    = 51,
    Apostrophe   = 52,
    Grave        = 53,
    Comma        = 54,
    Period       = 55,
    Slash        = 56,

    Capslock       = 57,
    F1             = 58,
    F2             = 59,
    F3             = 60,
    F4             = 61,
    F5             = 62,
    F6             = 63,
    F7             = 64,
    F8             = 65,
    F9             = 66,
    F10            = 67,
    F11            = 68,
    F12            = 69,
    PrintScreen    = 70,
    ScrollLock     = 71,
    Pause          = 72,
    Insert         = 73,
    Home           = 74,
    Pageup         = 75,
    Delete         = 76,
    End            = 77,
    Pagedown       = 78,
    Right          = 79,
    Left           = 80,
    Down           = 81,
    Up             = 82,
    NumLockClear   = 83,
    KpDivide       = 84,
    KpMultiply     = 85,
    KpMinus        = 86,
    KpPlus         = 87,
    KpEnter        = 88,
    KpOne          = 89,
    KpTwo          = 90,
    KpThree        = 91,
    KpFour         = 92,
    KpFive         = 93,
    KpSix          = 94,
    KpSeven        = 95,
    KpEight        = 96,
    KpNine         = 97,
    KpZero         = 98,
    KpPeriod       = 99,
    NonUsBackslash = 100,
    Application    = 101,
    Power          = 102,
    KpEquals       = 103,
    F13            = 104,
    F14            = 105,
    F15            = 106,
    F16            = 107,
    F17            = 108,
    F18            = 109,
    F19            = 110,
    F20            = 111,
    F21            = 112,
    F22            = 113,
    F23            = 114,
    F24            = 115,
    Execute        = 116,
    Help           = 117,
    Menu           = 118,

    Select             = 119,
    Stop               = 120,
    Again              = 121,
    Undo               = 122,
    Cut                = 123,
    Copy               = 124,
    Paste              = 125,
    Find               = 126,
    Mute               = 127,
    VolumeUp           = 128,
    VolumeDown         = 129,
    KpComma            = 133,
    KpEqualsAs400      = 134,
    International1     = 135,
    International2     = 136,
    International3     = 137,
    International4     = 138,
    International5     = 139,
    International6     = 140,
    International7     = 141,
    International8     = 142,
    International9     = 143,
    Lang1              = 144,
    Lang2              = 145,
    Lang3              = 146,
    Lang4              = 147,
    Lang5              = 148,
    Lang6              = 149,
    Lang7              = 150,
    Lang8              = 151,
    Lang9              = 152,
    AltErase           = 153,
    SysReq             = 154,
    Cancel             = 155,
    Clear              = 156,
    Prior              = 157,
    Return2            = 158,
    Separator          = 159,
    Out                = 160,
    Operation          = 161,
    ClearAgain         = 162,
    CrSel              = 163,
    ExSel              = 164,
    Kp00               = 176,
    Kp000              = 177,
    ThousandsSeparator = 178,
    DecimalSeparator   = 179,
    CurrencyUnit       = 180,
    CurrencySubUnit    = 181,
    KpLeftParen        = 182,
    KpRightParen       = 183,
    KpLeftBrace        = 184,
    KpRightBrace       = 185,
    KpTab              = 186,
    KpBackspace        = 187,
    KpA                = 188,
    KpB                = 189,
    KpC                = 190,
    KpD                = 191,
    KpE                = 192,
    KpF                = 193,
    KpXor              = 194,
    KpPower            = 195,
    KpPercent          = 196,
    KpLess             = 197,

    KpGreater          = 198,
    KpAmpersand        = 199,
    KpDblAmpersand     = 200,
    KpVerticalBar      = 201,
    KpDblVerticalBar   = 202,
    KpColon            = 203,
    KpHash             = 204,
    KpSpace            = 205,
    KpAt               = 206,
    KpExclamation      = 207,
    KpMemStore         = 208,
    KpMemRecall        = 209,
    KpMemClear         = 210,
    KpMemAdd           = 211,
    KpMemSubtract      = 212,
    KpMemMultiply      = 213,
    KpMemDivide        = 214,
    KpPlusMinus        = 215,
    KpClear            = 216,
    KpClearEntry       = 217,
    KpBinary           = 218,
    KpOctal            = 219,
    KpDecimal          = 220,
    KpHexadecimal      = 221,
    LCtrl              = 224,
    LShift             = 225,
    LAlt               = 226,
    LGui               = 227,
    RCtrl              = 228,
    RShift             = 229,
    RAlt               = 230,
    RGui               = 231,
    Mode               = 257,
    AudioNext          = 258,
    AudioPrev          = 259,
    AudioStop          = 260,
    AudioPlay          = 261,
    AudioMute          = 262,
    MediaSelect        = 263,
    Www                = 264,
    Mail               = 265,
    Calculator         = 266,
    Computer           = 267,
    AcSearch           = 268,
    AcHome             = 269,
    AcBack             = 270,
    AcForward          = 271,
    AcStop             = 272,
    AcRefresh          = 273,
    AcBookmarks        = 274,
    BrightnessDown     = 275,
    BrightnessUp       = 276,
    DisplaySwitch      = 277,
    IlluminationToggle = 278,
    IlluminationDown   = 279,
    IlluminationUp     = 280,
    Eject              = 281,
    Sleep              = 282,
    App1               = 283,
    App2               = 284,
    AudioRewind        = 285,
    AudioFastForward   = 286,

    Count = 512,
};
static constexpr std::size_t KEY_CODE_COUNT = static_cast<std::size_t>(KeyCode::Count);

enum class MouseButton : std::uint8_t
{
    Primary = 1,
    Middle,
    Secondary,
    Button4,
    Button5,
    Button6,
    Button7,
    Button8,
    Button9,
    Count,
};
static constexpr std::size_t MOUSE_BUTTON_COUNT = static_cast<std::size_t>(MouseButton::Count);

struct KeyPressEvent
{
    KeyCode key_code;
};

struct KeyReleaseEvent
{
    KeyCode key_code;
};

struct MouseMoveEvent
{
    std::int32_t x;
    std::int32_t y;
};

struct MouseButtonPressEvent
{
    std::int32_t x;
    std::int32_t y;
    MouseButton  button;
};

struct MouseButtonReleaseEvent
{
    std::int32_t x;
    std::int32_t y;
    MouseButton  button;
};

struct MouseWheelEvent
{
    std::int32_t x;
    std::int32_t y;
    bool         flipped;
};

struct WindowMoveEvent
{
    std::int32_t x;
    std::int32_t y;
};

struct WindowResizeEvent
{
    std::int32_t width;
    std::int32_t height;
};

struct RenderEvent
{
    float       seconds_elapsed;
    std::size_t frames_rendered;
};

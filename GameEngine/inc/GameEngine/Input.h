#pragma once

#include "Common.h"
#include "Math\Math.h"

namespace GameEngine
{
    constexpr int VKEY_COUNT = 256;

    /**
     * \brief WinUsers.h에 정의된 Virtual Key 값과 1:1로 대응되는 enum 타입입니다.
     *
     * https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
     * http://www.kbdedit.com/manual/low_level_vk_list.html
     */
    enum class EVirtualKey
    {
        /*
         * Mouse virtual keys
         */

        MouseLeft = 0x01,
        MouseRight = 0x02,
        MouseMiddle = 0x04,
        // mouse extension button 1
        XButton1 = 0x05,
        // mouse extension button 2
        XButton2 = 0x06,

        /*
         * Keyboard virtual keys
         */

         Backspace = 0x08,
         Tab = 0x09,
         // VK_RETURN
         Enter = 0x0D,
         Shift = 0x10,
         Control = 0x11,
         // VK_MENU
         Alt = 0x12,
         Pause = 0x13,
         CapsLock = 0x14,
         // VK_ESCAPE
         Escape = 0x1B,
         SpaceBar = 0x20,
         // VK_PRIOR
         PageUp = 0x21,
         // VK_NEXT
         PageDown = 0x22,
         End = 0x23,
         Home = 0x24,
         LeftArrow = 0x25,
         UpArrow = 0x26,
         RightArrow = 0x27,
         DownArrow = 0x28,
         // VK_SNAPSHOT
         PrintScreen = 0x2C,
         Insert = 0x2D,
         Delete = 0x2E,
         Key_0 = 0x30,
         Key_1 = 0x31,
         Key_2 = 0x32,
         Key_3 = 0x33,
         Key_4 = 0x34,
         Key_5 = 0x35,
         Key_6 = 0x36,
         Key_7 = 0x37,
         Key_8 = 0x38,
         Key_9 = 0x39,
         Key_A = 0x41,
         Key_B = 0x42,
         Key_C = 0x43,
         Key_D = 0x44,
         Key_E = 0x45,
         Key_F = 0x46,
         Key_G = 0x47,
         Key_H = 0x48,
         Key_I = 0x49,
         Key_J = 0x4A,
         Key_K = 0x4B,
         Key_L = 0x4C,
         Key_M = 0x4D,
         Key_N = 0x4E,
         Key_O = 0x4F,
         Key_P = 0x50,
         Key_Q = 0x51,
         Key_R = 0x52,
         Key_S = 0x53,
         Key_T = 0x54,
         Key_U = 0x55,
         Key_V = 0x56,
         Key_W = 0x57,
         Key_X = 0x58,
         Key_Y = 0x59,
         Key_Z = 0x5A,
         // VK_LWIN
         LeftWindow = 0x5B,
         // VK_RWIN
         RightWindow = 0x5C,
         // Key ','
         Separator = 0x6C,
         Numpad_0 = 0x60,
         Numpad_1 = 0x61,
         Numpad_2 = 0x62,
         Numpad_3 = 0x63,
         Numpad_4 = 0x64,
         Numpad_5 = 0x65,
         Numpad_6 = 0x66,
         Numpad_7 = 0x67,
         Numpad_8 = 0x68,
         Numpad_9 = 0x69,
         // Numpad '*'
         Multiply = 0x6A,
         // Key '+'
         Add = 0x6B,
         // Numpad '.'
         Decimal = 0x6E,
         // Key '-'
         Subtract = 0x6D,
         // Key '/'
         Divide = 0x6F,
         F1 = 0x70,
         F2 = 0x71,
         F3 = 0x72,
         F4 = 0x73,
         F5 = 0x74,
         F6 = 0x75,
         F7 = 0x76,
         F8 = 0x77,
         F9 = 0x78,
         F10 = 0x79,
         F11 = 0x7A,
         F12 = 0x7B,
         NumLock = 0x90,
         ScrollLock = 0x91,
         LeftShift = 0xA0,
         RightShift = 0xA1,
         LeftControl = 0xA2,
         RightControl = 0xA3,

         /*
          * IME keys
          */

          // VK_KANA
          IME_Kana = 0x15,
          // (deprecated) VK_HANGUEL
          IME_Hanguel = 0x15,
          // VK_HANGUL
          IME_Hangul = 0x15,
          IME_On = 0x16,
          IME_Junja = 0x17,
          IME_Final = 0x18,
          IME_Kanji = 0x19,
          IME_Off = 0x1A,
          IME_Convert = 0x1C,
          IME_NonConvert = 0x1D,
          IME_Accept = 0x1E,
          IME_ModeChange = 0x1F,

          /*
           * unused / rarely used
           */

           // VK_CANCEL
           Break = 0x03,
           Clear = 0x0C,
           Select = 0x29,
           // VK_PRINT
           Print = 0x2A,
           Execute = 0x2B,
           Help = 0x2F,
           // VK_APPS
           Applications = 0x5D,
           // VK_SLEEP
           Sleep = 0x5F,
           F13 = 0x7C,
           F14 = 0x7D,
           F15 = 0x7E,
           F16 = 0x7F,
           F17 = 0x80,
           F18 = 0x81,
           F19 = 0x82,
           F20 = 0x83,
           F21 = 0x84,
           F22 = 0x85,
           F23 = 0x86,
           F24 = 0x87,
           LeftMenu = 0xA4,
           RightMenu = 0xA5,
           Browser_Back = 0xA6,
           Browser_Forward = 0xA7,
           Browser_Refresh = 0xA8,
           Browser_Stop = 0xA9,
           Browser_Search = 0xAA,
           Browser_Favorites = 0xAB,
           Browser_Home = 0xAC,
           Volume_Mute = 0xAD,
           Volume_Down = 0xAE,
           Volume_Up = 0xAF,
           Media_NextTrack = 0xB0,
           Media_PrevTrack = 0xB1,
           Media_Stop = 0xB2,
           // VK_MEDIA_PLAY_PAUSE
           Media_PlayOrPause = 0xB3,
           Launch_Mail = 0xB4,
           // VK_LAUNCH_MEDIA_SELCT
           Launce_SelectMedia = 0xB5,
           Launch_App1 = 0xB6,
           Launch_App2 = 0xB7,
           OEM_1 = 0xBA,
           OEM_Plus = 0xBB,
           OEM_Comma = 0xBC,
           OEM_Minus = 0xBD,
           OEM_Period = 0xBE,
           OEM_2 = 0xBF,
           OEM_3 = 0xC0,
           OEM_4 = 0xDB,
           OEM_5 = 0xDC,
           OEM_6 = 0xDD,
           OEM_7 = 0xDE,
           OEM_8 = 0xDF,
           OEM_102 = 0xE2,
           // VK_PROCESSKEY
           IME_Process = 0xE5,
           // VK_PACKET
           Packet = 0xE7,
           Attn = 0xF6,
           CrSel = 0xF7,
           ExSel = 0xF8,
           EraseEOF = 0xF9,
           Play = 0xFA,
           Zoom = 0xFB,
           NoName = 0xFC,
           PA1 = 0xFD,
           OEM_Clear = 0xFE,

           // 키입력 없음
           None = 0xFF,
    };

    /**
     * @brief 키의 상태를 나타내는 열거형입니다. \n
     * 가장 하위 비트는 현재 프레임에 키가 눌려있었던 상태인지를 나타내고, \n
     * 밑에서 2번째 비트는 이전 프레임에 키가 눌려있었던 상태인지를 나타냅니다. \n
     * 밑에서 3, 4, 5번째 비트는 각각 Down, Up, Hold 상태인지를 나타냅니다.
    */
    enum class EKeyState : short
    {
        // 00
        None = 0b00000,
        // 16
        Up = 0b01010,
        // 01
        // 
        Down = 0b00101,
        // 17
        // 이전 프레임에서도 누르고 있었으며, 이번 프레임에서도 누르고 있는 상태를 나타냅니다.
        Hold = 0b10011,
        // 03
        // 한 프레임 내에서 매우 빠르게 눌렀다 뗀 상태를 나타냅니다.
        DownAndUp = 0b01111
    };

    /**
     * @brief EKeyState의 각 비트가 어떤 상태인지를 나타내는 것인지 정의한 열거형입니다.
    */
    enum class EKeyStateBit : short
    {
        // 이번 프레임에서 뗀 상태일 때 활성화되는 비트입니다.
        Bit_Up = 0b01000,

        // 이번 프레임부터 누르기 시작한 상태일 때 활성화되는 비트입니다.
        Bit_Down = 0b00100,

        // 이전 프레임에서도 누르고 있었으며, 이번 프레임에서도 누르고 있는 상태일 때 활성화되는 비트입니다.
        Bit_Hold = 0b10000,

        // 한 프레임 내에서 매우 빠르게 눌렀다 뗀 상태일 때 활성화되는 비트입니다.
        Bit_DownAndUp = 0b01100,

        // 이번 프레임에 누르고 있었던 상태일 때 활성화되는 비트입니다.
        // Down과 Hold, 그리고 DownAndUp 상태가 이 비트를 가지고 있습니다.
        Bit_Current = 0b00001,

        // 이전 프레임에 누르고 있었던 상태일 때 활성화되는 비트입니다.
        // Up과 DownAndUp 상태가 이 비트를 가지고 있습니다.
        Bit_Previous = 0b00010
    };

    struct KeyMessageInfo
    {
        // 눌린 virtual key의 식별값입니다.
        EVirtualKey vKey;

        // 발생한 메세지가 WM_KEYDOWN이라면 true이고, WM_KEYUP이라면 false입니다.
        bool isDown;

        // WinProc에 메세지가 수신된 time point입니다.
        std::chrono::system_clock::time_point timePoint;
    };

    struct MouseWheelMessageInfo
    {
        int wheelDelta;
        Math::Vector2 screenPoint;
    };

    inline bool operator&(EKeyState _lhs, short _rhs)
    {
        return static_cast<int>(_lhs) & _rhs;
    }

    inline bool operator&(EKeyState _state, EKeyStateBit _bit)
    {
        return static_cast<int>(_state) & static_cast<int>(_bit);
    }

	class Input
	{
		DECLARE_SINGLETON_CLASS(Input)

	public:
        /**
         * @brief 추적 대상의 키의 상태와 마지막으로 갱신된 프레임 시간을 포함하는 구조체입니다.
         */
        struct KeyInfo
        {
            EKeyState state;
            std::chrono::system_clock::time_point lastUpdatedTime;
        };

        void Initialize(HWND _hWnd);

        void PreTick();

        /**
         * @brief 현재 키의 상태를 반환합니다.
         */
        EKeyState GetKeyState(EVirtualKey _vKey);

        /**
         * @brief 현재 키가 눌려있는지에 대한 상태를 반환합니다.
         * 다른 말로, 키의 상태가 Down 또는 Hold일 때 true를 반환합니다.
         */
        bool GetKeyPress(EVirtualKey _vKey);

        /**
         * @brief 이전 프레임에서 키를 누르지 않다가 이번 프레임에 키를 누르기 시작했는지에 대한 여부를 반환합니다.
         * 다른 말로, 키의 상태가 Down 성분을 가질 때 true를 반환합니다.
         */
        bool GetKeyDown(EVirtualKey _vKey);

        /**
         * @brief 이전 프레임에서 키를 누르고 있다가 이번 프레임에 키를 떼기 시작했는지에 대한 여부를 반환합니다.
         * 다른 말로, 키의 상태가 Up 성분을 가질 때 true를 반환합니다.
         */
        bool GetKeyUp(EVirtualKey _vKey);

        int GetAxisHorizontal();

        int GetAxisVertical();

    private:
        static constexpr long long INPUT_TRACE_THRESHOLD_TIME = 500;

        static constexpr int MAX_KEY_MESSAGE_QUEUE_SIZE = 50;

        static constexpr int MAX_MOUSE_WHEEL_MESSAGE_QUEUE_SIZE = 50;

        HWND m_hWnd;

        /**
         * \brief 지금까지 추적된 키의 정보를 저장하는 배열입니다.
         *
         * 추적된 상태들은 항상 현재 프레임에서 갱신되었다는 것을 보장하지 않습니다.
         * 만약, 이번 프레임에서 처음으로 키의 상태가 요청되었을 때
         * 키의 정보의 상태를 갱신하고, 또한 가장 마지막으로 갱신된 시간을 이번 프레임의 시간으로 갱신합니다.
         * 그렇지 않다면 이 키의 상태는 이번 프레임에서 이미 추적된 것이므로, 이 배열의 값은 갱신되지 않습니다.
         */
        KeyInfo m_KeyInfos[VKEY_COUNT];

        bool m_LastKeyMessageDowns[VKEY_COUNT];

        std::queue<KeyMessageInfo> m_KeyMessageQueue;
        std::queue<MouseWheelMessageInfo> m_MouseWheelMessageQueue;

        Math::Vector2 m_CurrentMousePosition;
        Math::Vector2 m_PreviousMousePosition;
        Math::Vector2 m_MouseMovementDelta;
        Math::Vector2 m_LastMouseDragStartPosition;
        Math::Vector2 m_LastMouseDragEndPosition;
        bool m_bMouseMove;

        int m_MouseWheelDelta;
        Math::Vector2 m_MouseWheelPosition;

    public:
        inline Math::Vector2 GetMousePosition() const { return m_CurrentMousePosition; }

        inline Math::Vector2 GetMouseMovementDelta() const { return m_MouseMovementDelta; }

        inline Math::Vector2 GetLastMouseDragStartPosition() const { return m_LastMouseDragStartPosition; }

        inline Math::Vector2 GetLastMouseDragEndPosition() const { return m_LastMouseDragEndPosition; }

    public:
        /**
         * @brief (not_for_clients) KeyMessageQueue에 메세지 정보를 등록합니다. \n
         * 이 메소드는 WinProc에서 WM_KEYDOWN과 WM_KEYUP 메세지를 받았을 때만 호출해야 합니다.
         */
        inline void PushKeyMessage(const KeyMessageInfo& _messageInfo)
        {
            // 중복 키 메세지를 추가하지 않도록
            // message의 keyDown 상태가 이미 추적된 상태와 일치한지 여부를 검사하여 필터링합니다.
            if (m_LastKeyMessageDowns[static_cast<int>(_messageInfo.vKey)] == _messageInfo.isDown)
                return;

            m_LastKeyMessageDowns[static_cast<int>(_messageInfo.vKey)] = _messageInfo.isDown;

            m_KeyMessageQueue.push(_messageInfo);

            if (m_KeyMessageQueue.size() > MAX_KEY_MESSAGE_QUEUE_SIZE)
                m_KeyMessageQueue.pop();
        }

        /**
         * @brief (not_for_clients) keyMessageQueue에서 메세지를 가져옵니다.
         */
        inline bool PopKeyMessage(KeyMessageInfo& _outInfo)
        {
            if (m_KeyMessageQueue.empty())
                return false;

            _outInfo = m_KeyMessageQueue.front();
            m_KeyMessageQueue.pop();
            return true;
        }

        /**
         * @brief (not_for_clients) MouseWheelMessageQueue에 메세지 정보를 등록합니다.
         * 이 메소드는 WinProc에서 WM_MOUSEWHEEL 메세지를 받았을 때만 호출해야 합니다.
         */
        inline void PushMouseWheelMessage(const MouseWheelMessageInfo& _messageInfo)
        {
            m_MouseWheelMessageQueue.push(_messageInfo);

            if (m_MouseWheelMessageQueue.size() > MAX_MOUSE_WHEEL_MESSAGE_QUEUE_SIZE)
                m_MouseWheelMessageQueue.pop();
        }

        /**
         * @brief MouseWheelMessageQueue에서 메세지를 가져옵니다.
         */
        inline bool PopMouseWheelMessage(MouseWheelMessageInfo& _outInfo)
        {
            if (m_MouseWheelMessageQueue.empty())
                return false;

            _outInfo = m_MouseWheelMessageQueue.front();
            m_MouseWheelMessageQueue.pop();
            return true;
        }
	};
}
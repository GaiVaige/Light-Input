#pragma once
#include <map>
#include <string>
#include <functional>

#include "glad.h"
#include "glfw3.h"

//external dependencies
#include "glm/glm.hpp"

//Just lazy and can't really be bothered writing it every time
#define Callback std::function

//courtesy of Finn Morgan
enum class Key {
	Space = GLFW_KEY_SPACE,
	Apostrophe = GLFW_KEY_APOSTROPHE,
	Comma = GLFW_KEY_COMMA,
	Minus_Row = GLFW_KEY_MINUS,
	Minus = GLFW_KEY_MINUS,
	FullStop = GLFW_KEY_PERIOD,
	ForwardSlash = GLFW_KEY_SLASH,
	Zero_Row = GLFW_KEY_0,
	One_Row = GLFW_KEY_1,
	Two_Row = GLFW_KEY_2,
	Three_Row = GLFW_KEY_3,
	Four_Row = GLFW_KEY_4,
	Five_Row = GLFW_KEY_5,
	Six_Row = GLFW_KEY_6,
	Seven_Row = GLFW_KEY_7,
	Eight_Row = GLFW_KEY_8,
	Nine_Row = GLFW_KEY_9,
	Zero = GLFW_KEY_0,
	One = GLFW_KEY_1,
	Two = GLFW_KEY_2,
	Three = GLFW_KEY_3,
	Four = GLFW_KEY_4,
	Five = GLFW_KEY_5,
	Six = GLFW_KEY_6,
	Seven = GLFW_KEY_7,
	Eight = GLFW_KEY_8,
	Nine = GLFW_KEY_9,
	Semicolon = GLFW_KEY_SEMICOLON,
	Equals_Row = GLFW_KEY_EQUAL,
	Equals = GLFW_KEY_EQUAL,
	A = GLFW_KEY_A,
	B = GLFW_KEY_B,
	C = GLFW_KEY_C,
	D = GLFW_KEY_D,
	E = GLFW_KEY_E,
	F = GLFW_KEY_F,
	G = GLFW_KEY_G,
	H = GLFW_KEY_H,
	I = GLFW_KEY_I,
	J = GLFW_KEY_J,
	K = GLFW_KEY_K,
	L = GLFW_KEY_L,
	M = GLFW_KEY_M,
	N = GLFW_KEY_N,
	O = GLFW_KEY_O,
	P = GLFW_KEY_P,
	Q = GLFW_KEY_Q,
	R = GLFW_KEY_R,
	S = GLFW_KEY_S,
	T = GLFW_KEY_T,
	U = GLFW_KEY_U,
	V = GLFW_KEY_V,
	W = GLFW_KEY_W,
	X = GLFW_KEY_X,
	Y = GLFW_KEY_Y,
	Z = GLFW_KEY_Z,
	LeftBracket = GLFW_KEY_LEFT_BRACKET,
	BackSlash = GLFW_KEY_BACKSLASH,
	RightBracket = GLFW_KEY_RIGHT_BRACKET,
	Grave = GLFW_KEY_GRAVE_ACCENT,
	WorldKey1 = GLFW_KEY_WORLD_1,
	WorldKey2 = GLFW_KEY_WORLD_2,

	Escape = GLFW_KEY_ESCAPE,
	Enter = GLFW_KEY_ENTER,
	Tab = GLFW_KEY_TAB,
	Backspace = GLFW_KEY_BACKSPACE,
	Insert = GLFW_KEY_INSERT,
	Delete = GLFW_KEY_DELETE,
	RightArrow = GLFW_KEY_RIGHT,
	LeftArrow = GLFW_KEY_LEFT,
	DownArrow = GLFW_KEY_DOWN,
	UpArrow = GLFW_KEY_UP,
	PageUp = GLFW_KEY_PAGE_UP,
	PageDown = GLFW_KEY_PAGE_DOWN,
	Home = GLFW_KEY_HOME,
	End = GLFW_KEY_END,
	CapsLock = GLFW_KEY_CAPS_LOCK,
	ScrollLock = GLFW_KEY_SCROLL_LOCK,
	NumLock = GLFW_KEY_NUM_LOCK,
	PrintScreen = GLFW_KEY_PRINT_SCREEN,
	Pause = GLFW_KEY_PAUSE,
	F1 = GLFW_KEY_F1,
	F2 = GLFW_KEY_F2,
	F3 = GLFW_KEY_F3,
	F4 = GLFW_KEY_F4,
	F5 = GLFW_KEY_F5,
	F6 = GLFW_KEY_F6,
	F7 = GLFW_KEY_F7,
	F8 = GLFW_KEY_F8,
	F9 = GLFW_KEY_F9,
	F10 = GLFW_KEY_F10,
	F11 = GLFW_KEY_F11,
	F12 = GLFW_KEY_F12,
	F13 = GLFW_KEY_F13,
	F14 = GLFW_KEY_F14,
	F15 = GLFW_KEY_F15,
	F16 = GLFW_KEY_F16,
	F17 = GLFW_KEY_F17,
	F18 = GLFW_KEY_F18,
	F19 = GLFW_KEY_F19,
	F20 = GLFW_KEY_F20,
	F21 = GLFW_KEY_F21,
	F22 = GLFW_KEY_F22,
	F23 = GLFW_KEY_F23,
	F24 = GLFW_KEY_F24,
	F25 = GLFW_KEY_F25,
	Zero_Numpad = GLFW_KEY_KP_0,
	One_Numpad = GLFW_KEY_KP_1,
	Two_Numpad = GLFW_KEY_KP_2,
	Three_Numpad = GLFW_KEY_KP_3,
	Four_Numpad = GLFW_KEY_KP_4,
	Five_Numpad = GLFW_KEY_KP_5,
	Six_Numpad = GLFW_KEY_KP_6,
	Seven_Numpad = GLFW_KEY_KP_7,
	Eight_Numpad = GLFW_KEY_KP_8,
	Nine_Numpad = GLFW_KEY_KP_9,
	FullStop_Numpad = GLFW_KEY_KP_DECIMAL,
	ForwardSlash_Numpad = GLFW_KEY_KP_DIVIDE,
	Asterisk_Numpad = GLFW_KEY_KP_MULTIPLY,
	Minus_Numpad = GLFW_KEY_KP_SUBTRACT,
	Plus_Numpad = GLFW_KEY_KP_ADD,
	Enter_Numpad = GLFW_KEY_KP_ENTER,
	Equals_Numpad = GLFW_KEY_KP_EQUAL,
	LeftShift = GLFW_KEY_LEFT_SHIFT,
	LeftControl = GLFW_KEY_LEFT_CONTROL,
	LeftAlt = GLFW_KEY_LEFT_ALT,
	LeftSuper = GLFW_KEY_LEFT_SUPER,
	RightShift = GLFW_KEY_RIGHT_SHIFT,
	RightControl = GLFW_KEY_RIGHT_CONTROL,
	RightAlt = GLFW_KEY_RIGHT_ALT,
	RightSuper = GLFW_KEY_RIGHT_SUPER,
	Menu = GLFW_KEY_MENU,

	Invalid
};

bool AlreadyRegistered(std::vector<Key>& keysToCheck, Key k);
bool AlreadyRegistered(std::vector<int>& mouseButtonsToCheck, int mb);

struct KeyData {
	bool pressed = false;
	bool released = false;
	bool down = false;
	size_t framePressed = 0;
	size_t frameReleased = 0;
};

using MouseButtonData = KeyData;
struct MouseData {
	glm::vec2 currentMousePositionInScreenSpace = glm::vec2(0);
	glm::vec2 scrollAmount = glm::vec2(0); //x for horizontal, y being vertical
	glm::vec2 positionDelta = glm::vec2(0);
	bool captured = false;
	bool showCursor = true;
};



class InputListener {
private:
	void Init();
	std::map<Key, KeyData> keyMap;
	std::map<int, MouseButtonData> mouseMap;

	MouseData cursor;
	size_t inputThreshold = 3;

	Callback<size_t()> currentFrameFunction;

	GLFWwindow* window;
	float deltaTime = 0;
private:


public:

	struct InputAction {
		InputAction() {};
		InputAction(std::string name) : identifier(name) {};

		void RegisterKey(Key k) {
			if (!AlreadyRegistered(listenedKeys, k)) {
				listenedKeys.push_back(k);
			}
		}

		void RegisterMouseButton(int mb) {
			if (!AlreadyRegistered(listenedMouseButtons, mb)) {
				listenedMouseButtons.push_back(mb);
			}
		}

		std::string identifier;
		std::vector<Key> listenedKeys;
		std::vector<int> listenedMouseButtons;

	};

	struct InputAxis1D {
		InputAxis1D() {};
		InputAxis1D(std::string name) : identifier(name) {};

		void RegisterPositiveAxisKey(Key k) {

			if (AlreadyRegistered(negative, k)) {
				throw(std::exception("Key already registered to opposing axis!"));
				return;
			}

			if (!AlreadyRegistered(positive, k)) {
				positive.push_back(k);
			}
		}

		void RegisterNegativeAxisKey(Key k) {

			if (AlreadyRegistered(positive, k)) {
				throw(std::exception("Key already registered to opposing axis!"));
				return;
			}

			if (!AlreadyRegistered(negative, k)) {
				negative.push_back(k);
			}
		}

		std::string identifier;
		std::vector<Key> positive;
		std::vector<Key> negative;

	};

	struct InputAxis2D {
		InputAxis2D() {};
		InputAxis2D(std::string name) : identifier(name) {};
		std::string identifier = "";
		std::vector<Key> positiveX;
		std::vector<Key> negativeX;
		std::vector<Key> positiveY;
		std::vector<Key> negativeY;
		
		void RegisterPositiveXAxisKey(Key k) {

			if (AlreadyRegistered(negativeX, k)) {
				throw(std::exception("Key already registered to opposing axis!"));
				return;
			}

			if (!AlreadyRegistered(positiveX, k)) {
				positiveX.push_back(k);
			}
		}

		void RegisterNegativeXAxisKey(Key k) {

			if (AlreadyRegistered(positiveX, k)) {
				throw(std::exception("Key already registered to opposing axis!"));
				return;
			}

			if (!AlreadyRegistered(negativeX, k)) {
				negativeX.push_back(k);
			}
		}

		void RegisterPositiveYAxisKey(Key k) {

			if (AlreadyRegistered(negativeY, k)) {
				throw(std::exception("Key already registered to opposing axis!"));
				return;
			}

			if (!AlreadyRegistered(positiveY, k)) {
				positiveY.push_back(k);
			}
		}

		void RegisterNegativeYAxisKey(Key k) {

			if (AlreadyRegistered(positiveY, k)) {
				throw(std::exception("Key already registered to opposing axis!"));
				return;
			}

			if (!AlreadyRegistered(negativeY, k)) {
				negativeY.push_back(k);
			}
		}

	};

	struct InputProfile {
		InputProfile() {};
		InputProfile(std::string);
		std::string name = "";

		std::map<std::string, InputAction> storedInputActions;
		std::map<std::string, InputAxis1D> stored1DAxes;
		std::map<std::string, InputAxis2D> stored2DAxes;
	};


private:
	std::map<std::string, InputProfile> inputProfiles;
	InputProfile* activeInputProfile;


public:
	/// <summary>
	/// Should only be called after initialisation of the GLFWWindow
	/// </summary>
	/// <param name="window">: The active GLFW window</param>
	InputListener(GLFWwindow* window);

	/// <summary>
	/// Allows re-assignment of the window at runtime
	/// </summary>
	/// <param name="w">: the new GLFW window</param>
	void AttachToWindow(GLFWwindow* w) { window = w; }

	/// <summary>
	/// Called per frame, resets any per frame events (pressed, released) to ensure that between frames that state
	/// is not held.
	/// </summary>
	void UpdateInputState(float deltaTime);

	/// <summary>
	/// Sets the number of frames required to register an input as 'held down' for the 'Is[]Held' function
	/// </summary>
	/// <param name="amount">: number of frames of delay before being registered as held</param>
	inline void SetInputThreshold(size_t amount) { inputThreshold = amount; }

	bool IsKeyDown(Key k);

	bool IsKeyUp(Key k);

	bool IsKeyHeld(Key k);

	bool IsKeyPressed(Key k);

	bool IsKeyReleased(Key k);

	size_t GetFramesHeldDown(Key k);

	size_t GetFramesUp(Key k);

	bool IsMouseButtonDown(int m);

	bool IsMouseButtonUp(int m);
	bool IsMouseButtonPressed(int m);
	bool IsMouseButtonReleased(int m);

	size_t GetFramesHeldDown(int mb);
	size_t GetFramesUp(int mb);
	float GetScrollAmount();
	
	glm::vec2 GetMousePos();
	glm::vec2 GetMousePosNDC();
	glm::vec2 GetMousePosDelta();
	glm::vec2 GetMouseVelocity();
	
	void AttachCurrentFrameFunction(Callback<size_t()> func);

	void SetCaptureMouse(bool state);
	void SetShowMouseCursor(bool state);

private:
	void UpdateMouseState();
public:

	int GetInputAxis(Key pos, Key neg);

	void AddInputProfile(std::string name);
	void SetActiveInputProfile(std::string name);

	void RegisterInputAction(std::string name);
	void RegisterInputAction(InputAction action);
	bool GetInputActionState(std::string name);

	void RegisterInputAxis1D(std::string name);
	void RegisterInputAxis1D(InputAxis1D axis);
	int GetInputAxis1DValue(std::string name);

	void RegisterInputAxis2D(std::string name);
	void RegisterInputAxis2D(InputAxis2D axis);
	glm::vec2 GetInputAxis2DValue(std::string name);

	void SerialiseInputSettingsToFile();
	void LoadInputProfile();

private:

	void UpdateMousePosData();
};
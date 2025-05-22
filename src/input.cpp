#include "input.h"

#include <iostream>
#include <format>
#include <fstream>

//external dependencies
#include "nlohmannjson/json.hpp"

std::map<Key, KeyData>* keyMapRef = nullptr;
std::map<int, MouseButtonData>* mouseDataRef = nullptr;
MouseData* mouseRef = nullptr;
Callback<size_t()>* cFrameFunc = nullptr;
void key_callback_function(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN) // media control keys and other unhandled keys
		return;

	KeyData& k = keyMapRef->at((Key)key);
	if (action == GLFW_PRESS) {
		k.down = true;
		k.pressed = true;
		k.released = false;
		k.framePressed = (*cFrameFunc)();
		k.frameReleased = -1;
	}
	else if (action == GLFW_RELEASE) {
		k.released = true;
		k.pressed = false;
		k.down = false;
		k.framePressed = -1;
		k.frameReleased = (*cFrameFunc)();
	}
	else if (action == GLFW_REPEAT) {
		k.pressed = false;
	}
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	
	assert(button <= 8);

	MouseButtonData& m = mouseDataRef->at(button);
	if (action == GLFW_PRESS) {
		m.pressed = true;
		m.down = true;
		m.released = false;
		m.framePressed = (*cFrameFunc)();
		m.frameReleased = -1;
	}
	else if (action == GLFW_RELEASE) {
		m.released = true;
		m.pressed = false;
		m.down = false;
		m.framePressed = -1;
		m.frameReleased = (*cFrameFunc)();
	}
	else if (action == GLFW_REPEAT) {
		m.pressed = false;
	}
}

void mouse_scroll_callback(GLFWwindow* window, double xoff, double yoff) {
	mouseRef->scrollAmount = glm::vec2(xoff, yoff);
}

void InputListener::Init()
{
	keyMapRef = &keyMap;
	mouseDataRef = &mouseMap;
	mouseRef = &cursor;

	glfwSetKeyCallback(window, key_callback_function);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	for (int i = 1; i < 349; i++) {
		int x = glfwGetKeyScancode(i);
		if (x != -1) {
			keyMap[(Key)i] = KeyData();
		}
	}
	//missing F25 key lol
	
	//left is 0
	for (int i = 0; i < 8; i++) {
		mouseMap[i] = MouseButtonData();
	}

	//here we try and load input data
	LoadInputProfile();

}

InputListener::InputListener(GLFWwindow* win)
{
	window = win;
	//TODO: Remove when giving to Finn
	//if (systemdata::engine->inputManager) {
	//	LogErrorAndBreak(ErrorCode::ATTEMPTED_SINGLETON_OVERWRITE);
	//}
	Init();
}

void InputListener::UpdateInputState(float dt)
{
	deltaTime = dt;
	//firstly, pressed and released are only true for the update they were detected on!
	std::map<Key, KeyData>::iterator it;
	for (it = keyMap.begin(); it != keyMap.end(); it++) {
		if (it->second.pressed) it->second.pressed = false;
		if (it->second.released) it->second.released = false;
	}

	UpdateMousePosData();

	std::map<int, MouseButtonData>::iterator it2;
	for (it2 = mouseMap.begin(); it2 != mouseMap.end(); it2++) {
		if (it2->second.pressed) it2->second.pressed = false;
		if (it2->second.released) it2->second.released = false;
	}
}

bool InputListener::IsKeyDown(Key k)
{
	return keyMap[k].down;
}

bool InputListener::IsKeyUp(Key k)
{
	return !keyMap[k].down;
}

bool InputListener::IsKeyHeld(Key k)
{
	return GetFramesHeldDown(k) > inputThreshold;
}

bool InputListener::IsKeyPressed(Key k)
{
	return keyMap[k].pressed;
}

bool InputListener::IsKeyReleased(Key k)
{
	return keyMap[k].released;
}

size_t InputListener::GetFramesHeldDown(Key k)
{
	return keyMap[k].framePressed != -1 ? currentFrameFunction() - keyMap[k].framePressed : 0;
}

size_t InputListener::GetFramesUp(Key k)
{
	return keyMap[k].frameReleased != -1 ? currentFrameFunction() - keyMap[k].frameReleased : 0;
}

bool InputListener::IsMouseButtonDown(int m)
{
	return mouseMap[m].down;
}

bool InputListener::IsMouseButtonUp(int m)
{
	return !mouseMap[m].down;
}

bool InputListener::IsMouseButtonPressed(int m)
{
	return mouseMap[m].pressed;
}

bool InputListener::IsMouseButtonReleased(int m)
{
	return mouseMap[m].released;
}

size_t InputListener::GetFramesHeldDown(int mb)
{
	return mouseMap[mb].framePressed != -1 ? currentFrameFunction() - mouseMap[mb].framePressed : 0;
}

size_t InputListener::GetFramesUp(int mb)
{
	return mouseMap[mb].frameReleased != -1 ? currentFrameFunction() - mouseMap[mb].frameReleased : 0;;
}

float InputListener::GetScrollAmount()
{
	return cursor.scrollAmount.y;
}

glm::vec2 InputListener::GetMousePos()
{
	return cursor.currentMousePositionInScreenSpace;
}

glm::vec2 InputListener::GetMousePosNDC()
{
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	glm::vec2 mPos = GetMousePos();
	return glm::vec2(mPos.x / (w/2) - 1, -(mPos.y / (h /2) - 1));
}

glm::vec2 InputListener::GetMousePosDelta()
{
	return cursor.positionDelta;
}

glm::vec2 InputListener::GetMouseVelocity()
{
	return GetMousePosDelta() * deltaTime;
}

void InputListener::AttachCurrentFrameFunction(Callback<size_t()> func)
{
	currentFrameFunction = func;
	cFrameFunc = &currentFrameFunction;
}

void InputListener::SetCaptureMouse(bool state)
{
	if (mouseRef->captured != state) {
		mouseRef->captured = state;
		UpdateMouseState();
	}
	
}

void InputListener::SetShowMouseCursor(bool state)
{
	if (mouseRef->showCursor != state) {
		mouseRef->showCursor = state;
		UpdateMouseState();
	}
}

void InputListener::UpdateMouseState()
{
	if (mouseRef->showCursor) {
		if (mouseRef->captured) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	else {
		if (mouseRef->captured) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
	}
	
}

int InputListener::GetInputAxis(Key pos, Key neg)
{
	int a = keyMap[pos].down ? 1 : 0;
	int b = keyMap[neg].down ? 1 : 0;
	return a - b;
}

void InputListener::AddInputProfile(std::string name)
{
	if (!inputProfiles.contains(name)) {
		inputProfiles[name] = InputProfile(name);
	}
}

void InputListener::SetActiveInputProfile(std::string name)
{
	if (!inputProfiles.contains(name)) {
		return;
	}

	activeInputProfile = &inputProfiles[name];
}

void InputListener::RegisterInputAction(std::string name)
{
	if (!activeInputProfile->storedInputActions.contains(name)) {
		activeInputProfile->storedInputActions[name] = InputAction(name);
	}
}

void InputListener::RegisterInputAction(InputAction action)
{
	if (!activeInputProfile->storedInputActions.contains(action.identifier)) {
		activeInputProfile->storedInputActions[action.identifier] = InputAction(action);
	}
}

bool InputListener::GetInputActionState(std::string name)
{
	if (!activeInputProfile->storedInputActions.contains(name)) {
		throw(std::exception("Input action specified not defined in profile!"));
	}

	for (Key& k : activeInputProfile->storedInputActions[name].listenedKeys) {
		if (IsKeyDown(k)) {
			return true;
		}
	}

	for (int i : activeInputProfile->storedInputActions[name].listenedMouseButtons) {
		if (IsMouseButtonDown(i)) {
			return true;
		}
	}

	return false;
}

void InputListener::RegisterInputAxis1D(std::string name)
{
	if (!activeInputProfile->stored1DAxes.contains(name)) {
		activeInputProfile->stored1DAxes[name] = InputAxis1D(name);
	}
}

void InputListener::RegisterInputAxis1D(InputAxis1D axis)
{
	if (!activeInputProfile->stored1DAxes.contains(axis.identifier)) {
		activeInputProfile->stored1DAxes[axis.identifier] = axis;
	}
}

int InputListener::GetInputAxis1DValue(std::string name)
{
	if (!activeInputProfile->stored1DAxes.contains(name)) {
		throw(std::exception("Input axis specified not defined in profile!"));
	}

	int overall = 0;

	for (Key& k : activeInputProfile->stored1DAxes[name].positive) {
		if (IsKeyDown(k)) {
			overall++;
		}
	}

	for (Key& k : activeInputProfile->stored1DAxes[name].negative) {
		if (IsKeyDown(k)) {
			overall--;
		}
	}
	if (overall > 0) return 1;
	if (overall < 0) return -1;
	return 0;
}

void InputListener::RegisterInputAxis2D(std::string name)
{
	if (!activeInputProfile->stored2DAxes.contains(name)) {
		activeInputProfile->stored2DAxes[name] = InputAxis2D(name);
	}
}

void InputListener::RegisterInputAxis2D(InputAxis2D axis)
{
	if (!activeInputProfile->stored2DAxes.contains(axis.identifier)) {
		activeInputProfile->stored2DAxes[axis.identifier] = axis;
	}
}

glm::vec2 InputListener::GetInputAxis2DValue(std::string name)
{
	if (!activeInputProfile->stored2DAxes.contains(name)) {
		throw(std::exception("Input axis specified not defined in profile!"));
	}

	glm::vec2 overall = glm::vec2(0);
	for (Key& k : activeInputProfile->stored2DAxes[name].positiveX) {
		if (IsKeyDown(k)) {
			overall.x++;
		}
	}

	for (Key& k : activeInputProfile->stored2DAxes[name].negativeX) {
		if (IsKeyDown(k)) {
			overall.x--;
		}
	}

	for (Key& k : activeInputProfile->stored2DAxes[name].positiveY) {
		if (IsKeyDown(k)) {
			overall.y++;
		}
	}

	for (Key& k : activeInputProfile->stored2DAxes[name].negativeY) {
		if (IsKeyDown(k)) {
			overall.y--;
		}
	}

	if (glm::length(overall) < .05) {
		return glm::vec2(0);
	}

	return glm::normalize(overall);

}

void InputListener::SerialiseInputSettingsToFile()
{
	std::map<std::string, InputProfile>::iterator iter;
	nlohmann::json masterlist;
	masterlist = nlohmann::json::object();
	masterlist["profilenames"] = nlohmann::json::array();
	for (iter = inputProfiles.begin(); iter != inputProfiles.end(); iter++) {
		masterlist["profilenames"].push_back(iter->second.name);

		nlohmann::json file = nlohmann::json::object();
		file["profilename"] = iter->second.name;
		file["inputactions"] = nlohmann::json::array();
		//first we iterate the actions
		std::map<std::string, InputAction>::iterator it;
		for (it = iter->second.storedInputActions.begin(); it != iter->second.storedInputActions.end(); it++) {
			nlohmann::json add = nlohmann::json::object();

			add["identifier"] = it->second.identifier;
			add["keys"] = nlohmann::json::array();
			for (Key& k : it->second.listenedKeys) {
				add["keys"].push_back((int)k);
			}

			add["mousebuttons"] = nlohmann::json::array();
			for (int i : it->second.listenedMouseButtons) {
				add["mousebuttons"].push_back(i);
			}
			file["inputactions"].push_back(add);
		}

		file["inputaxes1D"] = nlohmann::json::array();
		std::map<std::string, InputAxis1D>::iterator it2;
		for (it2 = iter->second.stored1DAxes.begin(); it2 != iter->second.stored1DAxes.end(); it2++) {
			nlohmann::json add = nlohmann::json::object();
			add["identifier"] = it2->second.identifier;
			add["positiveaxiskeys"] = nlohmann::json::array();
			
			for (Key& k : it2->second.positive) {
				add["positiveaxiskeys"].push_back((int)k);
			}
			add["negativeaxiskeys"] = nlohmann::json::array();
			for (Key& k : it2->second.negative) {
				add["negativeaxiskeys"].push_back((int)k);
			}
			file["inputaxes1D"].push_back(add);
		}


		file["inputaxes2D"] = nlohmann::json::array();
		std::map<std::string, InputAxis2D>::iterator it3;
		for (it3 = iter->second.stored2DAxes.begin(); it3 != iter->second.stored2DAxes.end(); it3++) {
			nlohmann::json add = nlohmann::json::object();
			add["identifier"] = it3->second.identifier;

			add["positivexaxiskeys"] = nlohmann::json::array();
			for (Key& k : it3->second.positiveX) {
				add["positivexaxiskeys"].push_back((int)k);
			}
			add["positiveyaxiskeys"] = nlohmann::json::array();
			for (Key& k : it3->second.positiveY) {
				add["positiveyaxiskeys"].push_back((int)k);
			}

			add["negativexaxiskeys"] = nlohmann::json::array();
			for (Key& k : it3->second.negativeX) {
				add["negativexaxiskeys"].push_back((int)k);
			}
			add["negativeyaxiskeys"] = nlohmann::json::array();
			for (Key& k : it3->second.negativeY) {
				add["negativeyaxiskeys"].push_back((int)k);
			}
			file["inputaxes2D"].push_back(add);
		}



		std::ofstream dumper;
		dumper.open(std::format("Input/{}.iprof.json", iter->second.name));
		dumper << file.dump(2);
		dumper.close();
	}
	std::ofstream out;
	out.open("Input/inputprofiles.list.json");
	out << masterlist.dump(2);
	out.close();

}

void InputListener::LoadInputProfile()
{
	std::ifstream file;

	file.open("Input/inputprofiles.list.json");

	if (file.bad() || file.fail()) {
		std::cout << ("Profile list not found! Initialising input profile list... \n");
		std::cout << ("Please ensure that profile list is not excluded from source control! \n");
		std::ofstream initialiser;
		initialiser.open("Input/inputprofiles.list.json");
		initialiser.close();
		return;
	}

	std::stringstream fileContents;

	fileContents << file.rdbuf();
	file.close();

	std::string list = fileContents.str();

	auto listContents = nlohmann::json::parse(list);
	try {
		for (auto& prof : listContents["profilenames"]) {
			std::ifstream inpFile;
			inpFile.open(std::format("Input/{}.iprof.json", (std::string)prof));
			if (inpFile.bad() || inpFile.fail()) {
				throw(std::exception("Attempting to open missing input profile!"));
			}

			std::stringstream activeLoad;
			activeLoad << inpFile.rdbuf();
			inpFile.close();

			auto fileContents = nlohmann::json::parse(activeLoad.str());

			InputProfile profile;
			profile.name = fileContents["profilename"];

			for (auto& ia : fileContents["inputactions"]) {
				InputAction iatoadd(ia["identifier"]);
				for (auto& k : ia["keys"]) {
					iatoadd.RegisterKey((Key)k);
				}

				for (auto& mb : ia["mousebuttons"]) {
					iatoadd.RegisterMouseButton(mb);
				}
				profile.storedInputActions[iatoadd.identifier] = iatoadd;
			}

			for (auto& iax1 : fileContents["inputaxes1D"]) {
				InputAxis1D iaxtoadd(iax1["identifier"]);

				for (auto& k : iax1["positiveaxiskeys"]) {
					iaxtoadd.RegisterPositiveAxisKey((Key)k);
				}

				for (auto& k : iax1["negativeaxiskeys"]) {
					iaxtoadd.RegisterNegativeAxisKey((Key)k);
				}
				profile.stored1DAxes[iaxtoadd.identifier] = iaxtoadd;
			}

			for (auto& iax2 : fileContents["inputaxes2D"]) {
				InputAxis2D iaxtoadd(iax2["identifier"]);

				for (auto& k : iax2["positivexaxiskeys"]) {
					iaxtoadd.RegisterPositiveXAxisKey((Key)k);
				}

				for (auto& k : iax2["negativexaxiskeys"]) {
					iaxtoadd.RegisterNegativeXAxisKey((Key)k);
				}

				for (auto& k : iax2["positiveyaxiskeys"]) {
					iaxtoadd.RegisterPositiveYAxisKey((Key)k);
				}

				for (auto& k : iax2["negativeyaxiskeys"]) {
					iaxtoadd.RegisterNegativeYAxisKey((Key)k);
				}
				profile.stored2DAxes[iaxtoadd.identifier] = iaxtoadd;
			}

			inputProfiles[profile.name] = profile;

		}
		std::map<std::string, InputProfile>::iterator it = inputProfiles.begin();
		activeInputProfile = &it->second;

	}
	catch (nlohmann::json::parse_error& error) {
		throw(std::exception(std::format("Error in json loading: {}", error.what()).c_str()));
		//LogErrorAndBreak(INPUT_LOAD_FAIL, std::format("Failure when loading {} as json. {}", list, error.what()));
	}
	
}

void InputListener::UpdateMousePosData()
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec2 newPos = glm::vec2(xpos, ypos);

	//conversion step to get into +x right +y up
	mouseRef->positionDelta = newPos - mouseRef->currentMousePositionInScreenSpace;
	mouseRef->positionDelta = glm::vec2(mouseRef->positionDelta.x, -mouseRef->positionDelta.y);
	mouseRef->currentMousePositionInScreenSpace = newPos;
}

bool AlreadyRegistered(std::vector<Key>& keysToCheck, Key k)
{
	{
		if (std::find(keysToCheck.begin(), keysToCheck.end(), k) != keysToCheck.end()) {
			return true;
		}
		return false;
	}
}

bool AlreadyRegistered(std::vector<int>& mouseButtonsToCheck, int mb)
{
	{
		if (std::find(mouseButtonsToCheck.begin(), mouseButtonsToCheck.end(), mb) != mouseButtonsToCheck.end()) {
			return true;
		}
		return false;
	}
}

InputListener::InputProfile::InputProfile(std::string enteredName)
{
	name = enteredName;
}

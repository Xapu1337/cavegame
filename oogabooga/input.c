/*

	Main input juice:
	
		bool is_key_down(Input_Key_Code code)
		bool is_key_up(Input_Key_Code code)
		bool is_key_just_pressed(Input_Key_Code code)
		bool is_key_just_released(Input_Key_Code code)
		
		bool consume_key_down(Input_Key_Code code)
		bool consume_key_just_pressed(Input_Key_Code code)
		bool consume_key_just_released(Input_Key_Code code)

	To loop through events this frame:
	
		for (u64 i = 0; i < input_frame.number_of_events; i++) {
			Input_Event e = input_frame.events[i];
			
			switch (e.kind) {
				case INPUT_EVENT_KEY:    ...; break;
				case INPUT_EVENT_SCROLL: ...; break;
				case INPUT_EVENT_TEXT:   ...; break;
				case INPUT_EVENT_GAMEPAD_AXIS:   ...; break;
			}
		}
	
	Gamepad input:
	
		(!! Only Xbox gamepads are natively supported. Steam will map playstation controllers to xbox, but you need third party software to map playstation controllers to xbox. See DS4Windows. !!)
	
		Vector2 input_frame.left_stick
		Vector2 input_frame.right_stick
		float32 input_frame.left_trigger
		float32 input_frame.right_trigger
	
	Gamepad buttons are treated as regular keys.
	
	Example:
	
		is_key_just_pressed(GAMEPAD_A);
		is_key_down(GAMEPAD_DPAD_LEFT);
	
	To handle multiple gamepads, you need to poll input events and use the event.gamepad_index.
	
	For gamepad buttons: INPUT_EVENT_KEY
	For gamepad axes: INPUT_EVENT_GAMEPAD_AXIS
	
	Set axis deadzones with global variables:
		Vector2 deadzone_left_stick
		Vector2 deadzone_right_stick
		float32 deadzone_left_trigger
		float32 deadzone_right_trigger
		
	Set vibration with:
		set_gamepad_vibration(float32 left, float32 right)
		set_specific_gamepad_vibration(int gamepad_index, float32 left, float32 right)
	
	see input_example.c for example code in practice
*/

#include "input.h"

// #Global
forward_global const u64 MAX_NUMBER_OF_GAMEPADS; // Defined in os layer
ogb_instance Input_Frame input_frame;
ogb_instance Vector2 deadzone_left_stick;
ogb_instance Vector2 deadzone_right_stick;
ogb_instance float32 deadzone_left_trigger;
ogb_instance float32 deadzone_right_trigger;

#if !OOGABOOGA_LINK_EXTERNAL_INSTANCE
Input_Frame input_frame = ZERO(Input_Frame);
Vector2 deadzone_left_stick = {0.2, 0.2};
Vector2 deadzone_right_stick = {0.2, 0.2};
float32 deadzone_left_trigger = {0.07};
float32 deadzone_right_trigger = {0.07};
#endif

// 0.0 for no vibration, 1.0 for max vibration
void set_gamepad_vibration(float32 left, float32 right);
void set_specific_gamepad_vibration(u64 gamepad_index, float32 left, float32 right);

bool has_key_state(Input_Key_Code code, Input_State_Flags flags) {
	assert(code > 0 && code < INPUT_KEY_CODE_COUNT, "Invalid key code %d!", code);
	Input_State_Flags state = input_frame.key_states[code];
	
#if CONFIGURATION == DEBUG
	{
		Input_State_Flags impossible = (INPUT_STATE_JUST_RELEASED | INPUT_STATE_DOWN);
		assert((impossible & state) != impossible, "Key state for key '%d' is corrupt!", code);
		impossible = (INPUT_STATE_JUST_RELEASED | INPUT_STATE_JUST_PRESSED);
		assert((impossible & state) != impossible, "Key state for key '%d' is corrupt!", code);
	}
#endif
	return (state & flags) == flags;
}
bool is_key_down(Input_Key_Code code) {
	return has_key_state(code, INPUT_STATE_DOWN);
}
bool is_key_up(Input_Key_Code code) {
	return input_frame.key_states[code] == 0 || has_key_state(code, INPUT_STATE_JUST_RELEASED);
}
bool is_key_just_pressed(Input_Key_Code code) {
	return has_key_state(code, INPUT_STATE_JUST_PRESSED);
}
bool is_key_just_released(Input_Key_Code code) {
	return has_key_state(code, INPUT_STATE_JUST_RELEASED);
}

bool consume_key_down(Input_Key_Code code) {
	bool result = is_key_down(code);
	input_frame.key_states[code] &= ~(INPUT_STATE_DOWN);
	return result;
}
bool consume_key_just_pressed(Input_Key_Code code) {
	bool result = is_key_just_pressed(code);
	input_frame.key_states[code] &= ~(INPUT_STATE_JUST_PRESSED);
	return result;
}
bool consume_key_just_released(Input_Key_Code code) {
	bool result = is_key_just_released(code);
	input_frame.key_states[code] &= ~(INPUT_STATE_JUST_RELEASED);
	return result;
}
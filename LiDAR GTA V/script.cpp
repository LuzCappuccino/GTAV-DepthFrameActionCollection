#define _USE_MATH_DEFINES
#include "script.h"
#include "keyboard.h"
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include "inc\json.hpp"

#pragma warning(disable : 4244 4305) // double <-> float conversions

using json = nlohmann::json;

extern "C" {
	__declspec(dllimport) int export_get_depth_buffer(void** buf);
}

const Hash model_hashes[] = {
	/*Abigail*/1074457665,/*Tanisha*/226559113,
	/*Paige*/357551935, /* FinGuru*/ 1189322339,
	/*AirHostess*/ 1567728751, /*AirWorker*/ 1644266841,
	/*AntonB*/ -815646164, /*Barry*/ 797459875,
	/*BallaEast01*/ -198252413, /*BabyD*/ -636391810,
	/*BayWatch01*/ 1250841910, /*Beach02*/ 600300561,
	/*Bride*/ 1633872967, /*Business01*/ -912318012,
	/*Business02*/ 532905404, /*Business02*/ 826475330,
	/*Casey*/ -520477356, /*Chef01*/ 261586155,
	/*Chef*/ 1240128502, /*ChinGoonCutscene*/ -1463670378,
	/*Cletus*/ -429715051, /*ComJane*/ -1230338610,
	/*Construct01SMY*/ -673538407, /*Construct02SMY*/ -973145378,
	/*Cop01 SKIPPED*/ 368603149, /*Cop01*/ 1581098148,
	/*Dale*/ 1182012905, /*Doctor01*/ -730659924,
	/*Families01*/ 1309468115, /*Farmer01*/ -1806291497, 
	/*SouCent03 NOT IN*/ -1948675910,
	/*Scientist01 NOT*/ 1092080539, 
};

struct Action {
	std::string name;
	int duration;
	int sample_rate;
};

void notificationOnLeft(std::string notificationText) {
	UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
	const int maxLen = 99;
	for (int i = 0;i < notificationText.length(); i += maxLen) {
		std::string divideText = notificationText.substr(i, min(maxLen, notificationText.length() - i));
		const char* divideTextAsConstCharArray = divideText.c_str();
		char* divideTextAsCharArray = new char[divideText.length() + 1];
		strcpy_s(divideTextAsCharArray, divideText.length() + 1, divideTextAsConstCharArray);
		UI::_ADD_TEXT_COMPONENT_STRING(divideTextAsCharArray);
	}
	int handle = UI::_DRAW_NOTIFICATION(false, 1);
}

void MsgLog(std::string message) {
	std::ofstream file("LiDAR GTA V/DepthFramePlugin.log", std::ofstream::out | std::ofstream::app);
	if (!file.is_open()) {
		return;
	}
	file << message + "\n";
	file.close();
}

void GameplayFreeze(bool param) {
	GAMEPLAY::SET_GAME_PAUSED(param);
	TIME::PAUSE_CLOCK(param);
}

void ReadLocations(std::string file_name, std::vector<Vector3>& locations) {
	if (!locations.empty()) {
		locations.clear();
	}
	std::ifstream file(file_name);
	if (!file.is_open()) {
		return;
	}
	std::string line;
	while (std::getline(file, line)) {
		Vector3 coord;
		std::stringstream ss(line);

		ss >> coord.x; 
		ss >> coord.y;
		ss >> coord.z;
		locations.push_back(coord);
	}
}

void ReadActions(std::string file_name, std::vector<Action>& actions) {
	if (!actions.empty()) {
		actions.clear();
	}
	std::ifstream file(file_name);
	if (!file.is_open()) {
		return;
	}
	json j = json::parse(file);
	file.close();
	for (json::iterator it = j.begin(); it != j.end(); it++) {
		std::string name = it.key();
		int duration = j.at(it.key()).at("duration");
		int rate = j.at(it.key()).at("sample_rate");
		Action a = { name, duration, rate };
		actions.push_back(a);
	}
}

void TeleportToCoords(Entity e, Vector3 coords) {
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
	WAIT(0);
}

void RecordLocation(std::string loc_fn) {
	std::ofstream file(loc_fn, std::ios_base::app);
	if (!file.is_open()) {
		return;
	}
	Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
	file << coords.x << " " << coords.y << " " << coords.z << "\n";
	file.close();
}

void ReverseCamera() {
	float heading_offset = -175;
	float pitch_offset = -4;

	float new_heading = CAM::GET_GAMEPLAY_CAM_RELATIVE_HEADING() + heading_offset;
	float new_pitch = CAM::GET_GAMEPLAY_CAM_RELATIVE_PITCH() + pitch_offset;
	CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(new_heading);
	CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(new_pitch, 1.0f);
}

Ped SpawnPed(Hash model) {
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model)) {
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model)) {
			WAIT(1);
		}
		int ped_type = 26; // should just be "human"
		float heading = 0;
		float offset = 0.0;
		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), offset, offset, offset);
		Ped ped = PED::CREATE_PED(ped_type, model, coords.x, coords.y, coords.z, heading, false, false);
		return ped;
	}
	else {
		Ped ped = NULL;
		return ped;
	}
}

void ChangePlayerModel(Hash model) {
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model)) {
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model)) {
			WAIT(1);
		}
		PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
		WAIT(100);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}
}

void StartPedScenario(Ped ped, std::string scenario) {
	AI::TASK_START_SCENARIO_IN_PLACE(ped, const_cast<char*>(scenario.c_str()), 0, true);
}

void EndPedScenario(Ped ped) {
	AI::CLEAR_PED_TASKS(ped);
}

std::vector<int> StartingPoints(std::string filename) {
	std::ifstream file(filename);
	std::vector<int> start;
	if (!file.is_open()) {
		return start;
	}
	std::string ignore;
	std::string loc_start;
	std::string model_start;
	std::string loc_end;
	file >> ignore >> ignore >> loc_start;
	file >> ignore >> ignore >> loc_end;
	file >> ignore >> ignore >> model_start;
	file.close();
	start.push_back(std::stoi(loc_start));
	start.push_back(std::stoi(loc_end));
	start.push_back(std::stoi(model_start));
	return start;
}

void DepthFrameWrite(std::string action, int model, int loc_num, int frame_num) {
	std::string filename = "LiDAR GTA V/Depth_Samples/" + action + "/loc_" + std::to_string(loc_num) + "_" + std::to_string(model) + "_frame_" + std::to_string(frame_num) + ".raw";
	auto f = fopen(filename.c_str(), "wb");
	if (f == NULL) {
		MsgLog("Failed to create depth frame file.");
		return;
	}
	GameplayFreeze(true);
	void* buf;
	int size = export_get_depth_buffer(&buf);
	fwrite(buf, 1, size, f);
	fclose(f);
	GameplayFreeze(false);
}

void ScriptMain()
{
	srand(GetTickCount());
	while (true)
	{
		if (IsKeyJustUp(VK_F6)) {
			std::string loc_file = "LiDAR GTA V/locations.txt";
			std::string actions_file = "LiDAR GTA V/actions.json";
			std::string start_file = "LiDAR GTA V/start.cfg";

			std::vector<Vector3> locs;
			ReadLocations(loc_file, locs);
			std::vector<Action> actions;
			ReadActions(actions_file, actions);
			std::vector<int> starts = StartingPoints(start_file);

			if (locs.empty() || actions.empty() || starts.empty() || starts.size() < 3) {
				notificationOnLeft("Unable to open needed files.");
			}
			else {
				notificationOnLeft("Starting script...");
				for (int m = starts[2]; m < sizeof(model_hashes) / sizeof(Hash); m++) {
					Hash model = model_hashes[m];
					ChangePlayerModel(model);
					ReverseCamera();
					for (int l = starts[0]; l < starts[1]; l++) {
						Entity player = PLAYER::PLAYER_PED_ID();
						Vector3 coords = locs[l];
						TeleportToCoords(player, coords);
						WAIT(500);
						std::string msg = "Model " + std::to_string((int)model) + " at loc " + std::to_string(l);
						MsgLog(msg);
						for (int a = 0; a < actions.size(); a++) {
							CAM::POINT_CAM_AT_ENTITY(CAM::GET_RENDERING_CAM(), PLAYER::PLAYER_PED_ID(), 0, 0, 0, true);
							int timer = 0;
							StartPedScenario(PLAYER::PLAYER_PED_ID(), actions[a].name);
							while (timer < 15000) {
								DepthFrameWrite(actions[a].name, (int)model, l, timer / 1000);
								timer += 1000;
								WAIT(1000);
							}
							EndPedScenario(PLAYER::PLAYER_PED_ID());
						}
					}
				}
			}
		}
		if (IsKeyJustUp(VK_F7)) {
			// TODO: Record Location again
			std::string loc_file = "LiDAR GTA V/locations.txt";
			RecordLocation(loc_file);
			notificationOnLeft("Recorded location.");
		} 
		WAIT(0);
	}
}
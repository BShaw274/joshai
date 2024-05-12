#include "lightingManager.hpp"
#include <thread>
#include "httplib.h"
#include "cmdparser.hpp"  
#include <Windows.h>
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace httplib;
using namespace nlohmann;


using namespace lightManager;


static void collectData(lighting* lights) {

	string fullData;
	string lightsCommand = "/lights/";
	string id;
	json data;

	list<string> ids;

	httplib::Client cli("localhost", 8080);

	//initial collection of data for the ids
	if (auto res = cli.Get("/lights")) {
		if (res->status == StatusCode::OK_200) {
			data = json::parse(res->body);
		}
	}
	else {
		auto err = res.error();
		std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
	}

	for (int i = 0; i < data.size(); i++) {
		ids.push_back(data[i]["id"]);
	}

	ids.sort();

	int numLights = data.size();

	//this is where all of the incoming data will be stored
	json jsonData;


	//internal storage for the actual data that contains all fields of the light
	for (int i = 0; i < numLights; i++) {
		lightsCommand = "/lights/";
		id = ids.front();
		ids.pop_front();
		//cout << " ids: " << id << endl;
		lightsCommand.append(id);

		if (auto res = cli.Get(lightsCommand)) {
			if (res->status == StatusCode::OK_200) {
				jsonData[i] = json::parse(res->body);
			}
		}
		else {
			auto err = res.error();
			std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
		}

	}

	//checking to see if this is the initial set up
	if (lights->ifData())
	{
		lights->updateLights(jsonData);
	}
	else
	{
		lights->addLights(jsonData);
		cout << jsonData.dump(2) << endl;
	}


}

int main(int argc, char** argv)
{

	string userin = "";
	httplib::Client cli("localhost", 8080);
	string dataIn;

	//testing the server
	if (auto res = cli.Get("/lights")) {
		if (res->status == StatusCode::OK_200) {
			//std:: cout << res->body << std::endl;
			//dataIn = res->body;
		}
	}
	else {
		auto err = res.error();
		std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
	}

	//initializing the light object
	lighting* lights = new lighting();

	while (userin != "q") {

		//this is to ping the server on a 5 sec interval to check for updates.
		auto now = std::chrono::steady_clock::now;
		using namespace std::chrono_literals;
		auto wait = 5s;
		auto start = now();
		while ((now() - start) < wait)
		{
			//do nothing and wait for the allocated time
		};

		collectData(lights);

	}

	return 0;

}



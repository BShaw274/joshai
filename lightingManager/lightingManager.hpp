using namespace std;
#include <iostream>
#include "json.hpp"

using namespace nlohmann;

namespace lightManager {


	class lighting
	{
	public:
		/*
		json format
		1: id
		2: name
		3. room
		4. onOff
		5. brightness
		*/

		lighting() {};
		virtual ~lighting() {};

		void addLights(json data);
		void changeBrightness(string id, int brightness);
		void toggleStatus(string id, bool onOff);
		void updateLights(json data);
		string compareIds(json newData, json data);
		bool ifData();


	private:
		list<string> myIds = {};
		json myData;
		int myNumLights;
	};
}


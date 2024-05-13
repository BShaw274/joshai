#include "lightingManager.hpp"
#include <vector>
#include <sstream>

using namespace nlohmann;


namespace lightManager {


	//the initial adding of the data
	void lighting::addLights(json data) {
		myData = data;
		myNumLights += myData.size();
	}

	/*
	Function to print a change in brightness on a light.

	Does not actually update stored data since the incoming data is always set to be the interally stored lights.
	*/
	void lighting::changeBrightness(string id, int newBrightness) {
		if (newBrightness > 255 || newBrightness < 0) {
			cout << "BRIGHTNESS MUST BE IN RANGE OF 0-255" << endl;
		}
		else {
			for (int i = 0; i < myNumLights; i++) {
				if (myData[i]["id"] == id) {
					//checking the light is actually turned on
					if (myData[i]["on"] == true) {
						//number conversion from 255 to 100
						double brightness = (newBrightness / 255.0) * 100;
						cout << "{\nid: " << myData[i]["id"] << "\nbrightness: " << brightness << "%\n}\n";
					}
					else {
						double brightness = (newBrightness / 255.0) * 100;
						cout << "Your light " << myData[i]["id"] << " needs to be turned on to see changes!" << endl;
						cout << "{\nid: " << myData[i]["id"] << "\nbrightness: " << brightness << "%\n}\n";
					}
					return;
				}
			}
			cout << "ERROR: LIGHT NOT FOUND!" << endl;
		}
		

	}
	/*
	Function to print a change in status on a light.

	Does not actually update stored data since the incoming data is always set to be the interally stored lights.
	*/
	void lighting::toggleStatus(string id, bool onOff) {

		string out;
		if (onOff) {
			out = "True";
		}
		else {
			out = "False";
		}
		for (int i = 0; i < myNumLights; i++) {
			if (myData[i]["id"] == id) {
				//turning the light on
				if (onOff) {
					//checking if the light is already on
					if (myData[i]["on"] == onOff) {
						cout << "WARNING: Light is already on!" << endl;
					}
					else {
						cout << "{\nid: " << myData[i]["id"] << endl << "on: " << out << "\n}\n";
					}
				}
				//turning light off
				else
				{
					//checking if the light is already off
					if (myData[i]["on"] == onOff) {
						cout << "WARNING: Light is already off!" << endl;
					}
					else {
						cout << "{\nid: " << myData[i]["id"] << endl << "on: " << out << "\n}\n";
					}
				}
				return;
			}
		}
		cout << "ERROR: LIGHT NOT FOUND!" << endl;
		

	}
	
	/*
	Compares Ids to find the unique id. 
	*/
	string lighting::compareIds(json newData, json data) {
		list<string> newIds;
		list<string> ids;

		//adds both sets of ids into their respective list
		for (int i = 0; i < (newData.size()); i++)
		{
			newIds.push_back(newData[i]["id"]);
		}
		for (int i = 0; i < (data.size()); i++)
		{
			ids.push_back(data[i]["id"]);
		}

		//finds which list has more ids and removes the ids that appear in both lists
		if (newData.size() > data.size()) {
			for (int i = 0; i < (data.size()); i++) {
				newIds.remove(data[i]["id"]);
			}
			return newIds.front();
		}
		else {
			for (int i = 0; i < (newData.size()); i++) {
				ids.remove(newData[i]["id"]);
			}
			return ids.front();
		}
	}

	/*
	3 Important steps occur here.
	The data is checked to see if the size has changed to determine if a light was added or removed.
	If it is the same incoming data size, then the brightness and on status are both checked and changes are printed.
	If the incoming data is smaller, then a light has been removed and that id is found and printed.
	If the incoming data is larger, then a light as been added and that id is found and printed.

	At the end of all of these steps the new data is turned into the internal data to ensure everything is up to date.
	
	*/

	void lighting::updateLights(json newData) {

		//compare length to see if an element is added or removed


		//this means just an element was updated
		if(newData.size() == myData.size()) {
			for (int i = 0; i < newData.size(); i++) {
				if (newData[i]["brightness"] != myData[i]["brightness"]) {
					changeBrightness(myData[i]["id"], newData[i]["brightness"]);
				}
				if (newData[i]["on"] != myData[i]["on"]) {
					toggleStatus(myData[i]["id"], newData[i]["on"]);
				}
				if (newData[i]["name"] != myData[i]["name"]) {
					cout << "{\nid: " << myData[i]["id"] << endl << "name: " << newData[i]["name"] << "\n}\n";
				}
				if (newData[i]["room"] != myData[i]["room"]) {
					cout << "{\nid: " << myData[i]["id"] << endl << "room: " << newData[i]["room"] << "\n}\n";
				}
			}
			myData = newData;
			return;
		}
		
		//compare ids to find the outlier
		string importantId = compareIds(newData, myData);
		//a light has been added
		if (newData.size() > myData.size())
		{
			//need to print which light was added
			for (int i = 0; i < myNumLights; i++) {
				if (newData[i]["id"] == importantId) {
					cout << "Light added!" << endl << newData[i] << endl;

				}
			}
			myData = newData;
			myNumLights = newData.size();
		}
		//a light has been removed
		else 
		{
			//need to findout which one was removed
			for (int i = 0; i < myNumLights; i++) {
				if (myData[i]["id"] == importantId) {
					cout << myData[i]["name"] << " (" << myData[i]["id"] << ") has been removed\n";

				}
			}
			myData = newData;
			myNumLights = myData.size();
		}



	}
	/*
	A preliminary check to see if there is data stored in the lights object
	*/
	bool lighting::ifData() {

		if (myData.size() > 0)
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
}



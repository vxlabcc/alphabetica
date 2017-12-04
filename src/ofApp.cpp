#include "ofApp.h"

const string ofApp::alphabet = "abcdefghijklmnopqrstuvwxyz";
const int ROWS = 3;
const int COLUMNS = 9;
const char ofApp::KEYS[ROWS][COLUMNS] = {{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'},
                                         {'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'},
                                         {'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_'}};


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);

	// Init box
	box2d.init();
	box2d.setGravity(0, 3); // 10 could be a best but...
	box2d.createBounds();
	box2d.setFPS(60.0);

	// Hack: invert french(FR) keyboard layout to United States(US)
	localeTable['q'] = 'a';
	localeTable['a'] = 'q';
	localeTable['z'] = 'w';
	localeTable['w'] = 'z';
	localeTable[';'] = 'm';
	localeTable['m'] = ';';
}

//--------------------------------------------------------------
void ofApp::update(){
	box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackgroundGradient(ofColor(164, 232, 245), ofColor(190, 239, 244), OF_GRADIENT_LINEAR);
  for (unsigned int i = 0; i < letters.size(); i++) {
    if (letters.at(i).get()->timeToDie()) {
      letters.erase(letters.begin() + i);
    } else {
      letters.at(i).get()->draw();
    }
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (localeTable.find(key) != localeTable.end())
		key = localeTable[key];
	if (key != previousKey) { // Avoid repeat key
		if (alphabet.find((char)key) != string::npos) {
			int keyX = 0; // By default
			for (int row = 0; row < ROWS; row++) {
				for (int column = 0; column < COLUMNS; column++) {
					if (ofToString(KEYS[row][column]) == ofToUpper(ofToString((char)key))) {
						keyX = (ofGetWidth() / COLUMNS * (column + 1)) - ((ofGetWidth() / COLUMNS) / 2);
					}
				}
			}
			shared_ptr<Letter> letter = shared_ptr<Letter>(new Letter((char)key));
			letter.get()->setupLetter();
			letter.get()->setPhysics(1.0, 0.6, 0.9);
			letter.get()->setup(box2d.getWorld(), keyX, ofGetHeight(),
			                    letter.get()->getLetterWidth(), letter.get()->getLetterHeight());
			letter.get()->setVelocity(ofRandom(-20, 20), -30);
			letters.push_back(letter);
		}
	}
	previousKey = key;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	previousKey = 9999;
}

void ofApp::drawDebug(const string & text) {
	ofSetColor(0);
	ofFill();
	ofDrawBitmapString(text, 100,100);
}

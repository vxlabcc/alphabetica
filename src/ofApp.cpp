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

	setupSentence("Ecrivez_votre_nom", message);
}

//--------------------------------------------------------------
void ofApp::update(){
	box2d.update();

	if (!letters.empty() && !sentenceArray.empty() &&
			letters.back().get()->getElapsedTIme() >= 6000 &&
		  letters.back().get()->getElapsedTIme() <= 7000) {
		setSentence(sentenceArray);
		sentenceArray = "";
		setupSentence(getSentence(), sentence);
	}
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
	
	for (unsigned int i = 0; i < sentence.size(); i++) {
		if (sentence.at(i).get()->timeToDie()) {
			sentence.erase(sentence.begin() + i);
		} else {
			sentence.at(i).get()->draw();
		}
	}

	if (letters.empty() && sentence.empty()) {
		for (unsigned int i = 0; i < message.size(); i++) {
			message.at(i).get()->draw();
		}
	}
	drawDebug(getSentence());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (localeTable.find(key) != localeTable.end())
		key = localeTable[key];
	if (key != previousKey) { // Avoid repeat key
		if (alphabet.find((char)key) != string::npos) {
			int keyX = 0;
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

			composeSentence(ofToString((char)key));
		}
	}
	previousKey = key;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	previousKey = 9999;
}

void ofApp::composeSentence(const string & letter) {
	if (letters.size() >= 2 && letters.at(letters.size()-2).get()->getElapsedTIme() >= 3000) {
		sentenceArray.append("_");
		sentenceArray.append(letter);
	} else {
		sentenceArray.append(letter);
	}
}

void ofApp::setupSentence(const string & text, vector <shared_ptr<Letter> > & word) {
	word.clear();
	cursorX = 0;
	for (int i = 0; i < text.length(); i++) {
		shared_ptr<Letter> letter = shared_ptr<Letter>(new Letter(text.at(i)));
		letter.get()->setupLetter(); // Important setup letter first
		int marginX = (ofGetWindowWidth() - (letter.get()->getLetterHeight() * text.length())) / 2;
		int letterCenter = letter.get()->getLetterWidth() / 2;
		// letter.get()->setPhysics(0.3, 0.2, 0.3);
		letter.get()->setup(box2d.getWorld(), marginX + cursorX + letterCenter, ofGetHeight() / 2,
												letter.get()->getLetterWidth(), letter.get()->getLetterHeight());
		// letter.get()->setVelocity(ofRandom(-3, 3), -10);
		cursorX += letter.get()->getLetterWidth() + 5;
		word.push_back(letter);
	}
}

void ofApp::drawDebug(const string & text) {
	ofSetColor(0);
	ofFill();
	ofDrawBitmapString(text, 100,100);
}


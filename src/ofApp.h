#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

class Letter : public ofxBox2dRect {

public:
	// Constructor
	Letter(const char &keyChar) { _keyChar = keyChar; }

	void setupLetter() {
		// Load image based of the choosen letter
		image.load("letters-" + ofToString(ofGetWindowHeight()) + "/" + ofToUpper(ofToString(_keyChar)) + ".png");
		// song.load("songs/" + ofToUpper(ofToString(_keyChar)) + ".mp3");

		// song.setVolume(1.0);
		// song.setLoop(false);
		// song.play();

		startTime = ofGetElapsedTimeMillis();
		_timeout = false;
	}

	void draw() {
		ofPushMatrix();
		ofTranslate(getPosition());
		ofRotateZ(getRotation());
		ofSetColor(0, 0);
		ofFill();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofDrawRectRounded( 0, 0, getWidth(), getHeight(), 10);

		ofPushMatrix();
		ofSetColor(255);
		ofFill();
		image.draw(0, 0);
		ofPopMatrix();

		ofPopMatrix();

		_lifetime = ofGetElapsedTimeMillis() - startTime;

		_timeout = _lifetime >= MAX_LIFE_TIME;
	}
	// Return the width of the image
	float getLetterWidth() const { return image.getWidth(); }
	// Return the height of the image
	float getLetterHeight() const { return image.getHeight(); }
	// Return current age of the letter
	float getElapsedTIme() const { return _lifetime; }
	char getLetterChar() const { return _keyChar; } // TODO: delete this line in case no need it

	bool timeToDie() const { return _timeout; }

private:
	char _keyChar;
	ofImage image;
	ofSoundPlayer song;

	float startTime;
	bool _timeout;
	float _lifetime;
	
	const float MAX_LIFE_TIME = 30000; // 30 seconds
	const float ESCAPE_DELAY = 3000; // 3 seconds
};


class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);

	void composeSentence(const string &letter);
	void setupSentence(const string &text, vector <shared_ptr<Letter> > &word);
	void drawDebug(const string &text);

	// Getters
	string getSentence() const { return _sentence; }
	// Setters
	void setSentence(const string &text) { _sentence = text; }	
	// Static variables
	static const string alphabet;
	static const char KEYS[][9];

private:
	ofxBox2d box2d;
	vector <shared_ptr<Letter> > letters;
	vector <shared_ptr<Letter> > sentence;
	vector <shared_ptr<Letter> > message;
	string _sentence = "";
	string sentenceArray;
	int cursorX;

	int previousKey;
	map<int, int> localeTable;
};

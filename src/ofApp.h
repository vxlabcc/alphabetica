#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

class Letter : public ofxBox2dRect {

public:
	// Constructor
	Letter(const char &keyChar) { _keyChar = keyChar; }

	void setupLetter() {
		// Load image based of the choosen letter
		image.load("letters/" + ofToUpper(ofToString(_keyChar)) + ".png");
		song.load("songs/" + ofToUpper(ofToString(_keyChar)) + ".mp3");

		song.setVolume(1.0);
		song.setLoop(false);
		song.play();

		startTime = ofGetElapsedTimeMillis();
		endTime = false;
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

		float timer = ofGetElapsedTimeMillis() - startTime;

		if (timer >= LIFETIME) { endTime = true; }
	}
	// Return the width of the image
	float getLetterWidth() const { return image.getWidth(); }
	// Return the height of the image
	float getLetterHeight() const { return image.getHeight(); }

	bool timeToDie() const { return endTime; }

	static const float LIFETIME;

private:
	char _keyChar;
	ofImage image;
	ofSoundPlayer song;

	float startTime;
	bool endTime;
};


class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);

	static const string alphabet;
	static const char KEYS[][9];

private:
	ofxBox2d box2d;
	vector <shared_ptr<Letter> > letters;

	int previousKey;
	map<int, int> localeTable;
};

#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ImGuiUtils.h"
#include "ofxVideoRecorder.h"

//FileSaver
#include "ofxJSONElement.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR))) 

#define OF_KEY_SPACE 32
#define NScreen    11

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void loadScreensImages();
		void loadGuiImages();
		void screenStart();
		void screenName();
		void screenPlay();
		void screenPhotoVideo();
		void screenMediaName();
		void screenEndOrRestart();

		ofxImGui::Gui gui;
	    ofImage imageButtonLetsWorkSource, imageButtonVideoSource, imageButtonImageSource, imageButtonAddOneSource, imageButtonLetsStartSource, imageButtonExitSource, imageButtonRecordSource, imageButtonReStartSource, imageButtonStopRecordSource, imageButtonPauseRecordSource, imageButtonNextIconSource, imageButtonFlagRecordSource;
	    GLuint imageButtonLetsWorkID, imageButtonVideoSourceID, imageButtonImageSourceID, imageButtonAddOneSourceID, imageButtonLetsStartSourceID, imageButtonExitSourceID, imageButtonRecordSourceID, imageButtonReStartSourceID, imageButtonStopRecordSourceID, imageButtonPauseRecordSourceID, imageButtonNextIconSourceID, imageButtonFlagRecordSourceID;
	
		int screen = 0;
		ofImage imgScreen[NScreen];
		int change = 0;
		char projectName[128] = "";
		string projectNameString;
		char projectDescription[128] = "";
		char videoName[128] = "";
		string videoNameString;
		char videoDescription[128] = "";
		char videoCatergory[128] = ""; 


		int mediaFormat = 0;   // 0 = video, 1 = Photo,

		ofTrueTypeFont	verdana;
		int changeScreen = 0;

		// Video Recording
		void setupVideoRecorder();
		void screenVideoRecorder();
		void audioIn(float * input, int bufferSize, int nChannels);
	
		//Subtitles Functions
		string formatSubtitleTimeStamp(int hour, int minute, int second, int milisecond);
		void startSaveSubtitles();
		void addOneSubtitle(int idVideo, string description);
		void endSaveSubtitlesFile();
	    
	    ofVideoGrabber      vidGrabber;
	    ofxVideoRecorder    vidRecorder;
	    ofSoundStream       soundStream;
	    bool bRecording = false;
	    int sampleRate;
	    int channels;
	    string fileName;
	    string fileExt;
		
		
	    ofFbo recordFbo;
	    ofPixels recordPixels;
		
		//Save some Info (TAG) in a json
		ofxJSONElement resultJson;
		ofFile mySubtitlesTextFile;
		int counterVideos = 0;
		int counterSubtitles = 0;
		long initTimeVideo = 0;
		string timeStamp_init_subtitle = "00:00:00,000";
		string timeStamp_end_subtitle = "00:00:00,000";
		int secondsSubtitleVisible = 2;

		int flag = 0;

		vector<string> categories{ "Intro", "BOM", "Programming", "Electronics", "Devices", "Assembly" };
		int categoryID = 0;


		// Serial data
		bool		bSendSerialMessage;			// a flag for sending serial
		char		bytesRead[3];				// data from serial, we will be trying to read 3
		char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
		int			nBytesRead;					// how much did we read?
		int			nTimesRead;					// how many times did we read?
		float		readTime;					// when did we last read?				
		
		ofSerial	serial;
		void readSerialBus(int idUsb);
		void flags();
		int flagRect = 0;


		void recording();
};

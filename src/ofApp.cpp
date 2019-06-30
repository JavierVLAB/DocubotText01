#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	gui.setup();

	for (int i = 0; i < NScreen; i++)
	{
	
		if(i<10){
			imgScreen[i].load("SCREENS/SCREEN_0" + ofToString(i) + ".png");
		} else {
			imgScreen[i].load("SCREENS/SCREEN_" + ofToString(i) + ".png");
		}
	}


	ofTrueTypeFont::setGlobalDpi(72);

	verdana.load("verdana.ttf", 50, true, true);
	verdana.setLineHeight(18.0f);
	verdana.setLetterSpacing(1.037);

	/// Video Recorder

	sampleRate = 44100;
    channels = 2;

    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(640,400);
		#ifdef TARGET_WIN32
			vidRecorder.setFfmpegLocation("C:\\ffmpeg\\bin\\"); // use this is you have ffmpeg installed in your data folder
		#endif
		    fileName = "testMovie";
		    fileExt = ".mp4"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats

    // override the default codecs if you like
    // run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
    vidRecorder.setVideoCodec("mpeg4"); 
    vidRecorder.setVideoBitrate("800k");
    vidRecorder.setAudioCodec("mp3");
    vidRecorder.setAudioBitrate("192k");

//    soundStream.listDevices();
//    soundStream.setDeviceID(11);
    soundStream.setup(this, 0, channels, sampleRate, 256, 4);

    //ofSetWindowShape(vidGrabber.getWidth(), vidGrabber.getHeight()	);
    bRecording = false;
    ofEnableAlphaBlending();



    // Serial Setup

	
	bSendSerialMessage = false;
	ofBackground(255);	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	int baud = 115200;
	//serial.setup("/dev/cu.usbmodemFA142", baud); 
	serial.setup("/dev/cu.usbmodem412", baud); 
	
	nTimesRead = 0;
	nBytesRead = 0;
	readTime = 0;
	memset(bytesReadString, 0, 4);




}

//--------------------------------------------------------------
void ofApp::update(){

	if(screen == 5){
		vidGrabber.update();
    if(vidGrabber.isFrameNew() && bRecording){
        bool success = vidRecorder.addFrame(vidGrabber.getPixelsRef());
        if (!success) {
            ofLogWarning("This frame was not added!");
        }
    }
    
    // Check if the video recorder encountered any error while writing video frame or audio smaples.
    if (vidRecorder.hasVideoError()) {
        ofLogWarning("The video recorder failed to write some frames!");
    }
    
    if (vidRecorder.hasAudioError()) {
        ofLogWarning("The video recorder failed to write some audio samples!");
    }
  }

  readSerialBus(1);

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0,0,0);

	gui.begin();
	switch( screen )  
  	{	  
    case 0:  

      imgScreen[0].draw(0,0);
    	if(change){screen++;change=0;}
      break; 

    case 1:  
    	imgScreen[6].draw(0,0);
  		screenName();    
      break;

    case 2:  
      imgScreen[5].draw(0,0);
      screenPhotoVideo();
      break;  

    case 3:  
 		imgScreen[6].draw(0,0);
  		screenMediaName();    
      break;

    case 4:  
      imgScreen[3].draw(0,0);
      //screenPlay();
      if(change){screen++;change=0;}
      break; 

    case 5:
    	imgScreen[7].draw(0,0);
    	screenVideoRecorder();
    	if(change){screen++;change=0;}
    	break;

    case 6:
    	imgScreen[10].draw(0,0);
    	if(change){screen=0;change=0;}
    	break;

	}  


	change = 0;
	gui.end();

}

//--------------------------------------
void ofApp::screenVideoRecorder(){

    ofSetColor(255, 255, 255);
    vidGrabber.draw(ofGetWidth()/2 - 320,ofGetHeight()/2-200);

    stringstream ss;
    ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
    << "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
    << "FPS: " << ofGetFrameRate() << endl
    << (bRecording?"pause":"start") << " recording: r" << endl
    << (bRecording?"close current video file: c":"") << endl;

    ofSetColor(0,0,0,100);
    ofRect(0, 0, 260, 75);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ss.str(),15,15);

    if(bRecording){
	    ofSetColor(255, 0, 0);
	    ofCircle(ofGetWidth() - 20, 20, 5);
    }


    if(flagRect){
    	flagRect = 0;
    	ofBackground(0,255,255);
    	
    }

}

//---------------------------------------
void ofApp::screenName(){
	

	//imgScreen[1].draw(0,0); 
	ImGui::Text("Project");
	ImGui::InputText("Name", projectName, IM_ARRAYSIZE(projectName));
	ImGui::InputText("Description", projectDescription, IM_ARRAYSIZE(projectDescription));

	if (ImGui::Button("Next")) { 
		screen++;

		//projectNameString = std::string myTitle(projectName);
		//ofStringReplace(projectNameString," ","_");
		fileName = "Projects/";
		fileName += projectName; 

	}


}

//---------------------------------------
void ofApp::screenMediaName(){
	



	ImGui::Text("Video Description");
	ImGui::InputText("Name", videoName, IM_ARRAYSIZE(videoName));
	ImGui::InputText("Description", videoDescription, IM_ARRAYSIZE(videoDescription));
	if(ComboCinder("Categories", &categoryID, categories, categories.size())){
		cout << categoryID << endl;
	}

	if (ImGui::Button("Next")) { 
		screen++; 
		//ofStringReplace(videoName," ","_");
		fileName += "/" + categories[categoryID] + "/" + videoName;
	}


}

//---------------------------------------
void ofApp::screenPlay(){
	

	//ofSetColor(100);
	//verdana.drawString("PLAY", 400, 300);


}

//---------------------------------------
void ofApp::screenPhotoVideo(){
	

	//imgScreen[1].draw(0,0); 
	ImGui::Text("Choose media format");

  	ImGui::RadioButton("Video", &mediaFormat, 0); ImGui::SameLine();
  	ImGui::RadioButton("Photo", &mediaFormat, 1); 
	

	if (ImGui::Button("Next")) {
		screen++;
	}


}



//--------------------------------------------------------------
void ofApp::readSerialBus(int idUsb){
	// (2) read
	// now we try to read 3 bytes
	// since we might not get them all the time 3 - but sometimes 0, 6, or something else,
	// we will try to read three bytes, as much as we can
	// otherwise, we may have a "lag" if we don't read fast enough
	// or just read three every time. now, we will be sure to
	// read as much as we can in groups of three...
	
	nTimesRead = 0;
	nBytesRead = 0;
	int nRead  = 0;  // a temp variable to keep count per read
	
	unsigned char bytesReturned[3];
	
	memset(bytesReadString, 0, 4);
	memset(bytesReturned, 0, 3);
	
	while( (nRead = serial.readBytes( bytesReturned, 3)) > 0){
		nTimesRead++;
		nBytesRead = nRead;
	
	};
	
	memcpy(bytesReadString, bytesReturned, 3);
	
	
	if(bRecording){
		//flags();
	} else {
		//change = 1;
	}
	if(nTimesRead >0) cout << "Received something USB microbit = " << bytesReadString << endl;




}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}





//----------------   Video Recorder

void ofApp::audioIn(float *input, int bufferSize, int nChannels){
    if(bRecording)
        vidRecorder.addAudioSamples(input, bufferSize, nChannels);
}


//-------------------------------------
void ofApp::startSaveSubtitles(){
	cout << " ::::::::: bRecording && !vidRecorder.isInitialized() :::::::::" << endl;
	
	if(true){resultJson.clear();
}
	if(true){
		mySubtitlesTextFile.open(fileName+ofGetTimestampString() +".srt",ofFile::WriteOnly);
	}
	
	
	int actualVideoFrame = 0;
	counterVideos++;
	counterSubtitles = 0;
	initTimeVideo = ofGetElapsedTimeMillis();

}

//-----------------------------------------
string ofApp::formatSubtitleTimeStamp(int _hour, int _min, int _sec, int _milis){
	
	int n_zero = 2;
	string old_hour = ofToString(_hour);
	std::string new_hour = std::string(n_zero - old_hour.length(), '0') + old_hour;
	
	string old_minute = ofToString(_min);
	std::string new_minute = std::string(n_zero - old_minute.length(), '0') + old_minute;
	
	string old_second = ofToString(_sec);
	std::string new_second = std::string(n_zero - old_second.length(), '0') + old_second;
	
	n_zero = 3;
	string old_milisecond = ofToString(_milis);	
	std::string new_milisecond = std::string(n_zero - old_milisecond.length(), '0') + old_milisecond;
	
	string resultTimeStampSub = new_hour+":"+new_minute+":"+new_second+","+new_milisecond;
	
	return resultTimeStampSub;
}

//--------------------------------------------------------------
void ofApp::addOneSubtitle(int idVideo, string description) {
	if(true){
		cout << " ::::::::: add 1 FLAG !:::::::::" << endl;
		resultJson[ofToString(idVideo, 2)]["flag"]["videoFrame"] = ofToString(ofToString(counterVideos, 2), 2);
		resultJson[ofToString(idVideo, 2)]["flag"]["descripcion"] = description;
	}

	if(true){
		//Same as json method in txt
		//mySubtitlesTextFile << ofToString(idVideo, 0)+"\n";
		//mySubtitlesTextFile << description + "\n";
		//mySubtitlesTextFile << "\n";
		//If it's necessary do this one the memory, use ofBuffer then safve at the end. https://openframeworks.cc/documentation/utils/ofFile/
		
		//Save str format
		//id counter[1..X]
		counterSubtitles++;
		mySubtitlesTextFile << ofToString(counterSubtitles, 0)+"\n";
		//init time(hh:mm:ss,mss) --> end time(hh:mm:ss,mss)
		//int hourVid;
		//int minVid;
		//int secVid;
		int millisVid;
		long actualTimeVid = ofGetElapsedTimeMillis() - initTimeVideo;
		cout << "actualTimeVid = " << actualTimeVid << endl;
		//https://stackoverflow.com/questions/50727304/convert-milliseconds-to-hoursminutessecondsmilliseconds-in-c
		long milli = actualTimeVid;
		//3600000 milliseconds in an hour
		long hourVid = milli / 3600000;
		milli = milli - 3600000 * hourVid;
		//60000 milliseconds in a minute
		long minVid = milli / 60000;
		milli = milli - 60000 * minVid;
		//1000 milliseconds in a second
		long secVid = milli / 1000;
		milli = milli - 1000 * secVid;
		millisVid = milli;
		
		cout << hourVid << " hours and " << minVid << " minutes and " << secVid << " seconds and " << milli << " milliseconds." << endl;
		
		//timeStamp_init_subtitle = formatSubtitleTimeStamp(0,1,15,300); //example
		timeStamp_init_subtitle = formatSubtitleTimeStamp(hourVid,minVid,secVid,millisVid); //example
		timeStamp_end_subtitle = formatSubtitleTimeStamp(hourVid,minVid,secVid+secondsSubtitleVisible,millisVid);
		
		mySubtitlesTextFile << timeStamp_init_subtitle+" "+"-->"+" "+timeStamp_end_subtitle+"\n";
		//Text with severa end lines ("\n") if it's necesary
		mySubtitlesTextFile << description + "\n";
		mySubtitlesTextFile << "\n";
		
		
	}
	
}


//-------------------------------------
void ofApp::endSaveSubtitlesFile(){
	if(true){
		resultJson.save(fileName+ofGetTimestampString() + ".json", true);
	}
	
	if(true){
		mySubtitlesTextFile.close();
	}
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	if(key == OF_KEY_SPACE){
		change = 1;
	}



    if(key=='r'){
        bRecording = !bRecording;
        if(bRecording && !vidRecorder.isInitialized()) {
            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels);
//          vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, vidGrabber.getWidth(), vidGrabber.getHeight(), 30);
//            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, 0,0,0, sampleRate, channels); // no video
//          vidRecorder.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels, "-vcodec mpeg4 -b 1600k -acodec mp2 -ab 128k -f mpegts udp://localhost:1234"); // for custom ffmpeg output string (streaming, etc)
            
            // Start recording
            vidRecorder.start();
			
			//Save File subtitles
			startSaveSubtitles();
        }
        else if(!bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(true);
        }
        else if(bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(false);
        }
    }

    if(key=='c'){
        bRecording = false;
        flag = 0;
        vidRecorder.close();
		
		//subtitles
		endSaveSubtitlesFile();
    }
	
	if (key == 'f') { // adding one subtitle
		flags();
		
	}
}

void ofApp::flags(){
	flag += 1;

	int actualVideoFrame = vidRecorder.getNumVideoFramesRecorded();
	addOneSubtitle(actualVideoFrame, "flag" + ofToString(flag));
	flagRect = 1;
}





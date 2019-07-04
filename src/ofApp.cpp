#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	gui.setup();
	
	loadGuiImages();
	

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
	
	//select my external or internal camera
	vector<ofVideoDevice > namesDevices = vidGrabber.listDevices();
	if(namesDevices.size() > 1)vidGrabber.setDeviceID(1);
	else if (namesDevices.size() > 0)vidGrabber.setDeviceID(0);
	else cout << "NO CAMERAS detected!" << endl;
	
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

	//Uncomment for microbit Radio receiver
	if(false)readSerialBus(1);

}

//-----------------------------------------
void ofApp::loadGuiImages(){
	
	//making a image button. Follow ImGui OF example
	imageButtonLetsWorkSource.load("img/workPlay.png");
	imageButtonLetsWorkID = gui.loadImage(imageButtonLetsWorkSource);
	
	imageButtonVideoSource.load("img/fotoPlay.png");
	imageButtonVideoSourceID = gui.loadImage(imageButtonVideoSource);
	
	imageButtonImageSource.load("img/videoPlay.png");
	imageButtonImageSourceID = gui.loadImage(imageButtonImageSource);
	
	imageButtonRecordSource.load("img/record.png");
	imageButtonRecordSourceID = gui.loadImage(imageButtonRecordSource);

	imageButtonExitSource.load("img/exit.png");
	imageButtonExitSourceID = gui.loadImage(imageButtonExitSource);
	
	imageButtonLetsStartSource.load("img/letsStart.png");
	imageButtonLetsStartSourceID = gui.loadImage(imageButtonLetsStartSource);
	
	imageButtonAddOneSource.load("img/addOne.png");
	imageButtonAddOneSourceID = gui.loadImage(imageButtonAddOneSource);
	
	imageButtonReStartSource.load("img/restart.png");
	imageButtonReStartSourceID = gui.loadImage(imageButtonReStartSource);

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0,0,0);

	gui.begin();
	switch( screen )  
  	{	  
    case 0:
		imgScreen[0].draw(0,0);
		screenStart();
			
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
      screenPlay();
      if(change){screen++;change=0;}
      break; 

    case 5:
			
			
    	imgScreen[7].draw(0,0);
    	screenVideoRecorder();
    	if(change){screen++;change=0;}
    	break;

    case 6:
			
		screenEndOrRestart();
			
    	imgScreen[10].draw(0,0);
    	if(change){screen=0;change=0;}
    	break;

	}  


	change = 0;
	gui.end();

}


//---------------------------------------
void ofApp::screenStart(){
	
	bool pressed = false;
	
	cout << "Screen " << screen << endl;
	if(true)ImGui::SetWindowPos(ImVec2(29,14));
	else cout << "gui pos" << ImGui::GetWindowPos().x << " & " << ImGui::GetWindowPos().y << endl;
	
	if(true)ImGui::SetWindowSize((ImVec2(389,237)));
	else cout << "gui size" << ImGui::GetWindowSize().x << " & " << ImGui::GetWindowSize().y << endl;
	
	pressed = ImGui::ImageButton((ImTextureID)(uintptr_t)imageButtonLetsWorkID, ImVec2(367,200));
	
	if(pressed)change = 1;
}

//--------------------------------------
void ofApp::screenVideoRecorder(){

	cout << "Screen " << screen << endl;
	if(false)ImGui::SetWindowPos(ImVec2(146,360));
	else cout << "gui pos" << ImGui::GetWindowPos().x << " & " << ImGui::GetWindowPos().y << endl;
	
	if(false)ImGui::SetWindowSize((ImVec2(485,109)));
	else cout << "gui size" << ImGui::GetWindowSize().x << " & " << ImGui::GetWindowSize().y << endl;
	
	
	//////////////////////////////
	
    ofSetColor(255, 255, 255);
    vidGrabber.draw(ofGetWidth()/2 - 320,ofGetHeight()/2-200);

    stringstream ss;
    ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
    << "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
    << "FPS: " << ofGetFrameRate() << endl
    << (bRecording?"pause":"start") << " recording: r" << endl
    << (bRecording?"close current video file: c":"") << endl;
	
	
	//TODO ADD IMAGES OR MODIF R to

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
	
	cout << "Screen " << screen << endl;
	if(true)ImGui::SetWindowPos(ImVec2(146,360));
	else cout << "gui pos" << ImGui::GetWindowPos().x << " & " << ImGui::GetWindowPos().y << endl;
	
	if(true)ImGui::SetWindowSize((ImVec2(485,109)));
	else cout << "gui size" << ImGui::GetWindowSize().x << " & " << ImGui::GetWindowSize().y << endl;
	

	//////////////
	
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
	
	cout << "Screen " << screen << endl;
	if(false)ImGui::SetWindowPos(ImVec2(146,360));
	else cout << "gui pos" << ImGui::GetWindowPos().x << " & " << ImGui::GetWindowPos().y << endl;
	
	if(false)ImGui::SetWindowSize((ImVec2(485,109)));
	else cout << "gui size" << ImGui::GetWindowSize().x << " & " << ImGui::GetWindowSize().y << endl;
	
	//////
	
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
	
	bool pressed = false;
	
	cout << "Screen " << screen << endl;
	if(true)ImGui::SetWindowPos(ImVec2(63,57));
	else cout << "gui pos" << ImGui::GetWindowPos().x << " & " << ImGui::GetWindowPos().y << endl;
	
	if(true)ImGui::SetWindowSize((ImVec2(388,237)));
	else cout << "gui size" << ImGui::GetWindowSize().x << " & " << ImGui::GetWindowSize().y << endl;
	
	pressed = ImGui::ImageButton((ImTextureID)(uintptr_t)imageButtonLetsWorkID, ImVec2(367,200));

	if(pressed)change = 1;
	
	//ofSetColor(100);
	//verdana.drawString("PLAY", 400, 300);


}

//---------------------------------------
void ofApp::screenPhotoVideo(){
	
	cout << "Screen " << screen << endl;
	if(true)ImGui::SetWindowPos(ImVec2(159,299));
	else cout << "gui 1 pos" << ImGui::GetWindowPos().x << " & " << ImGui::GetWindowPos().y << endl;
	
	if(true)ImGui::SetWindowSize((ImVec2(383,260)));
	else cout << "gui 1 size" << ImGui::GetWindowSize().x << " & " << ImGui::GetWindowSize().y << endl;


	////////
	if(false){
		//imgScreen[1].draw(0,0);
		ImGui::Text("Choose media format");

		ImGui::RadioButton("Video", &mediaFormat, 0); ImGui::SameLine();
		ImGui::RadioButton("Photo", &mediaFormat, 1);
		

		if (ImGui::Button("Next")) {
			screen++;
		}
	}else{
		bool pressedPhoto = false;
		bool pressedVideo = false;
		
		pressedPhoto = ImGui::ImageButton((ImTextureID)(uintptr_t)imageButtonVideoSourceID, ImVec2(367,200));
		
		
		//....
		// 2. Show another window, this time using an explicit ImGui::Begin and ImGui::End
		if (true)
		{
			//note: ofVec2f and ImVec2f are interchangeable
			ImGui::SetNextWindowSize(ofVec2f(200,100), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Selection");
			pressedVideo = ImGui::ImageButton((ImTextureID)(uintptr_t)imageButtonImageSourceID, ImVec2(367,200));
			if(true)ImGui::SetWindowPos(ImVec2(733,299));
			else cout << "gui 2 pos" << ImGui::GetWindowPos().x << " & " << ImGui::GetWindowPos().y << endl;
			
			if(true)ImGui::SetWindowSize((ImVec2(383,260)));
			else cout << "gui 2 size" << ImGui::GetWindowSize().x << " & " << ImGui::GetWindowSize().y << endl;
			
			ImGui::End();
		}
		
		/*

		*/
	
		if(pressedPhoto){
			mediaFormat = 0;
			screen++;
		}
		if(pressedVideo){
			mediaFormat = 1;
			screen++;
		}
	}


}

//--------------------------------------
void ofApp::screenEndOrRestart(){
	
	bool pressedRestart = false;
	bool pressedAddOne = false;
	bool pressedExit = false;
	
	cout << "Screen " << screen << endl;
	if(true)ImGui::SetWindowPos(ImVec2(187,219));
	else cout << "gui pos" << ImGui::GetWindowPos().x << " & " << ImGui::GetWindowPos().y << endl;
	
	if(true)ImGui::SetWindowSize((ImVec2(272,530)));
	else cout << "gui size" << ImGui::GetWindowSize().x << " & " << ImGui::GetWindowSize().y << endl;
	
	pressedRestart = ImGui::ImageButton((ImTextureID)(uintptr_t)imageButtonReStartSourceID, ImVec2(245,160));
	pressedAddOne = ImGui::ImageButton((ImTextureID)(uintptr_t)imageButtonAddOneSourceID, ImVec2(245,160));
	pressedExit = ImGui::ImageButton((ImTextureID)(uintptr_t)imageButtonExitSourceID, ImVec2(245,160));
	
	if(pressedRestart)change = 1;
	if(pressedAddOne){screen = 3;change=0;}
	if(pressedExit)ofExit();
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

	//TODO: check case when it's recording, and press SPACE. Must not change until end video recording.
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
	
	if (key == OF_KEY_RETURN || 'f') { // adding one subtitle
		flags();
	}
}

void ofApp::flags(){
	flag += 1;

	int actualVideoFrame = vidRecorder.getNumVideoFramesRecorded();
	addOneSubtitle(actualVideoFrame, "flag" + ofToString(flag));
	flagRect = 1;
}





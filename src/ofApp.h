#pragma once

#include "ofMain.h"
#include <Kinect.h>
#include "ofBufferObject.h"

const int cDepthWidth = 512;
const int cDepthHeight = 424;
const int cColorWidth = 1920;
const int cColorHeight = 1080;

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void exit();

private:
	IKinectSensor * sensor;
	ICoordinateMapper * mapper;
	IMultiSourceFrameReader * reader;

	// Intermediate Buffers
	//unsigned char rgbimage[cColorWidth*cColorHeight * 4];    // Stores RGB color image
	vector<ColorSpacePoint> colorSpacePoints;
	vector<CameraSpacePoint> cameraSpacePoints;
	vector<unsigned char> colorData;
	vector<ofVec4f> colorResultData;

	void setupKinect();
	void getKinectData();
	void getDepthData(IMultiSourceFrame* frame);
	void getRgbData(IMultiSourceFrame* frame);

	struct Particle {
		ofVec3f pos;
		ofVec4f color;
	};

	ofShader compute;
	vector<Particle> particles;
	ofBufferObject colorSpacePointBuffer;
	ofBufferObject cameraSpacePointBuffer;
	ofBufferObject colorBuffer;
	ofBufferObject colorResultBuffer;
	ofVbo vbo;

	ofEasyCam camera;

};

// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

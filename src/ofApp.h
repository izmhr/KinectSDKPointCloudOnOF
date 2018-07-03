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
	ofEasyCam camera;

	// KinectSDK
	IKinectSensor * sensor;
	ICoordinateMapper * mapper;
	IMultiSourceFrameReader * reader;

	// cpu 層のバッファ
	vector<ColorSpacePoint> colorSpacePoints;
	vector<CameraSpacePoint> cameraSpacePoints;
	vector<unsigned char> colorData;
	vector<ofVec4f> colorResultData;

	// for Compute Shader
	ofShader compute;
	ofBufferObject colorSpacePointBuffer;
	ofBufferObject cameraSpacePointBuffer;
	ofBufferObject colorBuffer;
	ofBufferObject colorResultBuffer;

	// For VBO (rendering)
	ofVbo vbo;

	void setupKinect();
	void setupComputeShader();
	void getKinectData();
	void getDepthData(IMultiSourceFrame* frame);
	void getRgbData(IMultiSourceFrame* frame);
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

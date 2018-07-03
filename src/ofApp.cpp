// Kinectv2 PointCloud ← 余り参考にならなかった
// https://homes.cs.washington.edu/~edzhang/tutorials/kinect2/kinect3.html

// Compute Shader + VBO on OF ← 大変勉強になった。これがベース
// https://github.com/openframeworks/openFrameworks/tree/master/examples/gl/computeShaderParticlesExample 

// Kinect のデータを Compute Shader に渡す on Unity ← 大変勉強になった
// https://github.com/sugi-cho/KinectStudy-Unity

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	setupKinect();

	camera.setNearClip(0.01f);
	camera.setFarClip(ofGetWidth() * 10);

	compute.setupShaderFromFile(GL_COMPUTE_SHADER, "kinectpc.glsl");
	compute.linkProgram();

	colorSpacePoints.resize(cDepthWidth * cDepthHeight);
	cameraSpacePoints.resize(cDepthWidth * cDepthHeight);
	colorData.resize(cColorWidth * cColorHeight * 4);
	colorResultData.resize(cDepthWidth * cDepthHeight);

	colorSpacePointBuffer.allocate(colorSpacePoints, GL_DYNAMIC_DRAW);
	cameraSpacePointBuffer.allocate(cameraSpacePoints, GL_DYNAMIC_DRAW);
	colorBuffer.allocate(colorData, GL_DYNAMIC_DRAW);
	colorResultBuffer.allocate(colorResultData, GL_DYNAMIC_DRAW);

	vbo.setVertexBuffer(cameraSpacePointBuffer, 3, sizeof(CameraSpacePoint), 0);
	vbo.setColorBuffer(colorResultBuffer, sizeof(ofVec4f), 0);
	vbo.enableColors();
	//vbo.disableColors();

	colorSpacePointBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	colorBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);
	colorResultBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 2);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (!reader)
	{
		return;
	}

	getKinectData();

	compute.begin();
	compute.setUniform1i("CWIDTH", 1920);
	compute.setUniform1i("CHEIGHT", 1080);
	compute.setUniform1i("DWIDTH", 512);
	compute.setUniform1i("DHEIGHT", 424);

	compute.dispatchCompute((512 * 424 + 8 - 1) / 8, 1, 1);
	compute.end();

	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());
}


//--------------------------------------------------------------
void ofApp::draw() {
	getKinectData();

	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(255, 255);
	camera.begin();
	glPointSize(1);
	vbo.draw(GL_POINTS, 0, cDepthWidth * cDepthHeight);
	camera.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::exit() {
	cout << "exit" << endl;
	sensor->Close();
	sensor->Release();
	sensor = NULL;
}

void ofApp::setupKinect() {
	HRESULT hr = GetDefaultKinectSensor(&sensor);

	if (sensor)
	{
		hr = sensor->Open();
		if (SUCCEEDED(hr))
		{
			hr = sensor->OpenMultiSourceFrameReader(
				FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color,
				&reader);
		}

		if (SUCCEEDED(hr)) {
			hr = sensor->get_CoordinateMapper(&mapper);
		}
	}

	if (!sensor || FAILED(hr))
	{
		std::cout << "no kinect sensor found!" << std::endl;
	}
}

void ofApp::getDepthData(IMultiSourceFrame * frame) {
	IDepthFrame* depthFrame;
	IDepthFrameReference* depthFrameRef = NULL;
	frame->get_DepthFrameReference(&depthFrameRef);
	depthFrameRef->AcquireFrame(&depthFrame);

	if (depthFrameRef) depthFrameRef->Release();
	if (!depthFrame) return;

	unsigned int sz;
	unsigned short* buf;
	depthFrame->AccessUnderlyingBuffer(&sz, &buf);
	// Write vertex coordinates
	mapper->MapDepthFrameToCameraSpace(cDepthWidth*cDepthHeight, buf, cDepthWidth*cDepthHeight, cameraSpacePoints.data());
	//float* fdest = (float*)dest;
	//for (int i = 0; i < sz; i++) {
	//	particles[i].pos.x = depth2xyz[i].X * 100;
	//	particles[i].pos.y = depth2xyz[i].Y * 100;
	//	particles[i].pos.z = depth2xyz[i].Z * 100;
	//}

	// Fill in depth2rgb map
	mapper->MapDepthFrameToColorSpace(cDepthWidth*cDepthHeight, buf, cDepthWidth*cDepthHeight, colorSpacePoints.data());
	if (depthFrame) depthFrame->Release();

	cameraSpacePointBuffer.setData(cameraSpacePoints, GL_DYNAMIC_DRAW);
	colorSpacePointBuffer.setData(colorSpacePoints, GL_DYNAMIC_DRAW);
}

void ofApp::getRgbData(IMultiSourceFrame* frame) {
	IColorFrame* colorframe;
	IColorFrameReference* frameref = NULL;
	frame->get_ColorFrameReference(&frameref);
	frameref->AcquireFrame(&colorframe);
	if (frameref) frameref->Release();

	if (!colorframe) return;

	// Get data from frame
	colorframe->CopyConvertedFrameDataToArray(cColorWidth*cColorHeight * 4, colorData.data(), ColorImageFormat_Rgba);

	//// Write color array for vertices
	////float* fdest = (float*)dest;
	//for (int i = 0; i < cDepthWidth*cDepthHeight; i++) {
	//	ColorSpacePoint p = depth2rgb[i];
	//	// Check if color pixel coordinates are in bounds
	//	if (p.X < 0 || p.Y < 0 || p.X > cColorWidth || p.Y > cColorHeight) {
	//		particles[i].color.set(0, 0, 0, 0);
	//	}
	//	else {
	//		int idx = (int)p.X + cColorWidth * (int)p.Y;
	//		float r = rgbimage[4 * idx + 0] / 255.;
	//		float g = rgbimage[4 * idx + 1] / 255.;
	//		float b = rgbimage[4 * idx + 2] / 255.;
	//		particles[i].color.set(r, g, b, 1.0f);
	//	}
	//	// Don't copy alpha channel
	//}
	colorBuffer.setData(colorData, GL_DYNAMIC_DRAW);

	if (colorframe) colorframe->Release();
}

void ofApp::getKinectData() {
	IMultiSourceFrame* frame = NULL;
	if (SUCCEEDED(reader->AcquireLatestFrame(&frame))) {
		getDepthData(frame);
		getRgbData(frame);
	}
	if (frame) frame->Release();
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

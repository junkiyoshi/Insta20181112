#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofBackground(239);
	ofSetWindowTitle("Insta");
	ofSetFrameRate(30);

	this->cap.open("D:\\video\\bread.mp4");
	this->cap_size = cv::Size(1280, 720);

	this->rect_size = 40;
	for (int x = 0; x < this->cap_size.width; x += this->rect_size) {

		for (int y = 0; y < this->cap_size.height; y += this->rect_size) {

			ofImage* image = new ofImage();
			image->allocate(this->rect_size, this->rect_size, OF_IMAGE_COLOR);
			this->rect_images.push_back(image);

			cv::Mat frame = cv::Mat(cv::Size(image->getWidth(), image->getHeight()), CV_MAKETYPE(CV_8UC3, image->getPixels().getNumChannels()), image->getPixels().getData(), 0);
			this->rect_frames.push_back(frame);

			cv::Rect rect = cv::Rect(x, y, this->rect_size, this->rect_size);
			this->cv_rects.push_back(rect);
		}
	}

	cap.set(CV_CAP_PROP_POS_FRAMES, 600);
	this->number_of_frames = 600;
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, frame;
		this->cap >> src;
		if (src.empty()) {

			return;
		}

		cv::resize(src, frame, this->cap_size);
		cv::cvtColor(frame, frame, CV_BGR2RGB);

		this->frames.push_back(frame);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->rect_images.size(); i++) {

		int frames_index = ofMap(ofNoise(this->cv_rects[i].x * 0.0005, this->cv_rects[i].y * 0.0005, ofGetFrameNum() * 0.008), 0, 1, 0, this->number_of_frames);
		cv::Mat tmp_box_image(this->frames[frames_index], this->cv_rects[i]);
		tmp_box_image.copyTo(this->rect_frames[i]);

		this->rect_images[i]->update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->rect_images.size(); i++) {

		this->rect_images[i]->draw(this->cv_rects[i].x, this->cv_rects[i].y);
	}
}

//========================================================================
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
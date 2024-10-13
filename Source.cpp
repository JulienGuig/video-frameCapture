#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>
#include <chrono>

#include <windows.h>
#include <Commdlg.h>

using namespace std;
using namespace chrono;
using namespace cv;

string browse(HWND hwnd) //https://stackoverflow.com/questions/38774248/how-to-save-ofn-lpstrfile-to-string-properly
{
	std::string path(MAX_PATH, '\0');
	OPENFILENAME ofn = { sizeof(OPENFILENAME) };
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter =
		"Video files (*.mp4;*.gif)\0*.mp4;*.gif\0"
		"All files\0*.*\0";
	ofn.lpstrFile = &path[0];
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn))
	{
		//string::size() is still MAX_PATH
		//strlen is the actual string size (not including the null-terminator)
		//update size:
		path.resize(strlen(path.c_str()));
	}
	return path;
}

int main() {
	cout << "press escape to stop" << endl;

	string path = browse(0);
	
	VideoCapture cap(path); //video
	unsigned int vidFrame = 0;

	// Check if video opened successfully
	if (!cap.isOpened()) {
		cout << endl << "--------------------------" << endl;
		cout << "Error opening video stream or file" << endl;
		cout << "--------------------------" << endl;
		return -1;
	}

	string pathFile = path.substr(0, path.find_last_of('\\')+1);
	string fileName = path.substr(path.find_last_of('\\')+1, (path.length() - path.find_last_of('\\')) - 5);

	cout << "image will be saved in: " << pathFile << endl;
	cout << endl;

	
	while (true) {
		Mat frame;

		cap >> frame;

		// Display the resulting frame
		imshow("video frame capture", frame);

		//write the frame
		string imgName = pathFile + fileName + "-frame" + to_string(vidFrame) + ".jpg";
		imwrite(imgName, frame);

		vidFrame++;

		// Press ESC to exit
		char c = (char)(waitKey(1));
		if (c == 27) {
			cout << "video stoped" << endl;
			cap.release();
			return 0;
		}

		//When everything done, release the video capture object
		if (cap.get(CAP_PROP_POS_FRAMES) == cap.get(CAP_PROP_FRAME_COUNT)) {
			cout << "video ended" << endl;
			cap.release();
			return 0;
		}
	}

	//cap.release();
	return 0;
}

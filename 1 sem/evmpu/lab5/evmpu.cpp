#include <iostream>
#include <ctime>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
	VideoCapture capture;
	capture.open(0);
	Mat frame, convertedImage1, convertedImage2;
	namedWindow("frame", WINDOW_AUTOSIZE);
	namedWindow("converted", WINDOW_AUTOSIZE);
	double totalTime = 0, inputTime = 0, convTime = 0, outputTime = 0, percent;
	long long int framesCount = 0;

	if (capture.isOpened())
	{
		cout << "start" << endl;

		while (true)
		{
			clock_t t_start = clock();
			capture >> frame;
			clock_t t_end = clock();

			double timeTaken1 = (1.0 * t_end - t_start) / CLOCKS_PER_SEC;
			totalTime += timeTaken1;
			inputTime += timeTaken1;

			cout << "Time to take frame " << timeTaken1 << endl;

			t_start = clock();
			cvtColor(frame, convertedImage1, COLOR_BGR2GRAY);
			threshold(convertedImage1, convertedImage2, 127, 255, THRESH_BINARY);
			t_end = clock();
			double timeTaken2 = (1.0 * t_end - t_start) / CLOCKS_PER_SEC;
			totalTime += timeTaken2;
			convTime += timeTaken2;

			cout << "Time to convert frame " << timeTaken2 << endl;

			cout << convertedImage1 << endl;
			t_start = clock();
			imshow("frame", frame);
			imshow("converted", convertedImage2);
			t_end = clock();
			double timeTaken3 = (1.0 * t_end - t_start) / CLOCKS_PER_SEC;

			totalTime += timeTaken3;
			outputTime += timeTaken3;

			cout << "Time to output frame " << timeTaken3 << endl;

			t_start = clock();
			char c = (char)waitKey(1);
			if (c == 'q' || c == 'Q' || c == 27)
				break;

			t_end = clock();
			totalTime += (1.0 * t_end - t_start) / CLOCKS_PER_SEC;
			framesCount++;

		}
	}
	else
	{
		cout << "Can't open video capture" << endl;
	}

	cout << endl;

	percent = totalTime / 100.0;
	cout << "Part of time to read frames " << 1.0 * inputTime / percent << "%" << endl;
	cout << "Part of time to convert frames " << 1.0 * convTime / percent << "%" << endl;
	cout << "Part of time to print frames " << 1.0 * outputTime / percent << "%" << endl;
	cout << "Frames per second " << 1.0 * framesCount / totalTime << endl;

	return 0;
}

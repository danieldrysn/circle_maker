// circle_maker.cpp : Main entry point for the application.
// Created by: Daniel D. Doyle
//

#include <opencv2/opencv.hpp>
#include "circle_algorithms.h"

static void onMouse( int event, int x, int y, int, void* );

bool tracking = false;
bool complete = false;
std::vector<cv::Point> points;

int main(void)
{
	std::string winName = "Circle Interpreter:";
	cv::namedWindow( winName, 1 );
	
	cv::setMouseCallback( winName, onMouse, 0 );

	cv::Mat background = 255*cv::Mat::ones(800,1200,CV_8UC3);
	cv::imshow( winName, background );
	cv::waitKey( 1 );

	char key = 0;
	while( key != 'e' || key != 27 )
	{
		while( !complete )
		{
			key = cv::waitKey( 10 );
			if( key == 'e' || key == 27 )
				break;
		}

		cv::Mat_<float> pts( points.size(), 2 );
		for( size_t i = 0; i < points.size(); ++i )
		{
			pts.at<float>(i,0) = points[i].x;
			pts.at<float>(i,1) = points[i].y;
			cv::circle( background, points[i], 1, CV_RGB(255,255,255),1,8);
		}

		cv::Point2f ctr = cv::Point2f( 0, 0 );
		float rad = 0;

		if( points.size() > 3 )
		{
			circ_algebraic_dist( pts, ctr, rad );
			if( rad > 0 )
			{
				cv::circle( background, cv::Point( ctr.x, ctr.y ), (int)rad, CV_RGB(255,0,0),1,4);
				//std::cout << ctr.x << ", " << ctr.y << ", " << rad << "\n";
			}
			circ_geometric_dist( pts, ctr, rad );
			if( rad > 0 )
			{
				cv::circle( background, cv::Point( ctr.x, ctr.y ), (int)rad, CV_RGB(0,255,0),1,8);
				cv::imshow( winName, background );
				cv::waitKey( 1 );
			}
			//std::cout << ctr.x << ", " << ctr.y << ", " << rad << "\n";
			//std::cout << points << "\n\n";
		}

		complete = false;
		points.clear();
	}

	cv::waitKey( 0 );
	cv::destroyAllWindows();
}

static void onMouse( int event, int x, int y, int, void* )
{
	switch( event )
	{
	case cv::EVENT_LBUTTONDOWN:
		if(!tracking)
			points.push_back( cv::Point( x, y ) );
		tracking = true;
		break;
	case cv::EVENT_MOUSEMOVE:
		if( tracking )
			points.push_back( cv::Point( x, y ) );
		break;
	case cv::EVENT_LBUTTONUP:
		if( tracking )
		{
			tracking = false;
			complete = true;
		}
		break;
	default:
		break;
	}
}






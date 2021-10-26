/* Created by: Daniel D. Doyle
 * Based on:
 * Gander, W., Golub G. H., and Strebel, R., "Least Squares Circles and Ellipses",
 */

#include "circle_algorithms.h"


void circ_algebraic_dist( cv::Mat pts, cv::Point2f& center, float& radius )
{
	cv::Mat_<float> B(pts.rows,4);

	for( size_t i = 0; i < pts.rows; ++i )
	{
		B.at<float>(i,0) = pts.at<float>(i,0)*pts.at<float>(i,0)+pts.at<float>(i,1)*pts.at<float>(i,1); // Column 1: x(i)^2 + y(i)^2  
		B.at<float>(i,1) = pts.at<float>(i,0);															// Column 2: x(i)
		B.at<float>(i,2) = pts.at<float>(i,1);															// Column 3: y(i) 
		B.at<float>(i,3) = 1;																			// Column 4: 1
	}

	// Circle: Minimizing the Algebraic Distance
	cv::Mat w, u, vt;
	cv::SVD svd_B;
	svd_B.compute( B, w, u, vt );

	float a = vt.at<float>(3,0);
	float b1 = vt.at<float>(3,1);
	float b2 = vt.at<float>(3,2);
	float c = vt.at<float>(3,3);

	center.x = -b1/(2*a);
	center.y = -b2/(2*a);

	radius = cv::sqrt( (b1*b1+b2*b2)/(4*a*a)-c/a);
}

void circ_geometric_dist( cv::Mat pts, cv::Point2f& center, float& radius )
{
	// Circle: Minimizing the Geometric Distance
	cv::Mat F(pts.rows,1,CV_32F);
	cv::Mat J(pts.rows,3,CV_32F);
	cv::Mat U(3,1,CV_32F);
	U.at<float>(0,0) = center.x;
	U.at<float>(1,0) = center.y;
	U.at<float>(2,0) = radius;
	cv::Mat J_inv;
	float check = 1000;
	size_t iter = 0;
	while( check > 1e-4 && iter < 1000 )
	{
		for( size_t i=0; i < pts.rows; ++i )
		{
			F.at<float>(i,0) = cv::sqrt((U.at<float>(0,0)-pts.at<float>(i,0))*(U.at<float>(0,0)-pts.at<float>(i,0))+(U.at<float>(1,0)-pts.at<float>(i,1))*(U.at<float>(1,0)-pts.at<float>(i,1)))-U.at<float>(2,0);
			J.at<float>(i,0) = (U.at<float>(0,0)-pts.at<float>(i,0))/cv::sqrt( (U.at<float>(0,0)-pts.at<float>(i,0))*(U.at<float>(0,0)-pts.at<float>(i,0)) + (U.at<float>(1,0)-pts.at<float>(i,1))*(U.at<float>(1,0)-pts.at<float>(i,1)) );
			J.at<float>(i,1) = (U.at<float>(1,0)-pts.at<float>(i,1))/cv::sqrt( (U.at<float>(0,0)-pts.at<float>(i,0))*(U.at<float>(0,0)-pts.at<float>(i,0)) + (U.at<float>(1,0)-pts.at<float>(i,1))*(U.at<float>(1,0)-pts.at<float>(i,1)) );
			J.at<float>(i,2) = -1;
		}
		cv::invert(J,J_inv,cv::DECOMP_SVD);
		U = U-(J_inv*F);

		check = cv::norm(J_inv*F);
		//std::cout << check << "\n";
		iter++;
	}
	if( ~U.empty() )
	{
		center.x = U.at<float>(0,0);
		center.y = U.at<float>(1,0);
		radius = U.at<float>(2,0);
	}

	//std::cout << "Iterations = " << iter << "\n\n";

}
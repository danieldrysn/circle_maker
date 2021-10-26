/* Least Squares Circles and Ellipses
 * Gander, W., Golub G. H., and Strebel, R., "Least Squares Circles and Ellipses",
 */

#include </usr/local/include/opencv4/opencv2/opencv.hpp>

void circ_algebraic_dist( cv::Mat pts, cv::Point2f& center, float& radius );
void circ_geometric_dist( cv::Mat pts, cv::Point2f& center, float& radius );

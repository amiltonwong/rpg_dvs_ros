// This file is part of DVS-ROS - the RPG DVS ROS Package
//
// DVS-ROS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// DVS-ROS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with DVS-ROS.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DVS_CALIBRATION_H
#define DVS_CALIBRATION_H

#include <list>

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <dvs_msgs/Event.h>
#include <dvs_msgs/EventArray.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_srvs/Empty.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/SetCameraInfo.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "dvs_calibration/circlesgrid.hpp"
#include "dvs_calibration/board_detection.h"
#include "dvs_calibration/transition_map.h"
#include "dvs_calibration/calibration_parameters.h"


namespace dvs_calibration {

const int left_camera_id = 1;
const int right_camera_id = 2;
const int mono_camera_id = 3;

class DvsCalibration
{
public:
  DvsCalibration();
  virtual ~DvsCalibration() {};

protected:
  // parameters
  static const int sensor_width = 128;
  static const int sensor_height = 128;
  CalibrationParameters params;
  ros::Time last_pattern_found;

  // event maps
  std::map<int, TransitionMap> transition_maps;

  // status
  bool calibration_running;
  int num_detections;

  // callbacks
  void eventsCallback(const dvs_msgs::EventArray::ConstPtr& msg, int camera_id);

  // services
  bool resetCalibrationCallback(std_srvs::Empty::Request& request, std_srvs::Empty::Response& response);
  bool startCalibrationCallback(std_srvs::Empty::Request& request, std_srvs::Empty::Response& response);
  bool saveCalibrationCallback(std_srvs::Empty::Request& request, std_srvs::Empty::Response& response);

  // virtual functions for these services
  virtual void resetCalibration() = 0;
  virtual void startCalibration() = 0;
  virtual void saveCalibration() = 0;


  std::vector<cv::Point3f> world_pattern;
  virtual void add_pattern(int id) = 0;
  virtual void update_visualization(int id) = 0;

  // ROS interface
  ros::NodeHandle nh;
  ros::ServiceServer startCalibrationService;
  ros::ServiceServer saveCalibrationService;
  ros::ServiceServer resetCalibrationService;
  ros::Publisher numDetectionsPublisher;
  ros::Publisher calibrationOutputPublisher;


  void loadCalibrationParameters();
};

} // namespace

#endif // DVS_CALIBRATION_H

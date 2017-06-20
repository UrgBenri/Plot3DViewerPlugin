/*
	This file is part of the UrgBenri application.

	Copyright (c) 2016 Mehrez Kristou.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Please contact kristou@hokuyo-aut.jp for more details.

*/

#include "Converter3d.h"
#include <QVector3D>

bool with_mirror_unit = false;
double up_vector_aux[] = {1.0, 1.0, 1.0};
std::vector< double > up_vector;
double mirror_normal_right_aux[] = {-828.21, -828.21, -1171.08};
double mirror_normal_left_aux[] = {828.21, -828.21, 1171.08};
std::vector< double > mirror_normal_right;
std::vector< double > mirror_normal_left;
double MIRROR_CENTER_DISTANCE = 59.42 / 1000.0;
bool remove_points = true;

Converter3D::Converter3D()
{
    mirror_normal_left = std::vector<double>(mirror_normal_left_aux, mirror_normal_left_aux + sizeof(mirror_normal_left_aux) / sizeof(double));
    mirror_normal_right = std::vector<double>(mirror_normal_right_aux, mirror_normal_right_aux + sizeof(mirror_normal_right_aux) / sizeof(double));
}

Converter3D::~Converter3D()
{

}

void genPointCloud(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    enum {
        RIGHT_MIRROR_LIM_MIN = -570,
        RIGHT_MIRROR_LIM_MAX = RIGHT_MIRROR_LIM_MIN + 120,
        RIGHT_MIRROR_CENTER = (RIGHT_MIRROR_LIM_MAX + RIGHT_MIRROR_LIM_MIN)/2,
        LEFT_MIRROR_LIM_MAX = 570,
        LEFT_MIRROR_LIM_MIN = LEFT_MIRROR_LIM_MAX - 120,
        LEFT_MIRROR_CENTER = (LEFT_MIRROR_LIM_MAX + LEFT_MIRROR_LIM_MIN)/2,
        INVALID_STEPS_RIGHT_MIN = RIGHT_MIRROR_LIM_MIN + 61,
        INVALID_STEPS_RIGHT_MAX = RIGHT_MIRROR_LIM_MIN + 189,
        INVALID_STEPS_LEFT_MIN = LEFT_MIRROR_LIM_MAX - 189,
        INVALID_STEPS_LEFT_MAX = LEFT_MIRROR_LIM_MAX - 61,
    };
    double L = MIRROR_CENTER_DISTANCE;
    const double ang_right = (RIGHT_MIRROR_CENTER*0.25)*M_PI/180.0;
    const double ang_left = (LEFT_MIRROR_CENTER*0.25)*M_PI/180.0;
    double range = 0.0;
    double l_aux = 0.0;
    QVector3D p;
    int i = 0;
    QVector3D normal_right(mirror_normal_right_aux[0], mirror_normal_right_aux[1], mirror_normal_right_aux[2]);
    QVector3D normal_left(mirror_normal_left_aux[0], mirror_normal_left_aux[1], mirror_normal_left_aux[2]);
    QVector3D incoming(0,0,0);
    QVector3D reflection(0,0,0);
    QVector3D v_aux(0,0,0);
    double dot = 0.0;
    double max_height = -1.0;
    float* cloud2_data;
    QVector3D up_vector_tf;


    if (ros::param::get("~up_vector", up_vector)) {
        up_vector_tf = QVector3D(up_vector[0], up_vector[1], up_vector[2]);
        //printf("up vector=[%f,%f,%f]\n",
        //        up_vector_tf.x(), up_vector_tf.y(), up_vector_tf.z());
    }
    normal_right = QVector3D(mirror_normal_right[0], mirror_normal_right[1], mirror_normal_right[2]);
    normal_left = QVector3D(mirror_normal_left[0], mirror_normal_left[1], mirror_normal_left[2]);

    normal_right.normalize();
    normal_left.normalize();
    //printf("normal(R)=[%f,%f,%f], normal(L)=[%f,%f,%f]\n",
    //        normal_right.x(), normal_right.y(), normal_right.z(),
    //        normal_left.x(), normal_left.y(), normal_left.z());

    projector.projectLaser(*scan, cloud, -1.0, laser_geometry::channel_option::Distance | laser_geometry::channel_option::Intensity | laser_geometry::channel_option::Index);
    projector.projectLaser(*scan, cloud2, -1.0, laser_geometry::channel_option::Distance | laser_geometry::channel_option::Intensity | laser_geometry::channel_option::Index);

    cloud.header.frame_id = scan->header.frame_id;
    cloud.header.stamp = scan->header.stamp;
    cloud.header.seq = scan->header.seq;
    cloud2.header.frame_id = scan->header.frame_id;
    cloud2.header.stamp = scan->header.stamp;
    cloud2.header.seq = scan->header.seq;

    //is the step range outside the mirrors?
    if (urg_->getFirstStep() > RIGHT_MIRROR_LIM_MAX || urg_->getLastStep() < LEFT_MIRROR_LIM_MIN) {
        return;
    }
    //right mirror
    //compute the UP points
    if (urg_->getFirstStep() <= RIGHT_MIRROR_LIM_MAX) {
        for (i=urg_->getFirstStep(); i < RIGHT_MIRROR_LIM_MAX; i++) {
            l_aux = L / cos(urg_->step2rad(i) - ang_right);
            range = scan->ranges[urg_->step2index(i)] - l_aux;
            incoming.setX( l_aux * cos(urg_->step2rad(i)) );
            incoming.setY( l_aux * sin(urg_->step2rad(i)) );
            incoming.setZ(0.0);
            dot = 2.0 * incoming.dot(normal_right);
            v_aux = normal_right;
            v_aux *= dot;
            reflection = incoming;
            reflection -= v_aux;
            reflection.normalize();
            reflection *= range;
            //printf("(R) step %d range=%f laux=%f step2rad=%f:: incoming: [%f,%f,%f], normal=[%f,%f,%f], reflection=[%f,%f,%f]\n",
            //        i, range, l_aux, urg_->step2rad(i),
            //        incoming.x(), incoming.y(), incoming.z(),
            //        normal_right.x(), normal_right.y(), normal_right.z(),
            //        reflection.x(), reflection.y(), reflection.z());
            cloud.points[urg_->step2index(i)].x = reflection.x();
            cloud.points[urg_->step2index(i)].y = reflection.y();
            cloud.points[urg_->step2index(i)].z = reflection.z();
            reflection *= up_vector_tf;
            if (abs(reflection.z()) > max_height) {
                max_height = reflection.z();
            }

            cloud2_data = reinterpret_cast<float*>(&cloud2.data[ urg_->step2index(i)*cloud2.point_step + cloud2.fields[0].offset ]);
            *(cloud2_data++) = reflection.x();
            *(cloud2_data++) = reflection.y();
            *(cloud2_data++) = reflection.z();
            //*(float*)&(cloud2.data[urg_->step2index(i)*cloud2.point_step + cloud2.fields[0].offset]) = reflection.x();
            //*(float*)&(cloud2.data[urg_->step2index(i)*cloud2.point_step + cloud2.fields[1].offset]) = reflection.y();
            //*(float*)&(cloud2.data[urg_->step2index(i)*cloud2.point_step + cloud2.fields[2].offset]) = reflection.z();
        }
    }
    //remove invalid points
    if (remove_points && urg_->getFirstStep() <= INVALID_STEPS_RIGHT_MIN && urg_->getLastStep() >= INVALID_STEPS_RIGHT_MAX) {
        for (i=INVALID_STEPS_RIGHT_MIN; i <= INVALID_STEPS_RIGHT_MAX; i++) {
            cloud.points[urg_->step2index(i)].x = 0.0;
            cloud.points[urg_->step2index(i)].y = 0.0;
            cloud.points[urg_->step2index(i)].z = 0.0;
            cloud2_data = reinterpret_cast<float*>(&cloud2.data[ urg_->step2index(i)*cloud2.point_step + cloud2.fields[0].offset ]);
            *(cloud2_data++) = 0.0;
            *(cloud2_data++) = 0.0;
            *(cloud2_data++) = 0.0;
        }
    }
    //left mirror
    //compute the UP points
    if (urg_->getLastStep() >= LEFT_MIRROR_LIM_MIN) {
        for (i = LEFT_MIRROR_LIM_MIN; i < urg_->getLastStep(); i++) {
            l_aux = L / cos(urg_->step2rad(i) - ang_left);
            range = scan->ranges[urg_->step2index(i)] - l_aux;
            incoming.setX( l_aux * cos(urg_->step2rad(i)) );
            incoming.setY( l_aux * sin(urg_->step2rad(i)) );
            incoming.setZ(0.0);
            dot = 2.0 * incoming.dot(normal_left);
            v_aux = normal_left;
            v_aux *= dot;
            reflection = incoming;
            reflection -= v_aux;
            reflection.normalize();
            reflection *= range;
            //printf("(L) step %d range=%f laux=%f step2rad=%f:: incoming: [%f,%f,%f], normal=[%f,%f,%f], reflection=[%f,%f,%f]\n",
            //        i, range, l_aux, urg_->step2rad(i),
            //        incoming.x(), incoming.y(), incoming.z(),
            //        normal_left.x(), normal_left.y(), normal_left.z(),
            //        reflection.x(), reflection.y(), reflection.z());
            cloud.points[urg_->step2index(i)].x = reflection.x();
            cloud.points[urg_->step2index(i)].y = reflection.y();
            cloud.points[urg_->step2index(i)].z = reflection.z();
            reflection *= up_vector_tf;
            if (abs(reflection.z()) > max_height) {
                max_height = reflection.z();
            }

            cloud2_data = reinterpret_cast<float*>(&cloud2.data[ urg_->step2index(i)*cloud2.point_step + cloud2.fields[0].offset ]);
            *(cloud2_data++) = reflection.x();
            *(cloud2_data++) = reflection.y();
            *(cloud2_data++) = reflection.z();
            //*(float*)&(cloud2.data[urg_->step2index(i)*cloud2.point_step + cloud2.fields[0].offset]) = reflection.x();
            //*(float*)&(cloud2.data[urg_->step2index(i)*cloud2.point_step + cloud2.fields[1].offset]) = reflection.y();
            //*(float*)&(cloud2.data[urg_->step2index(i)*cloud2.point_step + cloud2.fields[2].offset]) = reflection.z();
            //cloud2.height = (unsigned int)max_height;
        }
    }
    //remove invalid points
    if (remove_points && urg_->getFirstStep() <= INVALID_STEPS_LEFT_MIN && urg_->getLastStep() >= INVALID_STEPS_LEFT_MAX) {
        for (i=INVALID_STEPS_LEFT_MIN; i <= INVALID_STEPS_LEFT_MAX; i++) {
            cloud.points[urg_->step2index(i)].x = 0.0;
            cloud.points[urg_->step2index(i)].y = 0.0;
            cloud.points[urg_->step2index(i)].z = 0.0;
            cloud2_data = reinterpret_cast<float*>(&cloud2.data[ urg_->step2index(i)*cloud2.point_step + cloud2.fields[0].offset ]);
            *(cloud2_data++) = 0.0;
            *(cloud2_data++) = 0.0;
            *(cloud2_data++) = 0.0;
        }
    }

}


QVector<QVector<QPointF> > Converter3D::getPoints(const QVector<QVector<long> > &steps
                                                , const QPointF &offset
                                                , qreal rotation
                                                , int max_length) const
{

    QVector<QVector<QPointF> > points;

    for (int i = 0; i < steps.size(); i++) {
        QVector<QPointF> point;
        for (int j = 0; j < steps[i].size(); j++) {
            point << range2point(index2Step(i)
                                 , steps[i][j]
                                 , rotation
                                 , offset
                                 , max_length);
        }
        points << point;
    }

    return points;
}


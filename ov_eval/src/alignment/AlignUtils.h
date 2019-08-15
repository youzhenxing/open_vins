#ifndef OV_EVAL_ALIGNUTILS_H
#define OV_EVAL_ALIGNUTILS_H


#include <string>
#include <sstream>
#include <iostream>
#include <Eigen/Eigen>

#include "utils/Math.h"


namespace ov_eval {


    /**
     * @brief Helper functions for the trajectory alignment class.
     *
     * The key function is an implementation of Umeyama's
     * [Least-squares estimation of transformation parameters between two point patterns](https://ieeexplore.ieee.org/document/88573).
     * This is what allows us to find the transform between the two
     * trajectories without worrying about singularities for the absolute trajectory error.
     */
    class AlignUtils {

    public:

        /**
         * @brief Gets best yaw from Frobenius problem.
         * Equation (17)-(18) in [Zhang et al. 2018 IROS](http://rpg.ifi.uzh.ch/docs/IROS18_Zhang.pdf) paper.
         * @param C Data matrix
         */
        static inline double get_best_yaw(const Eigen::Matrix<double, 3, 3> &C) {
            double A = C(0, 1) - C(1, 0);
            double B = C(0, 0) + C(1, 1);
            //return M_PI_2 - atan2(B, A);
            return atan2(A, B);
        }

        /**
         * @brief Gets mean of the vector of data
         * @param data Vector of data
         * @return Mean value
         */
        static inline Eigen::Matrix<double, 3, 1> get_mean(const std::vector<Eigen::Matrix<double, 3, 1>> &data) {
            Eigen::Matrix<double, 3, 1> mean = Eigen::Matrix<double, 3, 1>::Zero();
            for (size_t i = 0; i < data.size(); i++) {
                mean.noalias() += data[i];
            }
            mean /= data.size();
            return mean;
        }

        /**
         * @brief Given a set of points in a model frame and a set of points in a data frame,
         * finds best transform between frames (subject to constraints).
         *
         * @param data Vector of points in data frame (i.e., estimates)
         * @param model Vector of points in model frame (i.e., gt)
         * @param R Output rotation from data frame to model frame
         * @param t Output translation from data frame to model frame
         * @param s Output scale from data frame to model frame
         * @param known_scale Whether to fix scale
         * @param yaw_only Whether to only use yaw orientation (such as when frames are already gravity aligned)
         */
        static void align_umeyama(const std::vector<Eigen::Matrix<double, 3, 1>> &data,
                                  const std::vector<Eigen::Matrix<double, 3, 1>> &model,
                                  Eigen::Matrix<double, 3, 3> &R, Eigen::Matrix<double, 3, 1> &t,
                                  double &s, bool known_scale, bool yaw_only);


    };


}

#endif //OV_EVAL_ALIGNUTILS_H

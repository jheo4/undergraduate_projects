#
# Author : Jin Heo
# Association : Ajou Univ
# Date : 14 04 2018
#

import numpy as np
import cv2 as cv
import os
from evaluation import evaluateFunc

def get_result(_threshold):
    input_path = './input_image'
    truth_path = './groundtruth'
    result_path = './result'
    background_path = './background'
    input_image = [img for img in sorted(os.listdir(input_path)) if img.endswith(".jpg")]

    #gaussian_kernel = cv.getGaussianKernel(5, 3)
    cur_frame = cv.cvtColor(
        cv.imread(os.path.join(input_path, input_image[0])),
        cv.COLOR_BGR2GRAY
    ).astype(np.float64)
    #cur_frame = cv.filter2D(cur_frame, -1, gaussian_kernel).astype(np.float64)

    background_gaussian = cv.getGaussianKernel(21, 3)
    background_image = cv.cvtColor(
        cv.imread(os.path.join(background_path, "background.png")),
        cv.COLOR_BGR2GRAY
    )
    #background_image = cv.filter2D(background_image, -1, background_gaussian).astype(np.float64)
    background_image = cv.medianBlur(background_image, 3).astype(np.float64)
    diff_gaussian = cv.getGaussianKernel(29, 2)
    for image_index in range(len(input_image)-1):
        diff = np.abs(cur_frame - background_image)
        diff = cv.filter2D(diff, -1, diff_gaussian)
        diff = cv.medianBlur(diff.astype(np.uint8), 15).astype(np.float64)

        over_threshold = np.where(diff > _threshold, 1.0, 0.0)
        diff_mask = np.multiply(cur_frame, over_threshold)
        diff_mask_bin = np.where(diff_mask > 0, 255.0, 0.0)

        result_image = diff_mask_bin.astype(np.uint8)

        cv.imwrite(os.path.join(result_path, 'result%06d.png' % (image_index + 1)), result_image)

        cur_frame = cv.cvtColor(
            cv.imread(os.path.join(input_path, input_image[image_index + 1])),
            cv.COLOR_BGR2GRAY
        ).astype(np.float64)
        #cur_frame = cv.filter2D(cur_frame, -1, gaussian_kernel).astype(np.float64)

        k = cv.waitKey(30) & 0xff
        if k == 27:
            break

    return evaluateFunc(truth_path, result_path)

if __name__ == '__main__':
    get_result(20)

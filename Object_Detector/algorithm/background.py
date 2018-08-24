#
# Author : Jin Heo
# Association : Ajou Univ
# Date : 14 04 2018
#

import numpy as np
import cv2 as cv
import os

def get_background(_alpha):
    input_path = './input_image'
    result_path = './background'
    input_image = [img for img in sorted(os.listdir(input_path)) if img.endswith(".jpg")]
    cur_frame = cv.cvtColor(
        cv.imread(os.path.join(input_path, input_image[0])),
        cv.COLOR_BGR2GRAY
    ).astype(np.float64)

    back_frame = cur_frame
    for image_index in range(len(input_image)-1):
        back_frame = ((1 - _alpha) * back_frame).astype(np.float64) + (_alpha * cur_frame).astype(np.float64)
        cur_frame = cv.cvtColor(
            cv.imread(os.path.join(input_path, input_image[image_index + 1])),
            cv.COLOR_BGR2GRAY
        ).astype(np.float64)
        k = cv.waitKey(30) & 0xff
        if k == 27:
            break
    result_image = back_frame.astype(np.uint8)
    cv.imwrite(os.path.join(result_path, 'background.png'), result_image)

if __name__ == '__main__':
    get_background(0.00037)

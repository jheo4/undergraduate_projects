#
# Author : Jin Heo
# Association : Ajou Univ
# Date : 14 04 2018
#

import numpy as np
import cv2 as cv
import os
from evaluation import evaluateFunc


def background_mog():
    input_path = './input_image'
    truth_path = './groundtruth'
    result_path = './result'

    input_image = [img for img in sorted(os.listdir(input_path)) if img.endswith(".jpg")]

    current_frame = cv.cvtColor(
        cv.imread(os.path.join(input_path, input_image[0])),
        cv.COLOR_BGR2GRAY
    )
    fgbg = cv.createBackgroundSubtractorMOG2()

    for image_index in range(len(input_image)):
        masked_frame = fgbg.apply(current_frame)

        result_image = masked_frame.astype(np.uint8)
        cv.imwrite(os.path.join(result_path, 'result%06d.png' % (image_index + 1)), result_image)

        if image_index == len(input_image) - 1:
            break

        current_frame = cv.cvtColor(
            cv.imread(os.path.join(input_path, input_image[image_index + 1])),
            cv.COLOR_BGR2GRAY
        )

        k = cv.waitKey(30) & 0xff
        if k == 27:
            break
    rec_prec = evaluateFunc(truth_path, result_path)
    rec = rec_prec[0]
    prec = rec_prec[1]
    print("rec : ", rec, "precision : ", prec)
    print("sum : ", rec+prec)

if __name__ == '__main__':
    background_mog()

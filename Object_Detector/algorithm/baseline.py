import numpy as np
import cv2 as cv
import os
from evaluation import evaluateFunc


def baseline(_threshold):
    input_path = './input_image'
    truth_path = './groundtruth'
    result_path = './result'

    input_image = [img for img in sorted(os.listdir(input_path)) if img.endswith(".jpg")]

    # init
    current_frame = cv.cvtColor(
        cv.imread(os.path.join(input_path, input_image[0])),
        cv.COLOR_BGR2GRAY
    ).astype(np.float64)
    prev_frame = current_frame

    for image_index in range(len(input_image)):
        diff = np.abs(current_frame - prev_frame).astype(np.float64)
        over_threshold = np.where(diff > _threshold, 1.0, 0.0)

        diff_mask = np.multiply(current_frame, over_threshold)
        diff_mask_bin = np.where(diff_mask > 0, 255.0, 0.0)

        result_image = diff_mask_bin.astype(np.uint8)
        cv.imwrite(os.path.join(result_path, 'result%06d.png' % (image_index + 1)), result_image)

        if image_index == len(input_image) - 1:
            break

        prev_frame = current_frame
        current_frame = cv.cvtColor(
            cv.imread(os.path.join(input_path, input_image[image_index + 1])),
            cv.COLOR_BGR2GRAY
        ).astype(np.float64)

        k = cv.waitKey(30) & 0xff
        if k == 27:
            break

    rec_prec = evaluateFunc(truth_path, result_path)
    print(rec_prec[0], rec_prec[0])

if __name__ == "__main__":
    baseline(10)

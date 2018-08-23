import numpy as np
import cv2 as cv
import os

def evaluateFunc(datapath, resultpath):

    groundtruth = [img for img in sorted(os.listdir(datapath)) if img.endswith(".png")]
    result_image = [img for img in sorted(os.listdir(resultpath)) if img.endswith(".png")]
    assert len(groundtruth) == len(result_image), "Result should have the same number of samples as groundtruth"

    TP = 0   # True Positive
    TN = 0   # True Negative
    FP = 0   # False Positive
    FN = 0   # False Negative
    GT = 0   # Number of GroundTruth Pixels
    EST = 0  # Number of Estimated Pixels

    for index in range(len(groundtruth)):
        gt = cv.imread(os.path.join(datapath, groundtruth[index]))
        gt_gray = cv.cvtColor(gt, cv.COLOR_BGR2GRAY)
        gt_cvt = np.where(gt_gray == 255, 255, 0)
        gt_cvt_rev = np.where(gt_gray == 0, 255, 0)

        result_gray = cv.cvtColor(
            cv.imread(os.path.join(resultpath, result_image[index])), cv.COLOR_BGR2GRAY
        )
        ##### variable summation
        TP += np.sum(np.logical_and(gt_cvt, result_gray).astype(np.uint8))
        FP += np.sum(np.logical_and(gt_cvt_rev, result_gray).astype(np.uint8))
        GT += np.sum(gt_cvt) // 255

    # Return Recall & precision
    print("TP : ", TP)
    print("FP = ", FP)
    print("GT : ", GT)
    recall = ((100.0 * TP) / GT)
    precision = ((100.0 * TP) / (TP + FP))

    return recall, precision


if __name__ == '__main__':
    cal_result('./groundtruth', './result')

#
#   Author : Jin Heo
#   Contact : 993jin@gmail.com
#   Date : 27 03 2018
#
import cv2 as cv
import numpy as np

cv.namedWindow("all_window")
cam_video = cv.VideoCapture(0)

if cam_video.isOpened():
    is_run, frame = cam_video.read()
else:
    is_run = False

while is_run:
    gary_frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

    gaussian_img = cv.GaussianBlur(gary_frame, (3, 3), 0)
    canny_img = cv.Canny(gaussian_img, 100, 150)

    sobelKerX = np.array([[1, 0, -1], [2, 0, -2], [1, 0, -1]])
    sobelKerY = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]])
    img_sobelX = cv.filter2D(gaussian_img, -1, sobelKerX)
    img_sobelY = cv.filter2D(gaussian_img, -1, sobelKerY)
    img_sobel = img_sobelX + img_sobelY


    prewittKerX = np.array([[1, 0, -1],[1, 0, -1],[1, 0, -1]])
    prewittkerY = np.array([[1, 1, 1],[0, 0, 0],[-1, -1, -1]])
    img_prewittx = cv.filter2D(gaussian_img, -1, prewittKerX)
    img_prewitty = cv.filter2D(gaussian_img, -1, prewittkerY)
    prewitt_img = img_prewittx + img_prewitty

    cv.imshow("canny", canny_img)
    #cv.imshow("sobel", img_sobel)
    #cv.imshow("prewitt", prewitt_img)
    cv.imshow("img", frame)
    is_run, frame = cam_video.read()
    frame_flip = cv.flip(frame, 1)
    cv.imshow("flip_img", frame_flip)
    key = cv.waitKey(20)
    if key == 27:
        break

cv.destroyWindow("all_window")


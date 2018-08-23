# Object Detector with The Background Subtraction Algorithm

## What is the backgound subtractio algorithm?
[Reference]("https://en.wikipedia.org/wiki/Foreground_detection")

Breifly, it is the weighted sum of the previous frames' average and the current frame

u_i+1 = (1-a)u_i + af_i
where a : a weight factor of previous average u(i) and current frame f(i)

d_i = |f_i - u_i|, if d_i is over the threshold, the pixel value is 255(white). In opposite, the pixel value is 0(black)

## Performance
Precision and recall are calculated by comparison between groudn truths and output of my detector.

## How to use this program?
![fig1](./objectdetector.jpg)
virtualenv securityCam/venv --python=python3
. ./venv/bin/activate
pip install -r requirement.txt
python AutoTest.py

If you have anyquestion, email me "993jin@gmail.com"

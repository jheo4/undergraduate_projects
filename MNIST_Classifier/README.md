# MNIST Classifier with only 10K training data
## Model
In this project, the classifier is trained with only 10K MNIST data. To improve the classifier with small training data, additional 40K data are made by using given data. The model of the classifier is designed by considering the two dimensions of CNN; the filter dimension and the spatial dimension.

Randon rotation and shift are applied for augmenting the data. Therefore, the classifier is trained with 50K(given 10K + additional 40K) and the performance is improved as much as that of 55K training data.

## The Result
[Result logs](./result.log)

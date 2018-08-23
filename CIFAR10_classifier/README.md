# CIFAR10 Classifier less than 10 layers
## Models
### Model 1 : Vgg-10
This model is the upgraded version of Vgg-8. After applying input augmentation and hyperparameter optimization, the accuracy was 90.7%.

### Model 2 : CNN with 1x1 Convolution
In CNN, there are two dimension: spatial dimension and filter dimension. The initial spatial dimension of CIFAR10 is 32x32 and the filter dimension is varied by the programmer. In the VggNet, the spatial dimension is reduced by maxpooling layers and the filter dimension is increased by convolution layers. This model is conceived by the idea wheather the opposite way works well or not. Therefore, the spatial and filter dimensions are adjusted by 1x1 convolution. 

With this model, 8 convolution layers are used and 4 1x1 convolution layers are used for the adjustment of the dimensions. Accuracy of this model was 90.56%.

### Model 3 : ResNet-10
This model is the 10 layer ResNet. However, the performance is lower than Vgg-10. Since the characteristic of ResNet is that the deeper layers' problems are solved by Residual learning; vanishing gradient and the number of network parameters. 10 layers are not deep enough that the performance of ResNet is lower than VggNet.

## Methods for higher performance and optimization
1. Input augmentation
  - If the input space is too small, the overfitting problem easily occur. Therefore, input augmentation is used for complex models.
  - In this project, the augmentation is implemented by affine transforms(rotation, translation, scaling, shearing)
1. The number of network parameters
  - Each convolution layer increases the number of parameters by (NxNxInputDim+1)xOutputDim.
  - The more parameters are in the network, the larger amount of time for training is required.
1. Optimizer
  - There are many kinds of optimizer models such as Adam, SGD and Adadelta.
  - Given each problem to solve, the performance by an optimizer is different.


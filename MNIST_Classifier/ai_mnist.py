import tensorflow as tf
import numpy
from scipy import ndimage
from tensorflow.examples.tutorials.mnist import input_data

def compute_accuracy(v_x, v_y):
    global prediction
    y_pre = session.run(prediction, feed_dict={x: v_x})
    correct_prediction = tf.equal(tf.argmax(y_pre, 1), tf.argmax(v_y, 1))
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
    result = session.run(accuracy, feed_dict={x: v_x, y: v_y})
    return result


def expend_training_data(images, labels):
  expanded_images = []
  expanded_labels = []
  for x, y in zip(images, labels):
    expanded_images.append(x)
    expanded_labels.append(y)

    bg_value = numpy.median(x)
    image = numpy.reshape(x, (-1, 28))

    for i in range(4):
      angle = numpy.random.randint(-35, 35, 1)
      new_img = ndimage.rotate(image, angle, reshape=False, cval=bg_value)

      shift = numpy.random.randint(-2, 2, 2)
      new_img_ = ndimage.shift(new_img, shift, cval=bg_value)

      expanded_images.append(numpy.reshape(new_img_, 784))
      expanded_labels.append(y)

  expanded_train_total_data = numpy.concatenate((expanded_images,
                                                 expanded_labels), axis=1)
  numpy.random.shuffle(expanded_train_total_data)
  print("Input Augmentation End")
  return expanded_train_total_data


mnist = input_data.read_data_sets("MNIST_data/", one_hot=True,
                                  validation_size=50000)

x = tf.placeholder(tf.float32, [None, 784])
y = tf.placeholder(tf.float32, [None, 10])
keep_prob = tf.placeholder(tf.float32)
x_image = tf.reshape(x, [-1, 28, 28, 1])

# ********************** conv1 *********************************
conv1 = tf.layers.conv2d(inputs=x_image, filters=64, kernel_size=[3, 3],
                         padding='same')
b1 = tf.layers.batch_normalization(conv1)
b1 = tf.nn.relu(b1)

conv2 = tf.layers.conv2d(inputs=b1, filters=64, kernel_size=[3, 3],
                         padding='same')
b2 = tf.layers.batch_normalization(conv2)
b2 = tf.nn.relu(b2)

conv3 = tf.layers.conv2d(inputs=b2, filters=64, kernel_size=[3, 3],
                         padding='same')
b3 = tf.layers.batch_normalization(conv3)
b3 = tf.nn.relu(b3)


pool1 = tf.layers.max_pooling2d(inputs=b3, pool_size=[2, 2], strides=2)

# ********************** conv2 *********************************
conv4 = tf.layers.conv2d(inputs=pool1, filters=128, kernel_size=[3, 3],
                         padding='same')
b4 = tf.layers.batch_normalization(conv4)
b4 = tf.nn.relu(b4)

conv5 = tf.layers.conv2d(inputs=b4, filters=128, kernel_size=[3, 3],
                         padding='same')
b5 = tf.layers.batch_normalization(conv5)
b5 = tf.nn.relu(b5)

conv6 = tf.layers.conv2d(inputs=b5, filters=128, kernel_size=[3, 3],
                         padding='same')
b6 = tf.layers.batch_normalization(conv6)
b6 = tf.nn.relu(b6)

pool2 = tf.layers.max_pooling2d(inputs=b6, pool_size=[2, 2], strides=2)
# ********************* func1 layer *********************************
pool2_flat = tf.reshape(pool2, [-1, 7 * 7 * 128])
logits = tf.layers.dense(inputs=pool2_flat, units=10)
prediction = tf.nn.softmax(logits)

error = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels=y,
                                                               logits=logits))
train_step = tf.train.RMSPropOptimizer(learning_rate=0.0001, decay=0.9,
                                       momentum=0.9).minimize(error)

with tf.Session() as session:
  session.run(tf.global_variables_initializer())

  print("# of training dataset : ", len(mnist.train.labels))
  print("# of valiation dataset : ", len(mnist.validation.labels))
  print("# of testing dataset : ", len(mnist.train.labels))

  augmented_train_data = expend_training_data(mnist.train.images, mnist.train.labels)
  train_size = augmented_train_data.shape[0]
  train_data = augmented_train_data[:, :-10]
  train_labels = augmented_train_data[:, -10:]

  epochs = 100
  batch_size = 2000
  iterations = int(train_size/batch_size)

  best_trial = {'epoch' : 0, 'accuracy' : 0}

  for epoch in range(epochs):
    epoch_cost = 0
    epoch_accuracy = 0

    for iteration in range(iterations):
      offset = (iteration * batch_size) % train_size
      batch_x = train_data[offset:(offset + batch_size), :]
      batch_y = train_labels[offset:(offset + batch_size), :]
      _, cost = session.run([train_step, error],
                               feed_dict={x: batch_x, y: batch_y})
      epoch_cost += cost / iterations

    epoch_accuracy = compute_accuracy(mnist.test.images, mnist.test.labels)
    if best_trial['accuracy'] < epoch_accuracy:
      best_trial['epoch'] = epoch
      best_trial['accuracy'] = epoch_accuracy

    print((epoch+1), "Epoch Accuracy : %.3f" % epoch_accuracy)
    if((epoch+1) % 5 == 0):
      print((epoch + 1), "Epoch Cost : %.8f" % epoch_cost)

  print("Best Trial(epoch : ", best_trial['epoch'],
        " accuracy %.3f" % best_trial['accuracy'])

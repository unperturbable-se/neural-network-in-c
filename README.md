# training:
### implement an image processing pipeline in c.
### implement a custom neural network in c via a multilayered perceptron(input+2 hidden layers+output(10 nodes--1 for each number))
### recognize which number each image maps to.
### features:784 hidden layer:16 output layer:10

# preprocessing:
### use a python library to reduce the quality of each image to 32*32  (image size is already 28*28 so i guess this step not required)
### load the low res images in c using stb_image, and normalize them to grayscale(divided by 255)

# dataset:
### mnist_png (https://www.kaggle.com/datasets/alexanderyyy/mnist-png?resource=download)
### it is the old kaggle mnist dataset but as image form to make the project more realistic
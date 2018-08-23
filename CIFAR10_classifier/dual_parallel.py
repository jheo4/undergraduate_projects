import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.autograd import Variable
from torchvision import datasets, transforms
import time
import os
import torch.backends.cudnn as cudnn

os.environ["CUDA_VISIBLE_DEVICES"] = '0'
start_time = time.time()
batch_size = 256
learning_rate = 0.1

transform_train = transforms.Compose([
    transforms.RandomCrop(32, padding=4),
    transforms.RandomHorizontalFlip(),
    transforms.ToTensor(),
    transforms.Normalize(mean=(0.4914, 0.4824, 0.4467),
                             std=(0.2471, 0.2436, 0.2616))
])

transform_test = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize(mean=(0.4914, 0.4824, 0.4467),
                             std=(0.2471, 0.2436, 0.2616))
])


train_dataset = datasets.CIFAR10(root='/content/pytorch/data/cifar10/',
                                 train=True,
                                 transform=transform_train,
                                 download=True)

test_dataset = datasets.CIFAR10(root='/content/pytorch/data/cifar10/',
                                train=False,
                                transform=transform_test)

train_loader = torch.utils.data.DataLoader(dataset=train_dataset,
                                           batch_size=batch_size,
                                           shuffle=True,
                                           num_workers=4)

test_loader = torch.utils.data.DataLoader(dataset=test_dataset,
                                          batch_size=batch_size,
                                          shuffle=False,
                                          num_workers=4)
accuracies = []


def train(epoch):
    model.train()
    train_loss = 0
    total = 0
    correct = 0
    for batch_idx, (data, target) in enumerate(train_loader):
        if torch.cuda.is_available():
            data, target = Variable(data.cuda()), Variable(target.cuda())
        else:
            data, target = Variable(data), Variable(target)

        optimizer.zero_grad()
        output = model(data)
        loss = criterion(output, target)
        loss.backward()
        optimizer.step()

        train_loss += loss.data[0]
        _, predicted = torch.max(output.data, 1)
        # torch.max() : (maximum value, index of maximum value) return.
        # 1 :  row마다 max계산 (즉, row는 10개의 class를 의미)
        # 0 : column마다 max 계산
        total += target.size(0)
        correct += predicted.eq(target.data).cpu().sum()
    print('Epoch: {} | Batch_idx: {} |  Loss: ({:.4f}) | Acc: ({:.2f}%) ({}/{})'
            .format(epoch, batch_idx, train_loss/(batch_idx+1), 100.*correct/total, correct, total))


def test():
    model.eval()
    test_loss = 0
    correct = 0
    total = 0
    for batch_idx, (data, target) in enumerate(test_loader):
        if torch.cuda.is_available():
            data, target = Variable(data.cuda()), Variable(target.cuda())
        else:
            data, target = Variable(data), Variable(target)

        outputs = model(data)
        loss = criterion(outputs, target)

        test_loss += loss.data[0]
        _, predicted = torch.max(outputs.data, 1)
        total += target.size(0)
        correct += predicted.eq(target.data).cpu().sum()
    print('# TEST : Loss: ({:.4f}) | Acc: ({:.2f}%) ({}/{})'
      .format(test_loss/(batch_idx+1), 100.*correct/total, correct, total))
    test_accuracy = 100.*correct/total

    highest_accuracy = 0
    for index in range(len(accuracies)):
        if highest_accuracy < accuracies[index]['accuracy']:
            highest_accuracy = accuracies[index]['accuracy']

    if highest_accuracy < test_accuracy:
        accuracies.append({'epoch' : epoch, 'accuracy' : test_accuracy})


class Parallel_Layer3(nn.Module):
    def __init__(self, in_dim, reduced_dim1, kernel_dim1, reduced_dim2,
                 kernel_dim2, reduced_dim3, kernel_dim3, in_stride=1):
        super(Parallel_Layer3, self).__init__()
        # 1x1 conv branch
        self.block1 = nn.Sequential(
            nn.Conv2d(in_dim, reduced_dim1, kernel_size=1),
            nn.BatchNorm2d(reduced_dim1),
            nn.ReLU(True),
            nn.Conv2d(reduced_dim1, kernel_dim1, kernel_size=1,
                      stride=in_stride),
            nn.BatchNorm2d(kernel_dim1),
            nn.ReLU(True),
        )

        self.block2 = nn.Sequential(
            nn.Conv2d(in_dim, reduced_dim2, kernel_size=1),
            nn.BatchNorm2d(reduced_dim2),
            nn.ReLU(True),
            nn.Conv2d(reduced_dim2, kernel_dim2, kernel_size=1,
                      stride=in_stride),
            nn.BatchNorm2d(kernel_dim2),
            nn.ReLU(True),
        )

        self.block3 = nn.Sequential(
            nn.Conv2d(in_dim, reduced_dim3, kernel_size=1),
            nn.BatchNorm2d(reduced_dim3),
            nn.ReLU(True),
            nn.Conv2d(reduced_dim3, kernel_dim3, kernel_size=1,
                      stride=in_stride),
            nn.BatchNorm2d(kernel_dim3),
            nn.ReLU(True),
        )

    def forward(self, x):
        block1_result = self.block1(x)
        block2_result = self.block2(x)
        block3_result = self.block2(x)
        return torch.cat([block1_result, block2_result, block3_result,], 1)


class Parallel_Layer2(nn.Module):
    def __init__(self, in_dim, reduced_dim1, kernel_dim1, reduced_dim2,
                 kernel_dim2, in_stride=1):
        super(Parallel_Layer2, self).__init__()
        # 1x1 conv branch
        self.block1 = nn.Sequential(
            nn.Conv2d(in_dim, reduced_dim1, kernel_size=1),
            nn.BatchNorm2d(reduced_dim1),
            nn.ReLU(True),
            nn.Conv2d(reduced_dim1, kernel_dim1, kernel_size=1,
                      stride=in_stride),
            nn.BatchNorm2d(kernel_dim1),
            nn.ReLU(True),
        )

        self.block2 = nn.Sequential(
            nn.Conv2d(in_dim, reduced_dim2, kernel_size=1),
            nn.BatchNorm2d(reduced_dim2),
            nn.ReLU(True),
            nn.Conv2d(reduced_dim2, kernel_dim2, kernel_size=1,
                      stride=in_stride),
            nn.BatchNorm2d(kernel_dim2),
            nn.ReLU(True),
        )

    def forward(self, x):
        block1_result = self.block1(x)
        block2_result = self.block2(x)
        return torch.cat([block1_result, block2_result], 1)


class ParallelNet(nn.Module):
    def __init__(self):
        super(ParallelNet, self).__init__()
        pre_dim = 64
        self.conv_pre = nn.Sequential(
            nn.Conv2d(3, pre_dim, kernel_size=3, padding=1),
            nn.BatchNorm2d(pre_dim),
            nn.ReLU(True),
        )
        #self.layer1 = Parallel_Layer3(pre_dim, 16, 16, 32, 32, 64, 64)
        #self.layer2 = ParaParallel_Layer3llel_Layer(112, 32, 32, 64, 64, 128, 128)
        #self.layer3 = Parallel_Layer3(224, 64, 64, 128, 128, 256, 256)
        self.layer1 = Parallel_Layer2(pre_dim, 16, 16, 32, 32)
        self.layer2 = Parallel_Layer2(48, 32, 32, 64, 64, in_stride=2)
        self.layer3 = Parallel_Layer2(96, 64, 64, 128, 128)
        self.layer4 = Parallel_Layer2(192, 128, 128, 256, 256, in_stride=2)

        self.avgpool = nn.AvgPool2d(4)
        self.linear = nn.Linear(1536, 10)

    def forward(self, x):
        x = self.conv_pre(x)
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)
        x = self.avgpool(x)
        x = x.view(x.size(0), -1)
        x = self.linear(x)
        return x

model = ParallelNet()
optimizer = optim.SGD(model.parameters(), lr=learning_rate, momentum=0.9, weight_decay=1e-5)
criterion = nn.CrossEntropyLoss().cuda()

if torch.cuda.device_count() > 0:
    print("USE", torch.cuda.device_count(), "GPUs!")
    model = nn.DataParallel(model)
    cudnn.benchmark = True
else:
    print("USE ONLY CPU!")

if torch.cuda.is_available():
    model.cuda()

for epoch in range(0, 190):
    if epoch < 10:
        learning_rate = learning_rate * 30
    elif epoch < 20:
        learning_rate = learning_rate * 20
    elif epoch < 30:
        learning_rate = learning_rate * 10
    elif epoch < 40:
        learning_rate = learning_rate * 5
    elif epoch < 60:
        learning_rate = learning_rate
    elif epoch < 120:
        learning_rate = learning_rate * 0.1
    elif epoch < 160:
        learning_rate = learning_rate * 0.01
    else:
        learning_rate = learning_rate * 0.001
    for param_group in optimizer.param_groups:
        param_group['learning_rate'] = learning_rate

    train(epoch)
    test()
highest_accuracy = 0
epoch = 0
for index in range(len(accuracies)):
    if highest_accuracy < accuracies[index]['accuracy']:
        highest_accuracy = accuracies[index]['accuracy']
        epoch = accuracies[index]['epoch']
print("###### Best accuracy #####")
print("epoch : ", epoch, "  highest accuracy : ", highest_accuracy)
print("##########################")
now = time.gmtime(time.time() - start_time)
print('{} hours {} mins {} secs for training'.format(now.tm_hour, now.tm_min, now.tm_sec))


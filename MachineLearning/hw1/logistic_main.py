import numpy as np
from utils import _initialize, optimizer

np.random.seed(428)

# ========================= EDIT HERE ========================
# 1. Choose DATA : Titanic / Digit
# 2. Adjust Hyperparameters
# 3. Choose Optimizer : SGD / Momentum / RMSProp

# DATA
#DATA_NAME = 'Digit'
DATA_NAME = 'Titanic'

# HYPERPARAMETERS
batch_size = 10
num_epochs = 3000
learning_rate = 0.0007
epsilon = 0.01
gamma = 0.9

# OPTIMIZER
OPTIMIZER = 'SGD'
# ============================================================

assert DATA_NAME in ['Titanic', 'Digit']
assert OPTIMIZER in ['SGD', 'Momentum', 'RMSProp']

# Load dataset, model and evaluation metric
train_data, test_data, logistic_regression, metric = _initialize(DATA_NAME)

train_x, train_y = train_data

num_data, num_features = train_x.shape
print('# of Training data : ', num_data)

# Make model & optimizer
model = logistic_regression(num_features)
optim = optimizer(OPTIMIZER, gamma=gamma, epsilon=epsilon)

# TRAIN
loss = model.train(train_x, train_y, num_epochs, batch_size, learning_rate, optim)
print('Training Loss at last epoch: %.2f' % loss)

# EVALUATION
test_x, test_y = test_data
pred = model.eval(test_x)

pred = pred.reshape((len(pred), 1))
ACC = metric(pred, test_y)

print(OPTIMIZER, ' ACC on Test Data : %.3f' % ACC)
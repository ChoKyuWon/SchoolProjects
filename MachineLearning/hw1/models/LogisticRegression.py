import numpy as np

class LogisticRegression:
    def __init__(self, num_features):
        self.num_features = num_features
        self.W = np.zeros((self.num_features, 1))

    def train(self, x, y, epochs, batch_size, lr, optim):
        final_loss = None   # loss of final epoch

        # Train should be done for 'epochs' times with minibatch size of 'batch size'
        # The function 'train' should return the loss of final epoch
        # Loss of an epoch is calculated as an average of minibatch losses

        # ========================= EDIT HERE ========================
        final_loss=0
        num_data=len(y)
        k=0
       
        def dlossF(k, j):
            s=0
            size = batch_size
            for Xi, Yi in zip(x[k:k+batch_size], y[k:k+batch_size]):
                s = s + (self._sigmoid(Xi)-Yi)*Xi[j]
            if (num_data - k) < batch_size:
                size = num_data - k
            return s/size
        
        for iterative in range(0, epochs):
            k = k + batch_size
            if k == num_data:
                k = batch_size
            grad = np.zeros((self.num_features, 1))
            for j in range(0, self.num_features):
                grad[j] = dlossF(k, j)
            self.W =  optim.update(self.W, grad, lr)




        # ============================================================
        return final_loss

    def eval(self, x):
        threshold = 0.5
        pred = None

        # Evaluation Function
        # Given the input 'x', the function should return prediction for 'x'
        # The model predicts the label as 1 if the probability is greater or equal to 'threshold'
        # Otherwise, it predicts as 0

        # ========================= EDIT HERE ========================
        ylist=[]
        #print(self.W)
        for xline in x:
            y = self._sigmoid(xline)
            if y > threshold:
                ylist.append(1)
            else:
                ylist.append(0)
        pred = np.array(ylist)




        # ============================================================

        return pred

    def _sigmoid(self, x):
        sigmoid = None

        # Sigmoid Function
        # The function returns the sigmoid of 'x'

        # ========================= EDIT HERE ========================
        fx=np.transpose(x).dot(self.W)
        sigmoid = 1/ (1 + np.exp(-fx))
        #print(sigmoid)




        # ============================================================
        return sigmoid
